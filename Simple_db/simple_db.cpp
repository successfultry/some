#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <sstream>
#include <iomanip>
using namespace std;

class Date {
public:
  int GetYear() const {return year;}
  int GetMonth() const {return month;}
  int GetDay() const {return day;}

  Date(int y, int m, int d): year(y),month(m),day(d){};

  Date(const string& s)
  {
      istringstream ss(s);
      char c;

      ss >> year;
      if(!ss)
      {
          throw "Wrong date format: " + s;
      }

      ss >> c;
      if(c != '-')
      {
           throw "Wrong date format: " + s;
      }

      ss >> month;
      if(!ss)
      {
          throw "Wrong date format: " + s;
      }



      ss >> c;
      if(c != '-')
      {
           throw "Wrong date format: " + s;
      }


      ss >> day;
      if(!ss)
      {
          throw "Wrong date format: " + s;
      }

      ss >> c;
      if(ss)
      {
          throw "Wrong date format: " + s;
      }

      if((month > 12) || (month < 1))
      {
          throw "Month value is invalid: " + to_string(month);
      }
      else if((day > 31) || (day < 1))
      {
          throw "Day value is invalid: " + to_string(day);
      }

  };

private:
    int year;
    int month;
    int day;
};


bool operator<(const Date& lhs, const Date& rhs)
{
    if(lhs.GetYear() != rhs.GetYear())
    {
        return lhs.GetYear()<rhs.GetYear();
    }
    else if(lhs.GetMonth() != rhs.GetMonth())
    {
        return lhs.GetMonth()<rhs.GetMonth();
    }
    else if(lhs.GetDay() != rhs.GetDay())
    {
        return lhs.GetDay()<rhs.GetDay();
    }

    return false;
}

class Database {
public:
  void AddEvent(const Date& date, const string& event)
  {
      if(db.find(date) == db.end())
       {
            db[date].insert(event);
       }

      else
       {
            db[date].insert(event);
       }
  }


  bool DeleteEvent(const Date& date, const string& event)
  {
      if(db.find(date) == db.end())
      {
        cout << "Event not found" << endl;
        return false;
      }

      for(const auto& val : db.at(date))
      {
          if(val == event)
          {
              db[date].erase(val);
              cout << "Deleted successfully" << endl;
              return true;
          }
      }

    cout << "Event not found" << endl;
    return false;


  };


  int  DeleteDate(const Date& date)
  {

      if(db.find(date) == db.end())
      {
        cout << "Deleted 0 events" << endl;
        return 0;
      }


     int i = db.at(date).size();
     cout << "Deleted " <<  i << " events" << endl;
     db.erase(date);
     return i;


  };


  void Find(const Date& date) const
  {
      if(db.find(date) == db.end())
      {
        return ;
      }
      for(const auto& val : db.find(date)->second)
      {
          cout << val << endl;
      }

  };



  void Print() const
  {
      for(const auto& key: db)
      {
        for(const auto& val: key.second)
        {
             cout << setfill('0') << setw(4) << key.first.GetYear() << '-'
             << setfill('0') << setw(2) << key.first.GetMonth() << '-'
             << setfill('0') << setw(2) << key.first.GetDay() << " ";

             cout << val << endl;
        }

      }
  };

private:
    map<Date,set<string>> db;
};

int main() {
  Database db;

  string command;
  while (getline(cin, command))
  {
      istringstream tmp(command);

      string cmd,word;

      tmp >> cmd;
      if(!tmp)
      {
          continue;
      }

      else if(cmd == "Add")
      {

        string s;
        tmp >> s;

        try
        {
            Date date(s);
            tmp >> word;
            db.AddEvent(date,word);
        }
        catch(const string& except)
        {
            cout << except << endl;
            return 1;
        }


      }

     else if(cmd == "Print")
     {
         db.Print();
     }


     else if(cmd == "Find")
     {

        string s;
        tmp >> s;

        try
        {
            Date date(s);
            tmp >> word;
            db.Find(date);
        }
        catch(const string& except)
        {
            cout << except << endl;
            return 2;
        }

     }

     else if(cmd == "Del")
     {

        string s;
        tmp >> s;

        try
        {
            Date date(s);
            tmp >> word;
            if(!tmp)
            {
                db.DeleteDate(date);
            }
            else
            {
                db.DeleteEvent(date, word);
            }
        }
        catch(const string& except)
        {
            cout << except << endl;
            return 3;
        }

     }

    else
    {
       // string un_command;
       // tmp >> un_command;
        cout << "Unknown command: " << cmd << endl;
        return 4;
    }


  }

  return 0;
}
