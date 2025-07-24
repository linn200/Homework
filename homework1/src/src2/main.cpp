#include <iostream>
#include <vector>
#include <string>
#include <algorithm> 
#include "Header.h"
using namespace std;

void powerset(const string& s, int index, string current, vector<string>& result) {
    
    result.push_back(current);

    for (int i = index; i < s.length(); i++) {
        current += s[i]; 
        powerset(s, i + 1, current, result); 
        current.pop_back(); 
    }
}

int main() {
    string s;
    cout << "Enter the set elements (e.g., 'abc'): ";
    cin >> s;

    vector<string> result;
    powerset(s, 0, "", result);

   
    sort(result.begin(), result.end());

    cout << "Powerset of {" << s << "} = { ";
    for (size_t i = 0; i < result.size(); i++) {
        cout << "{";
        for (size_t j = 0; j < result[i].length(); j++) {
            cout << result[i][j];
            if (j < result[i].length() - 1) cout << ", ";
        }
        cout << "}";
        if (i < result.size() - 1) cout << ", ";
    }
    cout << " }" << endl;

    return 0;
}