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

#ifndef ARGPARSE_H
#define ARGPARSE_H 1

#include <stdint.h>

struct arguments {
	char *topology;
	uint32_t msg_size;
	uint32_t seed;
	int32_t height;
	int32_t width;
	uint8_t quiet, verbose;
};

int argparse(int argc, char **argv, struct arguments *arguments);

#endif