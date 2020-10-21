#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <QString>

class Expression
{
public:
    Expression();
    virtual ~Expression();
    virtual int eval();
    virtual QString expType();
};

class ConstantExp: public Expression
{
public:
    ConstantExp(int v);
    ~ConstantExp()override;
    int eval()override;
    QString expType()override;
private:
    int value;
};

class IdentifierExp: public Expression
{
public:
    IdentifierExp(QString n,int v);
    ~IdentifierExp()override;
    int eval()override;
    QString expType()override;
private:
    QString name;
    int value;
};

class CompoundExp: public Expression
{
public:
    CompoundExp(QString o,Expression *left=nullptr,Expression *right=nullptr);
    ~CompoundExp()override;
    int eval()override;
    QString expType()override;
private:
    Expression *leftExp;
    Expression *rightExp;
    int value;
    QString op;
};

#endif // EXPRESSION_H
