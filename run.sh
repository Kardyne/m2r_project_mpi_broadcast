#!/bin/bash

check_dependency() { command -v $1 >/dev/null 2>&1 || { printf "$1 is required but it is not installed. Aborting.\n"; exit 1; } }
check_dependencies() {
	for DEP in "$@"; do
		check_dependency $DEP
	done
}

is_integer() {
	local RE='^[0-9]+$'
	[[ $1 =~ $RE ]] || return 1
}

getopt_test() {
	getopt --test > /dev/null
	if [[ $? -ne 4 ]]; then
		printf "'getopt --test' failed in this environment.\n"
		exit 1
	fi
	return 0
}

getopt_parse() {
	getopt_test
	SHORT=w:h:dl:b:
	LONG=np:,width:,height:,help,latency:,bandwidth
	getopt --options $SHORT --longoptions $LONG --name "$0" -- "$@"
	if [[ $? -ne 0 ]]; then
		printf "getopt could not pass the arguments.\n"
		exit 2
	fi
}

parse_arguments() {
	help_opt=0
	default_opt=0
	eval set -- "$(getopt_parse $@)"
	while true; do
		case "$1" in
			--np)
                if ! is_integer "$2"; then
					printf "$2 is not a number\n"
					exit 1
				fi
				process_count="$2"
				shift 2
				;;
            -w|--width)
                if ! is_integer "$2"; then
					printf "$2 is not a number\n"
					exit 1
				fi
				width="$2"
				shift 2
				;;
            -h|--height)
                if ! is_integer "$2"; then
					printf "$2 is not a number\n"
					exit 1
				fi
				height="$2"
				shift 2
				;;
			-l|--latency)
				if ! is_integer "$2"; then
					printf "$2 is not a number\n"
					exit 1
				fi
				latency="$2"
				shift 2
				;;
			-b|--bandwidth)
				if ! is_integer "$2"; then
					printf "$2 is not a number\n"
					exit 1
				fi
				bandwidth="$2"
				shift 2
				;;
			-d)
				default_opt=1
				shift
				;;
			--help)
				help_opt=1
				shift
				;;
			--)
                if ! is_integer "$3"; then
					printf "$3 is not a number\n"
					exit 1
				fi
                topology="$2"
				process_count="$3"
				shift 3
				bin_args="$@"
				break
				;;
			*)
				printf "Unhandled parsing error\n"
				exit 3
				;;
		esac
	done
	check_arguments
}

check_arguments() {
	[[ $help_opt -eq 1 ]] && display_usage && exit 0
	bin_topology="$topology"
	[[ $default_opt -eq 1 ]] && bin_topology=""
    [[ -z "$topology" ]] && printf "Topology is mandatory.\n" && exit 1
	[[ -z "$process_count" ]] && printf "Process count is mandatory.\n" && exit 1
    if [[ "$topology" == "grid2d" ]]; then
        [[ -z "$width" ]] && printf "Width is mandatory for grid topology.\n" && exit 1
        [[ -z "$height" ]] && printf "Height is mandatory for grid topology.\n" && exit 1
    fi
	[[ -z "$latency" ]] && printf "Latency is mandatory.\n" && exit 1
	[[ -z "$bandwidth" ]] && printf "Bandwidth is mandatory.\n" && exit 1
}

print_usage() {
	printf "Options:\n"
	printf " --np, <count>\tNumber of processors (mandatory)\n"
    printf " -w, --width, <num>\tWidth (only for grid)\n"
    printf " -h, --height, <num>\tHeight (only for grid)\n"
	printf " -l, --latency, <num>\tLatency\n"
	printf " -b, --bandwidth, <num>\tBandwidth\n"
	printf " -d \tUse the default MPI_Allreduce call\n"
    printf " -- \tAll options after this will be passed to the mpi_allreduce program\n"
	printf " --help \tPrint this message\n"
}

display_usage() {
	printf "Usage: $(basename $0) topology process_count\n"
	printf "$(print_usage)" | column -t -s $'\t'
}

main() {
    check_dependency python
    python_bin=python
    if $python_bin -V | grep -q "Python 3"; then
        check_dependency python2
        python_bin=python2
    fi
	check_dependencies getopt printf smpirun column
	parse_arguments $@

    case "$topology" in
    "ring")
        $python_bin ./tools/generate_xml_ring_and_hostfile.py \
			$process_count ./hostfiles/ ./platforms/ \
			"$bandwidth" "$latency"
        smpirun -trace -np $process_count \
            -platform "platforms/${topology}_${process_count}.xml" \
            -hostfile "hostfiles/hostfile_$process_count.txt" \
            ./mpi_allreduce $bin_topology -w $width -h $height $bin_args
        ;;
    "grid2d")
        $python_bin ./tools/generate_xml_grid_and_hostfile.py \
			$height $width ./hostfiles/ ./platforms/ \
			"$bandwidth" "$latency"
        smpirun -trace -np $process_count \
            -platform "platforms/grid_${height}_${width}.xml" \
            -hostfile "hostfiles/grid_hostfile_${height},${width}.txt" \
            ./mpi_allreduce $bin_topology -w $width -h $height $bin_args
        ;;
    *)
        printf "Topology %s not implemented. Exiting.\n" "$topology"
        exit 1
    esac
}

main $@