//
// Created by Alex Latz on 2/9/22.
//

#ifndef PROGRAMMINGLANGUAGES_ALPHA_H
#define PROGRAMMINGLANGUAGES_ALPHA_H
#include "Lexer.h"
#include <fstream>

class Alpha {
    static void run(ifstream* file);
    static bool hadSyntaxError;
    public:
        static void runFile(const char* path);
        static void report(int lineNumber, string where, string message);
        static void error(Lexeme lexeme, string message);
};

#endif //PROGRAMMINGLANGUAGES_ALPHA_H
