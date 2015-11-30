#include "report/pruning_report.h"

#include "report/decision_tree_report.h"

void PruningReport::GenerateReport() const {
    // Creates the pruning directory
    std::string mkdir_cmd_pruning = "mkdir " +
        pruning_->tree_conf()->GeneratePruningDirectory(dir_conf_);
    system(mkdir_cmd_pruning.c_str());
    FILE* f = fopen(pruning_->tree_conf()->
            GeneratePruningReportFileName(dir_conf_).c_str(), "w");
    GenerateBeginning(f, "Pruning report");
    GenerateDataInformation(f, pruning_->tree_conf());
    std::list<DecisionTree*> to_print;
    if (all_pruned_trees_) {
        to_print = pruning_->pruned_decision_trees();
    } else {
        to_print.push_back(pruning_->full_decision_tree());
        to_print.push_back(pruning_->best_decision_tree());
    }
    for (auto decision_tree: to_print) {
        DecisionTreeReport report(instances_, decision_tree, dir_conf_, 0);
        report.SetMisclassifiedUsers(misclassified_instances_);
        report.SetWithTree();
        if (decision_tree->id() == 0) {
            report.GenerateTreeInformation(f);
        }
        fprintf(f, "\\newpage \n");
        fprintf(f, "\\begin{center} \\begin{huge} \n");
        fprintf(f, "Tree %i \n", decision_tree->id());
        fprintf(f, "\\end{huge} \\end{center} \n");
        report.GenerateTreeReportSection(f);
        report.GenerateJavascriptFiles();
    }
    GenerateEnd(f);
    fclose(f);
}
