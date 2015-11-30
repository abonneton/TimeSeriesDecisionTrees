#include "distance_comparaison.h"
#include "report/distance_evaluation_report.h"

DistanceComparaison::DistanceComparaison(
        DirectoriesConfiguration* dir_conf,
        Instances* instances,
        EvaluationConfiguration* evaluation_conf,
        const std::list<Distance*>& distances,
        const std::list<std::list<SplitKind>*>& splits) {
    dir_conf_ = dir_conf;
    instances_ = instances;
    InitDistanceEvaluations(evaluation_conf, distances, splits);
}

DistanceComparaison::~DistanceComparaison() {
    for (auto eval: *evaluations_) {
        delete eval; eval = NULL;
    }
    delete(evaluations_); evaluations_ = NULL;
}

void DistanceComparaison::CompareDistances() {
    for (auto evaluation: *evaluations_) {
        // Compute the distances matrix
        fprintf(stderr, "\n\n -------- %s --------\n",
            evaluation->eval_conf()->distance()->PrintDistance().c_str());
        fprintf(stderr, "Input file: %s\n", dir_conf_->input_file().c_str());
        fprintf(stderr, "Split(s): %s\n",
                ToString(*evaluation->eval_conf()->split_kinds()).c_str());
        fprintf(stderr, "Distances matrix computation\n");
        evaluation->InitDistancesMatrix();
        // Cross validation
        fprintf(stderr, "CrossValidation\n");
        evaluation->CrossValidation();
        // Free the allocated memory
        evaluation->DeleteDistancesMatrix();
        evaluation->DeleteTrees();
    }
}

void DistanceComparaison::InitDistanceEvaluations(
        EvaluationConfiguration* evaluation_conf,
        const std::list<Distance*>& distances,
        const std::list<std::list<SplitKind>*>& splits) {
    evaluations_ = new std::list<DistanceEvaluation*>();
    for (auto distance: distances) {
        for (auto split_candidates: splits) {
            DistanceEvaluationConfiguration* conf =
                new DistanceEvaluationConfiguration(evaluation_conf, distance,
                    split_candidates);
            DistanceEvaluation* evaluation =
                new DistanceEvaluation(instances_, conf);
            evaluations_->push_back(evaluation);
        }
    }
}
