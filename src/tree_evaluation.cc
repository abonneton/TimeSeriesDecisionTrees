#include "tree_evaluation.h"

#include "utils.h"

TreeEvaluation::TreeEvaluation(): specifications_() {
    training_evaluation_.Init();
    testing_evaluation_.Init();
}

TreeEvaluation::TreeEvaluation(double building_time, int32_t depth,
        int32_t num_splits,
        int32_t num_leaves):
    specifications_(building_time, depth, num_splits, num_leaves) {
    training_evaluation_.Init();
    testing_evaluation_.Init();
}

void TreeEvaluation::PrintLatex(DirectoriesConfiguration* conf,
        const TreeConfiguration& tree_conf, int32_t tree_id) const {
    specifications_.PrintLatex(conf, tree_conf, tree_id);
}
