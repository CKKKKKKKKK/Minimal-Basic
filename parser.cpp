#include "parser.h"

Parser::Parser(QMap<QString,int> *p)
{
    currentVariables=p;
}

Parser::~Parser()
{

}

Expression* Parser::parse(QStringList input)
{
    Expression *result=nullptr;
    Expression *currentValue=nullptr;
    int currentToken=0;
    while(currentToken<input.size())
    {
        /*Handle the case that '-' is the first character in the expression*/
        if(currentToken==0 && input[currentToken]=="-")
        {
            currentValue=new ConstantExp(0);
            operand_Stack.push(currentValue);
            operator_Stack.push(input[currentToken]);
            ++currentToken;
            continue;
        }

        if(isConstant(input[currentToken]))
        {
            long long tmpValue;
            tmpValue=input[currentToken].toLongLong();
            if(tmpValue>2147483647||tmpValue<-2147483648)
            {
                QString error="Out of range of int";
                throw error;
            }
            currentValue=new ConstantExp(static_cast<int>(tmpValue));
            operand_Stack.push(currentValue);
            ++currentToken;
            continue;
        }

        else if(isOperator(input[currentToken]))
        {
            if(operator_Stack.isEmpty())
            {
                operator_Stack.push(input[currentToken]);
                ++currentToken;
                continue;
            }
            handleOperator(input,currentToken);
            ++currentToken;
            continue;
        }

        /*The case of an variable*/
        else {
            if(!currentVariables->contains(input[currentToken]))
            {
                throw "Undefined reference to "+input[currentToken];
            }
            else
            {
                int value=currentVariables->value(input[currentToken]);
                currentValue=new IdentifierExp(input[currentToken],value);
                operand_Stack.push(currentValue);
                ++currentToken;
                continue;
            }
        }

    }
    while(!operator_Stack.isEmpty())
    {
        if(operand_Stack.isEmpty())
        {
            throw "Invalid input of expression";
        }
        else
        {
            Expression *firstExp=operand_Stack.pop();
            if(operand_Stack.isEmpty())
            {
                throw "Invalid input of expression";
            }
            else
            {
                Expression *secondExp=operand_Stack.pop();
                QString topOperator=operator_Stack.pop();
                currentValue=new CompoundExp(topOperator,firstExp,secondExp);
                operand_Stack.push(currentValue);
            }
        }
    }
    if(operand_Stack.isEmpty())
    {
        throw "Invalid input of expression";
    }
    else
    {
        result=operand_Stack.pop();
    }
    return result;
}


int Parser::evaluate(Expression *expression)
{
    try
    {
        return expression->eval();
    }
    catch(QString error)
    {
        throw std::move(error);
    }
}

bool Parser::isConstant(QString input)
{
    for(int i=0;i<input.size();++i)
    {
        if(!(input[i]>='0' && input[i]<='9'))   {return false;}
    }
    return true;;
}

bool Parser::isOperator(QString input)
{
    if(input=="(")  {return true;}
    if(input==")")  {return true;}
    if(input=="+")  {return true;}
    if(input=="-")  {return true;}
    if(input=="*")  {return true;}
    if(input=="/")  {return true;}
    if(input=="%")  {return true;}
    if(input=="=")  {return true;}
    if(input==">")  {return true;}
    if(input=="<")  {return true;}
    if(input==">=") {return true;}
    if(input=="<=") {return true;}
    if(input=="!=") {return true;}
    if(input=="==") {return true;}
    if(input=="**") {return true;}
    return false;
}

void Parser::handleOperator(QStringList input,int currentOperator)
{
    Expression *currentExpression=nullptr;
    if(input[currentOperator]=="(") {operator_Stack.push(input[currentOperator]);return;}

    /*Top priority*/
    if(input[currentOperator]=="**")    {operator_Stack.push(input[currentOperator]);return;}

    if(input[currentOperator]=="*"||input[currentOperator]=="/")
    {
        while((!operator_Stack.isEmpty())&&(operator_Stack.top()=="**"||operator_Stack.top()=="*"||operator_Stack.top()=="/"))
        {
            Expression *firstExp=operand_Stack.pop();
            Expression *secondExp=operand_Stack.pop();
            QString topOperator=operator_Stack.pop();
            currentExpression=new CompoundExp(topOperator,firstExp,secondExp);
            operand_Stack.push(currentExpression);
        }
        operator_Stack.push(input[currentOperator]);
        return;
    }

    if(input[currentOperator]=="+"||input[currentOperator]=="-")
    {
        while((!operator_Stack.isEmpty())&&(operator_Stack.top()=="**"||operator_Stack.top()=="+"||operator_Stack.top()=="-"||operator_Stack.top()=="*"||operator_Stack.top()=="/"))
        {
            if(operand_Stack.isEmpty())
            {
                throw "Invalid input of expression";
            }
            else
            {
                Expression *firstExp=operand_Stack.pop();
                if(operand_Stack.isEmpty())
                {
                    throw "Invalid input of expression";
                }
                else
                {
                    Expression *secondExp=operand_Stack.pop();
                    QString topOperator=operator_Stack.pop();
                    currentExpression=new CompoundExp(topOperator,firstExp,secondExp);
                    operand_Stack.push(currentExpression);
                }
            }
        }
        operator_Stack.push(input[currentOperator]);
        return;
    }

    if(input[currentOperator]==")")
    {
        while((!operator_Stack.isEmpty())&&(operator_Stack.top()!="("))
        {
            if(operand_Stack.isEmpty())
            {
                throw "Invalid input of expression";
            }
            else
            {
                Expression *firstExp=operand_Stack.pop();
                if(operand_Stack.isEmpty())
                {
                    throw "Invalid input of expression";
                }
                else
                {
                    Expression *secondExp=operand_Stack.pop();
                    QString topOperator=operator_Stack.pop();
                    currentExpression=new CompoundExp(topOperator,firstExp,secondExp);
                    operand_Stack.push(currentExpression);
                }
            }
        }
        operator_Stack.pop();
        return;
    }

    if(input[currentOperator]=="=")
    {
        while((!operator_Stack.isEmpty())&&(operator_Stack.top()=="**"||operator_Stack.top()=="+"||operator_Stack.top()=="-"||operator_Stack.top()=="*"||operator_Stack.top()=="/"||operator_Stack.top()=="="))
        {
            if(operand_Stack.isEmpty())
            {
                throw "Invalid input of expression";
            }
            else
            {
                Expression *firstExp=operand_Stack.pop();
                if(operand_Stack.isEmpty())
                {
                    throw "Invalid input of expression";
                }
                else
                {
                    Expression *secondExp=operand_Stack.pop();
                    QString topOperator=operator_Stack.pop();
                    currentExpression=new CompoundExp(topOperator,firstExp,secondExp);
                    operand_Stack.push(currentExpression);
                }
            }
        }
        operator_Stack.push(input[currentOperator]);
        return;
    }

}

