//
// Created by Alex Latz on 2/9/22.
//

#include "../includes/Alpha.h"
#include "../includes/Parsing/Parser.h"
#include "../includes/Environment/Environment.h"
#include "../includes/Evaluation/Evaluator.h"

vector<string> Alpha::runtimeErrors;
vector<string> Alpha::syntaxErrors;

void Alpha::runFile(const char* path) {
    auto* sourceCode = new ifstream(path, ifstream::in);
    if (sourceCode->fail()) {
        cout << "Error: Alpha has failed to read this file." << endl;
    } else {
        run(sourceCode);
        printErrors();
    }
}

void Alpha::run(ifstream* file) {
    Lexer lexer(file);
    vector<Lexeme*> lexemes = lexer.lex();
    for (Lexeme* lex : lexemes) cout << *lex << endl;
    Parser parser(lexemes);
    Lexeme* lex = parser.program();
    Environment global(nullptr);
    Evaluator eval;
    cout << *eval.eval(lex, global) << endl;
    cout << "PRINTING GLOBAL ENV" << endl;
    global.printSymbols();
}

void Alpha::syntaxError(int lineNumber, string where, string message) {
    Alpha::syntaxErrors.push_back("[line: " + to_string(lineNumber) + "] Syntax Error: " + where + ": " + message);
}

void Alpha::syntaxError(Lexeme lexeme, string message) {
    if (lexeme.getType() == TokenType::ENDFILE) {
        syntaxError(lexeme.getLineNum(), "at: end of file", message);
    } else {
        syntaxError(lexeme.getLineNum(), "at: " + lexeme.toString(), message);
    }
}

void Alpha::runtimeError(int lineNumber, string where, string message) {
    Alpha::runtimeErrors.push_back("[line: " + to_string(lineNumber) + "] Runtime Error: " + where + ": " + message);
    printErrors();
    exit(EXIT_FAILURE);
}

void Alpha::runtimeError(Lexeme lexeme, string message) {
    if (lexeme.getType() == TokenType::ENDFILE) {
        runtimeError(lexeme.getLineNum(), "at: end of file", message);
    } else {
        runtimeError(lexeme.getLineNum(), "at: " + lexeme.toString(), message);
    }
}

void Alpha::printErrors() {
    static const string ANSI_YELLOW = "\u001B[33m";
    static const string ANSI_CLEAR = "\u001B[0m";
    for (string& s : syntaxErrors) cout << ANSI_YELLOW << s << ANSI_CLEAR << endl;
    for (string& s : runtimeErrors) cerr << s << endl;
}

int main(int argc, char const *argv[]) {
    if (argc == 1) {
        cout << "Usage: alpha [path to .alpha file]" << endl;
        return 0;
    } else if (argc == 2) {
        Alpha::runFile(argv[1]);
    }
}