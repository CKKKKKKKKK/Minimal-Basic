#include "tokenizer.h"

Tokenizer::Tokenizer()
{

}

Tokenizer::~Tokenizer()
{

}

QStringList Tokenizer::token(QString input)
{
    QStringList result;
    QStringList handledResult;
    while(input.size())
    {
        for(int i=0;i<=input.size();++i)
        {
            /*The case of the last character*/
            if(i==input.size())
            {
                result.append(input);
                for(int i=0;i<result.size();++i)
                {
                    if(result[i]!=" ")  {handledResult.append(result[i]);}
                }
                return handledResult;
            }

            if((input[i]=="*"&&input[i+1]=="*")||(input[i]=="="&&input[i+1]=="=")||(input[i]==">"&&input[i+1]=="=")||(input[i]=="<"&&input[i+1]=="=")||(input[i]=="!"&&input[i+1]=="="))
            {
                if(i!=0)
                {
                    result.append(input.left(i));
                }
                result.append(input.mid(i,2));
                input=input.mid(i+2);
                break;
            }
            if((input[i]=="("&&input[i+1]=="-")||(input[i]=="="&&input[i+1]=="-"))
            {
                if(i!=0)
                {
                    result.append(input.left(i));
                }
                result.append(input.mid(i,1));
                result.append("0");
                result.append(input.mid(i+1,1));
                input=input.mid(i+2);
                break;
            }
            if(input[i]==" "||input[i]=="+"||input[i]=="-"||input[i]=="*"||input[i]=="/"||input[i]=="%"||input[i]=="="||input[i]=="<"||input[i]==">"||input[i]=="("||input[i]==")")
            {
                if(i!=0)
                {
                    result.append(input.left(i));
                }
                result.append(input.mid(i,1));
                input=input.mid(i+1);
                break;
            }
        }
    }
    /*Omit spaces*/
    for(int i=0;i<result.size();++i)
    {
        if(result[i]!=" ")  {handledResult.append(result[i]);}
    }
    return handledResult;
}
