#include "report/decision_tree_report.h"

#include <stdio.h>
#include <stdlib.h>

#include <string>

#include "report/print_tree_json.h"
#include "report/print_tree_latex.h"
#include "utils.h"

void DecisionTreeReport::GenerateReport() const {
    FILE* f = fopen(decision_tree_->tree_conf()->GenerateReportFileName(
                with_tree_, dir_conf_, decision_tree_->id()).c_str(),
            "w");
    GenerateBeginning(f, "Decision Tree report");
    GenerateDataInformation(f, decision_tree_->tree_conf());
    GenerateTreeInformation(f);
    GenerateTreeStatistics(f);
    GenerateExecutionTimeInformation(f);
    GenerateTrainingEvaluation(f);
    GenerateTestingEvaluation(f);
    if (with_tree_) GenerateTree(f);
    GenerateEnd(f);
    fclose(f);
}

void DecisionTreeReport::GenerateTreeReportSection(FILE* f) const {
    GenerateTreeStatistics(f);
    GenerateTrainingEvaluation(f);
    GenerateTestingEvaluation(f);
    if (with_tree_) GenerateTree(f);
}

void DecisionTreeReport::GenerateJavascriptFiles() const {
    TreeConfiguration* tree_conf = decision_tree_->tree_conf();
    // JSON info file
    tree_conf->PrintInfoJson(dir_conf_, decision_tree_->id());
    // JSON tree
    PrintTreeJson print_tree(dir_conf_, decision_tree_->id());
    print_tree.DisplayTree(*decision_tree_->tree());
    // CSV misclassified instances
    decision_tree_->tree_eval()->testing_evaluation()->
        PrintMisclassifiedUsersCsv(
                (tree_conf->GenerateMisclassifiedUsersFileName(dir_conf_,
                                decision_tree_->id())
                 + ".csv").c_str());
}

void DecisionTreeReport::GenerateTreeInformation(FILE* f) const {
    fprintf(f, "\\section*{Tree information}\n");
    std::string tree_info_file = decision_tree_->tree_conf()->
        GenerateTreeInfoFileName(dir_conf_, decision_tree_->id());
    fprintf(f, "\\input{%s}\n", tree_info_file.c_str());
    decision_tree_->tree_conf()->PrintInfoLatex(dir_conf_, decision_tree_->id());
}

void DecisionTreeReport::GenerateTreeStatistics(FILE* f) const {
    fprintf(f, "\\section*{Tree statistics}\n");
    std::string tree_stats_file = decision_tree_->tree_conf()->
        GenerateTreeStatsFileName(dir_conf_, decision_tree_->id());
    fprintf(f, "\\input{%s}\n", tree_stats_file.c_str());
    decision_tree_->tree_eval()->PrintLatex(dir_conf_,
            *decision_tree_->tree_conf(), decision_tree_->id());
}

void DecisionTreeReport::GenerateExecutionTimeInformation(FILE* f) const {
    fprintf(f, "\\section*{Execution Time information}\n");
    fprintf(f, "\\begin{tabular}{|l|l|}\n");
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Building tree}       & %s \\\\ \n",
        TimePrettyPrint((int32_t)decision_tree_->tree_eval()->
            building_time()).c_str());
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Distances matrix}       & %s \\\\ \n",
        TimePrettyPrint((int32_t)distances_computation_time_).c_str());
    fprintf(f, "\\hline\n");
    fprintf(f, "\\end{tabular}\n");
}

void DecisionTreeReport::GenerateTrainingEvaluation(FILE* f) const {
    fprintf(f, "\\section*{Training evaluation}\n");
    std::string train_eval_file = decision_tree_->tree_conf()->
        GenerateEvalFileName(true, dir_conf_, decision_tree_->id());
    fprintf(f, "\\input{%s}\n", train_eval_file.c_str());
    decision_tree_->tree_eval()->training_evaluation()->
        ComputeEvaluationEstimators();
    decision_tree_->tree_eval()->training_evaluation()->PrintEvaluationLatex(
            (train_eval_file + ".tex").c_str(), false);
}

void DecisionTreeReport::GenerateTestingEvaluation(FILE* f) const {
    fprintf(f, "\\section*{Testing evaluation}\n");
    std::string test_eval_file = decision_tree_->tree_conf()->
        GenerateEvalFileName(false, dir_conf_, decision_tree_->id());
    fprintf(f, "\\input{%s}\n", test_eval_file.c_str());
    decision_tree_->tree_eval()->testing_evaluation()->
        ComputeEvaluationEstimators();
    decision_tree_->tree_eval()->testing_evaluation()->PrintEvaluationLatex(
            (test_eval_file + ".tex").c_str(),
            misclassified_instances_);
}

void DecisionTreeReport::GenerateTree(FILE* f) const {
    std::string tree_file = decision_tree_->tree_conf()->
        GenerateTreeFileName(dir_conf_, decision_tree_->id());
    fprintf(f, "\\newpage\n");
    fprintf(f, "%%\\input{%s}\n", tree_file.c_str());
    PrintTreeLatex print_tree(dir_conf_, decision_tree_->id());
    print_tree.DisplayTree(*decision_tree_->tree());
}
