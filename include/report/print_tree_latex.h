#ifndef PRINT_TREE_LATEX_H_
#define PRINT_TREE_LATEX_H_

#include "directories_configuration.h"
#include "leaf.h"
#include "report/print_tree.h"
#include "tree.h"

class PrintTreeLatex: PrintTree {

    public:
        PrintTreeLatex(DirectoriesConfiguration* conf, int32_t tree_id):
            PrintTree(conf, tree_id) {};
        virtual ~PrintTreeLatex() { };

        virtual void DisplayTree(const Tree& tree) const;
        void DisplayLeaves(const Tree& tree) const;

    private:
        void PrintTreeLatexRec(const Tree& tree, FILE* f,
                int8_t square_size) const;
        void PrintLeafNodeLatex(const Leaf& leaf, int32_t id, FILE* f) const;
        void PrintLeafLatex(const Leaf& leaf, int32_t id, FILE* f) const;
        void PrintLeavesLatexRec(const Tree& tree, FILE* f) const;
};

#endif    // END_PRINT_TREE_LATEX_H_
