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

int main(int argc, const char *argv[]) {
    int max;
    cin >> max;

    vector<int> indices;
    map<int, string> filenames;
    string filename;

    for (int j = 0; j < max; ++j) {
        cin >> filename;
        int index = parseIndex(filename);
        filenames[index] = filename;
        indices.push_back(index);
    }

    sort(indices.begin(), indices.end());
    for (int i = 0; i < indices.size(); ++i) {
        cout << filenames[indices[i]] << endl;
    }
    return 0;
}
