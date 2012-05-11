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


#include <pthread.h>
#include <math.h>
#include "SDL/SDL.h"
#include "SDL_image.h"
#include "mini_heuristic_standalone.h"
#include "SDL/SDL_ttf.h"



int cyb_mini_heuristic_piece_position;
int cyb_mini_heuristic_piece_status;
int cyb_mini_heuristic_piece[2];
int cyb_mini_heuristic_board[8][8];
bool cyb_mini_heuristic_board_morphing[8][8];
bool cyb_mini_heuristic_morphing;
int cyb_mini_heuristic_morph_phase;
int cyb_mini_heuristic_morph_time;
bool cyb_mini_heuristic_end;
int cyb_mini_heuristic_maxpiece;
SDL_Surface* cyb_mini_heuristic_icons;
SDL_Rect cyb_mini_heuristic_icon_clip[12];
SDL_Rect cyb_mini_heuristic_grid_clip;
int cyb_mini_heuristic_iconsize;
SDL_Surface* cyb_mini_heuristic_grid;

int cyb_hardware;
SDL_Surface* cyb_screen;
//SDL_Surface* cyb_screen_gl;
int cyb_phase;
pthread_mutex_t cyb_mutex_graphic_phase;
int cyb_graphic_phase;
pthread_attr_t cyb_detached;
pthread_t cyb_thread_graphics;
SDL_Event cyb_event;
SDL_Rect cyb_blit_rect;

TTF_Font* font_TTFfont;
TTF_Font* font_TTFfont_mono;
TTF_Font* font_TTFfont_monoBold;
SDL_Color font_color, font_color_black, font_color_white, font_color_gold;

int punti=0;
char punteggiostr[100];
SDL_Rect punteggiorec;

int puntis[11];
bool win;

SDL_Surface* glfont_img;
SDL_Rect glfont_clip[100];

int main(int nOfArgs, char** args) {
	puntis[0]=1;
	puntis[1]=1;
	puntis[2]=1;
	puntis[3]=3;
	puntis[4]=9;
	puntis[5]=27;
	puntis[6]=81;
	puntis[7]=243;
	puntis[8]=729;
	puntis[9]=2187;
	puntis[10]=6561;
	puntis[11]=19683;
	win=false;
	if(SDL_Init(SDL_INIT_VIDEO)!=0) {
		fprintf(stderr, "ABORT: SDL_Init fails: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}
	if(atexit(SDL_Quit)!=0) {
		fprintf(stderr, "ABORT: couldn't set exit function\n");
		exit(EXIT_FAILURE);
		}
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
	SDL_EventState(SDL_KEYUP, SDL_ENABLE);
	SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
	SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBALLMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYHATMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);
	SDL_EventState(SDL_QUIT, SDL_ENABLE);
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);
	SDL_EventState(SDL_VIDEORESIZE, SDL_IGNORE);
	SDL_EventState(SDL_VIDEOEXPOSE, SDL_IGNORE);
	SDL_EventState(SDL_USEREVENT, SDL_IGNORE);

	cyb_hardware=CYB_HARDWARE_EEE;
//	cyb_hardware=CYB_HARDWARE_PC;
	int cyb_scr_w;
	int cyb_scr_h;

	if(cyb_hardware==CYB_HARDWARE_EEE) {
		cyb_scr_w=800;
		cyb_scr_h=480;
		}
	else {
//		cyb_scr_w=750;
//		cyb_scr_h=500;
		cyb_scr_w=800;
		cyb_scr_h=600;
		}

//	cyb_screen=SDL_SetVideoMode(CYB_GRAPHIC_SCREEN_WIDTH, CYB_GRAPHIC_SCREEN_HEIGHT, 32, SDL_SWSURFACE);
//	cyb_screen=SDL_SetVideoMode(cyb_scr_w, cyb_scr_h, 32, SDL_SWSURFACE);
	cyb_screen=SDL_SetVideoMode(0, 0, 32, SDL_FULLSCREEN);
	if(cyb_screen==NULL) {
		fprintf(stderr, "ABORT: SDL_SetVideoMode fails: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}
	SDL_WM_SetCaption("cyberspace", "cyberspace");
	SDL_FillRect(cyb_screen, NULL, 0xffff0000);
	SDL_UpdateRect(cyb_screen, 0, 0, 0, 0);

	pthread_mutexattr_t mutex_attr;
	pthread_mutexattr_init(&mutex_attr);
	pthread_mutex_init(&cyb_mutex_graphic_phase, &mutex_attr);
	cyb_graphic_phase=CYB_GRAPHIC_PHASE_BLACK;
	cyb_phase=CYB_PHASE_MINI_HEURISTIC;

	pthread_attr_init(&cyb_detached);
	pthread_attr_setdetachstate(&cyb_detached, PTHREAD_CREATE_DETACHED);

	pthread_create(&cyb_thread_graphics, &cyb_detached, cyb_graphics, NULL);


	while(cyb_phase!=CYB_PHASE_QUIT) {
		switch(cyb_phase) {
			case CYB_PHASE_MINI_HEURISTIC:
				cyb_mini_heuristic();
				break;
			default:
				fprintf(stderr, "ABORT: wrong cyb_phase: %d\n", cyb_phase);
				exit(EXIT_FAILURE);
			}
		}
	pthread_exit(NULL);
	}

void cyb_mini_heuristic(void) {
	struct timespec tosleep;
	struct timespec tosleepagain;
	cyb_mini_heuristic_piece_position=5;
	cyb_mini_heuristic_piece_status=0;
	for(int x=0;x<8;x++) {
		for(int y=0;y<8;y++) {
			cyb_mini_heuristic_board[x][y]=-1;
			cyb_mini_heuristic_board_morphing[x][y]=false;
			}
		}
/*	cyb_mini_heuristic_board[0][0]=9;
	cyb_mini_heuristic_board[1][0]=5;
	cyb_mini_heuristic_board[1][1]=2;
	cyb_mini_heuristic_board[1][2]=1;
	cyb_mini_heuristic_board[2][0]=0;
	cyb_mini_heuristic_board[2][1]=5;
	cyb_mini_heuristic_board[2][2]=0;
	cyb_mini_heuristic_board[3][0]=7;
	cyb_mini_heuristic_board[3][1]=4;
	cyb_mini_heuristic_board[4][0]=1;
	cyb_mini_heuristic_board[4][1]=6;
	cyb_mini_heuristic_board[4][2]=8;
	cyb_mini_heuristic_board[4][3]=7;
	cyb_mini_heuristic_board[4][4]=7;
	cyb_mini_heuristic_board[5][0]=8;
	cyb_mini_heuristic_board[5][1]=6;
	cyb_mini_heuristic_board[6][0]=3;
	cyb_mini_heuristic_board[6][1]=1;
	cyb_mini_heuristic_board[6][2]=1;
	cyb_mini_heuristic_board[6][3]=2;
	cyb_mini_heuristic_board[6][4]=4;
	cyb_mini_heuristic_board[7][0]=6;*/
	if(cyb_hardware==CYB_HARDWARE_EEE) {
		cyb_mini_heuristic_iconsize=30;
		cyb_mini_heuristic_icons=IMG_Load("img/icons_small.png");
		cyb_mini_heuristic_grid=IMG_Load("img/grid.png");
		}
	else {
		cyb_mini_heuristic_iconsize=50;
		cyb_mini_heuristic_icons=IMG_Load("img/icons.png");
		cyb_mini_heuristic_grid=IMG_Load("img/grid.png");
		}
	if(cyb_mini_heuristic_icons==NULL) {
		fprintf(stderr, "FATAL ERROR: failed to load icons file.\n");
		exit(EXIT_FAILURE);
		}
	for(int i=0;i<12;i++) {
		cyb_mini_heuristic_icon_clip[i].x=(i%4)*(cyb_mini_heuristic_iconsize+1);
		cyb_mini_heuristic_icon_clip[i].y=(i/4)*(cyb_mini_heuristic_iconsize+1);
		cyb_mini_heuristic_icon_clip[i].w=cyb_mini_heuristic_iconsize;
		cyb_mini_heuristic_icon_clip[i].h=cyb_mini_heuristic_iconsize;
		}
	cyb_mini_heuristic_maxpiece=3;
//	cyb_mini_heuristic_maxpiece=9;
	srandom(time(NULL));
	double newtype=(cyb_mini_heuristic_maxpiece*(((double)random())/((double)RAND_MAX)))-0.1;
	if(newtype<0) newtype*=-1;
	cyb_mini_heuristic_piece[0]=int(floor(newtype));
	newtype=(cyb_mini_heuristic_maxpiece*(((double)random())/((double)RAND_MAX)))-0.1;
	if(newtype<0) newtype*=-1;
	cyb_mini_heuristic_piece[1]=int(floor(newtype));
	int drop_x;
	int drop_y;
	cyb_mini_heuristic_end=false;
	cyb_mini_heuristic_morphing=false;
	cyb_mini_heuristic_morph_phase=0;
	cyb_mini_heuristic_morph_time=20;
	pthread_mutex_lock(&cyb_mutex_graphic_phase);
	cyb_graphic_phase=CYB_GRAPHIC_PHASE_MINI_HEURISTIC;
	pthread_mutex_unlock(&cyb_mutex_graphic_phase);
	while(SDL_WaitEvent(&cyb_event)==1 && cyb_phase==CYB_PHASE_MINI_HEURISTIC) {
		if(cyb_mini_heuristic_end || cyb_mini_heuristic_morphing) {
			if(cyb_mini_heuristic_end) {
				switch(cyb_event.type) {
					case SDL_QUIT:
						cyb_phase=CYB_PHASE_QUIT;
						break;
					case SDL_KEYDOWN:
						cyb_phase=CYB_PHASE_QUIT;
						break;
					default:
						break;
					}
				}
			}
		else {
			switch(cyb_event.type) {
				case SDL_QUIT:
					cyb_phase=CYB_PHASE_QUIT;
					break;
				case SDL_KEYDOWN:
					switch(cyb_event.key.keysym.sym) {
						case SDLK_ESCAPE:
							cyb_phase=CYB_PHASE_QUIT;
							break;
						case SDLK_UP:
							if(cyb_mini_heuristic_piece_position==7 && (cyb_mini_heuristic_piece_status==0 || cyb_mini_heuristic_piece_status==2)) cyb_mini_heuristic_piece_position--;
							cyb_mini_heuristic_piece_status=(cyb_mini_heuristic_piece_status+1)%4;
							break;
						case SDLK_DOWN:
							drop_x=cyb_mini_heuristic_piece_position;
							drop_y=0;
							switch(cyb_mini_heuristic_piece_status) {
								case 0:
									while(cyb_mini_heuristic_board[drop_x][drop_y]!=-1) drop_y++;
									cyb_mini_heuristic_board[drop_x][drop_y]=cyb_mini_heuristic_piece[0];
									drop_y++;
									cyb_mini_heuristic_board[drop_x][drop_y]=cyb_mini_heuristic_piece[1];
									break;
								case 1:
									while(cyb_mini_heuristic_board[drop_x][drop_y]!=-1) drop_y++;
									cyb_mini_heuristic_board[drop_x][drop_y]=cyb_mini_heuristic_piece[0];
									drop_x++;
									drop_y=0;
									while(cyb_mini_heuristic_board[drop_x][drop_y]!=-1) drop_y++;
									cyb_mini_heuristic_board[drop_x][drop_y]=cyb_mini_heuristic_piece[1];
									break;
								case 2:
									while(cyb_mini_heuristic_board[drop_x][drop_y]!=-1) drop_y++;
									cyb_mini_heuristic_board[drop_x][drop_y]=cyb_mini_heuristic_piece[1];
									drop_y++;
									cyb_mini_heuristic_board[drop_x][drop_y]=cyb_mini_heuristic_piece[0];
									break;
								case 3:
									while(cyb_mini_heuristic_board[drop_x][drop_y]!=-1) drop_y++;
									cyb_mini_heuristic_board[drop_x][drop_y]=cyb_mini_heuristic_piece[1];
									drop_x++;
									drop_y=0;
									while(cyb_mini_heuristic_board[drop_x][drop_y]!=-1) drop_y++;
									cyb_mini_heuristic_board[drop_x][drop_y]=cyb_mini_heuristic_piece[0];
									break;
								}
							newtype=(cyb_mini_heuristic_maxpiece*(((double)random())/((double)RAND_MAX)))-0.1;
							if(newtype<0) newtype*=-1;
							cyb_mini_heuristic_piece[0]=int(floor(newtype));
							newtype=(cyb_mini_heuristic_maxpiece*(((double)random())/((double)RAND_MAX)))-0.1;
							if(newtype<0) newtype*=-1;
							cyb_mini_heuristic_piece[1]=int(floor(newtype));
							for(int y=0;y<8;y++) {
								for(int x=0;x<8;x++) {
									if(cyb_mini_heuristic_ismorphing(x, y)) cyb_mini_heuristic_morphing=true;
									}
								}
							if(!cyb_mini_heuristic_morphing) {
								for(int x=0;x<8;x++) {
									if(cyb_mini_heuristic_board[x][6]!=-1) {
										cyb_mini_heuristic_end=true;
										pthread_mutex_lock(&cyb_mutex_graphic_phase);
										cyb_graphic_phase=CYB_GRAPHIC_PHASE_MINI_HEURISTIC_END;
										pthread_mutex_unlock(&cyb_mutex_graphic_phase);
										}
									}
								}
							break;
						case SDLK_LEFT:
							if(cyb_mini_heuristic_piece_position>0) cyb_mini_heuristic_piece_position--;
							break;
						case SDLK_RIGHT:
							if(cyb_mini_heuristic_piece_position<6) cyb_mini_heuristic_piece_position++;
							else {
								if(cyb_mini_heuristic_piece_position==6 && (cyb_mini_heuristic_piece_status==0 || cyb_mini_heuristic_piece_status==2)) cyb_mini_heuristic_piece_position++;
								}
							break;
						default:
							break;
						}
					break;
				case SDL_KEYUP:
					break;
				case SDL_ACTIVEEVENT:
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
				case SDL_JOYAXISMOTION:
				case SDL_JOYBALLMOTION:
				case SDL_JOYHATMOTION:
				case SDL_JOYBUTTONDOWN:
				case SDL_JOYBUTTONUP:
				case SDL_SYSWMEVENT:
				case SDL_VIDEORESIZE:
				case SDL_VIDEOEXPOSE:
				case SDL_USEREVENT:
					fprintf(stderr, "ABORT: unexpected event %d\n", cyb_event.type);
					exit(EXIT_FAILURE);
					break;
				default:
					fprintf(stderr, "ABORT: weird event %d\n", cyb_event.type);
					exit(EXIT_FAILURE);
					break;
				}
			}
		tosleep.tv_sec=0;
		tosleep.tv_nsec=10*1000000L;
		while(nanosleep(&tosleep, &tosleepagain)!=0) {
			tosleep=tosleepagain;
			}
		}
	pthread_mutex_lock(&cyb_mutex_graphic_phase);
	cyb_graphic_phase=CYB_GRAPHIC_PHASE_BLACK;
	pthread_mutex_unlock(&cyb_mutex_graphic_phase);
	SDL_FreeSurface(cyb_mini_heuristic_icons);
	}


bool cyb_mini_heuristic_ismorphing(int x, int y) {
	int type=cyb_mini_heuristic_board[x][y];
	if(type==-1 || cyb_mini_heuristic_board_morphing[x][y]) return(false);
	int count=1;
	bool res=false;
	if(x<7 && cyb_mini_heuristic_board[x+1][y]==type) {
		count++;
		if(y<7 && cyb_mini_heuristic_board[x+1][y+1]==type) res=true;
		if(x<6 && cyb_mini_heuristic_board[x+2][y]==type) res=true;
		}
	if(y<7 && cyb_mini_heuristic_board[x][y+1]==type) {
		count++;
		if(count==3) res=true;
		if(x>0 && cyb_mini_heuristic_board[x-1][y+1]==type) res=true;
		if(x<7 && cyb_mini_heuristic_board[x+1][y+1]==type) res=true;
		if(y<6 && cyb_mini_heuristic_board[x][y+2]==type) res=true;
		}
	if(res) {
		cyb_mini_heuristic_morph(x, y);
		cyb_mini_heuristic_board[x][y]=type+1;
		if(cyb_mini_heuristic_maxpiece<type+1) cyb_mini_heuristic_maxpiece=type+1;
		}
	return(res);
	}


void cyb_mini_heuristic_morph(int x, int y) {
	int type=cyb_mini_heuristic_board[x][y];
	cyb_mini_heuristic_board[x][y]=-1;
	cyb_mini_heuristic_board_morphing[x][y]=true;
	if(x>0 && cyb_mini_heuristic_board[x-1][y]==type) cyb_mini_heuristic_morph(x-1, y);
	if(x<7 && cyb_mini_heuristic_board[x+1][y]==type) cyb_mini_heuristic_morph(x+1, y);
	if(y<7 && cyb_mini_heuristic_board[x][y+1]==type) cyb_mini_heuristic_morph(x, y+1);
	}


void cyb_mini_heuristic_graphics(void) {
	if(cyb_mini_heuristic_morphing) {
		switch(cyb_mini_heuristic_morph_phase) {
			case 0:
				cyb_mini_heuristic_morph_time--;
				if(cyb_mini_heuristic_morph_time==0) {
					for(int x=0;x<8;x++) {
						for(int y=0;y<8;y++) {
							cyb_mini_heuristic_board_morphing[x][y]=false;
							}
						}
					cyb_mini_heuristic_morph_time=20;
					cyb_mini_heuristic_morph_phase++;
					}
				break;
			case 1:
				cyb_mini_heuristic_morph_time--;
				if(cyb_mini_heuristic_morph_time==0) {
					for(int x=0;x<8;x++) {
						for(int y=0;y<7;y++) {
							if(cyb_mini_heuristic_board[x][y]==-1) {
								for(int y2=y+1;y2<8;y2++) {
									if(cyb_mini_heuristic_board[x][y2]!=-1) {
										cyb_mini_heuristic_board[x][y]=cyb_mini_heuristic_board[x][y2];
										cyb_mini_heuristic_board[x][y2]=-1;
										break;
										}
									}
								}
							}
						}
					cyb_mini_heuristic_morph_time=20;
					cyb_mini_heuristic_morph_phase=0;
					cyb_mini_heuristic_morphing=false;
					for(int y=0;y<8;y++) {
						for(int x=0;x<8;x++) {
							if(cyb_mini_heuristic_ismorphing(x, y)) cyb_mini_heuristic_morphing=true;
							}
						}
					if(!cyb_mini_heuristic_morphing) {
						for(int x=0;x<8;x++) {
							if(cyb_mini_heuristic_board[x][6]!=-1) {
								cyb_mini_heuristic_end=true;
								cyb_graphic_phase=CYB_GRAPHIC_PHASE_MINI_HEURISTIC_END;
								}
							}
						}
					}
				break;
			}
		}
	if(cyb_graphic_phase==CYB_GRAPHIC_PHASE_MINI_HEURISTIC_END) SDL_FillRect(cyb_screen, NULL, 0xff550000);
	else SDL_FillRect(cyb_screen, NULL, 0xff555555);
	cyb_mini_heuristic_grid_clip.x=0;
	cyb_mini_heuristic_grid_clip.y=0;
	cyb_mini_heuristic_grid_clip.w=35;
	cyb_mini_heuristic_grid_clip.h=35;

	for(int x=0;x<8;x++) {
		for(int y=0;y<6;y++) {
//			SDL_BlitSurface(cyb_mini_heuristic_grid, &cyb_mini_heuristic_icon_clip[11], cyb_screen, &cyb_blit_rect);
			cyb_blit_rect.x=100+x*(cyb_mini_heuristic_iconsize+5)-2;
			cyb_blit_rect.y=400-y*(cyb_mini_heuristic_iconsize+5)-3;
//			cyb_blit_rect.y=600-y*(cyb_mini_heuristic_iconsize+5)-3;
			cyb_blit_rect.w=35;
			cyb_blit_rect.h=35;
			SDL_BlitSurface(cyb_mini_heuristic_grid, &cyb_mini_heuristic_grid_clip, cyb_screen, &cyb_blit_rect);
			}
		}

	punti=0;
	for(int x=0;x<8;x++) {
		for(int y=0;y<8;y++) {
			cyb_blit_rect.x=100+x*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.y=400-y*(cyb_mini_heuristic_iconsize+5);
//			cyb_blit_rect.y=600-y*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.w=cyb_mini_heuristic_iconsize;
			cyb_blit_rect.h=cyb_mini_heuristic_iconsize;
			if(cyb_mini_heuristic_board_morphing[x][y]) {
				SDL_BlitSurface(cyb_mini_heuristic_icons, &cyb_mini_heuristic_icon_clip[11], cyb_screen, &cyb_blit_rect);
				}
			else {
				if(cyb_mini_heuristic_board[x][y]!=-1) {
					SDL_BlitSurface(cyb_mini_heuristic_icons, &cyb_mini_heuristic_icon_clip[cyb_mini_heuristic_board[x][y]], cyb_screen, &cyb_blit_rect);
					}
				}
//			if(cyb_mini_heuristic_board[x][y]==10) win=true;
			if(cyb_mini_heuristic_maxpiece==9) win=true;
			punti+=puntis[cyb_mini_heuristic_board[x][y]];
			}
		}
	switch(cyb_mini_heuristic_piece_status) {
		case 0:
			cyb_blit_rect.x=100+cyb_mini_heuristic_piece_position*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.y=400-8*(cyb_mini_heuristic_iconsize+5);
//			cyb_blit_rect.y=600-8*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.w=cyb_mini_heuristic_iconsize;
			cyb_blit_rect.h=cyb_mini_heuristic_iconsize;
			SDL_BlitSurface(cyb_mini_heuristic_icons, &cyb_mini_heuristic_icon_clip[cyb_mini_heuristic_piece[0]], cyb_screen, &cyb_blit_rect);
			cyb_blit_rect.x=100+cyb_mini_heuristic_piece_position*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.y=400-9*(cyb_mini_heuristic_iconsize+5);
//			cyb_blit_rect.y=600-9*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.w=cyb_mini_heuristic_iconsize;
			cyb_blit_rect.h=cyb_mini_heuristic_iconsize;
			SDL_BlitSurface(cyb_mini_heuristic_icons, &cyb_mini_heuristic_icon_clip[cyb_mini_heuristic_piece[1]], cyb_screen, &cyb_blit_rect);
			break;
		case 1:
			cyb_blit_rect.x=100+cyb_mini_heuristic_piece_position*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.y=400-8*(cyb_mini_heuristic_iconsize+5);
//			cyb_blit_rect.y=600-8*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.w=cyb_mini_heuristic_iconsize;
			cyb_blit_rect.h=cyb_mini_heuristic_iconsize;
			SDL_BlitSurface(cyb_mini_heuristic_icons, &cyb_mini_heuristic_icon_clip[cyb_mini_heuristic_piece[0]], cyb_screen, &cyb_blit_rect);
			cyb_blit_rect.x=100+(cyb_mini_heuristic_piece_position+1)*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.y=400-8*(cyb_mini_heuristic_iconsize+5);
//			cyb_blit_rect.y=600-8*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.w=cyb_mini_heuristic_iconsize;
			cyb_blit_rect.h=cyb_mini_heuristic_iconsize;
			SDL_BlitSurface(cyb_mini_heuristic_icons, &cyb_mini_heuristic_icon_clip[cyb_mini_heuristic_piece[1]], cyb_screen, &cyb_blit_rect);
			break;
		case 2:
			cyb_blit_rect.x=100+cyb_mini_heuristic_piece_position*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.y=400-8*(cyb_mini_heuristic_iconsize+5);
//			cyb_blit_rect.y=600-8*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.w=cyb_mini_heuristic_iconsize;
			cyb_blit_rect.h=cyb_mini_heuristic_iconsize;
			SDL_BlitSurface(cyb_mini_heuristic_icons, &cyb_mini_heuristic_icon_clip[cyb_mini_heuristic_piece[1]], cyb_screen, &cyb_blit_rect);
			cyb_blit_rect.x=100+cyb_mini_heuristic_piece_position*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.y=400-9*(cyb_mini_heuristic_iconsize+5);
//			cyb_blit_rect.y=600-9*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.w=cyb_mini_heuristic_iconsize;
			cyb_blit_rect.h=cyb_mini_heuristic_iconsize;
			SDL_BlitSurface(cyb_mini_heuristic_icons, &cyb_mini_heuristic_icon_clip[cyb_mini_heuristic_piece[0]], cyb_screen, &cyb_blit_rect);
			break;
		case 3:
			cyb_blit_rect.x=100+cyb_mini_heuristic_piece_position*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.y=400-8*(cyb_mini_heuristic_iconsize+5);
//			cyb_blit_rect.y=600-8*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.w=cyb_mini_heuristic_iconsize;
			cyb_blit_rect.h=cyb_mini_heuristic_iconsize;
			SDL_BlitSurface(cyb_mini_heuristic_icons, &cyb_mini_heuristic_icon_clip[cyb_mini_heuristic_piece[1]], cyb_screen, &cyb_blit_rect);
			cyb_blit_rect.x=100+(cyb_mini_heuristic_piece_position+1)*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.y=400-8*(cyb_mini_heuristic_iconsize+5);
//			cyb_blit_rect.y=600-8*(cyb_mini_heuristic_iconsize+5);
			cyb_blit_rect.w=cyb_mini_heuristic_iconsize;
			cyb_blit_rect.h=cyb_mini_heuristic_iconsize;
			SDL_BlitSurface(cyb_mini_heuristic_icons, &cyb_mini_heuristic_icon_clip[cyb_mini_heuristic_piece[0]], cyb_screen, &cyb_blit_rect);
			break;
		}
	for(int i=0;i<=cyb_mini_heuristic_maxpiece;i++) {
		cyb_blit_rect.x=600;
		cyb_blit_rect.y=400-i*(cyb_mini_heuristic_iconsize-10);
//		cyb_blit_rect.y=600-i*(cyb_mini_heuristic_iconsize-10);
		cyb_blit_rect.w=cyb_mini_heuristic_iconsize;
		cyb_blit_rect.h=cyb_mini_heuristic_iconsize;
		SDL_BlitSurface(cyb_mini_heuristic_icons, &cyb_mini_heuristic_icon_clip[i], cyb_screen, &cyb_blit_rect);
		}
	sprintf(punteggiostr, "%10d", punti);
	punteggiorec.x=50;
	punteggiorec.y=50;
	font_print(punteggiostr, cyb_screen, &punteggiorec, FONT_BLENDED);
	if(win) {
		sprintf(punteggiostr, "***** craked --- \"TYCSV-54LK5-27674-PUPPA-MILAF\" *****");
		punteggiorec.x=50;
		punteggiorec.y=30;
		font_print(punteggiostr, cyb_screen, &punteggiorec, FONT_BLENDED);
		}
	else {
		if(punti<100) {
			sprintf(punteggiostr, "***** senti che il sistema vuole ucciderti *****");
			punteggiorec.x=50;
			punteggiorec.y=30;
			font_print(punteggiostr, cyb_screen, &punteggiorec, FONT_BLENDED);
			}
//		else if(punti<500) {
		else if(punti<250) {
			sprintf(punteggiostr, "***** senti che il sistema vuole farti del male *****");
			punteggiorec.x=50;
			punteggiorec.y=30;
			font_print(punteggiostr, cyb_screen, &punteggiorec, FONT_BLENDED);
			}
//		else if(punti<1000) {
		else if(punti<500) {
			sprintf(punteggiostr, "***** senti che il sistema ti sopporta *****");
			punteggiorec.x=50;
			punteggiorec.y=30;
			font_print(punteggiostr, cyb_screen, &punteggiorec, FONT_BLENDED);
			}
//		else if(punti<2000) {
		else if(punti<1000) {
			sprintf(punteggiostr, "***** senti che il sistema ti apprezza *****");
			punteggiorec.x=50;
			punteggiorec.y=30;
			font_print(punteggiostr, cyb_screen, &punteggiorec, FONT_BLENDED);
			}
		}
	SDL_UpdateRect(cyb_screen, 0, 0, 0, 0);
	}

void* cyb_graphics(void* voidinfo) {
	struct timespec tosleep;
	struct timespec tosleepagain;
	font_init();
	font_setColor(FONT_BLACK);
	font_setFont(FONT_MONOBOLD);
	while(cyb_phase!=CYB_PHASE_QUIT) {
		pthread_mutex_lock(&cyb_mutex_graphic_phase);
		switch(cyb_graphic_phase) {
			case CYB_GRAPHIC_PHASE_BLACK:
				SDL_FillRect(cyb_screen, NULL, 0xff000000);
				SDL_UpdateRect(cyb_screen, 0, 0, 0, 0);
				break;
			case CYB_GRAPHIC_PHASE_MINI_HEURISTIC:
			case CYB_GRAPHIC_PHASE_MINI_HEURISTIC_END:
				cyb_mini_heuristic_graphics();
				break;
			}
		pthread_mutex_unlock(&cyb_mutex_graphic_phase);
		tosleep.tv_sec=0;
		tosleep.tv_nsec=10*1000000L;
		while(nanosleep(&tosleep, &tosleepagain)!=0) {
			tosleep=tosleepagain;
			}
		}
	pthread_exit(NULL);
	}

void font_init(void) {
	glfont_img=IMG_Load("fonts/glfont2.png");
	if(glfont_img==NULL) {
		fprintf(stderr, "FATAL ERROR: failed to load icons file.\n");
		exit(EXIT_FAILURE);
		}
	for(int i=0;i<100;i++) {
		glfont_clip[i].x=i*10;
		glfont_clip[i].y=0;
		glfont_clip[i].w=8;
		glfont_clip[i].h=11;
		}

/*	if(TTF_Init()==-1) {
		fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
		}
	font_TTFfont_mono=TTF_OpenFont("img/fonts/FreeMono.ttf", FONT_H);
	if(!font_TTFfont_mono) {
		fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
		}
	font_TTFfont_monoBold=TTF_OpenFont("img/fonts/FreeMonoBold.ttf", FONT_H);
	if(!font_TTFfont_monoBold) {
		fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
		}
	font_setFont(FONT_MONO);
	font_color_black.r=0;
	font_color_black.g=0;
	font_color_black.b=0;
	font_color_white.r=255;
	font_color_white.g=255;
	font_color_white.b=255;
	font_color_gold.r=255;
	font_color_gold.g=255;
	font_color_gold.b=0;
	font_setColor(FONT_BLACK);*/
	}

void font_setFont(const int font) {
	switch(font) {
		case FONT_MONO:
			font_TTFfont=font_TTFfont_mono;
			break;
		case FONT_MONOBOLD:
			font_TTFfont=font_TTFfont_monoBold;
			break;
		default:
			fprintf(stderr, "font_setFont: Unexpected font %d\n", font);
			exit(EXIT_FAILURE);
		}
	}

void font_setColor(const int color) {
	switch(color) {
		case FONT_BLACK:
			font_color=font_color_black;
			break;
		case FONT_WHITE:
			font_color=font_color_white;
			break;
		case FONT_GOLD:
			font_color=font_color_gold;
			break;
		default:
			fprintf(stderr, "font_setColor: Unexpected color %d\n", color);
			exit(EXIT_FAILURE);
		}
	}

void font_print(const char* string, SDL_Surface* screen, const SDL_Rect* to, const int quality) {
	if(strlen(string)==0) return;
/*	SDL_Surface* txt;
	switch(quality) {
		case FONT_SOLID:
			txt=TTF_RenderText_Solid(font_TTFfont, string, font_color);
			break;
		case FONT_SHADED:
			txt=TTF_RenderText_Shaded(font_TTFfont, string, font_color, font_color_white);
			break;
		case FONT_BLENDED:
			txt=TTF_RenderText_Blended(font_TTFfont, string, font_color);
			break;
		default:
			fprintf(stderr, "font_print: Unexpected quality %d\n", quality);
			exit(EXIT_FAILURE);
		}
	if(txt==NULL) {
		fprintf(stderr, "font_print: rendertext fails: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
		}*/
	SDL_Rect txtTo;
	txtTo.x=to->x-10;
	txtTo.y=to->y;
	for(int i=0;i<strlen(string);i++) {
		txtTo.x+=10;
		if(string[i]>='0' && string[i]<='9') {
			if(SDL_BlitSurface(glfont_img, &glfont_clip[16+string[i]-'0'], screen, &txtTo)<0) {
				fprintf(stderr, "SDL_BlitSurface fails: %s\n", SDL_GetError());
				exit(EXIT_FAILURE);
				}
			}
		if(string[i]>='A' && string[i]<='Z') {
			if(SDL_BlitSurface(glfont_img, &glfont_clip[33+string[i]-'A'], screen, &txtTo)<0) {
				fprintf(stderr, "SDL_BlitSurface fails: %s\n", SDL_GetError());
				exit(EXIT_FAILURE);
				}
			}
		if(string[i]>='a' && string[i]<='z') {
			if(SDL_BlitSurface(glfont_img, &glfont_clip[65+string[i]-'a'], screen, &txtTo)<0) {
				fprintf(stderr, "SDL_BlitSurface fails: %s\n", SDL_GetError());
				exit(EXIT_FAILURE);
				}
			}
		if(string[i]==' ') {
			if(SDL_BlitSurface(glfont_img, &glfont_clip[0], screen, &txtTo)<0) {
				fprintf(stderr, "SDL_BlitSurface fails: %s\n", SDL_GetError());
				exit(EXIT_FAILURE);
				}
			}
		if(string[i]=='"') {
			if(SDL_BlitSurface(glfont_img, &glfont_clip[2], screen, &txtTo)<0) {
				fprintf(stderr, "SDL_BlitSurface fails: %s\n", SDL_GetError());
				exit(EXIT_FAILURE);
				}
			}
		if(string[i]=='*') {
			if(SDL_BlitSurface(glfont_img, &glfont_clip[10], screen, &txtTo)<0) {
				fprintf(stderr, "SDL_BlitSurface fails: %s\n", SDL_GetError());
				exit(EXIT_FAILURE);
				}
			}
		if(string[i]=='-') {
			if(SDL_BlitSurface(glfont_img, &glfont_clip[13], screen, &txtTo)<0) {
				fprintf(stderr, "SDL_BlitSurface fails: %s\n", SDL_GetError());
				exit(EXIT_FAILURE);
				}
			}
		}
/*	if(SDL_BlitSurface(txt, NULL, screen, &txtTo)<0) {
		fprintf(stderr, "SDL_BlitSurface fails: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}*/
//	SDL_FreeSurface(txt);
	}

void font_free(void) {
//	TTF_CloseFont(font_TTFfont_mono);
//	TTF_CloseFont(font_TTFfont_monoBold);
//	TTF_Quit();
	}

