#ifndef PRINT_TREE_JSON_H_
#define PRINT_TREE_JSON_H_

#include "directories_configuration.h"
#include "leaf.h"
#include "report/print_tree.h"
#include "tree.h"

class PrintTreeJson: PrintTree {

    public:
        PrintTreeJson(DirectoriesConfiguration* conf, int32_t tree_id):
            PrintTree(conf, tree_id) {};
        virtual ~PrintTreeJson() { };

        void DisplayTree(const Tree& tree) const;

    private:
        void PrintLeafNodeJson(const Leaf& leaf, FILE* f) const;
        void PrintSplitNodeJson(const Split& split, FILE* f) const;
        void PrintTreeJsonRec(const Tree& tree, FILE* f) const;
};

#endif    // END_PRINT_TREE_JSON_H_
