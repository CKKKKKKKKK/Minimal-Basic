#include "statement.h"
Statement::Statement(QString input,int n,Statement *p)
{
    text=input;
    lineNumber=n;
    next=p;
}

Statement::~Statement()
{

}

void Statement::run(QMap<QString,int> *&currentVariables,QString *&output)
{
    output=nullptr;
    return;
}

QString Statement::statementType()
{
    return "statement";
}

RemStatement::RemStatement(QString input,int n,Statement *p)
    :Statement(input,n,p)
{
    type="remark";
}

RemStatement::~RemStatement()
{

}

QString RemStatement::statementType()
{
    return type;
}

void RemStatement::run(QMap<QString,int> *&currentVariables,QString *&output)
{
    output=nullptr;
    return;
}

LetStatement::LetStatement(QString input,int n,Statement *p)
    :Statement (input,n,p)
{
    type="let";
}

LetStatement::~LetStatement()
{

}

QString LetStatement::statementType()
{
    return type;
}

void LetStatement::run(QMap<QString,int> *&currentVariables,QString *&output)
{
    Tokenizer tokenzier;
    Parser parser(currentVariables);
    QString variable;
    QStringList splitedInput=tokenzier.token(text);
    QStringList expression;
    for(int i=0;i<splitedInput.size();++i)
    {
        if(splitedInput[i]=="LET")
        {
            for(int j=i+1;j<splitedInput.size();++j)
            {
                if(splitedInput[j]==" ")    {continue;}
                variable=splitedInput[j];
                break;
            }
        }
        if(splitedInput[i]=="=")
        {
            for(int j=i+1;j<splitedInput.size();++j)
            {
                expression.append(splitedInput[j]);
            }
        }
    }

    /*Miss variable or value*/
    if(variable.size()==0 || expression.size()==0)
    {
        QString error="Invalid input of let statement";
        throw error;
    }

    else
    {
        try
        {
            Expression *expressionTree=parser.parse(expression);
            int value=parser.evaluate(expressionTree);
            if(!currentVariables->contains(variable))   {currentVariables->insert(variable,value);}
            else {
                currentVariables->remove(variable);
                currentVariables->insert(variable,value);
            }
            output=nullptr;
            return;
        }
        catch(QString error)
        {
            throw std::move(error)+" in line "+QString::number(lineNumber);
        }
    }

}

PrintStatement::PrintStatement(QString input,int n,Statement *p)
    :Statement (input,n,p)
{
    type="print";
}

PrintStatement::~PrintStatement()
{

}

QString PrintStatement::statementType()
{
    return type;
}

void PrintStatement::run(QMap<QString,int> *&currentVariables,QString *&output)
{
    Tokenizer tokenzier;
    Parser parser(currentVariables);
    QString toHandle;
    for(int i=0;i<text.size();++i)
    {
        if(text[i]=="P")    {toHandle=text.mid(i+6);break;}
    }
    QStringList tokenedResult=tokenzier.token(toHandle);
    try
    {
        Expression *expressionTree=parser.parse(tokenedResult);
        QString value=QString::number(parser.evaluate(expressionTree));
        output=new QString;
        *output=value;
        return;
    }
    catch(QString error)
    {
        throw std::move(error)+" in line "+QString::number(lineNumber);
    }
}

GotoStatement::GotoStatement(QString input,int n,Statement *p)
    :Statement(input,n,p)
{
    type="goto";
}

GotoStatement::~GotoStatement()
{

}

QString GotoStatement::statementType()
{
    return type;
}

void GotoStatement::run(QMap<QString,int> *&currentVariables,QString *&output)
{
    return;
}

IfStatement::IfStatement(QString input,int n,Statement *p)
    :Statement (input,n,p)
{
    type="if";
}

IfStatement::~IfStatement()
{

}

QString IfStatement::statementType()
{
    return type;
}

void IfStatement::run(QMap<QString,int> *&currentVariables,QString *&output)
{
    return;
}

EndStatement::EndStatement(QString input,int n,Statement *p)
    :Statement(input,n,p)
{
    type="end";
}

EndStatement::~EndStatement()
{

}

QString EndStatement::statementType()
{
    return type;
}

void EndStatement::run(QMap<QString,int> *&currentVariables,QString *&output)
{
    return;
}

InputStatement::InputStatement(QString input,int n,Statement *p)
    :Statement(input,n,p)
{
    type="input";
}

InputStatement::~InputStatement()
{

}

QString InputStatement::statementType()
{
    return type;
}

void InputStatement::run(QMap<QString,int> *&currentVariables,QString *&output)
{
    return;
}
