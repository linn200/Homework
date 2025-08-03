#ifndef HEADER_H
#define HEADER_H

#include <vector>
#include <iostream>
#include <chrono>

class Polynomial {
private:
    std::vector<std::pair<float, int>> termArray;

public:
    Polynomial();
    Polynomial Add(const Polynomial& poly) const;
    Polynomial Mult(const Polynomial& poly) const;
    float Eval(float x) const;

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& poly);
    friend std::istream& operator>>(std::istream& is, Polynomial& poly);
};

#endif