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


#include <stdio.h>
#include <stdlib.h>
//#include <pthread.h>
#include <unistd.h>
#include <string.h>

#include "cyberspace_frontdoor.h"
#include "cyberspace_statistics.h"


int main(int argn, char** args) {
	srandom(time(NULL));
	printf("main: STARTED\n");

	printf("main: init SDL\n");
	if(SDL_Init(SDL_INIT_EVENTTHREAD)!=0) {
		fprintf(stderr, "main: FATAL ERROR: SDL_Init fails: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}

	if(atexit(SDL_Quit)!=0) {
		fprintf(stderr, "main: FATAL ERROR: couldn't set exit function.\n");
		exit(EXIT_FAILURE);
		}

	if(SDLNet_Init()!=0) {
		fprintf(stderr, "main: FATAL ERROR: SDLNet_Init fails: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}

	FILE* file;
	char* path=new char[100];

/*
//	cyberspace_server_data.version_major=0;
//	cyberspace_server_data.version_minor=1;
	cyberspace_server_data.accounts=0;
	int filed;
	sprintf(path, "server_data.now");
	if((file=fopen(path, "w+"))==NULL) {
		perror("perror:\n");
		fprintf(stderr, "FATAL ERROR: unable to fopen file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if(fwrite(&cyberspace_server_data, sizeof(cyberspace_server_data), 1, file)!=1) {
		fprintf(stderr, "FATAL ERROR: unable to fwrite file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if((filed=fileno(file))==-1) {
		fprintf(stderr, "FATAL ERROR: unable to fileno file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if(fdatasync(filed)!=0) {
		fprintf(stderr, "FATAL ERROR: unable to fdatasync file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if(fclose(file)!=0) {
		fprintf(stderr, "FATAL ERROR: unable to fclose file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	printf("main: ONCE DONE\n");
	exit(EXIT_FAILURE);
*/

	sprintf(path, "server_data.now");
	if((file=fopen(path, "r+"))==NULL) {
		perror("perror:\n");
		fprintf(stderr, "FATAL ERROR: unable to fopen file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if(fread(&cyberspace_server_data, sizeof(cyberspace_server_data), 1, file)!=1) {
		fprintf(stderr, "FATAL ERROR: unable to fread file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if(fclose(file)!=0) {
		fprintf(stderr, "FATAL ERROR: unable to fclose file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	printf("main: server_data read\n");

	delete[] path;

	cyberspace_mutex_server_data_sdl=SDL_CreateMutex();

/*
	pthread_attr_t thread_detached;
	pthread_attr_init(&thread_detached);
	pthread_attr_setdetachstate(&thread_detached, PTHREAD_CREATE_DETACHED);
*/


/*
	pthread_t physical_engine;
	if(pthread_create(&physical_engine, &thread_detached, cyberspace_physical_engine_tf, NULL)!=0) {
		fprintf(stderr, "FATAL ERROR: physical_engine thread creation fails.\n");
		exit(EXIT_FAILURE);
		}
*/
	SDL_Thread* physical_engine_thread_sdl;
	physical_engine_thread_sdl=SDL_CreateThread(cyberspace_physical_engine_tf_sdl, NULL);
	if(physical_engine_thread_sdl==NULL) {
		fprintf(stderr, "FATAL ERROR: physical_engine thread creation fails: %s.\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}


/*
	pthread_t frontdoor;
	if(pthread_create(&frontdoor, &thread_detached, cyberspace_frontdoor_tf, NULL)!=0) {
		fprintf(stderr, "FATAL ERROR: frontdoor thread creation fails.\n");
		exit(EXIT_FAILURE);
		}
*/
	SDL_Thread* frontdoor_thread_sdl;
	frontdoor_thread_sdl=SDL_CreateThread(cyberspace_frontdoor_tf_sdl, NULL);
	if(frontdoor_thread_sdl==NULL) {
		fprintf(stderr, "FATAL ERROR: frontdoor thread creation fails: %s.\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}


/*
	pthread_t frontdoor_inputchannel;
	if(pthread_create(&frontdoor_inputchannel, &thread_detached, cyberspace_frontdoor_inputchannel_tf, NULL)!=0) {
		fprintf(stderr, "FATAL ERROR: frontdoor_inputchannel thread creation fails.\n");
		exit(EXIT_FAILURE);
		}
*/
	SDL_Thread* frontdoor_inputchannel_thread_sdl;
	frontdoor_inputchannel_thread_sdl=SDL_CreateThread(cyberspace_frontdoor_inputchannel_tf_sdl, NULL);
	if(frontdoor_inputchannel_thread_sdl==NULL) {
		fprintf(stderr, "FATAL ERROR: frontdoor_inputchannel thread creation fails: %s.\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}

/*
	pthread_t statistics;
	if(pthread_create(&statistics, &thread_detached, cyberspace_statistics_tf, NULL)!=0) {
		fprintf(stderr, "FATAL ERROR: statistics thread creation fails.\n");
		exit(EXIT_FAILURE);
		}
*/
	SDL_Thread* statistics_thread_sdl;
	statistics_thread_sdl=SDL_CreateThread(cyberspace_statistics_tf_sdl, NULL);
	if(statistics_thread_sdl==NULL) {
		fprintf(stderr, "FATAL ERROR: statistics thread creation fails: %s.\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}

	SDL_WaitThread(physical_engine_thread_sdl, NULL);
	SDL_WaitThread(frontdoor_thread_sdl, NULL);
	SDL_WaitThread(frontdoor_inputchannel_thread_sdl, NULL);
	SDL_WaitThread(statistics_thread_sdl, NULL);

	printf("main: ENDING\n");
//	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
	}


/*
	int readpartial;
	int readtotal;
	char* lastparsetmp;
	char* parsetmp;
	int towrite;
	int writtenpartial;
	int writtentotal;
	while(true) {
//		printf("distr(0) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
		readtotal=0;
		while(readtotal==0 || distribution_server_soket_buffer[readtotal-1]!='|') {
//			printf("distr(01) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
			readpartial=recv(communication_socket_fd, distribution_server_soket_buffer+readtotal, DISTRIBUTION_SOCKET_BUFFER-1-readtotal, 0);
//			printf("distr(02) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
			if(readpartial==-1) {
				fprintf(stderr, "ERROR: (server) communication with distributed process failed.\n");
				exit(EXIT_FAILURE);
				}
//			printf("distr(03) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
			readtotal+=readpartial;
			}
//		printf("distr(04) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
		distribution_server_soket_buffer[readtotal-1]='\0';
//		printf("distr(1) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
		if(DEBUG_OUTPUTLEVEL<=0) fprintf(stderr, "OK: (server) \"%s\".\n", distribution_server_soket_buffer);

		if(distribution_server_soket_buffer[0]=='~') {
			pthread_mutex_lock(&distribution_mutex_sync);
//			printf("from status=%d (ack=%d) (tosync=%d)\n", distribution_sync_status, distribution_sync, distribution_sync_tosync);
			switch(distribution_sync_status) {
					case DISTRIBUTION_SYNC_STATUS_NOTHING:
						distribution_sync_status=DISTRIBUTION_SYNC_STATUS_SYNC;
//						distribution_sync=1;
						distribution_sync++;
						distribution_sync_tosync++;
						break;
					case DISTRIBUTION_SYNC_STATUS_SYNC:
						distribution_sync++;
						if(distribution_sync==DISTRIBUTION_NODES-1) {
							distribution_sync_status=DISTRIBUTION_SYNC_STATUS_COMMCLOSING;
							distribution_sync=0;
							}
						break;
					case DISTRIBUTION_SYNC_STATUS_COMMCLOSING:
						distribution_sync++;
						if(distribution_sync==DISTRIBUTION_NODES-1) {
							distribution_sync_status=DISTRIBUTION_SYNC_STATUS_COMMCLOSED;
							distribution_sync=0;
							}
						break;
					case DISTRIBUTION_SYNC_STATUS_COMMCLOSED:
						distribution_sync++;
						if(distribution_sync==DISTRIBUTION_NODES-1) {
							distribution_sync_status=DISTRIBUTION_SYNC_STATUS_ALLWRITTEN;
							distribution_sync=0;
							}
						break;
					case DISTRIBUTION_SYNC_STATUS_ALLWRITTEN:
						distribution_sync++;
						if(distribution_sync==DISTRIBUTION_NODES-1) {
							distribution_sync_status=DISTRIBUTION_SYNC_STATUS_ALLDONE;
							distribution_sync=0;
							}
						break;
					default:
						fprintf(stderr, "ERROR: unknown DISTRIBUTION_SYNC_STATUS %d.\n", distribution_sync_status);
						exit(EXIT_FAILURE);
						break;
				}
//			printf("to status=%d (ack=%d) (tosync=%d)\n", distribution_sync_status, distribution_sync, distribution_sync_tosync);
//			printf("distr(2) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
			pthread_mutex_unlock(&distribution_mutex_sync);
			if(snprintf(distribution_server_soket_buffer, DISTRIBUTION_SOCKET_BUFFER, "ack|")>=DISTRIBUTION_SOCKET_BUFFER) {
				fprintf(stderr, "ERROR: (server in writing) DISTRIBUTION_SOCKET_BUFFER overflow.\n");
				exit(EXIT_FAILURE);
				}
//			printf("distr(3) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
			}
		else {
//			printf("distr(4) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
			lastparsetmp=distribution_server_soket_buffer;

			parsetmp=strchr(lastparsetmp, '~');
			if(parsetmp-lastparsetmp>=HTML_HREF) {
				fprintf(stderr, "ERROR: (server) a read buffer overflow (%d):\n\"%s\"\nfrom\"%s\"", parsetmp-lastparsetmp, distribution_server_soket_buffer, lastparsetmp);
				exit(EXIT_FAILURE);
				}
			strncpy(distribution_string_host, lastparsetmp, parsetmp-lastparsetmp);
			distribution_string_host[parsetmp-lastparsetmp]='\0';
			lastparsetmp=parsetmp+1;
	
			parsetmp=strchr(lastparsetmp, '~');
			if(parsetmp-lastparsetmp>=HTML_HREF) {
				fprintf(stderr, "ERROR: (server) a read buffer overflow (%d):\n\"%s\"\nfrom\"%s\"", parsetmp-lastparsetmp, distribution_server_soket_buffer, lastparsetmp);
				exit(EXIT_FAILURE);
				}
			strncpy(distribution_string_url, lastparsetmp, parsetmp-lastparsetmp);
			distribution_string_url[parsetmp-lastparsetmp]='\0';
			lastparsetmp=parsetmp+1;
	
			parsetmp=strchr(lastparsetmp, '~');
			if(parsetmp-lastparsetmp>=1000) {
				fprintf(stderr, "ERROR: (server) a read buffer overflow (%d):\n\"%s\"\nfrom\"%s\"", parsetmp-lastparsetmp, distribution_server_soket_buffer, lastparsetmp);
				exit(EXIT_FAILURE);
				}
			strncpy(distribution_depth, lastparsetmp, parsetmp-lastparsetmp);
			distribution_depth[parsetmp-lastparsetmp]='\0';
			lastparsetmp=parsetmp+1;

			parsetmp=strchr(lastparsetmp, '~');
			if(parsetmp-lastparsetmp>=1000) {
				fprintf(stderr, "ERROR: (server) a read buffer overflow (%d):\n\"%s\"\nfrom\"%s\"", parsetmp-lastparsetmp, distribution_server_soket_buffer, lastparsetmp);
				exit(EXIT_FAILURE);
				}
			strncpy(distribution_intradepth, lastparsetmp, parsetmp-lastparsetmp);
			distribution_intradepth[parsetmp-lastparsetmp]='\0';
			lastparsetmp=parsetmp+1;

//			printf("distr(5) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
	
			parsetmp=strchr(lastparsetmp, '\0');
//			printf("distr(50) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
			if(parsetmp-lastparsetmp!=1) {
				fprintf(stderr, "ERROR: (server) a read buffer overflow (%d):\n\"%s\"\nfrom\"%s\"", parsetmp-lastparsetmp, distribution_server_soket_buffer, lastparsetmp);
				exit(EXIT_FAILURE);
				}
//			printf("distr(51) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
			strncpy(distribution_ssl, lastparsetmp, parsetmp-lastparsetmp);
//			printf("distr(52) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
			distribution_ssl[parsetmp-lastparsetmp]='\0';
//			printf("distr(53) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
			lastparsetmp=parsetmp+1;

//			printf("distr(6) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);

			distribution_res=store_getorcreate_url_id(distribution_calling_id_host, distribution_string_host, distribution_string_url, atoi(distribution_depth), atoi(distribution_intradepth), (distribution_ssl[0]=='t'), true);

//			printf("distr(7) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);

			if(snprintf(distribution_server_soket_buffer, DISTRIBUTION_SOCKET_BUFFER, "%d~%d~%d|", distribution_res.id_jurisdiction, distribution_res.id_host, distribution_res.id_url)>=DISTRIBUTION_SOCKET_BUFFER) {
				fprintf(stderr, "ERROR: (server in writing) DISTRIBUTION_SOCKET_BUFFER overflow.\n");
				exit(EXIT_FAILURE);
				}
//			printf("distr(8) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
			}

		towrite=strlen(distribution_server_soket_buffer);
		writtentotal=0;
		while(towrite>writtentotal) {
			writtenpartial=send(communication_socket_fd, distribution_server_soket_buffer+writtentotal, towrite-writtentotal, 0);
			if(writtenpartial==-1) {
				fprintf(stderr, "ERROR: (server in writing) communication with distributed process failed.\n");
				exit(EXIT_FAILURE);
				}
			writtentotal+=writtenpartial;
			}
//		printf("distr(9) store_storageBaseLocation=\"%s\"\n", store_storageBaseLocation);
		}
	fprintf(stderr, "IMPOSSIBLE ERROR: (server) !!!.\n");
	exit(EXIT_FAILURE);
	pthread_exit(NULL);
	}
*/

