#include "distances/dtw_value_distance.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>
#include <vector>

#include "utils.h"

DtwValueDistance::DtwValueDistance(int32_t vector_size,
        int32_t num_threads) {
    vector_size_ = vector_size;
    num_threads_ = num_threads;
    local_distance_ = NULL;
    cost_matrix_ = NULL;
}

DtwValueDistance::~DtwValueDistance() {
    FreeTemps();
}

void DtwValueDistance::InitTemps() {
    cost_matrix_ = (double***) calloc(num_threads_, sizeof(double**));
    for (int thread_id = 0; thread_id < num_threads_; ++thread_id) {
        cost_matrix_[thread_id] =
            (double**) calloc(vector_size_ + 1, sizeof(double*));
        for (int32_t i = 0; i <= vector_size_; ++i) {
            cost_matrix_[thread_id][i] =
                (double*) calloc(vector_size_ + 1, sizeof(double));
            for (int32_t j = 0; j <= vector_size_; ++j) {
                cost_matrix_[thread_id][i][j] =
                    std::numeric_limits<double>::infinity();
            }
        }
    }
}

void DtwValueDistance::FreeTemps() {
    if (cost_matrix_ == NULL) return;
    for (int thread_id = 0; thread_id < num_threads_; ++thread_id) {
        for (int32_t i = 0; i <= vector_size_; ++i) {
            free(cost_matrix_[thread_id][i]); cost_matrix_[thread_id][i] = NULL;
        }
        free(cost_matrix_[thread_id]); cost_matrix_[thread_id] = NULL;
    }
    free(cost_matrix_); cost_matrix_ = NULL;
}

double DtwValueDistance::Compute(const Instance& instance0, const Instance& instance1,
        int thread_id) {
    double* vector0 = instance0.time_series();
    double* vector1 = instance1.time_series();
    int32_t size0 = instance0.vector_size();
    int32_t size1 = instance1.vector_size();
    cost_matrix_[thread_id][0][0] = 0.0;
    double local_cost;
    int32_t j_min, j_max;
    for (int32_t i = 1; i <= size0; ++i) {
        j_min = std::max(1, i - window_);
        j_max = std::min(size1, i + window_);
        for (int32_t j = j_min; j <= j_max; ++j) {
            local_cost = local_distance_->LocalCost(vector0[i-1], vector1[j-1]);
            cost_matrix_[thread_id][i][j] = min(
              cost_matrix_[thread_id][i-1][j-1] + matching_weight_ * local_cost,
              cost_matrix_[thread_id][i-1][j] + delinsert_weight_ * local_cost,
              cost_matrix_[thread_id][i][j-1] + delinsert_weight_ * local_cost);
        }
    }
    return local_distance_->FinalCost(cost_matrix_[thread_id][size0][size1]);
}

std::string DtwValueDistance::PrintDistance() const {
    return "DTW-value-localDistance-" + local_distance_->PrintLocalDistance() +
        "-w=" + TimePrettyPrintMinutes(window_);
}
