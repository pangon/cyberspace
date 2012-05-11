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


#include "cyberspace_minigame_client_fast.h"



void cyberspace_minigame_fast_inputkey(int key) {
	if(mini_fast_sent) return;
	SDL_mutexP(cyberspace_mutex_phase_sdl);
	int next;
	bool bloccato=false;
	bool morto=false;
	bool vinto=false;
	if(mini_fast_path_l>195) morto=true;
	switch(key) {
		case SDLK_UP:
			next=mini_fast.floor[mini_fast_p_x+(mini_fast_p_y-1)*CYBERSPACE_MINIGAME_FAST_MAP_L];
			if(next==CYBERSPACE_MINIGAME_FAST_MAP_CAN) break;
			mini_fast_path[mini_fast_path_l++]='w';
			for(int y=mini_fast_p_y-1;y>=0;y--) {
				next=mini_fast.floor[mini_fast_p_x+y*CYBERSPACE_MINIGAME_FAST_MAP_L];
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_CAN) {
					bloccato=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_RADIO) {
					morto=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_END) {
					vinto=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL) {
					mini_fast.floor[mini_fast_p_x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL_GET;
					}
				mini_fast_p_y=y;
				}
			break;
		case SDLK_DOWN:
			next=mini_fast.floor[mini_fast_p_x+(mini_fast_p_y+1)*CYBERSPACE_MINIGAME_FAST_MAP_L];
			if(next==CYBERSPACE_MINIGAME_FAST_MAP_CAN) break;
			mini_fast_path[mini_fast_path_l++]='s';
			for(int y=mini_fast_p_y+1;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
				next=mini_fast.floor[mini_fast_p_x+y*CYBERSPACE_MINIGAME_FAST_MAP_L];
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_CAN) {
					bloccato=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_RADIO) {
					morto=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_END) {
					vinto=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL) {
					mini_fast.floor[mini_fast_p_x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL_GET;
					}
				mini_fast_p_y=y;
				}
			break;
		case SDLK_LEFT:
			next=mini_fast.floor[mini_fast_p_x-1+mini_fast_p_y*CYBERSPACE_MINIGAME_FAST_MAP_L];
			if(next==CYBERSPACE_MINIGAME_FAST_MAP_CAN) break;
			mini_fast_path[mini_fast_path_l++]='a';
			for(int x=mini_fast_p_x-1;x>=0;x--) {
				next=mini_fast.floor[x+mini_fast_p_y*CYBERSPACE_MINIGAME_FAST_MAP_L];
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_CAN) {
					bloccato=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_RADIO) {
					morto=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_END) {
					vinto=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL) {
					mini_fast.floor[x+mini_fast_p_y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL_GET;
					}
				mini_fast_p_x=x;
				}
			break;
		case SDLK_RIGHT:
			next=mini_fast.floor[mini_fast_p_x+1+mini_fast_p_y*CYBERSPACE_MINIGAME_FAST_MAP_L];
			if(next==CYBERSPACE_MINIGAME_FAST_MAP_CAN) break;
			mini_fast_path[mini_fast_path_l++]='d';
			for(int x=mini_fast_p_x+1;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
				next=mini_fast.floor[x+mini_fast_p_y*CYBERSPACE_MINIGAME_FAST_MAP_L];
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_CAN) {
					bloccato=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_RADIO) {
					morto=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_END) {
					vinto=true;
					break;
					}
				if(next==CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL) {
					mini_fast.floor[x+mini_fast_p_y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL_GET;
					}
				mini_fast_p_x=x;
				}
			break;
		}
	if(morto) {
		mini_fast_path_l=0;
		for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
			for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
				if(mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_START) {
					mini_fast_p_x=x;
					mini_fast_p_y=y;
					}
				if(mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL_GET) {
					mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]=CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL;
					}
				}
			}
		SDL_mutexV(cyberspace_mutex_phase_sdl);
		return;
		}
	if(vinto) {
		mini_fast_sent=true;
		cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_MINIGAME_FAST_DONE;
		memcpy(cyberspace_output_command.data, &mini_fast_path_l, sizeof(mini_fast_path_l));
		memcpy(cyberspace_output_command.data+sizeof(mini_fast_path_l), mini_fast_path, mini_fast_path_l);
		cyberspace_neural_connector_output(&cyberspace_output_command);
		}
	SDL_mutexV(cyberspace_mutex_phase_sdl);
	}

void cyberspace_minigame_fast_print(void) {
	if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();

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

	cyberspace_GUI_font_print(10, y_tmp, 100, "programming module: fast algorithms");
	y_tmp+=20;
	if(cyberspace_server_message_fade>0 && strlen(cyberspace_server_message)>0) {
		glColor4f(1.0, 0.0, 0.0, 1.0);
		cyberspace_GUI_font_print((cyberspace_screen_width-(strlen(cyberspace_server_message)*10))/2, 20, 100, cyberspace_server_message);
		glColor4f(0.0, 0.8, 0.0, 1.0);
		}

	if(gettimeofday(&mini_fast_now, NULL)!=0) {
		fprintf(stderr, "FATAL ERROR: gettimeofday fails.\n");
		exit(EXIT_FAILURE);
		}

	mini_fast_milliseconds_left-=(1000*(mini_fast_now.tv_sec-mini_fast_start.tv_sec)+(mini_fast_now.tv_usec-mini_fast_start.tv_usec)/1000);

	mini_fast_start=mini_fast_now;

	sprintf(msg, "tempo rimanente: %d", mini_fast_milliseconds_left);
	cyberspace_GUI_font_print(10, y_tmp, 100, msg);
	y_tmp+=20;

	sprintf(msg, "punti: %d", mini_fast.points);
	cyberspace_GUI_font_print(10, y_tmp, 100, msg);
	y_tmp+=20;

	sprintf(msg, "percorso: ");
	for(int i=0;i<mini_fast_path_l;i++) {
		sprintf(msg, "%s%c", msg, mini_fast_path[i]);
		}
	cyberspace_GUI_font_print(10, y_tmp, 100, msg);
	y_tmp+=20;

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, cyberspace_textures_raw[CYBERSPACE_GUI_TEXTURE_TILE_32_FLOOR]);

	glPushMatrix();
	glTranslatef(200.0, 50.0, 0.0);
	for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
		glPushMatrix();
		for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
			glBegin(GL_QUADS);
				glTexCoord2f(0.0, 0.0); glVertex2i(0, 0);
				glTexCoord2f(1.0, 0.0); glVertex2i(32, 0);
				glTexCoord2f(1.0, 1.0); glVertex2i(32, 32);
				glTexCoord2f(0.0, 1.0);	glVertex2i(0, 32);
			glEnd();
			glTranslatef(32.0, 0.0, 0.0);
			}
		glPopMatrix();
		glTranslatef(0.0, 32.0, 0.0);
		}
	glPopMatrix();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, cyberspace_textures_raw[CYBERSPACE_GUI_TEXTURE_TILE_32_RADIO]);

	glPushMatrix();
	glTranslatef(200.0, 50.0, 0.0);
	for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
		glPushMatrix();
		for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
			if(mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_RADIO) {
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0); glVertex2i(0, 0);
					glTexCoord2f(1.0, 0.0); glVertex2i(32, 0);
					glTexCoord2f(1.0, 1.0); glVertex2i(32, 32);
					glTexCoord2f(0.0, 1.0);	glVertex2i(0, 32);
				glEnd();
				}
			glTranslatef(32.0, 0.0, 0.0);
			}
		glPopMatrix();
		glTranslatef(0.0, 32.0, 0.0);
		}
	glPopMatrix();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, cyberspace_textures_raw[CYBERSPACE_GUI_TEXTURE_TILE_32_CAN]);

	glPushMatrix();
	glTranslatef(200.0, 50.0, 0.0);
	for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
		glPushMatrix();
		for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
			if(mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_CAN) {
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0); glVertex2i(0, 0);
					glTexCoord2f(1.0, 0.0); glVertex2i(32, 0);
					glTexCoord2f(1.0, 1.0); glVertex2i(32, 32);
					glTexCoord2f(0.0, 1.0);	glVertex2i(0, 32);
				glEnd();
				}
			glTranslatef(32.0, 0.0, 0.0);
			}
		glPopMatrix();
		glTranslatef(0.0, 32.0, 0.0);
		}
	glPopMatrix();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, cyberspace_textures_raw[CYBERSPACE_GUI_TEXTURE_TILE_32_CRYSTAL]);

	glPushMatrix();
	glTranslatef(200.0, 50.0, 0.0);
	for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
		glPushMatrix();
		for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
			if(mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_CRYSTAL) {
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0); glVertex2i(0, 0);
					glTexCoord2f(1.0, 0.0); glVertex2i(32, 0);
					glTexCoord2f(1.0, 1.0); glVertex2i(32, 32);
					glTexCoord2f(0.0, 1.0);	glVertex2i(0, 32);
				glEnd();
				}
			glTranslatef(32.0, 0.0, 0.0);
			}
		glPopMatrix();
		glTranslatef(0.0, 32.0, 0.0);
		}
	glPopMatrix();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, cyberspace_textures_raw[CYBERSPACE_GUI_TEXTURE_TILE_32_START]);

	glPushMatrix();
	glTranslatef(200.0, 50.0, 0.0);
	for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
		glPushMatrix();
		for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
			if(mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_START) {
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0); glVertex2i(0, 0);
					glTexCoord2f(1.0, 0.0); glVertex2i(32, 0);
					glTexCoord2f(1.0, 1.0); glVertex2i(32, 32);
					glTexCoord2f(0.0, 1.0);	glVertex2i(0, 32);
				glEnd();
				}
			glTranslatef(32.0, 0.0, 0.0);
			}
		glPopMatrix();
		glTranslatef(0.0, 32.0, 0.0);
		}
	glPopMatrix();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, cyberspace_textures_raw[CYBERSPACE_GUI_TEXTURE_TILE_32_END]);

	glPushMatrix();
	glTranslatef(200.0, 50.0, 0.0);
	for(int y=0;y<CYBERSPACE_MINIGAME_FAST_MAP_L;y++) {
		glPushMatrix();
		for(int x=0;x<CYBERSPACE_MINIGAME_FAST_MAP_L;x++) {
			if(mini_fast.floor[x+y*CYBERSPACE_MINIGAME_FAST_MAP_L]==CYBERSPACE_MINIGAME_FAST_MAP_END) {
				glBegin(GL_QUADS);
					glTexCoord2f(0.0, 0.0); glVertex2i(0, 0);
					glTexCoord2f(1.0, 0.0); glVertex2i(32, 0);
					glTexCoord2f(1.0, 1.0); glVertex2i(32, 32);
					glTexCoord2f(0.0, 1.0);	glVertex2i(0, 32);
				glEnd();
				}
			glTranslatef(32.0, 0.0, 0.0);
			}
		glPopMatrix();
		glTranslatef(0.0, 32.0, 0.0);
		}
	glPopMatrix();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, cyberspace_textures_raw[CYBERSPACE_GUI_TEXTURE_TILE_32_CULT_0]);

	glPushMatrix();
	glTranslatef(200.0+32.0*mini_fast_p_x, 50.0+32.0*mini_fast_p_y, 0.0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex2i(0, 0);
		glTexCoord2f(1.0, 0.0); glVertex2i(32, 0);
		glTexCoord2f(1.0, 1.0); glVertex2i(32, 32);
		glTexCoord2f(0.0, 1.0);	glVertex2i(0, 32);
	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);

	glFlush();
	SDL_GL_SwapBuffers();

	delete[] msg;
	}

