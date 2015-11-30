#ifndef PRUNING_REPORT_H_
#define PRUNING_REPORT_H_

#include "pruning.h"
#include "report.h"

class PruningReport: public Report {

    public:
        PruningReport(Instances* instances,
                Pruning* pruning,
                DirectoriesConfiguration* dir_conf):
            Report(instances, dir_conf),
            pruning_(pruning),
            misclassified_instances_(false),
            all_pruned_trees_(false) {}
        ~PruningReport() {};

        void SetMisclassifiedUsers(bool b) { misclassified_instances_ = b; };
        void SetAllPrunedTrees(bool b) { all_pruned_trees_ = b; };

        virtual void GenerateReport() const;

    private:
        Pruning* pruning_;

        bool misclassified_instances_;
        bool all_pruned_trees_;
};

#endif    // END_PRUNING_REPORT_H_
