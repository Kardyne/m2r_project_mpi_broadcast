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

#include <argp.h>
#include <stdlib.h>
#include "argparse.h"

const char *argp_program_version =
	"m2r_project_mpi_broadcast 0.1";

/* Program documentation. */
static char doc[] =
	"Université Grenoble Alpes parallel systems course project\n\
This is a student project whose only aim is to better understand the MPI \
algorithms.\n\n\
TOPOLOGY may take any of these values:\n\
    ring\n\
    grid2d";

/* A description of the arguments we accept. */
static char args_doc[] = "TOPOLOGY";

/* The options we understand. */
static struct argp_option options[] = {
	{0,          'n', "SIZE",  0,
		"Broadcasts a message of SIZE bytes (default 0)", 0},
	{"seed",     's', "SEED",  0, "Seed (default 0)", 0},
	{"height",   'h', "HEIGHT",0, "[Grid - mandatory] Height", 0},
	{"width",    'w', "WIDTH", 0, "[Grid - mandatory] Width", 0},
	{"verbose",  'v', 0,       0, "Increase verbosity", 0},
	{"quiet",    'q', 0,       0, "Decrease verbosity", 0},
	{0}
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	/* Get the input argument from argp_parse, which we
   	know is a pointer to our arguments structure. */
	struct arguments *arguments = state->input;

	switch (key) {
	case 'n':
		arguments->msg_size = atol(arg);
		break;
	case 's':
		arguments->seed = atol(arg);
		break;
	case 'h':
		arguments->height = atol(arg);
		break;
	case 'w':
		arguments->width = atol(arg);
		break;
  	case 'q':
    		arguments->quiet += 1;
    		break;
  	case 'v':
    		arguments->verbose += 1;
    		break;
  	case ARGP_KEY_NO_ARGS:
		arguments->topology = "default";
		break;    
  	case ARGP_KEY_ARG:
		/* Here we know that state->arg_num == 0, since we
		force argument parsing to end before any more arguments can
		get here. */
		arguments->topology = arg;

		/* In addition, by setting state->next to the end
		of the arguments, we can force argp to stop parsing here and
		return. */
		state->next = state->argc;

		break;
  	default:
    		return ARGP_ERR_UNKNOWN;
  	}
	return 0;
}

/* Our argp parser. */
static struct argp argp = {options, parse_opt, args_doc, doc, 0, 0, 0};

int argparse(int argc, char **argv, struct arguments *arguments)
{
	/* Default values. */
	arguments->quiet = 0;
	arguments->verbose = 0;
	arguments->msg_size = 0;
	arguments->seed = 0;
	arguments->height = -1;
	arguments->width = -1;

	/* Parse our arguments; every option seen by parse_opt will be
   	reflected in arguments. */
	return argp_parse(&argp, argc, argv, 0, 0, arguments);
}