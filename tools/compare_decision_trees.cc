#include <iostream>
#include <list>
#include <string>

#include "directories_configuration.h"
#include "distance.h"
#include "distance_comparaison.h"
#include "report/distance_comparaison_report.h"
#include "distances/dtw_value_distance.h"
#include "distances/lp_local_distance.h"
#include "evaluation_configuration.h"
#include "split/split_kind.h"

int main(int argc, char* argv[]) {
    
    if (argc != 4) {
        std::cout <<"./compare_decision_trees \"output_dir\" \"data_dir\" \"input_filename\"" <<std::endl;
        return 1;
    }
    std::string output_path     = std::string(argv[1]);
    std::string data_path       = std::string(argv[2]);
    std::string input_file_name = std::string(argv[3]);

    DirectoriesConfiguration directories_conf(output_path,
            data_path, input_file_name);
    Instances instances(directories_conf);

    int8_t g = 5;
    int32_t distance_multi_threads = 5;

    std::list<std::list<SplitKind>*> splits;

    std::list<SplitKind> plain_cluster{SplitKind::kPlainCluster};
    //std::list<SplitKind> improved_cluster{SplitKind::kImprovedCluster};
    //std::list<SplitKind> standard{SplitKind::kStandard};
    //std::list<SplitKind> standard_plain_cluster{SplitKind::kStandard,
    //    SplitKind::kPlainCluster};
    //std::list<SplitKind> standard_improved_cluster{SplitKind::kStandard,
    //    SplitKind::kImprovedCluster};

    splits.push_back(&plain_cluster);
    //splits.push_back(&improved_cluster);
    //splits.push_back(&standard);
    //splits.push_back(&standard_plain_cluster);
    //splits.push_back(&standard_improved_cluster);

    std::list<int32_t> windows{0};

    std::list<Distance*> distances;

    int p_max = 2;
    // List containing all the local distances
    LpLocalDistance** local_distances = new LpLocalDistance*[p_max];
    for (int p = 1; p <= p_max; ++p) {
        local_distances[p - 1] = new LpLocalDistance(p);
    }
    for (int p = 1; p <= p_max; ++p) {
        LpLocalDistance* lp_local = local_distances[p - 1];
        for (auto window: windows) {
            DtwValueDistance* distance = new DtwValueDistance(
                    instances.vector_size(),
                    distance_multi_threads);
            distance->SetWindow(window);
            distance->SetLocalDistance(lp_local);
            distances.push_back(distance);
        }
    }

    EvaluationConfiguration eval_conf(g, distance_multi_threads);
    DistanceComparaison comparaison(&directories_conf, &instances, 
            &eval_conf, distances,
            splits);
    comparaison.CompareDistances();

    DistanceComparaisonReport report(&comparaison, &eval_conf,
            &directories_conf);
    report.GenerateCsv();

    // Free allocated memory
    for (auto distance: distances) {
        delete distance;
    }
    for (int p = 1; p <= p_max; ++p) {
        delete(local_distances[p - 1]);
        local_distances[p - 1] = NULL;
    }
    delete[](local_distances);

    return 0;
}
