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


#include "cyberspace_GUI_font.h"

GLubyte** cyberspace_GUI_font;

void cyberspace_GUI_font_init(void) {
	FILE* rawfile=fopen("fonts/glfont.raw", "rb");
	if(rawfile==NULL) {
		fprintf(stderr, "ABORT: fopen raw fails\n");
		exit(EXIT_FAILURE);
		}
	onebyte* pixels=new onebyte[950*13*3];
	fread(pixels, 950*13*3, 1, rawfile);
	fclose(rawfile);

/*
	SDL_Surface* glfont_img=IMG_Load("fonts/glfont.png");
	if(glfont_img==NULL) {
		fprintf(stderr, "FATAL ERROR: failed to load icons file.\n");
		exit(EXIT_FAILURE);
		}
	int bpp=glfont_img->format->BytesPerPixel;
//	printf("bpp: %d\n", bpp);
	onebyte* pixels=(onebyte*)(glfont_img->pixels);
//	int w=glfont_img->w;
//	printf("w: %d\n", w);
	int pitch=glfont_img->pitch;
//	printf("pitch: %d\n", pitch);
*/

//	int h=glfont_img->h;
//	printf("h: %d\n", h);
/*	for(int y=0;y<10;y++) {
		for(int x=0;x<20;x++) {
			if(pixels[y*pitch+x*bpp]==255) printf("1");
			else printf("0");
			if(pixels[y*pitch+x*bpp+1]==255) printf("1");
			else printf("0");
			if(pixels[y*pitch+x*bpp+2]==255) printf("1");
			else printf("0");
			printf("|");
			}
		printf("\n");
		}*/

	cyberspace_GUI_font=new GLubyte*[95];
	for(int c=0;c<95;c++) {
		cyberspace_GUI_font[c]=new GLubyte[13*2];
		for(int i=0;i<13*2;i++) cyberspace_GUI_font[c][i]=0u;
		for(int y=0;y<13;y++) {
			for(int x=0;x<10;x++) {
				if(pixels[y*950*3+(x+c*10)*3]==0) {
//				if(pixels[y*pitch+(x+c*10)*bpp]==0) {
					if((15-x)<8) cyberspace_GUI_font[c][(12-y)*2+1]|=1u<<(15-x);
					else cyberspace_GUI_font[c][(12-y)*2]|=1u<<(15-x)-8;
					}
				}
			}
		}

/*	cyberspace_GUI_font=new GLubyte*[95];
	for(int c=0;c<95;c++) {
		cyberspace_GUI_font[c]=new GLubyte[13*2];
		for(int i=0;i<13*2;i++) cyberspace_GUI_font[c][i]=0u;
		for(int y=0;y<13;y++) {
			for(int x=0;x<10;x++) {
				if(pixels[y*pitch+(x+c*10)*bpp]==0) {
					if((15-x)<8) cyberspace_GUI_font[c][(12-y)*2+1]|=1u<<(15-x);
					else cyberspace_GUI_font[c][(12-y)*2]|=1u<<(15-x)-8;
					}
				}
			}
		}*/

/*	cyberspace_GUI_font=new GLubyte*[95];
	for(int c=0;c<95;c++) {
		cyberspace_GUI_font[c]=new GLubyte[8];
		for(int i=0;i<8;i++) cyberspace_GUI_font[c][i]=0u;
		for(int x=0;x<8;x++) {
			for(int y=0;y<8;y++) {
				if(y%2==0) cyberspace_GUI_font[c][y]|=1u<<x;
//				if(x==0) cyberspace_GUI_font[c][y]|=1u<<x;
//				if(x==y) cyberspace_GUI_font[c][y]|=1u>>x;
				}
			}
		}*/

	delete[] pixels;
	}


void cyberspace_GUI_font_print(int x, int y, int w, const char* s) {

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 16);

	int tmp_y=y;
	glRasterPos2f(x, tmp_y);

	for(uint c=0;c<strlen(s);c++) {
		if(s[c]=='\n') {
			tmp_y+=15;
			glRasterPos2f(x, tmp_y);
			}
		if(s[c]>=32 && s[c]<=126) glBitmap(10, 13, 0.0, 0.0, 10, 0, cyberspace_GUI_font[s[c]-32]);
		}
	}

