#ifndef IMPROVED_CLUSTER_SPLIT_H_
#define IMPROVED_CLUSTER_SPLIT_H_

#include "split/cluster_split.h"

class ImprovedClusterSplit: public ClusterSplit {

    public:
        ImprovedClusterSplit(Impurity* impurity, Instance* instance0, Instance* instance1):
            ClusterSplit(impurity, instance0, instance1) {};
        ImprovedClusterSplit(Impurity* impurity): ClusterSplit(impurity) {};
        virtual ~ImprovedClusterSplit() {};

    protected:
        virtual void GenerateCandidateSplits(
                const std::list<Instance*>& instances,
                const DistancesMatrix& distances,
                std::list<std::shared_ptr<Split>>& candidates) const;
};

#endif    // END_IMPROVED_CLUSTER_SPLIT_H_
