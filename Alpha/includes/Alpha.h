//
// Created by Alex Latz on 2/9/22.
//

#ifndef PROGRAMMINGLANGUAGES_ALPHA_H
#define PROGRAMMINGLANGUAGES_ALPHA_H
#include "LexicalAnalysis/Lexer.h"
#include <fstream>

class Alpha {
    static void run(ifstream* file);
    static vector<string> syntaxErrors;
    static vector<string> runtimeErrors;
    public:
        static void runFile(const char* path);
        static void syntaxError(int lineNumber, string where, string message);
        static void syntaxError(Lexeme lexeme, string message);
        static void runtimeError(int lineNumber, string where, string message);
        static void runtimeError(Lexeme lexeme, string message);
        static void printErrors();
};

#endif //PROGRAMMINGLANGUAGES_ALPHA_H
