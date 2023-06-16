#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <cstring>
#include <iostream>
#include <vector>
using namespace ::std;

// reads in a C++ program from a cpp file, and put it to the vector program
void load(vector<char *> &program);

// deletes the comment beginning with "//" from sourceLine if any
void delComment(char sourceLine[]);

// deletes all string constants from sourceLine
void delStrConsts(char sourceLine[]);

// deletes all character constants from sourceLine
void delCharConsts(char sourceLine[]);

// extracts all identifiers from sourceLine, and put them into the vector identifiers
void extractIdentifiers(char sourceLine[], vector<char *> &identifiers);

// stores all non-keyword strings in the vector identifiers into a text file
void store(vector<char *> &identifiers);

// returns true if and only if str is a C++ keyword
bool keyword(char str[]);

// returns true if and only if identifiers[ pos ] belongs to identifiers[ 0 .. pos-1 ]
bool duplicate(vector<char *> &identifiers, int pos);

const char keywords[][20] = {"auto", "break", "case", "char", "const",
                             "continue", "default", "define", "do", "double",
                             "else", "enum", "extern", "float", "for",
                             "goto", "if", "int", "long", "register",
                             "return", "short", "signed", "sizeof",
                             "static", "struct", "switch", "typedef",
                             "union", "unsigned", "void", "volatile",
                             "while", "bool", "catch", "class",
                             "const_cast", "delete", "dynamic_cast",
                             "explicit", "false", "friend", "inline",
                             "mutable", "namespace", "new", "operator",
                             "private", "protected", "public",
                             "reinterpret_cast", "static_cast", "template",
                             "this", "throw", "true", "try", "typeid",
                             "typename", "using", "virtual", "include"};

int main() {
    vector<char *> program;

    // reads in a C++ program from a cpp file, and put it to the vector program
    load(program);
    vector<char *> identifiers;
    for (size_t i = 0; i < program.size(); i++) {
        delComment(program[i]);     // deletes the comment beginning with "//" from program[ i ]
        delStrConsts(program[i]);   // deletes all string constants from program[ i ]
        delCharConsts(program[i]);  // deletes all character constants from program[ i ]
        
        if (strcmp(program[i], "") != 0)
            extractIdentifiers(program[i], identifiers);
        // extracts all identifiers from program[ i ], and put them into the vector identifiers
    }

    // stores all non-keyword strings in the vector identifiers into a text file
    store(identifiers);

    system("pause");
}

void load(vector<char *> &program) {
    ifstream inFile("test1.cpp", ios::in);
    if (!inFile) {
        cerr << "File could not be opened" << endl;
        exit(1);
    }
    while (!inFile.eof()) {
        char *line = new char[100];
        inFile.getline(line, 100, '\n');
        program.push_back(line);
    }
    inFile.close();
}

void delComment(char sourceLine[]) {
    size_t length = strlen(sourceLine);
    if (length > 1)
        for (size_t i = 0; i < length - 1; i++)
            if (sourceLine[i] == '/' && sourceLine[i + 1] == '/') {
                sourceLine[i] = '\0';
                return;
            }
}

void delStrConsts(char sourceLine[]) {
    size_t len = strlen(sourceLine);
    for (int i = 0; i < len; i++) {
        if (sourceLine[i] == '"') {
            int j = i + 1;
            while (sourceLine[j] != '"') {
                if (sourceLine[j] == '\\') {
                    j++;
                }
                sourceLine[j] = ' ';
                j++;
            }
            sourceLine[j] = ' ';
        }
    }
}

void delCharConsts(char sourceLine[]) {
    size_t len = strlen(sourceLine);
    for (int i = 0; i < len; i++) {
        if (sourceLine[i] == '\'') {
            int j = i + 1;
            while (sourceLine[j] != '\'') {
                if (sourceLine[j] == '\\') {
                    j++;
                }
                sourceLine[j] = ' ';
                j++;
            }
            sourceLine[j] = ' ';
        }
    }
}

void extractIdentifiers(char sourceLine[], vector<char *> &identifiers) {
    size_t n = strlen(sourceLine);
    for(int i = 0; i < n; i++) {
        if (isalpha(sourceLine[i]) || sourceLine[i] == '_') {
            int j = i + 1;
            while (isalnum(sourceLine[j]) || sourceLine[j] == '_') {
                j++;
            }
            char *temp = new char[32];
            strncpy(temp, sourceLine + i, j - i);
            temp[j - i] = '\0';
            if (!keyword(temp)) {
                identifiers.push_back(temp);
            }
            i = j;
        }
    }
}

void store(vector<char *> &identifiers) {
    ofstream outFile("identifiers.txt", ios::out);
    if (!outFile) {
        cerr << "File could not be opened\n";
        exit(1);
    }
    for (int i = 0; i < identifiers.size(); i++) {
        if (!duplicate(identifiers, i)) {
            outFile << identifiers[i] << endl;
        }
    }
    outFile.close();
}

bool keyword(char str[]) {
    size_t numKeywords = sizeof(keywords) / 20;
    for (size_t i = 0; i < numKeywords; i++)
        if (strcmp(keywords[i], str) == 0)
            return true;

    return false;
}

bool duplicate(vector<char *> &identifiers, int pos) {
    for (int i = 0; i < pos; i++)
        if (strcmp(identifiers[i], identifiers[pos]) == 0)
            return true;

    return false;
}