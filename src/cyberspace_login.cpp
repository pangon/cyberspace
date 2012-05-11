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


//TODO controllare che un utente non si logghi piu' volte
#include "cyberspace_login.h"


void cyberspace_login(cyberspace_user_connection_data* data) {
	if(data->phase!=CYBERSPACE_PHASE_LOGIN) {
		fprintf(stderr, "ERROR: data->phase!=CYBERSPACE_PHASE_LOGIN (%d).\n", data->phase);
		exit(EXIT_FAILURE);
		}

	cyberspace_world_world* world;
	char* errormessage;
	char* login;
	char* password;
	int already_connected;

	FILE* file;
	char* path;

	switch(data->in_command.type) {
		case CYBERSPACE_COMMAND_USER_LOGIN:
			login=new char[21];
			password=new char[21];
			strcpy(login, (char*)(data->in_command.data));
			strcpy(password, ((char*)(data->in_command.data))+((strlen(login)+1)*sizeof(char)));

			path=new char[100];

			sprintf(path, "accounts/%s.now", login);
			if((file=fopen(path, "r+"))==NULL) {
				delete[] login;
				delete[] password;
				delete[] path;
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "nick inesistente");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			if(fread(&(data->personal), sizeof(data->personal), 1, file)!=1) {
				fprintf(stderr, "FATAL ERROR: unable to fread file \"%s\"\n", path);
				exit(EXIT_FAILURE);
				}
			if(fclose(file)!=0) {
				fprintf(stderr, "FATAL ERROR: unable to fclose file \"%s\"\n", path);
				exit(EXIT_FAILURE);
				}

			delete[] path;
			delete[] login;

			if(strcmp(password, data->personal.password)!=0) {
				delete[] password;
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "password errata");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}

			delete[] password;

			already_connected=0;
			for(int i=0;i<cyberspace_user_connected;i++) {
				if(cyberspace_user_connection_datas[i].personal.id_static==data->personal.id_static) {
					already_connected++;
					if(already_connected>1) break;
					}
				}

			if(already_connected>1) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "risulti gia' connesso");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}

			for(int sys=0;sys<CYBERSPACE_NEURAL_SYSTEM_MAX;sys++) {
				data->personal.systems_loaded[sys]=0;
				for(int subsys=0;subsys<CYBERSPACE_NEURAL_SUBSYSTEM_MAX;subsys++) {
					data->personal.subsystems_loaded[sys][subsys]=0;
					}
				}

			data->out_command.type=CYBERSPACE_COMMAND_SERVER_USER;
			memcpy(data->out_command.data, &(data->personal), sizeof(data->personal));
			cyberspace_user_connection_output(data, &(data->out_command));

			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_3D;
			cyberspace_user_connection_output(data, &(data->out_command));

			data->world_id=0;
			data->need_to_load_world=true;
			data->movement.rotation=0;
			data->movement.traslation=0;
			data->movement.strafe=false;

			SDL_mutexP(cyberspace_mutex_universe_sdl[data->world_id]);
//			pthread_mutex_lock(&cyberspace_mutex_universe[data->world_id]);
			world=&(cyberspace_universe[data->world_id]);

			if(world->dynamic_size>=CYBERSPACE_WORLD_DYNAMIC_OBJECTS_MAX) {
				fprintf(stderr, "FATAL ERROR: world->dynamic_size>=CYBERSPACE_WORLD_DYNAMIC_OBJECTS_MAX (%d in world %d).\n", world->dynamic_size, data->world_id);
				exit(EXIT_FAILURE);
				}

			data->obj=&(world->dynamic_obj[world->dynamic_size++]);
			data->obj->type=CYBERSPACE_WORLD_OBJECT_USER;
			data->obj->parameters[CYBERSPACE_WORLD_OBJECT_USER_PARAMETER_ID_STATIC]=(double)(data->personal.id_static);
			data->obj->parameters[CYBERSPACE_WORLD_OBJECT_USER_PARAMETER_ID_DYNAMIC]=(double)(data->id_dynamic);
			data->obj->point.position.x=300.0;
			data->obj->point.position.y=0.0;
			data->obj->point.position.z=400.0;
			data->obj->point.angle.alpha=M_PI*0.5;
			data->obj->point.angle.beta=0.0;
			data->obj->point.angle.gamma=0.0;
			data->falling=false;
			data->falling_speed=0.0;
			data->jumping=false;
			data->jumping_speed=0.0;

//			world->users[world->users_size]=data->id_static;
//			world->users_size++;
			SDL_mutexV(cyberspace_mutex_universe_sdl[data->world_id]);
//			pthread_mutex_unlock(&cyberspace_mutex_universe[data->world_id]);

			data->phase=CYBERSPACE_PHASE_3D;
			break;
		default:
			fprintf(stderr, "WARNING: unexpected message in login (%d) by connection %d (%d).\n", data->in_command.type, data->id_dynamic, data->personal.id_static);
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
/*			data->out_command.include_data_s=true;
			data->out_command.include_data_m=false;
			data->out_command.include_data_l=false;
			char* errormessage=new char[CYBERSPACE_COMMAND_SIZE_S];
			sprintf(errormessage, "unexpected message in login");
			memcpy(data->out_command.data_s, errormessage, strlen(errormessage)+1);*/
			char* errormessage=new char[100];
			sprintf(errormessage, "unexpected message in login");
			memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		}

	}

