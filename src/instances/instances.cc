#include "instances.h"

#include <cassert>
#include <cstring>
#include <ctime>

#include <fstream>
#include <iostream>
#include <string>

#include "utils.h"

using namespace std;

Instances::Instances(const DirectoriesConfiguration& conf) {

    own_instances_ = true;
    // Read the number of instances, the vector size
    // the sampling interval, and the normalisation from the input file
    input_file_ = conf.input_file();
    input_file_absolute_path_ = conf.input_file_absolute_path();
    
    string line;
    ifstream f(input_file_absolute_path_);
    
    num_instances_ = 0;
    num_ko_ = 0;
    num_ok_ = 0;

    if (f.is_open()) {
        
        string delimiter = ",";
        size_t start = 0;
        size_t next = 0;
    
        // Read the header
        getline(f, line);
        vector_size_ = stoi(line);

        // Read the instances
        string instance_filename;
        bool instance_label;
        while (getline(f, line)) {
            Instance* instance = new Instance();
            // Read the instance id and its label
            start = 0;
            next = line.find_first_of(delimiter, start);
            instance_filename = line.substr(start, next - start);
            start = next + 1;
            next = line.find_first_of(delimiter, start);
            instance_label = line.substr(start, next - start).compare("0") == 0;
            double* time_series = (double*)calloc(vector_size_, 
                    sizeof(double));
            // Read the time series
            int32_t i = 0;
            do { 
                start = next + 1;
                next = line.find_first_of(delimiter, start);
                time_series[i] = stod(line.substr(start, next - start));
                i++;
            } while (next != string::npos);
            instance->Init(num_instances_, instance_filename, instance_label, time_series, vector_size_);
            Add(instance);
        }
    }
}

Instances::~Instances() {
    if (own_instances_) {
        for (auto instance: instances_) {
            delete(instance); instance = NULL;
        }
    }
}

void Instances::AsVector(std::vector<Instance*>& vector) const {
    vector = std::vector<Instance*>(instances_.begin(), instances_.end());
}

void Instances::Add(Instance* instance) {
    instances_.push_back(instance);
    if (instance->label()) {
        num_ko_++;
    } else {
        num_ok_++;
    }
    num_instances_++;
}

void Instances::Add(const Instances& instances) {
    for (auto instance: instances.instances_) {
        Add(instance);
    }
}

void Instances::CrossValidationFolds(int8_t k,
        std::vector<Instances*>& folds) const {
    fprintf(stderr, "ko %i, ok %i, g=%i\n", num_ko_, num_ok_, k);
    
    // Init the k folds
    folds = std::vector<Instances*>(k, NULL);
    for (int32_t i = 0; i < k; ++i) {
        folds[i] = new Instances(*this);
    }

    // Number of ko and ok instances in each fold
    int32_t num_ko = num_ko_ / k;
    int32_t num_ok = num_ok_ / k;
    // Current folds
    int32_t curr_ko_fold = 0;
    int32_t curr_ok_fold = 0;

    // Fill the folds with the training data
    for (auto instance: instances_) {
        if (instance->label()) {
            if (folds[curr_ko_fold]->num_ko_ >=
                    num_ko) {
                curr_ko_fold = (curr_ko_fold + 1) % k;
            }
            folds[curr_ko_fold]->Add(instance);
            instance->SetFold(curr_ko_fold);
        } else {
            if (folds[curr_ok_fold]->num_ok_ >= num_ok) {
                curr_ok_fold = (curr_ok_fold + 1) % k;
            }
            folds[curr_ok_fold]->Add(instance);
            instance->SetFold(curr_ok_fold);
        }
    }
    return;
}

// Pruning 10%, Building 90%
void Instances::GenerateBuildingPruningData(Instances& building_data,
        Instances& pruning_data) const {

    int num_ko_pruning = num_ko_ / 10;
    int num_ok_pruning = num_ok_ / 10;
    int cur_num_ko_pruning = 0;
    int cur_num_ok_pruning = 0;
    for (auto instance: instances_) {
        if (instance->label()) {
            if (cur_num_ko_pruning < num_ko_pruning) {
                pruning_data.Add(instance);
                cur_num_ko_pruning++;
            } else {
                building_data.Add(instance);
            }
        } else {
            if (cur_num_ok_pruning < num_ok_pruning) {
                pruning_data.Add(instance);
                cur_num_ok_pruning++;
            } else {
                building_data.Add(instance);
            }
        }
    }
    return;
}

/**
void Instances::GenerateRInputData(int8_t k,
        const DirectoriesConfiguration& conf) const {
    std::vector<Instances*> folds;
    CrossValidationFolds(k, folds);
    FILE* f = fopen(conf.R_input_file().c_str(), "w");
    // Prints the CSV lines
    for (auto instance: instances_) {
        instance->PrintCSV(f);
    }
    fclose(f);
}
*/

void Instances::PrintInfo(const char* info_file) const {
    FILE* f = fopen(info_file, "w");
    fprintf(f, "\\begin{tabular}{|l|l|}\n");
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Input file}              & %s \\\\ \n",
            input_file_.c_str());
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Number instances}        & %i \\\\ \n",
            num_instances());
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Number ok}  & %i \\\\ \n",
            num_ok_);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Number ko}  & %i \\\\ \n",
            num_ko_);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Vector size}   & %i \\\\ \n",
            vector_size_);
    fprintf(f, "\\hline\n");
    /**
    fprintf(f, "\\emph{Loading data}   & %s \\\\ \n",
            TimePrettyPrint((int32_t)loading_time_).c_str());
            **/
    fprintf(f, "\\hline\n");
    fprintf(f, "\\end{tabular}\n");
    fclose(f);
}
