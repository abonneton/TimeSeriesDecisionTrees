#ifndef IMPURITY_H_
#define IMPURITY_H_

#include <string>

class Impurity {

  public:
    virtual ~Impurity() { };
    virtual double Compute(double p1) = 0;
    virtual std::string PrintImpurityMeasure() const = 0;
};

#endif    // END_IMPURITY_H_
