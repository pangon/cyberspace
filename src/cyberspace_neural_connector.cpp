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


#include "cyberspace_neural_connector.h"


int main(int argsn, char** args) {
	if(sizeof(onebyte)!=1) {
		fprintf(stderr, "FATAL ERROR: sizeof(onebyte)!=1.\n");
		exit(EXIT_FAILURE);
		}

	if(argsn!=1 && argsn!=3 && argsn!=2 && argsn!=4) {
		fprintf(stderr, "usage: %s [server] [screen_width screen_height]\n", args[0]);
		exit(EXIT_FAILURE);
		}

	char server_string[100];

	if(argsn>=3) {
		if(argsn==4) {
			cyberspace_screen_width=atoi(args[2]);
			cyberspace_screen_height=atoi(args[3]);
			sprintf(server_string, args[1]);
			}
		else {
			cyberspace_screen_width=atoi(args[1]);
			cyberspace_screen_height=atoi(args[2]);
			sprintf(server_string, CYBERSPACE_SERVER_NAME);
			}
		}
	else {
		cyberspace_screen_width=CYBERSPACE_GUI_WIDTH;
		cyberspace_screen_height=CYBERSPACE_GUI_HEIGHT;
		if(argsn==1) sprintf(server_string, CYBERSPACE_SERVER_NAME);
		else sprintf(server_string, args[1]);
		}

//	printf("server_string=\"%s\"\n", server_string);

	cyberspace_textures=new GLuint[CYBERSPACE_GUI_TEXTURES_MAX];
	cyberspace_textures_raw=new void*[CYBERSPACE_GUI_TEXTURES_MAX];

	FILE* rawfile;
	int l;

	for(int i=0;i<CYBERSPACE_GUI_TEXTURES_MAX;i++) {
		switch(i) {
			case CYBERSPACE_GUI_TEXTURE_TILE_32_CAN:
				l=32;
				rawfile=fopen("img/tile_32_can.raw", "rb");
				break;
			case CYBERSPACE_GUI_TEXTURE_TILE_32_RADIO:
				l=32;
				rawfile=fopen("img/tile_32_radio.raw", "rb");
				break;
			case CYBERSPACE_GUI_TEXTURE_TILE_32_CRYSTAL:
				l=32;
				rawfile=fopen("img/tile_32_crystal.raw", "rb");
				break;
			case CYBERSPACE_GUI_TEXTURE_TILE_32_START:
				l=32;
				rawfile=fopen("img/tile_32_start.raw", "rb");
				break;
			case CYBERSPACE_GUI_TEXTURE_TILE_32_END:
				l=32;
				rawfile=fopen("img/tile_32_end.raw", "rb");
				break;
			case CYBERSPACE_GUI_TEXTURE_TILE_32_FLOOR:
				l=32;
				rawfile=fopen("img/tile_32_floor.raw", "rb");
				break;
			case CYBERSPACE_GUI_TEXTURE_TILE_32_CULT_0:
				l=32;
				rawfile=fopen("img/tile_32_cult0.raw", "rb");
				break;
			case CYBERSPACE_GUI_TEXTURE_TILES_4X4_64_ICONS:
				l=256;
				rawfile=fopen("img/tiles_4x4_64_icons.raw", "rb");
				break;
/*
			case CYBERSPACE_GUI_TEXTURE_TILE_256_SELF:
				l=256;
				rawfile=fopen("img/self.raw", "rb");
				break;
*/
			default:
				l=32;
				rawfile=fopen("img/tile_32_can.raw", "rb");
				break;
			}
		cyberspace_textures_raw[i]=new onebyte[l*l*4];
		if(rawfile==NULL) {
			fprintf(stderr, "ABORT: fopen raw fails\n");
			exit(EXIT_FAILURE);
			}
		fread(cyberspace_textures_raw[i], l*l*4, 1, rawfile);
		fclose(rawfile);
		}

	for(int x=0;x<4;x++) {
		for(int y=0;y<4;y++) {
			cyberspace_clips_4x4_64_x[y*4+x][0]=0.25*x;
			cyberspace_clips_4x4_64_x[y*4+x][1]=0.25*(x+1);
			cyberspace_clips_4x4_64_y[y*4+x][0]=0.25*y;
			cyberspace_clips_4x4_64_y[y*4+x][1]=0.25*(y+1);
			}
		}

	printf("neural_connector: init SDL\n");
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTTHREAD)!=0) {
		fprintf(stderr, "neural_connector: FATAL ERROR: SDL_Init fails: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}

	if(atexit(SDL_Quit)!=0) {
		fprintf(stderr, "neural_connector: FATAL ERROR: couldn't set exit function.\n");
		exit(EXIT_FAILURE);
		}

	if(SDLNet_Init()!=0) {
		fprintf(stderr, "neural_connector: FATAL ERROR: SDLNet_Init fails: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}

	//non serve
/*	printf("neural_connector: init SDL_Image\n");
	if(IMG_Init(IMG_INIT_PNG)!=IMG_INIT_PNG) {
		fprintf(stderr, "neural_connector: FATAL ERROR: IMG_Init fails: %s\n", IMG_GetError());
		exit(EXIT_FAILURE);
		}*/

	//using X mutlithreading
	//TODO se abilito SDL con multithread probabilmente non serve
/*	if(XInitThreads()==0) {
		fprintf(stderr, "FATAL ERROR: XInitThreads fails.\n");
		exit(EXIT_FAILURE);
		}*/

	cyberspace_login_name=new char[100];
	cyberspace_login_name[0]='\0';
	cyberspace_login_password=new char[100];
	cyberspace_login_password[0]='\0';

	cyberspace_fps=new char[20];
	cyberspace_fps[0]='\0';

	cyberspace_chat=new char*[CYBERSPACE_GUI_CHAT_HISTORY];
	for(int i=0;i<CYBERSPACE_GUI_CHAT_HISTORY;i++) {
		cyberspace_chat[i]=new char[CYBERSPACE_GUI_CHAT_LINE_WIDTH+1+25];
		cyberspace_chat[i][0]='\0';
		}
	cyberspace_chat_input=new char[CYBERSPACE_GUI_CHAT_LINE_WIDTH+1];
	cyberspace_chat_input[0]='\0';

	cyberspace_server_message=new char[1000];
	cyberspace_server_message[0]='\0';

	cyberspace_selection_message=new char[1000];
	cyberspace_selection_message[0]='\0';

	cyberspace_GUI_input_focus=CYBERSPACE_GUI_INPUT_FOCUS_NONE;


	cyberspace_personal_data.id_static=-1;

/*	pthread_mutexattr_t mutex_attr;
	pthread_mutexattr_init(&mutex_attr);
	pthread_mutex_init(&cyberspace_mutex_phase, &mutex_attr);
	pthread_mutex_init(&cyberspace_mutex_world, &mutex_attr);*/
	cyberspace_mutex_phase_sdl=SDL_CreateMutex();
	cyberspace_mutex_world_sdl=SDL_CreateMutex();

//	pthread_mutex_lock(&cyberspace_mutex_phase);
	SDL_mutexP(cyberspace_mutex_phase_sdl);
	cyberspace_phase=CYBERSPACE_PHASE_CONNECTING;
	SDL_mutexV(cyberspace_mutex_phase_sdl);
//	pthread_mutex_unlock(&cyberspace_mutex_phase);

/*	pthread_attr_t thread_detached;
	pthread_attr_init(&thread_detached);
	pthread_attr_setdetachstate(&thread_detached, PTHREAD_CREATE_DETACHED);*/

/*	if(pthread_create(&cyberspace_GUI_thread, &thread_detached, cyberspace_GUI_tf, NULL)!=0) {
		fprintf(stderr, "FATAL ERROR: GUI thread creation fails.\n");
		exit(EXIT_FAILURE);
		}*/

	cyberspace_GUI_thread_sdl=SDL_CreateThread(cyberspace_GUI_tf_sdl, NULL);
	if(cyberspace_GUI_thread_sdl==NULL) {
		fprintf(stderr, "FATAL ERROR: GUI thread creation fails: %s.\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}

//	struct timespec tosleep;
//	struct timespec tosleepagain;

	printf("neural_connector: STARTED\n");

	IPaddress server_address;

	printf("neural_connector: creating local input socket...\n");
/*	if((cyberspace_input_socket_fd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==-1) {
		fprintf(stderr, "FATAL ERROR: local input socket creation fails.\n");
		exit(EXIT_FAILURE);
		}*/
	if(SDLNet_ResolveHost(&server_address, server_string, CYBERSPACE_FRONTDOOR_SOCKET_PORT_S2C)!=0) {
		fprintf(stderr, "FATAL ERROR: SDLNet_ResolveHost: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}
	if((cyberspace_input_socket=SDLNet_TCP_Open(&server_address))==NULL) {
		fprintf(stderr, "FATAL ERROR: local input socket creation fails: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}
	printf("neural_connector: creating local input socket DONE\n");

	int server_code;
	printf("neural_connector: receiving server code...\n");
	cyberspace_to_read=sizeof(server_code);
	if(SDLNet_TCP_Recv(cyberspace_input_socket, cyberspace_read_buffer, cyberspace_to_read)<=0) {
		fprintf(stderr, "FATAL ERROR: SDLNet_TCP_Recv: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}
	memcpy(&server_code, cyberspace_read_buffer, sizeof(server_code));
	printf("neural_connector: server code=%d\n", server_code);

	printf("neural_connector: creating local output socket...\n");
/*	if((cyberspace_output_socket_fd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==-1) {
		fprintf(stderr, "FATAL ERROR: local output socket creation fails.\n");
		exit(EXIT_FAILURE);
		}*/
	if(SDLNet_ResolveHost(&server_address, server_string, CYBERSPACE_FRONTDOOR_SOCKET_PORT_C2S)!=0) {
		fprintf(stderr, "FATAL ERROR: SDLNet_ResolveHost: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}
	if((cyberspace_output_socket=SDLNet_TCP_Open(&server_address))==NULL) {
		fprintf(stderr, "FATAL ERROR: local output socket creation fails: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}
	printf("neural_connector: creating local output socket DONE\n");

	printf("neural_connector: sending input/output coordination code...\n");
	cyberspace_to_write=sizeof(server_code);
	memcpy(cyberspace_write_buffer, &server_code, sizeof(server_code));
	if(SDLNet_TCP_Send(cyberspace_output_socket, cyberspace_write_buffer, cyberspace_to_write)<cyberspace_to_write) {
		fprintf(stderr, "FATAL ERROR: SDLNet_TCP_Send: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}
	printf("neural_connector: sending input/output coordination code DONE\n");

/*
	int output_remote_socket_fd;
	int input_remote_socket_fd;

	struct addrinfo sok_addr_info;
	struct addrinfo* sok_addr_res;

	memset(&sok_addr_info, 0, sizeof(sok_addr_info));
	sok_addr_info.ai_family=AF_INET;
	sok_addr_info.ai_socktype=SOCK_STREAM;
	sok_addr_info.ai_protocol=IPPROTO_TCP;

	printf("neural_connector: opening of input connection...\n");
	if(getaddrinfo(server_string, CYBERSPACE_FRONTDOOR_SOCKET_PORT_STRING_S2C, &sok_addr_info, &sok_addr_res)) {
		fprintf(stderr, "FATAL ERROR: getaddrinfo of server fails.\n");
		exit(EXIT_FAILURE);
		}

	while((input_remote_socket_fd=connect(cyberspace_input_socket_fd, sok_addr_res->ai_addr, sok_addr_res->ai_addrlen))!=0) {
		printf("neural_connector: connecting input... (%d)\n", input_remote_socket_fd);
		tosleep.tv_sec=5;
		tosleep.tv_nsec=0;
		while(nanosleep(&tosleep, &tosleepagain)!=0) {
			tosleep=tosleepagain;
			}
		}
	printf("neural_connector: opening of input connection DONE\n");

	int server_code;

	printf("neural_connector: receiving server code...\n");
	cyberspace_to_read=sizeof(server_code);
	cyberspace_read_total=0;
	while(cyberspace_read_total<cyberspace_to_read) {
		cyberspace_read_partial=recv(cyberspace_input_socket_fd, cyberspace_read_buffer+cyberspace_read_total, CYBERSPACE_USER_CONNECTION_READ_BUFFER-1-cyberspace_read_total, 0);
		if(cyberspace_read_partial==-1) {
			perror("perror: ");
			fprintf(stderr, "ERROR: (cyberspace_user_connection) recv failed.\n");
			exit(EXIT_FAILURE);
			}
		cyberspace_read_total+=cyberspace_read_partial;
		}
	printf("neural_connector: receiving server code DONE \n");

	memcpy(&server_code, cyberspace_read_buffer, sizeof(server_code));
	printf("neural_connector: server code=%d\n", server_code);

	memset(&sok_addr_info, 0, sizeof(sok_addr_info));
	sok_addr_info.ai_family=AF_INET;
	sok_addr_info.ai_socktype=SOCK_STREAM;
	sok_addr_info.ai_protocol=IPPROTO_TCP;

	printf("neural_connector: opening of output connection...\n");
	if(getaddrinfo(server_string, CYBERSPACE_FRONTDOOR_SOCKET_PORT_STRING_C2S, &sok_addr_info, &sok_addr_res)) {
//	if(getaddrinfo("127.1.1.1", CYBERSPACE_FRONTDOOR_SOCKET_PORT_STRING_C2S, &sok_addr_info, &sok_addr_res)) {
		fprintf(stderr, "FATAL ERROR: getaddrinfo of server fails.\n");
		exit(EXIT_FAILURE);
		}

	while((output_remote_socket_fd=connect(cyberspace_output_socket_fd, sok_addr_res->ai_addr, sok_addr_res->ai_addrlen))!=0) {
		printf("neural_connector: connecting output... (%d)\n", output_remote_socket_fd);
		tosleep.tv_sec=5;
		tosleep.tv_nsec=0;
		while(nanosleep(&tosleep, &tosleepagain)!=0) {
			tosleep=tosleepagain;
			}
		}
	printf("neural_connector: opening of output connection DONE\n");

	printf("neural_connector: sending input/output coordination code...\n");
	cyberspace_to_write=sizeof(server_code);
	memcpy(cyberspace_write_buffer, &server_code, sizeof(server_code));
	cyberspace_written_total=0;
	while(cyberspace_to_write>cyberspace_written_total) {
		cyberspace_written_partial=send(cyberspace_output_socket_fd, cyberspace_write_buffer+cyberspace_written_total, cyberspace_to_write-cyberspace_written_total, 0);
		if(cyberspace_written_partial==-1) {
			perror("perror: ");
			fprintf(stderr, "ERROR: (in writing) communication with remote process failed.\n");
			exit(EXIT_FAILURE);
			}
		cyberspace_written_total+=cyberspace_written_partial;
		}
	printf("neural_connector: sending input/output coordination code DONE\n");
*/

/*	if(pthread_create(&cyberspace_input_thread, &thread_detached, cyberspace_neural_connector_input_tf, NULL)!=0) {
		fprintf(stderr, "FATAL ERROR: input thread creation fails.\n");
		exit(EXIT_FAILURE);
		}*/

	cyberspace_input_thread_sdl=SDL_CreateThread(cyberspace_neural_connector_input_tf_sdl, NULL);
	if(cyberspace_input_thread_sdl==NULL) {
		fprintf(stderr, "FATAL ERROR: GUI thread creation fails: %s.\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}

	int command_code;

	cyberspace_to_end=false;

	while(!cyberspace_to_end) {
		printf("invia un comando\n");
		printf("\t0\tclose connection\n");
		scanf("%d", &command_code);

		cyberspace_output_command.type=command_code;

/*			data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
			data->out_command.include_data_s=true;
			data->out_command.include_data_m=false;
			data->out_command.include_data_l=false;
			char* errormessage=new char[CYBERSPACE_COMMAND_SIZE_S];
			sprintf(errormessage, "unexpected message in login");
			memcpy(data->out_command.data_s, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));*/

//		cyberspace_neural_connector_output(&cyberspace_output_command);

		printf("message NOT sent\n");
		}

	SDL_WaitThread(cyberspace_GUI_thread_sdl, NULL);
	SDL_WaitThread(cyberspace_input_thread_sdl, NULL);

	printf("neural_connector: ENDING\n");
//	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
	}

