#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Files.h"


class FileSystem {
private:
    Directory* rootDirectory;
    Directory* workingDirectory;
public:
    FileSystem();
    FileSystem(const FileSystem & other);
    FileSystem(FileSystem &&other);
    virtual ~FileSystem();
    FileSystem &operator = (const FileSystem & other);
    FileSystem &operator  =(FileSystem &&other);
    Directory& getRootDirectory() const; // Return reference to the root directory
    Directory& getWorkingDirectory() const; // Return reference to the working directory
    void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system
    void copy(Directory* rootDirectory);


};


#endif