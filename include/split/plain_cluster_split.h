#ifndef PLAIN_CLUSTER_SPLIT_H_
#define PLAIN_CLUSTER_SPLIT_H_

#include "split/cluster_split.h"

class PlainClusterSplit: public ClusterSplit {

    public:
        PlainClusterSplit(Impurity* impurity, Instance* instance0, Instance* instance1):
            ClusterSplit(impurity, instance0, instance1) {};
        PlainClusterSplit(Impurity* impurity): ClusterSplit(impurity) {};
        virtual ~PlainClusterSplit() {};

    protected:
        virtual void GenerateCandidateSplits(
                const std::list<Instance*>& instances,
                const DistancesMatrix& distances,
                std::list<std::shared_ptr<Split>>& candidates) const;
};

#endif    // END_PLAIN_CLUSTER_SPLIT_H_
