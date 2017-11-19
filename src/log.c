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
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdint.h>

uint8_t runtime_log_level = COMPILE_LOG_LEVEL;

void log_msg(enum log_level level, const char *fmt, ...)
{
	if (level < runtime_log_level)
		return;
	time_t t = time(NULL);
	struct tm *lt = localtime(&t);
	va_list args;
	char buf[32];
	buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", lt)] = '\0';
	fprintf(stderr, "[%s] %s%-7s" RESET_ATTR ": ",
		buf, level_colors[level], level_names[level]);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
}