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

