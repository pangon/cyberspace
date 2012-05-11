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
 
 
 #include "cyberspace_client_global.h"


//int cyberspace_output_socket_fd;
//int cyberspace_input_socket_fd;
TCPsocket cyberspace_output_socket;
TCPsocket cyberspace_input_socket;
int cyberspace_to_read;
int cyberspace_read_message_size;
int cyberspace_read_total;
int cyberspace_read_partial;
int cyberspace_to_write;
int cyberspace_written_total;
int cyberspace_written_partial;
onebyte cyberspace_read_buffer[CYBERSPACE_USER_CONNECTION_READ_BUFFER];
onebyte cyberspace_write_buffer[CYBERSPACE_USER_CONNECTION_WRITE_BUFFER];
//pthread_t cyberspace_input_thread;
SDL_Thread* cyberspace_input_thread_sdl;
bool cyberspace_to_end;
cyberspace_user_command cyberspace_output_command;
//pthread_mutex_t cyberspace_mutex_phase;
SDL_mutex* cyberspace_mutex_phase_sdl;
int cyberspace_phase;
//pthread_t cyberspace_GUI_thread;
SDL_Thread* cyberspace_GUI_thread_sdl;
//pthread_t cyberspace_GUI_input_thread;
SDL_Thread* cyberspace_GUI_input_thread_sdl;
int cyberspace_GUI_screen_type;
SDL_Surface* cyberspace_GUI_screen;
SDL_Surface* cyberspace_GUI_screen_gl;
//pthread_mutex_t cyberspace_mutex_world;
SDL_mutex* cyberspace_mutex_world_sdl;
cyberspace_GUI_world cyberspace_world;
//double cyberspace_GUI_rotation_alpha;
//double cyberspace_GUI_traslation_ahead;
cyberspace_user_movement cyberspace_movement;
int frames;
cyberspace_user_personal_data cyberspace_personal_data;
cyberspace_user_programming cyberspace_programming;
cyberspace_world_selection cyberspace_obj_selected;
bool cyberspace_sreen_reload;
char* cyberspace_login_name;
char* cyberspace_login_password;
int cyberspace_GUI_input_focus;
char** cyberspace_chat;
char* cyberspace_chat_input;
char* cyberspace_server_message;
int cyberspace_server_message_fade;
char* cyberspace_fps;
char* cyberspace_selection_message;
int cyberspace_programming_subphase;
int cyberspace_screen_width;
int cyberspace_screen_height;
GLuint* cyberspace_textures;
void** cyberspace_textures_raw;
double cyberspace_clips_4x4_64_x[16][2];
double cyberspace_clips_4x4_64_y[16][2];
int cyberspace_programming_sys;
int cyberspace_programming_subsys;
int cyberspace_programming_position;

cyberspace_user_connection_data_minigame_testing mini_testing;

cyberspace_user_connection_data_minigame_heuristic mini_heuristic;
bool mini_heuristic_sent;
int mini_heuristic_values[11];
bool mini_heuristic_morphing;
struct timeval mini_heuristic_morph_start;
struct timeval mini_heuristic_morph_now;
//int mini_heuristic_morph_phase;
//int mini_heuristic_morph_time;

cyberspace_user_connection_data_minigame_fast mini_fast;
int mini_fast_milliseconds_left;
int mini_fast_p_x;
int mini_fast_p_y;
bool mini_fast_sent;
onebyte mini_fast_path[200];
int mini_fast_path_l;
struct timeval mini_fast_start;
struct timeval mini_fast_now;

cyberspace_user_connection_data_minigame_patternmatching mini_patternmatching;

