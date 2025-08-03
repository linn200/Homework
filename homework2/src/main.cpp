#include <iostream>
#include <vector>
#include <cmath> // �� pow ���
#include <chrono>
#include "Header.h"
using namespace std;

Polynomial::Polynomial() {
    termArray.push_back({ 0.0, 0 });
}

Polynomial Polynomial::Add(const Polynomial& poly) const {
    Polynomial result;

    auto it1 = termArray.begin();
    auto it2 = poly.termArray.begin();

    while (it1 != termArray.end() && it2 != poly.termArray.end()) {
        if (it1->second == it2->second) {
            result.termArray.push_back({ it1->first + it2->first, it1->second });
            ++it1;
            ++it2;
        }
        else if (it1->second < it2->second) {
            result.termArray.push_back(*it1);
            ++it1;
        }
        else {
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
            result.termArray.push_back({ term1.first * term2.first, term1.second + term2.second });
        }
    }

    for (auto it = result.termArray.begin(); it != result.termArray.end(); ++it) {
        for (auto it2 = it + 1; it2 != result.termArray.end();) {
            if (it->second == it2->second) {
                it->first += it2->first;
                it2 = result.termArray.erase(it2);
            }
            else {
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
    cout << "��J�h����������: ";
    int numTerms;
    is >> numTerms;

    poly.termArray.clear();

    for (int i = 0; i < numTerms; ++i) {
        float coefficient;
        int exponent;
        cout << "��J�h�������� " << i + 1 << "�ӫY�Ƹ򦸤�: ";
        is >> coefficient >> exponent;
        poly.termArray.push_back({ coefficient, exponent });
    }

    return is;
}

int main() {
    Polynomial p1, p2;

    cout << "�Ĥ@�Ӧh����:\n";
    cin >> p1;

    cout << "�ĤG�Ӧh����:\n";
    cin >> p2;

    Polynomial sum = p1.Add(p2);
    Polynomial product = p1.Mult(p2);

    cout << "�h�����ۥ[:: " << sum << endl;
    cout << "�h�����ۭ�: " << product << endl;

    float x;
    cout << "��Jx�ȭp��h����: ";
    cin >> x;

    cout << "�h�����ۥ[��N�Jx��: " << sum.Eval(x) << endl;
    cout << "�h�����ۭ���a�Jx��: " << product.Eval(x) << endl;

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