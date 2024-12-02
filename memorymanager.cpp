// MemoryManager.cpp
#include "MemoryManager.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QByteArray>
#include <stdexcept>

MemoryManager::MemoryManager(size_t cacheCapacity, const QString& storageDir)
    : cacheManager(cacheCapacity), storageDirectory(storageDir) {
    initializeStorageDirectory();
    fileSystem = new FileSystem();
}

MemoryManager::~MemoryManager() {
    delete fileSystem;
}

void MemoryManager::initializeStorageDirectory() {
    QDir dir(storageDirectory);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            throw std::runtime_error("Не удалось создать директорию для хранения страниц: " + storageDirectory.toStdString());
        }
    }
}

uint32_t MemoryManager::getVirtualPageForImage(const QString& filePath) const {
     return std::hash<QString>{}(filePath) % 100000;
}

bool MemoryManager::readPageFromDisk(uint32_t virtualPage, Page* page) {
    QString fileName = QString("page_%1.dat").arg(virtualPage);
    QString filePath = storageDirectory + "/" + fileName;

    QFile file(filePath);
    if (!file.exists()) {
        qWarning() << "Файл страницы не существует:" << filePath;
        return false;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл для чтения:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    // Проверка на отрицательный размер (если qsizetype является знаковым типом)
    if (data.size() < 0) {
        qWarning() << "Негативный размер данных:" << data.size();
        return false;
    }

    // Приведение типов к size_t
    size_t bytesToCopy = std::min(static_cast<size_t>(data.size()), page->getSize());

    std::memcpy(page->getData(), data.constData(), bytesToCopy);

    return true;
}

bool MemoryManager::writePageToDisk(uint32_t virtualPage, Page* page) {
    QString fileName = QString("page_%1.dat").arg(virtualPage);
    QString filePath = storageDirectory + "/" + fileName;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось открыть файл для записи:" << filePath;
        return false;
    }

    QByteArray data(reinterpret_cast<const char*>(page->getData()), page->getSize());
    qint64 bytesWritten = file.write(data);
    file.close();

    if (bytesWritten != static_cast<qint64>(page->getSize())) {
        qWarning() << "Не удалось записать все данные в файл:" << filePath;
        return false;
    }

    return true;
}

Page* MemoryManager::loadPage(uint32_t virtualPage, uint32_t segmentId) {
    // Проверка кэша
    Page* page = cacheManager.getPage(virtualPage);
    if (page != nullptr) {
        return page;
    }

    // Проверка таблицы страниц
    page = pageTable.getPhysicalPage(virtualPage);
    if (page != nullptr) {
        cacheManager.putPage(virtualPage, page);
        return page;
    }

    // Если страница не найдена, загрузить её из хранилища
    page = new Page(virtualPage);
    bool loaded = readPageFromDisk(virtualPage, page);
    if (!loaded) {
        // Если страница отсутствует в хранилище, инициализировать её нулями
        std::memset(page->getData(), 0, page->getSize());
        qDebug() << "Создана новая страница с номером:" << virtualPage;
    }

    pageTable.mapPage(virtualPage, page);
    cacheManager.putPage(virtualPage, page);

    // Добавить страницу в сегмент
    segmentManager.createSegment(segmentId);
    Segment* segment = segmentManager.getSegment(segmentId);
    if (segment) {
        segment->addPage(virtualPage);
    }

    return page;
}

void MemoryManager::unloadPage(uint32_t virtualPage, uint32_t segmentId) {
    // Получить страницу из таблицы страниц
    Page* page = pageTable.getPhysicalPage(virtualPage);
    if (page == nullptr) {
        qWarning() << "Попытка выгрузить несуществующую страницу:" << virtualPage;
        return;
    }

    // Сохранить страницу обратно в хранилище
    bool saved = writePageToDisk(virtualPage, page);
    if (!saved) {
        qWarning() << "Не удалось сохранить страницу на диск:" << virtualPage;
    }

    // Удалить страницу из таблицы страниц и кэша
    pageTable.unmapPage(virtualPage);
    // CacheManager автоматически удаляет страницу из кэша при удалении из PageTable

    // Удалить страницу из сегмента
    Segment* segment = segmentManager.getSegment(segmentId);
    if (segment) {
        segment->removePage(virtualPage);
    }

    // Освобождение памяти страницы
    delete page;
}

QImage MemoryManager::loadImage(const QString& filePath, const QString& directoryPath) {
    uint32_t virtualPageNumber = getVirtualPageForImage(filePath);
    Page* page = loadPage(virtualPageNumber, /*segmentId=*/1);

    if (page) {
        // Чтение данных изображения из страницы
        readPageFromDisk(virtualPageNumber, page); // Загружаем данные из файла

        // Преобразование данных страницы в QImage
        QImage image(reinterpret_cast<const uchar*>(page->getData()), 512, 512, QImage::Format_RGB32);
        if (!image.isNull()) {
            // Добавить файл в файловую систему
            QString fileName = QFileInfo(filePath).fileName();
            fileSystem->createFile(directoryPath, fileName, image);

            return image;
        } else {
            qWarning() << "Не удалось создать QImage из данных страницы.";
        }
    }

    return QImage(); // Возвращает пустое изображение в случае ошибки
}

void MemoryManager::unloadImage(const QString& filePath, const QString& directoryPath) {
    uint32_t virtualPageNumber = getVirtualPageForImage(filePath);
    unloadPage(virtualPageNumber, /*segmentId=*/1); // Можно использовать отдельный сегмент для изображений

    // Удалить файл из файловой системы
    QString fileName = QFileInfo(filePath).fileName();
    fileSystem->deleteFile(directoryPath, fileName);
}

FileSystem* MemoryManager::getFileSystem() const {
    return fileSystem;
}
