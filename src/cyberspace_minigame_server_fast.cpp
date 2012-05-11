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


#include "cyberspace_minigame_server_fast.h"



int cyberspace_minigame_fast_1dx(int faces) {
	int res=1+int(floor(faces*(((double)(random()%RAND_MAX))/((double)RAND_MAX))));
	if(res==(faces+1)) return(cyberspace_minigame_fast_1dx(faces));
	return(res);
	}

bool cyberspace_minigame_fast_p(double p) {
	return(p>=(((double)(random()%RAND_MAX))/((double)RAND_MAX)));
	}

void cyberspace_minigame_fast_newmap(cyberspace_user_connection_data* data) {
	onebyte* map=data->mini_fast.floor;
	bool* hor=data->mini_fast.hor;
	bool* ver=data->mini_fast.ver;

	int startX, startY, endX, endY, curX, curY, tmpX, tmpY, direction, jumpL;
	bool reroll;
	int reroll_n;

	CYBERSPACE_MINIGAME_REDO:
//	printf("debug: (re)start\n");

	for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
		for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
			hor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]=false;
			ver[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]=false;
			if(x==0 || y==0 || x==CYBERSPACE_MINIGAME_FAST_MAP_L-1 || y==CYBERSPACE_MINIGAME_FAST_MAP_L-1) map[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_RADIO;
			else map[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_FLOOR;
			}
		}

//	printf("debug: a\n");

	startX=cyberspace_minigame_fast_1dx(CYBERSPACE_MINIGAME_FAST_MAP_L-2);
	startY=cyberspace_minigame_fast_1dx(CYBERSPACE_MINIGAME_FAST_MAP_L-2);
	curX=startX;
	curY=startY;

	//0 up
	//1 down
	//2 left
	//3 right

	for(int move=0;move<CYBERSPACE_MINIGAME_FAST_PATH_LENGTH;move++) {
		if(hor[curX+curY*CYBERSPACE_MINIGAME_FAST_MAP_L]) {
			if(ver[curX+curY*CYBERSPACE_MINIGAME_FAST_MAP_L]) goto CYBERSPACE_MINIGAME_REDO;
			if(map[curX+(curY-1)*CYBERSPACE_MINIGAME_FAST_MAP_L]!=CYBERSPACE_MINIGAME_FAST_MAP_FLOOR && map[curX+(curY+1)*CYBERSPACE_MINIGAME_FAST_MAP_L]!=CYBERSPACE_MINIGAME_FAST_MAP_FLOOR) goto CYBERSPACE_MINIGAME_REDO;
			}
		if(ver[curX+curY*CYBERSPACE_MINIGAME_FAST_MAP_L]) {
			if(map[curX+1+curY*CYBERSPACE_MINIGAME_FAST_MAP_L]!=CYBERSPACE_MINIGAME_FAST_MAP_FLOOR && map[curX-1+curY*CYBERSPACE_MINIGAME_FAST_MAP_L]!=CYBERSPACE_MINIGAME_FAST_MAP_FLOOR) goto CYBERSPACE_MINIGAME_REDO;
			}

//		printf("debug: b (%d)\n", move);

		reroll=true;
		reroll_n=0;
		while(reroll) {

//			printf("debug: c %d\n", reroll_n);
			if(reroll_n++>CYBERSPACE_MINIGAME_FAST_MAXREROLLS) goto CYBERSPACE_MINIGAME_REDO;
			reroll=false;

			direction=random()%4;
			if(hor[curX+curY*CYBERSPACE_MINIGAME_FAST_MAP_L]) direction=direction%2;
			else if(ver[curX+curY*CYBERSPACE_MINIGAME_FAST_MAP_L]) direction=2+direction%2;

			switch(direction) {
				case 0: //up
					if(curY==1) {
						reroll=true;
						break;
						}
					jumpL=cyberspace_minigame_fast_1dx(curY-1);
					break;
				case 1: //down
					if(curY==CYBERSPACE_MINIGAME_FAST_MAP_L-2) {
						reroll=true;
						break;
						}
					jumpL=cyberspace_minigame_fast_1dx(CYBERSPACE_MINIGAME_FAST_MAP_L-2-curY);
					break;
				case 2: //left
					if(curX==1) {
						reroll=true;
						break;
						}
					jumpL=cyberspace_minigame_fast_1dx(curX-1);
					break;
				case 3: //right
					if(curX==CYBERSPACE_MINIGAME_FAST_MAP_L-2) {
						reroll=true;
						break;
						}
					jumpL=cyberspace_minigame_fast_1dx(CYBERSPACE_MINIGAME_FAST_MAP_L-2-curX);
					break;
				}
			if(reroll) continue;

			for(int i=1;i<=jumpL;i++) {
//				printf("debug: d (%d)\n", i);
				tmpX=curX;
				tmpY=curY;
				switch(direction) {
					case 0: //up
						tmpY-=i;
						break;
					case 1: //down
						tmpY+=i;
						break;
					case 2: //left
						tmpX-=i;
						break;
					case 3: //right
						tmpX+=i;
						break;
					}
				if(map[tmpX+tmpY*CYBERSPACE_MINIGAME_FAST_MAP_L]!=CYBERSPACE_MINIGAME_FAST_MAP_FLOOR) {
					reroll=true;
					break;
					}
				}
			if(reroll) continue;

//			printf("debug: e\n");

			switch(direction) {
				case 0: //up
				case 1: //down
					if(hor[tmpX+tmpY*CYBERSPACE_MINIGAME_FAST_MAP_L]) {
						reroll=true;
						break;
						}
					if(direction==0 && (hor[tmpX+(tmpY-1)*CYBERSPACE_MINIGAME_FAST_MAP_L] || ver[tmpX+(tmpY-1)*CYBERSPACE_MINIGAME_FAST_MAP_L])) {
						reroll=true;
						break;
						}
					if(direction==1 && (hor[tmpX+(tmpY+1)*CYBERSPACE_MINIGAME_FAST_MAP_L] || ver[tmpX+(tmpY+1)*CYBERSPACE_MINIGAME_FAST_MAP_L])) {
						reroll=true;
						break;
						}
					break;
				case 2: //left
				case 3: //right
					if(ver[tmpX+tmpY*CYBERSPACE_MINIGAME_FAST_MAP_L]) {
						reroll=true;
						break;
						}
					if(direction==2 && (hor[tmpX-1+tmpY*CYBERSPACE_MINIGAME_FAST_MAP_L] || ver[tmpX-1+tmpY*CYBERSPACE_MINIGAME_FAST_MAP_L])) {
						reroll=true;
						break;
						}
					if(direction==3 && (hor[tmpX+1+tmpY*CYBERSPACE_MINIGAME_FAST_MAP_L] || ver[tmpX+1+tmpY*CYBERSPACE_MINIGAME_FAST_MAP_L])) {
						reroll=true;
						break;
						}
					break;
				}
			if(reroll) continue;
//			printf("debug: f\n");
			}

//		printf("debug: g\n");

		if(move<CYBERSPACE_MINIGAME_FAST_PATH_LENGTH-1) {
//			printf("debug: h\n");
			switch(direction) {
				case 0:
					map[tmpX+(tmpY-1)*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CAN;
					break;
				case 1:
					map[tmpX+(tmpY+1)*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CAN;
					break;
				case 2:
					map[tmpX-1+tmpY*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CAN;
					break;
				case 3:
					map[tmpX+1+tmpY*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CAN;
					break;
				}
			}

		for(int i=0;i<=jumpL;i++) {
//			printf("debug: i (%d)\n", i);
			tmpX=curX;
			tmpY=curY;
			switch(direction) {
				case 0:
					tmpY-=i;
					break;
				case 1:
					tmpY+=i;
					break;
				case 2:
					tmpX-=i;
					break;
				case 3:
					tmpX+=i;
					break;
				}
			if(direction==0 || direction==1) ver[tmpX+tmpY*CYBERSPACE_MINIGAME_FAST_MAP_L]=true;
			if(direction==2 || direction==3) hor[tmpX+tmpY*CYBERSPACE_MINIGAME_FAST_MAP_L]=true;
			}

		curX=tmpX;
		curY=tmpY;
		}
//	printf("debug: f\n");

	endX=curX;
	endY=curY;
	map[endX+endY*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_END;
	map[startX+startY*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_START;

	for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
		for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
//			printf("debug: g (%d-%d)\n", x, y);
			if(x==0 || y==0 || x==CYBERSPACE_MINIGAME_FAST_MAP_L-1 || y==CYBERSPACE_MINIGAME_FAST_MAP_L-1) if(cyberspace_minigame_fast_p(0.1)) map[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CAN;
			if(map[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]!=CYBERSPACE_MINIGAME_FAST_MAP_FLOOR) continue;
			if(cyberspace_minigame_fast_p(0.1)) map[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL;
			if(ver[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L] || hor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]) continue;
			if(cyberspace_minigame_fast_p(0.1)) map[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CAN;
			if(cyberspace_minigame_fast_p(0.05)) map[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_RADIO;
			}
		}
//	printf("debug: END\n");
	}

void cyberspace_minigame_fast_init(cyberspace_user_connection_data* data) {
	data->mini_fast.points=0;
	cyberspace_minigame_fast_newmap(data);
/*	for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
		for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
			printf("%d", data->mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]);
			}
		printf("\n");
		}*/
	data->mini_fast.milliseconds=1000+(30*1000)/(1+data->mini_fast.points/5);
	data->mini_fast.milliseconds+=1000;
	data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_FAST_POINTS_AND_MAP;
	memcpy(data->out_command.data, &(data->mini_fast.points), sizeof(data->mini_fast.points));
	memcpy(data->out_command.data+sizeof(data->mini_fast.points), data->mini_fast.floor, CYBERSPACE_MINIGAME_FAST_MAP_L*CYBERSPACE_MINIGAME_FAST_MAP_L);
	cyberspace_user_connection_output(data, &(data->out_command));
	}

void cyberspace_minigame_fast_frame(cyberspace_user_connection_data* data, int milliseconds) {
	data->mini_fast.milliseconds-=milliseconds;
	if(data->mini_fast.milliseconds>0) return;
	bool resend_personal=false;
//	bool success;
//	bool error=false;
//	int lv;
	char* errormessage;
	bool programmed;
	switch(data->programming.mini_cause) {
		case CYBERSPACE_COMMAND_MINIGAME_CAUSE_TRAINING_FAST:
			if(data->personal.fast_record<data->mini_fast.points) {
				SDL_mutexP(data->disk_mutex);
				data->personal.fast_record=data->mini_fast.points;
				resend_personal=true;
				}
			break;
		case CYBERSPACE_COMMAND_MINIGAME_CAUSE_PROGRAMMING:
			if(data->personal.fast_record<data->mini_fast.points) {
				SDL_mutexP(data->disk_mutex);
				data->personal.fast_record=data->mini_fast.points;
				resend_personal=true;
				}
			data->programming.mini_res_fast=data->mini_fast.points;
			programmed=true;
			if(programmed && data->programming.mini_req_heuristic>0) {
				programmed=(data->programming.mini_res_heuristic>=data->programming.mini_req_heuristic);
				}
			if(programmed && data->programming.mini_req_fast>0) {
				programmed=(data->programming.mini_res_fast>=data->programming.mini_req_fast);
				}
			if(programmed && data->programming.mini_req_patternmatching>0) {
				programmed=(data->programming.mini_res_patternmatching>=data->programming.mini_req_patternmatching);
				}
			if(programmed) {
				if(!resend_personal) SDL_mutexP(data->disk_mutex);
				data->personal.subsystems[data->programming.mini_cause_progr_sys][data->programming.mini_cause_progr_subsys]=data->personal.subsystems_hint[data->programming.mini_cause_progr_sys][data->programming.mini_cause_progr_subsys];
				if(data->personal.subsystems[data->programming.mini_cause_progr_sys][data->programming.mini_cause_progr_subsys]>data->personal.systems[data->programming.mini_cause_progr_sys]) data->personal.systems[data->programming.mini_cause_progr_sys]=data->personal.subsystems[data->programming.mini_cause_progr_sys][data->programming.mini_cause_progr_subsys];
				resend_personal=true;
				}
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PROGRAMMING;
			memcpy(data->out_command.data, &(data->programming), sizeof(data->programming));
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		default:
			fprintf(stderr, "WARNING: unexpected data->programming.mini_cause in minigame fast (%d) by connection %d (%d).\n", data->programming.mini_cause, data->personal.id_static, data->id_dynamic);
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
			errormessage=new char[100];
			sprintf(errormessage, "unexpected data->programming.mini_cause in minigame fast");
			memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		}
	if(resend_personal) {
		cyberspace_nonvolatile_save_personal(data);
		SDL_mutexV(data->disk_mutex);
		data->out_command.type=CYBERSPACE_COMMAND_SERVER_USER;
		memcpy(data->out_command.data, &(data->personal), sizeof(data->personal));
		cyberspace_user_connection_output(data, &(data->out_command));
		}
	data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_PROGRAMMING;
	cyberspace_user_connection_output(data, &(data->out_command));
	data->phase=CYBERSPACE_PHASE_PROGRAMMING;
	}

void cyberspace_minigame_fast_message2server(cyberspace_user_connection_data* data) {
	if(data->phase!=CYBERSPACE_PHASE_MINIGAME_FAST) {
		fprintf(stderr, "ERROR: data->phase!=CYBERSPACE_PHASE_MINIGAME_FAST (%d).\n", data->phase);
		exit(EXIT_FAILURE);
		}

	char* errormessage;
	char* path;
	int path_l;
	int tmp_x;
	int tmp_y;
	int start_x;
	int start_y;
	bool vinto=false;
	switch(data->in_command.type) {
		case CYBERSPACE_COMMAND_USER_MINIGAME_FAST_DONE:
			memcpy(&path_l, data->in_command.data, sizeof(path_l));
			printf("read path_l:%d\n", path_l);
			path=new char[path_l];
			memcpy(path, data->in_command.data+sizeof(path_l), path_l);
			printf("read path: ");
			for(int i=0;i<path_l;i++) {
				printf("%c", path[i]);
				}
			printf("\n");
			for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
				for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
					if(data->mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]
==CYBERSPACE_MINIGAME_FAST_MAP_START) {
						tmp_x=x;
						tmp_y=y;
						}
					if(data->mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]
==CYBERSPACE_MINIGAME_FAST_MAP_END) {
						start_x=x;
						start_y=y;
						}
					}
				}
			for(int i=0;i<path_l && !vinto;i++) {
				printf("debug fast: %d\n", i);
				switch(path[i]) {
					case 'w':
						while(data->mini_fast.floor[tmp_x+(tmp_y-1)*CYBERSPACE_MINIGAME_FAST_MAP_L]!=CYBERSPACE_MINIGAME_FAST_MAP_CAN) {
							tmp_y--;
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_RADIO) {
								fprintf(stderr, "ERROR: over radio!\n");
								exit(EXIT_FAILURE);
								}
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL) {
								data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_FLOOR;
								data->mini_fast.points++;
								}
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_END) {
								vinto=true;
								break;
								}
							}
						break;
					case 's':
						while(data->mini_fast.floor[tmp_x+(tmp_y+1)*CYBERSPACE_MINIGAME_FAST_MAP_L]!=CYBERSPACE_MINIGAME_FAST_MAP_CAN) {
							tmp_y++;
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_RADIO) {
								fprintf(stderr, "ERROR: over radio!\n");
								exit(EXIT_FAILURE);
								}
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL) {
								data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_FLOOR;
								data->mini_fast.points++;
								}
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_END) {
								vinto=true;
								break;
								}
							}
						break;
					case 'a':
						while(data->mini_fast.floor[tmp_x-1+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]!=CYBERSPACE_MINIGAME_FAST_MAP_CAN) {
							tmp_x--;
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_RADIO) {
								fprintf(stderr, "ERROR: over radio!\n");
								exit(EXIT_FAILURE);
								}
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL) {
								data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_FLOOR;
								data->mini_fast.points++;
								}
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_END) {
								vinto=true;
								break;
								}
							}
						break;
					case 'd':
						while(data->mini_fast.floor[tmp_x+1+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]!=CYBERSPACE_MINIGAME_FAST_MAP_CAN) {
							tmp_x++;
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_RADIO) {
								fprintf(stderr, "ERROR: over radio!\n");
								exit(EXIT_FAILURE);
								}
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL) {
								data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_FLOOR;
								data->mini_fast.points++;
								}
							if(data->mini_fast.floor[tmp_x+tmp_y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_END) {
								vinto=true;
								break;
								}
							}
						break;
					}
				}
			printf("debug fast: out\n");
			if(!vinto) {
				fprintf(stderr, "ERROR: over radio!\n");
				exit(EXIT_FAILURE);
				}
			data->mini_fast.points+=1+(data->mini_fast.points/20);
			printf("debug fast: newmap...\n");
			cyberspace_minigame_fast_newmap(data);
			printf("debug fast: done\n");
			data->mini_fast.milliseconds=1000+(30*1000)/(1+data->mini_fast.points/10);
			data->mini_fast.milliseconds+=1000;
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_FAST_POINTS_AND_MAP;
			memcpy(data->out_command.data, &(data->mini_fast.points), sizeof(data->mini_fast.points));
			memcpy(data->out_command.data+sizeof(data->mini_fast.points), data->mini_fast.floor, CYBERSPACE_MINIGAME_FAST_MAP_L*CYBERSPACE_MINIGAME_FAST_MAP_L);
			printf("debug fast: sending...\n");
			cyberspace_user_connection_output(data, &(data->out_command));
			printf("debug fast: done\n");
			break;
		default:
			fprintf(stderr, "WARNING: unexpected message in minigame fast (%d) by connection %d (%d).\n", data->in_command.type, data->personal.id_static, data->id_dynamic);
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
			errormessage=new char[100];
			sprintf(errormessage, "unexpected message in minigame fast");
			memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		}

	}

