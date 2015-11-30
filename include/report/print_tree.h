#ifndef PRINT_TREE_H_
#define PRINT_TREE_H_

#include "directories_configuration.h"
#include "tree.h"

class PrintTree {

    public:
        PrintTree(DirectoriesConfiguration* conf, int32_t tree_id):
            conf_(conf),
            tree_id_(tree_id) {};
        virtual ~PrintTree() { };

        virtual void DisplayTree(const Tree& tree) const = 0;

    protected:
        DirectoriesConfiguration* conf_;
        int32_t tree_id_;
};

#endif    // END_PRINT_TREE_H_
