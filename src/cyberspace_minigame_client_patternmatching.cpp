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


#include "cyberspace_minigame_client_patternmatching.h"



void cyberspace_minigame_patternmatching_inputkey(int key) {
//	if(mini_patternmatching_sent) return;
	}

void cyberspace_minigame_patternmatching_print(void) {
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

	cyberspace_GUI_font_print(10, y_tmp, 100, "minigame patternmatching");
	y_tmp+=20;
	if(cyberspace_server_message_fade>0 && strlen(cyberspace_server_message)>0) {
		glColor4f(1.0, 0.0, 0.0, 1.0);
		cyberspace_GUI_font_print((cyberspace_screen_width-(strlen(cyberspace_server_message)*10))/2, 20, 100, cyberspace_server_message);
		glColor4f(0.0, 0.8, 0.0, 1.0);
		}

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, cyberspace_textures_raw[CYBERSPACE_GUI_TEXTURE_TILE_32_FLOOR]);

	glPushMatrix();
	glTranslatef(200.0, 50.0, 0.0);
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

