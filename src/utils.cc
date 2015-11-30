#include <algorithm> // std::min
#include <cmath>     // pow

#include "utils.h"

timespec Diff(timespec start, timespec end) {
    timespec temp;
    if ((end.tv_nsec - start.tv_nsec) < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp;
}

double NumSeconds(timespec temp){
    return temp.tv_sec + pow(10.0, -9.0) * temp.tv_nsec;
}

std::string TimePrettyPrint(int32_t num_seconds) {
    int32_t hours = num_seconds / 3600;
    int32_t minutes = num_seconds / 60 - hours*60;
    int32_t seconds = num_seconds % 60;
    return (std::to_string(hours) + "h" +
            std::to_string(minutes) + "min" +
            std::to_string(seconds) + "s");
}

std::string TimePrettyPrintMinutes(int32_t num_seconds) {
    int32_t minutes = num_seconds / 60;
    return (std::to_string(minutes) + "min");
}

void EstimatorPrettyPrint(double estimator, FILE* f) {
    fprintf(f, "%.2f\\%%", estimator * 100);
}

double min(double v1, double v2, double v3) {
    if (v1 <= v2 && v1 <= v3) {
        return v1;
    } else if (v2 <= v1 && v2 <= v3) {
        return v2;
    } else {
        return v3;
    }
}
