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


#include "cyberspace_minigame_server_patternmatching.h"



void cyberspace_minigame_patternmatching_init(cyberspace_user_connection_data* data) {
	}

void cyberspace_minigame_patternmatching_frame(cyberspace_user_connection_data* data, int milliseconds) {
	}

void cyberspace_minigame_patternmatching_message2server(cyberspace_user_connection_data* data) {
	if(data->phase!=CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING) {
		fprintf(stderr, "ERROR: data->phase!=CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING (%d).\n", data->phase);
		exit(EXIT_FAILURE);
		}
	}

