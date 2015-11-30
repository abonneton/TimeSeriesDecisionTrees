#include "report/decision_tree_report.h"
#include "report/distance_evaluation_report.h"
#include "utils.h"

void DistanceEvaluationReport::GenerateReport() const {
    FILE* f = fopen(
            evaluation_->eval_conf()->GenerateReportFileName(dir_conf_).c_str(),
            "w");
    GenerateBeginning(f,
            evaluation_->eval_conf()->distance()->PrintDistance());
    GenerateDataInformation(f, evaluation_->tree_conf());
    fprintf(f, "\\newpage\n");
    DecisionTreeReport report(instances_, evaluation_->best_decision_tree(),
            dir_conf_,
            evaluation_->distances_computation_time());
    report.GenerateTreeReportSection(f);
    GenerateEnd(f);
    fclose(f);
}

void DistanceEvaluationReport::GenerateSummaryReport() const {
    FILE* f = fopen(
            evaluation_->eval_conf()->GenerateReportFileName(dir_conf_).c_str(),
            "w");
    GenerateBeginning(f,
            evaluation_->eval_conf()->distance()->PrintDistance());
    GenerateDataInformation(f, evaluation_->tree_conf());
    GenerateSummary(f);
    GenerateEnd(f);
    fclose(f);
}

void DistanceEvaluationReport::GenerateSummary(FILE* f) const {
    double distances_matrix_computation = evaluation_->
        distances_computation_time();
    fprintf(f, "Distances matrix computation time: %s\n",
            TimePrettyPrint((int32_t)distances_matrix_computation).c_str());
    fprintf(f, "\\paragraph{}\n");
    fprintf(f, "\\begin{tabular}{|l|l|l|l|l|l|l|}\n");
    fprintf(f, "\\hline\n");
    fprintf(f, "Pruning & F-score & Precision & Recall & ");
    fprintf(f, "Detection & FalseAlarms & Accuracy \\\\ \n");
    fprintf(f, "\\hline\n");
    //GenerateEvaluationSummary(f, evaluation_->full_confusion_matrix(), false);
    //GenerateEvaluationSummary(f, evaluation_->best_confusion_matrix(), true);
    fprintf(f, "\\end{tabular}\n");
}

void DistanceEvaluationReport::GenerateCsvLine(FILE* f, bool best) const {
    ConfusionMatrix* matrix = best ?
        evaluation_->best_confusion_matrix() :
        evaluation_->full_confusion_matrix();
    int32_t vector_size = instances_->vector_size();
    std::string distance =
        evaluation_->eval_conf()->distance()->PrintDistance();
    std::string split = ToString(*evaluation_->eval_conf()->split_kinds());
    std::string pruning = best ? "Best" : "Full";
    fprintf(f, "%i,", vector_size);
    fprintf(f, "%s,", distance.c_str());
    fprintf(f, "%s,", split.c_str());
    fprintf(f, "%s,", pruning.c_str());
    matrix->global_estimators()->PrintCSV(f);
    fprintf(f, ",%f,%f,%f,%f,%f\n",
            evaluation_->distances_computation_time(),
            best ? evaluation_->best_building_time() :
                evaluation_->full_building_time(),
            best ? evaluation_->best_num_splits() :
                evaluation_->full_num_splits(),
            best ? evaluation_->best_num_leaves() :
                evaluation_->full_num_leaves(),
            best ? evaluation_->best_depth() :
                evaluation_->full_depth());
}
