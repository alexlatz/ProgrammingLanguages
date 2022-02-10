cmake_minimum_required(VERSION 3.21)
project(ProgrammingLanguages)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE( Boost 1.7 COMPONENTS program_options REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

SET(TARGET_H
        Alpha/LexicalAnalysis/includes/Alpha.h
        Alpha/LexicalAnalysis/includes/Lexeme.h
        Alpha/LexicalAnalysis/includes/Lexer.h
        )
# Local source files here
SET(TARGET_SRC
        Alpha/LexicalAnalysis/src/Alpha.cpp
        Alpha/LexicalAnalysis/src/Lexeme.cpp
        Alpha/LexicalAnalysis/src/Lexer.cpp
        )

add_executable(LexicalAnalyzer ${TARGET_H} ${TARGET_SRC})
