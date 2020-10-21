#include "Console.h"
#include <QKeyEvent>
#include <QTextLine>
#include <QTextCursor>
#include <QStringList>

Console::Console(QWidget *parent) : QTextEdit(parent)
{
    currentLineNumber=0;
    inputProgram=nullptr;
    programOutput=nullptr;
    waitInput=false;
    waitQuickInput=false;
    write("Minimal BASIC -- Type HELP for help. Type RUN to run your code. Type LIST to print all your code. Type CLEAR to clear the screen and your code. Type QUIT to quit.\n\n");
}

void Console::write(QString msg)
{
    ++currentLineNumber;
    this->append(msg);
}

void Console::keyPressEvent(QKeyEvent *event)
{
    if (this->textCursor().hasSelection())
        return;
    if (event->key() == Qt::Key_Return) {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition(QTextCursor::End);
        cursor.select(QTextCursor::LineUnderCursor);
        QString lastLine = cursor.selectedText();
        newLineWritten(lastLine);
        userText=lastLine;
        handle_input();
    }
    QTextEdit::keyPressEvent(event);
}

void Console::handle_input()
{
    Tokenizer tokenizer;
    QStringList sList=tokenizer.token(userText);
    if(userText.toLower()=="help")  {showHelp();return;}
    else if(userText.toLower()=="list")  {Console::list();return;}
    else if(userText.toLower()=="quit")  {quit();}
    else if(userText.toLower()=="clear") {clear();program.clear();return;}
    else if(userText.toLower()=="run")
     {
        try
        {
            run();
        }
        catch(QString error)
        {
            write(error);
        }
        return;
    }

    else if(waitInput)
    {
        bool *ok= new bool;
        int value=userText.toInt(ok);
        if(*ok==true)
        {
            inputProgram=new int;
            *inputProgram=value;
            waitInput=false;
            run();
        }
        else {
            write("Please input an integer");
        }
        return;
    }

    else if(waitQuickInput)
    {
        bool *ok= new bool;
        int value=userText.toInt(ok);
        if(*ok==true)
        {
            waitQuickInput=false;

            /*Insert the input variable, if it already exists, cover it*/
            if(!program.global_Variables->contains(quickInputVariable))   {program.global_Variables->insert(quickInputVariable,value);}
            else {
                program.global_Variables->remove(quickInputVariable);
                program.global_Variables->insert(quickInputVariable,value);
            }

            quickInputVariable=nullptr;
        }
        else {
            write("Please input an integer");
        }
        return;
    }

    else if(waitQuickInput==false && waitInput==false && sList[0].toStdString()[0]>='0' && sList[0].toStdString()[0]<='9')
    {
        if(!program.insertLine(userText))
        {
            write("Invalid input");
        }
        return;
    }

    else if(sList[0]=="INPUT")
    {
        if(sList.size()>1)
        {
            quickInputVariable=sList[1];
            write("?");
            waitQuickInput=true;
            return;
        }
        else {
            write("Invalid input");
        }
    }

    else if(sList[0]=="PRINT")
    {
        if(sList.size()>1)
        {
            Parser parser(program.global_Variables);
            QStringList expression;
            for(int i=1;i<sList.size();++i)
            {
                expression.append(sList[i]);
            }
            try
            {
                int value=parser.evaluate(parser.parse(expression));
                write(QString::number(value));

            }
            catch(QString error)
            {
                write(error);
            }
        }
        else {
            write("Invalid input");
        }
    }

    else if(sList[0]=="LET")
    {
        if(sList.size()>3)
        {
            Parser parser(program.global_Variables);
            QString variable=sList[1];
            QStringList expression;
            for(int i=3;i<sList.size();++i)
            {
                expression.append(sList[i]);
            }
            try {
                int value=parser.evaluate(parser.parse(expression));
                if(!program.global_Variables->contains(variable))
                {
                    program.global_Variables->insert(variable,value);
                }
                else {
                    program.global_Variables->remove(variable);
                    program.global_Variables->insert(variable,value);
                }
            } catch (QString error) {
                write(error);
            }
        }
        else {
            write("Invalid input");
        }
    }

    else {
        write("Invalid input");
    }
}

void Console::showHelp()
{
    write("\n help.txt \t      For Minimal BASIC");
    write("1.Every line of statement should begin with a linenumber(except LET statement,PRINT statement and INPUT statement, which will be explained in the following text.)\n\n");
    write("2.There are totally 7 types of statements:\n");
    write("REM: The statement used for comments. Any text on the line after the keyword REM is ignored.\n");
    write("LET: This statement is BASIC’s assignment statement. The LET keyword is followed by a variable name, an equal sign, and an expression.\n ");
    write("PRINT: In minimal BASIC, the PRINT statement has the form: PRINT exp  The effect of this statement is to print the value of the expression on the console\n");
    write("INPUT: In the minimal version of the BASIC interpreter, the INPUT statement has the form: INPUT var  The effect of this statement is to print a prompt consisting of the string \"?\" and then to read in a value to be stored in the variable.\n");
    write("GOTO: This statement has the syntax GOTO n and forces an unconditional change in the control flow of the program. When the program hits this statement, the program continues from line n instead of continuing with the next statement.\n");
    write("IF: This statement provides conditional control. The syntax for this statement is: IF exp1 op exp2 THEN n  where exp1 and exp2 are expressions and op is one of the conditional operators =, <, or >. If the condition holds, the program should continue from line n just as in the GOTO statement. If not, the program continues on to the next line. \n");
    write("END: Marks the end of the program. Execution halts when this line is reached. \n\n");
    write("3.But in the case of LET statement, INPUT statement and PRINT statement, you can omit the linenumber. Then what you have input will be not record as statement,but will be taken as command to directly change the global variables.\n");
}

void Console::list()
{
    QString *programContent=nullptr;
    int totalLine=program.list(programContent);
    for(int i=0;i<totalLine;++i)
    {
        write(programContent[i]);
    }
}

void Console::quit()
{
    exit(0);
}

void Console::run()
{
    program.run(inputProgram,programOutput);
    if(program.endFlag) {program.endFlag=false;return;}
    delete inputProgram;
    if(programOutput)
    {
        write(*programOutput);
        if(*programOutput=="?") {waitInput=true;}
        else    {run();}
    }
}



