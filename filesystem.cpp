#include "filesystem.h"

FileSystem::FileSystem() {
    root = new Directory("root");
}

FileSystem::~FileSystem(){
    delete root;
}

Directory* FileSystem::getRootDirectory() const{
    return root;
}

Directory* FileSystem::getDirectory(const QString& path) const {
    if(path.isEmpty()){
        return root;
    }

    return traversePath(path);
}


Directory* FileSystem::createDirectory(const QString& path){
    QStringList parts = path.split('/', Qt::SkipEmptyParts);
    Directory* current = root;

    for(const QString part : parts){
        Directory* next = current->getSubDirectory(part);

        if(!next){
            next = new Directory(part, current);
            current->addSubDirectory(next);
        }

        current = next;
    }

    return current;
}

Directory* FileSystem::traversePath(const QString& path) const{
    QStringList parts = path.split('/', Qt::SkipEmptyParts);
    Directory* current = root;

    for(const QString& part : parts){
        current = current->getSubDirectory(part);
        if(!current){
            return nullptr;
        }
    }

    return current;
}

File* FileSystem::createFile(const QString& path, const QString& file_name, const QImage& image){
    Directory* dir = getDirectory(path);
    if(dir){
        File* file = new File(file_name, dir->getPath() + "/" + file_name);
        file->setImage(image);
        dir->addFile(file);
        return file;
    }
    return nullptr;
}

File* FileSystem::getFile(const QString& path, const QString& file_name) const{
    Directory* dir = getDirectory(path);
    if(dir){
        return dir->getFile(file_name);
    }
    return nullptr;
}

bool FileSystem::deleteDirectory(const QString& path){
    if(path.isEmpty()){
        return false;
    }

    QStringList parts = path.split('/', Qt::SkipEmptyParts);
    QString dir_name = parts.last();
    parts.removeLast();

    Directory* parent = traversePath(parts.join('/'));
    if(parent){
        Directory* dir = parent->getSubDirectory(dir_name);
        if(dir){
            parent->removeSubDirectory(dir_name);
            return true;
        }
    }
    return false;
}

bool FileSystem::deleteFile(const QString& path, const QString& file_name){
    Directory* dir = getDirectory(path);
    if(dir){
        dir->removeFile(file_name);
        return true;
    }

    return false;
}
