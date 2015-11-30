#ifndef INSTANCE_H_
#define INSTANCE_H_

#include <stdint.h>

#include <string>



class Instance {

    public:
        ~Instance();

        int32_t id() const { return id_; }
        double* time_series() const { return time_series_; }
        int32_t vector_size() const { return vector_size_; }
        std::string filename() const { return filename_; }
        bool label() const { return label_; }
        int8_t fold() const { return fold_; }

        void SetFold(int8_t fold) { fold_ = fold; }

        void Init(int32_t id, std::string filename, bool label,
                double* time_series, int32_t vector_size);

        void PrintCSV(FILE* f) const;

    private:

        int32_t id_;
        int32_t vector_size_;
        double* time_series_;
        bool label_;

        std::string filename_;

        // For the cross validation
        int8_t fold_;
};

#endif    // END_INSTANCE_H_
