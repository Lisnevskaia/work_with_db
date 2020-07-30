/*добавление события - Add Дата Событие (дата везде год-месяц-день)
  удаление события -  Del Дата Событие
  удаление всех событий за конкретную дату - Del Дата
  поиск событий за конкретную дату - Find Дата
  печать всех событий за все даты - Print
 */
/*
 * пример:
    Add 1-1-2 event1 //добавления event1
    Add 1-2-3 event2 //добавлене event2
    Find 1-1-2       //поиск события по дате - event1

    Del 1-1-2        //удаление событий 1-1-2 (event1)
    Print            //печать событий, должно остаться только event2
    Del 1-2-3 event2 //удаления события 1-2-3(event2)
    Del 1-2-3 event2 //попытка удалить событие event2 - его уже не существует
 */
#include <iostream>
#include <set>
#include <map>
#include <sstream>
#include <exception>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

class Date
{
public:
    Date(const int& y, const int& m, const int& d)
    {
        year = y;
        if(m < 1 || m > 12)
        {
            throw logic_error("Month value is invalid: " + to_string(m));
        }
        month = m;
        if(d < 1 || d > 31)
        {
            stringstream err;
            err << "Day value is invalid: " << d;
            throw runtime_error(err.str());
        }
        day = d;
    }
    int Get_year() const
    {
        return year;
    }
    int Get_month() const
    {
        return month;
    }
    int Get_day() const
    {
        return day;
    }
private:
    int year;
    int month;
    int day;
};

bool operator < (const Date& f, const Date& s) //for map
{
    return vector<int>{f.Get_year(),f.Get_month(),f.Get_day()} <
            vector<int>{s.Get_year(),s.Get_month(),s.Get_day()};
}

ostream& operator<<(ostream& stream, const Date& date)
{
    stream<<setw(4) << setfill('0') << date.Get_year() << "-" << setw(2) << setfill('0') << date.Get_month() << "-" <<setw(2) << setfill('0') << date.Get_day();
    return stream;
}


Date MakeDate(const string& str)
{
    //cout << str.length() << endl;
    stringstream stream(str);
    if((str[str.length()-1] < 48 )|| (str[str.length()-1] > 57))
    {
        stringstream err;
        err << "Wrong date format: " << str;
        throw runtime_error(err.str());
    }
    int year;
    stream >> year;
    if(stream.peek() != '-')
    {
        stringstream err;
        err << "Wrong date format: " << str;
        throw runtime_error(err.str());
    }
    stream.ignore();
    int month;
    stream >> month;
    if(stream.peek() != '-')
    {
        stringstream err;
        err << "Wrong date format: " << str;
        throw runtime_error(err.str());
    }
    stream.ignore();
    int day;
    stream >> day;
    if(!stream.eof())
    {
        stringstream err;
        err << "Wrong date format: " << str;
        throw runtime_error(err.str());
    }
    Date tmp(year,month,day);
    return tmp;
}

class Database
{
public:
    void AddEvent(const Date& date, const string& event)//add event
    {
         result[date].insert(event);
    }
    bool DeleteEvent(const Date& date,const string& event)
    {
        if(result.count(date))
        {
            if(count(cbegin(result[date]), cend(result[date]), event))
            {
                result[date].erase(event);
                return true;
            }
            return false;
        }
    }
    int DeleteDate(const Date& date) //delete Date
    {
        int result_of_operation = 0;
        result_of_operation = result[date].size();
        result.erase(date);
        return result_of_operation;
    }
    void FindDate(const Date& date) const
    {
        if(result.count(date))
        {
            //set<string> tmp = result[date];
            for(const string& r : result.at(date))
                cout << r << endl;
        }
    }
    void PrintAll() const
    {
        for(const auto& r: result)
        {
            for(const auto& event : r.second)
                cout << r.first << " " <<  event << endl;
        }
    }
private:
    map<Date, set<string>> result;
};

int main()
{
    try
    {
        Database Result;
        string command;
        while(getline(cin, command))
        {
            if(!command.empty())
            {
                string str;
                stringstream stream(command);
                stream >> str;
                if(str == "Add" || str == "Find" || str == "Del")
                {
                    string tmp;
                    string event;
                    stream >> tmp;
                    Date date = MakeDate(tmp);
                    stream >> event;
                    if(str == "Add") {
                        Result.AddEvent(date, event);
                    }
                    else if(str == "Del")
                    {
                        if(!event.empty()) {
                            if (Result.DeleteEvent(date, event))
                                cout << "Deleted successfully" << endl;
                            else
                                cout << "Event not found" << endl;
                        } else
                            cout << "Deleted " << Result.DeleteDate(date) << " events" << endl;
                    }
                    else
                        Result.FindDate(date);

                }
                else if(str == "Print")
                {
                    Result.PrintAll();
                }
                else
                {
                    stringstream err;
                    err << "Unknown command: " << str;
                    throw runtime_error(err.str());
                }
            }
        }
    }
    catch(exception& ex)
    {
        cout << ex.what() << endl;
    }
    return 0;
}


