#ifndef DECISION_TREE_REPORT_H_
#define DECISION_TREE_REPORT_H_

#include "decision_tree.h"
#include "directories_configuration.h"
#include "report.h"
#include "instances.h"

class DecisionTreeReport: public Report {

    public:
        DecisionTreeReport(Instances* instances,
                DecisionTree* decision_tree,
                DirectoriesConfiguration* dir_conf,
                double distances_computation_time):
                    Report(instances, dir_conf),
                    decision_tree_(decision_tree),
                    distances_computation_time_(distances_computation_time),
                    with_tree_(false), with_leaves_(false),
                    misclassified_instances_(false) {
            std::string mkdir_cmd = "mkdir " +
                decision_tree_->tree_conf()->GenerateTreeDirectory(dir_conf_,
                        decision_tree_->id());
            system(mkdir_cmd.c_str());
        };
        ~DecisionTreeReport() {};

        void SetWithTree() { with_tree_ = true; };
        void SetWithLeaves() { with_leaves_ = true; };
        void SetMisclassifiedUsers(bool b) { misclassified_instances_ = b; };

        virtual void GenerateReport() const;
        void GenerateTreeInformation(FILE* f) const;
        void GenerateTreeReportSection(FILE* f) const;
        void GenerateJavascriptFiles() const;

    private:
        DecisionTree* decision_tree_;
        double distances_computation_time_;
        bool with_tree_;
        bool with_leaves_;
        bool misclassified_instances_;

        void GenerateTreeStatistics(FILE* f) const;
        void GenerateExecutionTimeInformation(FILE* f) const;
        void GenerateTrainingEvaluation(FILE* f) const;
        void GenerateTestingEvaluation(FILE* f) const;
        void GenerateTree(FILE* f) const;
        void GenerateLeaves(FILE* f) const;
};

#endif    // END_DECISION_TREE_REPORT_H_
