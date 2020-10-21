#include "program.h"
#include <QStringList>

Variable::Variable(QString n,int v)
{
    name=n;
    value=v;
}

Variable::Variable()
{

}

Variable::~Variable()
{

}

Program::Program()
{
    global_Variables=new QMap<QString,int>;
    head=nullptr;
    totalLine=0;
    minLine=0;
    maxLine=0;
    statementOutput=nullptr;
    programCounter=-1;
    endFlag=false;
}

Program::~Program()
{
    while(head!=nullptr)
    {
        Statement *p=head;
        head=p->next;
        delete p;
    }
}

bool Program::insertLine(QString input)
{
    Statement* currentStatement=nullptr;
    Tokenizer tokenizer;
    QStringList list=tokenizer.token(input);
    int number=list[0].toInt();
    if(list[1]=="REM")
    {
        currentStatement=new RemStatement(input,number,nullptr);
    }
    else if(list[1]=="LET")
    {
        currentStatement=new LetStatement(input,number,nullptr);
    }
    else if(list[1]=="PRINT")
    {
        currentStatement=new PrintStatement(input,number,nullptr);
    }
    else if(list[1]=="GOTO")
    {
        currentStatement=new GotoStatement(input,number,nullptr);
    }
    else if(list[1]=="IF"&&list[list.size()-2]=="THEN")
    {
        currentStatement=new IfStatement(input,number,nullptr);
    }
    else if(list[1]=="END")
    {
        currentStatement=new EndStatement(input,number,nullptr);
    }
    else if(list[1]=="INPUT")
    {
        currentStatement=new InputStatement(input,number,nullptr);
    }
    else
    {
        return false;
    }
    if(totalLine==0)
    {
        head=currentStatement;
        minLine=number;
        maxLine=number;
        ++totalLine;
        programCounter=number;
        return true;
    }
    if(number<minLine)
    {
        Statement *p=head;
        head=currentStatement;
        currentStatement->next=p;
        minLine=number;
        programCounter=minLine;
        ++totalLine;
        return true;
    }
    if(number>maxLine)
    {
        Statement *p=head;
        while(p->next!=nullptr) {p=p->next;}
        p->next=currentStatement;
        maxLine=number;
        ++totalLine;
        return true;
    }

    /*The case that the inserted line is not the max line or min line in the program*/
    Statement *p=head;
    while(p!=nullptr)
    {
        if(p->lineNumber==number)
        {
            p->text=input;
            return true;
        }
        if(p->lineNumber<number && p->next->lineNumber>number)
        {
            Statement *q=p->next;
            p->next=currentStatement;
            currentStatement->next=q;
            ++totalLine;
            return true;
        }
        p=p->next;
    }
    return false;
}

int Program::list(QString *&p)
{
    p=new QString[totalLine];
    Statement *ptr=head;
    for(int i=0;i<totalLine;++i)
    {
        p[i]=ptr->text;
        ptr=ptr->next;
    }
    return totalLine;
}

void Program::clear()
{
    while(head!=nullptr)
    {
        Statement *p=head;
        head=p->next;
        delete p;
    }
    totalLine=0;
    minLine=0;
    maxLine=0;
    programCounter=0;
}

int Program::totalLineNumber()
{
    return totalLine;
}

int Program::returnValue(QString v)
{
    if(global_Variables->contains(v))
    {
        int result=global_Variables->value(v);
        return result;
    }
    else
    {
        throw "Undefined reference to "+v;
    }
}

void Program::run(int *&inputStream,QString *&outputStream)
{
    Statement *p=findLine(programCounter);
    while(p!=nullptr)
    {
        if(p->statementType()=="remark")
        {
            p=p->next;
            if(p)  {programCounter=p->lineNumber;}
            continue;
        }
        if(p->statementType()=="let")
        {
            try
            {
                p->run(global_Variables,statementOutput);
            }
            catch(QString error)
            {
                throw std::move(error);
            }
            p=p->next;
            if(p)  {programCounter=p->lineNumber;}
            continue;
        }

        if(p->statementType()=="print")
        {
            if(!outputStream)
            {
                try
                {
                    p->run(global_Variables,statementOutput);

                }
                catch(QString error)
                {
                    throw std::move(error);
                }
                outputStream=new QString;
                *outputStream=*statementOutput;
                return;
            }
            else if(outputStream)
            {
                outputStream=nullptr;
                p=p->next;
                if(p)  {programCounter=p->lineNumber;}
                continue;
            }
        }

        if(p->statementType()=="goto")
        {
            Tokenizer tokenizer;
            QString result=tokenizer.token(p->text)[2];
            if(findLine(result.toInt())!=nullptr)  {programCounter=result.toInt();p=findLine(programCounter);}
            else {
                throw "Line "+result+" does not exist"+" in line "+QString::number(p->lineNumber);
            }
            continue;
        }

        if(p->statementType()=="if")
        {
            if(p->next)   {programCounter=p->next->lineNumber;}
            Tokenizer tokenzier;
            QStringList splitResult=tokenzier.token(p->text);
            QString expression;
            for(int i=0;i<splitResult.size();++i)
            {
                if(splitResult[i]=="IF")
                {
                    for(int j=i+1;j<splitResult.size();++j)
                    {
                        expression.append(splitResult[j]);
                        if(splitResult[j+1]=="THEN") {break;}
                    }
                    break;
                }
            }
            Tokenizer spliter;
            Parser parser(global_Variables);
            QStringList expression_Tokened=spliter.token(expression);
            try
            {
                Expression *expressionTree =parser.parse(expression_Tokened);
                int value=parser.evaluate(expressionTree);
                int jumpTarget=splitResult[splitResult.size()-1].toInt();
                if(value!=0)   {programCounter=jumpTarget;}
                if(findLine(programCounter))    {p=findLine(programCounter);}
                continue;
            }
            catch(QString error)
            {
                throw error+" in line "+QString::number(p->lineNumber);
            }

        }

        if(p->statementType()=="end")
        {
            programCounter=minLine;
            p=findLine(programCounter);
            endFlag=true;
            break;
        }

        if(p->statementType()=="input")
        {
            /*The second time step into*/
            if(inputStream)
            {
                outputStream=nullptr;
                Tokenizer tokenizer;
                QString variable=tokenizer.token(p->text)[2];
                int value=*inputStream;
                if(!global_Variables->contains(variable))   {global_Variables->insert(variable,value);}
                else {
                    global_Variables->remove(variable);
                    global_Variables->insert(variable,value);
                }
                p=p->next;
                if(p)   {programCounter=p->lineNumber;}
                continue;
            }

            /*The first time step into*/
            else if(!inputStream)
            {
                outputStream=new QString;
                *outputStream="?";
                return;
            }
        }
    }
}

Statement* Program::findLine(int lineNumber)
{
    Statement *p=head;
    while(p!=nullptr)
    {
        if(p->lineNumber==lineNumber)    {return p;}
        p=p->next;
    }
    return nullptr;
}
