#ifndef INSTANCES_H_
#define INSTANCES_H_

#include <list>
#include <string>
#include <vector>

#include "directories_configuration.h"
#include "instance.h"

class Instances {

    public:
        explicit Instances(const DirectoriesConfiguration& conf);
        explicit Instances(const Instances& instances):
            input_file_absolute_path_(instances.input_file_absolute_path_),
            input_file_(instances.input_file_),
            vector_size_(instances.vector_size_),
            sampling_interval_(instances.sampling_interval_),
            num_instances_(0),
            num_ko_(0),
            num_ok_(0),
            own_instances_(false) {};
        ~Instances();

        const std::list<Instance*>& instances() const { return instances_; };

        int32_t num_instances() const { return num_instances_; };
        int32_t vector_size() const { return vector_size_; };
        int32_t sampling_interval() const { return sampling_interval_; };

        void AsVector(std::vector<Instance*>& vector) const;
        void Add(const Instances& instances);

        void CrossValidationFolds(int8_t k,
                std::vector<Instances*>& folds) const;
        void GenerateBuildingPruningData(Instances& building_data,
                Instances& pruning_data) const;

        void PrintInfo(const char* info_file) const;

    private:
        std::list<Instance*> instances_;

        std::string input_file_absolute_path_;
        std::string input_file_;

        int32_t vector_size_;
        int32_t sampling_interval_;

        int32_t num_instances_;
        int32_t num_ko_;
        int32_t num_ok_;
        double loading_time_;

        bool own_instances_;

        void Init(int32_t num_instances);
        void Add(Instance* instance);
};

#endif    // END_INSTANCES_
