#ifndef CLUSTER_SPLIT_H_
#define CLUSTER_SPLIT_H_

#include "split/split.h"

class ClusterSplit: public Split {

    public:
        ClusterSplit(Impurity* impurity, Instance* instance0, Instance* instance1):
            Split(impurity), instance0_(instance0), instance1_(instance1) {};
        ClusterSplit(Impurity* impurity): Split(impurity) {};
        virtual ~ClusterSplit() {};

        Instance* instance0() const { return instance0_; };
        Instance* instance1() const { return instance1_; };

        virtual void PrintJson(int32_t node_id, FILE* f) const;
        virtual void PrintLatex(int32_t node_id, FILE* f,
                int8_t square_size, std::string plot_path) const;

    protected:
        virtual void GenerateCandidateSplits(
                const std::list<Instance*>& instances,
                const DistancesMatrix& distances,
                std::list<std::shared_ptr<Split>>& candidates) const = 0;
        virtual void ComputeEvaluation(const std::list<Instance*>& instances,
                const DistancesMatrix& distances);
        virtual bool LeftChild(const Instance* const instance,
                const DistancesMatrix& distances_matrix) const;
        virtual void Assign(std::shared_ptr<Split> split);

    private:
        Instance* instance0_;
        Instance* instance1_;
};

#endif    // END_CLUSTER_SPLIT_H_
