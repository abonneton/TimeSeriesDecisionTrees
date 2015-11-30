#ifndef TOOLS_H_
#define TOOLS_H_

#include <sys/time.h>

#include <string>

timespec Diff(timespec start, timespec end);

double NumSeconds(timespec temp);

std::string TimePrettyPrint(int32_t num_seconds);

std::string TimePrettyPrintMinutes(int32_t num_seconds);

void EstimatorPrettyPrint(double estimator, FILE* f);

double min(double v1, double v2, double v3);

#endif    // END_TOOLS_H_
