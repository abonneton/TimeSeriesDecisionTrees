#include "report/print_tree_latex.h"

void PrintTreeLatex::DisplayTree(const Tree& tree) const {
    std::string file_name = tree.conf()->GenerateTreeFileName(conf_, tree_id_) +
        ".tex";
    FILE* f = fopen(file_name.c_str(), "w");
    int8_t level_distance = 4;
    int8_t sibling_distance_1 = 12;
    int8_t sibling_distance_2 = 6;
    int8_t sibling_distance_3 = 3;
    int8_t sibling_distance_4 = 2;
    int8_t sibling_distance_5 = 2;
    int8_t square_size = 2;
    fprintf(f, "\\begin{landscape}\n");
    fprintf(f, "\\begin{figure}\n");
    fprintf(f, "\\begin{tikzpicture}[level distance = %icm]\n", level_distance);
    fprintf(f, "\\tikzstyle{level 1}=[sibling distance=%icm]\n",
            sibling_distance_1);
    fprintf(f, "\\tikzstyle{level 2}=[sibling distance=%icm]\n",
            sibling_distance_2);
    fprintf(f, "\\tikzstyle{level 3}=[sibling distance=%icm]\n",
            sibling_distance_3);
    fprintf(f, "\\tikzstyle{level 4}=[sibling distance=%icm]\n",
            sibling_distance_4);
    fprintf(f, "\\tikzstyle{level 5}=[sibling distance=%icm]\n",
            sibling_distance_5);

    fprintf(f, "\\");
    PrintTreeLatexRec(tree, f, square_size);
    fprintf(f, ";\n");

    fprintf(f, "\\end{tikzpicture}\n");
    fprintf(f, "\\end{figure}\n");
    fprintf(f, "\\end{landscape}\n");
    fclose(f);
}

void PrintTreeLatex::PrintTreeLatexRec(const Tree& tree, FILE* f,
        int8_t square_size) const {
    if (tree.InternalNode()) {
        tree.split()->PrintLatex(tree.id(), f, square_size,
                conf_->plot_path());
        fprintf(f, "child {\n");
        PrintTreeLatexRec(*tree.left_child(), f, square_size);
        fprintf(f, "}\n");
        fprintf(f, "child {\n");
        PrintTreeLatexRec(*tree.right_child(), f, square_size);
        fprintf(f, "}\n");
    } else {
        PrintLeafNodeLatex(*tree.leaf(), tree.id(), f);
    }
}

void PrintTreeLatex::PrintLeafNodeLatex(const Leaf& leaf, int32_t id,
        FILE* f) const {
    int label = leaf.label() ? 0 : 1;
    std::string color;
    if (label == 0) {
        color = "green";
    } else {
        color = "red";
    }
    fprintf(f, "node[draw, fill=%s!50] (%i) {\\large{\\bfseries{%i}}}",
            color.c_str(), id, label);
}
