#ifndef MATHPARSER_H
#define MATHPARSER_H

#include <QObject>
#include <QDebug>
#include "token.h"

class MathParser
{
private:
    QHash<QString, QString> operators;
    QList<Token> stek;
    QList<Token> output;
    Token ParseToken(QString &input, bool &can_be_unar);
public:
    QHash<QString, int> operators_priority;
    MathParser();
    int Parse(QString s);
    double Calculate(bool &error);
};

#endif // MATHPARSER_H
