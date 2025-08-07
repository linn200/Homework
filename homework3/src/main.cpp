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