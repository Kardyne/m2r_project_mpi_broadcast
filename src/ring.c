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
#include "log.h"
#include <mpi.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

double timeval_diff_s(struct timeval *tv1, struct timeval *tv2)
{
	return (double) (tv2->tv_usec - tv1->tv_usec) / 1000000 +
		(double) (tv2->tv_sec - tv1->tv_sec);
}

void ring_broadcast(uint32_t msg_size)
{
	if(rank<0)
		return;
	if(!rank)
		MPI_LOG(LOG_INFO, "Message size: %d", msg_size);
	struct timeval tv1, tv2;
	char *sendbuf = malloc(msg_size);
	gen_random_stream(sendbuf, msg_size);
	char *recvbuf = malloc(msg_size);
	for(int32_t i=0; i<size; i++) {
		gettimeofday(&tv1, NULL);
		MPI_Sendrecv(sendbuf, msg_size, MPI_CHAR, (rank+1)%size, 0,
			recvbuf, msg_size, MPI_CHAR, (rank+size-1)%size, 0,
			MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		gettimeofday(&tv2, NULL);
		printf("%d, %d, %f\n", rank, i, timeval_diff_s(&tv1, &tv2));
		char *temp = sendbuf;
		sendbuf = recvbuf;
		recvbuf = temp;
	}
	free(sendbuf);
	free(recvbuf);
}