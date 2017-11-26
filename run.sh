#!/bin/sh

process_count=$1
topology=$2
shift; shift

python ./tools/generate_xml_ring_and_hostfile.py $process_count ./hostfiles/ ./platforms/

smpirun -np $process_count -platform platforms/"${topology}_${process_count}".xml -hostfile hostfiles/hostfile_"$process_count".txt ./mpi_allreduce $topology $@