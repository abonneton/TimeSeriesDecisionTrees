#include "distances_matrix.h"

#include <stdint.h>
#include <sys/time.h>

#include <cassert>
#include <thread>

#include <iostream>

#include "utils.h"

// Prerequisites
// num_instances % 2 == 0
// num_instances / 2 % num_threads == 0
void DistancesMatrix::Compute(const Instances& instances,
        double& computation_time) {

    num_instances_ = instances.num_instances();

    assert(num_instances_ % 2 == 0);
    assert(num_instances_ / 2 % num_threads_ == 0);

    // Create the distances distances
    CreateDistancesMatrix();
    // Init the distance temporary variables
    distance_->InitTemps();
    // Compute the distances matrix
    std::vector<Instance*> instances_vector;

    instances.AsVector(instances_vector);
    ComputeMultiThreads(instances_vector, computation_time);

    // Free the distance temporary variables
    distance_->FreeTemps();

}

double DistancesMatrix::GetDistance(const Instance& instance0,
        const Instance& instance1) const {
    int32_t id0 = instance0.id();
    int32_t id1 = instance1.id();
    return (id0 < id1 ? distances_matrix_[id1][id0] :
            distances_matrix_[id0][id1]);
}

void DistancesMatrix::Print() const {
    for (int32_t i = 0; i < num_instances_; ++i) {
        for (int32_t j = 0; j <= i; ++j) {
            printf("%f\t", distances_matrix_[i][j]);
        }
        printf("\n");
    }
}

void DistancesMatrix::CreateDistancesMatrix() {
    distances_matrix_ = std::vector<std::vector<double>>(num_instances_);
    for (int32_t i = 0; i < num_instances_; ++i) {
        distances_matrix_[i] = std::vector<double>(i + 1);
        distances_matrix_[i][i] = 0.0;
    }
}

void DistancesMatrix::ComputeMultiThreads(const std::vector<Instance*>& instances,
        double& computation_time) {
    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    // Launches all the threads
    std::vector<std::thread*> threads(num_threads_, NULL);
    for (int8_t i = 0; i < num_threads_; ++i) {
        threads[i] = new std::thread(&DistancesMatrix::ComputeMultiThreadsPart,
                this, i, instances);
    }
    // Waits for all the threads to finish
    for (int8_t i = 0; i < num_threads_; ++i) {
        threads[i]->join();
    }
    for (int8_t i = 0; i < num_threads_; ++i) {
        delete(threads[i]); threads[i] = NULL;
    }
    // Computes the execution time
    clock_gettime(CLOCK_REALTIME, &end);
    computation_time = NumSeconds(Diff(start, end));
}

void DistancesMatrix::ComputeMultiThreadsPart(int8_t part,
        const std::vector<Instance*>& instances) {
    int32_t i_min, i_max;
    ComputeLimits(part, i_min, i_max);
    for (int32_t i = i_min; i < i_max; ++i) {
            for (int32_t j = 0; j < i; ++j) {
                distances_matrix_[i][j] = distance_->Compute(*instances[i],
                    *instances[j], part);
            }
    }
    for (int32_t i = num_instances_ - i_max; i < num_instances_ - i_min; ++i) {
            for (int32_t j = 0; j < i; ++j) {
                distances_matrix_[i][j] = distance_->Compute(*instances[i],
                    *instances[j], part);
            }
    }
}

void DistancesMatrix::ComputeLimits(int8_t part, int32_t& i_min,
        int32_t& i_max) {
    i_min = part * num_instances_ / (2 * num_threads_);
    i_max = i_min + num_instances_ / (2 * num_threads_);
}
