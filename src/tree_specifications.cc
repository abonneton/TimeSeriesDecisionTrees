#include "tree_specifications.h"

#include "utils.h"


void TreeSpecifications::Add(const TreeSpecifications& specifications) {
    building_time_ += specifications.building_time_;
    depth_ += specifications.depth_;
    num_splits_ += specifications.num_splits_;
    num_leaves_ += specifications.num_leaves_;
}

void TreeSpecifications::ComputeMean(int32_t num) {
    building_time_ /= (double)num;
    depth_ /= (double)num;
    num_splits_ /= (double)num;
    num_leaves_ /= (double)num;
}

void TreeSpecifications::PrintLatex(DirectoriesConfiguration* conf,
        const TreeConfiguration& tree_conf, int32_t tree_id) const {
    std::string info_file = tree_conf.GenerateTreeStatsFileName(conf, tree_id) +
        ".tex";
    FILE* f = fopen(info_file.c_str(), "w");
    fprintf(f, "\\begin{tabular}{|l|l|}\n");
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Depth}                  & %i \\\\ \n", depth_);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Number of splits}       & %i \\\\ \n", num_splits_);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Number of leaves}       & %i \\\\ \n", num_leaves_);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Building execution time} & %s \\\\ \n",
            TimePrettyPrint(building_time_).c_str());
    fprintf(f, "\\hline\n");
    fprintf(f, "\\end{tabular}\n");
    fclose(f);
}
