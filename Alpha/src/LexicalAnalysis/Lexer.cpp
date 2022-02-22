//
// Created by Alex Latz on 2/8/22.
//

#include "../../includes/LexicalAnalysis/Lexer.h"

Lexer::Lexer(ifstream* file) {
    this->file = file;
    this->lineNum = 1;
}

vector<Lexeme*> Lexer::lex() {
    vector<Lexeme*> vec;
    while (!file->eof() && !file->fail()) {
        Lexeme* lexeme = getNextLexeme();
        if (lexeme != nullptr) vec.push_back(lexeme);
    }
    vec.push_back(new Lexeme(TokenType::ENDFILE, lineNum));
    return vec;
}

map<string, TokenType> Lexer::keywords {
    {"C", TokenType::OPEN_PAREN},
    {"D", TokenType::CLOSE_PAREN},
    {"L", TokenType::OPEN_BLOCK},
    {"J", TokenType::CLOSE_BLOCK},
    {"O", TokenType::LINE_END},
    {"II", TokenType::COMMENT_LINEEND},
    {"V", TokenType::OPEN_SQ_BRACKET},
    {"A", TokenType::CLOSE_SQ_BRACKET},
    {"dB", TokenType::STR_OPEN},
    {"Db", TokenType::STR_CLOSE},
    {"dC", TokenType::CHAR_OPEN},
    {"Dc", TokenType::CHAR_CLOSE},
    {"rtrn", TokenType::RETURN},
    {"if", TokenType::IF},
    {"elif", TokenType::ELIF},
    {"else", TokenType::ELSE},
    {"point", TokenType::POINT},
    {"let", TokenType::LET},
    {"fxn", TokenType::FXN},
    {"for", TokenType::FOR},
    {"while", TokenType::WHILE},
    {"in", TokenType::IN},
    {"be", TokenType::BE},
    {"add", TokenType::ADD},
    {"sub", TokenType::SUB},
    {"X", TokenType::X},
    {"div", TokenType::DIV},
    {"mod", TokenType::MOD},
    {"addbe", TokenType::ADDBE},
    {"subbe", TokenType::SUBBE},
    {"Xbe", TokenType::XBE},
    {"divbe", TokenType::DIVBE},
    {"modbe", TokenType::MODBE},
    {"inc", TokenType::INC},
    {"dec", TokenType::DEC},
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"not", TokenType::NOT},
    {"less", TokenType::LESS},
    {"more", TokenType::MORE},
    {"is", TokenType::IS},
    {"lessis", TokenType::LESSIS},
    {"moreis", TokenType::MOREIS},
};

map<string, double> Lexer::digits {
    {"zero", 0},
    {"one", 1},
    {"two", 2},
    {"three", 3},
    {"four", 4},
    {"five", 5},
    {"six", 6},
    {"seven", 7},
    {"eight", 8},
    {"nine", 9},
    {"ten", 10},
    {"eleven", 11},
    {"twelve", 12},
    {"thirteen", 13},
    {"fourteen", 14},
    {"fifteen", 15},
    {"sixteen", 16},
    {"seventeen", 17},
    {"eighteen", 18},
    {"nineteen", 19},
    {"twenty", 20},
    {"thirty", 30},
    {"forty", 40},
    {"fifty", 50},
    {"sixty", 60},
    {"seventy", 70},
    {"eighty", 80},
    {"ninety", 90},
    {"hundred", 100},
    {"thousand", 1000},
    {"million", 1000000},
    {"billion", 1000000000},
    {"trillion", 1000000000000},
    {"quadrillion", 1000000000000000},
    {"quintillion", 1000000000000000000},
    {"sextillion", 1000000000000000000000.0},
    {"septillion", 1000000000000000000000000.0},
    {"octillion", 1000000000000000000000000000.0},
    {"nonillion", 1000000000000000000000000000000.0},
    {"decillion", 1000000000000000000000000000000000.0},
    {"undecillion", 1000000000000000000000000000000000000.0},
};

Lexeme* Lexer::getNextLexeme() {
    char c;
    file->get(c);
    switch (c) {
        case 'O':
        case '\n':
            return advance(c);
        case '\t':
        case ' ':
        case '\0':
            return nullptr;
        case 'C':
            return new Lexeme(TokenType::OPEN_PAREN, lineNum);
        case 'D':
            return new Lexeme(TokenType::CLOSE_PAREN, lineNum);
        case 'L':
            return new Lexeme(TokenType::OPEN_BLOCK, lineNum);
        case 'J':
            return new Lexeme(TokenType::CLOSE_BLOCK, lineNum);
        case 'V':
            return new Lexeme(TokenType::OPEN_SQ_BRACKET, lineNum);
        case 'A':
            return new Lexeme(TokenType::CLOSE_SQ_BRACKET, lineNum);
        case 'X':
            return new Lexeme(TokenType::X, lineNum);
        default:
            return lexMultiChar(c);
    }
}

bool Lexer::validMultiChar(char c) {
    return isalpha(c) && c != 'D' && c != 'J' && c != 'A' && c != 'O';
}

Lexeme* Lexer::lexMultiChar(char c) {
    string word;
    while (validMultiChar(c) && !file->eof()) {
        word.push_back(c);
        file->get(c);
    }
    if (Lexer::keywords.count(word) > 0) {
        TokenType type = Lexer::keywords.at(word);
        if ((type == TokenType::STR_OPEN || type == TokenType::CHAR_OPEN) && c == ' ') return lexQuotes(type == TokenType::STR_OPEN);
        file->unget();
        return new Lexeme(type, lineNum);
    } else {
        if (c == '\n' || c == '\t' || c == '\0' || c == 'D' || c == 'J' || c == 'A' || c == 'O') file->unget();
        if (Lexer::digits.count(word) > 0) return lexNumber(word);
        else if (word == "true" || word == "false") return new Lexeme(TokenType::BOOL, lineNum, word == "true");
        else return new Lexeme(TokenType::IDENTIFIER, lineNum, word);
    }
}


Lexeme* Lexer::advance(char c) {
    if (c == 'O') return new Lexeme(TokenType::LINE_END, lineNum);
    else if (c == '\n') {
        char peek;
        file->get(peek);
        if (peek == '\t' || peek == ' ') {
            lineNum++;
            return nullptr;
        } else {
            file->unget();
            return new Lexeme(TokenType::LINE_END, lineNum++);
        }
    }
    return nullptr;
}

Lexeme* Lexer::lexQuotes(bool str) {
    string quote;
    while (!file->eof() && (quote.length() < 3 || (quote.substr(quote.length()-3, 3) != " Db")) && (quote.length() < 3 || quote.substr(quote.length()-3, 3) != " Dc")) {
        char c;
        file->get(c);
        quote.push_back(c);
    }
    if (str) {
        return new Lexeme(TokenType::STRING, lineNum, quote.substr(0, quote.length()-3));
    } else {
        if (quote.length() > 4) {
            Alpha::syntaxError(lineNum, "CHAR", "Invalid char: exceeds one character");
            return nullptr;
        }
        else return new Lexeme(TokenType::CHAR, lineNum, quote[0]);
    }
}

Lexeme* Lexer::lexNumber(string& firstWord) {
    vector<string> words;
    vector<string> decimals;
    words.push_back(firstWord);
    string nextWord;
    int fullInput = 0;
    bool first = true;
    bool pointMode = false;
    while ((first || (Lexer::digits.count(nextWord) > 0) || nextWord == "point") && !file->eof()) {
        if (first) first = false;
        nextWord.clear();
        char c;
        file->get(c);
        while (isalpha(c) && !file->eof()) {
            nextWord.push_back(c);
            fullInput++;
            file->get(c);
        }
        if (!isalpha(c)) fullInput++;
        if (!nextWord.empty() && (Lexer::digits.count(nextWord) > 0 || nextWord == "point")) {
            if (nextWord != "point" && !pointMode) words.push_back(nextWord);
            else if (!pointMode) pointMode = true;
            else decimals.push_back(nextWord);
            fullInput = 1;
        }
    }
    if (Lexer::digits.count(nextWord) == 0) {
        for (int i = 0; i < fullInput; i++) {
            file->unget();
        }
    }
    double value = 0, storeNum = 1;
    auto begin = words.rbegin();
    for (auto it = words.rbegin(); it != words.rend(); it++) {
        if (Lexer::digits.at(*it) > 100) {
            value += storeNum * parseHundredsGroup(begin, it);
            if (storeNum < Lexer::digits.at(*it)) storeNum = Lexer::digits.at(*it);
            else Alpha::syntaxError(lineNum, "NUMBER", "Invalid order of places (ex. millions)");
            begin = it + 1;
        }
    }
    if (begin != words.rend()) value += storeNum * parseHundredsGroup(begin, words.rend());
    for (int i = 1; i <= decimals.size(); i++) {
        if (Lexer::digits.count(decimals[i-1]) == 0 || Lexer::digits.at(decimals[i-1]) > 9) Alpha::syntaxError(lineNum, "NUMBER", "Invalid decimal");
        else  value += (Lexer::digits.at(decimals[i-1]) / (double) pow(10.0, (double)i));
    }
    return new Lexeme(TokenType::NUMBER, lineNum, value);
}

double Lexer::parseHundredsGroup(vector<string>::reverse_iterator begin, vector<string>::reverse_iterator end) const {
    bool firstGroup = false;
    double storeNum, finalVal = 0;
    for (auto it(begin); it != end; it++) {
        double value = Lexer::digits.at(*it);
        if (value < 100 && !firstGroup) {
            if (finalVal > 9 && finalVal < 20 && it != begin) Alpha::syntaxError(lineNum, "NUMBER", "Invalid: illegal teen-number");
            else finalVal += value;
        } else if (value < 10 && firstGroup) {
            finalVal += (storeNum * value);
        } else if (value == 100) {
            if (!firstGroup) firstGroup = true;
            storeNum = value;
        } else Alpha::syntaxError(lineNum, "NUMBER", "Invalid: general number parsing error");
    }
    return finalVal;
}


