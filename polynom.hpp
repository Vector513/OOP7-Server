#include "polynom.h"
#include <iomanip>
#include <sstream>

template <class number>
Polynom<number>::Polynom()
    : degree(0)
    , An()
    , roots()
    , coefs()
{}

template <class number>
Polynom<number>::~Polynom() {}

template <class number>
void Polynom<number>::clear() {}

template <class number>
void Polynom<number>::fillRoots(const std::string &input)
{
    roots.fill(input);
    degree = roots.getSize();
    calculateCoefs();
}

template <class number>
Array<number> Polynom<number>::multiplyPolynomials(const Array<number> &roots1, const Array<number> &roots2)
{
    Array<number> result;
    size_t newSize = roots1.getSize() + roots2.getSize() - 1;
    result.resize(newSize);

    for (size_t i = 0; i < roots1.getSize(); ++i) {
        for (size_t j = 0; j < roots2.getSize(); ++j) {
            result[i + j] += roots1[i] * roots2[j];
        }
    }

    return result;
}

template <class number>
void Polynom<number>::calculateCoefs()
{
    Array<number> currentPoly;
    currentPoly.add(An);

    for (size_t i = 0; i < roots.getSize(); ++i) {
        Array<number> factor;
        factor.add(-roots[i]);
        factor.add(1.0);
        currentPoly = multiplyPolynomials(currentPoly, factor);
    } // result[0] = (3-4i)*(-2-2i) = -6 - 6i + 8i -8 = -14 + 2i
    // result[1] = (3-4i)*1 = 3-4i

    coefs = currentPoly;
}

template <class number>
void Polynom<number>::setRoot(const int index, const number& newRoot)
{
    if (index == roots.getSize()) {
        roots.add(newRoot);
        degree++;
        calculateCoefs();
    }
    else if (index >= 0 && index < roots.getSize()) {
        if (newRoot != roots[index]) {
            roots[index] = newRoot;
            calculateCoefs();
        }
    }
}

template <class number>
void Polynom<number>::addRoot(const number &newRoot)
{
    roots.add(newRoot);
    degree++;
    calculateCoefs();
}

template <class number>
void Polynom<number>::setAn(const number& newAn)
{
    if (An != newAn) {
        An = newAn;
        calculateCoefs();
    }
}

template <class number>
size_t Polynom<number>::getDegree() const
{
    return degree;
}

template <class number>
const number &Polynom<number>::getAn() const
{
    return An;
}

template <class number>
const Array<number> &Polynom<number>::getRoots() const
{
    return roots;
}

template <class number>
const Array<number> &Polynom<number>::getCoefs() const
{
    return coefs;
}

template <class number>
number Polynom<number>::evaluate(const number &x) const
{
    number result = An;

    for (size_t i = 0; i < roots.getSize(); ++i) {
        result *= (x - roots[i]);
    }

    return result;
}

template <class number>
void Polynom<number>::resize(const size_t newSize)
{
    roots.resize(newSize);
    coefs.resize(newSize);
    degree = roots.getSize();
    calculateCoefs();
}

template <class number>
void Polynom<number>::show(std::ostream &output, bool isFirstForm) const
{
    if constexpr (std::is_same<number, double>::value) {
        // Логика для double
        if (isFirstForm) {
            output << "p(x) = ";
            bool firstTerm = true;

            for (size_t i = coefs.getSize(); i-- > 0;) {
                double term = coefs[i];
                if (term != 0.0) {
                    if (!firstTerm) {
                        output << (term < 0.0 ? " - " : " + ");
                    }

                    output << (!firstTerm ? std::abs(term) : term);

                    if (i > 0) {
                        output << "x";
                        if (i > 1) {
                            output << "^" << i;
                        }
                    }
                    firstTerm = false;
                }
            }

            if (firstTerm) {
                output << "0";
            }
            output << '\n';
        }
        else {
            output << "p(x) = " << An;

            for (size_t i = 0; i < roots.getSize(); ++i) {
                double root = roots[i];
                if (root != 0) {
                    output << "(x" << (root > 0.0 ? " - " : " + ") << std::abs(root) << ")";
                }
                else {
                    output << "(x)";
                }
            }
            output << '\n';
        }
    }
    else {
        if (isFirstForm) {
            output << "p(x) = ";
            bool firstTerm = true;
            bool needMinus = false;
            for (size_t i = coefs.getSize(); i-- > 0;) {
                if (coefs[i].getRe() != 0.0 || coefs[i].getIm() != 0.0) {
                    double re = coefs[i].getRe();
                    double im = coefs[i].getIm();

                    if (!firstTerm) {
                        needMinus = (re < 0.0 || (re == 0.0 && im < 0.0));
                        output << (needMinus ? " - " : " + ");
                    }

                    if (im == 0.0) {
                        output << (!firstTerm ? std::abs(re) : re);
                    } else {
                        output << '(';
                        if (re != 0.0) {
                            output << (!firstTerm ? std::abs(re) : re) << ((im > 0.0) ? " + " : " - ") << std::abs(im);
                        }
                        else if (!firstTerm) {
                            output << ((im > 0.0) ^ (needMinus) ? " + " : " - ") << std::abs(im);
                        }
                        else {
                            output << im;
                        }
                        output << "i)";
                    }

                    if (i > 0) {
                        output << "x";
                        if (i > 1) {
                            output << "^" << i;
                        }
                    }
                    firstTerm = false;
                }
            }

            if (firstTerm) {
                output << "0";
            }
            output << '\n';
        } else {
            output << "p(x) = ";

            if (An.getRe() != 0.0 || An.getIm() != 0.0) {
                if (An.getIm() == 0.0) {
                    output << An.getRe();
                } else {
                    output << '(';
                    if (An.getRe() != 0.0) {
                        output << An.getRe() << (An.getIm() > 0.0 ? " + " : " - ") << std::abs(An.getIm());
                    }
                    else {
                        output << An.getIm();
                    }
                    output << "i)";
                }

                if (roots.getSize() != 0) {
                    for (size_t i = 0; i < roots.getSize(); ++i) {
                        output << "(x";
                        double re = roots[i].getRe();
                        double im = roots[i].getIm();

                        if (re != 0.0) {
                            output << (re > 0.0 ? " - " : " + ") << std::abs(re);
                        }

                        if (im != 0.0) {
                            output << (im > 0.0 ? " - " : " + ") << std::abs(im) << "i";
                        }

                        output << ")";
                    }
                }
            } else {
                output << "0";
            }
            output << '\n';
        }
    }
}
