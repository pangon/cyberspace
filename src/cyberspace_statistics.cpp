/*
 * cyberspace is 3D first person mmorpg game engine, with 2D minigame
 * plugins, initially developed to be used on LAN as an additional
 * game element for a cyberpunk LARPG.
 *
 * Copyright (C) 2008-2012 Claudio Felicioli
 * mail: pangon@gmail.com
 *
 * cyberspace is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


#include "cyberspace_statistics.h"


int cyberspace_statistics_tf_sdl(void* nothing) {
	cyberspace_statistics_tf(nothing);
	return(0);
	}

void* cyberspace_statistics_tf(void* nothing) {
	printf("statistics: STARTED\n");

	struct timespec tosleep;
	struct timespec tosleepagain;

	while(true) {
		tosleep.tv_sec=CYBERSPACE_STATISTICS_FREQUENCY;
		tosleep.tv_nsec=0;
		while(nanosleep(&tosleep, &tosleepagain)!=0) {
			tosleep=tosleepagain;
			}
		printf("statistics: cyberspace_user_connected=%d\n", cyberspace_user_connected);
		}

	printf("statistics: ENDING\n");
//	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
	}

