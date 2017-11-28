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

void print_result(char *result, uint32_t msg_size)
{
	for(uint32_t i=0; i<msg_size; i++) {
		log_msg(LOG_DEBUG, "[%d] = %d", i, result[i]);
	}
}

void sum(struct mpi_parameters *mpi_parameters,
	char *result, char *array)
{
	for(uint32_t i=0; i<mpi_parameters->msg_size; i++) {
		result[i] += array[i];
	}
}

void gen_random_stream(char *stream, uint32_t size)
{
	log_msg(LOG_DEBUG, "Generating random bytestream of size %lu", size);
	for(uint32_t i=0; i<size; i++) {
		stream[i] = rand();
	}
	log_msg(LOG_TRACE, "Generated : [ %s ]", stream);
}
