#include "expression.h"

Expression::Expression()
{

}

Expression::~Expression()
{

}

int Expression::eval()
{
    return -1;
}

QString Expression::expType()
{
    return "expression";
}

ConstantExp::ConstantExp(int v)
{
    value=v;
}

ConstantExp::~ConstantExp()
{

}

int ConstantExp::eval()
{
    return value;
}

QString ConstantExp::expType()
{
    return "constant";
}

IdentifierExp::IdentifierExp(QString n,int v)
{
    name=n;
    value=v;
}

IdentifierExp::~IdentifierExp()
{

}

int IdentifierExp::eval()
{
    return value;
}

QString IdentifierExp::expType()
{
    return "identifier";
}

CompoundExp::CompoundExp(QString o,Expression *left,Expression *right)
{
    op=o;
    leftExp=left;
    rightExp=right;
    if(left==nullptr || right==nullptr)
    {
        throw "Invalid input of expression";
    }

    /*Use recursive way to traverse the expression tree*/
    else
    {
        if(op=="+") {value=leftExp->eval()+rightExp->eval();}
        else if(op=="-")    {value=rightExp->eval()-leftExp->eval();}
        else if(op=="*")    {value=leftExp->eval()*rightExp->eval();}
        else if(op=="/")    {value=rightExp->eval()/leftExp->eval();}
        else if(op=="==")   {value=(leftExp->eval()==rightExp->eval());}
        else if(op=="**")
        {
            long long tmpValue=1;
            for(int i=0;i<leftExp->eval();++i)
            {
                tmpValue*=rightExp->eval();

                /*Judge overflow*/
                if(tmpValue>2147483647||tmpValue<-2147483648)
                {
                    QString error="Out of range of int";
                    throw error;
                }

            }
            value=static_cast<int>(tmpValue);
        }
        else if(op==">")    {value=(rightExp->eval()>leftExp->eval());}
        else if(op=="<")    {value=(rightExp->eval()<leftExp->eval());}
        else if(op==">=")   {value=(rightExp->eval()>=leftExp->eval());}
        else if(op=="<=")   {value=(rightExp->eval()<=leftExp->eval());}
    }

}

CompoundExp::~CompoundExp()
{

}

int CompoundExp::eval()
{
    return value;
}

QString CompoundExp::expType()
{
    return "compound";
}
