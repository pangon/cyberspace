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


#include "cyberspace_minigame_client_heuristic.h"



void cyberspace_minigame_heuristic_inputkey(int key) {
	if(mini_heuristic_sent) return;
	int tmp_x, tmp_y;
	SDL_mutexP(cyberspace_mutex_phase_sdl);
	switch(key) {
		case SDLK_UP:
			mini_heuristic.piece_status++;
			mini_heuristic.piece_status=mini_heuristic.piece_status%4;
			if(mini_heuristic.piece_position==(CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-1) && (mini_heuristic.piece_status==0 || mini_heuristic.piece_status==2)) mini_heuristic.piece_position=CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-2;
			break;
		case SDLK_DOWN:
			if(mini_heuristic_morphing) break;
			mini_heuristic_sent=true;
			switch(mini_heuristic.piece_status) {
				case 0:
					tmp_x=mini_heuristic.piece_position;
					tmp_y=0;
					while(mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					mini_heuristic.board[tmp_y][tmp_x]=mini_heuristic.piece[0];
					tmp_x=mini_heuristic.piece_position+1;
					tmp_y=0;
					while(mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					mini_heuristic.board[tmp_y][tmp_x]=mini_heuristic.piece[1];
					break;
				case 1:
					tmp_x=mini_heuristic.piece_position;
					tmp_y=0;
					while(mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					mini_heuristic.board[tmp_y][tmp_x]=mini_heuristic.piece[1];
					mini_heuristic.board[tmp_y+1][tmp_x]=mini_heuristic.piece[0];
					break;
				case 2:
					tmp_x=mini_heuristic.piece_position+1;
					tmp_y=0;
					while(mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					mini_heuristic.board[tmp_y][tmp_x]=mini_heuristic.piece[0];
					tmp_x=mini_heuristic.piece_position;
					tmp_y=0;
					while(mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					mini_heuristic.board[tmp_y][tmp_x]=mini_heuristic.piece[1];
					break;
				case 3:
					tmp_x=mini_heuristic.piece_position;
					tmp_y=0;
					while(mini_heuristic.board[tmp_y][tmp_x]!=-1) tmp_y++;
					mini_heuristic.board[tmp_y][tmp_x]=mini_heuristic.piece[0];
					mini_heuristic.board[tmp_y+1][tmp_x]=mini_heuristic.piece[1];
					break;
				default:
					fprintf(stderr, "FATAL ERROR: wrong mini_heuristic.piece_status=%d. (in down)\n", mini_heuristic.piece_status);
					exit(EXIT_FAILURE);
				}
			mini_heuristic.points=0;
			for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
				for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
					if(mini_heuristic.board[y][x]!=-1) mini_heuristic.points+=mini_heuristic_values[mini_heuristic.board[y][x]];
					}
				}
			if(cyberspace_minigame_heuristic_ismorphing()) {
				mini_heuristic_morphing=true;
				for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
					for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
						mini_heuristic.board_morphing[y][x]=false;
						}
					}
				for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
					for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
						cyberspace_minigame_heuristic_morph(x, y);
						}
					}
				if(gettimeofday(&mini_heuristic_morph_start, NULL)!=0) {
					fprintf(stderr, "FATAL ERROR: gettimeofday fails.\n");
					exit(EXIT_FAILURE);
					}
				}
			else {
				cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_MINIGAME_HEURISTIC_DROP;
				memcpy(cyberspace_output_command.data, &mini_heuristic.piece_position, sizeof(mini_heuristic.piece_position));
				memcpy(cyberspace_output_command.data+sizeof(mini_heuristic.piece_position), &mini_heuristic.piece_status, sizeof(mini_heuristic.piece_status));
				cyberspace_neural_connector_output(&cyberspace_output_command);
				}
			break;
		case SDLK_LEFT:
			if(mini_heuristic.piece_position>0) mini_heuristic.piece_position--;
			break;
		case SDLK_RIGHT:
			if(mini_heuristic.piece_position<(CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-2)) {
				mini_heuristic.piece_position++;
				break;
				}
			if(mini_heuristic.piece_position==(CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-2) && (mini_heuristic.piece_status==1 || mini_heuristic.piece_status==3)) mini_heuristic.piece_position++;
			break;
		}
	SDL_mutexV(cyberspace_mutex_phase_sdl);
	}

void cyberspace_minigame_heuristic_print(void) {
	if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();

	if(mini_heuristic_morphing) {
		if(gettimeofday(&mini_heuristic_morph_now, NULL)!=0) {
			fprintf(stderr, "FATAL ERROR: gettimeofday fails.\n");
			exit(EXIT_FAILURE);
			}
		if((1000*(mini_heuristic_morph_now.tv_sec-mini_heuristic_morph_start.tv_sec)+(mini_heuristic_morph_now.tv_usec-mini_heuristic_morph_start.tv_usec)/1000)>=500) {
			cyberspace_minigame_heuristic_gravity();
			mini_heuristic_morphing=false;
			mini_heuristic.points=0;
			for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
				for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
					if(mini_heuristic.board[y][x]!=-1) mini_heuristic.points+=mini_heuristic_values[mini_heuristic.board[y][x]];
					}
				}
			if(cyberspace_minigame_heuristic_ismorphing()) {
				mini_heuristic_morphing=true;
				for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
					for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
						mini_heuristic.board_morphing[y][x]=false;
						}
					}
				for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
					for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
						cyberspace_minigame_heuristic_morph(x, y);
						}
					}
				if(gettimeofday(&mini_heuristic_morph_start, NULL)!=0) {
					fprintf(stderr, "FATAL ERROR: gettimeofday fails.\n");
					exit(EXIT_FAILURE);
					}
				}
			else {
				cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_MINIGAME_HEURISTIC_DROP;
				memcpy(cyberspace_output_command.data, &mini_heuristic.piece_position, sizeof(mini_heuristic.piece_position));
				memcpy(cyberspace_output_command.data+sizeof(mini_heuristic.piece_position), &mini_heuristic.piece_status, sizeof(mini_heuristic.piece_status));
				cyberspace_neural_connector_output(&cyberspace_output_command);
				}
			}
		}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glShadeModel(GL_FLAT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, double(cyberspace_screen_width)-1.0, double(cyberspace_screen_height)-1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(0.0, 0.8, 0.0, 1.0);
	int y_tmp=20;
	char* msg=new char[1000];

	cyberspace_GUI_font_print(10, y_tmp, 100, "programming module: heuristic");
	y_tmp+=20;
	if(cyberspace_server_message_fade>0 && strlen(cyberspace_server_message)>0) {
		glColor4f(1.0, 0.0, 0.0, 1.0);
		cyberspace_GUI_font_print((cyberspace_screen_width-(strlen(cyberspace_server_message)*10))/2, 20, 100, cyberspace_server_message);
		glColor4f(0.0, 0.8, 0.0, 1.0);
		}

	sprintf(msg, "punti: %d", mini_heuristic.points);
	cyberspace_GUI_font_print(10, y_tmp, 100, msg);
	y_tmp+=20;

	int icon_l=40;

	glColor4f(0.5, 0.5, 0.5, 1.0);
	glPushMatrix();
	glTranslatef(200.0, 20.0+double(4*(icon_l+1)), 0.0);
	for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H;y++) {
		glPushMatrix();
		for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
			glBegin(GL_QUADS);
				glVertex2i(0, 0);
				glVertex2i(icon_l, 0);
				glVertex2i(icon_l, icon_l);
				glVertex2i(0, icon_l);
			glEnd();
			glTranslatef(double(icon_l+1), 0.0, 0.0);
			}
		glPopMatrix();
		glTranslatef(0.0, double(icon_l+1), 0.0);
		}
	glPopMatrix();
	glColor4f(0.0, 0.8, 0.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, cyberspace_textures_raw[CYBERSPACE_GUI_TEXTURE_TILES_4X4_64_ICONS]);

	int icon_id;
	glPushMatrix();
	glTranslatef(200.0, 20.0+double((CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+4-1)*(icon_l+1)), 0.0);
	for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
		glPushMatrix();
		for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
			icon_id=mini_heuristic.board[y][x];
			if(mini_heuristic_morphing && mini_heuristic.board_morphing[y][x]) icon_id=11;
			if(icon_id!=-1) {
				glBegin(GL_QUADS);
					glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][0], cyberspace_clips_4x4_64_y[icon_id][0]); glVertex2i(0, 0);
					glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][1], cyberspace_clips_4x4_64_y[icon_id][0]); glVertex2i(icon_l, 0);
					glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][1], cyberspace_clips_4x4_64_y[icon_id][1]); glVertex2i(icon_l, icon_l);
					glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][0], cyberspace_clips_4x4_64_y[icon_id][1]); glVertex2i(0, icon_l);
				glEnd();
				}
			glTranslatef(double(icon_l+1), 0.0, 0.0);
			}
		glPopMatrix();
		glTranslatef(0.0, -double(icon_l+1), 0.0);
		}
	glPopMatrix();

	if(!mini_heuristic_sent) {
		int icon_x, icon_y;
		icon_id=mini_heuristic.piece[0];
		switch(mini_heuristic.piece_status) {
			case 0:
				icon_x=mini_heuristic.piece_position;
				icon_y=1;
				break;
			case 1:
				icon_x=mini_heuristic.piece_position;
				icon_y=0;
				break;
			case 2:
				icon_x=mini_heuristic.piece_position+1;
				icon_y=1;
				break;
			case 3:
				icon_x=mini_heuristic.piece_position;
				icon_y=1;
				break;
			default:
				fprintf(stderr, "FATAL ERROR: wrong mini_heuristic.piece_status=%d.\n", mini_heuristic.piece_status);
				exit(EXIT_FAILURE);
			}
		glPushMatrix();
		glTranslatef(200.0+double(icon_x*(icon_l+1)), 20.0+double(icon_y*(icon_l+1)), 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][0], cyberspace_clips_4x4_64_y[icon_id][0]); glVertex2i(0, 0);
			glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][1], cyberspace_clips_4x4_64_y[icon_id][0]); glVertex2i(icon_l, 0);
			glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][1], cyberspace_clips_4x4_64_y[icon_id][1]); glVertex2i(icon_l, icon_l);
			glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][0], cyberspace_clips_4x4_64_y[icon_id][1]); glVertex2i(0, icon_l);
		glEnd();
		glPopMatrix();
		icon_id=mini_heuristic.piece[1];
		switch(mini_heuristic.piece_status) {
			case 0:
				icon_x=mini_heuristic.piece_position+1;
				icon_y=1;
				break;
			case 1:
				icon_x=mini_heuristic.piece_position;
				icon_y=1;
				break;
			case 2:
				icon_x=mini_heuristic.piece_position;
				icon_y=1;
				break;
			case 3:
				icon_x=mini_heuristic.piece_position;
				icon_y=0;
				break;
			}
		glPushMatrix();
		glTranslatef(200.0+double(icon_x*(icon_l+1)), 20.0+double(icon_y*(icon_l+1)), 0.0);
		glBegin(GL_QUADS);
			glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][0], cyberspace_clips_4x4_64_y[icon_id][0]); glVertex2i(0, 0);
			glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][1], cyberspace_clips_4x4_64_y[icon_id][0]); glVertex2i(icon_l, 0);
			glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][1], cyberspace_clips_4x4_64_y[icon_id][1]); glVertex2i(icon_l, icon_l);
			glTexCoord2f(cyberspace_clips_4x4_64_x[icon_id][0], cyberspace_clips_4x4_64_y[icon_id][1]); glVertex2i(0, icon_l);
		glEnd();
		glPopMatrix();
		}

	glPushMatrix();
	glTranslatef(200.0+double((CYBERSPACE_MINIGAME_HEURISTIC_MAP_W+1)*(icon_l+1)), 20.0+double((CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+3)*(icon_l+1)), 0.0);
	for(int i=0;i<=mini_heuristic.max_symbol;i++) {
		glBegin(GL_QUADS);
			glTexCoord2f(cyberspace_clips_4x4_64_x[i][0], cyberspace_clips_4x4_64_y[i][0]); glVertex2i(0, 0);
			glTexCoord2f(cyberspace_clips_4x4_64_x[i][1], cyberspace_clips_4x4_64_y[i][0]); glVertex2i(icon_l, 0);
			glTexCoord2f(cyberspace_clips_4x4_64_x[i][1], cyberspace_clips_4x4_64_y[i][1]); glVertex2i(icon_l, icon_l);
			glTexCoord2f(cyberspace_clips_4x4_64_x[i][0], cyberspace_clips_4x4_64_y[i][1]); glVertex2i(0, icon_l);
		glEnd();
		glTranslatef(0.0, -double(icon_l+1), 0.0);
		}
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);

	glFlush();
	SDL_GL_SwapBuffers();

	delete[] msg;
	}

bool cyberspace_minigame_heuristic_ismorphing(void) {
	int icon_id;
	int count;
	for(int y=0;y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+2;y++) {
		for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
			count=1;
			icon_id=mini_heuristic.board[y][x];
			if(icon_id==-1 || icon_id==10) continue;
			if(y>0 && mini_heuristic.board[y-1][x]==icon_id) count++;
			if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1 && mini_heuristic.board[y+1][x]==icon_id) count++;
			if(x>0 && mini_heuristic.board[y][x-1]==icon_id) count++;
			if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-1 && mini_heuristic.board[y][x+1]==icon_id) count++;
			if(count>=3) return(true);
			}
		}
	return(false);
	}

void cyberspace_minigame_heuristic_morph(int x, int y) {
	int icon_id;
	int tmp_x;
	int tmp_y;
	int count=1;
	if(mini_heuristic.board_morphing[y][x]) return;
	icon_id=mini_heuristic.board[y][x];
	if(icon_id==-1 || icon_id==10) return;
	if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1) {
		tmp_x=x;
		tmp_y=y+1;
		if(!mini_heuristic.board_morphing[tmp_y][tmp_x] && mini_heuristic.board[tmp_y][tmp_x]==icon_id) {
			count++;
			if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H) {
				tmp_x=x;
				tmp_y=y+2;
				if(!mini_heuristic.board_morphing[tmp_y][tmp_x] && mini_heuristic.board[tmp_y][tmp_x]==icon_id) count++;
				}
			if(x>0) {
				tmp_x=x-1;
				tmp_y=y+1;
				if(!mini_heuristic.board_morphing[tmp_y][tmp_x] && mini_heuristic.board[tmp_y][tmp_x]==icon_id) count++;
				}
			if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-1) {
				tmp_x=x+1;
				tmp_y=y+1;
				if(!mini_heuristic.board_morphing[tmp_y][tmp_x] && mini_heuristic.board[tmp_y][tmp_x]==icon_id) count++;
				}
			}
		}
	if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-1) {
		tmp_x=x+1;
		tmp_y=y;
		if(!mini_heuristic.board_morphing[tmp_y][tmp_x] && mini_heuristic.board[tmp_y][tmp_x]==icon_id) {
			count++;
			if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-2) {
				tmp_x=x+2;
				tmp_y=y;
				if(!mini_heuristic.board_morphing[tmp_y][tmp_x] && mini_heuristic.board[tmp_y][tmp_x]==icon_id) count++;
				}
			if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1) {
				tmp_x=x+1;
				tmp_y=y+1;
				if(!mini_heuristic.board_morphing[tmp_y][tmp_x] && mini_heuristic.board[tmp_y][tmp_x]==icon_id) count++;
				}
			}
		}
	if(count<3) return;
	mini_heuristic.board_morphing[y][x]=true;
	mini_heuristic.board[y][x]++;
	if(mini_heuristic.board[y][x]>mini_heuristic.max_symbol) mini_heuristic.max_symbol=mini_heuristic.board[y][x];
	if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1) {
		tmp_x=x;
		tmp_y=y+1;
		if(mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_heuristic_morph_rec(tmp_x, tmp_y, icon_id);
		}
	if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-1) {
		tmp_x=x+1;
		tmp_y=y;
		if(mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_heuristic_morph_rec(tmp_x, tmp_y, icon_id);
		}
	}

void cyberspace_minigame_heuristic_morph_rec(int x, int y, int icon_id) {
	if(mini_heuristic.board_morphing[y][x]) return;
	int tmp_x;
	int tmp_y;
	mini_heuristic.board_morphing[y][x]=true;
	mini_heuristic.board[y][x]=-1;
	if(y<CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1) {
		tmp_x=x;
		tmp_y=y+1;
		if(mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_heuristic_morph_rec(tmp_x, tmp_y, icon_id);
		}
	if(x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W-1) {
		tmp_x=x+1;
		tmp_y=y;
		if(mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_heuristic_morph_rec(tmp_x, tmp_y, icon_id);
		}
	if(y>0) {
		tmp_x=x;
		tmp_y=y-1;
		if(mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_heuristic_morph_rec(tmp_x, tmp_y, icon_id);
		}
	if(x>0) {
		tmp_x=x-1;
		tmp_y=y;
		if(mini_heuristic.board[tmp_y][tmp_x]==icon_id) cyberspace_minigame_heuristic_morph_rec(tmp_x, tmp_y, icon_id);
		}
	}

void cyberspace_minigame_heuristic_gravity(void) {
	int top;
	for(int x=0;x<CYBERSPACE_MINIGAME_HEURISTIC_MAP_W;x++) {
		top=CYBERSPACE_MINIGAME_HEURISTIC_MAP_H+1;
		while(mini_heuristic.board[top][x]==-1 && top>0) top--;
		for(int y=top-1;y>=0;y--) {
			if(mini_heuristic.board[y][x]==-1) {
				for(int y2=y;y2<=top;y2++) mini_heuristic.board[y2][x]=mini_heuristic.board[y2+1][x];
				mini_heuristic.board[top][x]=-1;
				}
			}
		}
	}

