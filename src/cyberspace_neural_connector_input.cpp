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


#include "cyberspace_neural_connector_input.h"



int cyberspace_neural_connector_input_tf_sdl(void* nothing) {
	cyberspace_neural_connector_input_tf(nothing);
	return(0);
	}

void* cyberspace_neural_connector_input_tf(void* nothing) {
	cyberspace_server_command command;

	printf("input: STARTED\n");

	int toread;
	int read_offset;
	char* chat_tmp;
	int read_size;

	while(true) {
		cyberspace_to_read=sizeof(cyberspace_read_message_size);
		while((read_size=SDLNet_TCP_Recv(cyberspace_input_socket, cyberspace_read_buffer, cyberspace_to_read))<=0) {
			fprintf(stderr, "WARNING: SDLNet_TCP_Recv (get message size) of size %d: %s.\n", read_size, SDLNet_GetError());
//			fprintf(stderr, "FATAL ERROR: SDLNet_TCP_Recv (get message size): %s.\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
			}
		memcpy(&cyberspace_read_message_size, cyberspace_read_buffer, sizeof(cyberspace_read_message_size));
		if(cyberspace_read_message_size==0) {
			fprintf(stderr, "ERROR: (cyberspace input) received a cyberspace_read_message_size==0 message.\n");
//			continue;
			exit(EXIT_FAILURE);
			}
		if(cyberspace_read_message_size>=CYBERSPACE_USER_CONNECTION_READ_BUFFER-1) {
			fprintf(stderr, "ERROR: (cyberspace input) received a cyberspace_read_message_size==%d (>=%d) message.\n", cyberspace_read_message_size, CYBERSPACE_USER_CONNECTION_READ_BUFFER-1);
			exit(EXIT_FAILURE);
			}
		printf("input: received message of size %d\n", cyberspace_read_message_size);

		if(SDLNet_TCP_Recv(cyberspace_input_socket, cyberspace_read_buffer, cyberspace_read_message_size-sizeof(cyberspace_read_message_size))<=0) {
			fprintf(stderr, "FATAL ERROR: SDLNet_TCP_Recv (get message): %s.\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
			}
		memcpy(&command, cyberspace_read_buffer, sizeof(command));

/*
//		printf("start recving\n");
		cyberspace_read_total=0;
		cyberspace_read_message_size=0;
		while(cyberspace_read_total<(int)sizeof(cyberspace_read_message_size) || cyberspace_read_total<cyberspace_read_message_size) {
//			printf("recving...\n");
//			cyberspace_read_partial=recv(cyberspace_input_socket_fd, cyberspace_read_buffer+cyberspace_read_total, CYBERSPACE_USER_CONNECTION_READ_BUFFER-1-cyberspace_read_total, 0);
			if(cyberspace_read_message_size==0) {
				cyberspace_read_partial=recv(cyberspace_input_socket_fd, cyberspace_read_buffer+cyberspace_read_total, sizeof(cyberspace_read_message_size), 0);
				}
			else {
				cyberspace_read_partial=recv(cyberspace_input_socket_fd, cyberspace_read_buffer+cyberspace_read_total, cyberspace_read_message_size-cyberspace_read_total, 0);
				}
//			printf("recved\n");
			if(cyberspace_read_partial==-1) {
				perror("perror: ");
				fprintf(stderr, "ERROR: (cyberspace input) recv failed.\n");
				exit(EXIT_FAILURE);
				}
			cyberspace_read_total+=cyberspace_read_partial;
			if(cyberspace_read_message_size==0 && cyberspace_read_total>=(int)sizeof(cyberspace_read_message_size)) {
				memcpy(&(cyberspace_read_message_size), cyberspace_read_buffer, sizeof(cyberspace_read_message_size));
				if(cyberspace_read_message_size==0) {
					fprintf(stderr, "ERROR: (cyberspace input) received a cyberspace_read_message_size==0 message.\n");
					exit(EXIT_FAILURE);
					}
				if(cyberspace_read_message_size>=CYBERSPACE_USER_CONNECTION_READ_BUFFER-1) {
					fprintf(stderr, "ERROR: (cyberspace input) received a cyberspace_read_message_size==%d (>=%d) message.\n", cyberspace_read_message_size, CYBERSPACE_USER_CONNECTION_READ_BUFFER-1);
					exit(EXIT_FAILURE);
					}
				}
			}
//		printf("done recving\n");

		if(cyberspace_read_total!=cyberspace_read_message_size) {
			fprintf(stderr, "ERROR: (cyberspace input) cyberspace_read_total!=cyberspace_read_message_size (%d !=%d).\n", cyberspace_read_total, cyberspace_read_message_size);
			exit(EXIT_FAILURE);
			}

//		printf("input: received message of size %d\n", cyberspace_read_message_size);

//		memcpy(&(command.type), cyberspace_read_buffer+sizeof(cyberspace_read_message_size), sizeof(command.type));
		memcpy(&command, cyberspace_read_buffer+sizeof(cyberspace_read_message_size), sizeof(command));
*/

		printf("input: command: %d\n", command.type);

		switch(command.type) {
			case CYBERSPACE_COMMAND_SERVER_ERROR:
				printf("SERVER ERROR MESSAGE: \"%s\"\n", command.data);
//				pthread_mutex_lock(&cyberspace_mutex_world);
				SDL_mutexP(cyberspace_mutex_world_sdl);
				strcpy(cyberspace_server_message, (char*)(command.data));
				cyberspace_server_message_fade=5000;
				chat_tmp=cyberspace_chat[CYBERSPACE_GUI_CHAT_HISTORY-1];
				for(int i=CYBERSPACE_GUI_CHAT_HISTORY-1;i>=1;i--) {
					cyberspace_chat[i]=cyberspace_chat[i-1];
					}
				cyberspace_chat[0]=chat_tmp;
				strcpy(cyberspace_chat[0], "error : ");
				strcpy(cyberspace_chat[0]+8, (char*)(command.data));
				SDL_mutexV(cyberspace_mutex_world_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_world);
				break;
			case CYBERSPACE_COMMAND_SERVER_CHAT:
//				pthread_mutex_lock(&cyberspace_mutex_world);
				SDL_mutexP(cyberspace_mutex_world_sdl);
				chat_tmp=cyberspace_chat[CYBERSPACE_GUI_CHAT_HISTORY-1];
				for(int i=CYBERSPACE_GUI_CHAT_HISTORY-1;i>=1;i--) {
					cyberspace_chat[i]=cyberspace_chat[i-1];
					}
				cyberspace_chat[0]=chat_tmp;
				strcpy(cyberspace_chat[0], (char*)(command.data));
				SDL_mutexV(cyberspace_mutex_world_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_world);
				break;
			case CYBERSPACE_COMMAND_SERVER_PHASE_LOGIN:
				printf("input: transito to login.\n");
//				pthread_mutex_lock(&cyberspace_mutex_phase);
				cyberspace_login_name[0]='\0';
				cyberspace_login_password[0]='\0';
				//this needed for the clean logout
				for(int ci=0;ci<CYBERSPACE_GUI_CHAT_HISTORY;ci++) {
					cyberspace_chat[ci][0]='\0';
					}
				cyberspace_chat_input[0]='\0';
				cyberspace_GUI_input_focus=CYBERSPACE_GUI_INPUT_FOCUS_LOGIN;
				SDL_mutexP(cyberspace_mutex_phase_sdl);
				cyberspace_phase=CYBERSPACE_PHASE_LOGIN;
				SDL_mutexV(cyberspace_mutex_phase_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_phase);
				printf("input: transito to login: DONE\n");
				break;
			case CYBERSPACE_COMMAND_SERVER_PHASE_3D:
				printf("input: transito to 3D.\n");
//				pthread_mutex_lock(&cyberspace_mutex_phase);
				SDL_mutexP(cyberspace_mutex_phase_sdl);
				cyberspace_world.camera.position.x=0.0;
				cyberspace_world.camera.position.y=0.0;
				cyberspace_world.camera.position.z=0.0;
				cyberspace_world.camera.angle.alpha=0.0;
				cyberspace_world.camera.angle.beta=0.0;
				cyberspace_world.camera.angle.gamma=0.0;
				cyberspace_obj_selected.dynamic=true;
				cyberspace_obj_selected.id=-1;
				cyberspace_obj_selected.lock=false;
				cyberspace_phase=CYBERSPACE_PHASE_3D;
				SDL_mutexV(cyberspace_mutex_phase_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_phase);
				printf("input: transito to 3D: DONE\n");
				break;
			case CYBERSPACE_COMMAND_SERVER_PHASE_PROGRAMMING:
				printf("input: transito to programming.\n");
//				pthread_mutex_lock(&cyberspace_mutex_phase);
				SDL_mutexP(cyberspace_mutex_phase_sdl);
				cyberspace_programming_sys=0;
				cyberspace_programming_subsys=0;
				cyberspace_programming_position=0;
				if(cyberspace_programming_subphase!=CYBERSPACE_PHASE_PROGRAMMING_STEPS) cyberspace_programming_subphase=CYBERSPACE_PHASE_PROGRAMMING_SYS;
				cyberspace_phase=CYBERSPACE_PHASE_PROGRAMMING;
				SDL_mutexV(cyberspace_mutex_phase_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_phase);
				printf("input: transito to programming: DONE\n");
				break;
			case CYBERSPACE_COMMAND_SERVER_USER:
				printf("input: ottengo i dati personali.\n");
//				pthread_mutex_lock(&cyberspace_mutex_world);
				SDL_mutexP(cyberspace_mutex_world_sdl);
				memcpy(&cyberspace_personal_data, command.data, sizeof(cyberspace_personal_data));
				SDL_mutexV(cyberspace_mutex_world_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_world);
				break;
			case CYBERSPACE_COMMAND_SERVER_PROGRAMMING:
				printf("input: ottengo le informazioni per la programmazione.\n");
				SDL_mutexP(cyberspace_mutex_world_sdl);
				memcpy(&cyberspace_programming, command.data, sizeof(cyberspace_programming));
				SDL_mutexV(cyberspace_mutex_world_sdl);
				SDL_mutexP(cyberspace_mutex_phase_sdl);
				cyberspace_programming_subphase=CYBERSPACE_PHASE_PROGRAMMING_STEPS;
				SDL_mutexV(cyberspace_mutex_phase_sdl);
				break;
			case CYBERSPACE_COMMAND_SERVER_WORLD_LOAD:
				printf("input: world loading...\n");
				read_offset=0;
//				pthread_mutex_lock(&cyberspace_mutex_world);
				SDL_mutexP(cyberspace_mutex_world_sdl);
				toread=sizeof(int);
				memcpy(&(cyberspace_world.static_size), command.data+read_offset, toread);
//				printf("input: world loading: static=%d\n", cyberspace_world.static_size);
				read_offset+=toread;
				for(int i=0;i<cyberspace_world.static_size;i++) {
//					printf("input: world loading item %d\n", i);
					toread=sizeof(cyberspace_world_object);
					memcpy(&(cyberspace_world.static_obj[i]), command.data+read_offset, toread);
					read_offset+=toread;
					}
//				printf("input: world loaded all items\n");
				if(!cyberspace_obj_selected.dynamic) {
					cyberspace_obj_selected.id=-1;
					cyberspace_obj_selected.lock=false;
					}
				SDL_mutexV(cyberspace_mutex_world_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_world);
				printf("input: world loaded.\n");
				break;
			case CYBERSPACE_COMMAND_SERVER_WORLD_SYNC:
//				printf("input: world sync.\n");
				read_offset=0;
//				pthread_mutex_lock(&cyberspace_mutex_world);
				SDL_mutexP(cyberspace_mutex_world_sdl);
				toread=sizeof(int);
				memcpy(&(cyberspace_world.dynamic_size), command.data+read_offset, toread);
				read_offset+=toread;
				for(int i=0;i<cyberspace_world.dynamic_size;i++) {
					toread=sizeof(cyberspace_world_object);
					memcpy(&(cyberspace_world.dynamic_obj[i]), command.data+read_offset, toread);
					read_offset+=toread;
					if(cyberspace_world.dynamic_obj[i].type==CYBERSPACE_WORLD_OBJECT_USER && cyberspace_world.dynamic_obj[i].parameters[CYBERSPACE_WORLD_OBJECT_USER_PARAMETER_ID_STATIC]==cyberspace_personal_data.id_static) {
						cyberspace_world.camera=cyberspace_world.dynamic_obj[i].point;
						}
					}
				frames++;
				if(cyberspace_obj_selected.dynamic) {
					cyberspace_obj_selected.id=-1;
					cyberspace_obj_selected.lock=false;
					}
				SDL_mutexV(cyberspace_mutex_world_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_world);
				break;
			case CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_TESTING:
				printf("input: transito to minigame testing.\n");
//				pthread_mutex_lock(&cyberspace_mutex_phase);
				SDL_mutexP(cyberspace_mutex_phase_sdl);
				cyberspace_phase=CYBERSPACE_PHASE_MINIGAME_TESTING;
				SDL_mutexV(cyberspace_mutex_phase_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_phase);
				printf("input: transito to minigame testing: DONE\n");
				break;
			case CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_TESTING_POINTS:
				printf("input: ottengo i punti di minigame testing.\n");
//				pthread_mutex_lock(&cyberspace_mutex_world);
				SDL_mutexP(cyberspace_mutex_world_sdl);
				memcpy(&mini_testing.points, command.data, sizeof(mini_testing.points));
				SDL_mutexV(cyberspace_mutex_world_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_world);
				break;
			case CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_HEURISTIC:
				printf("input: transito to minigame heuristic.\n");
//				pthread_mutex_lock(&cyberspace_mutex_phase);
				SDL_mutexP(cyberspace_mutex_phase_sdl);
				for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
					for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
						mini_heuristic.board[y][x]=-1;
						}
					}
				mini_heuristic.points=0;
				mini_heuristic.piece_status=0;
				mini_heuristic.piece_position=4;
				mini_heuristic.max_symbol=3;
				mini_heuristic_values[0]=1;
				mini_heuristic_values[1]=1;
				mini_heuristic_values[2]=1;
				mini_heuristic_morphing=false;
				for(int i=3;i<11;i++) mini_heuristic_values[i]=mini_heuristic_values[i-1]*3;
				mini_heuristic_sent=false;
				cyberspace_phase=CYBERSPACE_PHASE_MINIGAME_HEURISTIC;
				SDL_mutexV(cyberspace_mutex_phase_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_phase);
				printf("input: transito to minigame heuristic: DONE\n");
				break;
			case CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_HEURISTIC_PIECE:
				printf("input: ottengo il nuovo pezzo in minigame heuristic.\n");
				SDL_mutexP(cyberspace_mutex_world_sdl);
				memcpy(mini_heuristic.piece, command.data, 2*sizeof(int));
				mini_heuristic_sent=false;
				SDL_mutexV(cyberspace_mutex_world_sdl);
				break;
			case CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_FAST:
				printf("input: transito to minigame fast.\n");
//				pthread_mutex_lock(&cyberspace_mutex_phase);
				SDL_mutexP(cyberspace_mutex_phase_sdl);
				cyberspace_phase=CYBERSPACE_PHASE_MINIGAME_FAST;
				SDL_mutexV(cyberspace_mutex_phase_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_phase);
				printf("input: transito to minigame fast: DONE\n");
				break;
			case CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_FAST_POINTS_AND_MAP:
				printf("input: ottengo i punti e una nuova mappa in minigame fast.\n");
//				pthread_mutex_lock(&cyberspace_mutex_world);
				SDL_mutexP(cyberspace_mutex_world_sdl);
				memcpy(&mini_fast.points, command.data, sizeof(mini_fast.points));
				memcpy(mini_fast.floor, command.data+sizeof(mini_fast.points), CYBERSPACE_MINIGAME_FAST_MAP_L*CYBERSPACE_MINIGAME_FAST_MAP_L);
				for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
					for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
//						printf("%d", mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]);
						if(mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_START) {
							mini_fast_p_x=x;
							mini_fast_p_y=y;
							}
						}
//					printf("\n");
					}
				mini_fast_milliseconds_left=1000+(30*1000)/(1+mini_fast.points/10);
				mini_fast_sent=false;
				if(gettimeofday(&mini_fast_start, NULL)!=0) {
					fprintf(stderr, "FATAL ERROR: gettimeofday fails.\n");
					exit(EXIT_FAILURE);
					}
//				mini_fast_path[200]='q';
				mini_fast_path_l=0;
				SDL_mutexV(cyberspace_mutex_world_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_world);
				break;
			case CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_PATTERNMATCHING:
				printf("input: transito to minigame patternmatching.\n");
//				pthread_mutex_lock(&cyberspace_mutex_phase);
				SDL_mutexP(cyberspace_mutex_phase_sdl);
				cyberspace_phase=CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING;
				SDL_mutexV(cyberspace_mutex_phase_sdl);
//				pthread_mutex_unlock(&cyberspace_mutex_phase);
				printf("input: transito to minigame patternmatching: DONE\n");
				break;
			default:
				fprintf(stderr, "ERROR: (cyberspace input) unknown command type: %d.\n", command.type);
				exit(EXIT_FAILURE);
			}
		}

	printf("input: ENDING\n");
//	pthread_exit(NULL);
	return(NULL);
	}

