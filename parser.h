#ifndef PARSER_H
#define PARSER_H
#include <QStringList>
#include <QString>
#include <QStack>
#include <QMap>
#include "expression.h"
/*
 * This is a tool used to construct the expression tree according to the QStringList input into it.
 * It is also used to calculate the value of an expression while constructing the expression tree.
 */

class Parser
{
public:
    Parser(QMap<QString,int> *p);
    ~Parser();
    Expression* parse(QStringList input);
    int evaluate(Expression *expression);
private:
    QStack<QString> operator_Stack;
    QStack<Expression*> operand_Stack;
    QMap<QString,int> *currentVariables;
    inline bool isConstant(QString input);
    inline bool isOperator(QString input);
    inline void handleOperator(QStringList input,int currentOperator);
};

#endif // PARSER_H
