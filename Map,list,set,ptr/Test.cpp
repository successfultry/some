#include <iostream>
#include <vector>
#include <memory>
#include <cstdio>
#include <fstream>
#include <cassert>
#include <functional>
#include <list>
#include <map>
#include <set>

#include "Shared_ptr.h"
#include "Unique_ptr.h"
#include "Iterator.h"
#include "List.h"
#include "Set.h"
#include "Map.h"
using namespace std;




int main()
{
    /*********************** Test for map ***********************/
   /*
    Map<int, char>m;

    m.insert(10, 'a');
    m.insert(5,  'b');
    m.insert(15, 'c');
    m.insert(20, 'd');
    m.insert(25, 'e');
    m.insert(13, 'f');
    m.insert(1,  'g');
    m.insert(7,  'h');
    m.insert(6,  'j');
    m.insert(9,  'k');
    m.insert(11, 'l');
    m.insert(14, 'm');

    m[11] = 'x';
    m[13] = 'y';
    m[9] = 'z';
    m[200] = '0';
    m.PrintMap();
    cout << endl;
    cout << m[300];
    cout << endl;

    m.erase(10);
    m.PrintMap();
    cout << endl;

    m.erase(11);
    m.PrintMap();
    cout << endl;

    m.erase(7);
    m.PrintMap();
         
   */
    /*********************** Test for set***********************/
    
    
    Set<int>s;

    s.insert(10);
    s.insert(5);
    s.insert(15);
    s.insert(20);
    s.insert(25);
    s.insert(13);
    s.insert(1);
    s.insert(7);
    s.insert(6);
    s.insert(9);
    s.insert(11);
    s.insert(14);

    s.erase(13);
    s.PrintSet();

   

   /*********************** Test for list N1***********************/

    /*List<int> l(1, 100);
    l.insert(l.End(), 200);
    l.insert(l.End(), 300);


    l.erase(l.Begin());
   // l.erase(l.End()); - it must not work
    for (Iterator<int> it = l.Begin(); it != l.End(); ++it)
    {
        cout << *it << endl;
    }

    l.insert(l.Begin(), 100);
    l.insert(l.End(), 400);
    for (Iterator<int> it = l.Begin(); it != l.End(); ++it)
    {
        cout << *it << endl;
    }*/


    /*********************** Test for list N2***********************/
    /*
    List<int> k;
    k.erase(k.Begin());
    for (Iterator<int> it = k.Begin(); it != k.End(); ++it)
    {
        cout << *it << endl;
    }

    */

    /*********************** Test for list N3***********************/
    /*
    List<int> k;
    List<int>l;


    k.insert(k.End(), 800);
    for (Iterator<int> it = k.Begin(); it != k.End(); ++it)
    {
        cout << *it << endl;
    }

    int a = 20;
    for (Iterator<int> it = l.Begin(); it != l.End(); ++it)
    {
        *it = ++a;
    }
    for (Iterator<int> it = l.Begin(); it != l.End(); ++it)
    {
        cout << *it << endl;
    }
    Iterator<int> it = l.Begin();
    //    ++it;
    l.insert(it, 119);
    for (Iterator<int> it = l.Begin(); it != l.End(); ++it)
    {
        cout << *it << endl;*/
    
    //    cout << k.front() << endl;


    return 0;
}









