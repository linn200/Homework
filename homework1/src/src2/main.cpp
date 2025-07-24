#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // 為 sort 使用
#include "Header.h"
using namespace std;

void powerset(const string& s, int index, string current, vector<string>& result) {
    // 將當前子集加入結果
    result.push_back(current);

    // 從當前索引開始，遞迴添加每個元素
    for (int i = index; i < s.length(); i++) {
        current += s[i]; // 包含當前元素
        powerset(s, i + 1, current, result); // 遞迴處理剩餘元素
        current.pop_back(); // 回溯，移除當前元素以嘗試下一個組合
    }
}

int main() {
    string s;
    cout << "Enter the set elements (e.g., 'abc'): ";
    cin >> s;

    vector<string> result;
    powerset(s, 0, "", result);

    // 將結果按字典序排序
    sort(result.begin(), result.end());

    // 輸出所有子集，使用逗號分隔元素
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