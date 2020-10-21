#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <QString>
#include <QStringList>
/*
 * This is a tool used to split the expression into several tokens.
 * It splits according to operator and space.
 * It will omit every space before finally returning the result.
 * It can also be used to handle user's input statement or command.
*/

class Tokenizer
{
public:
    Tokenizer();
    ~Tokenizer();
    QStringList token(QString input);
};

#endif // TOKENIZER_H
