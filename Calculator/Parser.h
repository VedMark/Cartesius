#pragma once

#include <QStack>
#include <QString>

class CalculationTreeModel;

class SyntaxError{};

class Parser
{
    typedef QStack<QString> StringStack;

public:
    enum Priority{VERY_LOW, LOW, MID, HIGH, EXTRA};
    explicit Parser();
    static qreal calcucateExpression(const QString&);

    static bool isOperator(QString);
    static bool isFunction(QString);
    static Priority getPriority(QString);

private:
    static StringStack stringToPolishNotice(const QString &);
    static StringStack splitToSimples(const QString &);
};
