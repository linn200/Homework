#ifndef HEADER_H
#define HEADER_H

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

#endif