#ifndef EVALUATION_ESTIMATORS_H_
#define EVALUATION_ESTIMATORS_H_

#include <stdint.h>
#include <stdio.h>

class EvaluationEstimators {

    public:
        EvaluationEstimators(int32_t TP, int32_t TN, int32_t FP, int32_t FN):
            TP_(TP), TN_(TN), FP_(FP), FN_(FN) {};
        ~EvaluationEstimators() {};

        void ComputeEstimators();

        double f1_score() const { return f1_score_; };
        double precision() const { return precision_; };
        double recall() const { return recall_; };
        double accuracy() const { return accuracy_; };
        double detection_rate() const { return detection_rate_; };
        double false_alarm_rate() const { return false_alarm_rate_; };

        void PrintLatex(FILE* f) const;
        void PrintCSV(FILE* f) const;

    private:
        int32_t TP_;
        int32_t TN_;
        int32_t FP_;
        int32_t FN_;

        double accuracy_;
        double precision_;
        double recall_;
        double f1_score_;

        double detection_rate_;
        double false_alarm_rate_;
};

#endif    // END_EVALUATION_ESTIMATORS_H_
