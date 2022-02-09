cmake_minimum_required(VERSION 3.21)
project(ProgrammingLanguages)

set(CMAKE_CXX_STANDARD 17)

add_executable(LexicalAnalyzer Alpha/LexicalAnalysis/src/Lexer.cpp Alpha/LexicalAnalysis/includes/Lexer.h Alpha/LexicalAnalysis/src/Lexeme.cpp Alpha/LexicalAnalysis/includes/Lexeme.h Alpha/LexicalAnalysis/src/Alpha.cpp Alpha/LexicalAnalysis/includes/Alpha.h)
