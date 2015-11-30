#ifndef EVALUATION_CONFIGURATION_H_
#define EVALUATION_CONFIGURATION_H_

#include <stdint.h>

#include <string>

#include "directories_configuration.h"

class EvaluationConfiguration {

    public:
        EvaluationConfiguration(int8_t g, int32_t distance_multi_threads):
            g_(g),
            distance_multi_threads_(distance_multi_threads) {};
        EvaluationConfiguration(EvaluationConfiguration* eval_conf):
            g_(eval_conf->g_),
            distance_multi_threads_(eval_conf->distance_multi_threads_) {};
        ~EvaluationConfiguration() { };

        int8_t g() const { return g_; };
        int32_t distance_multi_threads() const {
            return distance_multi_threads_; };

        std::string GenerateOutputFilesSuffix() const;
        void PrintInfo(const char* info_file) const;

        std::string GenerateEvaluationInfoFileName(
                DirectoriesConfiguration* conf) const;
        std::string GenerateReportFileName(
                DirectoriesConfiguration* conf) const;
        std::string GenerateCsvFileName(
                DirectoriesConfiguration* conf) const;

    protected:
        int8_t g_;
        int32_t distance_multi_threads_;

        std::string GenerateOutputFilesBasename(
                DirectoriesConfiguration* conf) const;
};

#endif    // END_EVALUATION_CONFIGURATION_H_
