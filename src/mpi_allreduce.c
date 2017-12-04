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

#include "argparse.h"
#include "log.h"
#include "mpi_common.h"
#include "ring.h"
#include "grid2d.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <mpi.h>

void update_log_level(struct arguments *arguments)
{
	int8_t log_change = arguments->quiet - arguments->verbose;
	int8_t new_log_level = runtime_log_level + log_change;
	if(new_log_level >= LOG_FATAL) {
		runtime_log_level = LOG_FATAL;
	} else if(new_log_level < LOG_TRACE) {
		runtime_log_level = LOG_TRACE;
	} else {
		runtime_log_level = new_log_level;
	}
}

void configure(struct arguments *arguments,
	struct mpi_parameters *mpi_parameters)
{
	update_log_level(arguments);
	mpi_parameters->msg_size = arguments->msg_size;
	mpi_parameters->height = arguments->height;
	mpi_parameters->width = arguments->width;
	if(!strcmp(arguments->topology, "grid2d")) {
		if(mpi_parameters->width < 0 || arguments->height < 0) {
			if(!mpi_parameters->p_rank)
				log_msg(LOG_FATAL, "Width and height should be positive. Width=%d, height=%d",
					mpi_parameters->width,
					mpi_parameters->height);
			exit(EXIT_FAILURE);
		}
		if(mpi_parameters->width * mpi_parameters->height != 
				mpi_parameters->p_count) {
			if(!mpi_parameters->p_rank)
				log_msg(LOG_FATAL, "Width*height!=processor count. Width*height=%d, processor count=%d",
				mpi_parameters->width*mpi_parameters->height,
				mpi_parameters->p_count);
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char **argv)
{
	struct mpi_parameters mpi_parameters = {0};
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_parameters.p_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &mpi_parameters.p_count);
	struct arguments arguments;
	argparse(argc, argv, &arguments);
	configure(&arguments, &mpi_parameters);
	srand(arguments.seed+mpi_parameters.p_rank);
	if(!mpi_parameters.p_rank) {
		log_msg(LOG_INFO, "Topology: [%s]", arguments.topology);
		log_msg(LOG_INFO, "Message size: %d", mpi_parameters.msg_size);
		log_msg(LOG_INFO, "Processor count: %d",
			mpi_parameters.p_count);
	}
	if(!strcmp(arguments.topology, "ring")) {
		ring_allreduce(&mpi_parameters, sum);
	} else if(!strcmp(arguments.topology, "grid2d")) {
		grid2d_allreduce(&mpi_parameters, sum);
	} else if(!mpi_parameters.p_rank) {
		log_msg(LOG_FATAL, "[%s] topology not implemented, exiting.",
			arguments.topology);
	}
	MPI_Finalize();
	return EXIT_SUCCESS;
}
