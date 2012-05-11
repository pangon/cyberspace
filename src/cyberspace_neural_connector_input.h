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


#ifndef CYBERSPACE_NEURAL_CONNECTOR_INPUT
#define CYBERSPACE_NEURAL_CONNECTOR_INPUT

#include <stdio.h>
#include <stdlib.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <sys/resource.h>
//#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include "cyberspace_types.h"
#include "cyberspace_constants.h"
#include "cyberspace_client_global.h"


void* cyberspace_neural_connector_input_tf(void* nothing);
int cyberspace_neural_connector_input_tf_sdl(void* nothing);

#endif /* CYBERSPACE_NEURAL_CONNECTOR_INPUT */
