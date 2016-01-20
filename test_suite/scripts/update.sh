#!/bin/bash

cur_dir=${PWD}
scrip_dir=${PWD}/scripts/
# find and list all the deepest(leaf) directories.
deepest_dir_array=( $(find ./test_cases/ -type d -links 2 ) )

for element in $(seq 0 $((${#deepest_dir_array[@]} - 1)))
do
    cd ${cur_dir}
    cp ./scripts/update_status.py ${cur_dir}/"${deepest_dir_array[$element]}"
    cp ./scripts/update_previous_value.py ${cur_dir}/"${deepest_dir_array[$element]}"
    # cp ./scripts/Makefile ${cur_dir}/"${deepest_dir_array[$element]}"
    cd ${cur_dir}/"${deepest_dir_array[$element]}"
    # make >/dev/null
    python update_status.py #${cur_dir}/"scripts"

    # if file "update_previous_value_more" exist, this means that this case is for 
    # multiple stages testing. Therefore, we need to update the multiple values.
    if [ -f $PWD/update_previous_value_more.py ] 
        then
            cp ${scrip_dir}/update_previous_value_more.py ${cur_dir}/"${deepest_dir_array[$element]}"
            python update_previous_value_more.py #${cur_dir}/"scripts"
            # rm update_previous_value_more.py
    else
        python update_previous_value.py #${cur_dir}/"scripts"
        rm update_previous_value.py
    fi

    rm update_status.py #${cur_dir}/"scripts"
done
