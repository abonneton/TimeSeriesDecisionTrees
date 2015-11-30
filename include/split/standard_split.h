#ifndef STANDARD_SPLIT_H
#define STANDARD_SPLIT_H

#include <list>
#include <string>

#include "distances_matrix.h"
#include "instance.h"
#include "split/split.h"

// Epsilon may be null

class StandardSplit: public Split {

    public:
        StandardSplit(Impurity* impurity, Instance* instance, double epsilon):
            Split(impurity), instance_(instance), epsilon_(epsilon) { };
        StandardSplit(Impurity* impurity): Split(impurity) { };
        virtual ~StandardSplit() { };

        Instance* instance() const { return instance_; };
        double epsilon() const { return epsilon_; };

        virtual void PrintJson(int32_t node_id, FILE* f) const;
        virtual void PrintLatex(int32_t node_id, FILE* f, int8_t square_size,
                std::string plot_path) const;

    protected:
        virtual void GenerateCandidateSplits(
                const std::list<Instance*>& instances,
                const DistancesMatrix& distances,
                std::list<std::shared_ptr<Split>>& candidates) const;
        virtual void ComputeEvaluation(const std::list<Instance*>& instances,
                const DistancesMatrix& distances);
        virtual bool LeftChild(const Instance* const instance,
                const DistancesMatrix& distances) const;
        virtual void Assign(std::shared_ptr<Split> split);


    private:
        Instance* instance_;
        double epsilon_;

        std::list<double> ComputeThresholds(const Instance& instance,
                const std::list<Instance*>& instances,
                const DistancesMatrix& distances) const;
};

#endif    // END_STANDARD_SPLIT_H
