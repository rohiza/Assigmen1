#include <iostream>
#include "../include/Environment.h"

using namespace std;

void Environment::start() {
    string args = "";
    while (args != "exit") {
        cout << fs.getWorkingDirectory().getAbsolutePath() + ">";
        getline(cin, args);
        if(args == "exit") {
            return;
        }
        if(verbose ==2 || verbose == 3)
            whichCommand(args)->toString();
        whichCommand(args)->execute(fs);
        addToHistory(whichCommand(args));
    }

}

Environment::Environment() : commandsHistory(), fs() {
}

Environment::Environment(const Environment &other) {
    if (verbose == 1 | verbose == 3) {
        cout << "Environment ::Environment(const Environment& other)" << endl;
    }
    envcopy(other);
}

FileSystem &Environment::getFileSystem() {
    return fs;
}

const vector<BaseCommand *> &Environment::getHistory() const {
    return commandsHistory;
}

void Environment::addToHistory(BaseCommand *command) {
    commandsHistory.push_back(command);
}

void Environment::envcopy(const Environment &other) {

    for (int i = 0; i < other.commandsHistory.size(); i++) {

        this->addToHistory(other.commandsHistory[i]);
    }
    fs = other.fs;
}


Environment::~Environment() {
    if (verbose == 1 | verbose == 3) {
        cout << "Environment ::~Environment()" << endl;
    }
    clear();
}

Environment::Environment(Environment &&other) {
    if (verbose == 1 | verbose == 3) {
        cout << "Environment ::Environment(Environment &&other)" << endl;
    }
    commandsHistory = other.getHistory();
    fs = other.getFileSystem();
}

Environment &Environment::operator=(const Environment &other) {
    if (verbose == 1 | verbose == 3) {
        cout << "Environment& Environment ::operator=(const Environment &other) " << endl;
    }
    if (this != &other) {
        clear();
        envcopy(other);
    }

    return *this;
}

Environment &Environment::operator=(Environment &&other) {
    if (verbose == 1 | verbose == 3) {
        cout << "Environment& Environment ::operator=(Environment &&other)" << endl;
    }
    if (this != &other) {
        commandsHistory = other.getHistory();
        fs = other.getFileSystem();
    }
    return *this;
}

void Environment::clear() {

    for (vector<BaseCommand *>::iterator it = commandsHistory.begin(); it != commandsHistory.end(); it++) {
        delete *it;
    }
    commandsHistory.clear();
}

BaseCommand *Environment::whichCommand(string arg) {

    unsigned long firstSpace = arg.find(' ');
    string command = firstSpace == string::npos ? arg : arg.substr(0, firstSpace);
    auto argsCommand = firstSpace == string::npos ? arg : arg.substr(arg.find(' ')+1);
    if (command == "pwd") {
        BaseCommand *pwd = new PwdCommand(argsCommand);
        return pwd;
    } else if (command == "cd") {
        BaseCommand *cd = new CdCommand(argsCommand);
        return cd;
    } else if (command == "ls") {
        BaseCommand *ls;
        if(argsCommand == "ls")
        ls = new LsCommand("");
        else
        ls = new LsCommand(argsCommand);
        return ls;
    } else if (command == "mkdir") {
        BaseCommand *mkdir = new MkdirCommand(argsCommand);
        return mkdir;
    } else if (command == "mkfile") {
        BaseCommand *mkfile = new MkfileCommand(argsCommand);
        return mkfile;
    } else if (command == "cp") {
        BaseCommand *cp = new CpCommand(argsCommand);
        return cp;
    } else if (command == "mv") {
        BaseCommand *mv = new MvCommand(argsCommand);
        return mv;
    } else if (command == "rename") {
        BaseCommand *rename = new RenameCommand(argsCommand);
        return rename;
    } else if (command == "rm") {
        BaseCommand *rm = new RmCommand(argsCommand);
        return rm;
    } else if (command == "history") {
        BaseCommand *history = new HistoryCommand(arg, getHistory());
        return history;
    } else if (command == "verbose") {
        BaseCommand *verbose = new VerboseCommand(argsCommand);
        return verbose;
    } else if (command == "exec") {
        BaseCommand *exec = new ExecCommand(argsCommand, getHistory());
        return exec;
    } else {
        BaseCommand *error = new ErrorCommand(arg);
        return error;
    }
}
