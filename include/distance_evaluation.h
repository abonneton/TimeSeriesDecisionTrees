#ifndef DISTANCE_EVALUATION_H_
#define DISTANCE_EVALUATION_H_

#include <array>

#include "confusion_matrix.h"
#include "distance_evaluation_configuration.h"
#include "distances_matrix.h"
#include "instances.h"
#include "split/split_kind.h"
#include "pruning.h"
#include "tree_configuration.h"
#include "tree_evaluation.h"

class DistanceEvaluation {

    public:
        DistanceEvaluation(Instances* instances,
                DistanceEvaluationConfiguration* eval_conf):
            instances_(instances),
            eval_conf_(eval_conf),
            distances_computation_time_(0),
            tree_configuration_(eval_conf_->GenerateTreeConfiguration()) {};
        ~DistanceEvaluation();

        DistancesMatrix* distances_matrix() const { return distances_matrix_; };
        DistanceEvaluationConfiguration* eval_conf() const {
            return eval_conf_; };
        double distances_computation_time() const {
            return distances_computation_time_; };

        double full_num_splits() const {
            return full_evaluation_->num_splits() ; };
        double best_num_splits() const {
            return best_evaluation_->num_splits() ; };
        double full_num_leaves() const {
            return full_evaluation_->num_leaves() ; };
        double best_num_leaves() const {
            return best_evaluation_->num_leaves() ; };
        double full_depth() const {
            return full_evaluation_->depth() ; };
        double best_depth() const {
            return best_evaluation_->depth() ; };
        double full_building_time() const {
            return full_evaluation_->building_time() ; };
        double best_building_time() const {
            return best_evaluation_->building_time() ; };

        ConfusionMatrix* full_confusion_matrix() const {
            return full_evaluation_->testing_evaluation() ; };
        ConfusionMatrix* best_confusion_matrix() const {
            return best_evaluation_->testing_evaluation() ; };

        TreeConfiguration* tree_conf() const { return tree_configuration_; };

        DecisionTree* best_decision_tree() const {
            return trees_->best_decision_tree(); };
        const std::list<DecisionTree*>& pruned_decision_trees() const {
            return trees_->pruned_decision_trees(); };
        DecisionTree* full_decision_tree() const {
            return trees_->full_decision_tree(); };

        //void Evaluate();

        void CrossValidation();

        void InitDistancesMatrix();
        void DeleteDistancesMatrix();

        void DeleteTrees();

    private:
        Instances* instances_;
        DistanceEvaluationConfiguration* eval_conf_;
        DistancesMatrix* distances_matrix_;

        // If Cross Validation, average on the g decision trees
        double distances_computation_time_;
        TreeEvaluation* full_evaluation_;
        TreeEvaluation* best_evaluation_;

        TreeConfiguration* tree_configuration_;
        Pruning* trees_;
};

#endif    // END_DISTANCE_EVALUATION_H_
