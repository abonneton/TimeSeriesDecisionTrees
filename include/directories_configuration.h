#ifndef DIRECTORIES_CONFIGURATION_H_
#define DIRECTORIES_CONFIGURATION_H_

#include <string>

class DirectoriesConfiguration {

    public:
        DirectoriesConfiguration(
                std::string output_path, std::string data_path,
                std::string input_file):
            output_path_(output_path), data_path_(data_path),
            input_file_(input_file) {}

        DirectoriesConfiguration(std::string data_path,
                std::string lists_path):
            data_path_(data_path), lists_path_(lists_path) {}

        ~DirectoriesConfiguration() {}

        std::string plot_path() const { return data_path_ + "plots/"; }
        std::string output_path() const { return output_path_; }
        std::string data_path() const { return data_path_; }
        std::string lists_path() const { return lists_path_; }

        std::string input_file() const { return input_file_; }
        std::string input_file_absolute_path() const { return
            data_path_ + input_file_; }


    private:
        std::string output_path_;
        std::string data_path_;
        std::string lists_path_;
        std::string input_file_;
};

#endif    // END_DIRECTORIES_CONFIGURATION_H_
