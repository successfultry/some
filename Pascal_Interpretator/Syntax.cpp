#include <iostream>
#include <vector>
#include "Sem.cpp"

class Syntax
{
    Lex curr_lex;
    Parser scan;

    enum type{INT,REAL,STRING};
    type_of_lex flag;

    void H();//beginning
    void A();// data type/EOF
    void B();//identifier checking 
    void Operators();
    void Perem();
    void Const();
    void SC();

    void Expr();//Expression
    bool ID();//Identifiers branch
    bool Op();//Operations
    bool Brackets();

    Stack_Lex St;get another token
    Stack_int st_jump;
    std::vector <Lex> Poliz;

    void Begend(int contbreak = 0);//Compound operator

    void gl () // get another lexem
    {
        curr_lex = scan.get_lex();
    }

public:
    Syntax( const char * program ) : scan(program), St(100), st_jump(100) {}; 
    void analyze();
    void compute();
};

void Syntax::analyze()
{
    try
    {
        gl();
        H();
        cout << "Ok!!!" << endl;
        for (int i = 0; i < Poliz.size(); i++)
        {

    //        cout << Poliz[i] << " ";
        }

        compute();
    }
    catch(Lex lex)
    {
        cout << "Lex is not correct! " << lex;
    }
    catch(string str)
    {
        cout << str << endl;
    }

}

bool check_assingment_types(type_of_lex a, type_of_lex b)
{
    if (a == LEX_VAL_STRING && b == LEX_VAL_STRING ||
        a == LEX_VAL_INT    && b == LEX_VAL_INT    ||
        a == LEX_VAL_REAL   && b == LEX_VAL_REAL   ||
        a == LEX_VAL_REAL   && b == LEX_VAL_INT    ||
        a == LEX_VAL_INT    && b == LEX_VAL_REAL
        )
    {
        return true;
    }
    else{
        return false; //if string with int/real
    }
}

void get_values_from_lex(Lex a,
                         type_of_lex& a_type,
                         int& lex1_int,
                         double& lex1_real,
                         string& lex1_string )
{
    if (a.get_type() == LEX_VAL_INT)
    {
        lex1_int = a.get_int_value();  //int
        a_type = LEX_VAL_INT;          //type of the returned value
    }
    else if (a.get_type() == LEX_VAL_REAL)
    {
        lex1_real = a.get_real_value();
        a_type = LEX_VAL_REAL;
    }
    else if (a.get_type() == LEX_VAL_STRING)
    {
        lex1_string = TID[a.get_int_value()].get_name();
        a_type = LEX_VAL_STRING;
    }
    else if (a.get_type() == LEX_IDENT && TID[a.get_int_value()].get_type() == LEX_VAL_INT)
    {
        lex1_int = ((IntIdent*)(TID[a.get_int_value()].get_ptr()))->get_value();
        a_type = LEX_VAL_INT;
    }
    else if (a.get_type() == LEX_IDENT && TID[a.get_int_value()].get_type() == LEX_VAL_REAL)
    {
        lex1_real = ((RealIdent*)TID[a.get_int_value()].get_ptr())->get_value();
        //lex1_real = ((RealIdent*)TID[a.get_int_value()].get_ptr())->get_value();
        a_type = LEX_VAL_REAL;
    }
    else if (a.get_type() == LEX_IDENT && TID[a.get_int_value()].get_type() == LEX_VAL_STRING)
    {
        lex1_string = ((StrIdent*)TID[a.get_int_value()].get_ptr())->get_value();
        //lex1_real = ((RealIdent*)TID[a.get_int_value()].get_ptr())->get_value();
        a_type = LEX_VAL_STRING;
    }
}

void Syntax::compute()
{
    Stack_Lex comp(100);
    //cout << endl;
    //cout << endl;
    for (int i = 0; i < Poliz.size(); i++)
    {
        Lex lex = Poliz[i];

        //cout << lex << "   Poliz" << endl;
        Lex a, b;
        switch (lex.get_type()){
        case LEX_CONTINUE:
        {
            i = lex.get_int_value() - 1;
            break;
        }
        case LEX_BREAK:
        {
            comp.push(Lex(LEX_VAL_INT, 0));
            i = lex.get_int_value() - 1;
            break;
        }

        case LEX_JUMP:
        {
            int jump = lex.get_int_value();
            i++;
            Lex lex = Poliz[i]; // take following element
            if (lex.get_type() == LEX_GOTO)
            {
                i = jump - 1;
            }
            else if (lex.get_type() == LEX_IF)
            {
                a = comp.pop(); //the result of calculating the condition for  while, for, if
                type_of_lex a_type;

                int lex1_int;
                double lex1_real;
                string lex1_string;
                get_values_from_lex(a, a_type,
                                lex1_int, lex1_real, lex1_string);

                if (a_type == LEX_VAL_INT)
                {
                    if (!lex1_int)
                    {
                        i = jump - 1;
                    }
                }

                else
                {
                    throw a.str_for_print() + " IF condition is not correct";

                }
the result from the calculation stack
            }
            break;
        }

        case LEX_READ:
        {
            a = comp.pop(); // result from the  stack
            type_of_lex a_type;

            int lex1_int;
            double lex1_real;
            string lex1_string;
            get_values_from_lex(a, a_type,
                          lex1_int, lex1_real, lex1_string);

            if (a_type == LEX_VAL_INT)
            {
                int aa;
                cin >> aa; //  enter a variable and put it in the name table
                ((IntIdent*)(TID[a.get_int_value()].get_ptr()))->put_value(aa);

                //cout << "READ " << ((IntIdent*)(TID[a.get_int_value()].get_ptr()))->get_value()<< endl;
            }
            else if (a_type == LEX_VAL_REAL)
            {
                double aa;
                cin >> aa;
                ((RealIdent*)(TID[a.get_int_value()].get_ptr()))->put_value(aa);

                //cout << "READ " << ((RealIdent*)(TID[a.get_int_value()].get_ptr()))->get_value()<< endl;
            }
            else if (a_type == LEX_VAL_STRING)
            {
                string aa;
                cin >> aa;
                ((StrIdent*)(TID[a.get_int_value()].get_ptr()))->put_value(aa);

                //cout << "READ " << ((StrIdent*)(TID[a.get_int_value()].get_ptr()))->get_value()<< endl;
            }
            else
            {
                throw a.str_for_print() + " READ types are incompatible";

            }
            break;
        }
        case LEX_WRITE:
        {
            a = comp.pop();
            type_of_lex a_type;

            int lex1_int;
            double lex1_real;
            string lex1_string;
            get_values_from_lex(a, a_type,
                          lex1_int, lex1_real, lex1_string);

            if (a_type == LEX_VAL_INT)
            {
                // don't delete!!!!!
                cout << lex1_int << endl;


                //cout << "WRITE "  << endl;
            }
            else if (a_type == LEX_VAL_REAL)
            {
                // don't delete!!!!!
                cout << lex1_real << endl;


                //cout << "WRITE " << endl;
            }
            else if (a_type == LEX_VAL_STRING)
            {
                // don't delete!!!!!
                cout << lex1_string << endl;


                //cout << "WRITE " << endl;
            }
            else
            {
                throw a.str_for_print() + " WRITE types are incompatible";

            }
            break;
        }


        case LEX_VAL_INT:
        case LEX_VAL_REAL:
        case LEX_VAL_STRING:
        case LEX_IDENT:
        {
            comp.push(lex);
            break;
        }
        case LEX_ASSIGNMENT:
        {


            b = comp.pop();
            a = comp.pop();
            if (a.get_type() != LEX_IDENT)
            {
                throw a.str_for_print() +  " is not Identifier";
            }

            type_of_lex id_type = TID[a.get_int_value()].get_type(); // identificator type


            type_of_lex b_type = b.get_type();  // value type

            //cout << name_of_lex[id_type] << " " << name_of_lex[b_type] << endl;
            //cout << check_assingment_types(id_type, b_type) << endl;
            if (!(check_assingment_types(id_type, b_type) ||
                b_type == LEX_IDENT && check_assingment_types(id_type, TID[b.get_int_value()].get_type())))
            {
                throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";
            }

            if (b_type == LEX_VAL_INT)
            {
                TID[a.get_int_value()].put_value(b.get_int_value());
                //cout << ((IntIdent*)TID[a.get_int_value()].get_ptr())->get_value() << endl;;

            }
            else if (b_type == LEX_VAL_REAL)
            {
                TID[a.get_int_value()].put_value(b.get_real_value());
                //cout << ((RealIdent*)TID[a.get_int_value()].get_ptr())->get_value() << endl;;

            }
            else if (b_type == LEX_VAL_STRING)
            {
                TID[a.get_int_value()].put_value(TID[b.get_int_value()].get_name());
                //cout << ((StrIdent*)TID[a.get_int_value()].get_ptr())->get_value() << endl;;

            }
            else if (b_type == LEX_IDENT)  // a=b
            {
                id_type = TID[b.get_int_value()].get_type();
                Value_type* tmp= TID[b.get_int_value()].get_ptr(); // need to find the value of b
                if (id_type == LEX_VAL_INT)
                {
                    TID[a.get_int_value()].put_value(((IntIdent*)tmp)->get_value());
                    //cout << ((IntIdent*)TID[a.get_int_value()].get_ptr())->get_value() << endl;;

                }
                else if (id_type == LEX_VAL_REAL)
                {
                    TID[a.get_int_value()].put_value(((RealIdent*)tmp)->get_value());
                    //cout << ((RealIdent*)TID[a.get_int_value()].get_ptr())->get_value() << endl;;

                }
                else if (id_type == LEX_VAL_STRING)
                {
                    TID[a.get_int_value()].put_value(((StrIdent*)tmp)->get_value());
                    //cout << ((StrIdent*)TID[a.get_int_value()].get_ptr())->get_value() << endl;;

                }
                else{
                    throw "unknown operation";
                }
                comp.push(a);
            }

            break;
        }


        case LEX_UNAR_MINUS:
        {
            a = comp.pop();
            type_of_lex a_type;

            int lex1_int;
            double lex1_real;
            string lex1_string;
            get_values_from_lex(a, a_type,
                          lex1_int, lex1_real, lex1_string);

            if (a_type == LEX_VAL_INT)
            {
                Lex res(LEX_VAL_INT, -lex1_int);
                //cout << "UNar minus res " << -lex1_int << endl;
                comp.push(res);
            }
            else if (a_type == LEX_VAL_REAL)
            {
                Lex res(LEX_VAL_REAL, -lex1_int);
                //cout << "UNar minus " << -lex1_int << endl;
                comp.push(res);
            }
            else
            {
                throw a.str_for_print() + " and UNARY_MINUS types are incompatible";

            }
            break;
        }
        case LEX_NOT:
        {
            a = comp.pop();
            type_of_lex a_type;

            int lex1_int;
            double lex1_real;
            string lex1_string;
            get_values_from_lex(a, a_type,
                          lex1_int, lex1_real, lex1_string);

            if (a_type == LEX_VAL_INT)
            {
                Lex res(LEX_VAL_INT, !lex1_int);
                //cout << "NOT res " << !lex1_int << endl;
                comp.push(res);
            }
            else
            {
                throw a.str_for_print() + " and NOT types are incompatible";

            }

            break;
        }

        case LEX_MINUS:
        case LEX_PLUS:
        case LEX_MULTIPLY:
        case LEX_DIVIDE:
        case LEX_OR:
        case LEX_AND:
        case LEX_LESS:
        case LEX_LESSEQ:
        case LEX_GREATER:
        case LEX_GREATEREQ:
        case LEX_EQ:
        case LEX_NEQ:
        {

            b = comp.pop();
            a = comp.pop();
            int lex1_int;
            double lex1_real;
            type_of_lex a_type;

            string lex1_string;
            get_values_from_lex(a, a_type,
                          lex1_int, lex1_real, lex1_string);
            int lex2_int;
            double lex2_real;
            string lex2_string;
            type_of_lex b_type;
            get_values_from_lex(b, b_type,
                          lex2_int, lex2_real, lex2_string);

            if (lex.get_type() == LEX_PLUS)
            {
                //cout<< "PLUS " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_int + lex2_int);
                    //cout << "Plus res " << lex1_int + lex2_int << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_INT && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_REAL, lex1_int + lex2_real);
                    //cout << "Plus res " << lex1_int + lex2_real << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_REAL, lex1_real + lex2_int);
                    //cout << "Plus res " << lex1_real + lex2_int << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_REAL, lex1_real + lex2_real);
                    //cout << "Plus res " << lex1_real + lex2_real << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_STRING && b_type == LEX_VAL_STRING )
                {
                    int index = TID.put(lex1_string + lex2_string);
                    //cout << "Plus res " << lex1_string + lex2_string << endl;
                    //TID[]
                    Lex res(LEX_VAL_STRING, index);
                    comp.push(res);
                }
                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";

                }
            }
            else if (lex.get_type() == LEX_MINUS)
            {
                //cout<< "Minus " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_int - lex2_int);
                    //cout << "Minus res " << lex1_int - lex2_int << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_INT && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_REAL, lex1_int - lex2_real);
                    //cout << "Minus res " << lex1_int - lex2_real << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_REAL, lex1_real - lex2_int);
                    //cout << "Minus res " << lex1_real - lex2_int << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_REAL, lex1_real - lex2_real);
                    //cout << "Minus res " << lex1_real - lex2_real << endl;
                    comp.push(res);
                }

                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";

                }
            }
            else if (lex.get_type() == LEX_MULTIPLY)
            {
                //cout<< "Mult " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_int * lex2_int);
                    //cout << "Mult res " << lex1_int * lex2_int << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_INT && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_REAL, lex1_int * lex2_real);
                    //cout << "Mult res " << lex1_int * lex2_real << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_REAL, lex1_real * lex2_int);
                    //cout << "Plus res " << lex1_real * lex2_int << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_REAL, lex1_real * lex2_real);
                    //cout << "Mult res " << lex1_real * lex2_real << endl;
                    comp.push(res);
                }

                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";

                }
            }
            else if (lex.get_type() == LEX_DIVIDE)
            {
                //cout<< "Div " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, (int)(lex1_int / lex2_int));
                    //cout << "Div res " << lex1_int / lex2_int << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_INT && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_REAL, lex1_int / lex2_real);
                    //cout << "Div res " << lex1_int / lex2_real << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_REAL, lex1_real / lex2_int);
                    //cout << "Div res " << lex1_real / lex2_int << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_REAL, lex1_real / lex2_real);
                    //cout << "Div res " << lex1_real / lex2_real << endl;
                    comp.push(res);
                }

                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";

                }
            }
            else if (lex.get_type() == LEX_OR)
            {
                //cout<< "OR " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_int || lex2_int);
                    //cout << "Or res " << (lex1_int || lex2_int) << endl;
                    comp.push(res);
                }
                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";

                }
            }
            else if (lex.get_type() == LEX_AND)
            {
                //cout<< "AND " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_int && lex2_int);
                    //cout << "AND res " << (lex1_int && lex2_int) << endl;
                    comp.push(res);
                }
                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";

                }
            }
            else if (lex.get_type() == LEX_LESS)
            {
                //cout<< "Less " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_int < lex2_int);
                    //cout << "Less res " << (lex1_int < lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_INT && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_int < lex2_real);
                    //cout << "Less res " << (lex1_int < lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_real < lex2_int);
                    //cout << "Less res " << (lex1_real < lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_real < lex2_real);
                    //cout << "Less res " << (lex1_real < lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_STRING && b_type == LEX_VAL_STRING )
                {
                    //cout << "Less res " << (lex1_string < lex2_string) << endl;
                    //TID[]
                    Lex res(LEX_VAL_INT, lex1_string < lex2_string);
                    comp.push(res);
                }
                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";

                }
            }
            else if (lex.get_type() == LEX_LESSEQ)
            {
                //cout<< "Lesseq " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_int <= lex2_int);
                    //cout << "Lesseq res " << (lex1_int <= lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_INT && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_int <= lex2_real);
                    //cout << "Lesseq res " << (lex1_int <= lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_real <= lex2_int);
                    //cout << "Lesseq res " << (lex1_real <= lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_real <= lex2_real);
                    //cout << "Lesseq res " << (lex1_real <= lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_STRING && b_type == LEX_VAL_STRING )
                {
                    //cout << "Lesseq res " << (lex1_string <= lex2_string) << endl;
                    //TID[]
                    Lex res(LEX_VAL_INT, lex1_string <= lex2_string);
                    comp.push(res);
                }
                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";

                }
            }
            else if (lex.get_type() == LEX_GREATER)
            {
                //cout<< "Greater " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_int > lex2_int);
                    //cout << "Greater res " << (lex1_int > lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_INT && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_int > lex2_real);
                    //cout << "Greater res " << (lex1_int > lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_real > lex2_int);
                    //cout << "Greater res " << (lex1_real > lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_real > lex2_real);
                    //cout << "Greater res " << (lex1_real > lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_STRING && b_type == LEX_VAL_STRING )
                {
                    //cout << "gr res " << (lex1_string > lex2_string) << endl;
                    //TID[]
                    Lex res(LEX_VAL_INT, lex1_string > lex2_string);
                    comp.push(res);
                }
                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";

                }
            }
            else if (lex.get_type() == LEX_GREATEREQ)
            {
                //cout<< "Greatereq " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_int >= lex2_int);
                    //cout << "Greatereq res " << (lex1_int >= lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_INT && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_int >= lex2_real);
                    //cout << "Greatereq res " << (lex1_int >= lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_real >= lex2_int);
                    //cout << "Greatereq res " << (lex1_real >= lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_real >= lex2_real);
                    //cout << "Greatereq res " << (lex1_real >= lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_STRING && b_type == LEX_VAL_STRING )
                {
                    //cout << "greq res " << (lex1_string >= lex2_string) << endl;
                    //TID[]
                    Lex res(LEX_VAL_INT, lex1_string >= lex2_string);
                    comp.push(res);
                }
                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";

                }
            }
            else if (lex.get_type() == LEX_EQ)
            {
                //cout<< "Eq " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_int == lex2_int);
                    //cout << "Eq res " << (lex1_int == lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_INT && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_int == lex2_real);
                    //cout << "Eq res " << (lex1_int == lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_real == lex2_int);
                    //cout << "Eq res " << (lex1_real == lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_real == lex2_real);
                    //cout << "Eq res " << (lex1_real == lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_STRING && b_type == LEX_VAL_STRING )
                {
                    //cout << "eq res " << (lex1_string == lex2_string) << endl;
                    //TID[]
                    Lex res(LEX_VAL_INT, lex1_string == lex2_string);
                    comp.push(res);
                }
                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";
                }
            }
            else if (lex.get_type() == LEX_NEQ)
            {
                //cout<< "Neq " << lex1_int << " " << lex1_real << " " << lex2_int << " " << lex2_real << endl;

                if (a_type == LEX_VAL_INT && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_int != lex2_int);
                    //cout << "Neq res " << (lex1_int != lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_INT && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_int != lex2_real);
                    //cout << "Neq res " << (lex1_int != lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_INT )
                {
                    Lex res(LEX_VAL_INT, lex1_real != lex2_int);
                    //cout << "Neq res " << (lex1_real != lex2_int) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_REAL && b_type == LEX_VAL_REAL )
                {
                    Lex res(LEX_VAL_INT, lex1_real != lex2_real);
                    //cout << "Neq res " << (lex1_real != lex2_real) << endl;
                    comp.push(res);
                }
                else if (a_type == LEX_VAL_STRING && b_type == LEX_VAL_STRING )
                {
                    //cout << "neq res " << (lex1_string != lex2_string) << endl;
                    //TID[]
                    Lex res(LEX_VAL_INT, lex1_string != lex2_string);
                    comp.push(res);
                }
                else
                {
                    throw a.str_for_print() + " and " + b.str_for_print() + " types are incompatible";
                }
            }

            break;
            }
        }

    }
}

void Syntax::H()
{
    if(curr_lex.get_type() == LEX_PROGRAM)
     {
        gl();
        if(curr_lex.get_type() == LEX_SEMICOLON)
            gl();
            A();
     }
    else
        throw curr_lex;
}

void Syntax::A()
{
    if(curr_lex.get_type() == LEX_NULL)
        return;

    else if(curr_lex.get_type() == LEX_INT) //  
        {
            flag=LEX_VAL_INT;
            gl();
            B();
            A();
        }
    else if(curr_lex.get_type() == LEX_REAL) // 
        {
            flag=LEX_VAL_REAL;
            gl();
            B();
            A();
        }
    else if(curr_lex.get_type() == LEX_STRING) //
        {
            flag=LEX_VAL_STRING;
            gl();
            B();
            A();
        }

    else
    {
        //cout << curr_lex << endl;
           Begend();
    }

}


void Syntax::Operators() // operator
{
    //cout << curr_lex << endl;
    if(curr_lex.get_type() == LEX_WRITE)
    {
            //St.push(curr_lex, Poliz);
            gl();
            if(curr_lex.get_type() != LEX_LPARENTHESIS)
            {
                throw curr_lex;
            }
            St.push(curr_lex, Poliz);
            gl();
            //cout << curr_lex  <<" EXPR"<< endl;
            Expr();
            while (St.get_top().get_type()!=LEX_LPARENTHESIS)
            {
                Poliz.push_back(St.pop());
            }
            Poliz.push_back(Lex(LEX_WRITE));
            //cout << curr_lex  <<" EXPR"<< endl;
            while (curr_lex.get_type() == LEX_COMMA)
            {
                gl();
                Expr();
                while (St.get_top().get_type()!=LEX_LPARENTHESIS)
                {
                    Poliz.push_back(St.pop());
                }
                Poliz.push_back(Lex(LEX_WRITE));
            }

            //gl();
            if(curr_lex.get_type() != LEX_RPARENTHESIS)
            {
                throw curr_lex;
            }
            St.push(curr_lex, Poliz);

            gl();
            if(curr_lex.get_type() != LEX_SEMICOLON)
            {
                throw curr_lex;
            }
            gl();
    }

    else if(curr_lex.get_type() == LEX_READ)
    {
            gl();
            //cout << curr_lex << endl;
            if(curr_lex.get_type() != LEX_LPARENTHESIS)
            {
                throw curr_lex;
            }

            gl();
            //cout << curr_lex << endl;
            if(curr_lex.get_type() != LEX_IDENT)
            {
                throw curr_lex;
            }

            if (!TID[curr_lex.get_int_value()].get_declare())
            {
                throw curr_lex;
            }

            Poliz.push_back(curr_lex);
            Poliz.push_back(Lex(LEX_READ));

            gl();
            //cout << curr_lex << endl;
            if(curr_lex.get_type() != LEX_RPARENTHESIS)
            {
                throw curr_lex;
            }
            gl();
            if(curr_lex.get_type() != LEX_SEMICOLON)
            {
                throw curr_lex;
            }
            gl();
            //cout << curr_lex << endl;
    }
///////////////IF/////////////////////////
    else if(curr_lex.get_type() == LEX_IF)
    {
            //cout << curr_lex << endl;
            gl();
            if(curr_lex.get_type() != LEX_LPARENTHESIS)
            {
                throw curr_lex;
            }
            St.push(curr_lex, Poliz);
           // cout << curr_lex << endl;
            gl();
            Expr();
            while (St.get_top().get_type()!=LEX_LPARENTHESIS)
            {
                Poliz.push_back(St.pop());
            }

            if(curr_lex.get_type() != LEX_RPARENTHESIS)
            {
                throw curr_lex;
            }
            St.push(curr_lex, Poliz);

            Poliz.push_back(Lex(LEX_JUMP)); //else
            int first_jmp = Poliz.size() - 1;
            Poliz.push_back(Lex(LEX_IF));
             //cout << curr_lex << endl;
            gl();
            Begend();
            //cout << curr_lex << "++++++" << endl;

            //gl();
           // cout << curr_lex << "-----" << endl;
            if(curr_lex.get_type() != LEX_SEMICOLON)
            {
                throw curr_lex;
            }
            gl();
            Poliz.push_back(Lex(LEX_JUMP)); //end
            int second_jmp = Poliz.size() - 1;
            Poliz.push_back(Lex(LEX_GOTO));


            Poliz[first_jmp].set_int_value(Poliz.size());
            //cout << curr_lex << "@@@" << endl;
            //cout << curr_lex << "@@@@" <<endl;
            if(curr_lex.get_type() == LEX_ELSE)
            {
                gl();
                Begend();
                if(curr_lex.get_type() != LEX_SEMICOLON)
                {
                    throw curr_lex;
                }
                gl();
            }
            Poliz[second_jmp].set_int_value(Poliz.size());
            //gl();

    }

    ///////////////WHILE/////////////////////////
    else if(curr_lex.get_type() == LEX_WHILE)
    {
            gl();
            //cout << curr_lex << endl;
            int begin_while = Poliz.size();
            if(curr_lex.get_type() != LEX_LPARENTHESIS)
            {
                throw curr_lex;
            }
            St.push(curr_lex, Poliz);
            gl();
            //cout << curr_lex << endl;
            Expr();
            while (St.get_top().get_type()!=LEX_LPARENTHESIS)
            {
                Poliz.push_back(St.pop());
            }
            //cout << curr_lex << endl;
            if(curr_lex.get_type() != LEX_RPARENTHESIS)
            {
                throw curr_lex;
            }
            St.push(curr_lex, Poliz);
            st_jump.push(begin_while); // continue
            st_jump.push(Poliz.size()); // break;
            //int jump_size = st_jump.get_size() - 1;
            Poliz.push_back(Lex(LEX_JUMP));
            int first_jump = Poliz.size() - 1;

            Poliz.push_back(Lex(LEX_IF));

            gl();
            //cout << curr_lex << endl;
            Begend(1);

            Poliz.push_back(Lex(LEX_JUMP, begin_while));

            Poliz.push_back(Lex(LEX_GOTO));

            Poliz[first_jump].set_int_value(Poliz.size());
            //st_jump.set_elem(jump_size, Poliz.size());
            //gl();
            if(curr_lex.get_type() != LEX_SEMICOLON)
            {
                throw curr_lex;
            }
            gl();
            st_jump.pop();
            st_jump.pop();
    }

///////////////FOR/////////////////////////
    else if(curr_lex.get_type() == LEX_FOR)
    {
            gl();

//cout << curr_lex << endl;
            if(curr_lex.get_type() != LEX_LPARENTHESIS)
            {
                throw curr_lex;
            }
            St.push(curr_lex, Poliz);


            gl();
//cout << curr_lex << endl;
            Expr();


//cout << curr_lex << endl;
            if(curr_lex.get_type() != LEX_SEMICOLON)
            {
                throw curr_lex;
            }

            while (St.get_top().get_type()!=LEX_LPARENTHESIS)
            {
                Poliz.push_back(St.pop());
            }

            int L0 = Poliz.size();
            //cout << curr_lex << endl;
            gl();
            //cout << curr_lex << endl;
            Expr();
//cout << curr_lex << endl;
            if(curr_lex.get_type() != LEX_SEMICOLON)
            {
                throw curr_lex;
            }
            while (St.get_top().get_type()!=LEX_LPARENTHESIS)
            {
                Poliz.push_back(St.pop());
            }
            st_jump.push(0); // continue
            st_jump.push(Poliz.size()); // break;

            Poliz.push_back(Lex(LEX_JUMP));
            int first_jmp = Poliz.size() - 1;
            Poliz.push_back(Lex(LEX_IF));

            Poliz.push_back(Lex(LEX_JUMP));
            int second_jmp = Poliz.size() - 1;
            Poliz.push_back(Lex(LEX_GOTO));

            int L3 = Poliz.size();
            st_jump.set_elem(st_jump.get_size() - 2, L3);
            gl();
            Expr();

            if(curr_lex.get_type() != LEX_RPARENTHESIS)
            {
                throw curr_lex;
            }
            while (St.get_top().get_type()!=LEX_LPARENTHESIS)
            {
                Poliz.push_back(St.pop());
            }
            St.push(curr_lex, Poliz);

            Poliz.push_back(Lex(LEX_JUMP, L0));
            Poliz.push_back(Lex(LEX_GOTO));
            Poliz[second_jmp].set_int_value(Poliz.size());

            gl();
            Begend(1);

            Poliz.push_back(Lex(LEX_JUMP, L3));

            Poliz.push_back(Lex(LEX_GOTO));
            Poliz[first_jmp].set_int_value(Poliz.size());

            //gl();
            if(curr_lex.get_type() != LEX_SEMICOLON)
            {
                throw curr_lex;
            }
            gl();
    }
    else if(curr_lex.get_type() == LEX_CONTINUE)
    {
        if (st_jump.get_size() < 2)
        {
            throw curr_lex;
        }
        Poliz.push_back(Lex(LEX_CONTINUE, st_jump.get_elem(st_jump.get_size()-2)));
        //cout << "CONTINUE" <<st_jump.get_elem(st_jump.get_size()-1) << " " << st_jump.get_elem(st_jump.get_size()-2) << endl;
        //Poliz.push_back(Lex(LEX_JUMP, )); // break
        //Poliz.push_back(Lex(LEX_JUMP), st_jump.get_elem(st_jump.get_size()-1)); // break
        gl();
        if(curr_lex.get_type() != LEX_SEMICOLON)
        {
            throw curr_lex;
        }
        gl();

    }
    else if(curr_lex.get_type() == LEX_BREAK)
    {
        if (st_jump.get_size() < 2)
        {
            throw curr_lex;
        }
        Poliz.push_back(Lex(LEX_BREAK, st_jump.get_elem(st_jump.get_size()- 1)));
        //Poliz.push_back(Lex(LEX_JUMP, )); // break
        //Poliz.push_back(Lex(LEX_JUMP), st_jump.get_elem(st_jump.get_size()-1)); // break
        gl();
        if(curr_lex.get_type() != LEX_SEMICOLON)
        {
            throw curr_lex;
        }
        gl();

    }
    else
    {
        Expr();
        if(curr_lex.get_type() != LEX_SEMICOLON)
        {
            throw curr_lex;
        }
        while (!St.is_empty())
        {

            Poliz.push_back(St.pop());
        }
        gl();
    }

}

void Syntax::B()
{
    if(curr_lex.get_type() == LEX_IDENT)
    {
        //cout << "!!!!!IDENT " << curr_lex << endl;
        //cout << TID[curr_lex.get_int_value()] << endl;
        Poliz.push_back(curr_lex);

        TID[curr_lex.get_int_value()].put_declare();
        TID[curr_lex.get_int_value()].put_type(flag);

        //cout << TID[curr_lex.get_int_value()] << endl;
        gl();
        Perem();

        //St.pop();
    }
    else
        throw curr_lex;
}


void Syntax::Perem() // Perem
{
    //if(curr_lex.get_type() == LEX_SEMICOLON)
    //cout << "!!!!!Perem " << name_of_lex[curr_lex.get_type()] << endl;
    //cout << "!!!!!Perem " << curr_lex << endl;

    if (curr_lex.get_type() == LEX_ASSIGNMENT)
    {
          gl();
          Const();
          Poliz.push_back(Lex(LEX_ASSIGNMENT));
    }
    else
    {
        Poliz.pop_back();
        SC();
    }
};

void Syntax::Const()
{

    if(curr_lex.get_type() == LEX_VAL_STRING && flag==LEX_VAL_STRING) // string
    {
        Poliz.push_back(curr_lex);
        gl();
        SC();
    }
    else if (curr_lex.get_type() == LEX_VAL_REAL && flag==LEX_VAL_REAL ||  //  int/real
             curr_lex.get_type() == LEX_VAL_INT && flag==LEX_VAL_INT)
    {
        Poliz.push_back(curr_lex);
        gl();
        SC();
    }
    else if (curr_lex.get_type() == LEX_PLUS) //Unary plus
    {
        gl();
        if (curr_lex.get_type() == LEX_VAL_REAL && flag==LEX_VAL_REAL ||
             curr_lex.get_type() == LEX_VAL_INT && flag==LEX_VAL_INT)
        {
            Poliz.push_back(curr_lex);
            gl();
            SC();
        }
    }

    else if (curr_lex.get_type() == LEX_MINUS) // Unary minus
    {
        gl();
        if (curr_lex.get_type() == LEX_VAL_REAL && flag==LEX_VAL_REAL ||
             curr_lex.get_type() == LEX_VAL_INT && flag==LEX_VAL_INT)
        {
            curr_lex.change_sign();
            Poliz.push_back(curr_lex);
            gl();
            SC();
        }
    }

    else
        throw curr_lex;
}


void Syntax::SC()
{
    if(curr_lex.get_type() == LEX_SEMICOLON)
    {
        gl();
        //A();
    }
    else
        throw curr_lex;
}

void Syntax::Begend(int contbreak)
{
 //cout << curr_lex << endl;
     if(curr_lex.get_type() != LEX_BEGIN)
     {
        throw curr_lex;
     }

     gl();
 //cout << curr_lex << " ###" << endl;
     //int count = 0;

     while(curr_lex.get_type() != LEX_END && curr_lex.get_type() != LEX_NULL)
     {
        //gl();
        try
        {
            Operators();
        }
        catch(Lex& lex)
        {
            if (contbreak == 0 ||  contbreak == 1 && lex.get_type() != LEX_BREAK && lex.get_type() != LEX_CONTINUE)
            {
                throw;
            }

        }
        //gl();
        //count ++;
        //cout << curr_lex << " !!!" << endl;
     }
     gl();

};

void Syntax::Expr()
{
    bool _exit = false; // Checking two consecutive variables

    while (_exit == false &&(curr_lex.get_type() == LEX_IDENT)      ||
                           (curr_lex.get_type() == LEX_VAL_INT)     ||
                           (curr_lex.get_type() == LEX_VAL_REAL)    ||
                           (curr_lex.get_type() == LEX_VAL_STRING)  ||
                           (curr_lex.get_type() == LEX_NOT)         ||
                           (curr_lex.get_type() == LEX_MINUS)       ||
                           (curr_lex.get_type() == LEX_LPARENTHESIS)
           )
    {
        _exit = true;
        //cout << curr_lex  << " while"<< endl;
        if(curr_lex.get_type() == LEX_IDENT)
        {
            if (!TID[curr_lex.get_int_value()].get_declare())
            {
                throw curr_lex;
            }
            Poliz.push_back(curr_lex);
            gl();
            //cout << curr_lex  << " ID"<< endl;
            if (ID())
            {
                _exit = false;
            }

        }

        else if ((curr_lex.get_type() == LEX_VAL_INT) ||
                 (curr_lex.get_type() == LEX_VAL_REAL)||
                 (curr_lex.get_type() == LEX_VAL_STRING)
                 )
        {
           //cout << curr_lex  << " REAL??????? " << curr_lex.get_real_value() << endl;
           Poliz.push_back(curr_lex);
            gl();
            if (Op())
            {
                _exit = false;
            }
        }

        else if (curr_lex.get_type() == LEX_NOT)
        {
            St.push(curr_lex, Poliz);
            gl();
            if (curr_lex.get_type() == LEX_LPARENTHESIS)
            {
                St.push(curr_lex, Poliz);
                gl();
                if (Brackets())
                {
                    _exit = false;
                }
                Poliz.push_back(St.pop());
            }
            else
            {
                throw curr_lex;
            }
        }

        else if (curr_lex.get_type() == LEX_LPARENTHESIS)
        {
            St.push(curr_lex, Poliz);
            gl();
            if(Brackets())
            {
                _exit = false;
            }
        }
        else if (curr_lex.get_type() == LEX_MINUS)
        {
            Lex tmp_lex(LEX_UNAR_MINUS);
            St.push(tmp_lex, Poliz);
            //Poliz.push_back(tmp_lex);
            gl();
        }
        else
        {
            throw curr_lex;
        }
    }

    /*while(!St.is_empty())
    {
        Poliz.push_back(St.pop());
    }*/
    //cout << curr_lex << endl;
    if (_exit == true &&
        curr_lex.get_type() != LEX_SEMICOLON &&
        curr_lex.get_type() != LEX_RPARENTHESIS &&
        curr_lex.get_type() != LEX_COMMA)
    {
        throw curr_lex;
    }
    //cout << curr_lex << endl;

    //gl();
}

bool Syntax::Op()
{
    if(curr_lex.get_type() == LEX_PLUS      ||
       curr_lex.get_type() == LEX_MINUS     ||
       curr_lex.get_type() == LEX_MULTIPLY  ||
       curr_lex.get_type() == LEX_DIVIDE    ||
       curr_lex.get_type() == LEX_LESS      ||
       curr_lex.get_type() == LEX_LESSEQ    ||
       curr_lex.get_type() == LEX_GREATER   ||
       curr_lex.get_type() == LEX_GREATEREQ ||
       curr_lex.get_type() == LEX_EQ        ||
       curr_lex.get_type() == LEX_NEQ       ||
       curr_lex.get_type() == LEX_OR        ||
       curr_lex.get_type() == LEX_AND)
    {
        St.push(curr_lex, Poliz);
        gl();
        return true;
    }

    else
    {
        return false;
    }
}

bool Syntax::ID()
{
    if(curr_lex.get_type() == LEX_ASSIGNMENT)
    {
        St.push(curr_lex, Poliz);
        gl();
        return true;
    }

    else
    {
        return Op();
    }
}

bool Syntax::Brackets()
{
    //cout << curr_lex  << " Brack1"<< endl;
    Expr();
    //cout << curr_lex  << " Brack2"<< endl;
    if (curr_lex.get_type() == LEX_RPARENTHESIS)
    {
        St.push(curr_lex, Poliz);
        gl();
        //cout << curr_lex  << " Brack!!!"<< endl;
        if(Op())
        {
            return true;
            //only = false;
        }
        else if (curr_lex.get_type() == LEX_RPARENTHESIS ||
                  curr_lex.get_type() == LEX_SEMICOLON ||
                 curr_lex.get_type() == LEX_COMMA)
        {
            return true;
        }
        else
        {
            throw curr_lex;
        }
    }

    else
    {
        throw curr_lex;
    }
}

int main(void)
{
    Syntax s("test5.txt");

    try
    {
        s.analyze();
    }

    catch(Lex& lex)
    {
        cout << lex << endl;
    }


}






