//
//  main.cpp
//  transpose
//
//  Created by Dan Park on 6/19/19.
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


void scanString() {
    string s1, s2;
    cin >> s1;
    cin >> s2;
//    scanf("%s", &s1);
//    scanf("%s", &s2);

    int len1 = s1.size();
    int len2 = s2.size();


    cout << len1 << " " << len2 << endl;
    cout << s1 + s2 << endl;

    char c1 = s2[0];
    s2[0] = s1[0];
    s1[0] = c1;

    cout << s1 << " " << s2;
    cout << endl;
}

vector<int> parseInts(string str) {
    stringstream ss(str);
    string token;

    vector<int> ints;
    while (getline(ss, token, '-')) {
        stringstream wordstream(token);

        int number;
        wordstream >> number;
        ints.push_back(number);
    }

    return ints;
}

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

int main(int argc, const char *argv[]) {
    vector<int> indices;
    map<int, string> filenames;
    string filename;

    // 1016900
    int max = 10;
    for (int i = 0; i < max; ++i) {
        cin >> filename;
        int index = parseIndex(filename);
        filenames[index] = filename;
        indices.push_back(index);
    }

    sort(indices.begin(), indices.end());
    cout << endl;

    for (int i = 0; i < indices.size(); ++i) {
        cout << filenames[indices[i]] << endl;
    }
    cout << endl;
    return 0;
}
