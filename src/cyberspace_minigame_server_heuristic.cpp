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


#include "cyberspace_minigame_server_heuristic.h"



void cyberspace_minigame_heuristic_newpiece(cyberspace_user_connection_data* data) {
	data->mini_heuristic.piece[0]=random()%data->mini_heuristic.max_symbol;
	data->mini_heuristic.piece[1]=random()%data->mini_heuristic.max_symbol;
	}

void cyberspace_minigame_heuristic_init(cyberspace_user_connection_data* data) {
	for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
		for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
			data->mini_heuristic.board[y][x]=-1;
			}
		}
	data->mini_heuristic.max_symbol=3;
	data->mini_heuristic.points=0;
	cyberspace_minigame_heuristic_newpiece(data);
	data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_HEURISTIC_PIECE;
	memcpy(data->out_command.data, data->mini_heuristic.piece, 2*sizeof(int));
	cyberspace_user_connection_output(data, &(data->out_command));
	}

void cyberspace_minigame_heuristic_frame(cyberspace_user_connection_data* data, int milliseconds) {
	}

void cyberspace_minigame_heuristic_message2server(cyberspace_user_connection_data* data) {
	if(data->phase!=CYBERSPACE_PHASE_MINIGAME_HEURISTIC) {
		fprintf(stderr, "ERROR: data->phase!=CYBERSPACE_PHASE_MINIGAME_HEURISTIC (%d).\n", data->phase);
		exit(EXIT_FAILURE);
		}

	char* errormessage;
	int tmp_x, tmp_y;
	bool end=false;
	bool resend_personal=false;
	bool programmed;
	switch(data->in_command.type) {
		case CYBERSPACE_COMMAND_USER_MINIGAME_HEURISTIC_DROP:
			memcpy(&(data->mini_heuristic.piece_position), data->in_command.data, sizeof(int));
			memcpy(&(data->mini_heuristic.piece_status), data->in_command.data+sizeof(int), sizeof(int));
			switch(data->mini_heuristic.piece_status) {
				case 0:
					tmp_x=data->mini_heuristic.piece_position;
					tmp_y=0;
					while(data->mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					data->mini_heuristic.board[tmp_y][tmp_x]=data->mini_heuristic.piece[0];
					tmp_x=data->mini_heuristic.piece_position+1;
					tmp_y=0;
					while(data->mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					data->mini_heuristic.board[tmp_y][tmp_x]=data->mini_heuristic.piece[1];
					break;
				case 1:
					tmp_x=data->mini_heuristic.piece_position;
					tmp_y=0;
					while(data->mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					data->mini_heuristic.board[tmp_y][tmp_x]=data->mini_heuristic.piece[1];
					data->mini_heuristic.board[tmp_y+1][tmp_x]=data->mini_heuristic.piece[0];
					break;
				case 2:
					tmp_x=data->mini_heuristic.piece_position+1;
					tmp_y=0;
					while(data->mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					data->mini_heuristic.board[tmp_y][tmp_x]=data->mini_heuristic.piece[0];
					tmp_x=data->mini_heuristic.piece_position;
					tmp_y=0;
					while(data->mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					data->mini_heuristic.board[tmp_y][tmp_x]=data->mini_heuristic.piece[1];
					break;
				case 3:
					tmp_x=data->mini_heuristic.piece_position;
					tmp_y=0;
					while(data->mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					data->mini_heuristic.board[tmp_y][tmp_x]=data->mini_heuristic.piece[0];
					data->mini_heuristic.board[tmp_y+1][tmp_x]=data->mini_heuristic.piece[1];
					break;
				default:
					fprintf(stderr, "WARNING: wrong mini_heuristic.piece_status=%d by connection %d (%d).\n", data->mini_heuristic.piece_status, data->personal.id_static, data->id_dynamic);
					data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
					errormessage=new char[100];
					sprintf(errormessage, "wrong mini_heuristic.piece_status=%d", data->mini_heuristic.piece_status);
					memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
					delete[] errormessage;
					cyberspace_user_connection_output(data, &(data->out_command));
					return;
				}

			while(cyberspace_minigame_server_heuristic_ismorphing(data)) {
				for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
					for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
						data->mini_heuristic.board_morphing[y][x]=false;
						}
					}
				for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
					for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
						cyberspace_minigame_server_heuristic_morph(data, x, y);
						}
					}
				cyberspace_minigame_server_heuristic_gravity(data);
				}

			for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
				if(data->mini_heuristic.board[CYBERSPACE_MINIGAME_HEURISTIC_MAP_H][x]!=-1) end=true;
				}
			if(end) {
				int val[11];
				val[0]=1;
				val[1]=1;
				val[2]=1;
				for(int i=3;i<11;i++) val[i]=val[i-1]*3;
				data->mini_heuristic.points=0;
				for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
					for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
						if(data->mini_heuristic.board[y][x]!=-1) data->mini_heuristic.points+=val[data->mini_heuristic.board[y][x]];
						}
					}
				switch(data->programming.mini_cause) {
					case CYBERSPACE_COMMAND_MINIGAME_CAUSE_TRAINING_HEURISTIC:
						if(data->personal.heuristic_record<data->mini_heuristic.points) {
							SDL_mutexP(data->disk_mutex);
							data->personal.heuristic_record=data->mini_heuristic.points;
							resend_personal=true;
							}
						break;
					case CYBERSPACE_COMMAND_MINIGAME_CAUSE_PROGRAMMING:
						if(data->personal.heuristic_record<data->mini_heuristic.points) {
							SDL_mutexP(data->disk_mutex);
							data->personal.heuristic_record=data->mini_heuristic.points;
							resend_personal=true;
							}
						data->programming.mini_res_heuristic=data->mini_heuristic.points;
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
						fprintf(stderr, "WARNING: unexpected data->programming.mini_cause in minigame heuristic (%d) by connection %d (%d).\n", data->programming.mini_cause, data->personal.id_static, data->id_dynamic);
						data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
						errormessage=new char[100];
						sprintf(errormessage, "unexpected data->programming.mini_cause in minigame heuristic");
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
				break;
				}
			cyberspace_minigame_heuristic_newpiece(data);
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_HEURISTIC_PIECE;
			memcpy(data->out_command.data, data->mini_heuristic.piece, 2*sizeof(int));
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		default:
			fprintf(stderr, "WARNING: unexpected message in minigame heuristic (%d) by connection %d (%d).\n", data->in_command.type, data->personal.id_static, data->id_dynamic);
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
			errormessage=new char[100];
			sprintf(errormessage, "unexpected message in minigame heuristic");
			memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		}
	}

bool cyberspace_minigame_server_heuristic_ismorphing(cyberspace_user_connection_data* data) {
	int icon_id;
	int count;
	for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
		for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
			count=1;
			icon_id=data->mini_heuristic.board[y][x];
			if(icon_id==-1 || icon_id==10) continue;
			if(y>0 && data->mini_heuristic.board[y-1][x]==icon_id) count++;
			if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1 && data->mini_heuristic.board[y+1][x]==icon_id) count++;
			if(x>0 && data->mini_heuristic.board[y][x-1]==icon_id) count++;
			if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-1 && data->mini_heuristic.board[y][x+1]==icon_id) count++;
			if(count>=3) return(true);
			}
		}
	return(false);
	}

void cyberspace_minigame_server_heuristic_morph(cyberspace_user_connection_data* data, int x, int y) {
	int icon_id;
	int tmp_x;
	int tmp_y;
	int count=1;
	if(data->mini_heuristic.board_morphing[y][x]) return;
	icon_id=data->mini_heuristic.board[y][x];
	if(icon_id==-1 || icon_id==10) return;
	if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1) {
		tmp_x=x;
		tmp_y=y+1;
		if(!data->mini_heuristic.board_morphing[tmp_y][tmp_x] && data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) {
			count++;
			if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H) {
				tmp_x=x;
				tmp_y=y+2;
				if(!data->mini_heuristic.board_morphing[tmp_y][tmp_x] && data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) count++;
				}
			if(x>0) {
				tmp_x=x-1;
				tmp_y=y+1;
				if(!data->mini_heuristic.board_morphing[tmp_y][tmp_x] && data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) count++;
				}
			if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-1) {
				tmp_x=x+1;
				tmp_y=y+1;
				if(!data->mini_heuristic.board_morphing[tmp_y][tmp_x] && data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) count++;
				}
			}
		}
	if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-1) {
		tmp_x=x+1;
		tmp_y=y;
		if(!data->mini_heuristic.board_morphing[tmp_y][tmp_x] && data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) {
			count++;
			if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-2) {
				tmp_x=x+2;
				tmp_y=y;
				if(!data->mini_heuristic.board_morphing[tmp_y][tmp_x] && data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) count++;
				}
			if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1) {
				tmp_x=x+1;
				tmp_y=y+1;
				if(!data->mini_heuristic.board_morphing[tmp_y][tmp_x] && data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) count++;
				}
			}
		}
	if(count<3) return;
	data->mini_heuristic.board_morphing[y][x]=true;
	data->mini_heuristic.board[y][x]++;

	if(data->mini_heuristic.board[y][x]>data->mini_heuristic.max_symbol) data->mini_heuristic.max_symbol=data->mini_heuristic.board[y][x];

	if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1) {
		tmp_x=x;
		tmp_y=y+1;
		if(data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_server_heuristic_morph_rec(data, tmp_x, tmp_y, icon_id);
		}
	if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-1) {
		tmp_x=x+1;
		tmp_y=y;
		if(data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_server_heuristic_morph_rec(data, tmp_x, tmp_y, icon_id);
		}
	}

void cyberspace_minigame_server_heuristic_morph_rec(cyberspace_user_connection_data* data, int x, int y, int icon_id) {
	if(data->mini_heuristic.board_morphing[y][x]) return;
	int tmp_x;
	int tmp_y;
	data->mini_heuristic.board_morphing[y][x]=true;
	data->mini_heuristic.board[y][x]=-1;
	if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1) {
		tmp_x=x;
		tmp_y=y+1;
		if(data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_server_heuristic_morph_rec(data, tmp_x, tmp_y, icon_id);
		}
	if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-1) {
		tmp_x=x+1;
		tmp_y=y;
		if(data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_server_heuristic_morph_rec(data, tmp_x, tmp_y, icon_id);
		}
	if(y>0) {
		tmp_x=x;
		tmp_y=y-1;
		if(data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_server_heuristic_morph_rec(data, tmp_x, tmp_y, icon_id);
		}
	if(x>0) {
		tmp_x=x-1;
		tmp_y=y;
		if(data->mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_server_heuristic_morph_rec(data, tmp_x, tmp_y, icon_id);
		}
	}

void cyberspace_minigame_server_heuristic_gravity(cyberspace_user_connection_data* data) {
	int top;
	for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
		top=CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1;
		while(data->mini_heuristic.board[top][x]==-1 && top>0) top--;
		for(int y=top-1;y>=0;y--) {
			if(data->mini_heuristic.board[y][x]==-1) {
				for(int y2=y;y2<=top;y2++) data->mini_heuristic.board[y2][x]=data->mini_heuristic.board[y2+1][x];
				data->mini_heuristic.board[top][x]=-1;
				}
			}
		}
	}

