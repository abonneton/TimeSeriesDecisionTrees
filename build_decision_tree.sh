dir=`pwd`
data_dir="${dir}/data/"
output_dir="${dir}/output/"

input_file=$1

if [ ! -d $output_dir ]; then
    mkdir $output_dir
fi
./build/tools/build_decision_tree $output_dir $data_dir $input_file
