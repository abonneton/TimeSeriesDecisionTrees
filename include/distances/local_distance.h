#ifndef LOCAL_DISTANCE_H_
#define LOCAL_DISTANCE_H_

#include <string>

class LocalDistance {

    public:
        virtual ~LocalDistance() {};
        virtual double LocalCost(double value0, double value1) = 0;
        virtual double FinalCost(double final_cost) = 0;
        virtual std::string PrintLocalDistance() const = 0;
};

#endif    // END_LOCAL_DISTANCE_H_
