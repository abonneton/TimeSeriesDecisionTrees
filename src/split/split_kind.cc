#include "split/split_kind.h"

std::string ToString(SplitKind split_kind) {
    switch (split_kind) {
        case SplitKind::kStandard:
            return "Standard";
        case SplitKind::kPlainCluster:
            return "Cluster";
        case SplitKind::kImprovedCluster:
            return "Cluster";
        default:
            fprintf(stderr, "ERR: wrong split kind");
            return "ERR: wrong split kind";
    }
}

std::string ToString(const std::list<SplitKind>& split_kinds) {
    std::string res = "";
    for (auto split: split_kinds) {
        res += ToString(split);
    }
    return res;
}
