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


#ifndef CYBERSPACE_USER_CONNECTION
#define CYBERSPACE_USER_CONNECTION

#include <stdio.h>
#include <stdlib.h>
//#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <SDL_net.h>

#include "cyberspace_constants.h"
#include "cyberspace_structs.h"
#include "cyberspace_structs.h"
#include "cyberspace_login.h"
#include "cyberspace_3D.h"
#include "cyberspace_programming.h"
#include "cyberspace_minigame_server_testing.h"
#include "cyberspace_minigame_server_heuristic.h"
#include "cyberspace_minigame_server_fast.h"
#include "cyberspace_minigame_server_patternmatching.h"

void* cyberspace_user_connection_tf(void* data_void);
int cyberspace_user_connection_tf_sdl(void* data_void);
void cyberspace_user_connection_output(cyberspace_user_connection_data* user, cyberspace_server_command* command);

#endif /* CYBERSPACE_USER_CONNECTION */

