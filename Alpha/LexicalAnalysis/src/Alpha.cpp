//
// Created by Alex Latz on 2/9/22.
//

#include "../includes/Alpha.h"

static void runFile(const char* path) {
    ifstream* sourceCode = new ifstream(path, ifstream::in);
    if (sourceCode->fail()) {
        cout << "Error: Alpha has failed to read this file." << endl;
    } else {
        run(sourceCode);
    }
}

static void run(ifstream* file) {
    Lexer lexer(file);
    vector<Lexeme*> lexemes = lexer.lex();
    for (Lexeme* lex : lexemes) cout << *lex << endl;
}

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        cout << "Usage: alpha [path to .alpha file]" << endl;
        return 0;
    } else if (argc == 2) {
        runFile(argv[1]);
    }
}