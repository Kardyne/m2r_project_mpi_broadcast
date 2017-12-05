/******************************************************************************
* Copyright (C) 2017 Alexis Janon
* Copyright (C) 2017 Benjamin Roussillon
* 
* This code is part of master parallel systems course project at the
* Université Grenoble Alpes.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version. 
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef MPI_COMMON_H
#define MPI_COMMON_H 1

#include "log.h"
#include <stdint.h>
#include <sys/time.h>
#include <mpi.h>

struct mpi_parameters {
	int32_t p_rank;
	int32_t p_count;
	int32_t height;
	int32_t width;
	uint32_t msg_size;
};

typedef void operation(struct mpi_parameters *mpi_parameters,
	char* result, char* array);

void sum(struct mpi_parameters *mpi_parameters,
	char *result, char *array);

void gen_random_stream(struct mpi_parameters *mpi_parameters, char *stream);

void print_array(struct mpi_parameters *mpi_parameters, char *result);

void allreduce(struct mpi_parameters *mpi_parameters, MPI_Op op);

#endif
