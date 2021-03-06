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


#ifndef CYBERSPACE_MINIGAME_SERVER_HEURISTIC
#define CYBERSPACE_MINIGAME_SERVER_HEURISTIC

#include <stdio.h>
#include <stdlib.h>
//#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include "cyberspace_types.h"
#include "cyberspace_constants.h"
#include "cyberspace_structs.h"
#include "cyberspace_server_global.h"
#include "cyberspace_user_connection.h"


void cyberspace_minigame_heuristic_message2server(cyberspace_user_connection_data* data);
void cyberspace_minigame_heuristic_init(cyberspace_user_connection_data* data);
void cyberspace_minigame_heuristic_frame(cyberspace_user_connection_data* data, int milliseconds);
void cyberspace_minigame_heuristic_newpiece(cyberspace_user_connection_data* data);
bool cyberspace_minigame_server_heuristic_ismorphing(cyberspace_user_connection_data* data);
void cyberspace_minigame_server_heuristic_morph(cyberspace_user_connection_data* data, int x, int y);
void cyberspace_minigame_server_heuristic_morph_rec(cyberspace_user_connection_data* data, int x, int y, int icon_id);
void cyberspace_minigame_server_heuristic_gravity(cyberspace_user_connection_data* data);

#endif /* CYBERSPACE_MINIGAME_SERVER_HEURISTIC */

