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


#include "cyberspace_user_connection.h"


int cyberspace_user_connection_tf_sdl(void* data_void) {
	cyberspace_user_connection_tf(data_void);
	return(0);
	}

void* cyberspace_user_connection_tf(void* data_void) {
	srandom(time(NULL));
	cyberspace_user_connection_data* data=(cyberspace_user_connection_data*)data_void;

	printf("user_connection_%d: STARTED\n", data->id_dynamic);

	data->write_buffer=new onebyte[CYBERSPACE_USER_CONNECTION_WRITE_BUFFER];

	printf("user_connection_%d: sending input/output coordination code...\n", data->id_dynamic);
	data->to_write=sizeof(data->server_code);
	memcpy(data->write_buffer, &(data->server_code), sizeof(data->server_code));
	data->written_total=0;

	if(SDLNet_TCP_Send(data->out_socket, data->write_buffer, data->to_write)<data->to_write) {
		fprintf(stderr, "FATAL ERROR: (in coordination) SDLNet_TCP_Send: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}

/*
	while(data->to_write>data->written_total) {
		data->written_partial=send(data->out_socket, data->write_buffer+data->written_total, data->to_write-data->written_total, 0);
		if(data->written_partial==-1) {
			perror("perror: ");
			fprintf(stderr, "ERROR: (in coordination) communication with remote process failed.\n");
			exit(EXIT_FAILURE);
			}
		data->written_total+=data->written_partial;
		}
*/

	printf("user_connection_%d: sending input/output coordination code DONE\n", data->id_dynamic);

	struct timespec tosleep;
	struct timespec tosleepagain;

	while(data->read_buffer==NULL) {
		printf("user_connection_%d: waiting for coordination\n", data->id_dynamic);
		tosleep.tv_sec=2;
		tosleep.tv_nsec=0;
		while(nanosleep(&tosleep, &tosleepagain)!=0) {
			tosleep=tosleepagain;
			}
		}

/*
	no, output only by function

	printf("user_connection_%d: creating output thread...\n", data->id);
	pthread_attr_t thread_detached;
	pthread_attr_init(&thread_detached);
	pthread_attr_setdetachstate(&thread_detached, PTHREAD_CREATE_DETACHED);
	if(pthread_create(&output_thread, &thread_detached, cyberspace_user_connection_output_tf, data)!=0) {
		fprintf(stderr, "FATAL ERROR: user_connection_%d: output thread creation fails.\n", data->id);
		exit(EXIT_FAILURE);
		}
	printf("user_connection_%d: creating output thread DONE\n", data->id);
*/

	printf("user_connection_%d: connection completed\n", data->id_dynamic);

	data->phase=CYBERSPACE_PHASE_LOGIN;

	data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_LOGIN;
	cyberspace_user_connection_output(data, &(data->out_command));

	data->to_end=false;

	char* errormessage;

	while(!data->to_end) {
		data->read_total=0;
		data->read_message_size=0;


		if(SDLNet_TCP_Recv(data->in_socket, data->read_buffer, sizeof(data->read_message_size))<=0) {
			fprintf(stderr, "FATAL ERROR: (cyberspace_user_connection) SDLNet_TCP_Recv (get message size): %s.\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
			}
		memcpy(&(data->read_message_size), data->read_buffer, sizeof(data->read_message_size));
		if(data->read_message_size==0) {
			fprintf(stderr, "ERROR: (cyberspace_user_connection) received a data->read_message_size==0 message.\n");
			exit(EXIT_FAILURE);
			}
		if(data->read_message_size>=CYBERSPACE_USER_CONNECTION_READ_BUFFER-1) {
			fprintf(stderr, "ERROR: (cyberspace_user_connection) received a data->read_message_size==%d (>=%d) message.\n", data->read_message_size, CYBERSPACE_USER_CONNECTION_READ_BUFFER-1);
			exit(EXIT_FAILURE);
			}

		if(SDLNet_TCP_Recv(data->in_socket, data->read_buffer, data->read_message_size-sizeof(data->read_message_size))<=0) {
			fprintf(stderr, "FATAL ERROR: (cyberspace_user_connection) SDLNet_TCP_Recv (get message): %s.\n", SDLNet_GetError());
			exit(EXIT_FAILURE);
			}
		memcpy(&(data->in_command), data->read_buffer, sizeof(data->in_command));


/*
//		while(data->read_total==0 || data->read_buffer[data->read_total-1]!='|') {
		while(data->read_total<(int)sizeof(data->read_message_size) || data->read_total<data->read_message_size) {
//			data->read_partial=recv(data->in_socket, data->read_buffer+data->read_total, CYBERSPACE_USER_CONNECTION_READ_BUFFER-1-data->read_total, 0);
			if(data->read_message_size==0) {
				data->read_partial=recv(data->in_socket, data->read_buffer+data->read_total, sizeof(data->read_message_size), 0);
				}
			else {
				data->read_partial=recv(data->in_socket, data->read_buffer+data->read_total, data->read_message_size-data->read_total, 0);
				}
			if(data->read_partial==-1) {
				perror("perror: ");
				fprintf(stderr, "ERROR: (cyberspace_user_connection) recv failed.\n");
				exit(EXIT_FAILURE);
				}
			data->read_total+=data->read_partial;
			if(data->read_message_size==0 && data->read_total>=(int)sizeof(data->read_message_size)) {
				memcpy(&(data->read_message_size), data->read_buffer, sizeof(data->read_message_size));
				if(data->read_message_size==0) {
					fprintf(stderr, "ERROR: (cyberspace_user_connection) received a data->read_message_size==0 message.\n");
					exit(EXIT_FAILURE);
					}
				if(data->read_message_size>=CYBERSPACE_USER_CONNECTION_READ_BUFFER-1) {
					fprintf(stderr, "ERROR: (cyberspace_user_connection) received a data->read_message_size==%d (>=%d) message.\n", data->read_message_size, CYBERSPACE_USER_CONNECTION_READ_BUFFER-1);
					exit(EXIT_FAILURE);
					}
				}
			}

		if(data->read_total!=data->read_message_size) {
			fprintf(stderr, "ERROR: (cyberspace_user_connection) data->read_total!=data->read_message_size (%d !=%d).\n", data->read_total, data->read_message_size);
			exit(EXIT_FAILURE);
			}

//		printf("user_connection_%d phase: %d: received message of size %d\n", data->id_dynamic, data->phase, data->read_message_size);

//		data->read_command.type=atoi(data->read_buffer);
//		memcpy(&(data->in_command.type), data->read_buffer+sizeof(data->read_message_size), sizeof(data->in_command.type));
		memcpy(&(data->in_command), data->read_buffer+sizeof(data->read_message_size), sizeof(data->in_command));
*/



		printf("user_connection_%d phase: %d GET %d\n", data->id_dynamic, data->phase, data->in_command.type);

		switch(data->phase) {
			case CYBERSPACE_PHASE_LOGIN:
				cyberspace_login(data);
				break;
			case CYBERSPACE_PHASE_3D:
				cyberspace_3D_message2server(data);
				break;
			case CYBERSPACE_PHASE_PROGRAMMING:
				cyberspace_programming_message2server(data);
				break;
			case CYBERSPACE_PHASE_MINIGAME_TESTING:
				cyberspace_minigame_testing_message2server(data);
				break;
			case CYBERSPACE_PHASE_MINIGAME_HEURISTIC:
				cyberspace_minigame_heuristic_message2server(data);
				break;
			case CYBERSPACE_PHASE_MINIGAME_FAST:
				cyberspace_minigame_fast_message2server(data);
				break;
			case CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING:
				cyberspace_minigame_patternmatching_message2server(data);
				break;
			default:
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "unknown data->phase: %d", data->phase);
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
			}

//		data->out_command.type=445;
//		cyberspace_user_connection_output(data, &(data->out_command));
		}

	printf("user_connection_%d (%d): ENDING\n", data->id_dynamic, data->personal.id_static);
//	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
	}


void cyberspace_user_connection_output(cyberspace_user_connection_data* user, cyberspace_server_command* command) {
//	printf("sending output...\n");
/*
	switch(command->type) {
		case 0:
		}
*/

	int memcpy_size_partial;
	int memcpy_size_total;

/*
	memcpy_size_total=sizeof(memcpy_size_total);

	memcpy_size_partial=sizeof(command->type);
	memcpy(user->write_buffer+memcpy_size_total, &(command->type), memcpy_size_partial);
	memcpy_size_total+=memcpy_size_partial;

	memcpy_size_partial=sizeof(memcpy_size_total);
	memcpy(user->write_buffer, &memcpy_size_total, memcpy_size_partial);
*/

	memcpy_size_total=sizeof(memcpy_size_total);

	memcpy_size_partial=sizeof(cyberspace_server_command);
	memcpy(user->write_buffer+memcpy_size_total, command, memcpy_size_partial);
	memcpy_size_total+=memcpy_size_partial;

	memcpy_size_partial=sizeof(memcpy_size_total);
	memcpy(user->write_buffer, &memcpy_size_total, memcpy_size_partial);

	user->to_write=memcpy_size_total;

	if(SDLNet_TCP_Send(user->out_socket, user->write_buffer, user->to_write)<user->to_write) {
		fprintf(stderr, "FATAL ERROR: (in connection_output) SDLNet_TCP_Send: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}

/*
	user->written_total=0;
	while(user->to_write>user->written_total) {
		user->written_partial=send(user->out_socket, user->write_buffer+user->written_total, user->to_write-user->written_total, 0);
		if(user->written_partial==-1) {
			perror("perror: ");
			fprintf(stderr, "ERROR: (in connection_output) communication with remote process failed.\n");
			exit(EXIT_FAILURE);
			}
		user->written_total+=user->written_partial;
		}
*/

	if(command->type!=CYBERSPACE_COMMAND_SERVER_WORLD_SYNC) printf("user_connection_%d (%d) phase: %d SEND %d (size %d)\n", user->id_dynamic, user->personal.id_static, user->phase, command->type, memcpy_size_total);
	}

