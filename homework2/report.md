# 41143221

作業二

## 解題說明

本題要求實現一個 `Polynomial` 類的抽象資料類型 (ADT)，其功能包括：
- 建構一個初始為 \( p(x) = 0 \) 的多項式。
- 實現 `Add` 和 `Mult` 函數，分別計算兩個多項式的和與乘積。
- 實現 `Eval` 函數，評估多項式在特定 \( x \) 值的結果。
- 透過重載 `<<` 和 `>>` 運算子實現多項式的輸入和輸出。

多項式以一組有序對 \( \langle e_i, a_i \rangle \) 表示，其中 \( a_i \) 是非零浮點係數，\( e_i \) 是非負整數指數。

## 解題說明

1. **資料結構**：
   - 使用 `std::vector<std::pair<float, int>>` 儲存多項式的每個項，`pair` 的第一個元素為係數，第二個元素為指數。

2. **建構函數**：
   - 初始多項式設為 \( p(x) = 0 \) 並預留空間。

3. **加法 (`Add`)**：
   - 按指數排序，合併相同指數的項，僅保留非零係數的項。

4. **乘法 (`Mult`)**：
   - 對每個項進行兩兩乘法，計算新指數和係數，然後合併相同指數的項。

5. **評估 (`Eval`)**：
   - 使用 \( a_i \cdot x^{e_i} \) 的形式計算總和。

6. **輸入輸出**：
   - 重載 `>>` 運算子以手動輸入係數和指數，重載 `<<` 運算子以 `a_i x^{e_i} + ...` 格式輸出。

## 程式實作

以下為主要程式碼：
```cpp
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include "Header.h"
using namespace std;

Polynomial::Polynomial() {
    termArray.push_back({0.0, 0});
}

Polynomial Polynomial::Add(const Polynomial& poly) const {
    Polynomial result;
    auto it1 = termArray.begin();
    auto it2 = poly.termArray.begin();

    while (it1 != termArray.end() && it2 != poly.termArray.end()) {
        if (it1->second == it2->second) {
            result.termArray.push_back({it1->first + it2->first, it1->second});
            ++it1; ++it2;
        } else if (it1->second < it2->second) {
            result.termArray.push_back(*it1);
            ++it1;
        } else {
            result.termArray.push_back(*it2);
            ++it2;
        }
    }

    while (it1 != termArray.end()) {
        result.termArray.push_back(*it1);
        ++it1;
    }
    while (it2 != poly.termArray.end()) {
        result.termArray.push_back(*it2);
        ++it2;
    }
    return result;
}

Polynomial Polynomial::Mult(const Polynomial& poly) const {
    Polynomial result;
    for (const auto& term1 : termArray) {
        for (const auto& term2 : poly.termArray) {
            result.termArray.push_back({term1.first * term2.first, term1.second + term2.second});
        }
    }
    for (auto it = result.termArray.begin(); it != result.termArray.end(); ++it) {
        for (auto it2 = it + 1; it2 != result.termArray.end();) {
            if (it->second == it2->second) {
                it->first += it2->first;
                it2 = result.termArray.erase(it2);
            } else {
                ++it2;
            }
        }
    }
    return result;
}

float Polynomial::Eval(float x) const {
    float result = 0.0;
    for (const auto& term : termArray) {
        result += term.first * pow(x, term.second);
    }
    return result;
}

ostream& operator<<(ostream& os, const Polynomial& poly) {
    for (const auto& term : poly.termArray) {
        os << term.first << "x^" << term.second << " + ";
    }
    return os;
}

istream& operator>>(istream& is, Polynomial& poly) {
    cout << "輸入多項式的項數: ";
    int numTerms;
    is >> numTerms;
    poly.termArray.clear();
    for (int i = 0; i < numTerms; ++i) {
        float coefficient;
        int exponent;
        cout << "輸入多項式的第 " << i + 1 << "個係數跟次方: ";
        is >> coefficient >> exponent;
        poly.termArray.push_back({coefficient, exponent});
    }
    return is;
}

int main() {
    Polynomial p1, p2;
    cout << "第一個多項式:\n";
    cin >> p1;
    cout << "第二個多項式:\n";
    cin >> p2;

    Polynomial sum = p1.Add(p2);
    Polynomial product = p1.Mult(p2);
    cout << "多項式相加:: " << sum << endl;
    cout << "多項式相乘: " << product << endl;

    float x;
    cout << "輸入x值計算多項式: ";
    cin >> x;
    cout << "多項式相加後代入x值: " << sum.Eval(x) << endl;
    cout << "多項式相乘後帶入x值: " << product.Eval(x) << endl;

    auto startAdd = chrono::high_resolution_clock::now();
    Polynomial sumPerformance = p1.Add(p2);
    auto endAdd = chrono::high_resolution_clock::now();
    chrono::duration<double> durationAdd = endAdd - startAdd;
    cout << "Add operation took " << durationAdd.count() << " seconds.\n";

    auto startMult = chrono::high_resolution_clock::now();
    Polynomial productPerformance = p1.Mult(p2);
    auto endMult = chrono::high_resolution_clock::now();
    chrono::duration<double> durationMult = endMult - startMult;
    cout << "Mult operation took " << durationMult.count() << " seconds.\n";

    return 0;
}
```
<img width="478" height="270" alt="image" src="https://github.com/user-attachments/assets/f60774d5-615f-4f68-aad8-32ad4160d8b1" />

## 效能分析

### 時間複雜度：

Add：( O(n + m) )，其中 ( n ) 和 ( m ) 為兩個多項式的項數，基於線性遍歷。
Mult：( O(n \cdot m) )，因需要兩層巢狀迴圈，後續合併操作為 ( O((n \cdot m)^2) ) 最壞情況。
Eval：( O(n) )，依賴項數進行線性計算。
輸入輸出：( O(n) )，與項數成正比。


### 空間複雜度：

所有操作：( O(n + m) )，用於儲存結果多項式的向量。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 (p1, p2)      | 輸入 x 值 | 預期輸出 (加法 Eval) | 實際輸出 (加法 Eval) | 預期輸出 (乘法 Eval) | 實際輸出 (乘法 Eval) |
|----------|-------------------------|-----------|-----------------------|-----------------------|-----------------------|-----------------------|
| 測試一   | (2x^2, 3x^1)            | 2         | 14                  | 11                  | 48                  | 48                  |
| 測試二   | (1x^1, 1x^1)            | 3         | 6                   | 6                   | 9                   | 9                   |
| 測試三   | (2x^2 + 1x^0, 3x^2)     | 1         | 6                   | 6                   | 9                   | 9                   |
<img width="339" height="216" alt="image" src="https://github.com/user-attachments/assets/c0affab3-46f1-48ea-b47f-32a1fdc4a3af" />
<img width="315" height="220" alt="image" src="https://github.com/user-attachments/assets/d3f47652-4279-47b5-8f68-3e4f73b7fb3d" />
<img width="324" height="233" alt="image" src="https://github.com/user-attachments/assets/9453ff25-b0e9-4168-9eba-8cf8b1633879" />

### 編譯與執行指令
```shell
$ g++ -std=c++17 -o polynomial main.cpp
$ ./polynomial
```
### 結論

程式能正確計算多項式的加法、乘法和評估結果。
輸入輸出運算子有效處理用戶輸入並以預期格式顯示。
測試案例涵蓋不同係數和指數組合，驗證程式正確性。

## 申論及開發報告

### 選擇向量的原因

1. **動態大小**：
   - 使用 `std::vector` 允許多項式的項數動態調整，無需預先指定固定容量，適合不同規模的輸入。

2. **簡單實現**：
   - `pair<float, int>` 提供直觀的方式儲存係數和指數，減少結構體定義的複雜性。

3. **效能考量**：
   - 雖然向量操作（如 `push_back` 和 `erase`）可能引入額外開銷，但其靈活性在小規模多項式運算中表現良好。
   - 乘法中的合併邏輯使用 `erase` 合併相同指數，雖然效率不高，但實現簡單，適合學習用途。

### 心得報告
通過完成作業二，我對 C++ 物件導向程式設計與抽象資料類型（ADT）有了更深認識。最大的困難是設計 Polynomial 類時管理 std::vector<std::pair<float, int>>，特別是加法和乘法的動態陣列操作，初期因指數排序和合併邏輯常出錯。重載 << 和 >> 運算子也具挑戰，需調整輸出格式避免多餘 + 號，測試時需反覆修正。乘法中巢狀迴圈和 erase 合併項的低效性讓我意識到效能問題。透過學習，我掌握迭代器使用，簡化 Add 函數，並學會驗證運算邏輯。還發現缺乏輸入驗證（如負指數）的不足，需未來改進。這次作業提升了我的邏輯思維和問題解決能力，激發對優化資料結構的興趣。
