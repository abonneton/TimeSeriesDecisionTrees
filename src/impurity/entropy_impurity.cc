#include "impurity/entropy_impurity.h"

#include <cmath>

double EntropyImpurity::Compute(double p1) {
    if (p1 == 0 || p1 == 1) {
        return 0;
    } else {
        return( -(p1 * log(p1) + (1 - p1) * log(1 - p1)) / log(2) );
    }
}

std::string EntropyImpurity::PrintImpurityMeasure() const {
    return "entropy";
}
