//
// Created by Alex Latz on 4/5/22.
//

#include "../../includes/Evaluation/Evaluator.h"
#include "../../includes/Alpha.h"

Lexeme* Evaluator::eval(Lexeme* tree, Environment& env) {
    switch (tree->getType()) {
        case NUMBER:
        case BOOL:
        case STRING:
        case CHAR:
        case COLLECTION:
            return tree;
        case IDENTIFIER:
            return env.lookup(boost::get<string>(tree->getValue()), tree->getLineNum());
        case RETURN:
            break;
        case IF:
            break;
        case ELIF:
            break;
        case ELSE:
            break;
        case LET:
            break;
        case FXN:
            break;
        case FOR:
            break;
        case WHILE:
            break;
        case IN:
            break;
        case BE:
            break;
        case ADD:
            return evalAdd(tree, env);
        case SUB:
            return evalSub(tree, env);
        case X:
            return evalX(tree, env);
        case DIV:
            return evalDiv(tree, env);
        case MOD:
            return evalMod(tree, env);
        case ADDBE:
        case SUBBE:
        case XBE:
        case DIVBE:
        case MODBE:
        case IS:
            return evalIs(tree, env);
        case LESS:
            return evalLess(tree, env);
        case MORE:
            return evalMore(tree, env);
        case LESSIS:
            return evalLessis(tree, env);
        case MOREIS:
            return evalMoreis(tree, env);
        case INC:
            break;
        case DEC:
            break;
        case AND:
            return evalAnd(tree, env);
        case OR:
            return evalOr(tree, env);
        case NOT:
            return evalNot(tree, env);
        case STATEMENTLIST:
            return evalStatementList(tree, env);
        case PARAMETERLIST:
            break;
    }
}

Lexeme *Evaluator::evalAdd(Lexeme *tree, Environment &env) {
    Lexeme* first = eval(tree->getChild(0), env);
    Lexeme* second = eval(tree->getChild(1), env);
    if (first->getType() == TokenType::NUMBER) {
        if (second->getType() == TokenType::NUMBER) return new Lexeme(TokenType::NUMBER, tree->getLineNum(), boost::get<double>(first->getValue()) + boost::get<double>(second->getValue()));
        else if (second->getType() == TokenType::STRING) return new Lexeme(TokenType::STRING, tree->getLineNum(), to_string(boost::get<double>(first->getValue())) + boost::get<string>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to add: number");
    }
    else if (first->getType() == TokenType::STRING) {
        string val = boost::get<string>(first->getValue());
        if (second->getType() == TokenType::NUMBER) return new Lexeme(TokenType::STRING, tree->getLineNum(), val + to_string(boost::get<double>(second->getValue())));
        else if (second->getType() == TokenType::STRING) return new Lexeme(TokenType::STRING, tree->getLineNum(), val + boost::get<string>(second->getValue()));
        else if (second->getType() == TokenType::CHAR) return new Lexeme(TokenType::STRING, tree->getLineNum(), val + boost::get<char>(second->getValue()));
        else if (second->getType() == TokenType::BOOL) return new Lexeme(TokenType::STRING, tree->getLineNum(), val + to_string(boost::get<bool>(second->getValue())));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to add: string");
    }
    else if (first->getType() == TokenType::CHAR) {
        char c = boost::get<char>(first->getValue());
        if (second->getType() == TokenType::STRING) return new Lexeme(TokenType::STRING, tree->getLineNum(), c + boost::get<string>(second->getValue()));
        else if (second->getType() == TokenType::CHAR) return new Lexeme(TokenType::STRING, tree->getLineNum(), (char)(c + boost::get<char>(second->getValue())));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to add: char");
    }
    else if (first->getType() == TokenType::BOOL) {
        bool val = boost::get<bool>(first->getValue());
        if (second->getType() == TokenType::STRING) return new Lexeme(TokenType::STRING, tree->getLineNum(), to_string(val) + boost::get<string>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to add: bool");
    }
    else Alpha::runtimeError(*tree, "Evaluating: Invalid first operand to add expression");
    return nullptr;
}

Lexeme *Evaluator::evalSub(Lexeme *tree, Environment &env) {
    Lexeme* first = eval(tree->getChild(0), env);
    if (tree->getChildrenLength() > 1) {
        Lexeme *second = eval(tree->getChild(1), env);
        if (first->getType() == TokenType::NUMBER) {
            if (second->getType() == TokenType::NUMBER)
                return new Lexeme(TokenType::NUMBER, tree->getLineNum(), boost::get<double>(first->getValue()) -
                                                                         boost::get<double>(second->getValue()));
            else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to sub: number");
        } else if (first->getType() == TokenType::STRING) {
            string val = boost::get<string>(first->getValue());
            if (second->getType() == TokenType::NUMBER)
                return new Lexeme(TokenType::STRING, tree->getLineNum(), val.substr(0, boost::get<double>(
                        second->getValue())));
            else if (second->getType() == TokenType::STRING) {
                string finder = boost::get<string>(second->getValue());
                while (val.find(finder) != string::npos) val.replace(val.find(finder), string::npos, finder);
                return new Lexeme(TokenType::STRING, tree->getLineNum(), val);
            } else if (second->getType() == TokenType::CHAR) {
                string finder = to_string(boost::get<char>(second->getValue()));
                while (val.find(finder) != string::npos) val.replace(val.find(finder), string::npos, finder);
                return new Lexeme(TokenType::STRING, tree->getLineNum(), val);
            } else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to sub: string");
        } else if (first->getType() == TokenType::CHAR) {
            char c = boost::get<char>(first->getValue());
            if (second->getType() == TokenType::STRING)
                return new Lexeme(TokenType::STRING, tree->getLineNum(), c + boost::get<string>(second->getValue()));
            else if (second->getType() == TokenType::CHAR)
                return new Lexeme(TokenType::STRING, tree->getLineNum(), (char) (c + boost::get<char>(
                        second->getValue())));
            else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to sub: char");
        } else if (first->getType() == TokenType::BOOL) {
            bool val = boost::get<bool>(first->getValue());
            if (second->getType() == TokenType::STRING)
                return new Lexeme(TokenType::STRING, tree->getLineNum(), to_string(val) +
                                                                         boost::get<string>(second->getValue()));
            else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to sub: bool");
        }
        else Alpha::runtimeError(*tree, "Evaluating: Invalid first operand to sub expression");
    } else {
        if (first->getType() == TokenType::NUMBER) return new Lexeme(TokenType::NUMBER, tree->getLineNum(), -1.0 * boost::get<double>(first->getValue()));
        else if (first->getType() == TokenType::STRING) {
            string val = boost::get<string>(first->getValue());
            return new Lexeme(TokenType::STRING, tree->getLineNum(), string(val.rbegin(), val.rend()));
        }
        else Alpha::runtimeError(*tree, "Evaluating: Invalid first operand to sub expression");
    }
    return nullptr;
}

Lexeme *Evaluator::evalX(Lexeme *tree, Environment &env) {
    Lexeme* first = eval(tree->getChild(0), env);
    Lexeme* second = eval(tree->getChild(1), env);
    if (first->getType() == TokenType::NUMBER) {
        if (second->getType() == TokenType::NUMBER) return new Lexeme(TokenType::NUMBER, tree->getLineNum(), boost::get<double>(first->getValue()) * boost::get<double>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to X: number");
    }
    else if (first->getType() == TokenType::STRING) {
        string val = boost::get<string>(first->getValue());
        if (second->getType() == TokenType::NUMBER) {
            string total;
            double num = boost::get<double>(second->getValue());
            if (num < 0) val = string(val.rbegin(), val.rend());
            for (int i = 0; i < num; i++) total += val;
            return new Lexeme(TokenType::STRING, tree->getLineNum(), val + to_string(num));
        }
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to X: string");
    }
    else Alpha::runtimeError(*tree, "Evaluating: Invalid first operand to X expression");
    return nullptr;
}

Lexeme *Evaluator::evalDiv(Lexeme *tree, Environment &env) {
    Lexeme* first = eval(tree->getChild(0), env);
    Lexeme* second = eval(tree->getChild(1), env);
    if (first->getType() == TokenType::NUMBER) {
        if (second->getType() == TokenType::NUMBER) return new Lexeme(TokenType::NUMBER, tree->getLineNum(), boost::get<double>(first->getValue()) / boost::get<double>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to div: number");
    }
    else if (first->getType() == TokenType::STRING) {
        string val = boost::get<string>(first->getValue());
        if (second->getType() == TokenType::NUMBER) return new Lexeme(TokenType::STRING, tree->getLineNum(), val.substr(0, boost::get<double>(second->getValue())));
        else if (second->getType() == TokenType::STRING) {
            string finder = boost::get<string>(second->getValue());
            while (val.find(finder) != string::npos) val.replace(val.find(finder), string::npos, "");
            return new Lexeme(TokenType::STRING, tree->getLineNum(), val);
        } else if (second->getType() == TokenType::CHAR) {
            string finder = to_string(boost::get<char>(second->getValue()));
            while (val.find(finder) != string::npos) val.replace(val.find(finder), string::npos, "");
            return new Lexeme(TokenType::STRING, tree->getLineNum(), val);
        }
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to div: string");
    }
    else Alpha::runtimeError(*tree, "Evaluating: Invalid first operand to div expression");
    return nullptr;
}

Lexeme *Evaluator::evalMod(Lexeme *tree, Environment &env) {
    Lexeme *first = eval(tree->getChild(0), env);
    Lexeme *second = eval(tree->getChild(1), env);
    if (first->getType() == TokenType::NUMBER) {
        if (second->getType() == TokenType::NUMBER) return new Lexeme(TokenType::NUMBER, tree->getLineNum(),(double)(
                                                                      (int)boost::get<double>(first->getValue()) %
                                                                              (int)boost::get<double>(second->getValue())));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to mod: number");
    }
    else Alpha::runtimeError(*tree, "Evaluating: Invalid first operand to mod expression");
    return nullptr;
}

Lexeme *Evaluator::evalMore(Lexeme *tree, Environment &env) {
    Lexeme* first = eval(tree->getChild(0), env);
    Lexeme* second = eval(tree->getChild(1), env);
    if (first->getType() == TokenType::NUMBER) {
        if (second->getType() == TokenType::NUMBER) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<double>(first->getValue()) > boost::get<double>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to more: number");
    } else if (first->getType() == TokenType::STRING) {
        if (second->getType() == TokenType::STRING) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<string>(first->getValue()) > boost::get<string>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to more: string");
    } else if (first->getType() == TokenType::CHAR) {
        if (second->getType() == TokenType::CHAR) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<char>(first->getValue()) > boost::get<char>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to more: char");
    } else if (first->getType() == TokenType::BOOL) {
        if (second->getType() == TokenType::BOOL) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<bool>(first->getValue()) > boost::get<bool>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to more: bool");
    } else if (first->getType() == TokenType::BOOL) {
        if (second->getType() == TokenType::BOOL) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<bool>(first->getValue()) > boost::get<bool>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to more: bool");
    } else if (first->getType() == TokenType::COLLECTION) {
        if (second->getType() == TokenType::COLLECTION) return new Lexeme(TokenType::BOOL, tree->getLineNum(), first->getChildrenLength() > second->getChildrenLength());
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to more: collection");
    } else Alpha::runtimeError(*tree, "Evaluating: Invalid first operand to more expression");
    return nullptr;
}
Lexeme *Evaluator::evalMoreis(Lexeme *tree, Environment &env) {
    Lexeme* first = eval(tree->getChild(0), env);
    Lexeme* second = eval(tree->getChild(1), env);
    if (first->getType() == TokenType::NUMBER) {
        if (second->getType() == TokenType::NUMBER) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<double>(first->getValue()) >= boost::get<double>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to moreis: number");
    } else if (first->getType() == TokenType::STRING) {
        if (second->getType() == TokenType::STRING) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<string>(first->getValue()) >= boost::get<string>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to moreis: string");
    } else if (first->getType() == TokenType::CHAR) {
        if (second->getType() == TokenType::CHAR) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<char>(first->getValue()) >= boost::get<char>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to moreis: char");
    } else if (first->getType() == TokenType::BOOL) {
        if (second->getType() == TokenType::BOOL) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<bool>(first->getValue()) >= boost::get<bool>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to moreis: bool");
    } else if (first->getType() == TokenType::BOOL) {
        if (second->getType() == TokenType::BOOL) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<bool>(first->getValue()) >= boost::get<bool>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to moreis: bool");
    } else if (first->getType() == TokenType::COLLECTION) {
        if (second->getType() == TokenType::COLLECTION) return new Lexeme(TokenType::BOOL, tree->getLineNum(), first->getChildrenLength() >= second->getChildrenLength());
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to moreis: collection");
    } else Alpha::runtimeError(*tree, "Evaluating: Invalid first operand to moreis expression");
    return nullptr;
}

Lexeme *Evaluator::evalLess(Lexeme *tree, Environment &env) {
    Lexeme* first = eval(tree->getChild(0), env);
    Lexeme* second = eval(tree->getChild(1), env);
    if (first->getType() == TokenType::NUMBER) {
        if (second->getType() == TokenType::NUMBER) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<double>(first->getValue()) < boost::get<double>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to less: number");
    } else if (first->getType() == TokenType::STRING) {
        if (second->getType() == TokenType::STRING) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<string>(first->getValue()) < boost::get<string>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to less: string");
    } else if (first->getType() == TokenType::CHAR) {
        if (second->getType() == TokenType::CHAR) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<char>(first->getValue()) < boost::get<char>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to less: char");
    } else if (first->getType() == TokenType::BOOL) {
        if (second->getType() == TokenType::BOOL) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<bool>(first->getValue()) < boost::get<bool>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to less: bool");
    } else if (first->getType() == TokenType::BOOL) {
        if (second->getType() == TokenType::BOOL) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<bool>(first->getValue()) < boost::get<bool>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to less: bool");
    } else if (first->getType() == TokenType::COLLECTION) {
        if (second->getType() == TokenType::COLLECTION) return new Lexeme(TokenType::BOOL, tree->getLineNum(), first->getChildrenLength() < second->getChildrenLength());
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to less: collection");
    } else Alpha::runtimeError(*tree, "Evaluating: Invalid first operand to less expression");
    return nullptr;
}

Lexeme *Evaluator::evalLessis(Lexeme *tree, Environment &env) {
    Lexeme* first = eval(tree->getChild(0), env);
    Lexeme* second = eval(tree->getChild(1), env);
    if (first->getType() == TokenType::NUMBER) {
        if (second->getType() == TokenType::NUMBER) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<double>(first->getValue()) <= boost::get<double>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to lessis: number");
    } else if (first->getType() == TokenType::STRING) {
        if (second->getType() == TokenType::STRING) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<string>(first->getValue()) <= boost::get<string>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to lessis: string");
    } else if (first->getType() == TokenType::CHAR) {
        if (second->getType() == TokenType::CHAR) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<char>(first->getValue()) <= boost::get<char>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to lessis: char");
    } else if (first->getType() == TokenType::BOOL) {
        if (second->getType() == TokenType::BOOL) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<bool>(first->getValue()) <= boost::get<bool>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to lessis: bool");
    } else if (first->getType() == TokenType::BOOL) {
        if (second->getType() == TokenType::BOOL) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<bool>(first->getValue()) <= boost::get<bool>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to lessis: bool");
    } else if (first->getType() == TokenType::COLLECTION) {
        if (second->getType() == TokenType::COLLECTION) return new Lexeme(TokenType::BOOL, tree->getLineNum(), first->getChildrenLength() <= second->getChildrenLength());
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to lessis: collection");
    } else Alpha::runtimeError(*tree, "Evaluating: Invalid first operand to lessis expression");
    return nullptr;
}

Lexeme *Evaluator::evalIs(Lexeme *tree, Environment &env) {
    Lexeme* first = eval(tree->getChild(0), env);
    Lexeme* second = eval(tree->getChild(1), env);
    if (first->getType() == TokenType::NUMBER) {
        if (second->getType() == TokenType::NUMBER) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<double>(first->getValue()) == boost::get<double>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to is: number");
    } else if (first->getType() == TokenType::STRING) {
        if (second->getType() == TokenType::STRING) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<string>(first->getValue()) == boost::get<string>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to is: string");
    } else if (first->getType() == TokenType::CHAR) {
        if (second->getType() == TokenType::CHAR) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<char>(first->getValue()) == boost::get<char>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to is: char");
    } else if (first->getType() == TokenType::BOOL) {
        if (second->getType() == TokenType::BOOL) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<bool>(first->getValue()) == boost::get<bool>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to is: bool");
    } else if (first->getType() == TokenType::BOOL) {
        if (second->getType() == TokenType::BOOL) return new Lexeme(TokenType::BOOL, tree->getLineNum(), boost::get<bool>(first->getValue()) == boost::get<bool>(second->getValue()));
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to is: bool");
    } else if (first->getType() == TokenType::COLLECTION) {
        if (second->getType() == TokenType::COLLECTION) return new Lexeme(TokenType::BOOL, tree->getLineNum(), first->getChildrenLength() == second->getChildrenLength());
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to is: collection");
    } else Alpha::runtimeError(*tree, "Evaluating: Invalid first operand to is expression");
    return nullptr;
}

Lexeme* Evaluator::evalAnd(Lexeme* tree, Environment& env) {
    Lexeme* first = eval(tree->getChild(0), env);
    Lexeme* second = eval(tree->getChild(1), env);
    return new Lexeme(TokenType::BOOL, tree->getLineNum(), evalBoolVal(first) && evalBoolVal(second));
}

Lexeme* Evaluator::evalOr(Lexeme* tree, Environment& env) {
    Lexeme* first = eval(tree->getChild(0), env);
    Lexeme* second = eval(tree->getChild(1), env);
    return new Lexeme(TokenType::BOOL, tree->getLineNum(), evalBoolVal(first) || evalBoolVal(second));
}

Lexeme* Evaluator::evalNot(Lexeme* tree, Environment& env) {
    Lexeme* val = eval(tree->getChild(0), env);
    return new Lexeme(TokenType::BOOL, tree->getLineNum(), !evalBoolVal(val));
}

bool Evaluator::evalBoolVal(Lexeme* tree) {
    if (tree->getType() == TokenType::NUMBER) return boost::get<double>(tree->getValue()) != 0;
    else if (tree->getType() == TokenType::STRING) return !boost::get<string>(tree->getValue()).empty();
    else if (tree->getType() == TokenType::CHAR) return !isblank(boost::get<char>(tree->getValue()));
    else if (tree->getType() == TokenType::BOOL) return boost::get<bool>(tree->getValue());
    else if (tree->getType() == TokenType::COLLECTION) return tree->getChildrenLength() != 0;
    else return false;
}

Lexeme *Evaluator::evalStatementList(Lexeme* tree, Environment& env) {
    Lexeme* statementList = new Lexeme(TokenType::STATEMENTLIST, 0);
    for (int i = 0; i < tree->getChildrenLength(); i++) {
        statementList->setChild(eval(tree->getChild(i), env));
    }
    return statementList;
}



