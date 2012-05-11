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


#include "cyberspace_neural_connector_output.h"



void cyberspace_neural_connector_output(cyberspace_user_command* command) {
	int memcpy_size_partial;
	int memcpy_size_total;

	memcpy_size_total=sizeof(memcpy_size_total);

	memcpy_size_partial=sizeof(cyberspace_user_command);
	memcpy(cyberspace_write_buffer+memcpy_size_total, command, memcpy_size_partial);
	memcpy_size_total+=memcpy_size_partial;

	memcpy_size_partial=sizeof(memcpy_size_total);
	memcpy(cyberspace_write_buffer, &memcpy_size_total, memcpy_size_partial);

	cyberspace_to_write=memcpy_size_total;

	if(SDLNet_TCP_Send(cyberspace_output_socket, cyberspace_write_buffer, cyberspace_to_write)<cyberspace_to_write) {
		fprintf(stderr, "FATAL ERROR: SDLNet_TCP_Send: %s.\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
		}

/*
	cyberspace_written_total=0;
	while(cyberspace_to_write>cyberspace_written_total) {
//		printf("sending\n");
		SDL_Delay(100);
		cyberspace_written_partial=send(cyberspace_output_socket_fd, cyberspace_write_buffer+cyberspace_written_total, cyberspace_to_write-cyberspace_written_total, 0);
//		printf("sendt (%d)\n", cyberspace_written_partial);
//		perror("perror: ");
//		SDL_Delay(100);
		if(cyberspace_written_partial==-1) {
//			printf("error\n");
//			SDL_Delay(100);
			perror("perror: ");
			fprintf(stderr, "ERROR: (in connector_output) communication with server failed.\n");
			exit(EXIT_FAILURE);
			}
		if(cyberspace_written_partial==0) {
//			printf("error\n");
//			SDL_Delay(100);
			perror("perror: ");
			fprintf(stderr, "ERROR: (in connector_output) communication with server failed.\n");
			exit(EXIT_FAILURE);
			}
//		printf("noerror\n");
		cyberspace_written_total+=cyberspace_written_partial;
		}
*/
	}

