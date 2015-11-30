#ifndef DISTANCE_EVALUATION_REPORT_H_
#define DISTANCE_EVALUATION_REPORT_H_

#include <stdio.h>

#include "confusion_matrix.h"
#include "directories_configuration.h"
#include "distance_evaluation.h"
#include "report.h"

class DistanceEvaluationReport: public Report {

    public:
        DistanceEvaluationReport(Instances* instances,
                DistanceEvaluation* evaluation,
                DirectoriesConfiguration* dir_conf):
            Report(instances, dir_conf), evaluation_(evaluation) {};
        ~DistanceEvaluationReport() { };

        virtual void GenerateReport() const;
        void GenerateSummaryReport() const;
        void GenerateSummary(FILE* f) const;
        void GenerateCsvLine(FILE* f, bool best) const;

    private:
        DistanceEvaluation* evaluation_;
/**
        void GenerateEvaluationSummary(FILE* f,
                ConfusionMatrix* confusion_matrix,
                bool best) const;
*/
};

#endif    // END_DISTANCE_EVALUATION_REPORT_H_
