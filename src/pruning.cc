#include "pruning.h"

#include <iostream>
#include <sys/time.h>
#include "utils.h"

using namespace std;

Pruning::Pruning(TreeConfiguration* tree_conf, const Instances& training_data):
        tree_conf_(tree_conf),
        building_data_(training_data),
        pruning_data_(training_data) {
    // Generate tree data and pruning data
    // 10% for the pruning data
    // 90% for the tree data
    training_data.GenerateBuildingPruningData(building_data_, pruning_data_);
    full_decision_tree_ = NULL;
    best_decision_tree_ = NULL;
    pruned_decision_trees_.clear();
}

Pruning::~Pruning() {
    for (auto decision_tree: pruned_decision_trees_) {
        delete(decision_tree); decision_tree = NULL;
    }
    pruned_decision_trees_.clear();
}

void Pruning::GenerateFullDecisionTree() {
    full_decision_tree_ = new DecisionTree(tree_conf_, 0);
    full_decision_tree_->BuildTree(building_data_.instances());
}

void Pruning::GeneratePrunedDecisionTrees() {
    DecisionTree* pruned_decision_tree = full_decision_tree_;
    int i = 1;
    do {
        pruned_decision_trees_.push_back(pruned_decision_tree);
        pruned_decision_tree =
            NextPrunedDecisionTree(pruned_decision_trees_.back());
        i++;
    } while (pruned_decision_tree != NULL);
}

void Pruning::SelectBestDecisionTree() {
    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    double best_evaluation = -1;
    best_decision_tree_ = NULL;
    double decision_tree_evaluation;
    for (auto decision_tree: pruned_decision_trees_) {
        // Evaluate the pruned decision tree
        decision_tree->Classify(pruning_data_.instances());
        ConfusionMatrix* testing_evaluation = decision_tree->tree_eval()->
            testing_evaluation();
        testing_evaluation->ComputeEvaluationEstimators();
        // Compare the pruned decision trees according to ACCURACY
        decision_tree_evaluation = testing_evaluation->global_estimators()->
            accuracy();
        // We prefer the smallest tree
        if (decision_tree_evaluation >= best_evaluation) {
            best_evaluation = decision_tree_evaluation;
            best_decision_tree_ = decision_tree;
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    double select_best_tree_time = NumSeconds(Diff(start, end));
    double previous_building_time =
        pruned_decision_trees_.back()->tree_eval()->building_time();
    best_decision_tree_->tree_eval()->SetBuildingTime(
            previous_building_time + select_best_tree_time);
}

void Pruning::BestDecisionTreeClassify(const Instances& testing_data,
        ConfusionMatrix& testing_evaluation) {
    best_decision_tree_->Classify(testing_data.instances(),
            testing_evaluation);
}

void Pruning::FullDecisionTreeClassify(const Instances& testing_data,
        ConfusionMatrix& testing_evaluation) {
    full_decision_tree_->Classify(testing_data.instances(),
            testing_evaluation);
}

DecisionTree* Pruning::NextPrunedDecisionTree(DecisionTree* decision_tree) {
    // Generate the pruned tree
    std::shared_ptr<Tree> tree = decision_tree->tree();
    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    std::shared_ptr<Tree> pruned_tree = NextPrunedTree(tree);
    clock_gettime(CLOCK_REALTIME, &end);
    // Return if there is no pruned decision tree to generate anymore
    if (pruned_tree == NULL) return NULL;
    // Generate the pruned decision tree from the pruned tree
    double pruning_execution_time = NumSeconds(Diff(start, end));
    double building_time = pruning_execution_time +
        decision_tree->tree_eval()->building_time();
    DecisionTree* pruned_decision_tree = new DecisionTree(pruned_tree,
            building_time, building_data_.instances(), decision_tree->id() + 1);
    return pruned_decision_tree;
}

std::shared_ptr<Tree> Pruning::NextPrunedTree(std::shared_ptr<Tree> tree) {
    if (!tree->InternalNode()) {
        return NULL;
    }
    // Compute the error complexity measure for each split
    tree->ComputeSplitsErrorComplexity();
    // Look for the splits having the smallest error complexity measure
    std::shared_ptr<Split> worst_split;
    // computeSplitsErrorComplexity
    tree->FindWorstSplit(&worst_split);
    // Return a tree pruning the worst split
    std::shared_ptr<Tree> pruned_tree =
        std::shared_ptr<Tree>(new Tree(tree, worst_split));
    return pruned_tree;
}
