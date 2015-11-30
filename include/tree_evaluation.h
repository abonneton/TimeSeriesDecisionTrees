#ifndef TREE_EVALUATION_H_
#define TREE_EVALUATION_H_

#include "confusion_matrix.h"
#include "tree_specifications.h"

class TreeEvaluation {

    public:
        TreeEvaluation();
        TreeEvaluation(double building_time, int32_t depth, int32_t num_splits,
                int32_t num_leaves);
        ~TreeEvaluation() {};

        ConfusionMatrix* training_evaluation() { return &training_evaluation_; }
        ConfusionMatrix* testing_evaluation() { return &testing_evaluation_; }

        // Building time in seconds
        double building_time() const { return specifications_.building_time(); };
        int32_t depth() const { return specifications_.depth(); };
        int32_t num_leaves() const { return specifications_.num_leaves(); };
        int32_t num_splits() const { return specifications_.num_splits(); };

        void SetBuildingTime(double time) {
            specifications_.SetBuildingTime(time); };
        void SetDepth(int32_t depth) {
            specifications_.SetDepth(depth); };
        void SetNumSplits(int32_t num_splits) {
            specifications_.SetNumSplits(num_splits); };
        void SetNumLeaves(int32_t num_leaves) {
            specifications_.SetNumLeaves(num_leaves); };

        void AddEvaluation(TreeEvaluation* tree_eval) {
            specifications_.Add(tree_eval->specifications_);
        }
        void ComputeMeanEvaluation(int32_t num) {
            specifications_.ComputeMean(num);
        }

        void PrintLatex(DirectoriesConfiguration* conf,
                const TreeConfiguration& tree_conf, int32_t tree_id) const;

    private:
        ConfusionMatrix training_evaluation_;
        ConfusionMatrix testing_evaluation_;
        TreeSpecifications specifications_;
};

#endif    // END_TREE_EVALUATION_H_
