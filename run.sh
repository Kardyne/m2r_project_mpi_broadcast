#!/bin/sh

python ./tools/generate_xml_ring_and_hostfile.py $1 ./hostfiles/ ./platforms/

smpirun -np $1 -platform platforms/"$2"_"$1".xml -hostfile hostfiles/hostfile_"$1".txt ./mpi_allreduce $2