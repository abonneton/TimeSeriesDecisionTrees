#ifndef VALUE_BASED_DISTANCE_H_
#define VALUE_BASED_DISTANCE_H_

#include <string>

#include "distance.h"
#include "instance.h"

class ValueBasedDistance: public Distance {

    public:
        virtual ~ValueBasedDistance() {};

        virtual double Compute(const Instance& instance0, const Instance& instance1,
                int thread_id) = 0;

        virtual std::string PrintDistance() const = 0;
};

#endif    // END_VALUE_BASED_DISTANCE_H_
