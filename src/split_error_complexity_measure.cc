#include "split_error_complexity_measure.h"

SplitErrorComplexityMeasure::SplitErrorComplexityMeasure(int32_t num_errors,
        int32_t num_leaves, int32_t num_errors_if_pruned):
            ErrorComplexityMeasure(num_errors, num_leaves),
            num_errors_if_pruned_(num_errors_if_pruned) {
    ErrorComplexityComputation();
}

void SplitErrorComplexityMeasure::ErrorComplexityComputation() {
    error_complexity_ =  (double)(num_errors_if_pruned_ - num_errors_) /
        (double)(num_leaves_ - 1);
}
