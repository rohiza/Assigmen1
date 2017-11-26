#include "../include/FileSystem.h"
#include "../include/Commands.h"
#include <iostream>
using namespace std;

FileSystem::FileSystem() {
    if (verbose == 1 | verbose == 3) {
        cout << "FileSystem ::FileSystem()" << endl;
    }
    rootDirectory = new Directory("", nullptr);
    workingDirectory = rootDirectory;
}

FileSystem::FileSystem(const FileSystem &other) {
    if (verbose == 1 | verbose == 3) {
        cout << "FileSystem ::FileSystem(const FileSystem& other)" << endl;
    }
    copy(&other.getRootDirectory());
}

void FileSystem::copy(Directory *otherRootDirectory) {
    rootDirectory = new Directory(*otherRootDirectory);
    workingDirectory = rootDirectory ;
}

FileSystem::~FileSystem() {
    if (verbose == 1 | verbose == 3) {
        cout << "FileSystem ::~FileSystem()" << endl;
    }
    if (rootDirectory != nullptr) {
        rootDirectory->clear();
        delete rootDirectory;
        rootDirectory = nullptr;
    }
    workingDirectory = nullptr;
}

FileSystem::FileSystem(FileSystem &&other) {
    if (verbose == 1 | verbose == 3)
        cout << "FileSystem ::FileSystem(FileSystem &&other)" << endl;
    rootDirectory = other.rootDirectory;
    workingDirectory = other.workingDirectory;
    other.rootDirectory = nullptr;
    other.workingDirectory = nullptr;
}

FileSystem &FileSystem::operator=(const FileSystem &other) {
    if (verbose == 1 | verbose == 3)
        cout << "FileSystem& FileSystem ::operator=(const FileSystem &other)" << endl;
    if (this != &other) {
        copy(&other.getRootDirectory());
    }
    return *this;
}

FileSystem &FileSystem::operator=(FileSystem &&other) {
    if (verbose == 1 | verbose == 3)
        cout << "FileSystem& FileSystem ::operator=(FileSystem &&other)" << endl;
    if (this != &other){
        rootDirectory = other.rootDirectory;
        workingDirectory = other.workingDirectory;
        other.rootDirectory = nullptr;
        other.workingDirectory = nullptr;
    }
    return *this;
}

Directory &FileSystem::getRootDirectory() const { return *rootDirectory; }

Directory &FileSystem::getWorkingDirectory() const { return *workingDirectory; }

void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory) {
    this->workingDirectory = newWorkingDirectory;
}