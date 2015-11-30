#ifndef LEAF_ERROR_COMPLEXITY_MEASURE_H_
#define LEAF_ERROR_COMPLEXITY_MEASURE_H_

#include <stdint.h>

#include "error_complexity_measure.h"

class LeafErrorComplexityMeasure: public ErrorComplexityMeasure {

    public:
        LeafErrorComplexityMeasure(int32_t num_errors):
            ErrorComplexityMeasure(num_errors, 1) {};
        virtual ~LeafErrorComplexityMeasure() {};
};

#endif    // END_LEAF_ERROR_COMPLEXITY_MEASURE_H
