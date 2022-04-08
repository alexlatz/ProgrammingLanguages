cmake_minimum_required(VERSION 3.21)
project(ProgrammingLanguages)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE( Boost 1.7 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

SET(TARGET_H
        Alpha/includes/Alpha.h
        Alpha/includes/LexicalAnalysis/Lexeme.h
        Alpha/includes/LexicalAnalysis/Lexer.h
        Alpha/includes/Parsing/Parser.h
        Alpha/includes/Environment/Environment.h Alpha/includes/Evaluation/Evaluator.h)
# Local source files here
SET(TARGET_SRC
        Alpha/src/Alpha.cpp
        Alpha/src/LexicalAnalysis/Lexeme.cpp
        Alpha/src/LexicalAnalysis/Lexer.cpp
        Alpha/src/Parsing/Parser.cpp
        Alpha/src/Environment/Environment.cpp Alpha/src/Evaluation/Evaluator.cpp)

add_executable(Alpha ${TARGET_H} ${TARGET_SRC})
