#include "split/split.h"

double Split::FindBestSplit(const std::list<Instance*>& instances,
        const DistancesMatrix& distances) {
    std::list<std::shared_ptr<Split>> candidates;
    GenerateCandidateSplits(instances, distances, candidates);
    std::shared_ptr<Split> best_split = NULL;
    for (auto candidate: candidates) {
        candidate->ComputeEvaluation(instances, distances);
        if (candidate->IsBetterThan(best_split)) {
            best_split = candidate;
        }
    }
    Assign(best_split);
    //DeleteCandidateSplits(candidates);
    return info_gain_;
}

void Split::PrintSplitInfoJson(FILE* f) const{
    fprintf(f, "\"info_gain\": \"%.2f\",\n", info_gain_);
    fprintf(f, "\"intra_cluster_distance\": \"%.2f\",\n", intra_cluster_dist_);
    fprintf(f, "\"inter_cluster_distance\": \"%.2f\",\n", inter_cluster_dist_);
}

void Split::Assign(std::shared_ptr<Split> split) {
    info_gain_ = split->info_gain();
    intra_cluster_dist_ = split->intra_cluster_dist();
    inter_cluster_dist_ = split->inter_cluster_dist();
}

void Split::SplitInstances(const std::list<Instance*>& instances,
        const DistancesMatrix& distances,
        std::list<Instance*>& left_child,
        std::list<Instance*>& right_child) {
    for (auto instance: instances) {
        if (LeftChild(instance, distances)) {
            left_child.push_back(instance);
        } else {
            right_child.push_back(instance);
        }
    }
}

void Split::DeleteCandidateSplits(std::list<Split*>& candidates) {
    for (auto candidate: candidates) {
        delete (candidate); candidate = NULL;
    }
}

bool Split::IsBetterThan(std::shared_ptr<Split> candidate) const {
    if (candidate == NULL) {
        return true;
    } else {
        return IsBetterThanIG(*candidate);
    }
}

bool Split::IsBetterThanIG(const Split& candidate) const {
    return (info_gain_ > candidate.info_gain_);
}

bool Split::IsBetterThanInter(const Split& candidate) const {
    if (inter_cluster_dist_ > candidate.inter_cluster_dist_) {
        return true;
    } else if (inter_cluster_dist_ < candidate.inter_cluster_dist_) {
        return false;
    } else { //inter_cluster_dist_ == candidate->inter_cluster_dist_
        return IsBetterThanIntra(candidate);
    }
}

bool Split::IsBetterThanIntra(const Split& candidate) const {
    return intra_cluster_dist_ < candidate.intra_cluster_dist_;
}
