#include "evaluation_configuration.h"

std::string EvaluationConfiguration::GenerateOutputFilesSuffix() const {
    std::string basename = ("__g" + std::to_string(g_));
    basename += ("_distanceMultithreads_" +
            std::to_string(distance_multi_threads_));
    return basename;
}

void EvaluationConfiguration::PrintInfo(const char* info_file) const {
    FILE* f = fopen(info_file, "w");
    fprintf(f, "\\begin{tabular}{|l|l|}\n");
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{g}        & %i \\\\ \n", g_);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\emph{Distance multi-threads}  & %i \\\\ \n",
            distance_multi_threads_);
    fprintf(f, "\\hline\n");
    fprintf(f, "\\end{tabular}\n");
    fclose(f);
}

std::string EvaluationConfiguration::GenerateEvaluationInfoFileName(
        DirectoriesConfiguration* conf) const {
    return GenerateOutputFilesBasename(conf) + "__eval_info";
}

std::string EvaluationConfiguration::GenerateReportFileName(
        DirectoriesConfiguration* conf) const {
    return GenerateOutputFilesBasename(conf) + "__report.tex";
}

std::string EvaluationConfiguration::GenerateCsvFileName(
        DirectoriesConfiguration* conf) const {
    return GenerateOutputFilesBasename(conf) + "__comparaison.csv";
}

std::string EvaluationConfiguration::GenerateOutputFilesBasename(
        DirectoriesConfiguration* conf) const {
    std::string basename = conf->output_path();
    basename += conf->input_file();
    basename += GenerateOutputFilesSuffix();
    return basename;
}
