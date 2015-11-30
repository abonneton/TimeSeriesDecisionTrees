#include "tree.h"

#include <sys/time.h>

#include <algorithm>
#include <cassert>
#include <limits>
#include <thread>

#include "leaf_error_complexity_measure.h"
#include "split_error_complexity_measure.h"
#include "split/improved_cluster_split.h"
#include "split/plain_cluster_split.h"
#include "split/standard_split.h"
#include "utils.h"

Tree::Tree(int32_t id, TreeConfiguration* conf) {
    id_ = id;
    conf_ = conf;
    leaf_ = NULL;
    split_ = NULL;
    left_child_ = NULL;
    right_child_ = NULL;
    error_complexity_ = NULL;
}

Tree::Tree(std::shared_ptr<Tree> tree, std::shared_ptr<Split> worst_split) {
    id_ = tree->id_;
    conf_ = tree->conf_;
    majority_label_ = tree->majority_label_;
    num_ok_ = tree->num_ok_;
    num_ko_ = tree->num_ko_;
    num_errors_ = tree->num_errors_;
    if (tree->InternalNode()) {
        if (tree->split() == worst_split) {
            bool leaf_label = tree->majority_label_;
            std::list<Instance*> instances;
            tree->AllUsers(instances);
            leaf_ = std::shared_ptr<Leaf>(new Leaf(leaf_label, instances));
            left_child_ = NULL;
            right_child_ = NULL;
        } else {
            split_ = tree->split();
            left_child_ = std::shared_ptr<Tree>(
                    new Tree(tree->left_child_, worst_split));
            right_child_ = std::shared_ptr<Tree>(
                    new Tree(tree->right_child_, worst_split));
        }
    } else {
        leaf_ = tree->leaf();
        left_child_ = NULL;
        right_child_ = NULL;
    }
    error_complexity_ = NULL;
}

Tree::~Tree() {
    delete(error_complexity_);      error_complexity_ = NULL;
}

double Tree::Build(const std::list<Instance*>& instances,
        ConfusionMatrix& training_confusion_matrix) {
    timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);
    BuildRec(instances, training_confusion_matrix);
    clock_gettime(CLOCK_REALTIME, &end);
    return NumSeconds(Diff(start, end));
}

bool Tree::Classify(const Instance& instance) const {
    if (InternalNode()) {
        if (split_->LeftChild(&instance, conf_->distances_matrix())) {
            return left_child_->Classify(instance);
        } else {
            return right_child_->Classify(instance);
        }
    } else {
        return leaf_->label();
    }
}

void Tree::Classify(const std::list<Instance*>& testing_set,
        ConfusionMatrix& confusion_matrix) const {
    bool predicted_label;
    for (auto test_case: testing_set) {
        predicted_label = Classify(*test_case);
        confusion_matrix.UpdateEvaluation(test_case, predicted_label);
    }
}

bool Tree::InternalNode() const {
    return (left_child_ != NULL && right_child_ != NULL);
}

void Tree::ComputeSplitsErrorComplexity() {
    if (!InternalNode()) {
        error_complexity_ = new LeafErrorComplexityMeasure(num_errors_);
    } else {
        left_child_->ComputeSplitsErrorComplexity();
        right_child_->ComputeSplitsErrorComplexity();
        int32_t num_leaves = left_child_->error_complexity_->num_leaves() +
            right_child_->error_complexity_->num_leaves();
        int32_t num_errors = left_child_->error_complexity_->num_errors() +
            right_child_->error_complexity_->num_errors();
        error_complexity_ = new SplitErrorComplexityMeasure(num_errors,
                num_leaves, num_errors_);
    }
}

void Tree::FindWorstSplit(std::shared_ptr<Split>* worst_split) {
    double worst_error_complexity = std::numeric_limits<double>::infinity();
    FindWorstSplitRec(worst_split, worst_error_complexity);
}

void Tree::FindWorstSplitRec(std::shared_ptr<Split>* worst_split,
        double& worst_error_complexity) {
    if (InternalNode()) {
        SplitErrorComplexityMeasure* split_error_complexity =
            dynamic_cast<SplitErrorComplexityMeasure*>(error_complexity_);
        double error_complexity = split_error_complexity->error_complexity();
        if (error_complexity < worst_error_complexity) {
            worst_error_complexity = error_complexity;
            *worst_split = split();
        }
        // Recursive calls
        left_child_->FindWorstSplitRec(worst_split, worst_error_complexity);
        right_child_->FindWorstSplitRec(worst_split, worst_error_complexity);
    }
    // Free the allocated memory
    delete(error_complexity_); error_complexity_ = NULL;
}

void Tree::BuildRec(const std::list<Instance*>& instances,
        ConfusionMatrix& training_confusion_matrix) {

    //// Base case ////
    if (StopCase(instances)) {
        // creates a leaf
        leaf_ = std::shared_ptr<Leaf>(new Leaf(majority_label_, instances));
        for (auto instance: instances) {
            training_confusion_matrix.UpdateEvaluation(instance,
                    majority_label_);
        }
        return;
    }

    //// Recursive calls ////
    // Creates the split candidate for each split kind in the configuration
    std::list<std::shared_ptr<Split>> split_candidates;
    CreateSplitCandidates(split_candidates, instances);

    // Selects the best split
    SelectBestSplit(split_candidates);

    // Splits the data according to the best split
    std::list<Instance*> left_child_data;
    std::list<Instance*> right_child_data;
    SplitData(instances, left_child_data, right_child_data);

    // Recursive calls for the two children
    BuildChildren(left_child_data, right_child_data, training_confusion_matrix);
}

// Stop cases
// 2- there is only one instance in IPs_list, OR
// 3- all the instances belong to the same class
// 4- all the instances are "equals" according to the distance
bool Tree::StopCase(const std::list<Instance*>& instances) {

    // Compute the number of instances for each label, the majority label
    num_ko_ = 0;
    num_ok_ = 0;
    for(auto instance: instances) {
        if (instance->label()) {
            num_ko_++;
        } else {
            num_ok_++;
        }
    }
    majority_label_ = (num_ko_ > num_ok_);
    num_errors_ =
        majority_label_ ? num_ok_ : num_ko_;

    // STOP CASE: there is only one instance
    if (num_ok_ + num_ko_ == 1) {
        return true;
    }

    // STOP CASE: All the labels are OK or OK
    if (num_ko_ == 0 || num_ok_ == 0) {
        return true;
    }

    // STOP CASE: all the instances are "equals"
    // according to the distance metric
    bool all_equals = true;
    Instance* instance_0 = instances.front();
    for(auto instance: instances) {
        if (conf_->distances_matrix().GetDistance(*instance_0, *instance) != 0) {
            all_equals =  false;
            break;
        }
    }
    return all_equals;
}

void Tree::CreateSplitCandidates(std::list<std::shared_ptr<Split>>& split_candidates,
        const std::list<Instance*>& instances) {
    // Creates a split for each split kind
    // and computes the best split
    std::shared_ptr<Split> split = NULL;
    for (auto split_kind: *conf_->split_kinds()) {
        switch (split_kind) {
            case SplitKind::kPlainCluster:
                split = std::shared_ptr<Split>(new PlainClusterSplit(conf_->impurity()));
                break;
            case SplitKind::kImprovedCluster:
                split = std::shared_ptr<Split>(new ImprovedClusterSplit(conf_->impurity()));
                break;
            case SplitKind::kStandard:
                split = std::shared_ptr<Split>(new StandardSplit(conf_->impurity()));
                break;
            default:
                fprintf(stderr, "ERR: Split must be Cluster or Standard\n");
                split = NULL;
                assert(false);
                break;
        }
        split->FindBestSplit(instances, conf_->distances_matrix());
        split_candidates.push_back(split);
    }
}

void Tree::SelectBestSplit(const std::list<std::shared_ptr<Split>>& split_candidates) {
    // Looks for the best split
    std::shared_ptr<Split> best_split = NULL;
    for (auto candidate: split_candidates) {
        if (candidate->IsBetterThan(best_split)) {
            best_split = candidate;
        }
    }
    // Assign the best split
    split_ = best_split;
}

void Tree::SplitData(const std::list<Instance*>& instances,
        std::list<Instance*>& left_child_data,
        std::list<Instance*>& right_child_data) {
    // Creates the children node
    left_child_ = std::shared_ptr<Tree>(new Tree(id_*2 +1, conf_));
    right_child_ = std::shared_ptr<Tree>(new Tree(id_*2 + 2, conf_));
    // Split the data
    split_->SplitInstances(instances, conf_->distances_matrix(),
            left_child_data, right_child_data);
}


void Tree::BuildChildren(const std::list<Instance*>& left_child_data,
        const std::list<Instance*>& right_child_data,
        ConfusionMatrix& training_confusion_matrix) {
    left_child_->BuildRec(left_child_data, training_confusion_matrix);
    right_child_->BuildRec(right_child_data, training_confusion_matrix);
}

void Tree::AllUsers(std::list<Instance*>& instances) {
    if (InternalNode())  {
        left_child_->AllUsers(instances);
        right_child_->AllUsers(instances);
    } else {
        auto it = instances.begin();
        instances.insert(it, leaf()->instances().begin(), leaf()->instances().end());
    }
}

int32_t Tree::Depth() const {
    if (!InternalNode()) {
        return 0;
    } else {
        return 1 + std::max(left_child_->Depth(), right_child_->Depth());
    }
}

int32_t Tree::NumLeaves() const {
    if (!InternalNode()) {
        return 1;
    } else {
        return left_child_->NumLeaves() + right_child_->NumLeaves();
    }
}

int32_t Tree::NumSplits() const {
    if (!InternalNode()) {
        return 0;
    } else {
        return 1 + left_child_->NumSplits() + right_child_->NumSplits();
    }
}
