#ifndef POLYNOM_H
#define POLYNOM_H

#include "array.h"

template <class number>
class Polynom
{
private:
    size_t degree;
    number An;
    Array<number> roots;
    Array<number> coefs;

public:
    Polynom();
    ~Polynom();

    void clear();

    void fillRoots(const std::string &input);
    Array<number> multiplyPolynomials(const Array<number> &poly1, const Array<number> &poly2);
    void calculateCoefs();
    void setRoot(const int index, const number &newRoot);
    void addRoot(const number &newRoot);
    void setAn(const number &newAn);
    size_t getDegree() const;
    const number &getAn() const;
    const Array<number> &getRoots() const;
    const Array<number> &getCoefs() const;
    number evaluate(const number &x) const;

    void resize(const size_t newSize);

    void show(std::ostream &output, bool isFirstForm = true) const;

};

#include "polynom.hpp"

#endif // POLYNOM_H
