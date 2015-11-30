#include "tree_configuration.h"

std::string TreeConfiguration::GenerateOutputSuffixFileName() const {
    std::string basename = ("__" + ToString(*split_kinds_));
    basename += ("_" + distances_matrix_->distance().PrintDistance());
    return basename;
}

void TreeConfiguration::PrintInfoLatex(DirectoriesConfiguration* conf,
        int32_t tree_id) const {
    std::string info_file = GenerateTreeInfoFileName(conf, tree_id) + ".tex";
    FILE* f = fopen(info_file.c_str(), "w");
    fprintf(f, "\\begin{tabular}{|l|l|}\n");
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Distance}    & %s \\\\ \n",
            distances_matrix_->distance().PrintDistance().c_str());
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Impurity}    & %s \\\\ \n",
            impurity_->PrintImpurityMeasure().c_str());
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Split}       & %s \\\\ \n",
            ToString(*split_kinds_).c_str());
    fprintf(f, "\\hline\n");
    fprintf(f, "\\end{tabular}\n");
    fclose(f);
}

void TreeConfiguration::PrintInfoJson(DirectoriesConfiguration* conf,
        int32_t tree_id) const {
    std::string info_file = GenerateTreeInfoFileName(conf, tree_id) + ".json";
    FILE* f = fopen(info_file.c_str(), "w");
    fprintf(f, "{\n");
    fprintf(f, "\t\"distance\":\"%s\",\n", distances_matrix_->distance().
            PrintDistance().c_str());
    fprintf(f, "\t\"impurity\":\"%s\",\n",
            impurity_->PrintImpurityMeasure().c_str());
    fprintf(f, "\t\"split\":\"%s\"\n", ToString(*split_kinds_).c_str());
    fprintf(f, "}\n");
    fclose(f);
}

std::string TreeConfiguration::GenerateOutputFilesBasename(
        DirectoriesConfiguration* conf) const {
    std::string basename = conf->output_path();
    basename += conf->input_file();
    basename += GenerateOutputSuffixFileName();
    return basename;
}

std::string TreeConfiguration::GeneratePruningDirectory(
        DirectoriesConfiguration* conf) const {
    return GenerateOutputFilesBasename(conf) + "/";
}

std::string TreeConfiguration::GenerateTreeDirectory(
        DirectoriesConfiguration* conf, int32_t tree_id) const {
    return GeneratePruningDirectory(conf) +
        "Tree_" + std::to_string(tree_id)+ "/";
}

std::string TreeConfiguration::GenerateDataInfoFileName(
        DirectoriesConfiguration* conf) const {
    return GeneratePruningDirectory(conf) + "data_info";
}

std::string TreeConfiguration::GenerateTreeInfoFileName(
        DirectoriesConfiguration* conf, int32_t tree_id) const {
    return GenerateTreeDirectory(conf, tree_id) + "Tree_" +
        std::to_string(tree_id) + "__tree_info";
}

std::string TreeConfiguration::GenerateTreeStatsFileName(
        DirectoriesConfiguration* conf, int32_t tree_id) const {
    return GenerateTreeDirectory(conf, tree_id) + "Tree_" +
       std::to_string(tree_id) + "__tree_stats";
}

std::string TreeConfiguration::GenerateEvalFileName(bool train,
        DirectoriesConfiguration* conf, int32_t tree_id) const {
    std::string eval_kind = train ? "train" : "test";
    return GenerateTreeDirectory(conf, tree_id) + "Tree_" +
        std::to_string(tree_id) + "__" + eval_kind + "_eval";
}

std::string TreeConfiguration::GenerateMisclassifiedUsersFileName(
        DirectoriesConfiguration* conf, int32_t tree_id) const {
    return GenerateTreeDirectory(conf, tree_id) + "Tree_" +
        std::to_string(tree_id) + "__misclassified_instances";
}

std::string TreeConfiguration::GenerateTreeFileName(
        DirectoriesConfiguration* conf, int32_t tree_id) const {
    return GenerateTreeDirectory(conf, tree_id) + "Tree_" +
        std::to_string(tree_id) + "__tree";
}

std::string TreeConfiguration::GenerateReportFileName(bool with_tree,
        DirectoriesConfiguration* conf, int32_t tree_id) const {
    std::string base_name = GenerateTreeDirectory(conf, tree_id);
    base_name +=  ("Tree_" + std::to_string(tree_id));
    if (with_tree) base_name += "_withTree";
    return base_name + "__report.tex";
}

std::string TreeConfiguration::GeneratePruningReportFileName(
        DirectoriesConfiguration* conf) const {
    std::string base_name = GeneratePruningDirectory(conf);
    base_name +=  (conf->input_file() + GenerateOutputSuffixFileName());
    return base_name + "__pruning_report.tex";
}
