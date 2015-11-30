#ifndef TREE_H_
#define TREE_H_

#include <stdint.h>

#include <list>
#include <memory>
#include <mutex>

#include "confusion_matrix.h"
#include "instance.h"
#include "error_complexity_measure.h"
#include "leaf.h"
#include "split/split.h"
#include "tree_configuration.h"

class Tree {

    public:
        Tree(int32_t id, TreeConfiguration* conf);

        Tree(std::shared_ptr<Tree> tree, std::shared_ptr<Split> worst_split);

        ~Tree();

        int32_t id() const { return id_; };
        std::shared_ptr<Leaf> leaf() const { return leaf_; };
        std::shared_ptr<Split> split() const { return split_; };
        std::shared_ptr<Tree> left_child() const { return left_child_; };
        std::shared_ptr<Tree> right_child() const { return right_child_; };

        bool majority_label() const { return majority_label_; };
        int32_t num_ok() const { return num_ok_; };
        int32_t num_ko() const { return num_ko_; };
        int32_t num_errors() const { return num_errors_; };

        TreeConfiguration* conf() const { return conf_; };

        double Build(const std::list<Instance*>& instances,
                ConfusionMatrix& training_confusion_matrix);

        bool Classify(const Instance& instance) const;
        void Classify(const std::list<Instance*>& testing_set,
                ConfusionMatrix& confusion_matrix) const;

        bool InternalNode() const;

        void ComputeSplitsErrorComplexity();
        void FindWorstSplit(std::shared_ptr<Split>* worst_split);

        int32_t Depth() const;
        int32_t NumLeaves() const;
        int32_t NumSplits() const;

    private:
        std::shared_ptr<Leaf> leaf_;
        std::shared_ptr<Split> split_;

        bool majority_label_;
        int32_t num_ok_;
        int32_t num_ko_;
        int32_t num_errors_;

        ErrorComplexityMeasure* error_complexity_;

        int32_t id_;
        std::shared_ptr<Tree> left_child_;
        std::shared_ptr<Tree> right_child_;

        TreeConfiguration* conf_;

        void FindWorstSplitRec(std::shared_ptr<Split>* worst_split,
                double& worst_error_complexity);

        void BuildRec(const std::list<Instance*>& instances,
                ConfusionMatrix& training_confusion_matrix);
        bool StopCase(const std::list<Instance*>& instances);

        void CreateSplitCandidates(std::list<std::shared_ptr<Split>>& split_candidates,
                const std::list<Instance*>& instances);
        void SelectBestSplit(const std::list<std::shared_ptr<Split>>& split_candidates);
        void SplitData(const std::list<Instance*>& instances,
                std::list<Instance*>& left_child_data,
                std::list<Instance*>& right_child_data);

        void BuildChildren(const std::list<Instance*>& left_child_data,
                const std::list<Instance*>& right_child_data,
                ConfusionMatrix& training_confusion_matrix);

        void AllUsers(std::list<Instance*>& instances);

};

#endif    // END_TREE_H_
