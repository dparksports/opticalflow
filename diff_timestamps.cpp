//
//  main.cpp
//  transpose
//
//  Created by Dan Park on 8/3/19.
//  Copyright © 2019 Dan Park. All rights reserved.
//

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <map>

#include <cmath>
#include <cstdio>
#include <algorithm>
#include <cassert>

#include <exception>

#include <algorithm>

using namespace std;



int parseIndex(string str) {
    stringstream ss(str);
    string token;

    getline(ss, token, '-');
    getline(ss, token, '-');
    stringstream wordstream(token);

    int number;
    wordstream >> number;
    return number;
}

// Make a tm structure representing this date
tm make_tm(int year, int month, int day, int hour, int minute, int second)
{
    tm tm = {0};
    tm.tm_year = year - 1900; // years count from 1900
    tm.tm_mon = month - 1;    // months count from January=0
    tm.tm_mday = day;         // days count from 1
    return tm;
}

string parseTimestamp(string str) {
    stringstream ss(str);
    string index, year, month, date, hour, minute, second;


    getline(ss, index, '-');
    getline(ss, index, '-');
    stringstream wordstream1(index);

    getline(ss, year, '-');
    stringstream wordstream2(year);

    getline(ss, month, '-');
    stringstream wordstream3(month);

    getline(ss, date, '_');
    stringstream wordstream4(date);

    getline(ss, hour, '-');
    stringstream wordstream5(hour);

    getline(ss, minute, '-');
    stringstream wordstream6(minute);

    getline(ss, second, '.');
    stringstream wordstream7(second);

    string timestamp = year + "/" + month + "/" + date + "-" + hour + ":" + minute + ":" + second;
    return timestamp;
}

tm parseTM(string str) {
    stringstream ss(str);
    string index, year, month, date, hour, minute, second;
    int iyear, imonth, idate, ihour, iminute, isecond;

    getline(ss, index, '-');
    getline(ss, index, '-');
    stringstream wordstream1(index);

    getline(ss, year, '-');
    stringstream wordstream2(year);
    wordstream2 >> iyear;

    getline(ss, month, '-');
    stringstream wordstream3(month);
    wordstream3 >> imonth;

    getline(ss, date, '_');
    stringstream wordstream4(date);
    wordstream4 >> idate;

    getline(ss, hour, '-');
    stringstream wordstream5(hour);
    wordstream5 >> ihour;

    getline(ss, minute, '-');
    stringstream wordstream6(minute);
    wordstream6 >> iminute;

    getline(ss, second, '.');
    stringstream wordstream7(second);
    wordstream7 >> isecond;

    time_t now = time(0);
    tm *ltm = localtime(&now);
    tm tm = *ltm;

    tm.tm_year = iyear; // years count from 1900
    tm.tm_mon = imonth - 1;    // months count from January=0
    tm.tm_mday = idate;         // days count from 1
    tm.tm_hour = ihour;         // days count from 1
    tm.tm_min = iminute;         // days count from 1
    tm.tm_sec = isecond;         // days count from 1
    return tm;
}

ostream& operator<< (ostream& os, const tm& tm) {
    os << tm.tm_year << "/" << tm.tm_mon << "/" << tm.tm_mday << " " << tm.tm_hour << ":" << tm.tm_min << ":" << tm.tm_sec;
    return os;
}

int main(int argc, const char *argv[]) {
    int max;
    cin >> max;

    vector<string> timestamps;
    map<string, string> filenames;
    string filename;

    for (int j = 0; j < max; ++j) {
        cin >> filename;
        string timestamp = parseTimestamp(filename);

        filenames[timestamp] = filename;
        timestamps.push_back(timestamp);
    }

    sort(timestamps.begin(), timestamps.end());


    const int seconds_per_day = 60*60*24;
    time_t p_time = time(0);

    for (int i = 0; i < timestamps.size(); ++i) {
        string timestamp = timestamps[i];
        string filename = filenames[timestamps[i]];

        tm c_tm = parseTM(filename);
        time_t c_time = mktime(&c_tm);
        time_t difference = c_time - p_time;
        int diff = difference;
        p_time = c_time;

        cout << timestamp  << " = " << diff << " = " << filename << endl;
    }
    return 0;
}
