#ifndef DISTANCE_COMPARAISON_H_
#define DISTANCE_COMPARAISON_H_

#include <list>

#include "directories_configuration.h"
#include "distance_evaluation.h"
#include "evaluation_configuration.h"

class DistanceComparaison {

    public:
        DistanceComparaison(DirectoriesConfiguration* dir_conf,
                Instances* instances,
                EvaluationConfiguration* evaluation_conf,
                const std::list<Distance*>& distances,
                const std::list<std::list<SplitKind>*>& splits);
        ~DistanceComparaison();

        Instances* instances() const { return instances_; };
        std::list<DistanceEvaluation*>* evaluations() const {
            return evaluations_; };

        void CompareDistances();

    private:
        Instances* instances_;
        std::list<DistanceEvaluation*>* evaluations_;
        DirectoriesConfiguration* dir_conf_;

        void InitDistanceEvaluations(EvaluationConfiguration* evaluation_conf,
                const std::list<Distance*>& distances,
                const std::list<std::list<SplitKind>*>& splits);
};

#endif    // END_DISTANCE_COMPARAISON_H_
