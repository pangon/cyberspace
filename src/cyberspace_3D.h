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


#ifndef CYBERSPACE_3D
#define CYBERSPACE_3D

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>

#include "cyberspace_types.h"
#include "cyberspace_constants.h"
#include "cyberspace_structs.h"
#include "cyberspace_server_global.h"
#include "cyberspace_user_connection.h"
#include "cyberspace_world_loader.h"
#include "cyberspace_minigame_server_testing.h"
#include "cyberspace_minigame_server_heuristic.h"
#include "cyberspace_minigame_server_fast.h"
#include "cyberspace_minigame_server_patternmatching.h"
#include "cyberspace_nonvolatile.h"


void cyberspace_3D_message2server(cyberspace_user_connection_data* data);
void* cyberspace_physical_engine_tf(void* nothing);
int cyberspace_physical_engine_tf_sdl(void* nothing);
double cyberspace_3D_collision_line_circle(cyberspace_world_point* p_i, double V_x, double V_z, cyberspace_world_point* p_a, double alpha, double* tangent, double* reduction);
double cyberspace_3D_collision_line_segment(cyberspace_world_point* p_i, double V_x, double V_z, cyberspace_world_point* p_a, cyberspace_world_point* p_b, double alpha, double* tangent, double* reduction);

#endif /* CYBERSPACE_3D */

