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
        case RETURN:
            return evalReturn(tree, env);
        case IDENTIFIER:
            return evalIdentifier(tree, env);
        case IF:
        case ELIF:
            return evalCondition(tree, env);
        case ELSE:
            return evalElse(tree, env);
        case LET:
            return evalLet(tree, env);
        case FXN:
            return evalFxn(tree, env);
        case FOR:
            return evalFor(tree, env);
        case WHILE:
            return evalWhile(tree, env);
        case IN:
            break;
        case BE:
            return evalBe(tree, env);
        case ADD:
        case ADDBE:
            return evalAdd(tree, env);
        case SUB:
        case SUBBE:
            return evalSub(tree, env);
        case X:
        case XBE:
            return evalX(tree, env);
        case DIV:
        case DIVBE:
            return evalDiv(tree, env);
        case MOD:
        case MODBE:
            return evalMod(tree, env);
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
            return evalInc(tree, env);
        case DEC:
            return evalDec(tree, env);
        case AND:
            return evalAnd(tree, env);
        case OR:
            return evalOr(tree, env);
        case NOT:
            return evalNot(tree, env);
        case STATEMENTLIST:
            return evalStatementList(tree, env, TokenType::ENDFILE);
        default:
            Alpha::runtimeError(*tree, "Evaluating: Invalid lexeme type");
            return nullptr;
    }
    Alpha::runtimeError(*tree, "Evaluating: Invalid lexeme type");
    return nullptr;
}

Lexeme* Evaluator::eval(Lexeme* tree, Environment& env, TokenType caller) {
    if (tree->getType() == TokenType::STATEMENTLIST) return evalStatementList(tree, env, caller);
    else return eval(tree, env);
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
                string finder(1, boost::get<char>(second->getValue()));
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
            return new Lexeme(TokenType::STRING, tree->getLineNum(), total);
        }
        else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to X: string");
    }
    else if (first->getType() == TokenType::CHAR) {
        char val = boost::get<char>(first->getValue());
        if (second->getType() == TokenType::NUMBER) {
            string total;
            double num = boost::get<double>(second->getValue());
            for (int i = 0; i < num; i++) total += val;
            return new Lexeme(TokenType::STRING, tree->getLineNum(), total);
        } else Alpha::runtimeError(*tree, "Evaluating: Invalid second operand to X: char");
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
            while (val.find(finder) != string::npos) val.replace(val.find(finder), finder.size(), "");
            return new Lexeme(TokenType::STRING, tree->getLineNum(), val);
        } else if (second->getType() == TokenType::CHAR) {
            string finder(1, boost::get<char>(second->getValue()));
            while (val.find(finder) != string::npos) val.replace(val.find(finder), finder.size(), "");
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

Lexeme* Evaluator::evalInc(Lexeme* tree, Environment& env) {
    Lexeme* first = eval(tree->getChild(0), env);
    if (first->getType() == TokenType::NUMBER) {
        Lexeme* value = new Lexeme(TokenType::NUMBER, tree->getLineNum(), boost::get<double>(first->getValue()) + 1);
        env.modifySymbol(boost::get<string>(tree->getChild(0)->getValue()), value);
        return value;
    }
    else if (first->getType() == TokenType::STRING) {
        string val = boost::get<string>(first->getValue());
        Lexeme* value =  new Lexeme(TokenType::STRING, tree->getLineNum(), val + " ");
        env.modifySymbol(boost::get<string>(tree->getChild(0)->getValue()), value);
        return value;
    }
    else Alpha::runtimeError(*tree, "Evaluating: Invalid operand to inc expression");
}

Lexeme* Evaluator::evalDec(Lexeme* tree, Environment& env) {
    Lexeme* first = eval(tree->getChild(0), env);
    if (first->getType() == TokenType::NUMBER) {
        Lexeme* value = new Lexeme(TokenType::NUMBER, tree->getLineNum(), boost::get<double>(first->getValue()) - 1);
        env.modifySymbol(boost::get<string>(tree->getChild(0)->getValue()), value);
        return value;
    }
    else if (first->getType() == TokenType::STRING) {
        string val = boost::get<string>(first->getValue());
        Lexeme* value = new Lexeme(TokenType::STRING, tree->getLineNum(), val.substr(0, val.size()-1));
        env.modifySymbol(boost::get<string>(tree->getChild(0)->getValue()), value);
        return value;
    }
    else Alpha::runtimeError(*tree, "Evaluating: Invalid operand to dec expression");
}

Lexeme *Evaluator::evalStatementList(Lexeme* tree, Environment& env, TokenType caller) {
    Lexeme* statementList = new Lexeme(TokenType::STATEMENTLIST, 0);
    for (int i = 0; i < tree->getChildrenLength(); i++) {
        Lexeme* result = eval(tree->getChild(i), env);
        if (result->getType() == TokenType::RETURN) {
            if (caller == TokenType::FXN) return result->getChild(0);
            else return result;
        }
        statementList->setChild(result);
    }
    return statementList;
}

Lexeme* Evaluator::evalBe(Lexeme* tree, Environment& env) {
    Lexeme* first = tree->getChild(0);
    Lexeme* second = eval(tree->getChild(1), env);
    env.modifySymbol(boost::get<string>(first->getValue()), second);
    return second;
}

Lexeme* Evaluator::evalLet(Lexeme* tree, Environment& env) {
    Lexeme* first = tree->getChild(0);
    if (first->getType() == TokenType::BE) {
        env.addSymbol(boost::get<string>(first->getChild(0)->getValue()), first);
        Lexeme *second = eval(tree->getChild(0), env);
        return second;
    } else {
        env.addSymbol(boost::get<string>(first->getValue()), new Lexeme(TokenType::NOT, tree->getLineNum(), boost::get<string>(first->getValue())));
        return first;
    }
        /*
    } else if (first->getType() == TokenType::IDENTIFIER) {
        env.addSymbol(boost::get<string>(first->getValue()), tree->getChild(1));
        return tree->getChild(1);
    }
         */
}

Lexeme* Evaluator::evalCondition(Lexeme* tree, Environment& env) {
    Lexeme* conditionLex = eval(tree->getChild(0), env);
    bool condition = boost::get<bool>(conditionLex->getValue());
    if (condition) {
        Environment newEnv(&env);
        return eval(tree->getChild(1), newEnv, tree->getType());
    } else if (tree->getChildrenLength() > 2) {
        return eval(tree->getChild(2), env);
    }
    return conditionLex;
}

Lexeme* Evaluator::evalElse(Lexeme* tree, Environment& env) {
    Environment newEnv(&env);
    return eval(tree->getChild(0), newEnv, tree->getType());
}

Lexeme* Evaluator::evalFor(Lexeme* tree, Environment& env) {
    Environment newEnv(&env);
    Lexeme* body = eval(tree->getChild(0), newEnv);
    if (tree->getChild(1)->getType() != TokenType::IN) {
        bool condition = boost::get<bool>(eval(tree->getChild(1), newEnv)->getValue());
        while (condition) {
            Environment newerEnv(&newEnv);
            body = eval(tree->getChild(3), newerEnv, tree->getType());
            eval(tree->getChild(2), newEnv);
            Lexeme* result = eval(tree->getChild(1), newEnv);
            condition = boost::get<bool>(result->getValue());
        }
        return body;
    }
}

Lexeme* Evaluator::evalWhile(Lexeme* tree, Environment& env) {
    Environment newEnv(&env);
    bool condition = boost::get<bool>(eval(tree->getChild(0), newEnv)->getValue());
    Lexeme* body;
    while (condition) {
        body = eval(tree->getChild(1), newEnv, tree->getType());
        condition = boost::get<bool>(eval(tree->getChild(0), newEnv)->getValue());
    }
    return body;
}

Lexeme* Evaluator::evalIdentifier(Lexeme* tree, Environment& env) {
    if (tree->getChildrenLength() == 0) {
        Lexeme* result = env.lookup(boost::get<string>(tree->getValue()), tree->getLineNum());
        if (result != nullptr) return result;
        else Alpha::runtimeError(*tree, "Evaluating: Null Result");
    }
    //else if (tree->getChild(0)->getType() == TokenType::NUMBER) return evalCollectionGet(tree, env);
    else return evalFxnCall(tree, env);
};

Lexeme* Evaluator::evalFxn(Lexeme *tree, Environment &env) {
    string name = boost::get<string>(tree->getChild(0)->getValue());
    env.addSymbol(name, tree);
    return tree;
}

Lexeme* Evaluator::evalFxnCall(Lexeme *tree, Environment &env) {
    string name = boost::get<string>(tree->getValue());
    if (name == "print" || name == "println") {
        Lexeme* list = tree->getChild(0);
        for (int i = 0; i < list->getChildrenLength(); i++) {
            if (name == "print") cout << Lexeme::toStringValue(*eval(list->getChild(i), env));
            else if (name == "println") cout << Lexeme::toStringValue(*eval(list->getChild(i), env)) << endl;
        }
        return tree;
    }
    Lexeme* function = env.lookup(name, tree->getLineNum());
    if (function->getChild(1)->getChildrenLength() == tree->getChild(0)->getChildrenLength()) {
        Environment fxnEnv(&env);
        Lexeme* paramName = function->getChild(1);
        Lexeme* paramVal = tree->getChild(0);
        for (int i = 0; i < paramName->getChildrenLength(); i++) {
            string name = boost::get<string>(paramName->getChild(i)->getValue());
            Lexeme* value = eval(paramVal->getChild(i), env);
            fxnEnv.addSymbol(name, value);
        }
        return eval(function->getChild(2), fxnEnv, TokenType::FXN);
    } else Alpha::runtimeError(*tree, "Evaluating: No function matches these arguments.");
    return nullptr;
}

Lexeme* Evaluator::evalReturn(Lexeme *tree, Environment &env) {
    Lexeme* rtrn = new Lexeme(TokenType::RETURN, tree->getLineNum());
    rtrn->setChild(eval(tree->getChild(0), env));
    return rtrn;
}

/*
Lexeme* Evaluator::evalCollectionGet(Lexeme *tree, Environment &env) {
    Lexeme* coll = env.lookup(boost::get<string>(tree->getValue()), tree->getLineNum());
    int num = (int)floor(boost::get<double>(tree->getChild(0)->getValue()));
    int size = (int)floor(boost::get<double>(coll->getChild(0)->getValue()));
    if (num < size) {
        Lexeme* child = coll->getChild(num);
        if (child == nullptr) Alpha::runtimeError(*coll, "Evaluating: array index not set");
        return child;
    }
    else Alpha::runtimeError(*coll, "Evaluating: array index out of bounds");
    return nullptr;
}
 */
