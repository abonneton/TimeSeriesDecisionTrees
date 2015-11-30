#include "distance_evaluation_configuration.h"

#include "impurity/entropy_impurity.h"

DistanceEvaluationConfiguration::~DistanceEvaluationConfiguration() {
    delete(impurity_); impurity_ = NULL;
}

TreeConfiguration* DistanceEvaluationConfiguration::GenerateTreeConfiguration() const {
    TreeConfiguration* tree_configuration = new TreeConfiguration(
            split_kinds_,
            impurity_);
    return tree_configuration;
}

std::string DistanceEvaluationConfiguration::GenerateReportFileName(
        DirectoriesConfiguration* conf) const {
    return GenerateOutputFilesBasename(conf) + "__report.tex";
}

std::string DistanceEvaluationConfiguration::GenerateSummaryFileName(
        DirectoriesConfiguration* conf) const {
    return GenerateOutputFilesBasename(conf) + "__summary.tex";
}

std::string DistanceEvaluationConfiguration::GenerateOutputFilesBasename(
        DirectoriesConfiguration* conf) const {
    std::string basename = conf->output_path();
    basename += conf->input_file();
    basename += ("__" + distance_->PrintDistance());
    basename += eval_conf_->GenerateOutputFilesSuffix();
    return basename;
}
