#ifndef SPLIT_ERROR_COMPLEXITY_MEASURE_H_
#define SPLIT_ERROR_COMPLEXITY_MEASURE_H_

#include <stdint.h>

#include "error_complexity_measure.h"

class SplitErrorComplexityMeasure: public ErrorComplexityMeasure {

    public:
        SplitErrorComplexityMeasure(int32_t num_errors, int32_t num_leaves,
                int32_t num_errors_if_pruned);
        virtual ~SplitErrorComplexityMeasure() {};
        double error_complexity() const { return error_complexity_; };

    private:
        // num errors if the split node is tranformed into a leaf
        double num_errors_if_pruned_;
        double error_complexity_;
        void ErrorComplexityComputation();
};

#endif    // END_SPLIT_ERROR_COMPLEXITY_MEASURE_H
