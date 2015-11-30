#include "report/distance_comparaison_report.h"

#include "report/decision_tree_report.h"
#include "report/distance_evaluation_report.h"

void DistanceComparaisonReport::GenerateReport() const {
    FILE* f = fopen(
            evaluation_conf_->GenerateReportFileName(dir_conf_).c_str(),
            "w");
    GenerateBeginning(f, "Comparaison Report");
    GenerateEvaluationInformation(f);
    GenerateDistanceSections(f);
    GenerateEnd(f);
    fclose(f);
}

void DistanceComparaisonReport::GenerateCsv() const {
    FILE* f = fopen(
            evaluation_conf_->GenerateCsvFileName(dir_conf_).c_str(),
            "w");
    fprintf(f, "VectorSize,");
    fprintf(f, "Distance,Split,Pruning,");
    fprintf(f, "Fscore,Precision,Recall,");
    fprintf(f, "AlarmRate,DetectionRate,Accuracy,");
    fprintf(f, "DistancesMatrixComputation,BuildingComputation,");
    fprintf(f, "NumSplits,NumLeaves,Depth\n");
    std::list<DistanceEvaluation*>* evals = evaluations_->evaluations();
    for (auto eval: *evals) {
        DistanceEvaluationReport distance_report(instances_, eval, dir_conf_);
        distance_report.GenerateCsvLine(f, false);
        distance_report.GenerateCsvLine(f, true);
    }
    fclose(f);
}

void DistanceComparaisonReport::GenerateEvaluationInformation(FILE* f) const {
    fprintf(f, "\\section*{Evaluation information}\n");
    std::string eval_info_file =
        evaluation_conf_->GenerateEvaluationInfoFileName(dir_conf_);
    fprintf(f, "\\input{%s}\n", eval_info_file.c_str());
    evaluation_conf_->PrintInfo((eval_info_file + ".tex").c_str());
}

void DistanceComparaisonReport::GenerateDistanceSections(FILE* f) const {
    std::list<DistanceEvaluation*>* evals = evaluations_->evaluations();
    for (auto eval: *evals) {
        fprintf(f, "\\section*{%s}\n", eval->eval_conf()->distance()->
                PrintDistance().c_str());
        DistanceEvaluationReport distance_report(instances_, eval, dir_conf_);
        distance_report.GenerateSummary(f);
    }
}
