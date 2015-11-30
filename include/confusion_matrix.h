#ifndef CONFUSION_MATRIX_H_
#define CONFUSION_MATRIX_H_

#include <list>

#include "instance.h"
#include "evaluation_estimators.h"

class ConfusionMatrix {

    public:
        ConfusionMatrix();
        ~ConfusionMatrix();

        EvaluationEstimators* global_estimators() const {
            return global_estimators_; };

        void Init();
        void UpdateEvaluation(Instance* instance, bool predicted_label);
        void ComputeEvaluationEstimators();

        void PrintEvaluationLatex(const char* tree_evaluation_file,
                bool misclassified_instances) const;
        void PrintMisclassifiedUsersCsv(const char* file_name) const;

    private:

        // First dimension:     actual label
        // Second dimension:    predicted label
        int32_t confusion_matrix_[2][2];

        EvaluationEstimators* global_estimators_;

        std::list<Instance*> misclassified_instances_;

        int32_t GetTP() const;
        int32_t GetTN() const;
        int32_t GetFP() const;
        int32_t GetFN() const;

        void UpdateConfusionMatrix(const Instance& instance, bool predicted_label);
        void UpdateMisclassifiedUsersList(Instance* instance, bool predicted_label);

        void PrintConfusionMatrixLatex(FILE* f) const;

        void PrintMisclassifiedUsersLatex(FILE* f) const;
};

#endif    // END_CONFUSION_MATRIX_H_
