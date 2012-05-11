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


#include "cyberspace_3D.h"


int cyberspace_physical_engine_tf_sdl(void* nothing) {
	cyberspace_physical_engine_tf(nothing);
	return(0);
	}

void* cyberspace_physical_engine_tf(void* nothing) {
	printf("physical engine: STARTED\n");

	cyberspace_world_angle baseangle;
	baseangle.alpha=0.0;
	baseangle.beta=0.0;
	baseangle.gamma=0.0;

	cyberspace_universe=new cyberspace_world_world[3];
//	cyberspace_mutex_universe=new pthread_mutex_t[3];
	cyberspace_mutex_universe_sdl=new SDL_mutex*[3];

//	pthread_mutexattr_t mutex_attr;
//	pthread_mutexattr_init(&mutex_attr);
//	for(int i=0;i<3;i++) pthread_mutex_init(&(cyberspace_mutex_universe[i]), &mutex_attr);
	for(int i=0;i<3;i++) cyberspace_mutex_universe_sdl[i]=SDL_CreateMutex();

	int towrite;

	cyberspace_server_command static_message[3];
	int static_data_wrote[3];
	for(int i=0;i<3;i++) static_message[i].type=CYBERSPACE_COMMAND_SERVER_WORLD_LOAD;

	cyberspace_server_command sync_message[3];
	int sync_data_wrote[3];
	for(int i=0;i<3;i++) sync_message[i].type=CYBERSPACE_COMMAND_SERVER_WORLD_SYNC;

	cyberspace_world_world* world;
	cyberspace_world_object* obj;
	cyberspace_server_command* message;
	int* message_wrote;

	FILE* world_file;
	char world_file_name[100];
	cyberspace_world_object* vector;

	for(int world_id=0;world_id<3;world_id++) {
		SDL_mutexP(cyberspace_mutex_universe_sdl[world_id]);
//		pthread_mutex_lock(&cyberspace_mutex_universe[world_id]);
		world=&(cyberspace_universe[world_id]);

		sprintf(world_file_name, "worlds/%d.world", world_id);
		world_file=fopen(world_file_name, "r");
		if(world_file==NULL) {
			fprintf(stderr, "cannot open file \"%s\".\n", world_file_name);
			exit(EXIT_FAILURE);
			}

		printf("loading world %d\n", world_id);
		cyberspace_world_loader(world_file, world);

		if(world_id==0) {
			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD;
			world->static_obj[world->static_size].point.position.x=2450.0;
			world->static_obj[world->static_size].point.position.y=-200.0;
			world->static_obj[world->static_size].point.position.z=0.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_WORLD]=1.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_X]=0.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Y]=0.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Z]=0.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_SKIN]=1.0;
			world->static_size++;

			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD;
			world->static_obj[world->static_size].point.position.x=2450.0;
			world->static_obj[world->static_size].point.position.y=-200.0;
			world->static_obj[world->static_size].point.position.z=750.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_WORLD]=1.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_X]=0.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Y]=0.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Z]=0.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_SKIN]=1.0;
			world->static_size++;

			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD;
			world->static_obj[world->static_size].point.position.x=2450.0;
			world->static_obj[world->static_size].point.position.y=-200.0;
			world->static_obj[world->static_size].point.position.z=1500.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_WORLD]=1.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_X]=0.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Y]=0.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Z]=0.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_SKIN]=1.0;
			world->static_size++;

			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD;
			world->static_obj[world->static_size].point.position.x=1800.0;
			world->static_obj[world->static_size].point.position.y=-200.0;
			world->static_obj[world->static_size].point.position.z=1100.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_obj[world->static_size].point.angle.alpha=M_PI/3.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_WORLD]=0.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_X]=650.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Y]=10.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Z]=300.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_SKIN]=0.0;
			world->static_size++;

			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD;
			world->static_obj[world->static_size].point.position.x=175.0;
			world->static_obj[world->static_size].point.position.y=100.0;
			world->static_obj[world->static_size].point.position.z=700.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_obj[world->static_size].point.angle.alpha=M_PI/6.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_WORLD]=1.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_X]=300.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Y]=110.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Z]=400.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_SKIN]=1.0;
			world->static_size++;

			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD;
			world->static_obj[world->static_size].point.position.x=875.0;
			world->static_obj[world->static_size].point.position.y=0.0;
			world->static_obj[world->static_size].point.position.z=275.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_obj[world->static_size].point.angle.alpha=M_PI/8.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_WORLD]=0.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_X]=200.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Y]=110.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Z]=1100.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_SKIN]=0.0;
			world->static_size++;

			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_CONSOLE;
			world->static_obj[world->static_size].point.position.x=110.0;
			world->static_obj[world->static_size].point.position.y=0.0;
			world->static_obj[world->static_size].point.position.z=750.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_obj[world->static_size].point.angle.alpha=M_PI/2.0;
			world->static_size++;

			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_CONSOLE;
			world->static_obj[world->static_size].point.position.x=250.0;
			world->static_obj[world->static_size].point.position.y=0.0;
			world->static_obj[world->static_size].point.position.z=1240.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_size++;

			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_CONSOLE;
			world->static_obj[world->static_size].point.position.x=1090.0;
			world->static_obj[world->static_size].point.position.y=0.0;
			world->static_obj[world->static_size].point.position.z=350.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_obj[world->static_size].point.angle.alpha=M_PI/2.0;
			world->static_size++;

			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_CONSOLE;
			world->static_obj[world->static_size].point.position.x=1225.0;
			world->static_obj[world->static_size].point.position.y=0.0;
			world->static_obj[world->static_size].point.position.z=210.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_size++;

			world->dynamic_obj[world->dynamic_size].type=CYBERSPACE_WORLD_OBJECT_VECTOR;
			world->dynamic_obj[world->dynamic_size].point.position.x=100.0;
			world->dynamic_obj[world->dynamic_size].point.position.y=0.0;
			world->dynamic_obj[world->dynamic_size].point.position.z=100.0;
			world->dynamic_obj[world->dynamic_size].point.angle=baseangle;
			world->dynamic_obj[world->dynamic_size].point.angle.alpha=0.0;
			world->dynamic_obj[world->dynamic_size].parameters[CYBERSPACE_WORLD_OBJECT_VECTOR_PARAMETER_LENGTH]=50.0;
			vector=&(world->dynamic_obj[world->dynamic_size]);
			world->dynamic_size++;

/*			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_CUBE;
			world->static_obj[world->static_size].point.position.x=250.0;
			world->static_obj[world->static_size].point.position.y=0.0;
			world->static_obj[world->static_size].point.position.z=250.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_size++;*/

/*			world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_REDLINE;
			world->static_obj[world->static_size].point.position.x=100.0;
			world->static_obj[world->static_size].point.position.y=0.0;
			world->static_obj[world->static_size].point.position.z=150.0;
			world->static_obj[world->static_size].point.angle=baseangle;
			world->static_obj[world->static_size].point.angle.alpha=M_PI/4.0;
			world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_REDLINE_PARAMETER_LENGTH]=100.0;
			world->red_line[world->red_line_size].p0.x=world->static_obj[world->static_size].point.position.x+cos(world->static_obj[world->static_size].point.angle.alpha)*world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_REDLINE_PARAMETER_LENGTH]*-0.5;
			world->red_line[world->red_line_size].p0.z=world->static_obj[world->static_size].point.position.z+sin(world->static_obj[world->static_size].point.angle.alpha)*world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_REDLINE_PARAMETER_LENGTH]*-0.5;
			world->red_line[world->red_line_size].p1.x=world->static_obj[world->static_size].point.position.x+cos(world->static_obj[world->static_size].point.angle.alpha)*world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_REDLINE_PARAMETER_LENGTH]*0.5;
			world->red_line[world->red_line_size].p1.z=world->static_obj[world->static_size].point.position.z+sin(world->static_obj[world->static_size].point.angle.alpha)*world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_REDLINE_PARAMETER_LENGTH]*0.5;
			world->red_line_size++;
			world->static_size++;*/
			}
//		pthread_mutex_unlock(&cyberspace_mutex_universe[world_id]);
		SDL_mutexV(cyberspace_mutex_universe_sdl[world_id]);
		}

	//writing static world message
	for(int world_id=0;world_id<3;world_id++) {
		SDL_mutexP(cyberspace_mutex_universe_sdl[world_id]);
//		pthread_mutex_lock(&cyberspace_mutex_universe[world_id]);
		world=&(cyberspace_universe[world_id]);
		message=&(static_message[world_id]);
		message_wrote=&(static_data_wrote[world_id]);
		*message_wrote=0;
		towrite=sizeof(world->static_size);
		if((*message_wrote+towrite)>=CYBERSPACE_COMMAND_SIZE_L) {
			fprintf(stderr, "FATAL ERROR: writing more than %d in static world %d message\n", CYBERSPACE_COMMAND_SIZE_L, world_id);
			exit(EXIT_FAILURE);
			}
		memcpy(message->data+(*message_wrote), &(world->static_size), towrite);
		*message_wrote+=towrite;
		for(int o=0;o<world->static_size;o++) {
			obj=&(world->static_obj[o]);
			towrite=sizeof(*obj);
			if((*message_wrote+towrite)>=CYBERSPACE_COMMAND_SIZE_L) {
				fprintf(stderr, "FATAL ERROR: writing more than %d in static world %d message\n", CYBERSPACE_COMMAND_SIZE_L, world_id);
				exit(EXIT_FAILURE);
				}
			memcpy(message->data+(*message_wrote), obj, towrite);
			*message_wrote+=towrite;
			}
//		pthread_mutex_unlock(&cyberspace_mutex_universe[world_id]);
		SDL_mutexV(cyberspace_mutex_universe_sdl[world_id]);
		}

	cyberspace_user_connection_data* user;
	cyberspace_user_movement* move;
	cyberspace_world_floor* floor;

	struct timeval start_t;
	struct timeval tmp_t;
	struct timeval lastite_t;
	struct timeval nextite_t;
	struct timeval tosleep_t;
	struct timeval frequency_t;

	frequency_t.tv_sec=0;
	frequency_t.tv_usec=30000;

	int tics=0;
	int ticfrequency=33*5;
	int dice_skill;
	bool learned_skill;
	int sys_skill;
	int subsys_skill;

	struct timespec tosleep;
	struct timespec tosleepagain;

	if(gettimeofday(&start_t, NULL)!=0) {
		fprintf(stderr, "FATAL ERROR: gettimeofday fails.\n");
		exit(EXIT_FAILURE);
		}
	lastite_t=start_t;

	cyberspace_world_vector v;
//	double cd_A, cd_B, cd_C, cd_D;
	double collision;
	double collision_min;
//	cyberspace_world_object* obj_collider;
	cyberspace_world_red_line* red_line;
	cyberspace_world_point* p_i;
	cyberspace_world_point* p_a;
	cyberspace_world_point* p_b;
	cyberspace_world_point p_a2;
	cyberspace_world_point p_b2;
	double V_x, V_z;
	double perp_x, perp_z;
	double L;
	double tangent;
	double tangent_min;
	double reduction;
	double reduction_min;
	cyberspace_world_point bar;
	double f_A, f_B, f_C, f_D, f_E;
	double bar_t;
	double falling_h;
	double jumping_h;
	double min_y, max_y;
	double delta;

	int milliseconds;
	int seconds;

	char* errormessage;

	while(true) {

		tics++;
		//frequency
		timeradd(&lastite_t, &frequency_t, &nextite_t);

		if(gettimeofday(&tmp_t, NULL)!=0) {
			fprintf(stderr, "FATAL ERROR: gettimeofday fails.\n");
			exit(EXIT_FAILURE);
			}

		if(tmp_t.tv_sec<nextite_t.tv_sec || (tmp_t.tv_sec==nextite_t.tv_sec && tmp_t.tv_usec<nextite_t.tv_usec)) {
			timersub(&nextite_t, &tmp_t, &tosleep_t);
			tosleep.tv_sec=tosleep_t.tv_sec;
			tosleep.tv_nsec=tosleep_t.tv_usec*1000;
			while(nanosleep(&tosleep, &tosleepagain)!=0) {
				tosleep=tosleepagain;
				}
			milliseconds=1000*frequency_t.tv_sec+frequency_t.tv_usec/1000;
			}
		else {
			milliseconds=1000*(tmp_t.tv_sec-nextite_t.tv_sec)+(tmp_t.tv_usec-nextite_t.tv_usec)/1000;
			}

		if(gettimeofday(&lastite_t, NULL)!=0) {
			fprintf(stderr, "FATAL ERROR: gettimeofday fails.\n");
			exit(EXIT_FAILURE);
			}

		//engine
		for(int world_id=0;world_id<3;world_id++) {
			SDL_mutexP(cyberspace_mutex_universe_sdl[world_id]);
//			pthread_mutex_lock(&cyberspace_mutex_universe[world_id]);
			world=&(cyberspace_universe[world_id]);
			for(int o=0;o<world->dynamic_size;o++) {
				obj=&(world->dynamic_obj[o]);
				if(obj->type!=CYBERSPACE_WORLD_OBJECT_USER) continue;
				user=&(cyberspace_user_connection_datas[(int)(obj->parameters[CYBERSPACE_WORLD_OBJECT_USER_PARAMETER_ID_DYNAMIC])]);
				//here and in communication
				if(user->phase!=CYBERSPACE_PHASE_3D && user->phase!=CYBERSPACE_PHASE_PROGRAMMING && user->phase!=CYBERSPACE_PHASE_MINIGAME_TESTING && user->phase!=CYBERSPACE_PHASE_MINIGAME_HEURISTIC && user->phase!=CYBERSPACE_PHASE_MINIGAME_FAST && user->phase!=CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING) {
					fprintf(stderr, "FATAL ERROR: user %d (%d) not in 3D but with 3D presence\n", user->id_dynamic, user->personal.id_static);
					exit(EXIT_FAILURE);
					}
				if(user->world_id!=world_id) {
					fprintf(stderr, "FATAL ERROR: user %d (%d) in world %d but with 3D presence in world %d\n", user->id_dynamic, user->personal.id_static, user->world_id, world_id);
					exit(EXIT_FAILURE);
					}
				move=&(user->movement);
				obj->point.angle.alpha+=0.05*move->rotation;
				if(obj->point.angle.alpha<0.0) obj->point.angle.alpha+=2.0*M_PI;
				else if(obj->point.angle.alpha>=2.0*M_PI) obj->point.angle.alpha-=2.0*M_PI;

				v.module=1.0*move->traslation;
				v.module*=5.0;

				if(user->falling) {
					if(user->falling_speed<10.0) user->falling_speed+=CYBERSPACE_WORLD_GRAVITY;
					falling_h=user->falling_speed;
					}
				else falling_h=0.0;

				if(user->jumping) {
					user->jumping_speed-=CYBERSPACE_WORLD_GRAVITY;
					if(user->jumping_speed<=0.0) {
						user->jumping=false;
						user->jumping_speed=0.0;
						jumping_h=0.0;
						}
					else jumping_h=user->jumping_speed;
					}
				else jumping_h=0.0;

				min_y=obj->point.position.y;
				max_y=obj->point.position.y+CYBERSPACE_WORLD_OBJECT_USER_H;
				if(falling_h>0.0) min_y-=falling_h;
				else if(jumping_h>0.0) max_y+=jumping_h;

				if(v.module==0.0) goto FLOOR_CHECK;

				v.alpha=obj->point.angle.alpha;
				v.beta=0.0;

				if(v.module<0.0) {
					v.module*=-1.0;
					v.alpha+=M_PI;
					if(v.alpha>=2.0*M_PI) v.alpha-=2.0*M_PI;
					}

				//collision detection
				//ora semplificato con red lines
				//utente un punto spostato di v
				//red line estesa di R

				collision_min=2.0*v.module;

				p_i=&(obj->point.position);
				V_x=cos(v.alpha);
				V_z=sin(v.alpha);

				for(int line_id=0;line_id<world->red_line_size;line_id++) {
					red_line=&(world->red_line[line_id]);
					if(red_line->p0.y>=max_y || red_line->p1.y<=min_y) continue;
					L=sqrt((red_line->p1.z-red_line->p0.z)*(red_line->p1.z-red_line->p0.z)+(red_line->p1.x-red_line->p0.x)*(red_line->p1.x-red_line->p0.x));

					perp_x=(red_line->p1.z-red_line->p0.z)*(CYBERSPACE_WORLD_OBJECT_USER_R/L);
					perp_z=(red_line->p1.x-red_line->p0.x)*(CYBERSPACE_WORLD_OBJECT_USER_R/L);

					p_a2=red_line->p0;
					p_b2=red_line->p1;
					p_a2.x+=perp_x;
					p_a2.z-=perp_z;
					p_b2.x+=perp_x;
					p_b2.z-=perp_z;
					p_a=&p_a2;
					p_b=&p_b2;
					collision=cyberspace_3D_collision_line_segment(p_i, V_x, V_z, p_a, p_b, v.alpha, &tangent, &reduction);
					if(collision>0 && collision<collision_min) {
						collision_min=collision;
						tangent_min=tangent;
						reduction_min=reduction;
						}

					p_a2=red_line->p0;
					p_b2=red_line->p1;
					p_a2.x-=perp_x;
					p_a2.z+=perp_z;
					p_b2.x-=perp_x;
					p_b2.z+=perp_z;
					p_a=&p_a2;
					p_b=&p_b2;
					collision=cyberspace_3D_collision_line_segment(p_i, V_x, V_z, p_a, p_b, v.alpha, &tangent, &reduction);
					if(collision>0 && collision<collision_min) {
						collision_min=collision;
						tangent_min=tangent;
						reduction_min=reduction;
						}

					p_a=&(red_line->p0);
					collision=cyberspace_3D_collision_line_circle(p_i, V_x, V_z, p_a, v.alpha, &tangent, &reduction);
					if(collision>0 && collision<collision_min) {
						collision_min=collision;
						tangent_min=tangent;
						reduction_min=reduction;
						}

					p_a=&(red_line->p1);
					collision=cyberspace_3D_collision_line_circle(p_i, V_x, V_z, p_a, v.alpha, &tangent, &reduction);
					if(collision>0 && collision<collision_min) {
						collision_min=collision;
						tangent_min=tangent;
						reduction_min=reduction;
						}
					}

				if(collision_min<v.module) {
					if(collision_min>0.01) {
						collision_min-=0.01;
						obj->point.position.x+=cos(v.alpha)*collision_min;
						obj->point.position.z+=sin(v.alpha)*collision_min;
						}

					v.module-=collision_min;
					v.alpha=tangent_min;
					v.module*=reduction_min;
					V_x=cos(v.alpha);
					V_z=sin(v.alpha);
					collision_min=2.0*v.module;

					vector->point.position=obj->point.position;
					vector->point.angle.alpha=v.alpha;
					vector->point.angle.beta=0.0;
					vector->point.angle.gamma=0.0;

					for(int line_id2=0;line_id2<world->red_line_size;line_id2++) {
						red_line=&(world->red_line[line_id2]);
						if(red_line->p0.y>=max_y || red_line->p1.y<=min_y) continue;
						L=sqrt((red_line->p1.z-red_line->p0.z)*(red_line->p1.z-red_line->p0.z)+(red_line->p1.x-red_line->p0.x)*(red_line->p1.x-red_line->p0.x));

						perp_x=(red_line->p1.z-red_line->p0.z)*(CYBERSPACE_WORLD_OBJECT_USER_R/L);
						perp_z=(red_line->p1.x-red_line->p0.x)*(CYBERSPACE_WORLD_OBJECT_USER_R/L);

						p_a2=red_line->p0;
						p_b2=red_line->p1;
						p_a2.x+=perp_x;
						p_a2.z-=perp_z;
						p_b2.x+=perp_x;
						p_b2.z-=perp_z;
						p_a=&p_a2;
						p_b=&p_b2;
						collision=cyberspace_3D_collision_line_segment(p_i, V_x, V_z, p_a, p_b, v.alpha, &tangent, &reduction);
						if(collision>0 && collision<collision_min) {
							collision_min=collision;
							tangent_min=tangent;
							reduction_min=reduction;
							}

						p_a2=red_line->p0;
						p_b2=red_line->p1;
						p_a2.x-=perp_x;
						p_a2.z+=perp_z;
						p_b2.x-=perp_x;
						p_b2.z+=perp_z;
						p_a=&p_a2;
						p_b=&p_b2;
						collision=cyberspace_3D_collision_line_segment(p_i, V_x, V_z, p_a, p_b, v.alpha, &tangent, &reduction);
						if(collision>0 && collision<collision_min) {
							collision_min=collision;
							tangent_min=tangent;
							reduction_min=reduction;
							}

						p_a=&(red_line->p0);
						collision=cyberspace_3D_collision_line_circle(p_i, V_x, V_z, p_a, v.alpha, &tangent, &reduction);
						if(collision>0 && collision<collision_min) {
							collision_min=collision;
							tangent_min=tangent;
							reduction_min=reduction;
							}

						p_a=&(red_line->p1);
						collision=cyberspace_3D_collision_line_circle(p_i, V_x, V_z, p_a, v.alpha, &tangent, &reduction);
						if(collision>0 && collision<collision_min) {
							collision_min=collision;
							tangent_min=tangent;
							reduction_min=reduction;
							}
						}

					if(collision_min<v.module) {
						if(collision_min>0.01) {
							collision_min-=0.01;
							obj->point.position.x+=cos(v.alpha)*collision_min;
							obj->point.position.z+=sin(v.alpha)*collision_min;
							}
						}
					else {
						obj->point.position.x+=cos(v.alpha)*v.module;
						obj->point.position.z+=sin(v.alpha)*v.module;
						}
					}
				else {
					obj->point.position.x+=cos(v.alpha)*v.module;
					obj->point.position.z+=sin(v.alpha)*v.module;
					vector->point=obj->point;
					}

FLOOR_CHECK:
				if(!user->jumping) user->falling=true;

				for(int f_id=0;f_id<world->floor_size && (user->falling || user->jumping || falling_h>0.0 || jumping_h>0.0);f_id++) {
					floor=&(world->floor[f_id]);

					if(user->jumping || jumping_h>0.0) {
						if(floor->p0.y<=min_y || floor->p0.y>=max_y) continue;
						}
					if(user->falling || falling_h>0.0) {
						if(floor->p0.y>=max_y || floor->p0.y<=min_y) continue;
						}

					bar.x=(floor->p0.x+floor->p1.x+floor->p2.x)/3.0;
					bar.z=(floor->p0.z+floor->p1.z+floor->p2.z)/3.0;
					if((bar.x==obj->point.position.x) && (bar.z==obj->point.position.z)) {
						if(user->falling || falling_h>0.0) {
							user->falling=false;
							user->falling_speed=0.0;
							if(falling_h>0.0) {
								falling_h=obj->point.position.y-floor->p0.y;
								min_y=obj->point.position.y-falling_h;
								}
							continue;
							}
						if(user->jumping || jumping_h>0.0) {
							user->jumping=false;
							user->jumping_speed=0.0;
							if(jumping_h>0.0) {
								jumping_h=floor->p0.y-(obj->point.position.y+CYBERSPACE_WORLD_OBJECT_USER_H);
								max_y=obj->point.position.y+CYBERSPACE_WORLD_OBJECT_USER_H+jumping_h;
								}
							continue;
							}
						}

					p_a=&(floor->p0);
					p_b=&(floor->p1);
					if((p_b->x-p_a->x)==0.0) {
						if((obj->point.position.x-bar.x)!=0.0) {
							bar_t=(p_b->x-bar.x)/(obj->point.position.x-bar.x);
							if(bar_t<1.0 && bar_t>=0.0) continue;
							}
						}
					else {
						f_A=(bar.x-p_a->x)/(p_b->x-p_a->x);
						f_B=(obj->point.position.x-bar.x)/(p_b->x-p_a->x);
						f_C=obj->point.position.z-bar.z;
						f_D=p_a->z-bar.z;
						f_E=p_b->z-p_a->z;
						if((f_C-f_E*f_B)!=0.0) {
							bar_t=(f_D+f_E*f_A)/(f_C-f_E*f_B);
							if(bar_t<1.0 && bar_t>=0.0) continue;
							}
						}

					p_a=&(floor->p1);
					p_b=&(floor->p2);
					if((p_b->x-p_a->x)==0.0) {
						if((obj->point.position.x-bar.x)!=0.0) {
							bar_t=(p_b->x-bar.x)/(obj->point.position.x-bar.x);
							if(bar_t<1.0 && bar_t>=0.0) continue;
							}
						}
					else {
						f_A=(bar.x-p_a->x)/(p_b->x-p_a->x);
						f_B=(obj->point.position.x-bar.x)/(p_b->x-p_a->x);
						f_C=obj->point.position.z-bar.z;
						f_D=p_a->z-bar.z;
						f_E=p_b->z-p_a->z;
						if((f_C-f_E*f_B)!=0.0) {
							bar_t=(f_D+f_E*f_A)/(f_C-f_E*f_B);
							if(bar_t<1.0 && bar_t>=0.0) continue;
							}
						}

					p_a=&(floor->p2);
					p_b=&(floor->p0);
					if((p_b->x-p_a->x)==0.0) {
						if((obj->point.position.x-bar.x)!=0.0) {
							bar_t=(p_b->x-bar.x)/(obj->point.position.x-bar.x);
							if(bar_t<1.0 && bar_t>=0.0) continue;
							}
						}
					else {
						f_A=(bar.x-p_a->x)/(p_b->x-p_a->x);
						f_B=(obj->point.position.x-bar.x)/(p_b->x-p_a->x);
						f_C=obj->point.position.z-bar.z;
						f_D=p_a->z-bar.z;
						f_E=p_b->z-p_a->z;
						if((f_C-f_E*f_B)!=0.0) {
							bar_t=(f_D+f_E*f_A)/(f_C-f_E*f_B);
							if(bar_t<1.0 && bar_t>=0.0) continue;
							}
						}

					if(user->falling || falling_h>0.0) {
						user->falling=false;
						user->falling_speed=0.0;
						if(falling_h>0.0) {
							falling_h=obj->point.position.y-floor->p0.y;
							min_y=obj->point.position.y-falling_h;
							}
						continue;
						}
					if(user->jumping || jumping_h>0.0) {
						user->jumping=false;
						user->jumping_speed=0.0;
						if(jumping_h>0.0) {
							jumping_h=floor->p0.y-(obj->point.position.y+CYBERSPACE_WORLD_OBJECT_USER_H);
							max_y=obj->point.position.y+CYBERSPACE_WORLD_OBJECT_USER_H+jumping_h;
							}
						continue;
						}
					}

				for(int line_id=0;line_id<world->red_line_size && (user->falling || user->jumping || falling_h>0.0 || jumping_h>0.0);line_id++) {
					red_line=&(world->red_line[line_id]);

					if(user->jumping || jumping_h>0.0) {
						if(red_line->p0.y<=min_y || red_line->p0.y>=max_y) continue;
						}
					if(user->falling || falling_h>0.0) {
						if(red_line->p1.y>=max_y || red_line->p1.y<=min_y) continue;
						}

					//assumo che distanza(red_line->p0 , red_line->p1)>0.0
					f_A=(red_line->p1.x-red_line->p0.x)*(red_line->p1.x-red_line->p0.x)+(red_line->p1.z-red_line->p0.z)*(red_line->p1.z-red_line->p0.z);
					f_B=2.0*((red_line->p1.x-red_line->p0.x)*(red_line->p0.x-obj->point.position.x)+(red_line->p1.z-red_line->p0.z)*(red_line->p0.z-obj->point.position.z));
					f_C=(red_line->p0.x-obj->point.position.x)*(red_line->p0.x-obj->point.position.x)+(red_line->p0.z-obj->point.position.z)*(red_line->p0.z-obj->point.position.z)-CYBERSPACE_WORLD_OBJECT_USER_R*CYBERSPACE_WORLD_OBJECT_USER_R;

					delta=f_B*f_B-4.0*f_A*f_C;
					if(delta<0.0) continue;
					bar_t=(-f_B+sqrt(delta))/(2.0*f_A);
					if(bar_t<0.0 || bar_t>1.0) {
						bar_t=(-f_B-sqrt(delta))/(2.0*f_A);
						if(bar_t<0.0 || bar_t>1.0) continue;
						}

					if(user->falling || falling_h>0.0) {
						user->falling=false;
						user->falling_speed=0.0;
						if(falling_h>0.0) {
							falling_h=obj->point.position.y-red_line->p1.y;
							min_y=obj->point.position.y-falling_h;
							}
						continue;
						}
					if(user->jumping || jumping_h>0.0) {
						user->jumping=false;
						user->jumping_speed=0.0;
						if(jumping_h>0.0) {
							jumping_h=red_line->p0.y-(obj->point.position.y+CYBERSPACE_WORLD_OBJECT_USER_H);
							max_y=obj->point.position.y+CYBERSPACE_WORLD_OBJECT_USER_H+jumping_h;
							}
						continue;
						}
					}

				if(falling_h>0.0) obj->point.position.y-=falling_h;
				else if(jumping_h>0.0) obj->point.position.y+=jumping_h;
				if(user->falling && falling_h==0.0) user->falling_speed=0.0;
				}
			SDL_mutexV(cyberspace_mutex_universe_sdl[world_id]);
//			pthread_mutex_unlock(&cyberspace_mutex_universe[world_id]);
			}

		//writing dynamic world message
		for(int world_id=0;world_id<3;world_id++) {
			SDL_mutexP(cyberspace_mutex_universe_sdl[world_id]);
//			pthread_mutex_lock(&cyberspace_mutex_universe[world_id]);
			world=&(cyberspace_universe[world_id]);
			message=&(sync_message[world_id]);
			message_wrote=&(sync_data_wrote[world_id]);
			*message_wrote=0;
			towrite=sizeof(world->dynamic_size);
			if((*message_wrote+towrite)>=CYBERSPACE_COMMAND_SIZE_L) {
				fprintf(stderr, "FATAL ERROR: writing more than %d in dynamic world %d message\n", CYBERSPACE_COMMAND_SIZE_L, world_id);
				exit(EXIT_FAILURE);
				}
			memcpy(message->data+(*message_wrote), &(world->dynamic_size), towrite);
			*message_wrote+=towrite;
			for(int o=0;o<world->dynamic_size;o++) {
				obj=&(world->dynamic_obj[o]);
				towrite=sizeof(*obj);
				if((*message_wrote+towrite)>=CYBERSPACE_COMMAND_SIZE_L) {
					fprintf(stderr, "FATAL ERROR: writing more than %d in dynamic world %d message\n", CYBERSPACE_COMMAND_SIZE_L, world_id);
					exit(EXIT_FAILURE);
					}
				memcpy(message->data+(*message_wrote), obj, towrite);
				*message_wrote+=towrite;
				}
			SDL_mutexV(cyberspace_mutex_universe_sdl[world_id]);
//			pthread_mutex_unlock(&cyberspace_mutex_universe[world_id]);
			}

		//communication
		for(int i=0;i<cyberspace_user_connected;i++) {
			user=&(cyberspace_user_connection_datas[i]);
			if(user->phase!=CYBERSPACE_PHASE_3D && user->phase!=CYBERSPACE_PHASE_PROGRAMMING && user->phase!=CYBERSPACE_PHASE_MINIGAME_TESTING && user->phase!=CYBERSPACE_PHASE_MINIGAME_HEURISTIC && user->phase!=CYBERSPACE_PHASE_MINIGAME_FAST && user->phase!=CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING) continue;
//			if(user->world_id!=world_id) continue;
			world=&(cyberspace_universe[user->world_id]);
			if(user->need_to_load_world) {
				message=&(static_message[user->world_id]);
				cyberspace_user_connection_output(user, message);
				user->need_to_load_world=false;
				}
			message=&(sync_message[user->world_id]);
			cyberspace_user_connection_output(user, message);
			}

		//minigames
		for(int i=0;i<cyberspace_user_connected;i++) {
			user=&(cyberspace_user_connection_datas[i]);
			if(user->phase==CYBERSPACE_PHASE_MINIGAME_TESTING) {
				cyberspace_minigame_testing_frame(user, milliseconds);
				}
			else if(user->phase==CYBERSPACE_PHASE_MINIGAME_HEURISTIC) {
				cyberspace_minigame_heuristic_frame(user, milliseconds);
				}
			else if(user->phase==CYBERSPACE_PHASE_MINIGAME_FAST) {
				cyberspace_minigame_fast_frame(user, milliseconds);
				}
			else if(user->phase==CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING) {
				cyberspace_minigame_patternmatching_frame(user, milliseconds);
				}
			}

		//log outs
		for(int i=0;i<cyberspace_user_connected;i++) {
			user=&(cyberspace_user_connection_datas[i]);
			if(!user->logging_out) continue;
			seconds=user->milliseconds_to_logout/1000;
			user->milliseconds_to_logout-=milliseconds;
			if(user->milliseconds_to_logout<=0) {
//				user->obj->type=CYBERSPACE_WORLD_OBJECT_CONSOLE;
//				no, trovare il punto in cui aggiungo il tizio al mondo e undoarlo
				world=&(cyberspace_universe[user->world_id]);
				for(int j=0;j<world->dynamic_size;j++) {
					if(user->obj==&(world->dynamic_obj[j])) {
						world->dynamic_obj[j]=world->dynamic_obj[world->dynamic_size-1];
						world->dynamic_size--;
						user->obj=NULL;
						user->logging_out=false;
						break;
						}
					}
				if(user->obj!=NULL) {
					fprintf(stderr, "ERROR: disconnection fails.\n");
					exit(EXIT_FAILURE);
					}
				user->phase=CYBERSPACE_PHASE_LOGIN;
				user->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_LOGIN;
				cyberspace_user_connection_output(user, &(user->out_command));
				user->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "logout effettuato con successo! puoi rimuovere il connettore neurale");
				memcpy(user->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(user, &(user->out_command));
				continue;
				}
			if(seconds!=user->milliseconds_to_logout/1000) {
				user->out_command.type=CYBERSPACE_COMMAND_SERVER_CHAT;
				errormessage=new char[100];
//				sprintf(errormessage, "logout fra %d millisecondi", user->milliseconds_to_logout);
				sprintf(errormessage, "system : logout fra %d secondi", (int)ceil(double(user->milliseconds_to_logout)/1000.0));
				memcpy(user->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(user, &(user->out_command));
				}
			}

		//ticscount
		if(tics==ticfrequency) {
			tics=0;
			for(int i=0;i<cyberspace_user_connected;i++) {
				user=&(cyberspace_user_connection_datas[i]);
				if(user->phase!=CYBERSPACE_PHASE_3D && user->phase!=CYBERSPACE_PHASE_PROGRAMMING && user->phase!=CYBERSPACE_PHASE_MINIGAME_TESTING && user->phase!=CYBERSPACE_PHASE_MINIGAME_HEURISTIC && user->phase!=CYBERSPACE_PHASE_MINIGAME_FAST && user->phase!=CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING) continue;

				dice_skill=random()%10;

/*
				user->out_command.type=CYBERSPACE_COMMAND_SERVER_CHAT;
				errormessage=new char[100];
				sprintf(errormessage, "system : dice:%d", dice_skill);
				memcpy(user->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(user, &(user->out_command));
*/

				SDL_mutexP(user->disk_mutex);

				if(user->personal.skill<=dice_skill) {
					SDL_mutexV(user->disk_mutex);
					continue;
					}

				learned_skill=false;

				sys_skill=CYBERSPACE_NEURAL_SYSTEM_MOVEMENT;

				subsys_skill=CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_SPEED;
				if(!learned_skill && user->personal.subsystems_hint[sys_skill][subsys_skill]==0) {
					dice_skill=random()%100;

/*
					user->out_command.type=CYBERSPACE_COMMAND_SERVER_CHAT;
					errormessage=new char[100];
					sprintf(errormessage, "system : dice:%d%%", dice_skill);
					memcpy(user->out_command.data, errormessage, strlen(errormessage)+1);
					delete[] errormessage;
					cyberspace_user_connection_output(user, &(user->out_command));
*/

					if(dice_skill<50) {
						learned_skill=true;
						user->personal.subsystems_hint[sys_skill][subsys_skill]=1;
						}
					}

				subsys_skill=CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_JUMP;
				if(!learned_skill && user->personal.subsystems_hint[sys_skill][subsys_skill]==0) {
					dice_skill=random()%100;
					if(dice_skill<50) {
						learned_skill=true;
						user->personal.subsystems_hint[sys_skill][subsys_skill]=1;
						}
					}

				sys_skill=CYBERSPACE_NEURAL_SYSTEM_SCAN;

				subsys_skill=CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_COOR;
				if(!learned_skill && user->personal.subsystems_hint[sys_skill][subsys_skill]==0) {
					dice_skill=random()%100;
					if(dice_skill<50) {
						learned_skill=true;
						user->personal.subsystems_hint[sys_skill][subsys_skill]=1;
						}
					}

				if(learned_skill) {
					cyberspace_nonvolatile_save_personal(user);

					user->out_command.type=CYBERSPACE_COMMAND_SERVER_USER;
					memcpy(user->out_command.data, &(user->personal), sizeof(user->personal));
					cyberspace_user_connection_output(user, &(user->out_command));

					user->out_command.type=CYBERSPACE_COMMAND_SERVER_CHAT;
					errormessage=new char[100];
					sprintf(errormessage, "system : pensi di aver capito come poter programmare un nuovo processo cognitivo artificiale", dice_skill);
					memcpy(user->out_command.data, errormessage, strlen(errormessage)+1);
					delete[] errormessage;
					cyberspace_user_connection_output(user, &(user->out_command));
					}

				SDL_mutexV(user->disk_mutex);
				}
			}
		}

	printf("physical engine: ENDING\n");
//	pthread_exit(NULL);
	exit(EXIT_SUCCESS);
	}

void cyberspace_3D_message2server(cyberspace_user_connection_data* data) {
	if(data->phase!=CYBERSPACE_PHASE_3D) {
		fprintf(stderr, "ERROR: data->phase!=CYBERSPACE_PHASE_3D (%d).\n", data->phase);
		exit(EXIT_FAILURE);
		}

	char* errormessage;
	char* commandp;
	char* commandp2;
	cyberspace_user_connection_data* user;
	cyberspace_user_personal_data* account;
	FILE* file;
	int filed;
	char* path;

//	bool toomuchdistant;
	switch(data->in_command.type) {
		case CYBERSPACE_COMMAND_USER_CHAT:
			if(data->world_id==-1) break;
//			chatmessage=new char[CYBERSPACE_GUI_CHAT_LINE_WIDTH+1];
//			strcpy(chatmessage, (char*)(data->in_command.data));
//			printf("CHAT: \"%s\"\n", chatmessage);
			if(((char*)(data->in_command.data))[0]=='\\') {
				commandp=(char*)(data->in_command.data);
				commandp++;
				if(strcmp(commandp, "quit")==0) {
					if(data->personal.id_static!=1) {
						data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
						errormessage=new char[100];
						sprintf(errormessage, "non hai i permessi per invocare il comando");
						memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
						delete[] errormessage;
						cyberspace_user_connection_output(data, &(data->out_command));
						break;
						}
					exit(EXIT_FAILURE);
					break;
					}
				if(strcmp(commandp, "accounts")==0) {
					if(data->personal.id_static!=1) {
						data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
						errormessage=new char[100];
						sprintf(errormessage, "non hai i permessi per invocare il comando");
						memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
						delete[] errormessage;
						cyberspace_user_connection_output(data, &(data->out_command));
						break;
						}
					data->out_command.type=CYBERSPACE_COMMAND_SERVER_CHAT;
					errormessage=new char[100];
					sprintf(errormessage, "system : numero di utenti: %d", cyberspace_server_data.accounts);
					memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
					delete[] errormessage;
					cyberspace_user_connection_output(data, &(data->out_command));
					break;
					}
				if(strncmp(commandp, "account ", 8)==0) {
					if(data->personal.id_static!=1) {
						data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
						errormessage=new char[100];
						sprintf(errormessage, "non hai i permessi per invocare il comando");
						memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
						delete[] errormessage;
						cyberspace_user_connection_output(data, &(data->out_command));
						break;
						}
					commandp+=8;
					if((commandp2=strchr(commandp, ' '))==NULL) {
						data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
						errormessage=new char[100];
						sprintf(errormessage, "formato invalido");
						memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
						delete[] errormessage;
						cyberspace_user_connection_output(data, &(data->out_command));
						break;
						}
					*commandp2='\0';

					SDL_mutexP(cyberspace_mutex_server_data_sdl);

					account=new cyberspace_user_personal_data;
					sprintf(account->name, "%s", commandp);
					account->password[0]='\0';
					account->skill=atoi(commandp2+1);
					account->connector=CYBERSPACE_CONNECTOR_MODEL_CRAP;
					account->systems_max=2;
					if(account->skill==1) account->systems_max--;
					if(account->skill==4) account->systems_max++;
					if(account->skill==5) account->systems_max++;

					for(int sys=0;sys<CYBERSPACE_NEURAL_SYSTEM_MAX;sys++) {
						account->systems[sys]=0;
						account->systems_loaded[sys]=0;
						account->systems_hint[sys]=0;
						for(int subsys=0;subsys<CYBERSPACE_NEURAL_SUBSYSTEM_MAX;subsys++) {
							account->subsystems[sys][subsys]=0;
							account->subsystems_loaded[sys][subsys]=0;
							account->subsystems_hint[sys][subsys]=0;
							}
						}
					account->heuristic_record=0;
					account->fast_record=0;
					account->patternmatching_record=0;

					account->id_static=cyberspace_server_data.accounts+1;

					path=new char[100];

					sprintf(path, "accounts/%s.now", account->name);
					if((file=fopen(path, "w+"))==NULL) {
						perror("perror:\n");
						fprintf(stderr, "FATAL ERROR: unable to fopen file \"%s\"\n", path);
						exit(EXIT_FAILURE);
						}
					if(fwrite(account, sizeof(*account), 1, file)!=1) {
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

					sprintf(path, "accounts/%s.old", account->name);
					if((file=fopen(path, "w+"))==NULL) {
						perror("perror:\n");
						fprintf(stderr, "FATAL ERROR: unable to fopen file \"%s\"\n", path);
						exit(EXIT_FAILURE);
						}
					if(fwrite(account, sizeof(*account), 1, file)!=1) {
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

					delete[] path;
					delete account;

					cyberspace_server_data.accounts++;

					cyberspace_nonvolatile_save_server(data);

					SDL_mutexV(cyberspace_mutex_server_data_sdl);

					data->out_command.type=CYBERSPACE_COMMAND_SERVER_CHAT;
					errormessage=new char[100];
					sprintf(errormessage, "system : [%d] creato utente \"%s\" skill %d", cyberspace_server_data.accounts, commandp, atoi(commandp2+1));
					memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
					delete[] errormessage;
					cyberspace_user_connection_output(data, &(data->out_command));
					break;
					}
				if(strncmp(commandp, "password ", 9)==0) {
					commandp+=9;
					if(strlen(commandp)>20) {
						data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
						errormessage=new char[100];
						sprintf(errormessage, "password troppo lunga (max. 20 caratteri)");
						memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
						delete[] errormessage;
						cyberspace_user_connection_output(data, &(data->out_command));
						break;
						}
					SDL_mutexP(data->disk_mutex);
					sprintf(data->personal.password, "%s", commandp);

					cyberspace_nonvolatile_save_personal(data);
					SDL_mutexV(data->disk_mutex);

					data->out_command.type=CYBERSPACE_COMMAND_SERVER_CHAT;
					errormessage=new char[100];
					sprintf(errormessage, "system : password cambiata in \"%s\"", data->personal.password);
					memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
					delete[] errormessage;
					cyberspace_user_connection_output(data, &(data->out_command));
					break;
					}
				if(strcmp(commandp, "version")==0) {
					data->out_command.type=CYBERSPACE_COMMAND_SERVER_CHAT;
					errormessage=new char[100];
					sprintf(errormessage, "system : cyberspace server version: %d.%d", CYBERSPACE_SERVER_VERSION_MAJOR, CYBERSPACE_SERVER_VERSION_MINOR);
					memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
					delete[] errormessage;
					cyberspace_user_connection_output(data, &(data->out_command));
					break;
					}
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "comando sconosciuto");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			SDL_mutexP(cyberspace_mutex_universe_sdl[data->world_id]);
//			pthread_mutex_lock(&cyberspace_mutex_universe[data->world_id]);
			for(int i=0;i<cyberspace_user_connected;i++) {
				user=&(cyberspace_user_connection_datas[i]);
				if(user->phase!=CYBERSPACE_PHASE_3D) continue;
				if(user->world_id!=data->world_id) continue;
				user->out_command.type=CYBERSPACE_COMMAND_SERVER_CHAT;
				memcpy(user->out_command.data, data->personal.name, sizeof(char)*strlen((char*)(data->personal.name)));
				memcpy(user->out_command.data+sizeof(char)*strlen((char*)(data->personal.name)), " : ", sizeof(char)*3);
				memcpy(user->out_command.data+sizeof(char)*(3+strlen((char*)(data->personal.name))), data->in_command.data, sizeof(char)*(strlen((char*)(data->in_command.data))+1));
				cyberspace_user_connection_output(user, &(user->out_command));
				}
			SDL_mutexV(cyberspace_mutex_universe_sdl[data->world_id]);
//			pthread_mutex_unlock(&cyberspace_mutex_universe[data->world_id]);
			break;
		case CYBERSPACE_COMMAND_USER_MOVEMENT:
			if(data->world_id==-1) break;
			if(data->logging_out) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "stai effettuando il logout: non puoi muoverti");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			SDL_mutexP(cyberspace_mutex_universe_sdl[data->world_id]);
//			pthread_mutex_lock(&cyberspace_mutex_universe[data->world_id]);
			memcpy(&(data->movement), data->in_command.data, sizeof(data->movement));
			if((data->movement.rotation!=1 && data->movement.rotation!=-1 && data->movement.rotation!=0) || (data->movement.traslation!=1 && data->movement.traslation!=-1 && data->movement.traslation!=0)) {
				data->movement.rotation=0;
				data->movement.traslation=0;
				SDL_mutexV(cyberspace_mutex_universe_sdl[data->world_id]);
//				pthread_mutex_unlock(&cyberspace_mutex_universe[data->world_id]);
				fprintf(stderr, "WARNING: unexpected translation/rotation by connection %d (%d).\n", data->personal.id_static, data->id_dynamic);
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "unexpected translation/rotation");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				}
			else SDL_mutexV(cyberspace_mutex_universe_sdl[data->world_id]);
//			else pthread_mutex_unlock(&cyberspace_mutex_universe[data->world_id]);
			printf("movement: rotation(%d) traslation(%d)\n", data->movement.rotation, data->movement.traslation);
			break;
		case CYBERSPACE_COMMAND_USER_SKILL_0:
			if(data->world_id==-1) break;
			if(data->logging_out) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "stai effettuando il logout: non puoi usare abilita'");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			printf("skill0\n");
			SDL_mutexP(cyberspace_mutex_universe_sdl[data->world_id]);
//			pthread_mutex_lock(&cyberspace_mutex_universe[data->world_id]);
			if(!data->jumping) {
				data->falling=false;
				data->falling_speed=0.0;
				data->jumping=true;
				data->jumping_speed=5.0;
				}
			SDL_mutexV(cyberspace_mutex_universe_sdl[data->world_id]);
//			pthread_mutex_unlock(&cyberspace_mutex_universe[data->world_id]);
			break;
		case CYBERSPACE_COMMAND_USER_ACTION:
			if(data->world_id==-1) break;
			if(data->logging_out) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "stai effettuando il logout: non puoi azionare oggetti");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			printf("selection: type(\"%s\") id(%d)\n", (data->selection.dynamic ? "dyn" : "sta"), data->selection.id);
			SDL_mutexP(cyberspace_mutex_universe_sdl[data->world_id]);
//			pthread_mutex_lock(&cyberspace_mutex_universe[data->world_id]);
			memcpy(&(data->selection), data->in_command.data, sizeof(data->selection));
			SDL_mutexV(cyberspace_mutex_universe_sdl[data->world_id]);
//			pthread_mutex_unlock(&cyberspace_mutex_universe[data->world_id]);
			if(data->selection.id<0 || (data->selection.dynamic && data->selection.id>=cyberspace_universe[data->world_id].dynamic_size) || (!data->selection.dynamic && data->selection.id>=cyberspace_universe[data->world_id].static_size)) {
				fprintf(stderr, "WARNING: wrong data->selection.id (%d) by connection %d (%d).\n", data->selection.id, data->personal.id_static, data->id_dynamic);
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "unexpected data->selection.id value");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			cyberspace_world_object* obj;
			double dist;
			bool toomuchdistant;
			if(data->selection.dynamic) obj=&(cyberspace_universe[data->world_id].dynamic_obj[data->selection.id]);
			else obj=&(cyberspace_universe[data->world_id].static_obj[data->selection.id]);
			dist=sqrt((obj->point.position.x-data->obj->point.position.x)*(obj->point.position.x-data->obj->point.position.x)+(obj->point.position.z-data->obj->point.position.z)*(obj->point.position.z-data->obj->point.position.z)+(obj->point.position.y-data->obj->point.position.y)*(obj->point.position.y-data->obj->point.position.y));
			toomuchdistant=false;
			switch(obj->type) {
				case CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD:
					if(dist>200.0) {
						toomuchdistant=true;
						break;
						}

					SDL_mutexP(cyberspace_mutex_universe_sdl[data->world_id]);
//					pthread_mutex_lock(&cyberspace_mutex_universe[data->world_id]);
					data->obj->point.position.x=obj->parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_X];
					data->obj->point.position.y=obj->parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Y];
					data->obj->point.position.z=obj->parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_TARGET_Z];
//					data->obj->point.angle.alpha=0.0;
//					data->obj->point.angle.beta=0.0;
//					data->obj->point.angle.gamma=0.0;
					SDL_mutexV(cyberspace_mutex_universe_sdl[data->world_id]);
//					pthread_mutex_unlock(&cyberspace_mutex_universe[data->world_id]);

/*
					data->world_id=0;
					data->need_to_load_world=true;
					data->movement.rotation=0;
					data->movement.traslation=0;
					data->movement.strafe=false;

					pthread_mutex_lock(&cyberspace_mutex_universe[data->world_id]);

					if(cyberspace_universe[data->world_id].dynamic_size>=CYBERSPACE_WORLD_DYNAMIC_OBJECTS_MAX) {
						fprintf(stderr, "FATAL ERROR: world->dynamic_size>=CYBERSPACE_WORLD_DYNAMIC_OBJECTS_MAX (%d in world %d).\n", cyberspace_universe[data->world_id].dynamic_size, data->world_id);
						exit(EXIT_FAILURE);
						}

					data->obj=&(world->dynamic_obj[world->dynamic_size++]);
					data->obj->type=CYBERSPACE_WORLD_OBJECT_USER;
					data->obj->parameters[CYBERSPACE_WORLD_OBJECT_USER_PARAMETER_ID_STATIC]=(double)(data->personal.id_static);
					data->obj->parameters[CYBERSPACE_WORLD_OBJECT_USER_PARAMETER_ID_DYNAMIC]=(double)(data->id_dynamic);
					data->obj->point.position.x=50.0;
					data->obj->point.position.y=0.0;
					data->obj->point.position.z=50.0;
					data->obj->point.angle.alpha=0.0;
					data->obj->point.angle.beta=0.0;
					data->obj->point.angle.gamma=0.0;

//			world->users[world->users_size]=data->id_static;
//			world->users_size++;
			pthread_mutex_unlock(&cyberspace_mutex_universe[data->world_id]);

*/
					break;
				case CYBERSPACE_WORLD_OBJECT_USER:
					if(dist>200.0) {
						toomuchdistant=true;
						break;
						}
					break;
				case CYBERSPACE_WORLD_OBJECT_CONSOLE:
					if(dist>200.0) {
						toomuchdistant=true;
						break;
						}
					if(data->personal.id_static==7) {
						fprintf(stderr, "WARNING: non possiedi capacita' di programmazione.\n");
						data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
						errormessage=new char[100];
						sprintf(errormessage, "non possiedi capacita' di programmazione");
						memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
						delete[] errormessage;
						cyberspace_user_connection_output(data, &(data->out_command));
						}
					else {
						data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_PROGRAMMING;
						cyberspace_user_connection_output(data, &(data->out_command));
						data->phase=CYBERSPACE_PHASE_PROGRAMMING;
						}
					break;
				default:
					fprintf(stderr, "WARNING: unexpected selected obj type: %d by connection %d (%d).\n", obj->type, data->personal.id_static, data->id_dynamic);
					data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
					errormessage=new char[100];
					sprintf(errormessage, "unexpected selected obj type: %d", obj->type);
					memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
					delete[] errormessage;
					cyberspace_user_connection_output(data, &(data->out_command));
					break;
				}
			if(toomuchdistant) {
				fprintf(stderr, "WARNING: selected obj too much distant: %d by connection %d (%d).\n", data->selection.id, data->personal.id_static, data->id_dynamic);
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "oggetto selezionato troppo distante");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				}
			break;
//		case CYBERSPACE_COMMAND_USER_LOGIN:
//			memcpy(&(data->in_command.type), data->read_buffer+sizeof(data->read_message_size), sizeof(data->in_command.type));
			//TODO rimuovere questa opzione

//			data->phase=CYBERSPACE_PHASE_LOGIN;

//			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_LOGIN;
//			cyberspace_user_connection_output(data, &(data->out_command));


//			break;
		case CYBERSPACE_COMMAND_USER_LOGOUT:
			if(data->world_id==-1) break;
			if(data->logging_out) {
				data->logging_out=false;
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "logout abortito");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			SDL_mutexP(cyberspace_mutex_universe_sdl[data->world_id]);
//			pthread_mutex_lock(&cyberspace_mutex_universe[data->world_id]);
			data->logging_out=true;
			data->milliseconds_to_logout=CYBERSPACE_LOGOUT_MILLISECONDS;
			data->movement.rotation=0;
			data->movement.traslation=0;
			SDL_mutexV(cyberspace_mutex_universe_sdl[data->world_id]);
//			pthread_mutex_unlock(&cyberspace_mutex_universe[data->world_id]);
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_CHAT;
			errormessage=new char[100];
			sprintf(errormessage, "system : logout fra %d secondi", (int)ceil(double(data->milliseconds_to_logout)/1000.0));
//			sprintf(errormessage, "logout fra %d millisecondi", data->milliseconds_to_logout);
			memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		default:
			fprintf(stderr, "WARNING: unexpected message in 3D (%d) by connection %d (%d).\n", data->in_command.type, data->personal.id_static, data->id_dynamic);
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
/*			data->out_command.include_data_s=true;
			data->out_command.include_data_m=false;
			data->out_command.include_data_l=false;
			errormessage=new char[CYBERSPACE_COMMAND_SIZE_S];
			sprintf(errormessage, "unexpected message in 3D");
			memcpy(data->out_command.data_s, errormessage, strlen(errormessage)+1);*/
			errormessage=new char[100];
			sprintf(errormessage, "unexpected message in 3D");
			memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		}

	}

double cyberspace_3D_collision_line_circle(cyberspace_world_point* p_i, double V_x, double V_z, cyberspace_world_point* p_a, double alpha, double* tangent, double* reduction) {
	//intersezione retta lungo V con cerchio puntato in un estremo di redline
	//At^2+Bt+C=0
	//A=(v_x)^2+(v_y)^2=cos(v.alpha)*cos(v.alpha)+sin(v.alpha)*sin(v.alpha)=1
	//B=2v_x(x_i-x_0)+2v_y(y_i-y_0)=2.0*cos(v.alpha)*(obj->point.position.x-red_line->p0.x)+2.0*sin(v.alpha)*(obj->point.position.y-red_line->p0.y)
	//C=(x_i-x_0)^2+(y_i-y_0)^2-R^2=(obj->point.position.x-red_line->p0.x)*(obj->point.position.x-red_line->p0.x)+(obj->point.position.y-red_line->p0.y)*(obj->point.position.y-red_line->p0.y)-CYBERSPACE_WORLD_OBJECT_USER_R*CYBERSPACE_WORLD_OBJECT_USER_R
	//t=-B/2A +/- sqrt(B^2-4AC)/2A
	//il denominatore A e' sempre diverso da 0
	//DELTA=B^2-4AC
	//se DELTA==0 colpisco tangenzialmente e nessun urto
	//altrimenti prendo il t positivo piu' piccolo
	double cd_A=1.0;
	double cd_B=2.0*V_x*(p_i->x-p_a->x)+2.0*V_z*(p_i->z-p_a->z);
	double cd_C=(p_i->x-p_a->x)*(p_i->x-p_a->x)+(p_i->z-p_a->z)*(p_i->z-p_a->z)-CYBERSPACE_WORLD_OBJECT_USER_R*CYBERSPACE_WORLD_OBJECT_USER_R;
	double cd_D=cd_B*cd_B-4.0*cd_A*cd_C;
	if(cd_D<=0.0) return -1.0;
	double collision=(-cd_B-sqrt(cd_D))/(2.0*cd_A);
	if(collision<0) collision=(-cd_B+sqrt(cd_D))/(2.0*cd_A);
	if(collision<0) return -1.0;
	double arcocos=acos(((p_i->x+collision*V_x)-p_a->x)/CYBERSPACE_WORLD_OBJECT_USER_R);
	double arcosin=asin(((p_i->z+collision*V_z)-p_a->z)/CYBERSPACE_WORLD_OBJECT_USER_R);
	if(arcocos<=M_PI/2.0) {
		if(arcosin>=0.0) *tangent=arcosin;
		else *tangent=(M_PI*2.0)+arcosin;
		}
	else {
		if(arcosin>=0.0) *tangent=arcocos;
		else *tangent=(M_PI*2.0)-arcocos;
		}
	*tangent+=M_PI/2.0;
	if(*tangent>2.0*M_PI) *tangent-=2.0*M_PI;

//	controllare qua
/*	double beta=alpha-*tangent;
	if(beta<0.0) beta*=-1.0;
	if(beta>M_PI) beta-=M_PI;
	if(beta>M_PI/2.0) {
		*tangent+=M_PI;
		if(*tangent>2.0*M_PI) *tangent-=2.0*M_PI;
		beta=alpha-*tangent;
		if(beta<0.0) beta*=-1.0;
		if(beta>M_PI) beta-=M_PI;
		}
	*reduction=cos(beta);*/

	double beta=alpha-*tangent;
	if(beta<0.0) beta*=-1.0;
	if(beta>M_PI) beta=2.0*M_PI-beta;
	if(beta>M_PI/2.0) {
		*tangent+=M_PI;
		if(*tangent>2.0*M_PI) *tangent-=2.0*M_PI;
		beta=alpha-*tangent;
		if(beta<0.0) beta*=-1.0;
		if(beta>M_PI) beta=2.0*M_PI-beta;
		}
	*reduction=cos(beta);

	return collision;
	}

double cyberspace_3D_collision_line_segment(cyberspace_world_point* p_i, double V_x, double V_z, cyberspace_world_point* p_a, cyberspace_world_point* p_b, double alpha, double* tangent, double* reduction) {
	double cd_Xba=p_b->x-p_a->x;
	double cd_Zba=p_b->z-p_a->z;
	double cd_Xia=p_i->x-p_a->x;
	double cd_Zia=p_i->z-p_a->z;
	double cd_L=sqrt((cd_Xba*cd_Xba)+(cd_Zba*cd_Zba));
	double cd_T;
	double cd_M;
	double cd_DIV;
	if(cd_Xba!=0.0) {
		cd_DIV=V_z-(V_x*cd_Zba)/cd_Xba;
		if(cd_DIV==0.0) return -1;
		cd_T=(-cd_Zia+(cd_Xia*cd_Zba)/cd_Xba)/cd_DIV;
		cd_M=(cd_Xia+V_x*cd_T)/cd_Xba;
		}
	else {
		cd_DIV=V_x-(V_z*cd_Xba)/cd_Zba;
		if(cd_DIV==0.0) return -1;
		cd_T=(-cd_Xia+(cd_Zia*cd_Xba)/cd_Zba)/cd_DIV;
		cd_M=(cd_Zia+V_z*cd_T)/cd_Zba;
		}
	if(cd_M<0.0 || cd_M>1.0) return -1;

//	*tangent=acos(cd_Xba/cd_L);
	double arcocos=acos(cd_Xba/cd_L);
	double arcosin=asin(cd_Zba/cd_L);
	if(arcocos<=M_PI/2.0) {
		if(arcosin>=0.0) *tangent=arcosin;
		else *tangent=(M_PI*2.0)+arcosin;
		}
	else {
		if(arcosin>=0.0) *tangent=arcocos;
		else *tangent=(M_PI*2.0)-arcocos;
		}

	double beta=alpha-*tangent;
	if(beta<0.0) beta*=-1.0;
	if(beta>M_PI) beta=2.0*M_PI-beta;
	if(beta>M_PI/2.0) {
		*tangent+=M_PI;
		if(*tangent>2.0*M_PI) *tangent-=2.0*M_PI;
		beta=alpha-*tangent;
		if(beta<0.0) beta*=-1.0;
		if(beta>M_PI) beta=2.0*M_PI-beta;
		}
	*reduction=cos(beta);

	return cd_T;
	}

