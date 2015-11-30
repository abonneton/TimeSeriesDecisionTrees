#ifndef TREE_CONFIGURATION_H_
#define TREE_CONFIGURATION_H_

#include <stdint.h>

#include <list>
#include <string>

#include "directories_configuration.h"
#include "distances_matrix.h"
#include "impurity/impurity.h"
#include "split/split_kind.h"

class TreeConfiguration {

    public:
        TreeConfiguration(std::list<SplitKind>* split_kinds, Impurity* impurity):
            split_kinds_(split_kinds),
            impurity_(impurity) {};
        ~TreeConfiguration() { };

        const DistancesMatrix& distances_matrix() const {
            return *distances_matrix_; };
        std::list<SplitKind>* split_kinds() const { return split_kinds_; };
        Impurity* impurity() const { return impurity_; };

        void SetDistancesMatrix(DistancesMatrix* distances_matrix) {
            distances_matrix_ = distances_matrix; };

        void PrintInfoLatex(DirectoriesConfiguration* conf,
                int32_t tree_id) const;
        void PrintInfoJson(DirectoriesConfiguration* conf,
                int32_t tree_id) const;

        std::string GeneratePruningDirectory(
                DirectoriesConfiguration* conf) const;

        std::string GenerateTreeDirectory(DirectoriesConfiguration* conf,
                int32_t tree_id) const;

        std::string GenerateDataInfoFileName(
                DirectoriesConfiguration* conf) const;

        std::string GenerateTreeInfoFileName(
                DirectoriesConfiguration* conf, int32_t tree_id) const;
        std::string GenerateTreeStatsFileName(
                DirectoriesConfiguration* conf, int32_t tree_id) const;
        std::string GenerateEvalFileName(bool train,
                DirectoriesConfiguration* conf, int32_t tree_id) const;
        std::string GenerateMisclassifiedUsersFileName(
                DirectoriesConfiguration* conf, int32_t tree_id) const;
        std::string GenerateTreeFileName(
                DirectoriesConfiguration* conf, int32_t tree_id) const;
        std::string GenerateLeavesFileName(
                DirectoriesConfiguration* conf, int32_t tree_id) const;

        std::string GenerateReportFileName(bool with_tree,
                DirectoriesConfiguration* conf, int32_t tree_id) const;
        std::string GeneratePruningReportFileName(
                DirectoriesConfiguration* conf) const;

    private:
        DistancesMatrix* distances_matrix_;
        std::list<SplitKind>* split_kinds_;
        Impurity* impurity_;

        std::string GenerateOutputSuffixFileName() const;
        std::string GenerateOutputFilesBasename(
                DirectoriesConfiguration* conf) const;
};

#endif    // END_TREE_CONFIGURATION_H_
