#ifndef LP_LOCAL_DISTANCE_H_
#define LP_LOCAL_DISTANCE_H_

#include <stdint.h>
#include <string>

#include "local_distance.h"

class LpLocalDistance: public LocalDistance {

    public:
        LpLocalDistance(int32_t p): LocalDistance(), p_(p) {};
        virtual ~LpLocalDistance() {};
        virtual double LocalCost(double value0, double value1);
        virtual double FinalCost(double final_cost);
        virtual std::string PrintLocalDistance() const;

    private:
        int32_t p_;
};

#endif    // END_LP_LOCAL_DISTANCE_H_
