#include "Parser.h"

#include <QtMath>
#include <QRegExp>
#include "Model/TreeItem.h"

Parser::Parser()
{}

qreal Parser::calcucateExpression(const QString &notice)
{
    StringStack polishNotice = stringToPolishNotice(notice);
    QStack<qreal> numbers;
    auto ok = true;

    foreach (QString str, polishNotice) {
        auto number = str.toDouble(&ok);
        if(ok)
            numbers.push(number);
        else if(str == "+")
            numbers.push(numbers.pop() + numbers.pop());
        else if(str == "-")
            numbers.push(numbers.pop() - numbers.pop());
        else if(str == "×")
            numbers.push(numbers.pop() * numbers.pop());
        else if(str == "÷"){
            auto first = numbers.pop();
            auto second = numbers.pop();
            numbers.push(second / first);
        }
        else if(str == "√")
            numbers.push(qSqrt(numbers.pop()));
        else if(str == "%")
            numbers.push(numbers.pop() / 100);
        else if(str == "sin")
            numbers.push(qSin(numbers.pop()));
        else if(str == "cos")
            numbers.push(qCos(numbers.pop()));
        else if(str == "tg")
            numbers.push(qTan(numbers.pop()));
        else if(str == "ctg")
            numbers.push(1 / qTan(numbers.pop()));
        else if(str == "asin")
            numbers.push(qAsin(numbers.pop()));
        else if(str == "acos")
            numbers.push(qAcos(numbers.pop()));
        else if(str == "atg")
            numbers.push(qAtan(numbers.pop()));
        else if(str == "actg")
            numbers.push(qAtan(1 / numbers.pop()));
    }
    return numbers.pop();
}

bool Parser::isOperator(QString op)
{
    return op == "+" || op == "-" || op == "×" || op == "÷" || op == "%" || op == "√";
}

bool Parser::isFunction(QString fun)
{
    return fun == "sin" || fun == "cos" || fun == "tg" || fun == "ctg" ||
            fun == "asin" || fun == "acos" || fun == "atg" || fun == "actg";
}

Parser::Priority Parser::getPriority(QString str)
{
   if(isFunction(str) || str == "%" || str == "√")
       return HIGH;
   if(str == "×" || str == "÷")
       return MID;
   if(str == "+" || str == "-")
       return LOW;
   return VERY_LOW;
}

Parser::StringStack Parser::stringToPolishNotice(const QString& notice)
{
    StringStack inf = splitToSimples(notice), stack, polishNotice;
    QString entity;
    auto ok = true;

    while(!inf.isEmpty()){
        entity = inf.first();
        inf.erase(inf.begin());
        entity.toDouble(&ok);
        if(ok)
            polishNotice.push(entity);
        else if(isFunction(entity))
            stack.push(entity);
        else if(entity == "(")
            stack.push(entity);
        else if(entity == ")"){
            while(!stack.isEmpty() && stack.last() != "("){
                polishNotice.push(stack.pop());
            }
            if(stack.isEmpty())
                throw SyntaxError();
            stack.pop();
        }
        else if(isOperator(entity)){
            while(!stack.isEmpty() && getPriority(entity) <= getPriority(stack.last())){
                polishNotice.push(stack.pop());
            }
            stack.push(entity);
        }
    }

    while(!stack.isEmpty()){
        if(!(isOperator(stack.last()) || isFunction(stack.last())))
            throw SyntaxError();
        polishNotice.push(stack.last());
        stack.erase(stack.end() - 1);
    }
    return polishNotice;
}

Parser::StringStack Parser::splitToSimples(const QString &notice)
{
    StringStack list;
    QString word = "";
    foreach (QChar ch, notice){
        if(QRegExp("[0-9.]").exactMatch(ch) && QRegExp("[0-9.]*").exactMatch(word))
            word.append(ch);
        else if(QRegExp("[a-z]").exactMatch(ch) && QRegExp("[a-z]*").exactMatch(word))
            word.append(ch);
        else if(!word.isEmpty()){
            list.push(word);
            list.push(ch);
            word = "";
        }
        else
            list.push(ch);
    }
    if(!word.isEmpty())
        list.push(word);
    return list;
}
