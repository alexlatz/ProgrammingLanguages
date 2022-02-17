//
// Created by Alex Latz on 2/9/22.
//

#include "../includes/Alpha.h"

bool Alpha::hadSyntaxError = false;

void Alpha::runFile(const char* path) {
    auto* sourceCode = new ifstream(path, ifstream::in);
    if (sourceCode->fail()) {
        cout << "Error: Alpha has failed to read this file." << endl;
    } else {
        run(sourceCode);
    }
}

void Alpha::run(ifstream* file) {
    Lexer lexer(file);
    vector<Lexeme*> lexemes = lexer.lex();
    for (Lexeme* lex : lexemes) cout << *lex << endl;
}

void Alpha::report(int lineNumber, string where, string message) {
    cerr << "[line: " << lineNumber << "] Error " << where << ": " << message;
    hadSyntaxError = true;
}

void Alpha::error(Lexeme lexeme, string message) {
    if (lexeme.getType() == TokenType::ENDFILE) {
        report(lexeme.getLineNum(), "at: end of file", message);
    } else {
        report(lexeme.getLineNum(), "at: " + tokenName[lexeme.getType()], message);
    }
}

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        cout << "Usage: alpha [path to .alpha file]" << endl;
        return 0;
    } else if (argc == 2) {
        Alpha::runFile(argv[1]);
    }
}