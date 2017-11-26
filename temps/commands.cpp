#include <iostream>
#include <fstream>
#include "../include/Commands.h"

using namespace std;

BaseCommand::BaseCommand(string args) : args(args) {}

string BaseCommand::getArgs() {
    return args;
}

vector<string> BaseCommand::splitString(string args) {
    vector<string> check;
    string delim = "/";
    auto start = 0U;
    auto end = args.find(delim);
    while (end != string::npos) {
        check.push_back(args.substr(start, end - start));
        start = end + delim.length();
        end = args.find(delim, start);
    }
    check.push_back(args.substr(start, end));

    return check;
}

Directory* BaseCommand:: getLastDir(string args, FileSystem &fs) {
    Directory *start; //check if works
    if (args[0] == '/') { //absolute path or relative
        start = &fs.getRootDirectory();
    }
    else {
        start = &fs.getWorkingDirectory();
    }
    vector<string> fileNames = splitString(args); //splits the path to seperate words
    int i;
    for (size_t  j = 0; j <fileNames.size(); j++) {
        if (fileNames[j] == "..") {
            if(start->getParent() != nullptr)
            start = start->getParent();
            else
                break;
        }
        else {
            //go over all (start->getChildren()), check if directory and name = fileName
            for (i = 0; i < start->getChildren().size(); i++) {
                if (start->getName() == fileNames[j]){
                    break;
                }
                if (typeid(*(start->getChildren()[i])) == typeid(Directory) &&
                    start->getChildren()[i]->getName() == fileNames[j]) {
                    start = dynamic_cast<Directory *>(start->getChildren()[i]);
                    break;
                }
                if (j == fileNames.size()-1) {
                    if (i == start->getChildren().size() - 1 && start->getChildren()[i]->getName() != fileNames[j]) {
                        return start;
                    }
                }
            }
        }
    }
    return start;
}

bool BaseCommand::isFileExists(string args, FileSystem &fs) {

    Directory *start; //check if works
    if (args[0] == '/') { //absolute path or relative
        start = &fs.getRootDirectory();
    } else {
        start = &fs.getWorkingDirectory();
    }
    vector<string> fileNames = splitString(getArgs()); //splits the path to seperate words
    for (size_t j = 0; j < fileNames.size(); j++) {
        for (int i = 0; i < (start->getChildren().size()); i++) {
            if (j == fileNames.size()-1){
                if (!(start->getChildren()[i]->DirOrFile()) && (start->getChildren()[i]->getName() == fileNames[j])) {
                    return true;
                }
                else if(start->getChildren()[i]->DirOrFile() && (start->getChildren()[i]->getName() == fileNames[j])){
                    return true;
                }
            }
            else if (start->getChildren()[i]->DirOrFile() && (start->getChildren()[i]->getName() == fileNames[j])){
                start = dynamic_cast<Directory *>((start->getChildren())[i]);
                break;
            }
            if (i >= (start->getChildren()).size()) { //gone through all (start->getChildren()) and didnt find next directory
                return false;
            }
        }
        return false;
    }
    return true;

}

PwdCommand::PwdCommand(string args) : BaseCommand(args) {}
void PwdCommand::execute(FileSystem &fs) {
    cout << fs.getWorkingDirectory().getAbsolutePath() << endl;
}

string PwdCommand::toString() {
    return "pwd";
}

CdCommand::CdCommand(string args) : BaseCommand(args) {}

void CdCommand::execute(FileSystem &fs) {
    if (!isFileExists(getArgs(), fs)) {
        cout << "The System cannot find the path specified" << endl;
    }
    else {
        Directory *start = getLastDir(getArgs(), fs);
        fs.setWorkingDirectory(start);
    }
}

string CdCommand::toString() {
    return "cd";
}

LsCommand::LsCommand(string args) : BaseCommand(args) {

}

void LsCommand::execute(FileSystem &fs) {
    if(getArgs() == ""){
        cout << fs.getWorkingDirectory().getName() + "\t" << endl;
        fs.getWorkingDirectory().printChildren();
    }
    else{
        if (!isFileExists(getArgs(), fs)) {
            cout << "The System cannot find the path specified" << endl;
        } else {
            if (getArgs().find("-s")) {
                fs.getWorkingDirectory().sortBySize();
            } else {
                fs.getWorkingDirectory().sortByName();
            }
            cout << fs.getWorkingDirectory().getName() + "\t" << endl;
            fs.getWorkingDirectory().printChildren();
        }
    }
    }

string LsCommand::toString() {
    return "ls";
}

MkdirCommand::MkdirCommand(string args) : BaseCommand(args) {

}

void MkdirCommand::execute(FileSystem &fs) {
    if (isFileExists(getArgs(), fs)) {
        cout << "The directory already exists" << endl;
    }
    else{
        Directory *start = getLastDir(getArgs(), fs);
        vector<string> fileNames = splitString(getArgs());
        for (size_t j = 0; j<fileNames.size(); j++) {
            if (start->getName() == fileNames[j] || (start->getParent() != nullptr && start->getParent()->getName() == fileNames[j])){
                continue;
            }
            start->addFile(new Directory(fileNames[j], start));
            if(j != fileNames.size() -1) {
                start = dynamic_cast<Directory *>(start->getChildren()[start->getChildren().size() - 1]);
            }
        }
    }
}

string MkdirCommand::toString() {
    return "mkdir";
}

MkfileCommand::MkfileCommand(string args) : BaseCommand(args) {
}

void MkfileCommand::execute(FileSystem &fs) {
    string path = getArgs().substr(0,getArgs().find_last_of('/'));
    string newFile = getArgs().substr(getArgs().find_last_of('/')+1);
    string file = "";
    string sizeString;
    size_t k = 0;
    while (newFile.at(k) != ' ') {  //seperates the last string to the name and size of the newfile
        file = file + newFile.at(k);
        k++;
    }
    sizeString = newFile.substr((k + 1));
    int size = atoi(sizeString.c_str());
    vector<string> filePath = splitString(path);
    if(filePath.size() == 1) {
        Directory *start = getLastDir(path, fs);
        File newOne = File(file, size);
        start->addFile(dynamic_cast<BaseFile *>(&newOne));
        return;
    }
    if (!isFileExists(path, fs)) {
        cout << "The System cannot find the path specified" << endl;
    }
    else if (isFileExists(path, fs)) {
        cout << "File already exists" << endl;
    }
    else {
        Directory *start = getLastDir(path, fs);
        File newOne = File(file, size);
        start->addFile(dynamic_cast<BaseFile*>(&newOne));
    }
}

string MkfileCommand::toString() {
    return "mkfile";
}

CpCommand::CpCommand(string args) : BaseCommand(args) {

}

void CpCommand::execute(FileSystem &fs) {
    string sourcePath = getArgs().substr(0, getArgs().find_last_of(' '));
    string destinationPath = getArgs().substr(getArgs().find_first_of(' ') + 1);
    if (!isFileExists(sourcePath, fs)) {
        cout << "No such file or directory" << endl;
    }
    if (!isFileExists(destinationPath, fs)) {
        cout << "No such file or directory" << endl;
    } else {
        Directory *start = getLastDir(sourcePath, fs);
        Directory *start2 = getLastDir(destinationPath, fs);
        vector<string> fileNames = splitString(sourcePath);
        if (fileNames[fileNames.size() - 1] == start->getName()) { //the last file is a directory
            start2->addFile(new Directory(*start));
        }
        else {
            vector<BaseFile *> children;
            children = start->getChildren();
            //go over all (start->getChildren()), check if directory and name = fileName
            for (int i = 0; i < children.size(); i++) {
                if (children[i]->getName() == fileNames[fileNames.size()-1]) {
                    start2->addFile(children[i]);
                }
            }
        }
    }
}

string CpCommand::toString() {
    return "cp";
}

MvCommand::MvCommand(string args) : BaseCommand(args) {

}

void MvCommand::execute(FileSystem &fs) {
    string sourcePath = getArgs().substr(0, getArgs().find_last_of(' '));
    string destinationPath = getArgs().substr(getArgs().find_first_of(' ') + 1);
    if (!isFileExists(sourcePath, fs)) {
        cout << "No such file or directory" << endl;
    }
    if (!isFileExists(destinationPath, fs)) {
        cout << "No such file or directory" << endl;
    }
    else {
        Directory *start = getLastDir(sourcePath, fs);
        Directory *start2 = getLastDir(destinationPath, fs);
        vector<string> fileNames = splitString(sourcePath);
        if (fileNames[fileNames.size() - 1] == start->getName()) { //the last file is a directory
            if (start == &fs.getWorkingDirectory() || start == &fs.getRootDirectory()
                || start == (fs.getWorkingDirectory().getParent())) {
                cout << "Can't move directory" << endl;
            } else {
                start2->addFile(new Directory(*start));
                start->getParent()->removeFile(start->getName());
            }
        }
        else {
            vector<BaseFile *> children;
            children = start->getChildren();
            //go over all (start->getChildren()), check if directory and name = fileName
            for (int i = 0; i < children.size(); i++) {
                if (children[i]->getName() == fileNames[fileNames.size() - 1]) {
                    start2->addFile(children[i]);
                    start->removeFile(children[i]->getName());
                }
            }
        }
    }
}


string MvCommand::toString() {
    return "mv";
}

RenameCommand::RenameCommand(string args) : BaseCommand(args) {

}

void RenameCommand::execute(FileSystem &fs) {
    string oldNamePos = getArgs().substr(0, getArgs().find_last_of(' '));
    string newName = getArgs().substr(getArgs().find_last_of(' ')+1);
    string sameName = oldNamePos.substr(0, getArgs().find_last_of('/') + 1) + newName;
    string oldName = oldNamePos.substr(oldNamePos.find_last_of('/')+1);
    if (!isFileExists(oldNamePos, fs)) {
        cout << "No such file or directory" << endl;
    }
    else {
        Directory *start = getLastDir(oldNamePos, fs);
        vector<BaseFile *> children;
        children = start->getChildren();
        for (int i = 0; i < children.size(); i++) {
            if(children[i]->getName() == oldName){
                if(children[i] == &fs.getWorkingDirectory()) {
                    cout << "Can't rename the working directory" << endl;
                }
                else if(!isFileExists(sameName, fs)) {
                    children[i]->setName(newName);
                }
            }
        }
    }
}

string RenameCommand::toString() {
    return "rename";
}

RmCommand::RmCommand(string args) : BaseCommand(args) {

}

void RmCommand::execute(FileSystem &fs) {
    if (!isFileExists(getArgs(), fs)) {
        cout << "No such file or directory" << endl;
    } else {
        Directory *start = getLastDir(getArgs(), fs);
        string lastFile = getArgs().substr(getArgs().find_last_of('/')+1);
        vector<BaseFile *> children;
        int i;
        if(start->getName() == lastFile){
            if(start == &fs.getWorkingDirectory() || start == &fs.getRootDirectory()
               || start == (fs.getWorkingDirectory().getParent())){
                cout << "Can't remove directory" << endl;
            }
            else{
                delete start;
            }
        }
        else {
            children = start->getChildren();
            //go over all (start->getChildren()), check if directory and name = fileName
            for (i = 0; i < children.size(); i++) {
                if(children[i]->getName() == lastFile){
                    start->removeFile(lastFile);
                }
            }
        }
    }
}



string RmCommand::toString() {
    return "rm";
}

HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> &history)
        : BaseCommand(args), history(history) {

}

void HistoryCommand::execute(FileSystem &fs) {
    for(int i= 0;i<history.size();i++){
        cout << i + "\t" + history[i]->toString() +getArgs().substr(getArgs().find_first_of("/")) <<endl;
    }

}

string HistoryCommand::toString() {
    return "history";
}

VerboseCommand::VerboseCommand(string args) : BaseCommand(args) {

}

void VerboseCommand::execute(FileSystem &fs) {
    int j = atoi(getArgs().c_str());
    if( j ==0 | j== 1| j==2 |j==3)
        verbose =j;
    else
        cout << "Wrong verbose input";
}

string VerboseCommand::toString() {
    return "verbose";
}

ErrorCommand::ErrorCommand(string args) : BaseCommand(args) {

}

void ErrorCommand::execute(FileSystem &fs) {
    size_t k = getArgs().find(' ');
    cout << getArgs().substr(0, k) + ": Unknown command";
}

string ErrorCommand::toString() {
    return "ErrorCommand";
}

ExecCommand::ExecCommand(string args, const vector<BaseCommand *> &history)
        : BaseCommand(args), history(history) {

}

void ExecCommand::execute(FileSystem &fs) {
    string number;
    number = getArgs();
    int j = atoi(getArgs().c_str());
    if (j>history.size()-1 | j < 0)
        cout <<"Command not found";
    history[j]->execute(fs);
}

string ExecCommand::toString() {
    return "exec";
}
