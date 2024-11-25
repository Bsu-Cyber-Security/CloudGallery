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

