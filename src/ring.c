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

void ring_allreduce(struct mpi_parameters *mpi_parameters, operation *op)
{
	char *sendbuf = malloc(mpi_parameters->msg_size);
	gen_random_stream(mpi_parameters, sendbuf);
	char *recvbuf = malloc(mpi_parameters->msg_size);
	char *result = calloc(mpi_parameters->msg_size, sizeof(char));
	for(int32_t i=0; i<mpi_parameters->p_count; i++) {
		if(!mpi_parameters->p_rank)
			log_msg(LOG_DEBUG, "All reduce step %d", i);
		MPI_Sendrecv(sendbuf, mpi_parameters->msg_size, MPI_CHAR,
			(mpi_parameters->p_rank+1)%mpi_parameters->p_count, 0,
			recvbuf, mpi_parameters->msg_size, MPI_CHAR,
			(mpi_parameters->p_rank+mpi_parameters->p_count-1)
				%mpi_parameters->p_count, 0,
			MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		op(mpi_parameters, result, sendbuf);
		char *temp = sendbuf;
		sendbuf = recvbuf;
		recvbuf = temp;
	}
	print_array(mpi_parameters, result);
	free(sendbuf);
	free(recvbuf);	
}