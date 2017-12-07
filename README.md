# Université Grenoble Alpes parallel systems course project

This is a student project whose only aim is to better understand the MPI
algorithms.

This project aims to study different MPI_allreduce algorithms.
The first part consists in an analysis of the algorithm for
various network topologies, followed by an implementation and a testing
part which will be used to confirm or disprove the analysis.
Any discrepancy found will be investigated to find its source and
explain the differences encountered.

Usage:

Compile with make debug or make release
Use the run.sh script to execute an experiment.
Please refer to the script usage for more information (run.sh --help)
To run all designed experiments, use experiment.sh.


The generated data of all run tests can be found at:
https://github.com/Kardyne/m2r_project_mpi_broadcast_data
