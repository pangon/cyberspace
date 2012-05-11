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


#include "cyberspace_frontdoor.h"
//TODO possibile un sacco di connessioni a input senza poi coordinare per creare un user, anche connessioni ad input senza mai avere avviato connessione a frontdoor


int cyberspace_frontdoor_tf_sdl(void* nothing) {
	cyberspace_frontdoor_tf(nothing);
	return(0);
	}


void* cyberspace_frontdoor_tf(void* nothing) {
	for(int i=0;i<CYBERSPACE_CONNECTIONS_MAX;i++) {
		cyberspace_user_connection_datas[i].personal.id_static=0;
		cyberspace_user_connection_datas[i].id_dynamic=0;
		cyberspace_user_connection_datas[i].server_code=0;
		}
	cyberspace_user_connected=0;

	printf("frontdoor: STARTED\n");

	printf("frontdoor: creating server socket...\n");
	IPaddress self_address;
	if(SDLNet_ResolveHost(&self_address, NULL, CYBERSPACE_FRONTDOOR_SOCKET_PORT_S2C)!=0) {
		fprintf(stderr, "FATAL ERROR: SDLNet_ResolveHost in server socket creation: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}
	TCPsocket server_socket;
	if((server_socket=SDLNet_TCP_Open(&self_address))==NULL) {
		fprintf(stderr, "FATAL ERROR: server socket creation fails: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}
	printf("frontdoor: creating server socket DONE\n");
/*
	printf("frontdoor: creating server socket...\n");
	int server_socket_fd;
	if((server_socket_fd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==-1) {
		fprintf(stderr, "FATAL ERROR: server socket creation fails.\n");
		exit(EXIT_FAILURE);
		}
	printf("frontdoor: creating server socket DONE\n");

	printf("frontdoor: binding to port %d...\n", CYBERSPACE_FRONTDOOR_SOCKET_PORT_S2C);
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=INADDR_ANY;
	server_addr.sin_port=htons(CYBERSPACE_FRONTDOOR_SOCKET_PORT_S2C);
	if(bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))!=0) {
		fprintf(stderr, "FATAL ERROR: binding fails.\n");
		exit(EXIT_FAILURE);
		}
	printf("frontdoor: binding to port %d DONE\n", CYBERSPACE_FRONTDOOR_SOCKET_PORT_S2C);

	listen(server_socket_fd, 1000);
	*/

/*
	pthread_attr_t thread_detached;
	pthread_attr_init(&thread_detached);
	pthread_attr_setdetachstate(&thread_detached, PTHREAD_CREATE_DETACHED);
*/

	TCPsocket client_socket;
	while(true) {
		SDL_Delay(1000);
		client_socket=SDLNet_TCP_Accept(server_socket);
		if(client_socket==NULL) continue;
		printf("frontdoor: user connection detected\n");
		cyberspace_user_connection_datas[cyberspace_user_connected].id_dynamic=cyberspace_user_connected;
		cyberspace_user_connection_datas[cyberspace_user_connected].phase=CYBERSPACE_PHASE_CONNECTING;
		cyberspace_user_connection_datas[cyberspace_user_connected].out_socket=client_socket;
		//TODO generare random server code controllando che non sia ripetuto in altro user e comunque !=0
		cyberspace_user_connection_datas[cyberspace_user_connected].server_code=100+cyberspace_user_connected;
		cyberspace_user_connection_datas[cyberspace_user_connected].read_buffer=NULL;
		cyberspace_user_connection_datas[cyberspace_user_connected].out_socket=client_socket;
		cyberspace_user_connection_datas[cyberspace_user_connected].logging_out=false;
		cyberspace_user_connection_datas[cyberspace_user_connected].disk_mutex=SDL_CreateMutex();

/*
		if(pthread_create(&(cyberspace_user_connection_threads[cyberspace_user_connected]), &thread_detached, cyberspace_user_connection_tf, &(cyberspace_user_connection_datas[cyberspace_user_connected]))!=0) {
			fprintf(stderr, "FATAL ERROR: user_connection thread creation fails.\n");
			exit(EXIT_FAILURE);
			}
*/
		cyberspace_user_connection_threads[cyberspace_user_connected]=SDL_CreateThread(cyberspace_user_connection_tf_sdl, &(cyberspace_user_connection_datas[cyberspace_user_connected]));
		if(cyberspace_user_connection_threads[cyberspace_user_connected]==NULL) {
			fprintf(stderr, "FATAL ERROR: user_connection thread creation fails: %s.\n", SDL_GetError());
			exit(EXIT_FAILURE);
			}

		cyberspace_user_connected++;
		}

/*
	int client_socket_fd;
	struct sockaddr_in client_addr;
	socklen_t client_addr_length=sizeof(client_addr);

	pthread_attr_t thread_detached;
	pthread_attr_init(&thread_detached);
	pthread_attr_setdetachstate(&thread_detached, PTHREAD_CREATE_DETACHED);

	while(true) {
		printf("frontdoor: waiting for new user connections\n");
		while((client_socket_fd=accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_length))==-1) {
			fprintf(stderr, "FATAL ERROR: accepting from server socket fails.\n");
			exit(EXIT_FAILURE);
			}
		printf("frontdoor: user connection detected\n");
		cyberspace_user_connection_datas[cyberspace_user_connected].id_dynamic=cyberspace_user_connected;
		cyberspace_user_connection_datas[cyberspace_user_connected].phase=CYBERSPACE_PHASE_CONNECTING;
		cyberspace_user_connection_datas[cyberspace_user_connected].out_socket=client_socket_fd;
		//TODO generare random server code controllando che non sia ripetuto in altro user e comunque !=0
		cyberspace_user_connection_datas[cyberspace_user_connected].server_code=100+cyberspace_user_connected;
		cyberspace_user_connection_datas[cyberspace_user_connected].read_buffer=NULL;
		if(pthread_create(&(cyberspace_user_connection_threads[cyberspace_user_connected]), &thread_detached, cyberspace_user_connection_tf, &(cyberspace_user_connection_datas[cyberspace_user_connected]))!=0) {
			fprintf(stderr, "FATAL ERROR: user_connection thread creation fails.\n");
			exit(EXIT_FAILURE);
			}
		cyberspace_user_connected++;
		}
*/

	printf("frontdoor: ENDING\n");
//	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
	}


int cyberspace_frontdoor_inputchannel_tf_sdl(void* nothing) {
	cyberspace_frontdoor_inputchannel_tf(nothing);
	return(0);
	}


void* cyberspace_frontdoor_inputchannel_tf(void* nothing) {
	printf("frontdoor_inputchannel: STARTED\n");

	printf("frontdoor_inputchannel: creating server socket...\n");
	IPaddress self_address;
	if(SDLNet_ResolveHost(&self_address, NULL, CYBERSPACE_FRONTDOOR_SOCKET_PORT_C2S)!=0) {
		fprintf(stderr, "FATAL ERROR: SDLNet_ResolveHost in server socket creation: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}
	TCPsocket server_socket;
	if((server_socket=SDLNet_TCP_Open(&self_address))==NULL) {
		fprintf(stderr, "FATAL ERROR: server socket creation fails: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}
	printf("frontdoor_inputchannel: creating server socket DONE\n");

/*
	pthread_attr_t thread_detached;
	pthread_attr_init(&thread_detached);
	pthread_attr_setdetachstate(&thread_detached, PTHREAD_CREATE_DETACHED);
*/

//	pthread_t tmp;
	SDL_Thread* tmp_thread_sdl;

//	TCPsocket* client_socket;
	cyberspace_user_connection_coordination* coordination_data;
	coordination_data=new cyberspace_user_connection_coordination;

//	client_socket=new TCPsocket;
	while(true) {
		SDL_Delay(1000);
		coordination_data->in_socket=SDLNet_TCP_Accept(server_socket);
//		*client_socket=SDLNet_TCP_Accept(server_socket);
		if(coordination_data->in_socket==NULL) continue;
//		if(*client_socket==NULL) continue;
		printf("frontdoor_inputchannel: user connection detected\n");
//		coordination_data=new cyberspace_user_connection_coordination;
//		coordination_data->in_socket_p=client_socket;
//		client_socket=new TCPsocket;

/*
		if(pthread_create(&tmp, &thread_detached, cyberspace_user_connection_coordination_tf, coordination_data)!=0) {
			fprintf(stderr, "FATAL ERROR: coordination thread creation fails.\n");
			exit(EXIT_FAILURE);
			}
*/
//		printf("MEM location pre: %lld\n", (void*)coordination_data);
		tmp_thread_sdl=SDL_CreateThread(cyberspace_user_connection_coordination_tf_sdl, coordination_data);
		if(tmp_thread_sdl==NULL) {
			fprintf(stderr, "FATAL ERROR: coordination thread creation fails: %s.\n", SDL_GetError());
			exit(EXIT_FAILURE);
			}
//		printf("MEM location post: %lld\n", (void*)coordination_data);
		coordination_data=new cyberspace_user_connection_coordination;
//		printf("MEM location new: %lld\n", (void*)coordination_data);
		}

/*
	printf("frontdoor_inputchannel: creating server socket...\n");
	int server_socket_fd;
	if((server_socket_fd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==-1) {
		fprintf(stderr, "FATAL ERROR: server socket creation fails.\n");
		exit(EXIT_FAILURE);
		}
	printf("frontdoor_inputchannel: creating server socket DONE\n");

	printf("frontdoor_inputchannel: binding to port %d...\n", CYBERSPACE_FRONTDOOR_SOCKET_PORT_C2S);
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(struct sockaddr_in));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=INADDR_ANY;
	server_addr.sin_port=htons(CYBERSPACE_FRONTDOOR_SOCKET_PORT_C2S);
	if(bind(server_socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))!=0) {
		fprintf(stderr, "FATAL ERROR: binding fails.\n");
		exit(EXIT_FAILURE);
		}
	printf("frontdoor_inputchannel: binding to port %d DONE\n", CYBERSPACE_FRONTDOOR_SOCKET_PORT_C2S);

	listen(server_socket_fd, 1000);
	int client_socket_fd;
	struct sockaddr_in client_addr;
	socklen_t client_addr_length=sizeof(client_addr);

	cyberspace_user_connection_coordination* coordination_data;

	pthread_attr_t thread_detached;
	pthread_attr_init(&thread_detached);
	pthread_attr_setdetachstate(&thread_detached, PTHREAD_CREATE_DETACHED);

	pthread_t tmp;

	while(true) {
		printf("frontdoor_inputchannel: waiting for new user connections\n");
		while((client_socket_fd=accept(server_socket_fd, (struct sockaddr*)&client_addr, &client_addr_length))==-1) {
			fprintf(stderr, "FATAL ERROR: accepting from server socket fails.\n");
			exit(EXIT_FAILURE);
			}
		printf("frontdoor_inputchannel: user connection detected\n");
		coordination_data=new cyberspace_user_connection_coordination;
		coordination_data->in_socket=client_socket_fd;
		if(pthread_create(&tmp, &thread_detached, cyberspace_user_connection_coordination_tf, coordination_data)!=0) {
			fprintf(stderr, "FATAL ERROR: coordination thread creation fails.\n");
			exit(EXIT_FAILURE);
			}
		}
*/

	printf("frontdoor_inputchannel: ENDING\n");
//	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
	}


int cyberspace_user_connection_coordination_tf_sdl(void* coordination_data) {
	cyberspace_user_connection_coordination_tf(coordination_data);
	return(0);
	}


void* cyberspace_user_connection_coordination_tf(void* coordination_data) {
//	printf("MEM location in: %lld\n", (void*)coordination_data);
	cyberspace_user_connection_coordination* data=(cyberspace_user_connection_coordination*) coordination_data;

	printf("coordination: STARTED\n");
	data->read_buffer=new onebyte[CYBERSPACE_USER_CONNECTION_READ_BUFFER];

	int server_code;
//	int read_total=0;
//	int read_partial=0;

	printf("coordination: rec server code...\n");

	if(SDLNet_TCP_Recv(data->in_socket, data->read_buffer, sizeof(server_code))<=0) {
		fprintf(stderr, "FATAL ERROR: (coordination) SDLNet_TCP_Recv: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}

/*
	while(read_total<(int)sizeof(server_code)) {
		read_partial=recv(data->in_socket, data->read_buffer+read_total, CYBERSPACE_USER_CONNECTION_READ_BUFFER-1-read_total, 0);
		if(read_partial==-1) {
			perror("perror: ");
			fprintf(stderr, "ERROR: (coordination) server code recv failed.\n");
			exit(EXIT_FAILURE);
			}
		read_total+=read_partial;
		}

	if(read_total>(int)sizeof(server_code)) {
		fprintf(stderr, "ERROR: (coordination) read_total>sizeof(server_code).\n");
		exit(EXIT_FAILURE);
		}
*/

	memcpy(&server_code, data->read_buffer, sizeof(server_code));

	printf("coordination: server code=%d\n", server_code);

	cyberspace_user_connection_data* coordata=NULL;

	//TODO cosi' e' posibile un furto di connessione, forse di identita', controllare che il tizio con quel code sia in phase connecting
	for(int i=0;i<CYBERSPACE_CONNECTIONS_MAX && coordata==NULL;i++) {
		if(cyberspace_user_connection_datas[i].server_code==server_code) coordata=&(cyberspace_user_connection_datas[i]);
		}

	if(coordata==NULL) {
		fprintf(stderr, "WARNING: (coordination) coordata==NULL.\n");
		}
	else {
		coordata->in_socket=data->in_socket;
		coordata->read_buffer=data->read_buffer;
		}

	printf("coordination: DONE\n");

	delete data;
//	pthread_exit(NULL);
//	exit(EXIT_SUCCESS);
	return(NULL);
	}

