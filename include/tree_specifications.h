#ifndef TREE_SPECIFICATIONS_H_
#define TREE_SPECIFICATIONS_H_

#include <stdint.h>

#include "confusion_matrix.h"
#include "directories_configuration.h"
#include "tree_configuration.h"

class TreeSpecifications {

    public:
        TreeSpecifications(): building_time_(0), depth_(0), num_splits_(0),
            num_leaves_(0) {};
        TreeSpecifications(double building_time, int32_t depth,
                int32_t num_splits, int32_t num_leaves):
            building_time_(building_time),
            depth_(depth),
            num_splits_(num_splits),
            num_leaves_(num_leaves) {};

        ~TreeSpecifications() {};

        double building_time() const { return building_time_; };
        int32_t depth() const { return depth_; };
        int32_t num_leaves() const { return num_leaves_; };
        int32_t num_splits() const { return num_splits_; };

        void SetBuildingTime(double time) { building_time_ = time; };
        void SetDepth(int32_t depth) { depth_ = depth; };
        void SetNumSplits(int32_t num_splits) { num_splits_ = num_splits; };
        void SetNumLeaves(int32_t num_leaves) { num_leaves_ = num_leaves; };

        void Add(const TreeSpecifications& specifications);
        void ComputeMean(int32_t num);

        void PrintLatex(DirectoriesConfiguration* conf,
                const TreeConfiguration& tree_conf, int32_t tree_id) const;

    private:
        double building_time_;
        int32_t depth_;
        int32_t num_splits_;
        int32_t num_leaves_;
};

#endif    // END_TREE_SPECIFICATIONS_H_
