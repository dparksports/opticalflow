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

    string timestamp = year + "/" + month + "/" + date + "-" + hour + ":" + minute + ":" + second;
    return timestamp;
}


int main(int argc, const char *argv[]) {
    int max;
    cin >> max;

    vector<int> indices;
    vector<string> timestamps;

    map<string, string> filenames;
    string filename;

    for (int j = 0; j < max; ++j) {
        cin >> filename;
        int index = parseIndex(filename);
        string timestamp = parseTimestamp(filename);

        filenames[timestamp] = filename;
        timestamps.push_back(timestamp);
        indices.push_back(index);

    }

    sort(timestamps.begin(), timestamps.end());

    for (int i = 0; i < timestamps.size(); ++i) {
        cout << timestamps[i] + "  =  " + filenames[timestamps[i]] << endl;
    }
    return 0;
}
