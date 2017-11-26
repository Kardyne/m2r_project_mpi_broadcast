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

int32_t rank = -1, size = -1;

void print_result(char *result, uint32_t msg_size)
{
	for(uint32_t i=0; i<msg_size; i++) {
		MPI_LOG(LOG_DEBUG, "[%d] = %d", i, result[i]);
	}
}

double timeval_diff_s(struct timeval *tv1, struct timeval *tv2)
{
	return (double) (tv2->tv_usec - tv1->tv_usec) / 1000000 +
		(double) (tv2->tv_sec - tv1->tv_sec);
}

void sum(char *result, char *array, uint32_t msg_size)
{
	for(uint32_t i=0; i<msg_size; i++) {
		result[i] += array[i];
	}
}

void allreduce(allreduce_sendrecv *sendrecv, operation *op,
	uint32_t msg_size)
{
	if(rank<0)
		return;
	if(!rank)
		MPI_LOG(LOG_INFO, "Message size: %d", msg_size);
	struct timeval tv1, tv2;
	char *sendbuf = malloc(msg_size);
	gen_random_stream(sendbuf, msg_size);
	char *recvbuf = malloc(msg_size);
	char *result = calloc(msg_size, sizeof(char));
	for(int32_t i=0; i<size; i++) {
		gettimeofday(&tv1, NULL);
		sendrecv(sendbuf, msg_size, recvbuf, msg_size);
		gettimeofday(&tv2, NULL);
		printf("%d, %d, %f\n", rank, i, timeval_diff_s(&tv1, &tv2));
		op(result, sendbuf, msg_size);
		char *temp = sendbuf;
		sendbuf = recvbuf;
		recvbuf = temp;
	}
	print_result(result, msg_size);
	free(sendbuf);
	free(recvbuf);
}

void mpi_common_init(int *argc, char ***argv)
{
	if(rank > -1 || size > -1)
		return;
	MPI_Init(argc, argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
}

void gen_random_stream(char *stream, uint32_t size)
{
	MPI_LOG(LOG_DEBUG, "Generating random bytestream of size %lu", size);
	for(uint32_t i=0; i<size; i++) {
		stream[i] = rand();
	}
	MPI_LOG(LOG_TRACE, "Generated : [ %s ]", stream);
}
