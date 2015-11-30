#ifndef DISTANCE_H_
#define DISTANCE_H_

#include <string>
#include "instance.h"

class Distance {

    public:
        virtual ~Distance() { };

        virtual double Compute(const Instance& instance0, const Instance& instance1,
                int thread_id) = 0;
        virtual std::string PrintDistance() const = 0;

        virtual void InitTemps() = 0;
        virtual void FreeTemps() = 0;

};

#endif    // END_DISTANCE_H_
