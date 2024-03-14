#include <iostream>
#include <map>
#include <vector>
#include "Lex.cpp"

using namespace std;

//template <class T, int max_size >
class Stack_int // for break
{
    int* s;//[max_size];
    int top;
    int max_size;

public:
    Stack_int(int max_size_){
        max_size = max_size_;
        s = new int[max_size];
        top = 0;
    }
 //   void reset() { top = 0; };
    int get_size() {return top;} //Current size
    int get_elem(int index) { return s[index];}
    void set_elem(int index, int elem){s[index] = elem;}
    //template <class U>
    void push(int i);

    //void push(T i);
//    T top{return s[top];}
    int pop();
    bool is_empty(){ return top == 0; }
    bool is_full() { return top == max_size; }
};


class Stack_Lex
{
    Lex* s;//[max_size];
    int top;
    int max_size;

public:
    Stack_Lex(int max_size_){
        max_size = max_size_;
        s = new Lex[max_size];
        top = 0;
    }
   // void reset() { top = 0; };

    //template <class U>
    void push(Lex i, std::vector<Lex>& answer); // for assembly and processing of Polish reverse notation
    void push(Lex i);  //!!!!!!!
    Lex get_top() {return s[top-1];}
    //void push(T i);
//    T top{return s[top];}
    Lex pop();
    bool is_empty(){ return top == 0; }
    bool is_full() { return top == max_size; }
};


void Stack_Lex::push(Lex i)  //!!!!!!!
{

    if ( !is_full() )
    {
            s[top] = i;
            ++top;
    }
    else
        throw "Stack_is_full";
}

//template <class T>
map<type_of_lex, int> priority =
{
    {LEX_NULL,0},
    {LEX_ASSIGNMENT,0},
    {LEX_OR,        1},
    {LEX_AND,       2},
    {LEX_LESS,      3},
    {LEX_LESSEQ,    3},
    {LEX_GREATER,   3},
    {LEX_GREATEREQ, 3},
    {LEX_EQ,        3},
    {LEX_NEQ,       3},
    {LEX_PLUS,      4},
    {LEX_MINUS,     4},
    {LEX_MULTIPLY,  5},
    {LEX_DIVIDE,    5},
    {LEX_NOT,       6},
    {LEX_UNAR_MINUS, 6},

};


void Stack_int::push(int i)  //Polish reverse notation processing
{

    if ( !is_full() )
    {
        s[top] = i;
        ++top;
    }
    else
        throw "Stack_is_full";
}


void Stack_Lex::push(Lex i, std::vector<Lex>& vec)  // Polish reverse notation creation
{
    if ( !is_full() )
    {
        //cout <<"Stack TOP " << s[top-1] << endl;
        //cout <<"CUR " << i << endl;
        if (i.get_type() == LEX_RPARENTHESIS)
        {
            while (s[top-1].get_type() != LEX_LPARENTHESIS)
            {

                vec.push_back(pop());
               // cout <<"Stack " << s[top-1] << endl;
            }
            pop(); // The left bracket is up
        }
        else if (i.get_type() == LEX_LPARENTHESIS)
        {
            s[top] = i;
            ++top;
        }
        else
        {
            map<type_of_lex, int>::iterator it; 

            it = priority.find(i.get_type());
            if (it != priority.end())

            {
                while (priority[i.get_type()] < priority[s[top-1].get_type()])
                {
                    vec.push_back(pop());
                }
            }
            else
            {
                cout<<"Check STACK!!!  "<< endl;
            }

            s[top] = i;
            //cout <<"PUSH " << s[top] << endl;
            ++top;
            //cout <<"STACK DATA " << endl;
            for (int j = 0; j < top; j++)
            {
               // cout << s[j] << endl;
            }
        }
    }
    else
        throw "Stack_is_full";
}


int Stack_int::pop()
{

    if ( !is_empty() )
    {
        --top;
        return s[top];
    }
    else
        throw "Stack_is_empty";
}

Lex Stack_Lex::pop()
{
    /*cout <<"STACK DATA pop " << endl;
            for (int j = 0; j < top; j++)
            {
                cout << s[j] << endl;
            }*/
    if ( !is_empty() )
    {
        --top;
        return s[top];
    }
    else
        throw "Stack_is_empty";
}










/*
int main()
{

    if (0)
    {

        std::cout << "!!!";
    }
}

*/
