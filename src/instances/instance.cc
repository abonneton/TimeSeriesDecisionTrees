#include "instance.h"

#include <cmath>
#include <string>

using namespace std;

Instance::~Instance() {
    free(time_series_);
}

void Instance::Init(int32_t id, string filename, bool label,
        double* time_series, int32_t vector_size) {
    id_ = id;
    filename_ = filename;
    label_ = label;
    time_series_ = time_series;
    vector_size_ = vector_size;
}

void Instance::PrintCSV(FILE* f) const {
    fprintf(f, "%s,%i",
            filename_.c_str(), label_);
    for (int32_t i = 0; i < vector_size_; ++i) {
        fprintf(f, ",%.0f", time_series_[i]);
    }
    fprintf(f, "\n");
}
