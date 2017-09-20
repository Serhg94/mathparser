#ifndef TOKEN_H
#define TOKEN_H

#include <QObject>

class Token
{
private:
    //-1 - ошибка
    //0 - число
    //1 - операция бинарная
    //2 - операция унарная
    //3 - функция
    //4 = скобка
    //5 = запятая
    int type;
    QString data;
public:
    Token()
    {
        type = -1;
    }
    Token(int t, QString d)
    {
        type = t;
        data = d;
    }
    int GetType()
    {
        return type;
    }
    void SetType(int t)
    {
        type = t;
    }
    QString GetData()
    {
        return data;
    }
    void SetData(QString d)
    {
        data = d;
    }
};

#endif // TOKEN_H
