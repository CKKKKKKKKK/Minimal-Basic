#ifndef STATEMENT_H
#define STATEMENT_H
#include <QString>
#include "expression.h"
#include "parser.h"
#include "tokenizer.h"

class Statement
{
public:
    friend class Program;
    Statement(QString input,int n=0,Statement *p=nullptr);
    virtual ~Statement();
    virtual void run(QMap<QString,int> *&currentVariables,QString *&output);
    virtual QString statementType();
protected:
    int lineNumber;
    QString text;
    Statement *next;
};

class RemStatement: public Statement
{
public:
    RemStatement(QString input,int n=0,Statement *p=nullptr);
    ~RemStatement() override;
    QString statementType() override;
    void run(QMap<QString,int> *&currentVariables,QString *&output) override;
private:
    QString type;
};

class LetStatement: public Statement
{
public:
    LetStatement(QString input,int n=0,Statement *p=nullptr);
    ~LetStatement() override;
    QString statementType()override;
    void run(QMap<QString,int> *&currentVariables,QString *&output) override;
private:
    QString type;
};

class PrintStatement: public Statement
{
public:
    PrintStatement(QString input,int n=0,Statement *p=nullptr);
    ~PrintStatement() override;
    QString statementType()override;
    void run(QMap<QString,int> *&currentVariables,QString *&output) override;
private:
    QString type;
};

class GotoStatement: public Statement
{
public:
    GotoStatement(QString input,int n=0,Statement *p=nullptr);
    ~GotoStatement() override;
    QString statementType() override;
    void run(QMap<QString,int> *&currentVariables,QString *&output) override;
private:
    QString type;
};

class IfStatement: public Statement
{
public:
    IfStatement(QString input,int n=0,Statement *p=nullptr);
    ~IfStatement() override;
    QString statementType() override;
    void run(QMap<QString,int> *&currentVariables,QString *&output) override;
private:
    QString type;
};

class EndStatement: public Statement
{
public:
    EndStatement(QString input,int n=0,Statement *p=nullptr);
    ~EndStatement() override;
    QString statementType() override;
    void run(QMap<QString,int> *&currentVariables,QString *&output) override;
private:
    QString type;
};

class InputStatement: public Statement
{
public:
    InputStatement(QString input,int n=0,Statement *p=nullptr);
    ~InputStatement() override;
    QString statementType() override;
    void run(QMap<QString,int> *&currentVariables,QString *&output) override;
private:
    QString type;
};

#endif // STATEMENT_H
