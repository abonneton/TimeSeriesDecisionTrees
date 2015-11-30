#include "split/cluster_split.h"

void ClusterSplit::PrintJson(int32_t node_id, FILE* f) const {
    fprintf(f, "\"kind\": \"Cluster\",\n");
    fprintf(f, "\"node_id\": \"%i\",\n", node_id);
    fprintf(f, "\"instance_0_id\": \"%i\",\n", instance0_->id());
    fprintf(f, "\"instance_1_id\": \"%i\",\n", instance1_->id());
    PrintSplitInfoJson(f);
}

void ClusterSplit::PrintLatex(int32_t node_id, FILE* f,
        int8_t square_size, std::string plot_path) const {
    // Begining of the latex node
    fprintf(f, "node[draw] (%i) {\n", node_id);
    // First instance time serie
    fprintf(f, "\\includegraphics[width=%icm, height=%icm,",
            square_size, square_size);
    fprintf(f, "type=pdf, ext = .pdf, read = .pdf]");
    fprintf(f, "{%s%s}\n", plot_path.c_str(), instance0_->filename().c_str());
    // Second instance time serie
    fprintf(f, "\\includegraphics[width=%icm, height=%icm,",
            square_size, square_size);
    fprintf(f, "type=pdf, ext = .pdf, read = .pdf]");
    fprintf(f, "{%s%s}\n", plot_path.c_str(), instance1_->filename().c_str());
    // End of the latex node
    fprintf(f, "}\n");
}

void ClusterSplit::ComputeEvaluation(const std::list<Instance*>& instances,
        const DistancesMatrix& distances_matrix) {
    //intra_cluster_dist_ = distances_matrix->GetDistance(instance0_, instance1_);
    //inter_cluster_dist_ =  0;
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
            //inter_cluster_dist_ +=
            //distances_matrix->GetDistance(instance, instance0_);
        } else {
            if (instance->label()) {
                num_ko_right_child++;
            }
            //inter_cluster_dist_ +=
            //distances_matrix->GetDistance(instance, instance1_);
        }
    }

    //inter_cluster_dist_ /= (double)num_instances;

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

bool ClusterSplit::LeftChild(const Instance* const instance,
        const DistancesMatrix& distances_matrix) const {
    if (instance == instance0_) {
        return true;
    } else if (instance == instance1_) {
        return false;
    } else {
        if (distances_matrix.GetDistance(*instance, *instance0_) <
                distances_matrix.GetDistance(*instance, *instance1_)) return true;
        if (distances_matrix.GetDistance(*instance, *instance0_) >
                distances_matrix.GetDistance(*instance, *instance1_)) return false;
        return true;
    }
}

void ClusterSplit::Assign(std::shared_ptr<Split> split) {
    Split::Assign(split);
    std::shared_ptr<ClusterSplit> cluster_split =
      std::dynamic_pointer_cast<ClusterSplit>(split);
    instance0_ = cluster_split->instance0();
    instance1_ = cluster_split->instance1();
}
