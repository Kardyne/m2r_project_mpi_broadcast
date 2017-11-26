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

#ifndef LOG_H
#define LOG_H 1

#include <stdint.h>

#define ESCAPE_STR				"\x1b["
#define RESET_ATTR		ESCAPE_STR	"0m"
#define COLOR_DEFAULT		ESCAPE_STR	"39m"
#define COLOR_BLACK		ESCAPE_STR	"30m"
#define COLOR_RED		ESCAPE_STR	"31m"
#define COLOR_GREEN		ESCAPE_STR	"32m"
#define COLOR_YELLOW		ESCAPE_STR	"33m"
#define COLOR_BLUE		ESCAPE_STR	"34m"
#define COLOR_MAGENTA		ESCAPE_STR	"35m"
#define COLOR_CYAN		ESCAPE_STR	"36m"
#define COLOR_LIGHT_GRAY	ESCAPE_STR	"37m"
#define COLOR_DARK_GRAY		ESCAPE_STR	"90m"
#define COLOR_LIGHT_RED		ESCAPE_STR	"91m"
#define COLOR_LIGHT_GREEN	ESCAPE_STR	"92m"
#define COLOR_LIGHT_YELLOW	ESCAPE_STR	"93m"
#define COLOR_LIGHT_BLUE	ESCAPE_STR	"94m"
#define COLOR_LIGHT_MAGENTA	ESCAPE_STR	"95m"
#define COLOR_LIGHT_CYAN	ESCAPE_STR	"96m"
#define COLOR_LIGHT_WHITE	ESCAPE_STR	"97m"

enum log_level {
	LOG_TRACE = 0,
	LOG_DEBUG,
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR,
	LOG_FATAL
};

#define COMPILE_LOG_LEVEL	LOG_INFO

extern uint8_t runtime_log_level;

extern const char *level_names[];

extern const char *level_colors[];

void log_msg(enum log_level level, const char *fmt, ...);

#endif