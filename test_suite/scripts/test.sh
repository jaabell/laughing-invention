#!/bin/bash

cur_dir=${PWD}
scrip_dir=${PWD}/scripts/
essi_dir="$(dirname "$cur_dir")"


cd ${cur_dir}
# find and list all the deepest(leaf) directories.
deepest_dir_array=( $(find ./test_cases/ -type d -links 2 ) )

# loop over the leaf directories.
for element in $(seq 0 $((${#deepest_dir_array[@]} - 1)))
do
    cd ${cur_dir}
    cp ./scripts/status.py ${cur_dir}/"${deepest_dir_array[$element]}"
    cp ./scripts/report.py ${cur_dir}/"${deepest_dir_array[$element]}"
    cp ./scripts/Makefile ${cur_dir}/"${deepest_dir_array[$element]}"
    cd ${cur_dir}/"${deepest_dir_array[$element]}"
    
    # show the working folder first.
    # even the model has break down, we also know the folder location.
    python status.py 
    
    # make >/dev/null
    # change to show "essi" command explicitly:
    ${essi_dir}/bin/essi -f main.fei >/dev/null  
    # If you have "essi" in your system PATH, 
    # you can also use the command line below:
    # essi -f main.fei >/dev/null

    # if file "reportmore.py" exist, this means that this test case is for 
    # multiple stages testing. Therefore, we need to report more values.
    if [ -f $PWD/reportmore.py ] 
        then
            cp ${scrip_dir}/reportmore.py ${cur_dir}/"${deepest_dir_array[$element]}"
            python reportmore.py
    else
    	python report.py #${cur_dir}/"scripts"
    fi
done
