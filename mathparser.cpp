#include "mathparser.h"

MathParser::MathParser()
{
    operators_priority.insert("^",2);
    operators_priority.insert("*",4);
    operators_priority.insert("/",4);
    operators_priority.insert("+",6);
    operators_priority.insert("-",6);
    operators_priority.insert("(",8);
    operators_priority.insert(")",8);
}

int MathParser::Parse(QString str)
{
    stek.clear();
    output.clear();
    QString curr_str;
    Token next_token;
    Token last_t;
    bool can_be_unar = true;
    bool success = false;
    curr_str = str;
    while (curr_str.length()!= 0)
    {
        next_token = ParseToken(curr_str, can_be_unar);
        switch (next_token.GetType()) {
        case -1:
            return 0;
        case 0:
            output.append(next_token);
            break;
        case 3:
            stek.append(next_token);
            break;
        case 5:
            success = false;
            while (stek.size() != 0)
            {
                last_t = stek.last();
                if (last_t.GetData()=="(")
                {
                    success = true;
                    break;
                }
                output.append(last_t);
                stek.pop_back();
            }
            if (!success)
                return 0;
            break;
        case 1:
            while (stek.size() != 0)
            {
                last_t = stek.last();
                if (((last_t.GetType()==1)||(last_t.GetType()==2))&&
                        (operators_priority[next_token.GetData()]>=operators_priority[last_t.GetData()]))
                {
                    output.append(last_t);
                    stek.pop_back();
                }
                else
                    break;
            }
            stek.append(next_token);
            break;
        case 2:
            while (stek.size() != 0)
            {
                last_t = stek.last();
                if (((last_t.GetType()==1)||(last_t.GetType()==2))&&
                        (operators_priority[next_token.GetData()]>operators_priority[last_t.GetData()]))
                {
                    output.append(last_t);
                    stek.pop_back();
                }
                else
                    break;
            }
            stek.append(next_token);
            break;
        case 4:
            if (next_token.GetData()=="(")
            {
                stek.append(next_token);
                break;
            }
            success = false;
            while (stek.size() != 0)
            {
                last_t = stek.last();
                if (last_t.GetData()=="(")
                {
                    stek.pop_back();
                    success = true;
                    break;
                }
                output.append(last_t);
                stek.pop_back();
            }
            if (!success)
                return 0;
            break;
        }
    }
    while (stek.size() != 0)
    {
        last_t = stek.last();
        if (last_t.GetType()==4)
        {
            return 0;
        }
        output.append(last_t);
        stek.pop_back();
    }

    //for (int i =0; i<output.size(); i++)
    //    qDebug() << output[i].GetType() << output[i].GetData();

    return 1;


        //-1 - ошибка
        //0 - число
        //1 - операция бинарная
        //2 - операция унарная
        //3 - функция
        //4 = скобка
        //5 = запятая
}


double MathParser::Calculate(bool &error)
{
    stek.clear();
    double second;
    double first;
    QString oper;
    for (int i =0; i<output.size(); i++)
        switch (output[i].GetType()) {
        case 0:
            stek.append(output[i]);
            break;
        case 1:
            if (stek.size()<2)
            {
                error = true;
                return 0.0;
            }
            oper = output[i].GetData();
            second = stek.takeLast().GetData().toDouble();
            first = stek.takeLast().GetData().toDouble();
            if (oper == "+")
            {
                stek.append(Token(0, QString::number(first+second)));
            }
            else if (oper == "-")
            {
                stek.append(Token(0, QString::number(first-second)));
            }
            else if (oper == "*")
            {
                stek.append(Token(0, QString::number(first*second)));
            }
            else if (oper == "/")
            {
                stek.append(Token(0, QString::number(first/second)));
            }
            else if (oper == "^")
            {
                stek.append(Token(0, QString::number(pow(first,second))));
            }
            break;
        case 2:
            if (stek.size()<1)
            {
                error = true;
                return 0.0;
            }
            oper = output[i].GetData();
            first = stek.takeLast().GetData().toDouble();
            if (oper == "+")
            {
                stek.append(Token(0, QString::number(first)));
            }
            else if (oper == "-")
            {
                stek.append(Token(0, QString::number((-1)*first)));
            }
            break;
    }
    if (stek.size()!=1)
    {
        error = true;
        return 0.0;
    }
    else
        return stek.last().GetData().toDouble();
}



Token MathParser::ParseToken(QString &input, bool &can_be_unar)
{
    int token_size = 0;
    int type = -1;
    QString data;
    //если число
    if (input.at(token_size).isDigit())
    {
        int dot_cnt = 0;
        // разрешаем только цифры и точку
        while (token_size < input.length() &&
               ((input.at(token_size).isDigit()) || input.at(token_size) == '.'))
        {
            // но также проверям, что в числе может быть только одна точка!
            if (input.at(token_size) == '.' && ++dot_cnt > 1) {
                return Token(-1, "");
            }
            token_size++;
        }
        data = input.mid(0, token_size);
        type = 0;
        can_be_unar = false;
    }
    else if (input.at(token_size).isLetter())
    {
        while (token_size < input.length() && (input.at(token_size).isLetter()))
            token_size++;
        data = input.mid(0, token_size);
        type = 3;
        can_be_unar = false;
    }
    else if ((input.at(token_size) == '+')||(input.at(token_size) == '-'))
    {
        data = input.mid(0, 1);
        if (can_be_unar)
            type = 2;
        else
            type = 1;
        can_be_unar = false;
        token_size++;
    }
    else if ((input.at(token_size) == '*')||(input.at(token_size) == '/')
             || (input.at(token_size) == '^'))
    {
        data = input.mid(0, 1);
        type = 1;
        can_be_unar = false;
        token_size++;
    }
    else if (input.at(token_size) == '(')
    {
        data = input.mid(0, 1);
        type = 4;
        can_be_unar = true;
        token_size++;
    }
    else if (input.at(token_size) == ')')
    {
        data = input.mid(0, 1);
        type = 4;
        can_be_unar = false;
        token_size++;
    }
    else if (input.at(token_size) == ',')
    {
        data = input.mid(0, 1);
        type = 5;
        can_be_unar = true;
        token_size++;
    }
    input = input.mid(token_size);
    return Token(type, data);
}






