#include "report/print_tree_json.h"

void PrintTreeJson::DisplayTree(const Tree& tree) const {
    std::string file_name = tree.conf()->GenerateTreeFileName(conf_, tree_id_) +
        ".json";
    FILE* f = fopen(file_name.c_str(), "w");
    PrintTreeJsonRec(tree, f);
    fclose(f);
}

void PrintTreeJson::PrintTreeJsonRec(const Tree& tree, FILE* f) const {
    fprintf(f, "{\n");
    if (tree.InternalNode()) {
        tree.split()->PrintJson(tree.id(), f);
        fprintf(f, "\"children\": [\n");
        PrintTreeJsonRec(*tree.left_child(), f);
        fprintf(f, ",\n");
        PrintTreeJsonRec(*tree.right_child(), f);
        fprintf(f, "]\n");
    } else {
        PrintLeafNodeJson(*tree.leaf(), f);
    }
    fprintf(f, "}\n");
}

void PrintUserList(const std::list<Instance*>& instances, FILE* f) {
    int32_t i = 0;
    for (auto instance: instances) {
        if (i == 0) {
            fprintf(f, "\"%i\"", instance->id());
        } else {
            fprintf(f, ",\"%i\"", instance->id());
        }
        i++;
    }
}

void PrintTreeJson::PrintLeafNodeJson(const Leaf& leaf, FILE* f) const {
    fprintf(f, "\"label\": \"%i\",\n", leaf.label());
    std::list<Instance*> ko_instances;
    std::list<Instance*> ok_instances;
    for (auto instance : leaf.instances()) {
        if (instance->label()) {
            ko_instances.push_back(instance);
        } else {
            ok_instances.push_back(instance);
        }
    }
    // List of ko instances
    fprintf(f, "\"leaf_ko_instances\": [");
    PrintUserList(ko_instances, f);
    fprintf(f, "],\n");
    // List of non-ko instances
    fprintf(f, "\"leaf_ok_instances\": [");
    PrintUserList(ok_instances, f);
    fprintf(f, "]\n");
}
