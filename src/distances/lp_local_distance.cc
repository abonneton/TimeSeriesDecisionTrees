#include "distances/lp_local_distance.h"

#include <cmath>

double LpLocalDistance::LocalCost(double value0, double value1) {
    return pow(std::abs(value0 - value1), p_);
}

double LpLocalDistance::FinalCost(double final_cost) {
    return pow(final_cost, 1/double(p_));
}

std::string LpLocalDistance::PrintLocalDistance() const {
    return "Lp-"+ std::to_string(p_);
}
