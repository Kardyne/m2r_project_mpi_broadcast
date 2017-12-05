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

#include "log.h"
#include "mpi_common.h"
#include <mpi.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

void print_array(struct mpi_parameters *mpi_parameters, char *result)
{
	for(uint32_t i=0; i<mpi_parameters->msg_size; i++) {
		log_msg(LOG_TRACE, "(%02d) [%d] = %d",
			mpi_parameters->p_rank, i, result[i]);
	}
}

void sum(struct mpi_parameters *mpi_parameters,
	char *result, char *array)
{
	for(uint32_t i=0; i<mpi_parameters->msg_size; i++) {
		result[i] += array[i];
	}
}

void gen_random_stream(struct mpi_parameters *mpi_parameters, char *stream)
{
	log_msg(LOG_DEBUG, "(%02d) Generating random bytestream of size %lu",
		mpi_parameters->p_rank, mpi_parameters->msg_size);
	for(uint32_t i=0; i<mpi_parameters->msg_size; i++) {
		stream[i] = rand();
	}
	log_msg(LOG_TRACE, "(%02d) Generated : [ %s ]",
		mpi_parameters->p_rank, stream);
}

void allreduce(struct mpi_parameters *mpi_parameters, MPI_Op op)
{
	char *sendbuf = malloc(mpi_parameters->msg_size);
	gen_random_stream(mpi_parameters, sendbuf);
	char *recvbuf = malloc(mpi_parameters->msg_size);
	MPI_Allreduce(sendbuf, recvbuf, mpi_parameters->msg_size, MPI_CHAR,
			op, MPI_COMM_WORLD);
}
