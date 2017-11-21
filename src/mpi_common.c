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

int rank = -1, size = -1;

void mpi_common_init(int argc, char **argv)
{
	if(rank > -1 || size > -1)
		return;
	MPI_Init(&argc, &argv);
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