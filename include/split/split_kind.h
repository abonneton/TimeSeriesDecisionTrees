#ifndef SPLIT_KIND_H
#define SPLIT_KIND_H

#include <string>
#include <list>

enum class SplitKind {
    kStandard = 0,
    kPlainCluster = 1,
    kImprovedCluster = 2
};

std::string ToString(SplitKind split_kind);
std::string ToString(const std::list<SplitKind>& split_kinds);

#endif    // END_SPLIT_KIND_H_
