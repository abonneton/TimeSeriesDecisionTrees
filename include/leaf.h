#ifndef LEAF_H_
#define LEAF_H_

#include <list>

#include "instance.h"

class Leaf {

    public:
        Leaf(bool label, const std::list<Instance*>& instances):
            label_(label), instances_(instances) {};
        ~Leaf() {};

        bool label() const { return label_; };
        const std::list<Instance*>& instances() const { return instances_; };

    private:
        bool label_;
        std::list<Instance*> instances_;
};

#endif    // END_LEAF_H_
