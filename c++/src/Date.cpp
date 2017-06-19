// MegaShow
#include <iomanip>
#include <sstream>
#include "Date.hpp"
using namespace std;


Date::Date() : m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0) {}


Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) 
    : m_year(t_year), m_month(t_month), m_day(t_day), m_hour(t_hour), m_minute(t_minute) {
    /*if (!isValid(*this)) {
        m_year = m_month = m_day = m_hour = m_minute = 0;
    }*/
}


Date::Date(const std::string &dateString) {
    *this = stringToDate(dateString);
}


int Date::getYear(void) const {
    return m_year;
}


void Date::setYear(const int t_year) {
    m_year = t_year;
}


int Date::getMonth(void) const {
    return m_month;
}


void Date::setMonth(const int t_month) {
    m_month = t_month;
}


int Date::getDay(void) const {
    return m_day;
}


void Date::setDay(const int t_day) {
    m_day = t_day;
}


int Date::getHour(void) const {
    return m_hour;
}


void Date::setHour(const int t_hour) {
    m_hour = t_hour;
}


int Date::getMinute(void) const {
    return m_minute;
}


void Date::setMinute(const int t_minute) {
    m_minute = t_minute;
}


bool Date::isValid(const Date &t_date) {
    if (t_date.m_year < 1000 || t_date.m_year > 9999) {
        return false;
    } else if (t_date.m_month < 1 || t_date.m_month > 12) {
        return false;
    } else if (t_date.m_hour < 0 || t_date.m_hour > 23) {
        return false;
    } else if (t_date.m_minute < 0 || t_date.m_minute > 59) {
        return false;
    } else {
        int arr[13] = {0, 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        if ((t_date.m_year % 4 == 0 && t_date.m_year % 100 != 0) || t_date.m_year % 400 == 0) {
            arr[2] = 29;
        } else {
            arr[2] = 28;
        }
        if (t_date.m_day < 1 || t_date.m_day > arr[t_date.m_month]) {
            return false;
        } else {
            return true;
        }
    }
}


Date Date::stringToDate(const std::string &t_dateString) {
    for (int i = 0; i < 16; i++) {
        if (i == 4 || i == 7) {
            if (t_dateString[i] != '-')
                return Date(0, 0, 0, 0, 0);
        } else if (i == 10) {
            if (t_dateString[i] != '/')
                return Date(0, 0, 0, 0, 0);
        } else if (i == 13) {
            if (t_dateString[i] != ':')
                return Date(0, 0, 0, 0, 0);
        } else {
            if (t_dateString[i] < '0' || t_dateString[i] > '9')
                return Date(0, 0, 0, 0, 0);
        }
    }
    stringstream ss;
    Date tmp;
    char c;
    ss << t_dateString;
    ss >> tmp.m_year >> c >> tmp.m_month >> c >> tmp.m_day >> c >> tmp.m_hour >> c >> tmp.m_minute;
    if (isValid(tmp)) {
        return tmp;
    } else {
        return Date(0, 0, 0, 0, 0);
    }
}


std::string Date::dateToString(const Date &t_date) {
    if (isValid(t_date)) {
        stringstream ss;
        ss << setw(4) << setfill('0') << t_date.m_year << '-'
           << setw(2) << setfill('0') << t_date.m_month << '-'
           << setw(2) << setfill('0') << t_date.m_day << '/'
           << setw(2) << setfill('0') << t_date.m_hour << ':'
           << setw(2) << setfill('0') << t_date.m_minute << setfill(' ');
        return ss.str();
    } else {
        return std::string("0000-00-00/00:00");
    }
}


Date& Date::operator=(const Date &t_date) {
    m_year = t_date.m_year;
    m_month = t_date.m_month;
    m_day = t_date.m_day;
    m_hour = t_date.m_hour;
    m_minute = t_date.m_minute;
    return *this;
}


bool Date::operator==(const Date &t_date) const {
    return m_year == t_date.m_year && m_month == t_date.m_month &&
        m_day == t_date.m_day && m_hour == t_date.m_hour &&
        m_minute == t_date.m_minute;
}


bool Date::operator<(const Date &t_date) const {
    if (m_year < t_date.m_year) {
        return true;
    } else if (m_year == t_date.m_year && m_month < t_date.m_month) {
        return true;
    } else if (m_year == t_date.m_year && m_month == t_date.m_month && m_day < t_date.m_day) {
        return true;
    } else if (m_year == t_date.m_year && m_month == t_date.m_month &&
        m_day == t_date.m_day && m_hour < t_date.m_hour) {
        return true;
    } else if (m_year == t_date.m_year && m_month == t_date.m_month &&
        m_day == t_date.m_day && m_hour == t_date.m_hour && m_minute < t_date.m_minute) {
        return true;
    } else {
        return false;
    }
}


bool Date::operator>(const Date &t_date) const {
    return !(*this == t_date || *this < t_date);
}


bool Date::operator>=(const Date &t_date) const {
    return *this == t_date || *this > t_date;
}


bool Date::operator<=(const Date &t_date) const {
    return *this == t_date || *this < t_date;
}



// === TEST ===
/*#ifdef MEGACODE
#include <iostream>
using namespace std;
int main() {
    // construct
    Date d1;
    Date d2(2017, 11, 3, 3, 4);
    Date d3(2017, 0, 3, 3, 4);
    Date d4("3017-12-31/12:25");
    Date d5("9999-11-1/15:30");
    Date d6(d4);
    Date d7;
    d7 = d6;
    Date d8 = d2;
    cout << "d1: " << Date::dateToString(d1) << endl
         << "d2: " << Date::dateToString(d2) << endl
         << "d3: " << Date::dateToString(d3) << endl
         << "d4: " << Date::dateToString(d4) << endl
         << "d5: " << Date::dateToString(d5) << endl
         << "d6: " << Date::dateToString(d6) << endl
         << "d7: " << Date::dateToString(d7) << endl
         << "d8: " << Date::dateToString(d8) << endl << endl;
    
    // get/set
    cout << Date::dateToString(d2) << endl
         << d2.getYear() << ' ' << d2.getMonth() << ' ' << d2.getDay() << ' ' 
         << d2.getHour() << ' ' << d2.getMinute() << endl;
    d2.setYear(d2.getYear()-1);
    d2.setMonth(d2.getMonth()+1);
    d2.setDay(d2.getDay()+3);
    d2.setHour(d2.getHour()+10);
    d2.setMinute(d2.getMinute()+10);
    cout << d2.getYear() << ' ' << d2.getMonth() << ' ' << d2.getDay() << ' ' << d2.getHour() << ' ' << d2.getMinute() << endl;
    cout << Date::dateToString(d2) << endl << endl;

    // isValid
    cout << "true: " << Date::isValid(Date("9999-12-31/23:59")) << endl
         << "true: " << Date::isValid(Date("1000-01-01/00:00")) << endl
         << "true: " << Date::isValid(Date(9999,12,31,23,59)) << endl
         << "true: " << Date::isValid(Date(1000,1,1,0,0)) << endl
         << "true: " << Date::isValid(Date(2000,2,29,0,0)) << endl
         << "true: " << Date::isValid(Date(2004,2,29,0,0)) << endl
         << "false: " << Date::isValid(Date(1900,2,29,0,0)) << endl
         << "false: " << Date::isValid(Date(1900,4,31,0,0)) << endl
         << endl;
    
    // str/Date
    cout << Date::dateToString(Date::stringToDate("2012-3-31/05:12")) << endl
         << Date::dateToString(Date::stringToDate("2012-03-31/05:12")) << endl
         << Date::dateToString(Date::stringToDate("2012-03-32/05:12")) << endl
         << Date::dateToString(Date::stringToDate("2012-03-00/05:12")) << endl << endl;

    // Operator
    cout << "true: " << (d2==Date("2016-12-06/13:14")) << endl
         << "true: " << (d2<Date("2016-12-07/09:00")) << endl
         << "true: " << (d2>Date("2016-11-07/09:00")) << endl
         << "true: " << (d2<=Date("2016-12-06/13:14")) << endl
         << "true: " << (d2<=Date("2016-12-07/09:00")) << endl
         << "true: " << (d2>=Date("2016-12-06/13:14")) << endl
         << "true: " << (d2>=Date("2016-11-07/09:00")) << endl
         << "false: " << (d8==d2) << endl
         << "false: " << (d8<=d2) << endl
         << "false: " << (d2>=d8) << endl
         << "false: " << (d2>d2) << endl
         << "false: " << (d8<d8) << endl << endl;
}
#endif*/
