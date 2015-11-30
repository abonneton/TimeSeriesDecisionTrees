#include "report/report.h"

void Report::GenerateBeginning(FILE* f, std::string title) const {
    fprintf(f, "\\documentclass[11pt]{article}\n");
    fprintf(f, "\\usepackage{lscape}\n");
    fprintf(f, "\\usepackage{graphicx}\n");
    fprintf(f, "\\usepackage{tikz}\n");
    fprintf(f, "\\usepackage{caption}\n");
    fprintf(f, "\\usepackage{subcaption}\n");
    fprintf(f, "\\usepackage{amssymb}\n");
    fprintf(f, "\\usepackage{amsmath}\n");
    fprintf(f, "\\usepackage{amsfonts}\n");
    fprintf(f, "\\usepackage{fullpage}\n");
    fprintf(f, "\\usepackage{multicol}\n");
    fprintf(f, "\\usepackage[utf8]{inputenc}\n");
    fprintf(f, "\\usepackage{color}\n\n");

    fprintf(f, "\\title{\\bfseries{%s}}\n",
            title.c_str());
    fprintf(f, "\\date{\\today}\n");
    fprintf(f, "\\begin{document}\n");
    fprintf(f, "\\maketitle\n\n");
}

void Report::GenerateEnd(FILE* f) const {
    fprintf(f, "\\end{document}\n");
}

void Report::GenerateDataInformation(FILE* f, TreeConfiguration* tree_conf) const {
    fprintf(f, "\\section*{Data information}\n");
    std::string data_info_file = tree_conf->GenerateDataInfoFileName(dir_conf_);
    fprintf(f, "\\input{%s}\n", data_info_file.c_str());
    instances_->PrintInfo((data_info_file + ".tex").c_str());
}
