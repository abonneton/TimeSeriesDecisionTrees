#include "evaluation_estimators.h"

void EvaluationEstimators::ComputeEstimators() {
    // If there is no instance (TP_ + TN_ + FP_ + FN_ == 0)
    // Accuracy = Fscore = Precision = Recall = 1
    // False alarm rate = 0
    if (TP_ + TN_ + FP_ + FN_ == 0) {
        accuracy_ = 1;
        precision_ = 1;
        recall_ = 1;
        f1_score_ = 1;
        detection_rate_ = 1;
        false_alarm_rate_ = 0;
    } else {
        accuracy_ = (double)(TP_ + TN_) / (double)(TP_ + TN_ + FP_ + FN_);
        precision_ = (TP_ + FP_ == 0) ? 0 : (double)TP_ / (double)(TP_ + FP_);
        recall_ = (TP_ + FN_ == 0) ? 0 : (double)TP_ / (double)(TP_ + FN_);
        f1_score_ = (precision_ + recall_ == 0) ?
            0 : 2 * precision_ * recall_ / (precision_ + recall_);
        detection_rate_ = recall_;
        false_alarm_rate_ = (FP_ + TN_ == 0) ?
            0 : (double)FP_ / (double)(FP_ + TN_);
    }
}

void EvaluationEstimators::PrintLatex(FILE* f) const {
    fprintf(f, "\\begin{tabular}{|l|l|}\n");
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Accuracy}        & %.2f\\%% \\\\ \n",
            accuracy_*100);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Precision}       & %.2f\\%% \\\\ \n",
            precision_*100);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Recall}          & %.2f\\%% \\\\ \n",
            recall_*100);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{F1-score}        & %.2f\\%% \\\\ \n",
            f1_score_*100);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Detection rate}  & %.2f\\%% \\\\ \n",
            detection_rate_*100);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{False alarm rate} & %.2f\\%% \\\\ \n",
            false_alarm_rate_*100);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\end{tabular}\n");
}

void EvaluationEstimators::PrintCSV(FILE* f) const {
    fprintf(f, "%f,%f,%f,%f,%f,%f", f1_score_, precision_, recall_,
            false_alarm_rate_, detection_rate_, accuracy_);
}
