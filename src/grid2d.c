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

#include "mpi_common.h"
#include <mpi.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void grid2d_sendrecv(struct mpi_parameters *mpi_parameters, operation *op,
	char* sendbuf, char* recvbuf, char* result,
	int32_t loop_bound, uint32_t next_rank, uint32_t prev_rank)
{
	for(int32_t i=0; i<loop_bound-1; i++) {
		MPI_Sendrecv(sendbuf, mpi_parameters->msg_size, MPI_CHAR,
			next_rank, 0,
			recvbuf, mpi_parameters->msg_size, MPI_CHAR,
			prev_rank, 0,
			MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		op(mpi_parameters, result, recvbuf);
		char *temp = sendbuf;
		sendbuf = recvbuf;
		recvbuf = temp;
	}
}

void grid2d_allreduce(struct mpi_parameters *mpi_parameters, operation *op)
{
	char *sendbuf = malloc(mpi_parameters->msg_size);
	gen_random_stream(mpi_parameters, sendbuf);
	char *recvbuf = malloc(mpi_parameters->msg_size);
	char *result = calloc(mpi_parameters->msg_size, sizeof(char));
	memcpy(result, sendbuf, mpi_parameters->msg_size);
	uint32_t cur_row = mpi_parameters->p_rank / mpi_parameters->width;
	uint32_t cur_col = mpi_parameters->p_rank % mpi_parameters->width;
	uint32_t next_rank =
		(cur_col+1)%mpi_parameters->width
		+ cur_row*mpi_parameters->width;
	uint32_t prev_rank =
		(cur_col+mpi_parameters->width-1)%mpi_parameters->width
		+ cur_row*mpi_parameters->width;
	grid2d_sendrecv(mpi_parameters, op, sendbuf, recvbuf, result,
		mpi_parameters->width, next_rank, prev_rank);
	memcpy(sendbuf, result, mpi_parameters->msg_size);
	next_rank = cur_col
		+ ((cur_row+1)%mpi_parameters->height)*mpi_parameters->width;
	prev_rank = cur_col
		+ ((cur_row+mpi_parameters->height-1)%mpi_parameters->height)*mpi_parameters->width;
	grid2d_sendrecv(mpi_parameters, op, sendbuf, recvbuf, result,
		mpi_parameters->height, next_rank, prev_rank);
	print_array(mpi_parameters, result);
	free(sendbuf);
	free(recvbuf);	
}