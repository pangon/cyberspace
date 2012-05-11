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


#include "cyberspace_server_global.h"


int cyberspace_user_connected;
cyberspace_user_connection_data cyberspace_user_connection_datas[CYBERSPACE_CONNECTIONS_MAX];
//pthread_t cyberspace_user_connection_threads[CYBERSPACE_CONNECTIONS_MAX];
SDL_Thread* cyberspace_user_connection_threads[CYBERSPACE_CONNECTIONS_MAX];
cyberspace_world_world* cyberspace_universe;
//pthread_mutex_t* cyberspace_mutex_universe;
SDL_mutex** cyberspace_mutex_universe_sdl;
cyberspace_server_nonvolatile_data cyberspace_server_data;
SDL_mutex* cyberspace_mutex_server_data_sdl;

