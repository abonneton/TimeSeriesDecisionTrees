#ifndef DISTANCE_EVALUATION_CONFIGURATION_H_
#define DISTANCE_EVALUATION_CONFIGURATION_H_

#include <stdint.h>

#include <list>

#include "directories_configuration.h"
#include "distances_matrix.h"
#include "impurity/entropy_impurity.h"
#include "evaluation_configuration.h"
#include "tree_configuration.h"

class DistanceEvaluationConfiguration {

    public:
        DistanceEvaluationConfiguration(EvaluationConfiguration* eval_conf,
                Distance* distance, std::list<SplitKind>* split_kinds):
            eval_conf_(eval_conf),
            distance_(distance),
            split_kinds_(split_kinds),
            impurity_(new EntropyImpurity()) {};
        ~DistanceEvaluationConfiguration();

        EvaluationConfiguration* eval_conf() const { return eval_conf_;};
        Distance* distance() const { return distance_; };
        std::list<SplitKind>* split_kinds() const { return split_kinds_; };
        TreeConfiguration* GenerateTreeConfiguration() const;

        std::string GenerateReportFileName(
                DirectoriesConfiguration* conf) const;
        std::string GenerateSummaryFileName(
                DirectoriesConfiguration* conf) const;

    private:
        EvaluationConfiguration* eval_conf_;
        Distance* distance_;
        std::list<SplitKind>* split_kinds_;
        Impurity* impurity_;

        std::string GenerateOutputFilesBasename(
                DirectoriesConfiguration* conf) const;
};

#endif    // END_DISTANCE_EVALUATION_CONFIGURATION_H_
