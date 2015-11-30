#ifndef DISTANCES_MATRIX_H_
#define DISTANCES_MATRIX_H_

#include <stdint.h>

#include <vector>

#include "distance.h"
#include "instance.h"
#include "instances.h"

// Distances matrix is symmetric, only half of the matrix is computed
class DistancesMatrix {

    public:
        DistancesMatrix(Distance* distance, int32_t num_threads):
            distance_(distance), num_threads_(num_threads) { };
        ~DistancesMatrix() { };

        const Distance& distance() const { return *distance_; };

        void Compute(const Instances& instances, double& computation_time);
        double GetDistance(const Instance& instance0, const Instance& instance1) const;
        void Print() const;

    private:
        Distance* distance_;
        int32_t num_threads_;

        std::vector<std::vector<double>> distances_matrix_;
        int32_t num_instances_;

        void CreateDistancesMatrix();

        void ComputeMultiThreads(const std::vector<Instance*>& instances,
                double& computation_time);
        void ComputeMultiThreadsPart(int8_t part,
                const std::vector<Instance*>& instances);
        void ComputeLimits(int8_t part, int32_t& i_min, int32_t& i_max);
};

#endif  // END_DISTANCES_MATRIX_H_
