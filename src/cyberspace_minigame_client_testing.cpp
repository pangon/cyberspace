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


#include "cyberspace_minigame_client_testing.h"



void cyberspace_minigame_testing_inputkey(int key) {
	switch(key) {
		case SDLK_a:
			cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_MINIGAME_TESTING_DONE;
			cyberspace_neural_connector_output(&cyberspace_output_command);
			break;
		default:
			break;
		}
	}

void cyberspace_minigame_testing_print(void) {
	if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();

	glDisable(GL_DEPTH_TEST);
	if(!CYBERSPACE_GUI_WIREFRAMES) {
		glDisable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, double(cyberspace_screen_width)-1.0, double(cyberspace_screen_height)-1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor4f(0.0, 0.8, 0.0, 1.0);
	int y_tmp=20;
	char* msg=new char[1000];

	cyberspace_GUI_font_print(10, y_tmp, 100, "minigame testing");
	y_tmp+=20;
	if(cyberspace_server_message_fade>0 && strlen(cyberspace_server_message)>0) {
		glColor4f(1.0, 0.0, 0.0, 1.0);
		cyberspace_GUI_font_print((cyberspace_screen_width-(strlen(cyberspace_server_message)*10))/2, 20, 100, cyberspace_server_message);
		glColor4f(0.0, 0.8, 0.0, 1.0);
		}

	cyberspace_GUI_font_print(10, y_tmp, 100, "bla bla");
	y_tmp+=20;

	sprintf(msg, "punti: %d", mini_testing.points);
	cyberspace_GUI_font_print(10, y_tmp, 100, msg);
	y_tmp+=20;

	glColor4f(0.0, 0.8, 0.0, 1.0);
	y_tmp+=5;
	cyberspace_GUI_font_print(10, y_tmp, 100, "premi [A] per terminare");
	y_tmp+=20;

	if(!CYBERSPACE_GUI_WIREFRAMES) {
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		}
	glEnable(GL_DEPTH_TEST);

	glFlush();
	SDL_GL_SwapBuffers();

	delete[] msg;
	}

