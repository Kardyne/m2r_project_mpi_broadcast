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
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <mpi.h>

void update_log_level(struct arguments *arguments)
{
	int8_t log_change = arguments->verbose - arguments->quiet;
	int8_t new_log_level = runtime_log_level + log_change;
	if(new_log_level >= LOG_FATAL) {
		runtime_log_level = LOG_FATAL;
	} else if(new_log_level < LOG_TRACE) {
		runtime_log_level = LOG_TRACE;
	} else {
		runtime_log_level = new_log_level;
	}
}



int main(int argc, char **argv)
{
	struct arguments arguments;
	argparse(argc, argv, &arguments);
	update_log_level(&arguments);
	mpi_common_init(argc, argv);
	if (!rank)
		log_msg(LOG_INFO, "Topology = [%s]", arguments.topology);
	MPI_Finalize();
	exit(EXIT_SUCCESS);
}