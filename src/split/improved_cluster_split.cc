#include "split/improved_cluster_split.h"

void ImprovedClusterSplit::GenerateCandidateSplits(
        const std::list<Instance*>& instances,
        const DistancesMatrix&,
        std::list<std::shared_ptr<Split>>& candidates) const {
    for (auto instance_ok: instances) {
        if (instance_ok->label()) continue;
        for (auto instance_ko: instances) {
            if (!instance_ko->label()) continue;
            std::shared_ptr<ImprovedClusterSplit> candidate =
              std::shared_ptr<ImprovedClusterSplit>(new ImprovedClusterSplit(
                    impurity_,
                    instance_ok,
                    instance_ko));
            candidates.push_back(candidate);
        }
    }
}
