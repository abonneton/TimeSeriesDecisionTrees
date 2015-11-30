#include "confusion_matrix.h"

ConfusionMatrix::ConfusionMatrix() {
    Init();
}

ConfusionMatrix::~ConfusionMatrix() {
    delete(global_estimators_);
    global_estimators_ = NULL;
}

void ConfusionMatrix::Init() {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            confusion_matrix_[i][j] = 0;
        }
    }
    misclassified_instances_.clear();
    global_estimators_ = NULL;
}

void ConfusionMatrix::UpdateEvaluation(Instance* instance,
        bool predicted_label) {
    UpdateConfusionMatrix(*instance, predicted_label);
    UpdateMisclassifiedUsersList(instance, predicted_label);
}

void ConfusionMatrix::ComputeEvaluationEstimators() {
    if (global_estimators_ != NULL) {
        delete(global_estimators_);
        global_estimators_ = NULL;
    }
    global_estimators_ = new EvaluationEstimators(GetTP(), GetTN(), GetFP(),
            GetFN());
    global_estimators_->ComputeEstimators();
}

void ConfusionMatrix::PrintEvaluationLatex(const char* tree_evaluation_file,
        bool misclassified_instances) const{
    FILE* f = fopen(tree_evaluation_file, "w");
    PrintConfusionMatrixLatex(f);
    global_estimators_->PrintLatex(f);
    // List of misclassified instances
    if (misclassified_instances) {
        fprintf(f, "\\newline\n");
        PrintMisclassifiedUsersLatex(f);
    }
    fclose(f);
}

void ConfusionMatrix::PrintMisclassifiedUsersCsv(const char* file_name) const {
    FILE* f = fopen(file_name, "w");
    // Header
    fprintf(f, "IP,ACTUAL_LABEL\n");
    // Lines
    for (auto instance: misclassified_instances_) {
        fprintf(f, "%i,%i\n", instance->id(), instance->label());
    }
    fclose(f);
}

void ConfusionMatrix::PrintMisclassifiedUsersLatex(FILE* f) const {
    fprintf(f, "\\paragraph{Misclassified instances}\n");
    if (misclassified_instances_.empty()) return;
    fprintf(f, "\\begin{multicols}{3}\n");
    fprintf(f, "\\begin{itemize}\n");
    for (auto instance: misclassified_instances_) {
        fprintf(f, "\t \\item %i (%i)\n",
                instance->id(), instance->label());
    }
    fprintf(f, "\\end{itemize}\n");
    fprintf(f, "\\end{multicols}\n");
}

void ConfusionMatrix::UpdateConfusionMatrix(const Instance& instance,
        bool predicted_label) {
    confusion_matrix_[instance.label()][predicted_label]++;
}

void ConfusionMatrix::UpdateMisclassifiedUsersList(Instance* instance,
        bool predicted_label) {
    if (predicted_label != instance->label()) {
        misclassified_instances_.push_back(instance);
    }
}

void ConfusionMatrix::PrintConfusionMatrixLatex(FILE* f) const {
    fprintf(f, "\\begin{tabular}{|l|l|l|l|}\n");
    fprintf(f, "\\hline\n");
    fprintf(f, "                & \\multicolumn{3}{c|}{Predicted}   \\\\ \n");
    fprintf(f, "\\hline\n");
    fprintf(f, "                &           & OK    & KO      \\\\ \n");
    fprintf(f, "\\cline{2-4} Actual & OK        & %i    & %i            \\\\ \n",
            GetTN(),
            GetFP());
    fprintf(f, "\\cline{2-4}    & KO  & %i    & %i            \\\\ \n",
            GetFN(),
            GetTP());
    fprintf(f, "\\hline\n");
    fprintf(f, "\\end{tabular}\n");
}

int32_t ConfusionMatrix::GetTP() const {
    int32_t TP = confusion_matrix_[1][1];
    return TP;
}

int32_t ConfusionMatrix::GetTN() const {
    int32_t TN = confusion_matrix_[0][0];
    return TN;
}

int32_t ConfusionMatrix::GetFP() const {
    int32_t FP = confusion_matrix_[0][1];
    return FP;
}

int32_t ConfusionMatrix::GetFN() const {
    int32_t FN = confusion_matrix_[1][0];
    return FN;
}
