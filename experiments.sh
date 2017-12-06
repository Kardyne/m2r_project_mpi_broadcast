#!/bin/bash

readonly c_default="\e[39m"
readonly c_red="\e[31m"
readonly c_green="\e[32m"

echo "Creating data directory"
mkdir -p ./data

total_time=0
for topology in ring grid2d; do
    topology_time=0
    # 'latency(us) bandwidth(Mbps)'
    for network in "30000 800" "1000 25600"; do
        latency=$(echo $network | cut -f1 -d " ")
        bandwidth=$(echo $network | cut -f2 -d " ")
        for proc_count in 16 256 625; do
            grid_arg=""
            if [[ "$topology" == "grid2d" ]]; then
                grid_size=$(echo "sqrt($proc_count)" | bc)
                grid_arg="-w $grid_size -h $grid_size"
            fi
            for exp in "4 7" "10 4"; do
                base=$(echo $exp | cut -f1 -d " ")
                max_exp=$(echo $exp | cut -f2 -d " ")
                for exponent in $(seq 1 $max_exp); do
                    msg_size=$(echo "$base^$exponent" | bc)
                    for default_arg in "" -d; do
                        cmd="./run.sh $topology $proc_count -l $latency -b $bandwidth $default_arg $grid_arg -- -n $msg_size"
                        sec=$(/usr/bin/time -f "%e" bash -c "$cmd > /dev/null 2>&1" 2>&1)
                        if [ $? -eq 0 ]; then
                            echo -e "${c_green}SUCCESS${c_default} in ${sec}s: ${cmd}"
                        else
                            echo -e "${c_red}FAILURE${c_default} in ${sec}s: ${cmd}"
                            exit 1
                        fi
                        topology_time=$(echo "$topology_time + $sec" | bc)
                        pj_dump -z "./smpi_simgrid.trace" \
                            > "./data/smpi_simgrid_${topology}_${latency}_${bandwidth}_${proc_count}_${msg_size}${default_arg}.csv"
                    done
                done
            done
        done
    done
    echo "$topology run took ${topology_time}s"
    total_time=$(echo "$total_time + $topology_time" | bc)
done
echo "Full run took ${total_time}s"
