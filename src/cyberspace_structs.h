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


#ifndef CYBERSPACE_STRUCTS
#define CYBERSPACE_STRUCTS

#include "cyberspace_types.h"
#include <SDL_net.h>


typedef struct {
	double x, y, z;
	} cyberspace_world_point;

/*typedef struct {
	double w, h, d;
	} cyberspace_world_size;*/

typedef struct {
	double alpha, beta, gamma;
	} cyberspace_world_angle;

typedef struct {
	cyberspace_world_point position;
	cyberspace_world_angle angle;
	} cyberspace_world_material_point;

typedef struct {
	double module;
	double alpha;
	double beta;
	} cyberspace_world_vector;


/*typedef struct {
	cyberspace_world_point point;
	cyberspace_world_angle angle;
	cyberspace_world_size size;
	} cyberspace_world_plane;*/

/*typedef struct {
	cyberspace_world_plane green;
	int reds_size;
	cyberspace_world_plane reds[CYBERSPACE_WORLD_RED_PLANES_MAX];
	} cyberspace_world_green_plane;*/

typedef struct {
	int type;
	double parameters[CYBERSPACE_WORLD_OBJECT_PARAMETERS_MAX];
	cyberspace_world_material_point point;
	} cyberspace_world_object;

typedef struct {
	//p0.y e' la coordinata della base
	//p1.y e' la coordinata della cima
	cyberspace_world_point p0;
	cyberspace_world_point p1;
	} cyberspace_world_red_line;

typedef struct {
	cyberspace_world_point p0;
	cyberspace_world_point p1;
	cyberspace_world_point p2;
	} cyberspace_world_floor;

typedef struct {
//	int objects_size;
//	cyberspace_world_object objects[CYBERSPACE_WORLD_OBJECTS_MAX];
//	bool objects_changed[CYBERSPACE_WORLD_OBJECTS_MAX];
	int static_size;
	cyberspace_world_object static_obj[CYBERSPACE_WORLD_STATIC_OBJECTS_MAX];
	int dynamic_size;
	cyberspace_world_object dynamic_obj[CYBERSPACE_WORLD_DYNAMIC_OBJECTS_MAX];
	int red_line_size;
	cyberspace_world_red_line red_line[CYBERSPACE_WORLD_RED_LINES_MAX];
	int floor_size;
	cyberspace_world_floor floor[CYBERSPACE_WORLD_FLOORS_MAX];
//	int users_size;
//	int users[CYBERSPACE_CONNECTIONS_MAX];
	} cyberspace_world_world;

typedef struct {
	int type;
	onebyte data[CYBERSPACE_COMMAND_SIZE_L];
/*	bool include_data_s;
	bool include_data_m;
	bool include_data_l;
	onebyte data_s[CYBERSPACE_COMMAND_SIZE_S];
	onebyte data_m[CYBERSPACE_COMMAND_SIZE_M];
	onebyte data_l[CYBERSPACE_COMMAND_SIZE_L];*/
	} cyberspace_server_command;

typedef struct {
	int type;
	onebyte data[CYBERSPACE_COMMAND_SIZE_M];
/*	bool include_data_s;
	bool include_data_m;
	bool include_data_l;
	onebyte data_s[CYBERSPACE_COMMAND_SIZE_S];
	onebyte data_m[CYBERSPACE_COMMAND_SIZE_M];
	onebyte data_l[CYBERSPACE_COMMAND_SIZE_L];*/
	} cyberspace_user_command;

typedef struct {
	TCPsocket in_socket;
	onebyte* read_buffer;
	} cyberspace_user_connection_coordination;

typedef struct {
	int traslation;
	int rotation;
	bool strafe;
	} cyberspace_user_movement;

typedef struct {
	bool dynamic;
	int id;
	bool lock;
	} cyberspace_world_selection;

typedef struct {
	int id_static;
	char name[101];
	char password[101];
	int skill;
	int connector;
	int systems_max;
	int systems[CYBERSPACE_NEURAL_SYSTEM_MAX];
	int subsystems[CYBERSPACE_NEURAL_SYSTEM_MAX][CYBERSPACE_NEURAL_SUBSYSTEM_MAX];
	int systems_loaded[CYBERSPACE_NEURAL_SYSTEM_MAX];
	int subsystems_loaded[CYBERSPACE_NEURAL_SYSTEM_MAX][CYBERSPACE_NEURAL_SUBSYSTEM_MAX];
	int systems_hint[CYBERSPACE_NEURAL_SYSTEM_MAX];
	int subsystems_hint[CYBERSPACE_NEURAL_SYSTEM_MAX][CYBERSPACE_NEURAL_SUBSYSTEM_MAX];
	int heuristic_record;
	int fast_record;
	int patternmatching_record;
	} cyberspace_user_personal_data;

typedef struct {
	int points;
	} cyberspace_user_connection_data_minigame_testing;

typedef struct {
	int points;
	int piece[2];
	int piece_position;
	int piece_status;
	int max_symbol;
	int board[CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2][CYBERSPACE_MINIGAME_HEURISTIC_MAP_W];
	bool board_morphing[CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2][CYBERSPACE_MINIGAME_HEURISTIC_MAP_W];
	} cyberspace_user_connection_data_minigame_heuristic;

typedef struct {
	int points;
	onebyte floor[CYBERSPACE_MINIGAME_FAST_MAP_L*CYBERSPACE_MINIGAME_FAST_MAP_L];
	bool hor[CYBERSPACE_MINIGAME_FAST_MAP_L*CYBERSPACE_MINIGAME_FAST_MAP_L];
	bool ver[CYBERSPACE_MINIGAME_FAST_MAP_L*CYBERSPACE_MINIGAME_FAST_MAP_L];
	int milliseconds;
	} cyberspace_user_connection_data_minigame_fast;

typedef struct {
	int points;
	} cyberspace_user_connection_data_minigame_patternmatching;

typedef struct {
	int mini_cause;
	int mini_cause_progr_sys;
	int mini_cause_progr_subsys;
	int mini_req_heuristic;
	int mini_req_fast;
	int mini_req_patternmatching;
	int mini_res_heuristic;
	int mini_res_fast;
	int mini_res_patternmatching;
	} cyberspace_user_programming;

typedef struct {
	int id_dynamic;
	SDL_mutex* disk_mutex;
	cyberspace_user_personal_data personal;

	//lifecycle data
	int phase;
	bool to_end;
	bool logging_out;
	int milliseconds_to_logout;

	//communication data
	TCPsocket in_socket;
	TCPsocket out_socket;
	int read_message_size;
	int read_partial;
	int read_total;
	int to_write;
	int written_partial;
	int written_total;
	onebyte* read_buffer;
	onebyte* write_buffer;
	int server_code;
	cyberspace_user_command in_command;
	cyberspace_server_command out_command;
	//no, adesso otput only by function
	//pthread_t output_thread;

	//parsing data
	char* read_parse;
	char* read_parse_last;

	//world data
	int world_id;
	bool need_to_load_world;
	cyberspace_user_movement movement;
//	cyberspace_world_object obj;
	cyberspace_world_object* obj;
	cyberspace_world_selection selection;
	bool falling;
	double falling_speed;
	bool jumping;
	double jumping_speed;

	//programming data
	cyberspace_user_programming programming;

	//minigame data
	cyberspace_user_connection_data_minigame_testing mini_testing;
	cyberspace_user_connection_data_minigame_heuristic mini_heuristic;
	cyberspace_user_connection_data_minigame_fast mini_fast;
	cyberspace_user_connection_data_minigame_patternmatching mini_patternmatching;
	} cyberspace_user_connection_data;

typedef struct {
//	int version_major;
//	int version_minor;
	int accounts;
	} cyberspace_server_nonvolatile_data;

#endif /* CYBERSPACE_STRUCTS */

