#include <vector>

#include "decision_tree.h"
#include "pruning.h"

DecisionTree::DecisionTree(std::shared_ptr<Tree> tree, double building_time,
        const std::list<Instance*>& training_data, int32_t tree_id) {
    tree_conf_ = tree->conf();
    tree_ = tree;
    tree_eval_ = new TreeEvaluation(building_time, tree_->Depth(),
            tree_->NumSplits(), tree_->NumLeaves());
    tree_->Classify(training_data, *tree_eval_->training_evaluation());
    tree_id_ = tree_id;
}

DecisionTree::~DecisionTree() {
    delete(tree_eval_); tree_eval_ = NULL;
}

void DecisionTree::BuildTree(const std::list<Instance*>& instances_list) {
    tree_ = std::shared_ptr<Tree>(new Tree(0, tree_conf_));
    tree_eval_ = new TreeEvaluation();
    double computation_time = tree_->Build(instances_list,
            *tree_eval_->training_evaluation());
    tree_eval_->SetBuildingTime(computation_time);
    tree_eval_->SetDepth(tree_->Depth());
    tree_eval_->SetNumSplits(tree_->NumSplits());
    tree_eval_->SetNumLeaves(tree_->NumLeaves());
}

bool DecisionTree::Classify(const Instance& instance) const {
    return tree_->Classify(instance);
}

void DecisionTree::Classify(const std::list<Instance*>& testing_set) const {
    tree_->Classify(testing_set, *tree_eval_->testing_evaluation());
}

void DecisionTree::Classify(const std::list<Instance*>& testing_set,
        ConfusionMatrix& confusion_matrix) const {
    tree_->Classify(testing_set, confusion_matrix);
}
