// Directory.cpp
#include "Directory.h"

Directory::Directory(const QString& name, Directory* parent)
    : name(name), parent(parent) {}

Directory::~Directory() {
    qDeleteAll(subDirectories);
    qDeleteAll(files);
}

QString Directory::getName() const {
    return name;
}

QString Directory::getPath() const {
    if (parent) {
        return parent->getPath() + "/" + name;
    }
    return name; // Корневая директория
}

void Directory::addSubDirectory(Directory* dir) {
    subDirectories.append(dir);
}

void Directory::removeSubDirectory(const QString& name) {
    for(int i = 0; i < subDirectories.size(); ++i) {
        if(subDirectories[i]->getName() == name) {
            delete subDirectories[i];
            subDirectories.removeAt(i);
            break;
        }
    }
}

Directory* Directory::getSubDirectory(const QString& name) const {
    for(auto dir : subDirectories) {
        if(dir->getName() == name) {
            return dir;
        }
    }
    return nullptr;
}

void Directory::addFile(File* file) {
    files.append(file);
}

void Directory::removeFile(const QString& fileName) {
    for(int i = 0; i < files.size(); ++i) {
        if(files[i]->getName() == fileName) {
            delete files[i];
            files.removeAt(i);
            break;
        }
    }
}

File* Directory::getFile(const QString& fileName) const {
    for(auto file : files) {
        if(file->getName() == fileName) {
            return file;
        }
    }
    return nullptr;
}

QVector<Directory*> Directory::getSubDirectories() const {
    return subDirectories;
}

QVector<File*> Directory::getFiles() const {
    return files;
}
