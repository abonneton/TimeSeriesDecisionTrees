#include <iostream>
#include <list>
#include <string>

#include "directories_configuration.h"
#include "distances_matrix.h"
#include "distances/dtw_value_distance.h"
#include "distances/lp_local_distance.h"
#include "impurity/entropy_impurity.h"
#include "impurity/impurity.h"
#include "instances.h"
#include "pruning.h"
#include "report/pruning_report.h"
#include "split/split_kind.h"
#include "tree_configuration.h"

int main(int argc, char* argv[]) {

    if (argc != 4) {
        std::cout <<"./build_decision_tree \"output_dir\" \"data_dir\" \"input_filename\"" <<std::endl;
        return 1;
    }
    std::string output_path     = std::string(argv[1]);
    std::string data_path       = std::string(argv[2]);
    std::string input_file_name = std::string(argv[3]);
    
    DirectoriesConfiguration directories_conf(output_path,
            data_path, input_file_name);
    Instances instances(directories_conf);

    int32_t distance_multi_threads = 1;

    std::list<SplitKind> split{SplitKind::kPlainCluster};
    //std::list<SplitKind> split{SplitKind::kStandard};
    //std::list<SplitKind> split{SplitKind::kImprovedCluster};
    //std::list<SplitKind> split{SplitKind::kStandard,
    //    SplitKind::kImprovedCluster};

    Impurity* impurity = new EntropyImpurity();

    LpLocalDistance* lp_local = new LpLocalDistance(2);
    double window = 0;
    DtwValueDistance* distance = new DtwValueDistance(instances.vector_size(),
            distance_multi_threads);
    distance->SetWindow(window);
    distance->SetLocalDistance(lp_local);

    TreeConfiguration tree_conf(&split, impurity);
    DistancesMatrix matrix(distance, distance_multi_threads);
    
    double execution_time = 0;
    matrix.Compute(instances, execution_time);
    
    tree_conf.SetDistancesMatrix(&matrix);

    Pruning pruning(&tree_conf, instances);
    pruning.GenerateFullDecisionTree();
    pruning.GeneratePrunedDecisionTrees();
    pruning.SelectBestDecisionTree();

    PruningReport report(&instances, &pruning, &directories_conf);
    report.SetAllPrunedTrees(true);
    report.GenerateReport();

    // Free allocated memory
    delete lp_local;
    delete impurity;
    delete distance;

    return 0;
}
