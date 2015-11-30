#include "split/standard_split.h"

void StandardSplit::PrintJson(int32_t node_id, FILE* f) const {
    fprintf(f, "\"kind\": \"Standard\",\n");
    fprintf(f, "\"node_id\": \"%i\",\n", node_id);
    fprintf(f, "\"instance_id\": \"%i\",\n", instance_->id());
    fprintf(f, "\"epsilon\": \"%.2f\",\n", epsilon_);
    PrintSplitInfoJson(f);
}

void StandardSplit::PrintLatex(int32_t node_id, FILE* f,
        int8_t square_size, std::string plot_path) const {
    // Begining of the latex node
    fprintf(f, "node[draw] (%i) {\n", node_id);
    // User time serie
    fprintf(f, "\\includegraphics[width=%icm, height=%icm,",
            square_size, square_size);
    fprintf(f, "type=pdf, ext = .pdf, read = .pdf]");
    fprintf(f, "{%s%s}\n", plot_path.c_str(), instance_->filename().c_str());
    // Epsilon threshold
    fprintf(f, "\\ $\\epsilon = %.2f$", epsilon_);
    // End of the latex node
    fprintf(f, "}\n");
}

void StandardSplit::GenerateCandidateSplits(const std::list<Instance*>& instances,
        const DistancesMatrix& distances_matrix,
        std::list<std::shared_ptr<Split>>& candidates) const {
    for (auto instance: instances) {
        auto thresholds = ComputeThresholds(*instance, instances,
                distances_matrix);
        for (auto t: thresholds) {
            std::shared_ptr<StandardSplit> candidate =
                std::shared_ptr<StandardSplit>(
                        new StandardSplit(impurity_, instance, t));
            candidates.push_back(candidate);
        }
    }
}

void StandardSplit::ComputeEvaluation(const std::list<Instance*>& instances,
        const DistancesMatrix& distances_matrix) {
    int32_t num_instances = 0;
    int32_t num_left_child = 0;
    int32_t num_ko_left_child = 0;
    int32_t num_ko_right_child = 0;

    for (auto instance: instances) {
        num_instances++;
        if (LeftChild(instance, distances_matrix)) {
            num_left_child++;
            if (instance->label()) {
                num_ko_left_child++;
            }
        } else {
            if (instance->label()) {
                num_ko_right_child++;
            }
        }
    }

    double ko_probability =
        (double) (num_ko_left_child + num_ko_right_child) /
        (double) num_instances;
    double ko_probability_left_child =
        (double) num_ko_left_child / (double) num_left_child;
    double ko_probability_right_child =
        (double) num_ko_right_child /
        (double) (num_instances - num_left_child);
    double probability_left_child =
        (double) num_left_child / (double) num_instances;

    double root_impurity = impurity_->Compute(ko_probability);
    double left_child_impurity = impurity_->Compute(
            ko_probability_left_child);
    double right_child_impurity = impurity_->Compute(
            ko_probability_right_child);

    info_gain_ = root_impurity - (probability_left_child * left_child_impurity
            +  (1 - probability_left_child) * right_child_impurity);

    intra_cluster_dist_ = 0;
    inter_cluster_dist_ = 0;
}

bool StandardSplit::LeftChild(const Instance* const instance,
        const DistancesMatrix& distances_matrix) const {
    return (distances_matrix.GetDistance(*instance, *instance_) < epsilon_);
}

std::list<double> StandardSplit::ComputeThresholds(const Instance& instance,
        const std::list<Instance*>& instances,
        const DistancesMatrix& distances_matrix) const {
    std::list<double> instance_distances;
    for (auto inst: instances) {
        instance_distances.push_front(distances_matrix.GetDistance(*inst, instance));
    }
    instance_distances.sort();
    // Delete the duplicates from the distance list
    // Very important in order to prevent empty leaves !!
    instance_distances.unique();
    double prev = 0;
    std::list<double> thresholds;
    for (auto instance_distance: instance_distances) {
        thresholds.push_front((prev + instance_distance) / 2);
        prev = instance_distance;
    }
    thresholds.pop_back();  // Delete the threshold 0 corresponding to the
    // distance the instance and itself
    return thresholds;
}

void StandardSplit::Assign(std::shared_ptr<Split> split) {
    Split::Assign(split);
    std::shared_ptr<StandardSplit> standard_split =
        std::dynamic_pointer_cast<StandardSplit>(split);
    instance_ = standard_split->instance();
    epsilon_ = standard_split->epsilon();
}
