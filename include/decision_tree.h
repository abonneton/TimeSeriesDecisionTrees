#ifndef DECISION_TREE_H_
#define DECISION_TREE_H_

#include <list>

#include "instance.h"
#include "tree.h"
#include "tree_configuration.h"
#include "tree_evaluation.h"

class DecisionTree {

    public:
        DecisionTree(TreeConfiguration* tree_conf, int32_t tree_id):
            tree_conf_(tree_conf),
            tree_(NULL),
            tree_eval_(NULL),
            tree_id_(tree_id) {};
        DecisionTree(std::shared_ptr<Tree> tree, double building_time,
                const std::list<Instance*>& training_data, int32_t tree_id);
        ~DecisionTree();

        TreeConfiguration* tree_conf() const { return tree_conf_; };
        std::shared_ptr<Tree> tree() const { return tree_; };
        TreeEvaluation* tree_eval() const { return tree_eval_; };
        int32_t id() const { return tree_id_; };

        void BuildTree(const std::list<Instance*>& instances);

        bool Classify(const Instance& instance) const;
        void Classify(const std::list<Instance*>& testing_set) const;
        void Classify(const std::list<Instance*>& testing_set,
                ConfusionMatrix& confusion_matrix) const;

    private:
        TreeConfiguration* tree_conf_;
        std::shared_ptr<Tree> tree_;
        TreeEvaluation* tree_eval_;

        int32_t tree_id_;
};

#endif    // END_DECISION_TREE_H_
