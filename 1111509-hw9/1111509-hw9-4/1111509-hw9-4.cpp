#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace ::std;

// reads in a C++ program from a cpp file, and put it to the vector program
void load(vector<string> &program);

// deletes the comment beginning with "//" from sourceLine if any
void delComment(string &sourceLine);

// deletes all string constants from sourceLine
void delStrConsts(string &sourceLine);

// deletes all character constants from sourceLine
void delCharConsts(string &sourceLine);

// extracts all identifiers from sourceLine, and put them into the vector identifiers
void extractIdentifiers(string &sourceLine, vector<string> &identifiers);

// stores all non-keyword strings in the vector identifiers into a text file
void store(vector<string> &identifiers);

// return true if and only if "str" is a C++ keyword
bool keyword(string str);

// returns true if and only if identifiers[ pos ] belongs to identifiers[ 0 .. pos-1 ]
bool duplicate(vector<string> &identifiers, int pos);

const string keywords[] = {"auto", "break", "case", "char", "const",
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
    vector<string> program;

    // reads in a C++ program from a cpp file, and put it to the vector program
    load(program);

    vector<string> identifiers;
    string null;

    for (size_t i = 0; i < program.size(); i++) {
        delComment(program[i]);     // deletes the comment beginning with "//" from program[ i ]
        delStrConsts(program[i]);   // deletes all string constants from program[ i ]
        delCharConsts(program[i]);  // deletes all character constants from program[ i ]

        if (program[i] != null)
            extractIdentifiers(program[i], identifiers);
        // extracts all identifiers from program[ i ], and put them into the vector identifiers
    }

    // stores all non-keyword strings in the vector identifiers into a text file
    store(identifiers);

    system("pause");
}

void load(vector<string> &program) {
    ifstream inFile("test2.cpp", ios::in);
    if (!inFile) {
        cerr << "File could not be opened" << endl;
        exit(1);
    }
    while (!inFile.eof()) {
        string line;
        getline(inFile, line, '\n');
        program.push_back(line);
    }
    inFile.close();
}

void delComment(string &sourceLine) {
    size_t length = sourceLine.size();
    if (length > 1)
        for (size_t i = 0; i < length - 1; i++)
            if (sourceLine[i] == '/' && sourceLine[i + 1] == '/') {
                sourceLine.erase(i, length);
                break;
            }
}

void delStrConsts(string &sourceLine) {
    size_t len = sourceLine.length();
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

void delCharConsts(string &sourceLine) {
    size_t len = sourceLine.length();
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

void extractIdentifiers(string &sourceLine, vector<string> &identifiers) {
    size_t len = sourceLine.length();
    for (int i = 0; i < len; i++) {
        if (isalpha(sourceLine[i]) || sourceLine[i] == '_') {
            int j = i + 1;
            while ( isalnum(sourceLine[j]) || sourceLine[j] == '_') {
                j++;
            }
            string temp = sourceLine.substr(i, j - i);//¨úi¨ìj-iªº¦r
            if (!keyword(temp)) {
                identifiers.push_back(temp);
            }
            i = j;
        }
    }
}

void store(vector<string> &identifiers) {
    ofstream outFile("identifiers.txt", ios::out);
    if (!outFile) {
        cerr << "File could not be opened" << endl;
        exit(1);
    }
    for (int i = 0; i < identifiers.size(); i++) {
        if (!duplicate(identifiers, i)) {
            outFile << identifiers[i] << endl;
        }
    }
    outFile.close();
}

bool keyword(string str) {
    size_t numKeywords = 64;
    for (size_t i = 0; i < numKeywords; i++)
        if (keywords[i] == str)
            return true;

    return false;
}

bool duplicate(vector<string> &identifiers, int pos) {
    for (int i = 0; i < pos; i++)
        if (identifiers[i] == identifiers[pos])
            return true;

    return false;
}