#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QTextEdit>
#include <QWidget>
#include "program.h"

/*The interface between the user and the interpreter.*/

class Console : public QTextEdit
{
    Q_OBJECT
public:
    explicit Console(QWidget *parent = nullptr);
    QString userText;
signals:
    void newLineWritten(QString newline);
public slots:
    void write(QString msg);
private:
    void handle_input();
    void showHelp();
    void list();
    void quit() __attribute__((noreturn));
    void run();
    /*The input flag for program statement*/
    bool waitInput;
    /*The input flag for immediate command*/
    bool waitQuickInput;

    int currentLineNumber;
    /*Used for transfer value into the program*/
    int *inputProgram;
    /*To store the name of quick input variable*/
    QString quickInputVariable;
    /*To get the output stream from program*/
    QString *programOutput;

    Program program;
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};

#endif // CONSOLE_H
