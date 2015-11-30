#ifndef DISTANCE_COMPARAISON_REPORT_H_
#define DISTANCE_COMPARAISON_REPORT_H_

#include "directories_configuration.h"
#include "distance_comparaison.h"
#include "evaluation_configuration.h"
#include "report.h"

class DistanceComparaisonReport: public Report {

    public:
        DistanceComparaisonReport(DistanceComparaison* evaluations,
                EvaluationConfiguration* evaluation_conf,
                DirectoriesConfiguration* directories_conf):
            Report(evaluations->instances(), directories_conf),
            evaluations_(evaluations), evaluation_conf_(evaluation_conf) { };
        ~DistanceComparaisonReport() { };

        virtual void GenerateReport() const;
        void GenerateCsv() const;

    private:
        DistanceComparaison* evaluations_;
        EvaluationConfiguration* evaluation_conf_;

        void GenerateEvaluationInformation(FILE* f) const;
        void GenerateDistanceSections(FILE* f) const;
};

#endif    // DISTANCE_COMPARAISON_REPORT_H_
