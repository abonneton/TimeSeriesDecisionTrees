#include "split/plain_cluster_split.h"

void PlainClusterSplit::GenerateCandidateSplits(
        const std::list<Instance*>& instances,
        const DistancesMatrix&,
        std::list<std::shared_ptr<Split>>& candidates) const {
    int32_t i = 1;
    int32_t j = 0;
    for (auto it1 = std::next(instances.begin()); it1 != instances.end(); ++it1) {
        j = 0;
        for (auto instance: instances) {
            if (j == i) break;
            std::shared_ptr<PlainClusterSplit> candidate = std::shared_ptr<PlainClusterSplit>(
                new PlainClusterSplit(impurity_,
                    *it1, instance));
            candidates.push_back(candidate);
            j++;
        }
        i++;
    }
}
