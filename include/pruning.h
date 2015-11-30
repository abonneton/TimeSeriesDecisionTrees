#ifndef PRUNING_H_
#define PRUNING_H_

#include <list>
#include <memory>

#include "decision_tree.h"
#include "directories_configuration.h"
#include "instances.h"
#include "leaf.h"
#include "tree_configuration.h"

class Pruning {

    public:
        Pruning(TreeConfiguration* tree_conf, const Instances& training_data);
        ~Pruning();

        DecisionTree* full_decision_tree() const {
            return full_decision_tree_; }
        const std::list<DecisionTree*>& pruned_decision_trees() const {
            return pruned_decision_trees_; }
         DecisionTree* best_decision_tree() const {
            return best_decision_tree_; }

        TreeConfiguration* tree_conf() const { return tree_conf_; };

        void GenerateFullDecisionTree();
        void GeneratePrunedDecisionTrees();
        void SelectBestDecisionTree();

        void BestDecisionTreeClassify(const Instances& testing_data,
                ConfusionMatrix& testing_evaluation);
        void FullDecisionTreeClassify(const Instances& testing_data,
                ConfusionMatrix& testing_evaluation);

    private:
        TreeConfiguration* tree_conf_;
        // Data for building the full tree
        Instances building_data_;
        // Data for finding the best pruned tree
        Instances pruning_data_;

        DecisionTree* full_decision_tree_;
        std::list<DecisionTree*> pruned_decision_trees_;
        DecisionTree* best_decision_tree_;

        DecisionTree* NextPrunedDecisionTree(DecisionTree* decision_tree);
        std::shared_ptr<Tree> NextPrunedTree(std::shared_ptr<Tree> tree);
};

#endif    // END_PRUNING_H
