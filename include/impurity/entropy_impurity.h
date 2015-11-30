#ifndef ENTROPY_IMPURITY_H_
#define ENTROPY_IMPURITY_H_

#include <string>

#include "impurity/impurity.h"

class EntropyImpurity : public Impurity {

    public:
        virtual ~EntropyImpurity() { };

        virtual double Compute(double p1);
        virtual std::string PrintImpurityMeasure() const;
};

#endif  // END_ENTROPY_IMPURITY_H_
