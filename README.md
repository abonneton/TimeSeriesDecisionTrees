# Time Series Decision Trees

A decision tree is a tree-structured classification model. 
Usual implementations take numeric and categorical features as input.
Here, we propose an implementation based on time series features.

The implementation is mostly based on the description of Time Series Decision Trees (TSDT) in [1].
See [2] for an overview of the implementation.

### Main Features

* Error-complexity pruning method [3]
* Splits : standard, cluster (plain or improved) [1]
* DTW distance [4, 5]

### Limitations

* Only binary decision trees
* Supports only time series features (not categorical or numeric features)

### References
[1]  Yamada, Yuu, et al. "Decision-tree induction from time-series data based on a standard-example split test." ICML. Vol. 3. 2003.

[2]  Bonneton, Anael, et al. "DGA Bot Detection with Time Series Decision Trees." BADGERS 2015.

[3] Breiman, Leo, et al. Classification and regression trees. CRC press, 1984.

[4] Sakoe, Hiroaki, and Seibi Chiba. "Dynamic programming algorithm optimization for spoken word recognition." Acoustics, Speech and Signal Processing, IEEE Transactions on 26.1 (1978): 43-49.

[5] Giorgino, Toni. "Computing and visualizing dynamic time warping alignments in R: the dtw package." Journal of statistical Software 31.7 (2009): 1-24.
 
# Building the project

    mkdir build
    cd build
    cmake ..
    make

### Generating a time series dataset
    ./generate_increasing_decreasing_data.sh

It generates a dataset with Increasing and Decreasing time series stored in 
`data/increase-decrease.txt`.
A plot representing each time series is stored in the directory `data/plots`.

### Building a decision tree
    ./build_decision_tree.sh increase-decrease.txt

It builds a TSDT from the dataset `data/increase-decrease.txt` and stores 
a report in the directory `output`. 

See `output/decision_tree_report.pdf` for an example of report.

### Comparing several decision trees
    ./compare_decision_trees.sh increase-decrease.txt

It builds several TSDTs with different parameters from the dataset 
`data/increase-decrease.txt` and stores a CSV report comparing the different 
settings in the directory `output`.

See `output/comparison.csv` for an example of CSV comparison report.
