#include "File.h"

File::File(const QString& name, const QString& path)
    : name(name), path(path), image() {}

File::~File() {}

QString File::getName() const {
    return name;
}

QString File::getPath() const {
    return path;
}

QImage File::getImage() const {
    return image;
}

void File::setImage(const QImage& img) {
    image = img;
}
