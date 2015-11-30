#include <utility> // std::pair

#include "decision_tree.h"
#include "report/decision_tree_report.h"
#include "distance_evaluation.h"
#include "impurity/entropy_impurity.h"
#include "tree_configuration.h"

DistanceEvaluation::~DistanceEvaluation() {
    delete(eval_conf_);             eval_conf_ = NULL;
    delete(distances_matrix_);      distances_matrix_ = NULL;
    delete(trees_);                 trees_ = NULL;
    delete(full_evaluation_);       full_evaluation_ = NULL;
    delete(best_evaluation_);       best_evaluation_ = NULL;
    delete(tree_configuration_);    tree_configuration_ = NULL;
}

void GenerateTrainTestData(const std::vector<Instances*>& folds, int32_t i,
        int32_t g, std::pair<Instances*, Instances*>& train_test_data);

void DistanceEvaluation::CrossValidation() {
    // g the number of folds for the CV
    int8_t g = eval_conf_->eval_conf()->g();
    // Creates the g folds of instances
    std::vector<Instances*> folds;
    instances_->CrossValidationFolds(g, folds);
    std::pair<Instances*, Instances*> train_test_data;
    // Evaluation of the full decision tree
    full_evaluation_ =  new TreeEvaluation();
    // Evaluation of the best pruned tree
    best_evaluation_ =  new TreeEvaluation();
    tree_configuration_->SetDistancesMatrix(distances_matrix_);
    for (int8_t i = 0; i < g; ++i) {
        GenerateTrainTestData(folds, i, g, train_test_data);
        // Build the decision tree with the training_set
        trees_ = new Pruning(tree_configuration_, *train_test_data.first);
        trees_->GenerateFullDecisionTree();
        trees_->GeneratePrunedDecisionTrees();
        trees_->SelectBestDecisionTree();
        // Test the full decision tree and the best pruned tree
        // with the testing_set
        trees_->FullDecisionTreeClassify(*train_test_data.second,
                *full_evaluation_->testing_evaluation());
        trees_->BestDecisionTreeClassify(*train_test_data.second,
                *best_evaluation_->testing_evaluation());
        // Update tree evaluations
        full_evaluation_->AddEvaluation(
                trees_->full_decision_tree()->tree_eval());
        best_evaluation_->AddEvaluation(
                trees_->best_decision_tree()->tree_eval());
        // Free the memory allocated for pruning
        delete(trees_); trees_ = NULL;
        // Free the memory allocated for train_test data
        delete(train_test_data.first);      train_test_data.first = NULL;
        delete(train_test_data.second);     train_test_data.second = NULL;
    }
    full_evaluation_->testing_evaluation()->ComputeEvaluationEstimators();
    best_evaluation_->testing_evaluation()->ComputeEvaluationEstimators();
    full_evaluation_->ComputeMeanEvaluation(g);
    best_evaluation_->ComputeMeanEvaluation(g);
    // Free the memory allocated for the folds
    for (int8_t i = 0; i < g; ++i) {
        delete(folds[i]);   folds[i] = NULL;
    }
}

void DistanceEvaluation::DeleteDistancesMatrix() {
    delete(distances_matrix_); distances_matrix_ = NULL;
}

void DistanceEvaluation::DeleteTrees() {
    delete(trees_); trees_ = NULL;
}

void DistanceEvaluation::InitDistancesMatrix() {
    distances_matrix_ = new DistancesMatrix(eval_conf_->distance(),
            eval_conf_->eval_conf()->distance_multi_threads());
    distances_matrix_->Compute(*instances_, distances_computation_time_);
}

void GenerateTrainTestData(const std::vector<Instances*>& folds, int32_t i,
        int32_t g,
        std::pair<Instances*, Instances*>& train_test_data) {
    train_test_data.first = new Instances(*folds[0]);
    train_test_data.second = new Instances(*folds[0]);
    for (int8_t j = 0; j < g; j++) {
        if (j == i) {   // testing_set
            train_test_data.second->Add(*folds[j]);
        } else {        // training_set
            train_test_data.first->Add(*folds[j]);
        }
    }
}
