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


#ifndef CYBERSPACE_CLIENT_GLOBAL
#define CYBERSPACE_CLIENT_GLOBAL

#include "cyberspace_types.h"
#include "cyberspace_constants.h"
#include "cyberspace_structs.h"
//#include "SDL/SDL.h"
//#include "SDL/SDL_opengl.h"
#include <sys/time.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_net.h>

typedef struct {
	cyberspace_world_material_point camera;
	int static_size;
	cyberspace_world_object static_obj[CYBERSPACE_WORLD_STATIC_OBJECTS_MAX];
	int dynamic_size;
	cyberspace_world_object dynamic_obj[CYBERSPACE_WORLD_DYNAMIC_OBJECTS_MAX];
//	int planes_size;
//	cyberspace_world_green_plane planes[CYBERSPACE_WORLD_GREEN_PLANES_MAX];
//	int objects_size;
//	cyberspace_world_object objects[CYBERSPACE_WORLD_OBJECTS_MAX];
	} cyberspace_GUI_world;

//extern int cyberspace_output_socket_fd;
//extern int cyberspace_input_socket_fd;
extern TCPsocket cyberspace_output_socket;
extern TCPsocket cyberspace_input_socket;
extern int cyberspace_to_read;
extern int cyberspace_read_message_size;
extern int cyberspace_read_total;
extern int cyberspace_read_partial;
extern int cyberspace_to_write;
extern int cyberspace_written_total;
extern int cyberspace_written_partial;
extern onebyte cyberspace_read_buffer[CYBERSPACE_USER_CONNECTION_READ_BUFFER];
extern onebyte cyberspace_write_buffer[CYBERSPACE_USER_CONNECTION_WRITE_BUFFER];
//extern pthread_t cyberspace_input_thread;
extern SDL_Thread* cyberspace_input_thread_sdl;
extern bool cyberspace_to_end;
extern cyberspace_user_command cyberspace_output_command;
//extern pthread_mutex_t cyberspace_mutex_phase;
extern SDL_mutex* cyberspace_mutex_phase_sdl;
extern int cyberspace_phase;
//extern pthread_t cyberspace_GUI_thread;
extern SDL_Thread* cyberspace_GUI_thread_sdl;
//extern pthread_t cyberspace_GUI_input_thread;
extern SDL_Thread* cyberspace_GUI_input_thread_sdl;
extern int cyberspace_GUI_screen_type;
extern SDL_Surface* cyberspace_GUI_screen;
extern SDL_Surface* cyberspace_GUI_screen_gl;
//extern pthread_mutex_t cyberspace_mutex_world;
extern SDL_mutex* cyberspace_mutex_world_sdl;
extern cyberspace_GUI_world cyberspace_world;
//extern double cyberspace_GUI_rotation_alpha;
//extern double cyberspace_GUI_traslation_ahead;
extern cyberspace_user_movement cyberspace_movement;
extern int frames;
extern cyberspace_user_personal_data cyberspace_personal_data;
extern cyberspace_user_programming cyberspace_programming;
extern cyberspace_world_selection cyberspace_obj_selected;
extern bool cyberspace_sreen_reload;
extern char* cyberspace_login_name;
extern char* cyberspace_login_password;
extern int cyberspace_GUI_input_focus;
extern char** cyberspace_chat;
extern char* cyberspace_chat_input;
extern char* cyberspace_server_message;
extern int cyberspace_server_message_fade;
extern char* cyberspace_fps;
extern char* cyberspace_selection_message;
extern int cyberspace_programming_subphase;
extern int cyberspace_screen_width;
extern int cyberspace_screen_height;
extern GLuint* cyberspace_textures;
extern void** cyberspace_textures_raw;
extern double cyberspace_clips_4x4_64_x[16][2];
extern double cyberspace_clips_4x4_64_y[16][2];
extern int cyberspace_programming_sys;
extern int cyberspace_programming_subsys;
extern int cyberspace_programming_position;

extern cyberspace_user_connection_data_minigame_testing mini_testing;

extern cyberspace_user_connection_data_minigame_heuristic mini_heuristic;
extern bool mini_heuristic_sent;
extern int mini_heuristic_values[11];
extern bool mini_heuristic_morphing;
extern struct timeval mini_heuristic_morph_start;
extern struct timeval mini_heuristic_morph_now;
//extern int mini_heuristic_morph_phase;
//extern int mini_heuristic_morph_time;

extern cyberspace_user_connection_data_minigame_fast mini_fast;
extern int mini_fast_milliseconds_left;
extern int mini_fast_p_x;
extern int mini_fast_p_y;
extern bool mini_fast_sent;
extern onebyte mini_fast_path[200];
extern int mini_fast_path_l;
extern struct timeval mini_fast_start;
extern struct timeval mini_fast_now;

extern cyberspace_user_connection_data_minigame_patternmatching mini_patternmatching;

#endif /* CYBERSPACE_CLIENT_GLOBAL */

