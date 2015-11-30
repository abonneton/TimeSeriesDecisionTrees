dir=`pwd`
data_dir="${dir}/data/"
plot_dir="${data_dir}/plots/"

if [ ! -d $data_dir ]; then
    mkdir $data_dir
fi
if [ ! -d $plot_dir ]; then
    mkdir $plot_dir
fi
python tools/generate_increasing_decreasing_data.py $data_dir $plot_dir 
