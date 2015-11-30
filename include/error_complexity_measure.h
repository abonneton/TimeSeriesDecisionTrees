#ifndef ERROR_COMPLEXITY_MEASURE_H_
#define ERROR_COMPLEXITY_MEASURE_H_

#include <stdint.h>

class ErrorComplexityMeasure {

    public:
        ErrorComplexityMeasure(int32_t num_errors, int32_t num_leaves):
            num_errors_(num_errors), num_leaves_(num_leaves) {};
        virtual ~ErrorComplexityMeasure() {};

        int32_t num_errors() const { return num_errors_; };
        int32_t num_leaves() const { return num_leaves_; };

    protected:
        // num errors if not pruned
        double num_errors_;
        // number of leaves in the subtree having this split as root node
        int32_t num_leaves_;
};

#endif    // END_ERROR_COMPLEXITY_MEASURE_H
