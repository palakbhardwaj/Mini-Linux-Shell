#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;

vector<string> split(const string &input) {
    vector<string> tokens;
    stringstream ss(input);
    string word;
    while (ss >> word) tokens.push_back(word);
    return tokens;
}

void executeCommand(vector<string> tokens, bool background) {
    vector<char*> args;

    for (auto &t : tokens)
        args.push_back(&t[0]);

    args.push_back(NULL);

    pid_t pid = fork();

    if (pid == 0) {
        execvp(args[0], args.data());
        perror("exec failed");
        exit(1);
    } else {
        if (!background)
            wait(NULL);
    }
}

void handlePipe(string input) {
    int pos = input.find("|");

    string left = input.substr(0, pos);
    string right = input.substr(pos + 1);

    vector<string> leftTokens = split(left);
    vector<string> rightTokens = split(right);

    int fd[2];
    pipe(fd);

    if (fork() == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);

        vector<char*> args;
        for (auto &t : leftTokens)
            args.push_back(&t[0]);
        args.push_back(NULL);

        execvp(args[0], args.data());
        exit(1);
    }

    if (fork() == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);

        vector<char*> args;
        for (auto &t : rightTokens)
            args.push_back(&t[0]);
        args.push_back(NULL);

        execvp(args[0], args.data());
        exit(1);
    }

    close(fd[0]);
    close(fd[1]);
    wait(NULL);
    wait(NULL);
}

int main() {
    while (true) {
        cout << ">>> ";
        string input;
        getline(cin, input);

        if (input == "exit") break;

        if (input.find("|") != string::npos) {
            handlePipe(input);
            continue;
        }

        vector<string> tokens = split(input);
        if (tokens.empty()) continue;

        bool background = false;

        if (tokens.back() == "&") {
            background = true;
            tokens.pop_back();
        }

        executeCommand(tokens, background);
    }

    return 0;
}