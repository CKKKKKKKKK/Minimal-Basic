#ifndef PROGRAM_H
#define PROGRAM_H
#include "statement.h"
#include "expression.h"
#include <QString>
#include <QMap>
#include <QStack>

class Variable
{
public:
    friend class Program;
    Variable(QString n,int v);
    Variable();
    ~Variable();
private:
    QString name;
    int value;
};

/*It is used to store statements by a link list*/

class Program
{
public:
    friend class Console;
    Program();
    ~Program();
    bool insertLine(QString input);
    int list(QString *&p);
    void clear();
    int totalLineNumber();
    int returnValue(QString v);
    void run(int *&inputStream,QString *&outputStream);
private:
    Statement* findLine(int lineNumber);
    QMap<QString,int> *global_Variables;
    Statement *head;
    QString *statementOutput;
    bool endFlag;
    int totalLine;
    int minLine;
    int maxLine;
    int programCounter;
};

#endif // PROGRAM_H
