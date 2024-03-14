#include <iostream>
#include <map>
#include <cmath>

using namespace std;


enum type_of_lex //Enum with all lexems
{
    LEX_PROGRAM,
    LEX_INT,
    LEX_REAL,
    LEX_STRING,
    LEX_VAL_STRING,
    LEX_VAL_INT,
    LEX_VAL_REAL,
    LEX_IF,
    LEX_ELSE,
    LEX_DO,
    LEX_WHILE,
    LEX_FOR,
    LEX_CONTINUE,
    LEX_BREAK,
    LEX_NOT,
    LEX_AND,
    LEX_OR,
    LEX_READ,
    LEX_WRITE,
    LEX_SEMICOLON,
    LEX_LPARENTHESIS,
    LEX_RPARENTHESIS,
    LEX_ASSIGNMENT,
    LEX_LESS,
    LEX_GREATER,
    LEX_LESSEQ,
    LEX_GREATEREQ,
    LEX_EQ,
    LEX_PLUS,
    LEX_MINUS,
    LEX_UNAR_MINUS,
    LEX_MULTIPLY,
    LEX_DIVIDE,
    LEX_NEQ,
    LEX_IDENT,
    LEX_COMMA,
    LEX_BEGIN,
    LEX_END,
    LEX_NULL,
    LEX_ERROR,
    LEX_JUMP,
    LEX_GOTO
};

string name_of_lex[] = // array of lines to output
{
    "LEX_PROGRAM",
    "LEX_INT",
    "LEX_REAL",
    "LEX_STRING",
    "LEX_VAL_STRING",
    "LEX_VAL_INT",
    "LEX_VAL_REAL",
    "LEX_IF",
    "LEX_ELSE",
    "LEX_DO",
    "LEX_WHILE",
    "LEX_FOR",
    "LEX_CONTINUE",
    "LEX_BREAK",
    "LEX_NOT",
    "LEX_AND",
    "LEX_OR",
    "LEX_READ",
    "LEX_WRITE",
    "LEX_SEMICOLON",
    "LEX_LPARENTHESIS",
    "LEX_RPARENTHESIS",
    "LEX_ASSIGNMENT",
    "LEX_LESS",
    "LEX_GREATER",
    "LEX_LESSEQ",
    "LEX_GREATEREQ",
    "LEX_EQ",
    "LEX_PLUS",
    "LEX_MINUS",
    "LEX_UNAR_MINUS",
    "LEX_MULTIPLY",
    "LEX_DIVIDE",
    "LEX_NEQ",
    "LEX_IDENT",
    "LEX_COMMA",
    "LEX_BEGIN",
    "LEX_END",
    "LEX_NULL",
    "LEX_ERROR",
    "LEX_JUMP",
    "LEX_GOTO"
};

map<string, type_of_lex> keywords =
{
    {"program", LEX_PROGRAM},
    {"int",     LEX_INT},
    {"real",    LEX_REAL},
    {"string",  LEX_STRING},
    {"if",      LEX_IF},
    {"else",    LEX_ELSE},
    {"do",      LEX_DO},
    {"while",   LEX_WHILE},
    {"for",     LEX_FOR},
    {"continue",LEX_CONTINUE},
    {"break",   LEX_BREAK},
    {"not",     LEX_NOT},
    {"and",     LEX_AND},
    {"or",      LEX_OR},
    {"read",    LEX_READ},
    {"write",   LEX_WRITE},
    {"begin",   LEX_BEGIN},
    {"end",     LEX_END},
};

map<string, type_of_lex> operators =
{
    {";",  LEX_SEMICOLON},
    {",",  LEX_COMMA},
    {"(",  LEX_LPARENTHESIS},
    {")",  LEX_RPARENTHESIS},
    {"=",  LEX_ASSIGNMENT},
    {"<",  LEX_LESS},
    {">",  LEX_GREATER},
    {"<=", LEX_LESSEQ},
    {">=", LEX_GREATEREQ},
    {"==", LEX_EQ},
    {"+",  LEX_PLUS},
    {"-",  LEX_MINUS},
    {"*",  LEX_MULTIPLY},
    {"/",  LEX_DIVIDE},
    {"!=", LEX_NEQ},
};

class Value_type
{/*
public:
    virtual int get_value () = 0;
    virtual void put_value (int v) = 0;*/
};

class IntIdent : public Value_type 
{
    int value;

public:
    int get_value ()
    {
        return value;
    }

    void put_value (int v)
    {
        value = v;
    }
};

class RealIdent : public Value_type 
{
    double r_value;

public:
    double get_value ()
    {
        return r_value;
    }

    void put_value (double r)
    {
        r_value = r;
    }
};


class StrIdent : public Value_type 
{
    string str;

public:
    string get_value ()
    {
        return str;
    }

    void put_value (string s)
    {
        str = s;
    }
};

class Ident
{
    string name;
    bool declare;
    type_of_lex type;
    bool was_assigned;

    Value_type* value;
   // int value;
   // double r_value;

public:

    friend ostream& operator << ( ostream& s, Ident id )
    {

        s << ' ' << id.name << ' ' << name_of_lex[id.type]<< ' ' << id.declare << ' ' << id.was_assigned  <<  ");";

        return s;
    }

    void put_value(int val)
    {
        value = new IntIdent;
        ((IntIdent*)value)->put_value(val);
    }

    void put_value(double val)
    {
        value = new RealIdent;
        ((RealIdent*)value)->put_value(val);
    }

    void put_value(string s)
    {
        value = new StrIdent;
        ((StrIdent*)value)->put_value(s);
    }

    Value_type* get_ptr()
    {
        /*if (type == LEX_REAL)
            return (RealIdent*)value;
        if (type == LEX_INT)
            return (IntIdent*)value;
        if (type == LEX_STRING)
            return (StrIdent*)value;*/
        return value;
    }

    Ident () 
    {
        declare = false;
        was_assigned = false;
    }

    string get_name ()
    {
        return name;
    }

    void put_name (const string &n)
    {
        name = n;
    }

    bool get_declare ()
    {
        return declare;
    }

    void put_declare ()
    {
        declare = true;
    }

    type_of_lex get_type ()
    {
        return type;
    }

    void put_type (type_of_lex t)
    {
        type = t;
    }

    bool get_assign ()
    {
        return was_assigned;
    }

    void put_assign ()
    {
        was_assigned = true;
    }
};



class Tabl_Ident
{
    Ident * p;
    int size;
    int top;

public:
    Tabl_Ident ( int max_size )
    {
        p = new Ident[size=max_size]; // array of identifiers
        top = 0;
    }

    ~Tabl_Ident ()  
    {
        delete[]p;
    }

    Ident& operator[] ( int k )
    {
        return p[k];
    }

    int put ( string buf );

};

int Tabl_Ident::put ( string buf )
{
    for ( int j=0; j<top; j++ )
    {
      if ( buf == p[j].get_name() ) //pointer to the Identifier class
          return j;
    }

    p[top].put_name(buf);
    top++;
    return top-1;
}


Tabl_Ident TID(100);

class Lex
{
    type_of_lex type_lex = LEX_NULL;
    int int_value = 0;
    double real_value; 

    enum TYPE{INT, REAL, OTHER};
    TYPE is_int = OTHER;
The specification of the token
public:
    Lex ( type_of_lex t = LEX_NULL ) 
    {
        type_lex = t;
    }

    Lex ( type_of_lex t, int v ) 
    {
        type_lex = t; int_value = v;
        is_int = INT;            // specification of the lexem
    }

    Lex ( type_of_lex t, double real_v) 
    {
        type_lex = t; real_value = real_v;
        is_int = REAL;
    }

    type_of_lex get_type () { return type_lex; }
    int get_int_value () { return int_value; }
    void set_int_value(int a) {int_value = a;} // jump (number in the Reverse Polish Notation )
    double get_real_value () { return real_value; }
    void change_sign() {real_value *= -1; int_value *= -1; }
    TYPE get_value_type() { return is_int; }

    string str_for_print()
    {
        string s;
        //cout << is_int << endl;;

        if (is_int == INT || type_lex == LEX_JUMP)
        {
            s += "(" + name_of_lex[type_lex] + "," + std::to_string(int_value)  +  ");";
        }
        else if (is_int == REAL)
        {
            s += "(" + name_of_lex[type_lex] + "," + std::to_string(real_value)  +  ");";
        }
        else
        {
            s += "(" + name_of_lex[type_lex] +  ");";
        }
        return s;
    }
    friend ostream& operator << ( ostream& s, Lex l )
    {
        //cout << l.str_for_print();
        s << l.str_for_print();
        return s;
    }

   /* Lex prog(LEX_PROGRAM, 0, 12);
    cout << prog << " " << ",";*/
};


class Parser{
    enum state { H, ID, NUM, RNUM, STR}; // States

    char c;
    string buf;
    int number;
    int flag;
    int fract;
    int count_10;
    state CS;
    FILE* fp;

public:

    Parser ( const char * program )
    {
        fp = fopen ( program, "r" );
        CS = H;
        gc();
    }

    char get_c()
    {
        return c;
    }

    void gc()
    {
        c = fgetc(fp);
    }

    Lex get_lex();
    Lex Check_keywords();

};

Lex Parser::get_lex ()
{
   //  cout << c << endl;
    CS = H;
    std::map<string,type_of_lex>::iterator it; 

    do
    {
        switch ( CS )
        {
        case H:
            if ( c ==' ' || c =='\n' || c=='\r' || c =='\t' )
                gc ();
            else if (isalpha(c))
            {
                buf = c;
                gc ();
                CS = ID;
            }
            else if (isdigit(c))
            {
                number =c-'0'; // culculating the number
                gc();
                CS=NUM;
            }
            else if(c=='>')
            {
                gc();
                if (c == '=')
                {
                    gc();
                    return Lex(LEX_GREATEREQ);
                }
                return Lex(LEX_GREATER);
            }

            else if(c=='<')
            {
                gc();
                if (c == '=')
                {
                    gc();
                    return Lex(LEX_LESSEQ);
                }
                return Lex(LEX_LESS);
            }

            else if(c=='!')
            {
                gc();
                if (c == '=')
                {
                    gc();
                    return Lex(LEX_NEQ);
                }
               return Lex(LEX_ERROR);
            }

            else if(c=='=')
            {
                gc();
                if (c == '=')
                {
                    gc();
                    return Lex(LEX_EQ);
                }
               return Lex(LEX_ASSIGNMENT);
            }
            else if(c=='\"')
            {
               gc();
               buf="";
               CS=STR;
            }

            else if ((it = operators.find(string(1, c))) != operators.end()){
                gc();
                return Lex(it->second);
            }

            else if (c == EOF)
            {
                return Lex(); 
            }
            else
            {
                return Lex(LEX_ERROR);
            }
            break;

        case ID:
            if ( isalpha(c) || isdigit(c))
            {
                buf += c;
                gc ();
            }
            else
            {
                return Check_keywords();
            }
            break;

        case NUM:
            if(isdigit(c))
            {

                number=(number*10)+(c-'0');
                gc();
            }
            else
            {
                if(c=='.')
                {
                    gc();
                    fract = 0;
                    count_10 = 0;
                    CS=RNUM;
                }

                else
                {
                    return Lex(LEX_VAL_INT, number);
                }
            }
            break;

        case RNUM:
            if(isdigit(c))
            {
                fract=(fract*10)+(c-'0');
                count_10++;
                gc();
            }
            else
            {

                double r = number+((double)fract/pow(10,count_10));
                //cout << "double " << number << " " << fract << " " << count_10 <<" "  << r << endl;
                return Lex(LEX_VAL_REAL, r);
            }
            break;

        case STR:
            //cout << (int)c;
            if(c!='\"')
            {
                if (c == EOF)
                {
                    return Lex(LEX_ERROR);
                }
                buf+=c;
                gc();
            }
            else
            {
                gc();
                // Identificators' table
        //        cout << buf << endl;
                int index = TID.put(buf);
                //return Lex(LEX_IDENT, index);
                return Lex(LEX_VAL_STRING, index);
            }
            break;
        }
    }
    while (true);
}


Lex Parser::Check_keywords()
{
    std::map<string,type_of_lex>::iterator it; 

    it = keywords.find(buf);
   // cout  << "!!!!!!" << buf << endl;
    if (it == keywords.end())
    {
        int index = TID.put(buf);
        return Lex(LEX_IDENT, index);
    }
    else
    {
        return Lex(it->second);  
    }
}

//vector<Ident> vec;

//string NAMES(100);


/*int main()
{
    //cout <<  static_cast<type_of_lex>(LEX_NULL) << endl;
    Parser p("test.txt");
    string cur_lex;

    while (p.get_c() != EOF && cur_lex != "LEX_ERROR")
    {
        //cur_lex =  name_of_lex[p.get_lex().get_type()];
        //cout << cur_lex << endl;
        cout << p.get_lex() << endl;
    }
 return 0;
}

/*
    Ident a;
    a.put_type(LEX_INT);
    a.put_value(12);
    cout << ((IntIdent*)a.get_ptr())->get_value() << endl;

    Ident b;
    b.put_value(12.32);
    b.put_type(LEX_REAL);
    cout << ((RealIdent*)b.get_ptr())->get_value() << endl;

*/


