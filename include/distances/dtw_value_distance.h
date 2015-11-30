#ifndef DTW_VALUE_DISTANCE_H_
#define DTW_VALUE_DISTANCE_H_


#include "local_distance.h"
#include "value_based_distance.h"

class DtwValueDistance: public ValueBasedDistance {

    public:
        DtwValueDistance(int32_t vector_size,
                int32_t num_threads);
        virtual ~DtwValueDistance();

        void SetWindow(int32_t window) { window_ = window; };
        void SetLocalDistance(LocalDistance* local_distance) {
            local_distance_ = local_distance; };

        virtual void InitTemps();
        virtual void FreeTemps();

        virtual double Compute(const Instance& instance0, const Instance& instance1,
                int thread_id);
        virtual std::string PrintDistance() const;

    private:
        int32_t window_;
        LocalDistance* local_distance_;

        double*** cost_matrix_;
        int32_t num_threads_;
        int32_t vector_size_;

        // Same weight for insertion and deletion (symmetric distance)
        double delinsert_weight_ = 1.;
        double matching_weight_ = 1.;
};

#endif  // END_DTW_DISTANCE_H_
