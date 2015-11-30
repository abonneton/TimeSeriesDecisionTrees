#ifndef SPLIT_H_
#define SPLIT_H_

#include <list>
#include <memory>
#include <string>

#include "distances_matrix.h"
#include "instance.h"
#include "impurity/impurity.h"

class Split {

    public:
        Split(Impurity* impurity): impurity_(impurity) { };
        virtual ~Split() { };

        double info_gain() const { return info_gain_; };
        double intra_cluster_dist() const { return intra_cluster_dist_; };
        double inter_cluster_dist() const { return inter_cluster_dist_; };

        virtual bool LeftChild(const Instance* const instance,
                const DistancesMatrix& distances_matrix) const = 0;
        double FindBestSplit(const std::list<Instance*>& instances,
                const DistancesMatrix& distances);
        void SplitInstances(const std::list<Instance*>& instances,
                const DistancesMatrix& distances,
                std::list<Instance*>& left_child,
                std::list<Instance*>& right_child);

        virtual void PrintJson(int32_t node_id, FILE* f) const = 0;
        virtual void PrintLatex(int32_t node_id, FILE* f, int8_t square_size,
                std::string plot_path) const = 0;

        bool IsBetterThan(std::shared_ptr<Split> candidate) const;

    protected:
        virtual void GenerateCandidateSplits(
                const std::list<Instance*>& instances,
                const DistancesMatrix& distances,
                std::list<std::shared_ptr<Split>>& candidates) const = 0;
        virtual void ComputeEvaluation(const std::list<Instance*>& instances,
                const DistancesMatrix& distances) = 0;
        virtual void Assign(std::shared_ptr<Split> split);

        void PrintSplitInfoJson(FILE* f) const;

        Impurity* impurity_;
        double info_gain_;
        double intra_cluster_dist_;
        double inter_cluster_dist_;

    private:
        void DeleteCandidateSplits(std::list<Split*>& candidates);

        bool IsBetterThanIG(const Split& candidate) const;
        bool IsBetterThanInter(const Split& candidate) const;
        bool IsBetterThanIntra(const Split& candidate) const;
};

#endif    // END_SPLIT_H_
