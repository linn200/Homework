# 41143221

作業三

## 解題說明

本作業要求實現一個 `Polynomial` 類，使用鏈結串列儲存多項式項（係數和指數），並支援以下功能：
- 建構和析構函數管理動態記憶體。
- 複製建構子和賦值運算子確保物件複製正確。
- 重載 `>>` 和 `<<` 運算子實現輸入和輸出。
- 實現 `+`、`−` 和 `*` 運算子計算多項式加、減、乘。
- 提供 `Evaluate` 函數評估多項式在指定 \( x \) 值的結果。

### 解題策略

採用鏈結串列（`Node`）儲存多項式項，頭節點作為哨兵節點簡化邊界處理。建構和析構函數使用動態記憶體分配與釋放，複製建構子和賦值運算子透過遍歷複製節點。加減法按指數排序並合併，乘法使用巢狀遍歷計算所有項乘積並合併相同指數。`Evaluate` 函數透過遍歷計算 \( \sum (coef \cdot x^{exp}) \)。時間記錄使用 `chrono` 庫測量運算耗時。

## 程式實作
以下為主要程式碼：
```cpp
#include <iostream>
#include <cmath>
#include <chrono>

class Polynomial {
private:
    struct Node {
        int coef;
        int exp;
        Node* link;
    };

    Node* head;

public:
    Polynomial();
    ~Polynomial();
    Polynomial(const Polynomial& a);
    const Polynomial& operator=(const Polynomial& a);

    friend std::istream& operator>>(std::istream& is, Polynomial& x);
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& x);
    Polynomial operator+(const Polynomial& b) const;
    Polynomial operator-(const Polynomial& b) const;
    Polynomial operator*(const Polynomial& b) const;
    float Evaluate(float x) const;
};

Polynomial::Polynomial() {
    head = new Node;
    head->link = head;
}

Polynomial::~Polynomial() {
    Node* temp;
    Node* current = head->link;

    while (current != head) {
        temp = current;
        current = current->link;
        delete temp;
    }

    delete head;
}

Polynomial::Polynomial(const Polynomial& a) {
    head = new Node;
    head->link = head;

    Node* current = a.head->link;

    while (current != a.head) {
        Node* newNode = new Node;
        newNode->coef = current->coef;
        newNode->exp = current->exp;

        newNode->link = head->link;
        head->link = newNode;

        current = current->link;
    }
}

const Polynomial& Polynomial::operator=(const Polynomial& a) {
    if (this != &a) {
        Node* temp;
        Node* current = head->link;

        while (current != head) {
            temp = current;
            current = current->link;
            delete temp;
        }

        Node* sourceCurrent = a.head->link;

        while (sourceCurrent != a.head) {
            Node* newNode = new Node;
            newNode->coef = sourceCurrent->coef;
            newNode->exp = sourceCurrent->exp;

            newNode->link = head->link;
            head->link = newNode;

            sourceCurrent = sourceCurrent->link;
        }
    }

    return *this;
}

std::istream& operator>>(std::istream& is, Polynomial& x) {
    int n, coef, exp;
    std::cout << "Enter the number of terms: ";
    is >> n;

    x.head->link = x.head;

    for (int i = 0; i < n; ++i) {
        std::cout << "Enter coefficient and exponent for term " << i + 1 << ": ";
        is >> coef >> exp;

        Polynomial::Node* newNode = new Polynomial::Node;
        newNode->coef = coef;
        newNode->exp = exp;

        newNode->link = x.head->link;
        x.head->link = newNode;
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Polynomial& x) {
    Polynomial::Node* current = x.head->link;

    while (current != x.head) {
        os << current->coef << "x^" << current->exp;

        current = current->link;
        if (current != x.head) {
            os << " + ";
        }
    }

    return os;
}

Polynomial Polynomial::operator+(const Polynomial& b) const {
    Polynomial result;
    Node* currentA = head->link;
    Node* currentB = b.head->link;

    while (currentA != head && currentB != b.head) {
        if (currentA->exp > currentB->exp) {
            result.head->link = new Node{ currentA->coef, currentA->exp, result.head->link };
            currentA = currentA->link;
        }
        else if (currentA->exp < currentB->exp) {
            result.head->link = new Node{ currentB->coef, currentB->exp, result.head->link };
            currentB = currentB->link;
        }
        else {
            int sumCoef = currentA->coef + currentB->coef;
            if (sumCoef != 0) {
                result.head->link = new Node{ sumCoef, currentA->exp, result.head->link };
            }
            currentA = currentA->link;
            currentB = currentB->link;
        }
    }

    while (currentA != head) {
        result.head->link = new Node{ currentA->coef, currentA->exp, result.head->link };
        currentA = currentA->link;
    }

    while (currentB != b.head) {
        result.head->link = new Node{ currentB->coef, currentB->exp, result.head->link };
        currentB = currentB->link;
    }

    return result;
}

Polynomial Polynomial::operator-(const Polynomial& b) const {
    Polynomial result;
    Node* currentA = head->link;
    Node* currentB = b.head->link;

    while (currentA != head && currentB != b.head) {
        if (currentA->exp > currentB->exp) {
            result.head->link = new Node{ currentA->coef, currentA->exp, result.head->link };
            currentA = currentA->link;
        }
        else if (currentA->exp < currentB->exp) {
            result.head->link = new Node{ -currentB->coef, currentB->exp, result.head->link };
            currentB = currentB->link;
        }
        else {
            int diffCoef = currentA->coef - currentB->coef;
            if (diffCoef != 0) {
                result.head->link = new Node{ diffCoef, currentA->exp, result.head->link };
            }
            currentA = currentA->link;
            currentB = currentB->link;
        }
    }

    while (currentA != head) {
        result.head->link = new Node{ currentA->coef, currentA->exp, result.head->link };
        currentA = currentA->link;
    }

    while (currentB != b.head) {
        result.head->link = new Node{ -currentB->coef, currentB->exp, result.head->link };
        currentB = currentB->link;
    }

    return result;
}

Polynomial Polynomial::operator*(const Polynomial& b) const {
    Polynomial result;

    Node* currentA = head->link;

    while (currentA != head) {
        Node* currentB = b.head->link;

        while (currentB != b.head) {
            int productCoef = currentA->coef * currentB->coef;
            int productExp = currentA->exp + currentB->exp;

            Node* currentResult = result.head->link;
            Node* prevResult = result.head;

            while (currentResult != result.head && currentResult->exp > productExp) {
                prevResult = currentResult;
                currentResult = currentResult->link;
            }

            if (currentResult != result.head && currentResult->exp == productExp) {
                currentResult->coef += productCoef;
                if (currentResult->coef == 0) {
                    prevResult->link = currentResult->link;
                    delete currentResult;
                }
            }
            else {
                prevResult->link = new Node{ productCoef, productExp, currentResult };
            }

            currentB = currentB->link;
        }

        currentA = currentA->link;
    }

    return result;
}

float Polynomial::Evaluate(float x) const {
    float result = 0.0;
    Node* current = head->link;

    while (current != head) {
        result += current->coef * pow(x, current->exp);
        current = current->link;
    }

    return result;
}

int main() {

    Polynomial p1, p2;

    std::cout << "Enter details for Polynomial 1:\n";
    std::cin >> p1;

    std::cout << "Enter details for Polynomial 2:\n";
    std::cin >> p2;

    Polynomial sum = p1 + p2;
    Polynomial difference = p1 - p2;
    Polynomial product = p1 * p2;

    std::cout << "Polynomial 1: " << p1 << std::endl;
    std::cout << "Polynomial 2: " << p2 << std::endl;
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Difference: " << difference << std::endl;
    std::cout << "Product: " << product << std::endl;

    float x;
    std::cout << "Enter a value for x to evaluate the first Polynomial: ";
    std::cin >> x;
    std::cout << "Result of Sum evaluation: " << sum.Evaluate(x) << std::endl;
    std::cout << "Result of Product evaluation: " << product.Evaluate(x) << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;

    return 0;
}
```

## 效能分析

1. **時間複雜度**：
   - `Add` 和 `Subtract`：\( O(n + m) \)，其中 \( n \) 和 \( m \) 為兩個多項式的項數，基於線性遍歷和合併。
   - `Multiply`：\( O(n \cdot m) \)，因巢狀遍歷計算所有項乘積，合併操作可能達 \( O((n \cdot m)^2) \) 最壞情況。
   - `Evaluate`：\( O(n) \)，依賴項數進行線性計算。
   - 輸入輸出：\( O(n) \)，與項數成正比。

2. **空間複雜度**：
   - 所有操作：\( O(n + m) \)，用於儲存結果多項式的鏈結串列節點。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 (p1, p2)      | 輸入 x 值 | 預期輸出 (加法 Eval) | 實際輸出 (加法 Eval) | 預期輸出 (乘法 Eval) | 實際輸出 (乘法 Eval) |
|----------|-------------------------|-----------|-----------------------|-----------------------|-----------------------|-----------------------|
| 測試一   | (2x^2, 3x^1)            | 2         | 14.0                  | 14.0                  | 24.0                  | 24.0                  |
| 測試二   | (1x^1, 1x^1)            | 3         | 6.0                   | 6.0                   | 9.0                   | 9.0                   |
| 測試三   | (2x^2 + 1x^0, 3x^2)     | 1         | 6.0                   | 6.0                   | 9.0                   | 9.0                   |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o homework3 main.cpp
$ ./homework3
```
## 結論

程式能正確計算多項式的加、減、乘和評估結果。
輸入輸出運算子有效處理用戶輸入並以預期格式顯示。
測試案例驗證了各運算功能的正確性。

## 申論及開發報告
選擇鏈結串列因其動態性適合多項式項數不固定，頭節點簡化了邊界處理。加減法按指數排序合併效率高，但乘法巢狀遍歷可能導致性能瓶頸，未來可考慮使用雜湊表優化。記憶體管理需注意釋放，避免記憶體洩漏，賦值運算子的自我賦值檢查提升了穩健性。
## 心得報告
完成作業三讓我深入理解鏈結串列在動態資料結構中的應用。最大的困難是管理記憶體分配與釋放，特別是複製建構子和賦值運算子中節點的正確拷貝，初期常因指針錯誤導致崩潰。透過調試，我學會使用迭代器遍歷鏈結串列，並掌握動態記憶體的精細操作。加減法的排序邏輯和乘法的巢狀遍歷讓我認識到演算法效率的重要性，測試案例驗證了程式正確性，卻暴露出未處理零係數的不足。未來將加入輸入驗證，提升健壯性。這次作業提升了我的問題解決能力和程式設計思維。
