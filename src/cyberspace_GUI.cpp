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


#include "cyberspace_GUI.h"



int cyberspace_GUI_input_tf_sdl(void* nothing) {
	cyberspace_GUI_input_tf(nothing);
	return(0);
	}

void* cyberspace_GUI_input_tf(void* nothing) {
	printf("GUI input: STARTED\n");

	SDL_Event event;

	struct timespec tosleep;
	struct timespec tosleepagain;

	Uint8* keystate;

//	pthread_mutex_lock(&cyberspace_mutex_world);
	SDL_mutexP(cyberspace_mutex_world_sdl);
/*	cyberspace_world.camera.point.x=50.0;
	cyberspace_world.camera.point.y=0.0;
	cyberspace_world.camera.point.z=50.0;
	cyberspace_world.camera.angle.alpha=M_PI;
	cyberspace_world.camera.angle.beta=0.0;
	cyberspace_world.camera.angle.gamma=0.0;*/

//	cyberspace_GUI_rotation_alpha=0.0;
//	cyberspace_GUI_traslation_ahead=0.0;

//	cyberspace_world.planes_size=0;
//	cyberspace_world.objects_size=0;
	cyberspace_world.static_size=0;
	cyberspace_world.dynamic_size=0;

	SDL_mutexV(cyberspace_mutex_world_sdl);
//	pthread_mutex_unlock(&cyberspace_mutex_world);

	cyberspace_movement.traslation=0;
	cyberspace_movement.rotation=0;
	cyberspace_movement.strafe=false;

	bool dontskip=false;
	int sys;
//	int subsys;
	int position;

	while(true) {
//		pthread_mutex_lock(&cyberspace_mutex_phase);
		switch(cyberspace_phase) {
			case CYBERSPACE_PHASE_PROGRAMMING:
				while(dontskip || SDL_WaitEvent(&event)==1) {
					if(dontskip) dontskip=false;
					if(cyberspace_phase!=CYBERSPACE_PHASE_PROGRAMMING) {
						dontskip=true;
						break;
						}
					switch(event.type) {
						case SDL_QUIT:
							exit(EXIT_FAILURE);
							break;
						case SDL_KEYDOWN:
							if(event.key.keysym.sym==SDLK_ESCAPE) {
//								exit(EXIT_FAILURE);
								cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_LOGOUT;
								cyberspace_neural_connector_output(&cyberspace_output_command);
								break;
								}
							switch(cyberspace_programming_subphase) {
								case CYBERSPACE_PHASE_PROGRAMMING_SYS:
									switch(event.key.keysym.sym) {
										case SDLK_SPACE:
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_STOP;
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										case SDLK_a:
											cyberspace_programming_position=0;
											cyberspace_programming_subphase=CYBERSPACE_PHASE_PROGRAMMING_SUBSYS;
											break;
										case SDLK_b:
											cyberspace_programming_position=0;
											cyberspace_programming_subphase=CYBERSPACE_PHASE_PROGRAMMING_DEVELOP;
											break;
										case SDLK_0:
										case SDLK_1:
										case SDLK_2:
										case SDLK_3:
										case SDLK_4:
											sys=event.key.keysym.sym-SDLK_0;
											if(cyberspace_personal_data.systems_loaded[sys]==0) cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_SYSTEM_LOAD;
											else cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_SYSTEM_UNLOAD;
											memcpy(cyberspace_output_command.data, &sys, sizeof(int));
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										default:
											break;
										}
									break;
								case CYBERSPACE_PHASE_PROGRAMMING_SUBSYS:
									switch(event.key.keysym.sym) {
										case SDLK_SPACE:
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_STOP;
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										case SDLK_a:
											cyberspace_programming_position=0;
											cyberspace_programming_subphase=CYBERSPACE_PHASE_PROGRAMMING_SYS;
											break;
										case SDLK_b:
											cyberspace_programming_subphase=CYBERSPACE_PHASE_PROGRAMMING_DEVELOP;
											break;
										case SDLK_RIGHT:
											position=0;
											for(int sys=0;sys<CYBERSPACE_NEURAL_SYSTEM_MAX;sys++) {
												for(int subsys=0;subsys<CYBERSPACE_NEURAL_SUBSYSTEM_MAX;subsys++) {
													if(cyberspace_personal_data.subsystems[sys][subsys]>cyberspace_personal_data.subsystems[sys][subsys]) {
														if(position++==cyberspace_programming_position) {
															cyberspace_programming_sys=sys;
															cyberspace_programming_subsys=subsys;
															}
														}
													}
												}
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_LOAD;
											memcpy(cyberspace_output_command.data, &cyberspace_programming_sys, sizeof(int));
											memcpy(cyberspace_output_command.data+sizeof(int), &cyberspace_programming_subsys, sizeof(int));
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										case SDLK_DOWN:
											position=0;
											for(int sys=0;sys<CYBERSPACE_NEURAL_SYSTEM_MAX;sys++) {
												for(int subsys=0;subsys<CYBERSPACE_NEURAL_SUBSYSTEM_MAX;subsys++) {
													if(cyberspace_personal_data.subsystems_[sys][subsys]>0) position++;
													}
												}
											if(position>cyberspace_programming_position+1) cyberspace_programming_position++;
											break;
										case SDLK_UP:
											if(cyberspace_programming_position>0) cyberspace_programming_position--;
											break;
										default:
											break;
										}
									break;
								case CYBERSPACE_PHASE_PROGRAMMING_DEVELOP:
									switch(event.key.keysym.sym) {
										case SDLK_SPACE:
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_STOP;
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										case SDLK_a:
											cyberspace_programming_position=0;
											cyberspace_programming_subphase=CYBERSPACE_PHASE_PROGRAMMING_SYS;
											break;
										case SDLK_b:
											cyberspace_programming_subphase=CYBERSPACE_PHASE_PROGRAMMING_SUBSYS;
											break;
										case SDLK_RIGHT:
											position=0;
											for(int sys=0;sys<CYBERSPACE_NEURAL_SYSTEM_MAX;sys++) {
												for(int subsys=0;subsys<CYBERSPACE_NEURAL_SUBSYSTEM_MAX;subsys++) {
													if(cyberspace_personal_data.subsystems_hint[sys][subsys]>cyberspace_personal_data.subsystems[sys][subsys]) {
														if(position++==cyberspace_programming_position) {
															cyberspace_programming_sys=sys;
															cyberspace_programming_subsys=subsys;
															}
														}
													}
												}
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_CODE;
											memcpy(cyberspace_output_command.data, &cyberspace_programming_sys, sizeof(int));
											memcpy(cyberspace_output_command.data+sizeof(int), &cyberspace_programming_subsys, sizeof(int));
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										case SDLK_DOWN:
											position=0;
											for(int sys=0;sys<CYBERSPACE_NEURAL_SYSTEM_MAX;sys++) {
												for(int subsys=0;subsys<CYBERSPACE_NEURAL_SUBSYSTEM_MAX;subsys++) {
													if(cyberspace_personal_data.subsystems_hint[sys][subsys]>cyberspace_personal_data.subsystems[sys][subsys]) position++;
													}
												}
											if(position>cyberspace_programming_position+1) cyberspace_programming_position++;
											break;
										case SDLK_UP:
											if(cyberspace_programming_position>0) cyberspace_programming_position--;
											break;
										case SDLK_0:
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_TRAINING_HEURISTIC;
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										case SDLK_1:
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_TRAINING_FAST;
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										case SDLK_2:
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_TRAINING_PATTERNMATCHING;
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										default:
											break;
										}
									break;
								case CYBERSPACE_PHASE_PROGRAMMING_STEPS:
									switch(event.key.keysym.sym) {
										case SDLK_SPACE:
											cyberspace_programming_subphase=CYBERSPACE_PHASE_PROGRAMMING_DEVELOP;
											break;
										case SDLK_0:
											if(cyberspace_programming.mini_req_heuristic==0 || cyberspace_programming.mini_res_heuristic>=0) break;
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_HEURISTIC;
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										case SDLK_1:
											if(cyberspace_programming.mini_req_fast==0 || cyberspace_programming.mini_res_fast>=0) break;
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_FAST;
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										case SDLK_2:
											if(cyberspace_programming.mini_req_patternmatching==0 || cyberspace_programming.mini_res_patternmatching>=0) break;
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_PROGRAMMING_PATTERNMATCHING;
											cyberspace_neural_connector_output(&cyberspace_output_command);
											break;
										default:
											break;
										}
									break;
								default:
									fprintf(stderr, "FATAL ERROR: unknown cyberspace_programming_subphase %d\n", cyberspace_programming_subphase);
									exit(EXIT_FAILURE);
									break;
								}
							break;
						}
//					pthread_mutex_unlock(&cyberspace_mutex_phase);
					}
				break;
			case CYBERSPACE_PHASE_CONNECTING:
//				pthread_mutex_unlock(&cyberspace_mutex_phase);
				while(dontskip || SDL_WaitEvent(&event)==1) {
					if(dontskip) dontskip=false;
//					pthread_mutex_lock(&cyberspace_mutex_phase);
					if(cyberspace_phase!=CYBERSPACE_PHASE_CONNECTING) {
//						pthread_mutex_unlock(&cyberspace_mutex_phase);
						dontskip=true;
						break;
						}
					switch(event.type) {
						case SDL_QUIT:
							exit(EXIT_FAILURE);
							break;
						case SDL_KEYDOWN:
							switch(event.key.keysym.sym) {
								case SDLK_ESCAPE:
									exit(EXIT_FAILURE);
									break;
								default:
									break;
								}
							break;
						}
//					pthread_mutex_unlock(&cyberspace_mutex_phase);
					}
				break;
			case CYBERSPACE_PHASE_LOGIN:
//				pthread_mutex_unlock(&cyberspace_mutex_phase);
				while(dontskip || SDL_WaitEvent(&event)==1) {
					if(dontskip) dontskip=false;
//					pthread_mutex_lock(&cyberspace_mutex_phase);
					if(cyberspace_phase!=CYBERSPACE_PHASE_LOGIN) {
//						pthread_mutex_unlock(&cyberspace_mutex_phase);
						dontskip=true;
						break;
						}
					switch(event.type) {
						case SDL_QUIT:
							exit(EXIT_FAILURE);
							break;
						case SDL_KEYDOWN:
							if(event.key.keysym.sym>=32 && event.key.keysym.sym<=126) {
								if(cyberspace_GUI_input_focus==CYBERSPACE_GUI_INPUT_FOCUS_LOGIN) {
									if(strlen(cyberspace_login_name)<20) {
										cyberspace_login_name[strlen(cyberspace_login_name)+1]='\0';
										cyberspace_login_name[strlen(cyberspace_login_name)]=event.key.keysym.sym;
										}
									}
								if(cyberspace_GUI_input_focus==CYBERSPACE_GUI_INPUT_FOCUS_PASSWORD) {
									if(strlen(cyberspace_login_password)<20) {
										cyberspace_login_password[strlen(cyberspace_login_password)+1]='\0';
										cyberspace_login_password[strlen(cyberspace_login_password)]=event.key.keysym.sym;
										}
									}
								break;
								}
							switch(event.key.keysym.sym) {
								case SDLK_ESCAPE:
									exit(EXIT_FAILURE);
									break;
								case SDLK_BACKSPACE:
									if(cyberspace_GUI_input_focus==CYBERSPACE_GUI_INPUT_FOCUS_LOGIN) {
										if(strlen(cyberspace_login_name)>0) {
											cyberspace_login_name[strlen(cyberspace_login_name)-1]='\0';
											}
										}
									if(cyberspace_GUI_input_focus==CYBERSPACE_GUI_INPUT_FOCUS_PASSWORD) {
										if(strlen(cyberspace_login_password)>0) {
											cyberspace_login_password[strlen(cyberspace_login_password)-1]='\0';
											}
										}
									break;
								case SDLK_TAB:
									if(cyberspace_GUI_input_focus==CYBERSPACE_GUI_INPUT_FOCUS_LOGIN) {
										cyberspace_GUI_input_focus=CYBERSPACE_GUI_INPUT_FOCUS_PASSWORD;
										}
									else cyberspace_GUI_input_focus=CYBERSPACE_GUI_INPUT_FOCUS_LOGIN;
									break;
								case SDLK_RETURN:
									if(strlen(cyberspace_login_name)==0) break;
//									if(strlen(cyberspace_login_password)==0) break;
									cyberspace_server_message[0]='\0';
									cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_LOGIN;
									memcpy(cyberspace_output_command.data, cyberspace_login_name, sizeof(char)*(strlen(cyberspace_login_name)+1));
									memcpy(cyberspace_output_command.data+(sizeof(char)*(strlen(cyberspace_login_name)+1)), cyberspace_login_password, sizeof(char)*(strlen(cyberspace_login_password)+1));
									cyberspace_neural_connector_output(&cyberspace_output_command);
									break;
								default:
									break;
								}
							break;
						}
//					pthread_mutex_unlock(&cyberspace_mutex_phase);
					}
				break;
			case CYBERSPACE_PHASE_3D:
//				pthread_mutex_unlock(&cyberspace_mutex_phase);
				while(dontskip || SDL_WaitEvent(&event)==1) {
					if(dontskip) dontskip=false;
//					pthread_mutex_lock(&cyberspace_mutex_phase);
					if(cyberspace_phase!=CYBERSPACE_PHASE_3D) {
//						pthread_mutex_unlock(&cyberspace_mutex_phase);
						dontskip=true;
						break;
						}
					switch(event.type) {
						case SDL_QUIT:
							exit(EXIT_FAILURE);
							break;
						case SDL_KEYDOWN:
							if(event.key.keysym.sym>=32 && event.key.keysym.sym<=126) {
								if(cyberspace_GUI_input_focus==CYBERSPACE_GUI_INPUT_FOCUS_CHAT) {
									if(strlen(cyberspace_chat_input)<CYBERSPACE_GUI_CHAT_LINE_WIDTH) {
										cyberspace_chat_input[strlen(cyberspace_chat_input)+1]='\0';
										cyberspace_chat_input[strlen(cyberspace_chat_input)]=event.key.keysym.sym;
										}
									break;
									}
								}
							switch(event.key.keysym.sym) {
								case SDLK_ESCAPE:
//									exit(EXIT_FAILURE);
									cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_LOGOUT;
									cyberspace_neural_connector_output(&cyberspace_output_command);
									break;
								case SDLK_RETURN:
									if(cyberspace_GUI_input_focus==CYBERSPACE_GUI_INPUT_FOCUS_CHAT) {
										if(strlen(cyberspace_chat_input)>0) {
											cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_CHAT;
											memcpy(cyberspace_output_command.data, cyberspace_chat_input, sizeof(char)*(strlen(cyberspace_chat_input)+1));
											cyberspace_chat_input[0]='\0';
											cyberspace_neural_connector_output(&cyberspace_output_command);
											}
										cyberspace_GUI_input_focus=CYBERSPACE_GUI_INPUT_FOCUS_NONE;
										}
									else cyberspace_GUI_input_focus=CYBERSPACE_GUI_INPUT_FOCUS_CHAT;
									break;
								case SDLK_SPACE:
									if(cyberspace_obj_selected.id!=-1) {
										cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_ACTION;
										memcpy(cyberspace_output_command.data, &cyberspace_obj_selected, sizeof(cyberspace_world_selection));
										cyberspace_neural_connector_output(&cyberspace_output_command);
										}
									break;
								case SDLK_F1:
									cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_SKILL_0;
									memcpy(cyberspace_output_command.data, &cyberspace_obj_selected, sizeof(cyberspace_world_selection));
									cyberspace_neural_connector_output(&cyberspace_output_command);
									break;
//								case SDLK_r:
//									cyberspace_sreen_reload=true;
//									break;
								default:
									break;
								}
						case SDL_KEYUP:
							if(event.key.keysym.sym==SDLK_LEFT || event.key.keysym.sym==SDLK_RIGHT || event.key.keysym.sym==SDLK_UP || event.key.keysym.sym==SDLK_DOWN) {
								keystate=SDL_GetKeyState(NULL);
								switch(event.key.keysym.sym) {
									case SDLK_DOWN:
										cyberspace_movement.traslation=-1;
										break;
									case SDLK_UP:
										cyberspace_movement.traslation=1;
										break;
									case SDLK_LEFT:
										cyberspace_movement.rotation=-1;
										break;
									case SDLK_RIGHT:
										cyberspace_movement.rotation=1;
										break;
									default:
										fprintf(stderr, "FATAL ERROR: unexpected event.key.keysym.sym in the SDL_KEYUP inner switch: %d.\n", event.key.keysym.sym);
										exit(EXIT_FAILURE);
									}
								if(keystate[SDLK_DOWN]) {
									if(!keystate[SDLK_UP]) cyberspace_movement.traslation=-1;
									}
								else {
									if(keystate[SDLK_UP]) cyberspace_movement.traslation=1;
									else cyberspace_movement.traslation=0;
									}
								if(keystate[SDLK_LEFT]) {
									if(!keystate[SDLK_RIGHT]) cyberspace_movement.rotation=-1;
									}
								else {
									if(keystate[SDLK_RIGHT]) cyberspace_movement.rotation=1;
									else cyberspace_movement.rotation=0;
									}
								cyberspace_output_command.type=CYBERSPACE_COMMAND_USER_MOVEMENT;
								memcpy(cyberspace_output_command.data, &cyberspace_movement, sizeof(cyberspace_movement));
								cyberspace_neural_connector_output(&cyberspace_output_command);
//								cyberspace_GUI_rotation_alpha=-0.01;
//								cyb_walking=CYB_WALKING_FOWARD;
								break;
								}
							break;
						}
//					pthread_mutex_unlock(&cyberspace_mutex_phase);
					}
				break;
			case CYBERSPACE_PHASE_MINIGAME_TESTING:
				while(dontskip || SDL_WaitEvent(&event)==1) {
					if(dontskip) dontskip=false;
					if(cyberspace_phase!=CYBERSPACE_PHASE_MINIGAME_TESTING) {
						dontskip=true;
						break;
						}
					switch(event.type) {
						case SDL_QUIT:
							exit(EXIT_FAILURE);
							break;
						case SDL_KEYDOWN:
//							if(event.key.keysym.sym==SDLK_ESCAPE) exit(EXIT_FAILURE);
							cyberspace_minigame_testing_inputkey(event.key.keysym.sym);
							break;
						}
					}
				break;
			case CYBERSPACE_PHASE_MINIGAME_HEURISTIC:
				while(dontskip || SDL_WaitEvent(&event)==1) {
					if(dontskip) dontskip=false;
					if(cyberspace_phase!=CYBERSPACE_PHASE_MINIGAME_HEURISTIC) {
						dontskip=true;
						break;
						}
					switch(event.type) {
						case SDL_QUIT:
							exit(EXIT_FAILURE);
							break;
						case SDL_KEYDOWN:
							if(event.key.keysym.sym==SDLK_ESCAPE) exit(EXIT_FAILURE);
							cyberspace_minigame_heuristic_inputkey(event.key.keysym.sym);
							break;
						}
					}
				break;
			case CYBERSPACE_PHASE_MINIGAME_FAST:
				while(dontskip || SDL_WaitEvent(&event)==1) {
					if(dontskip) dontskip=false;
					if(cyberspace_phase!=CYBERSPACE_PHASE_MINIGAME_FAST) {
						dontskip=true;
						break;
						}
					switch(event.type) {
						case SDL_QUIT:
							exit(EXIT_FAILURE);
							break;
						case SDL_KEYDOWN:
//							if(event.key.keysym.sym==SDLK_ESCAPE) exit(EXIT_FAILURE);
							cyberspace_minigame_fast_inputkey(event.key.keysym.sym);
							break;
						}
					}
				break;
			case CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING:
				while(dontskip || SDL_WaitEvent(&event)==1) {
					if(dontskip) dontskip=false;
					if(cyberspace_phase!=CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING) {
						dontskip=true;
						break;
						}
					switch(event.type) {
						case SDL_QUIT:
							exit(EXIT_FAILURE);
							break;
						case SDL_KEYDOWN:
							if(event.key.keysym.sym==SDLK_ESCAPE) exit(EXIT_FAILURE);
							cyberspace_minigame_patternmatching_inputkey(event.key.keysym.sym);
							break;
						}
					}
				break;
			default:
				fprintf(stderr, "FATAL ERROR: unexpected cyberspace_phase: %d.\n", cyberspace_phase);
				exit(EXIT_FAILURE);
				break;
			}
		tosleep.tv_sec=0;
		tosleep.tv_nsec=10*1000000L;
		while(nanosleep(&tosleep, &tosleepagain)!=0) {
			tosleep=tosleepagain;
			}
//		SDL_Delay(100);
		}

	printf("GUI input: ENDING\n");
//	pthread_exit(NULL);
	return(NULL);
	}

/*#include "SDL/SDL_ttf.h"
#define FONT_H 14
TTF_Font* font_TTFfont;
TTF_Font* font_TTFfont_mono;
SDL_Color font_color, font_color_black, font_color_white, font_color_gold;*/

int cyberspace_GUI_tf_sdl(void* nothing) {
	cyberspace_GUI_tf(nothing);
	return(0);
	}

void* cyberspace_GUI_tf(void* nothing) {
	printf("GUI: STARTED\n");

	cyberspace_GUI_screen_type=CYBERSPACE_GUI_UNSET;

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

/*	pthread_attr_t thread_detached;
	pthread_attr_init(&thread_detached);
	pthread_attr_setdetachstate(&thread_detached, PTHREAD_CREATE_DETACHED);*/

/*	if(pthread_create(&cyberspace_GUI_input_thread, &thread_detached, cyberspace_GUI_input_tf, NULL)!=0) {
		fprintf(stderr, "FATAL ERROR: GUI input thread creation fails.\n");
		exit(EXIT_FAILURE);
		}*/

	cyberspace_GUI_input_thread_sdl=SDL_CreateThread(cyberspace_GUI_input_tf_sdl, NULL);
	if(cyberspace_GUI_input_thread_sdl==NULL) {
		fprintf(stderr, "FATAL ERROR: GUI input thread creation fails: %s.\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}

	printf("GUI: init fonts\n");
	cyberspace_GUI_font_init();
/*	if(TTF_Init()==-1) {
		fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
		}
	font_TTFfont_mono=TTF_OpenFont("fonts/FreeMono.ttf", FONT_H);
	if(!font_TTFfont_mono) {
		fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
		exit(EXIT_FAILURE);
		}
	font_TTFfont=font_TTFfont_mono;
	font_color_black.r=0;
	font_color_black.g=0;
	font_color_black.b=0;
	font_color=font_color_black;
	font_color_white.r=255;
	font_color_white.g=255;
	font_color_white.b=255;
	font_color_gold.r=255;
	font_color_gold.g=255;
	font_color_gold.b=0;*/

	struct timespec tosleep;
	struct timespec tosleepagain;

	struct timeval tv_start;
	struct timeval tv_end;
	int milli_interval;
	int frames=0;

	if(gettimeofday(&tv_start, NULL)!=0) {
		fprintf(stderr, "FATAL ERROR: gettimeofday fails.\n");
		exit(EXIT_FAILURE);
		}

	while(true) {

//		printf("GUI: drawing\n");
//		pthread_mutex_lock(&cyberspace_mutex_phase);
		SDL_mutexP(cyberspace_mutex_phase_sdl);
//		printf("GUI: drawing (phase %d)\n", cyberspace_phase);
		switch(cyberspace_phase) {
			case CYBERSPACE_PHASE_CONNECTING:
/*				if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_SDL) cyberspace_GUI_screen_set_SDL();
				SDL_FillRect(cyberspace_GUI_screen, NULL, 0xffff0000);
				SDL_UpdateRect(cyberspace_GUI_screen, 0, 0, 0, 0);*/
				if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();
				cyberspace_connecting_print();
				break;
			case CYBERSPACE_PHASE_LOGIN:
/*				if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_SDL) cyberspace_GUI_screen_set_SDL();
				SDL_FillRect(cyberspace_GUI_screen, NULL, 0xff00ff00);
				SDL_UpdateRect(cyberspace_GUI_screen, 0, 0, 0, 0);*/
				if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();
				cyberspace_login_print();
				break;
			case CYBERSPACE_PHASE_3D:
				if(CYBERSPACE_DEBUG_GUI_MODE) {
					if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_SDL) cyberspace_GUI_screen_set_SDL();
					}
				else {
					if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();
					}
				cyberspace_world_print();
				break;
			case CYBERSPACE_PHASE_PROGRAMMING:
				if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();
				cyberspace_programming_print();
				break;
			case CYBERSPACE_PHASE_MINIGAME_TESTING:
				if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();
				cyberspace_minigame_testing_print();
				break;
			case CYBERSPACE_PHASE_MINIGAME_HEURISTIC:
				if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();
				cyberspace_minigame_heuristic_print();
				break;
			case CYBERSPACE_PHASE_MINIGAME_FAST:
				if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();
				cyberspace_minigame_fast_print();
				break;
			case CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING:
				if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();
				cyberspace_minigame_patternmatching_print();
				break;
			default:
				fprintf(stderr, "FATAL ERROR: unexpected cyberspace_phase: %d.\n", cyberspace_phase);
				exit(EXIT_FAILURE);
				break;
			}
		SDL_mutexV(cyberspace_mutex_phase_sdl);
//		pthread_mutex_unlock(&cyberspace_mutex_phase);

//		printf("GUI: sleeping\n");
//		tosleep.tv_sec=0;
//		tosleep.tv_nsec=10*1000000L;
//		while(nanosleep(&tosleep, &tosleepagain)!=0) {
//			tosleep=tosleepagain;
//			}
//		SDL_Delay(100);

		if(gettimeofday(&tv_end, NULL)!=0) {
			fprintf(stderr, "FATAL ERROR: gettimeofday fails.\n");
			exit(EXIT_FAILURE);
			}

		milli_interval=(tv_end.tv_sec-tv_start.tv_sec)*1000+(tv_end.tv_usec-tv_start.tv_usec)/1000;

		frames++;
		if(tv_end.tv_sec!=tv_start.tv_sec) {
			sprintf(cyberspace_fps, "fps: %d", frames);
			frames=0;
			}

		//approssimazione
		if(cyberspace_server_message_fade>0) {
			cyberspace_server_message_fade-=milli_interval;
			if(cyberspace_server_message_fade<0) cyberspace_server_message_fade=0;
			}

		if(milli_interval<CYBERSPACE_GUI_FRAME_FREQUENCY) {
			tosleep.tv_sec=0;
			tosleep.tv_nsec=(CYBERSPACE_GUI_FRAME_FREQUENCY-milli_interval)*1000000L;
			while(nanosleep(&tosleep, &tosleepagain)!=0) {
				tosleep=tosleepagain;
				}
			}

		tv_start=tv_end;
		}

	printf("GUI: ENDING\n");

	SDL_WaitThread(cyberspace_GUI_input_thread_sdl, NULL);

//	pthread_exit(NULL);
	return(NULL);
	}

void cyberspace_GUI_screen_set_SDL(void) {
	if(cyberspace_GUI_screen_type==CYBERSPACE_GUI_GL) SDL_FreeSurface(cyberspace_GUI_screen_gl);
	printf("WARNING!!!! setVideoMode\n");
	exit(EXIT_FAILURE);
	cyberspace_GUI_screen=SDL_SetVideoMode(cyberspace_screen_width, cyberspace_screen_height, 32, SDL_SWSURFACE);
	if(cyberspace_GUI_screen==NULL) {
		fprintf(stderr, "ABORT: SDL_SetVideoMode fails: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}
	SDL_WM_SetCaption("cyberspace", "cyberspace");
	cyberspace_GUI_screen_type=CYBERSPACE_GUI_SDL;
	}

void cyberspace_GUI_screen_set_GL(void) {
	printf("frame drawing: set GL: 0\n");
	if(cyberspace_GUI_screen_type==CYBERSPACE_GUI_SDL) {
		printf("frame drawing: set GL: 0.5\n");
		SDL_FreeSurface(cyberspace_GUI_screen);
		}
	printf("frame drawing: set GL: 1\n");
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
//	glFlush();
	printf("frame drawing: set GL: 2\n");
//	cyberspace_GUI_screen_gl=SDL_SetVideoMode(cyberspace_screen_width, cyberspace_screen_height, 0, SDL_OPENGL|SDL_FULLSCREEN);
	cyberspace_GUI_screen_gl=SDL_SetVideoMode(cyberspace_screen_width, cyberspace_screen_height, 0, SDL_OPENGL);
	printf("frame drawing: set GL: 2.5\n");
	if(cyberspace_GUI_screen_gl==NULL) {
		printf("frame drawing: set GL: 2.75\n");
		fprintf(stderr, "ABORT: SDL_SetVideoMode fails: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
		}
	printf("frame drawing: set GL: 3\n");
//	SDL_WM_SetCaption("cyberspace GL", "cyberspace GL");
	printf("frame drawing: set GL: 4\n");
	cyberspace_GUI_screen_type=CYBERSPACE_GUI_GL;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glViewport(0, 0, cyberspace_screen_width, cyberspace_screen_height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DITHER);
//	glEnable(GL_TEXTURE_2D);

//	glEnable(GL_POINT_SMOOTH);
//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if(CYBERSPACE_GUI_WIREFRAMES) glShadeModel(GL_FLAT);
	else {
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		glShadeModel(GL_SMOOTH);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

		GLfloat light_global_ambient[]={0.2, 0.2, 0.2, 1.0};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_global_ambient);

		glEnable(GL_LIGHT0);

//	GLfloat light0_position[]={10.0, 10.0, 10.0, 1.0};
//	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

		GLfloat light0_ambient[]={0.0, 0.0, 0.0, 1.0};
		glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);

		GLfloat light0_diffuse[]={1.0, 1.0, 1.0, 1.0};
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

		GLfloat light0_specular[]={1.0, 1.0, 1.0, 1.0};
		glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);

		glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
		glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0005);
		glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.000001);
		}

/*	glEnable(GL_LIGHT1);

	GLfloat light1_position[]={10.0, 10.0, 10.0, 1.0};
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

	GLfloat light1_ambient[]={0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

	GLfloat light1_diffuse[]={0.0, 0.0, 1.0, 1.0};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);

	GLfloat light1_specular[]={0.0, 0.0, 1.0, 1.0};
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);*/

//	glEnable(GL_COLOR_MATERIAL);
//	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

//	glEnable(GL_VERTEX_ARRAY);

	//loading textures
/*	for(int i=0;i<CYBERSPACE_GUI_TEXTURES_MAX;i++) {
		cyberspace_textures[i]=42;
		}
	glGenTextures(CYBERSPACE_GUI_TEXTURES_MAX, cyberspace_textures);
	for(int i=0;i<CYBERSPACE_GUI_TEXTURES_MAX;i++) {
		fprintf(stderr, "glGenTextures %d: %d\n", i, cyberspace_textures[i]);
		}*/
//	exit(EXIT_FAILURE);

/*	GLuint texture_id;
	glGenTextures(1, &texture_id);
	cyberspace_textures[0]=texture_id;*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

/*
	void* rawbuffer;
	FILE* rawfile;
	int l;

//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, cyberspace_textures[0]);
	l=32;
	rawbuffer=new onebyte[l*l*4];
	rawfile=fopen("img/tile_32_can.raw", "rb");
	if(rawfile==NULL) {
		fprintf(stderr, "ABORT: fopen raw fails\n");
		exit(EXIT_FAILURE);
		}
	fread(rawbuffer, l*l*4, 1, rawfile);
	fclose(rawfile);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, l, l, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawbuffer);

	glBindTexture(GL_TEXTURE_2D, cyberspace_textures[1]);
	l=64;
	rawbuffer=new onebyte[l*l*4];
	rawfile=fopen("img/self.raw", "rb");
	if(rawfile==NULL) {
		fprintf(stderr, "ABORT: fopen raw fails\n");
		exit(EXIT_FAILURE);
		}
	fread(rawbuffer, l*l*4, 1, rawfile);
	fclose(rawfile);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, l, l, 0, GL_RGBA, GL_UNSIGNED_BYTE, rawbuffer);
*/

	glFlush();
	cyberspace_sreen_reload=false;
	}

void cyberspace_world_print(void) {

//	printf("frame drawing: START\n");

//	pthread_mutex_lock(&cyberspace_mutex_world);
	//invocato nella selezione del *_print adatto
//	SDL_mutexP(cyberspace_mutex_world_sdl);

//	printf("frame drawing: locked\n");

	//frame init
	if(CYBERSPACE_DEBUG_GUI_MODE) {
		if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_SDL) cyberspace_GUI_screen_set_SDL();
		SDL_FillRect(cyberspace_GUI_screen, NULL, 0xff666600);
		}
	else {
//		printf("frame drawing: initing: 0\n");
		if(cyberspace_GUI_screen_type!=CYBERSPACE_GUI_GL) cyberspace_GUI_screen_set_GL();
//		printf("frame drawing: initing: 1\n");
		if(cyberspace_sreen_reload) {
			cyberspace_GUI_screen_set_SDL();
			cyberspace_GUI_screen_set_GL();
			}
//		printf("frame drawing: initing: 2\n");

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0, (double(cyberspace_screen_width))/((double)cyberspace_screen_height), 2.0, 10000.0);

//		glTranslatef(-cyberspace_world.camera.position.x+(600.0*sin(cyberspace_world.camera.angle.alpha)), -cyberspace_world.camera.position.y-200.0, -cyberspace_world.camera.position.z-(600.0*cos(cyberspace_world.camera.angle.alpha)));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glRotatef((cyberspace_world.camera.angle.alpha+M_PI/2.0)*180.0/M_PI, 0.0, 1.0, 0.0);
		glTranslatef(-cyberspace_world.camera.position.x, -cyberspace_world.camera.position.y-CYBERSPACE_WORLD_OBJECT_USER_H, -cyberspace_world.camera.position.z);

		if(!CYBERSPACE_GUI_WIREFRAMES) {
			GLfloat light0_position[]={cyberspace_world.camera.position.x, cyberspace_world.camera.position.y+CYBERSPACE_WORLD_OBJECT_USER_H, cyberspace_world.camera.position.z, 1.0};
			glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
			}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

//	printf("frame drawing: inited\n");

	//debug only
	//TODO remove
/*
	cyberspace_world.camera.angle.alpha+=cyberspace_GUI_rotation_alpha*10.0;
	while(cyberspace_world.camera.angle.alpha<0.0) cyberspace_world.camera.angle.alpha+=(2.0*M_PI);
	if(cyberspace_world.camera.angle.alpha>=(2.0*M_PI)) cyberspace_world.camera.angle.alpha-=(2.0*M_PI);

	cyberspace_world.camera.point.z-=cyberspace_GUI_traslation_ahead*10.0*cos(cyberspace_world.camera.angle.alpha);
	cyberspace_world.camera.point.x+=cyberspace_GUI_traslation_ahead*10.0*sin(cyberspace_world.camera.angle.alpha);
*/

	//drawing
	cyberspace_world_object* obj;
	cyberspace_world_point* center;
	cyberspace_world_angle* orientation;
	double w, h, d;
	double tessel_x_from, tessel_y_from;
//	double tessel_z_from; //unused
	double tessel_x_to, tessel_y_to, tessel_z_to;
//	double tessel_L; //unused
	cyberspace_world_point floor_0, floor_1, floor_2;
	cyberspace_world_point floor_0_tes_a, floor_1_tes_a;
	cyberspace_world_point floor_0_tes_b, floor_1_tes_b;
	double tessel_L_02, tessel_L_12, tessel_L_a, tessel_L_b;
	SDL_Rect rect;
	double sel_L_plane;
	double sel_L;
	double sel_teta;
	double sel_alpha;
	double sel_teta_y;
	double sel_acos;
	double sel_asin;
	double sel_teta_min=M_PI;
	int subtype;

	cyberspace_obj_selected.dynamic=false;
	cyberspace_obj_selected.id=-1;
	for(int i=0;i<cyberspace_world.static_size+cyberspace_world.dynamic_size;i++) {
		if(cyberspace_personal_data.id_static==-1) break;
		if(i<cyberspace_world.static_size) obj=&(cyberspace_world.static_obj[i]);
		else obj=&(cyberspace_world.dynamic_obj[i-cyberspace_world.static_size]);
		center=&(obj->point.position);

		//obj selection check
		if(!cyberspace_obj_selected.lock && (obj->type==CYBERSPACE_WORLD_OBJECT_CONSOLE || obj->type==CYBERSPACE_WORLD_OBJECT_USER || obj->type==CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD)) {
			sel_L_plane=sqrt((cyberspace_world.camera.position.x-center->x)*(cyberspace_world.camera.position.x-center->x)+(cyberspace_world.camera.position.z-center->z)*(cyberspace_world.camera.position.z-center->z));

			sel_acos=acos((center->x-cyberspace_world.camera.position.x)/sel_L_plane);
			sel_asin=asin((center->z-cyberspace_world.camera.position.z)/sel_L_plane);
			if(sel_acos<=M_PI/2.0) {
				if(sel_asin>=0.0) sel_alpha=sel_asin;
				else sel_alpha=(M_PI*2.0)+sel_asin;
				}
			else {
				if(sel_asin>=0.0) sel_alpha=sel_acos;
				else sel_alpha=(M_PI*2.0)-sel_acos;
				}
			sel_teta=sel_alpha-cyberspace_world.camera.angle.alpha;
			if(sel_teta<0.0) sel_teta*=-1.0;
			if(sel_teta>M_PI) sel_teta=2.0*M_PI-sel_teta;

			sel_L=sqrt(sel_L_plane*sel_L_plane+(cyberspace_world.camera.position.y-center->y)*(cyberspace_world.camera.position.y-center->y));

			sel_acos=acos(sel_L_plane/sel_L);
			sel_asin=asin((center->y-cyberspace_world.camera.position.y)/sel_L);
			if(sel_acos<=M_PI/2.0) {
				if(sel_asin>=0.0) sel_alpha=sel_asin;
				else sel_alpha=(M_PI*2.0)+sel_asin;
				}
			else {
				if(sel_asin>=0.0) sel_alpha=sel_acos;
				else sel_alpha=(M_PI*2.0)-sel_acos;
				}
			sel_teta_y=sel_alpha;
			if(sel_teta_y<0.0) sel_teta_y*=-1.0;
			if(sel_teta_y>M_PI) sel_teta_y=2.0*M_PI-sel_teta_y;

			if(sel_teta<M_PI/16.0 && sel_teta_y<M_PI/16.0 && sel_teta<sel_teta_min && sel_L<750.0) {
				sel_teta_min=sel_teta;
				if(i<cyberspace_world.static_size) {
					cyberspace_obj_selected.dynamic=false;
					cyberspace_obj_selected.id=i;
					}
				else {
					cyberspace_obj_selected.dynamic=true;
					cyberspace_obj_selected.id=i-cyberspace_world.static_size;
					}
				}
			}
		}

	for(int write_special_phase=0;write_special_phase<=1;write_special_phase++) {
		for(int i=0;i<cyberspace_world.static_size+cyberspace_world.dynamic_size;i++) {
			if(cyberspace_personal_data.id_static==-1) break;
			if(i<cyberspace_world.static_size) obj=&(cyberspace_world.static_obj[i]);
			else obj=&(cyberspace_world.dynamic_obj[i-cyberspace_world.static_size]);
			center=&(obj->point.position);
			orientation=&(obj->point.angle);

			if(!CYBERSPACE_DEBUG_GUI_MODE) glPushMatrix();
			switch(obj->type) {
				case CYBERSPACE_WORLD_OBJECT_USER:
					if(write_special_phase==1) break;
					w=15.0;
					h=50.0;
					d=15.0;
					if(CYBERSPACE_DEBUG_GUI_MODE) {
						rect.w=w;
						rect.h=d;
						rect.x=center->x-rect.w/2;
						rect.y=center->z-rect.h/2;
						SDL_FillRect(cyberspace_GUI_screen, &rect, 0xffffff00);
						rect.w=w/3;
						rect.h=d/3;
						rect.x=center->x-rect.w/2;
						rect.x+=10.0*cos(orientation->alpha);
						rect.y=center->z-rect.h/2;
						rect.y+=10.0*sin(orientation->alpha);
						SDL_FillRect(cyberspace_GUI_screen, &rect, 0xffff5500);
						}
					else {
	//					printf("printing %f? ", obj->parameters[CYBERSPACE_WORLD_OBJECT_USER_PARAMETER_ID_STATIC]);
						if(obj->parameters[CYBERSPACE_WORLD_OBJECT_USER_PARAMETER_ID_STATIC]==cyberspace_personal_data.id_static) {
	//						printf("no\n");
							break;
							}
	//					printf("yes\n");
						glTranslatef(center->x, center->y, center->z);
						glRotatef(orientation->alpha*180.0/M_PI, 0.0, 1.0, 0.0);
						cyberspace_GUI_materials_set(CYBERSPACE_GUI_MATERIALS_USER_GREEN);

						if(cyberspace_obj_selected.dynamic==true && cyberspace_obj_selected.id==(i-cyberspace_world.static_size)) {
							GLfloat material_emission_start[]={0.3, 0.3, 0.3, 1.0};
							glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_emission_start);
							}

						glBegin(GL_TRIANGLES);

						glNormal3f(1.0, CYBERSPACE_WORLD_OBJECT_USER_R/(CYBERSPACE_WORLD_OBJECT_USER_H/4.0), 0.0);
						glVertex3f(0.0, CYBERSPACE_WORLD_OBJECT_USER_H, 0.0);
						glVertex3f(CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, CYBERSPACE_WORLD_OBJECT_USER_R);
						glVertex3f(CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, -CYBERSPACE_WORLD_OBJECT_USER_R);

						glNormal3f(0.0, CYBERSPACE_WORLD_OBJECT_USER_R/(CYBERSPACE_WORLD_OBJECT_USER_H/4.0), -1.0);
						glVertex3f(0.0, CYBERSPACE_WORLD_OBJECT_USER_H, 0.0);
						glVertex3f(CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, -CYBERSPACE_WORLD_OBJECT_USER_R);
						glVertex3f(-CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, -CYBERSPACE_WORLD_OBJECT_USER_R);

						glNormal3f(-1.0, CYBERSPACE_WORLD_OBJECT_USER_R/(CYBERSPACE_WORLD_OBJECT_USER_H/4.0), 0.0);
						glVertex3f(0.0, CYBERSPACE_WORLD_OBJECT_USER_H, 0.0);
						glVertex3f(-CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, -CYBERSPACE_WORLD_OBJECT_USER_R);
						glVertex3f(-CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, CYBERSPACE_WORLD_OBJECT_USER_R);

						glNormal3f(0.0, CYBERSPACE_WORLD_OBJECT_USER_R/(CYBERSPACE_WORLD_OBJECT_USER_H/4.0), 1.0);
						glVertex3f(0.0, CYBERSPACE_WORLD_OBJECT_USER_H, 0.0);
						glVertex3f(-CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, CYBERSPACE_WORLD_OBJECT_USER_R);
						glVertex3f(CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, CYBERSPACE_WORLD_OBJECT_USER_R);

						glNormal3f(1.0, -CYBERSPACE_WORLD_OBJECT_USER_R/(CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0), 0.0);
						glVertex3f(CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, CYBERSPACE_WORLD_OBJECT_USER_R);
						glVertex3f(0.0, 0.0, 0.0);
						glVertex3f(CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, -CYBERSPACE_WORLD_OBJECT_USER_R);

						glNormal3f(0.0, -CYBERSPACE_WORLD_OBJECT_USER_R/(CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0), -1.0);
						glVertex3f(CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, -CYBERSPACE_WORLD_OBJECT_USER_R);
						glVertex3f(0.0, 0.0, 0.0);
						glVertex3f(-CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, -CYBERSPACE_WORLD_OBJECT_USER_R);

						glNormal3f(-1.0, -CYBERSPACE_WORLD_OBJECT_USER_R/(CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0), 0.0);
						glVertex3f(-CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, -CYBERSPACE_WORLD_OBJECT_USER_R);
						glVertex3f(0.0, 0.0, 0.0);
						glVertex3f(-CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, CYBERSPACE_WORLD_OBJECT_USER_R);

						glNormal3f(0.0, -CYBERSPACE_WORLD_OBJECT_USER_R/(CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0), 1.0);
						glVertex3f(-CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, CYBERSPACE_WORLD_OBJECT_USER_R);
						glVertex3f(0.0, 0.0, 0.0);
						glVertex3f(CYBERSPACE_WORLD_OBJECT_USER_R, CYBERSPACE_WORLD_OBJECT_USER_H*3.0/4.0, CYBERSPACE_WORLD_OBJECT_USER_R);

						glEnd();

						if(cyberspace_obj_selected.dynamic==true && cyberspace_obj_selected.id==(i-cyberspace_world.static_size)) {
							GLfloat material_emission_stop[]={0.0, 0.0, 0.0, 1.0};
							glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_emission_stop);
							}
						}
					break;
				case CYBERSPACE_WORLD_OBJECT_CUBE:
					if(write_special_phase==1) break;
					w=100.0;
					h=300.0;
					d=150.0;
					if(CYBERSPACE_DEBUG_GUI_MODE) {
						rect.w=w;
						rect.h=d;
						rect.x=center->x-rect.w/2;
						rect.y=center->z-rect.h/2;
						SDL_FillRect(cyberspace_GUI_screen, &rect, 0xffff00ff);
						}
					else {
						break;
						glTranslatef(center->x, center->y, center->z);
						glRotatef(orientation->alpha*180.0/M_PI, 0.0, 1.0, 0.0);
						glColor4f(0.5, 0.0, 0.0, 1.0);
						glBegin(GL_QUADS);
							glVertex3f(0.0, 0.0, 0.0);
							glVertex3f(w, 0.0, 0.0);
							glVertex3f(w, h, 0.0);
							glVertex3f(0.0, h, 0.0);

							glVertex3f(0.0, 0.0, d);
							glVertex3f(w, 0.0, d);
							glVertex3f(w, h, d);
							glVertex3f(0.0, h, d);

							glVertex3f(0.0, h, 0.0);
							glVertex3f(w, h, 0.0);
							glVertex3f(w, h, d);
							glVertex3f(0.0, h, d);
						glEnd();
						}
					break;
				case CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD:
					if(write_special_phase==1) break;
					w=30.0;
					h=30.0;
					d=30.0;
					if(CYBERSPACE_DEBUG_GUI_MODE) {
						rect.w=w;
						rect.h=d;
						rect.x=center->x-rect.w/2;
						rect.y=center->z-rect.h/2;
						SDL_FillRect(cyberspace_GUI_screen, &rect, 0xffffffff);
						}
					else {
						glTranslatef(center->x, center->y, center->z);
						glRotatef(orientation->alpha*180.0/M_PI, 0.0, 1.0, 0.0);

						if(obj->parameters[CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD_PARAMETER_SKIN]==0.0) cyberspace_GUI_materials_set(CYBERSPACE_GUI_MATERIALS_BRASS);
						else cyberspace_GUI_materials_set(CYBERSPACE_GUI_MATERIALS_RUBY);

						if(cyberspace_obj_selected.dynamic==false && cyberspace_obj_selected.id==i) {
							GLfloat material_emission_start[]={0.5, 0.5, 0.5, 1.0};
							glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_emission_start);
							}

						glBegin(GL_QUADS);
							glNormal3f(0.0, 1.0, 0.0);
							glVertex3f(-w/2.0, 15.0+h, d/2.0);
							glVertex3f(w/2.0, 15.0+h, d/2.0);
							glVertex3f(w/2.0, 15.0+h, -d/2.0);
							glVertex3f(-w/2.0, 15.0+h, -d/2.0);

							glNormal3f(0.0, -1.0, 0.0);
							glVertex3f(-w/2.0, 15.0, d/2.0);
							glVertex3f(w/2.0, 15.0, d/2.0);
							glVertex3f(w/2.0, 15.0, -d/2.0);
							glVertex3f(-w/2.0, 15.0, -d/2.0);

							glNormal3f(1.0, 0.0, 0.0);
							glVertex3f(w/2.0, 15.0, -d/2.0);
							glVertex3f(w/2.0, 15.0+h, -d/2.0);
							glVertex3f(w/2.0, 15.0+h, d/2.0);
							glVertex3f(w/2.0, 15.0, d/2.0);

							glNormal3f(-1.0, 0.0, 0.0);
							glVertex3f(-w/2.0, 15.0, d/2.0);
							glVertex3f(-w/2.0, 15.0+h, d/2.0);
							glVertex3f(-w/2.0, 15.0+h, -d/2.0);
							glVertex3f(-w/2.0, 15.0, -d/2.0);

							glNormal3f(0.0, 0.0, -1.0);
							glVertex3f(-w/2.0, 15.0, -d/2.0);
							glVertex3f(-w/2.0, 15.0+h, -d/2.0);
							glVertex3f(w/2.0, 15.0+h, -d/2.0);
							glVertex3f(w/2.0, 15.0, -d/2.0);

							glNormal3f(0.0, 0.0, 1.0);
							glVertex3f(w/2.0, 15.0, d/2.0);
							glVertex3f(w/2.0, 15.0+h, d/2.0);
							glVertex3f(-w/2.0, 15.0+h, d/2.0);
							glVertex3f(-w/2.0, 15.0, d/2.0);
						glEnd();

						if(cyberspace_obj_selected.dynamic==false && cyberspace_obj_selected.id==i) {
							GLfloat material_emission_stop[]={0.0, 0.0, 0.0, 1.0};
							glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_emission_stop);
							}
						}
					break;
				case CYBERSPACE_WORLD_OBJECT_CONSOLE:
					if(write_special_phase==1) break;
					w=50.0;
					h=35.0;
					d=3.0;
					if(CYBERSPACE_DEBUG_GUI_MODE) {
						rect.w=w;
						rect.h=d;
						rect.x=center->x-rect.w/2;
						rect.y=center->z-rect.h/2;
						SDL_FillRect(cyberspace_GUI_screen, &rect, 0xffffffff);
						}
					else {
						glTranslatef(center->x, center->y, center->z);
						glRotatef(orientation->alpha*180.0/M_PI, 0.0, 1.0, 0.0);
						cyberspace_GUI_materials_set(CYBERSPACE_GUI_MATERIALS_OBSIDIAN);

						if(cyberspace_obj_selected.dynamic==false && cyberspace_obj_selected.id==i) {
							GLfloat material_emission_start[]={0.2, 0.2, 0.2, 1.0};
							glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_emission_start);
							}

						glBegin(GL_QUADS);
							glNormal3f(0.0, 1.0, 0.0);
							glVertex3f(-w/2.0, 30.0+h, d/2.0);
							glVertex3f(w/2.0, 30.0+h, d/2.0);
							glVertex3f(w/2.0, 30.0+h, -d/2.0);
							glVertex3f(-w/2.0, 30.0+h, -d/2.0);

							glNormal3f(0.0, -1.0, 0.0);
							glVertex3f(-w/2.0, 30.0, d/2.0);
							glVertex3f(w/2.0, 30.0, d/2.0);
							glVertex3f(w/2.0, 30.0, -d/2.0);
							glVertex3f(-w/2.0, 30.0, -d/2.0);

							glNormal3f(1.0, 0.0, 0.0);
							glVertex3f(w/2.0, 30.0, -d/2.0);
							glVertex3f(w/2.0, 30.0+h, -d/2.0);
							glVertex3f(w/2.0, 30.0+h, d/2.0);
							glVertex3f(w/2.0, 30.0, d/2.0);

							glNormal3f(-1.0, 0.0, 0.0);
							glVertex3f(-w/2.0, 30.0, d/2.0);
							glVertex3f(-w/2.0, 30.0+h, d/2.0);
							glVertex3f(-w/2.0, 30.0+h, -d/2.0);
							glVertex3f(-w/2.0, 30.0, -d/2.0);

							glNormal3f(0.0, 0.0, -1.0);
							glVertex3f(-w/2.0, 30.0, -d/2.0);
							glVertex3f(-w/2.0, 30.0+h, -d/2.0);
							glVertex3f(w/2.0, 30.0+h, -d/2.0);
							glVertex3f(w/2.0, 30.0, -d/2.0);

							glNormal3f(0.0, 0.0, 1.0);
							glVertex3f(w/2.0, 30.0, d/2.0);
							glVertex3f(w/2.0, 30.0+h, d/2.0);
							glVertex3f(-w/2.0, 30.0+h, d/2.0);
							glVertex3f(-w/2.0, 30.0, d/2.0);
						glEnd();

						if(cyberspace_obj_selected.dynamic==false && cyberspace_obj_selected.id==i) {
							GLfloat material_emission_stop[]={0.0, 0.0, 0.0, 1.0};
							glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_emission_stop);
							}
						}
					break;
				case CYBERSPACE_WORLD_OBJECT_REDLINE:
					if(write_special_phase==1) break;
					w=1.0;
					h=1.0;
					d=1.0;
					if(CYBERSPACE_DEBUG_GUI_MODE) {
						rect.w=w;
						rect.h=d;
						for(double j=-0.5;j<=0.5;j+=0.01) {
							rect.x=center->x+round(cos(orientation->alpha)*j*obj->parameters[CYBERSPACE_WORLD_OBJECT_REDLINE_PARAMETER_LENGTH])-rect.w/2;
							rect.y=center->z+round(sin(orientation->alpha)*j*obj->parameters[CYBERSPACE_WORLD_OBJECT_REDLINE_PARAMETER_LENGTH])-rect.h/2;
							SDL_FillRect(cyberspace_GUI_screen, &rect, 0xffff0000);
							}
						}
					break;
				case CYBERSPACE_WORLD_OBJECT_WALL:
//					if(write_special_phase==1) break;
					if(CYBERSPACE_DEBUG_GUI_MODE) {
						w=1.0;
						h=1.0;
						d=1.0;
						rect.w=w;
						rect.h=d;
						for(double j=-0.5;j<=0.5;j+=0.01) {
							rect.x=center->x+round(cos(orientation->alpha)*j*obj->parameters[CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_LENGTH])-rect.w/2;
							rect.y=center->z+round(sin(orientation->alpha)*j*obj->parameters[CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_LENGTH])-rect.h/2;
							SDL_FillRect(cyberspace_GUI_screen, &rect, 0xffff0000);
							}
						}
					else {
						w=obj->parameters[CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_LENGTH];
						h=obj->parameters[CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_HEIGHT];
						subtype=obj->parameters[CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE];
						if(subtype==CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_GLASS_STEEL || subtype==CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_FORCEFIELD) {
							if(write_special_phase==0) break;
							}
						else {
							if(write_special_phase==1) break;
							}
						glTranslatef(center->x, center->y, center->z);
						glRotatef(orientation->alpha*180.0/M_PI, 0.0, 1.0, 0.0);
						if(CYBERSPACE_GUI_WIREFRAMES) {
							glColor4f(1.0, 1.0, 0.8, 1.0);
							glBegin(GL_LINE_STRIP);
							}
						else {
							switch(subtype) {
								case CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_STEEL:
									cyberspace_GUI_materials_set(CYBERSPACE_GUI_MATERIALS_STEEL);
									break;
								case CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_GLASS_STEEL:
									cyberspace_GUI_materials_set(CYBERSPACE_GUI_MATERIALS_GLASS_STEEL);
									break;
								case CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_WIRE:
									cyberspace_GUI_materials_set(CYBERSPACE_GUI_MATERIALS_USER_GREEN);
									break;
								case CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_FORCEFIELD:
									cyberspace_GUI_materials_set(CYBERSPACE_GUI_MATERIALS_FORCEFIELD_BLUE);
									break;
								default:
									fprintf(stderr, "ABORT: unknown wall subtype %d\n", subtype);
									exit(EXIT_FAILURE);
									break;
								}
							glBegin(GL_QUADS);
							glNormal3f(0.0, 0.0, 1.0);
							}
						if(subtype==CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_WIRE) {
							for(int tessel_i=0;tessel_i<int(ceil(w/CYBERSPACE_GUI_TESSELLATION_SIZE_WALL_WIRE));tessel_i++) {
								tessel_x_from=tessel_i*CYBERSPACE_GUI_TESSELLATION_SIZE_WALL_WIRE;
								tessel_x_to=(tessel_i+1)*CYBERSPACE_GUI_TESSELLATION_SIZE_WALL_WIRE;
								if(tessel_x_to>w) tessel_x_to=w;
								tessel_x_from-=w/2.0;
								tessel_x_to-=w/2.0;
								for(int tessel_j=0;tessel_j<int(ceil(h/CYBERSPACE_GUI_TESSELLATION_SIZE_WALL_WIRE));tessel_j++) {
									tessel_y_from=tessel_j*CYBERSPACE_GUI_TESSELLATION_SIZE_WALL_WIRE;
									tessel_y_to=(tessel_j+1)*CYBERSPACE_GUI_TESSELLATION_SIZE_WALL_WIRE;
									if(tessel_y_to>h) tessel_y_to=h;

									glVertex3f(tessel_x_from, tessel_y_from, 0.0);
									glVertex3f(tessel_x_to, tessel_y_from, 0.0);
									glVertex3f(tessel_x_to-2.0, tessel_y_from+2.0, 0.0);
									glVertex3f(tessel_x_from+2.0, tessel_y_from+2.0, 0.0);

									glVertex3f(tessel_x_to, tessel_y_from, 0.0);
									glVertex3f(tessel_x_to, tessel_y_to, 0.0);
									glVertex3f(tessel_x_to-2.0, tessel_y_to-2.0, 0.0);
									glVertex3f(tessel_x_to-2.0, tessel_y_from+2.0, 0.0);

									glVertex3f(tessel_x_to, tessel_y_to, 0.0);
									glVertex3f(tessel_x_from, tessel_y_to, 0.0);
									glVertex3f(tessel_x_from+2.0, tessel_y_to-2.0, 0.0);
									glVertex3f(tessel_x_to-2.0, tessel_y_to-2.0, 0.0);

									glVertex3f(tessel_x_from, tessel_y_to, 0.0);
									glVertex3f(tessel_x_from, tessel_y_from, 0.0);
									glVertex3f(tessel_x_from+2.0, tessel_y_from+2.0, 0.0);
									glVertex3f(tessel_x_from+2.0, tessel_y_to-2.0, 0.0);
									}
								}
							}
						else {
							for(int tessel_i=0;tessel_i<int(ceil(w/CYBERSPACE_GUI_TESSELLATION_SIZE_WALL));tessel_i++) {
								tessel_x_from=tessel_i*CYBERSPACE_GUI_TESSELLATION_SIZE_WALL;
								tessel_x_to=(tessel_i+1)*CYBERSPACE_GUI_TESSELLATION_SIZE_WALL;
								if(tessel_x_to>w) tessel_x_to=w;
								tessel_x_from-=w/2.0;
								tessel_x_to-=w/2.0;
								for(int tessel_j=0;tessel_j<int(ceil(h/CYBERSPACE_GUI_TESSELLATION_SIZE_WALL));tessel_j++) {
									tessel_y_from=tessel_j*CYBERSPACE_GUI_TESSELLATION_SIZE_WALL;
									tessel_y_to=(tessel_j+1)*CYBERSPACE_GUI_TESSELLATION_SIZE_WALL;
									if(tessel_y_to>h) tessel_y_to=h;

									glVertex3f(tessel_x_from, tessel_y_from, 0.0);
									glVertex3f(tessel_x_to, tessel_y_from, 0.0);
									glVertex3f(tessel_x_to, tessel_y_to, 0.0);
									glVertex3f(tessel_x_from, tessel_y_to, 0.0);
									}
								}
							}
						glEnd();
						}
					break;
				case CYBERSPACE_WORLD_OBJECT_FLOOR:
					if(write_special_phase==1) break;
					if(CYBERSPACE_DEBUG_GUI_MODE) break;
					floor_0.x=obj->parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_X0];
					floor_0.z=obj->parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_Z0];
					floor_1.x=obj->parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_X1];
					floor_1.z=obj->parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_Z1];
					floor_2.x=obj->parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_X2];
					floor_2.z=obj->parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_Z2];
					glTranslatef(center->x, center->y, center->z);
					glRotatef(orientation->alpha*180.0/M_PI, 0.0, 1.0, 0.0);
					if(CYBERSPACE_GUI_WIREFRAMES) {
						glColor4f(1.0, 0.5, 0.5, 1.0);
						glBegin(GL_LINES);
						}
					else {
						cyberspace_GUI_materials_set(CYBERSPACE_GUI_MATERIALS_RUBBER_RED);
						glBegin(GL_QUAD_STRIP);
						glNormal3f(0.0, 1.0, 0.0);
						}
					tessel_L_02=sqrt((floor_2.x-floor_0.x)*(floor_2.x-floor_0.x)+(floor_2.z-floor_0.z)*(floor_2.z-floor_0.z));
					tessel_L_12=sqrt((floor_2.x-floor_1.x)*(floor_2.x-floor_1.x)+(floor_2.z-floor_1.z)*(floor_2.z-floor_1.z));
					for(int tessel_j=0;tessel_j<int(ceil(tessel_L_02/CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR));tessel_j++) {

						floor_0_tes_a.x=floor_0.x+tessel_j*(floor_2.x-floor_0.x)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_02;
						floor_0_tes_a.z=floor_0.z+tessel_j*(floor_2.z-floor_0.z)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_02;

						floor_1_tes_a.x=floor_1.x+tessel_j*(floor_2.x-floor_1.x)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_02;
						floor_1_tes_a.z=floor_1.z+tessel_j*(floor_2.z-floor_1.z)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_02;

						tessel_L_a=sqrt((floor_1_tes_a.x-floor_0_tes_a.x)*(floor_1_tes_a.x-floor_0_tes_a.x)+(floor_1_tes_a.z-floor_0_tes_a.z)*(floor_1_tes_a.z-floor_0_tes_a.z));

						if((tessel_j+1)>=int(ceil(tessel_L_02/CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR))) {
							glEnd();
							if(CYBERSPACE_GUI_WIREFRAMES) glBegin(GL_LINES);
							else glBegin(GL_TRIANGLE_FAN);
							glNormal3f(0.0, -1.0, 0.0);
							glVertex3f(floor_2.x, 0.0, floor_2.z);
							glVertex3f(floor_0_tes_a.x, 0.0, floor_0_tes_a.z);

							for(int tessel_i=0;tessel_i<int(ceil(tessel_L_a/CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR));tessel_i++) {

								if((tessel_i+1)>=int(ceil(tessel_L_a/CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR))) {
									tessel_x_to=floor_1_tes_a.x;
									tessel_z_to=floor_1_tes_a.z;
									}
								else {
									tessel_x_to=floor_0_tes_a.x+(tessel_i+1)*(floor_1_tes_a.x-floor_0_tes_a.x)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_a;
									tessel_z_to=floor_0_tes_a.z+(tessel_i+1)*(floor_1_tes_a.z-floor_0_tes_a.z)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_a;
									}

								if(CYBERSPACE_GUI_WIREFRAMES) glVertex3f(floor_2.x, 0.0, floor_2.z);
								glVertex3f(tessel_x_to, 0.0, tessel_z_to);
								}
							}
						else {
							glEnd();
							if(CYBERSPACE_GUI_WIREFRAMES) {
								glBegin(GL_LINES);
								glVertex3f(floor_0_tes_a.x, 0.0, floor_0_tes_a.z);
								glVertex3f(floor_1_tes_a.x, 0.0, floor_1_tes_a.z);
								}
							else glBegin(GL_QUAD_STRIP);

							floor_0_tes_b.x=floor_0.x+(tessel_j+1)*(floor_2.x-floor_0.x)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_02;
							floor_0_tes_b.z=floor_0.z+(tessel_j+1)*(floor_2.z-floor_0.z)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_02;

							floor_1_tes_b.x=floor_1.x+(tessel_j+1)*(floor_2.x-floor_1.x)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_02;
							floor_1_tes_b.z=floor_1.z+(tessel_j+1)*(floor_2.z-floor_1.z)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_02;

							tessel_L_b=sqrt((floor_1_tes_b.x-floor_0_tes_b.x)*(floor_1_tes_b.x-floor_0_tes_b.x)+(floor_1_tes_b.z-floor_0_tes_b.z)*(floor_1_tes_b.z-floor_0_tes_b.z));

	//						glVertex3f(floor_0_tes_b.x, 5.0, floor_0_tes_b.z);
	//						glVertex3f(floor_1_tes_b.x, 5.0, floor_1_tes_b.z);

							if(!CYBERSPACE_GUI_WIREFRAMES) {
								glVertex3f(floor_0_tes_a.x, 0.0, floor_0_tes_a.z);
								glVertex3f(floor_0_tes_b.x, 0.0, floor_0_tes_b.z);
								}

							for(int tessel_i=0;tessel_i<int(ceil(tessel_L_a/CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR));tessel_i++) {
								if((tessel_i+1)>=int(ceil(tessel_L_a/CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR))) {
									tessel_x_to=floor_1_tes_a.x;
									tessel_z_to=floor_1_tes_a.z;
									}
								else {
									tessel_x_to=floor_0_tes_a.x+(tessel_i+1)*(floor_1_tes_a.x-floor_0_tes_a.x)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_a;
									tessel_z_to=floor_0_tes_a.z+(tessel_i+1)*(floor_1_tes_a.z-floor_0_tes_a.z)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_a;
									}

								glVertex3f(tessel_x_to, 0.0, tessel_z_to);

								if((tessel_i+1)>=int(ceil(tessel_L_a/CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR))) {
									tessel_x_to=floor_1_tes_b.x;
									tessel_z_to=floor_1_tes_b.z;
									}
								else {
									tessel_x_to=floor_0_tes_b.x+(tessel_i+1)*(floor_1_tes_b.x-floor_0_tes_b.x)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_a;
									tessel_z_to=floor_0_tes_b.z+(tessel_i+1)*(floor_1_tes_b.z-floor_0_tes_b.z)*CYBERSPACE_GUI_TESSELLATION_SIZE_FLOOR/tessel_L_a;
									}

								glVertex3f(tessel_x_to, 0.0, tessel_z_to);
								}
							}
						}
					glEnd();
					break;
				case CYBERSPACE_WORLD_OBJECT_VECTOR:
					if(write_special_phase==1) break;
					w=1.0;
					h=1.0;
					d=1.0;
					if(CYBERSPACE_DEBUG_GUI_MODE) {
						rect.w=w;
						rect.h=d;
						for(double j=0.0;j<=1.0;j+=0.01) {
							rect.x=center->x+round(cos(orientation->alpha)*j*obj->parameters[CYBERSPACE_WORLD_OBJECT_VECTOR_PARAMETER_LENGTH])-rect.w/2;
							rect.y=center->z+round(sin(orientation->alpha)*j*obj->parameters[CYBERSPACE_WORLD_OBJECT_VECTOR_PARAMETER_LENGTH])-rect.h/2;
							SDL_FillRect(cyberspace_GUI_screen, &rect, 0xffff00ff);
							}
						}
					break;
				default:
					fprintf(stderr, "FATAL ERROR: unknown obj->type: %d\n", obj->type);
					exit(EXIT_FAILURE);
				}
			if(!CYBERSPACE_DEBUG_GUI_MODE) glPopMatrix();
			}
		}

	if(CYBERSPACE_DEBUG_GUI_MODE) {
/*		SDL_Surface* txt;
		SDL_Rect txtTo;
		char* fps=new char[100];

		sprintf(fps, "%d", frames);
		frames=0;
		txt=TTF_RenderText_Solid(font_TTFfont, fps, font_color);
		if(txt==NULL) {
			fprintf(stderr, "font_print: rendertext fails: %s\n", TTF_GetError());
			exit(EXIT_FAILURE);
			}
		txtTo.x=5;
		txtTo.y=5;
		if(SDL_BlitSurface(txt, NULL, cyberspace_GUI_screen, &txtTo)<0) {
			fprintf(stderr, "SDL_BlitSurface fails: %s\n", SDL_GetError());
			exit(EXIT_FAILURE);
			}
		SDL_FreeSurface(txt);

		if(cyberspace_obj_selected.id!=-1) {
			if(cyberspace_obj_selected.dynamic) obj=&(cyberspace_world.dynamic_obj[cyberspace_obj_selected.id]);
			else obj=&(cyberspace_world.static_obj[cyberspace_obj_selected.id]);
//			sprintf(fps, "%s - %d - L:%f - (x:%.1f - y:%.1f- x:%.1f)", (cyberspace_obj_selected.dynamic ? "dyn" : "sta"), cyberspace_obj_selected.id, sqrt((cyberspace_world.camera.position.x-obj->point.position.x)*(cyberspace_world.camera.position.x-obj->point.position.x)+(cyberspace_world.camera.position.z-obj->point.position.z)*(cyberspace_world.camera.position.z-obj->point.position.z)), obj->point.position.x, obj->point.position.y, obj->point.position.z);
			txt=TTF_RenderText_Solid(font_TTFfont, fps, font_color);
			if(txt==NULL) {
				fprintf(stderr, "font_print: rendertext fails: %s\n", TTF_GetError());
				exit(EXIT_FAILURE);
				}
			txtTo.x=5;
			txtTo.y=15;
			if(SDL_BlitSurface(txt, NULL, cyberspace_GUI_screen, &txtTo)<0) {
				fprintf(stderr, "SDL_BlitSurface fails: %s\n", SDL_GetError());
				exit(EXIT_FAILURE);
				}
			SDL_FreeSurface(txt);
			}
		delete[] fps;*/
		}


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

	if(cyberspace_obj_selected.id!=-1) {
		if(cyberspace_obj_selected.dynamic) obj=&cyberspace_world.dynamic_obj[cyberspace_obj_selected.id];
		else obj=&cyberspace_world.static_obj[cyberspace_obj_selected.id];
		double distance_sel_min=0;
		if(obj->type==CYBERSPACE_WORLD_OBJECT_TELEPORT_INTERWORLD) {
			sprintf(cyberspace_selection_message, "teletrasporto");
			distance_sel_min=200.0;
			}
		else if(obj->type==CYBERSPACE_WORLD_OBJECT_USER) {
			sprintf(cyberspace_selection_message, "umano connesso");
			distance_sel_min=200.0;
			}
		else if(obj->type==CYBERSPACE_WORLD_OBJECT_CONSOLE) {
			sprintf(cyberspace_selection_message, "console di hacking cognitivo");
			distance_sel_min=200.0;
			}
		double distance_sel=sqrt((obj->point.position.x-cyberspace_world.camera.position.x)*(obj->point.position.x-cyberspace_world.camera.position.x)+(obj->point.position.y-cyberspace_world.camera.position.y)*(obj->point.position.y-cyberspace_world.camera.position.y)+(obj->point.position.z-cyberspace_world.camera.position.z)*(obj->point.position.z-cyberspace_world.camera.position.z));
		sprintf(cyberspace_selection_message+strlen(cyberspace_selection_message), "\nx: %.3f\ny: %.3f\nz: %.3f\ndist: %.3f", obj->point.position.x-cyberspace_world.camera.position.x, obj->point.position.y-cyberspace_world.camera.position.y, obj->point.position.z-cyberspace_world.camera.position.z, distance_sel);
		if(distance_sel<(distance_sel_min*0.8)) glColor4f(0.0, 0.8, 0.0, 1.0);
		else glColor4f(0.0, 0.5, 0.0, 1.0);
		cyberspace_GUI_font_print(5, 15, 100, cyberspace_selection_message);
		}

	//invocato nella selezione del *_print adatto
//	SDL_mutexV(cyberspace_mutex_world_sdl);
//	pthread_mutex_unlock(&cyberspace_mutex_world);

	//chat system
	glColor4f(1.0, 1.0, 1.0, 1.0);
	cyberspace_GUI_font_print(10, cyberspace_screen_height-10, 100, cyberspace_chat_input);
	if(cyberspace_GUI_input_focus==CYBERSPACE_GUI_INPUT_FOCUS_CHAT) {
		cyberspace_GUI_font_print(10+strlen(cyberspace_chat_input)*10, cyberspace_screen_height-10+2, 100, "_");
		}

	char* chattmp;
	for(int i=0;i<CYBERSPACE_GUI_CHAT_HISTORY;i++) {
		if(strncmp(cyberspace_chat[i], "error ", 6)==0) {
			glColor4f(1.0, 0.2, 0.2, 1.0);
			cyberspace_GUI_font_print(10, cyberspace_screen_height-25-i*15, 100, cyberspace_chat[i]);
			}
		else if(strncmp(cyberspace_chat[i], "system ", 7)==0) {
			glColor4f(0.2, 0.2, 1.0, 1.0);
			cyberspace_GUI_font_print(10, cyberspace_screen_height-25-i*15, 100, cyberspace_chat[i]);
			}
		else {
			chattmp=strchr(cyberspace_chat[i], ':');
			if(chattmp==NULL || *(chattmp+1)!=' ') {
				glColor4f(1.0, 1.0, 1.0, 1.0);
				cyberspace_GUI_font_print(10, cyberspace_screen_height-25-i*15, 100, cyberspace_chat[i]);
				}
			else {
				*(chattmp+1)='\0';
				glColor4f(0.2, 1.0, 0.2, 1.0);
				cyberspace_GUI_font_print(10, cyberspace_screen_height-25-i*15, 100, cyberspace_chat[i]);
				*(chattmp+1)=' ';
				glColor4f(0.0, 0.8, 0.0, 1.0);
				cyberspace_GUI_font_print(10+(chattmp+1-cyberspace_chat[i])*10, cyberspace_screen_height-25-i*15, 100, chattmp+1);
				}
			}
		}

	if(cyberspace_server_message_fade>0 && strlen(cyberspace_server_message)>0) {
		glColor4f(1.0, 0.0, 0.0, 1.0);
		cyberspace_GUI_font_print((cyberspace_screen_width-(strlen(cyberspace_server_message)*10))/2, 20, 100, cyberspace_server_message);
		}

	glColor4f(1.0, 1.0, 1.0, 1.0);
	cyberspace_GUI_font_print(cyberspace_screen_width-100, 15, 100, cyberspace_fps);

	if(!CYBERSPACE_GUI_WIREFRAMES) {
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		}
	glEnable(GL_DEPTH_TEST);

	//frame ending
	if(CYBERSPACE_DEBUG_GUI_MODE) {
		SDL_UpdateRect(cyberspace_GUI_screen, 0, 0, 0, 0);
		}
	else {
		glFlush();
		SDL_GL_SwapBuffers();
		}

//	printf("frame drawing: END\n");
	}

void cyberspace_login_print(void) {
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

	cyberspace_GUI_font_print((cyberspace_screen_width-(20*10))/2, (cyberspace_screen_height+13)/2-50, 100, "nick");

	glBegin(GL_LINE_STRIP);
		glVertex3f((cyberspace_screen_width-20*10)/2-10, (cyberspace_screen_height)/2+6-13-5-30, 0.0);
		glVertex3f((cyberspace_screen_width+20*10)/2+10, (cyberspace_screen_height)/2+6-13-5-30, 0.0);
		glVertex3f((cyberspace_screen_width+20*10)/2+10, (cyberspace_screen_height)/2+6+5-30, 0.0);
		glVertex3f((cyberspace_screen_width-20*10)/2-10, (cyberspace_screen_height)/2+6+5-30, 0.0);
		glVertex3f((cyberspace_screen_width-20*10)/2-10, (cyberspace_screen_height)/2+6-13-5-30, 0.0);
	glEnd();

	cyberspace_GUI_font_print((cyberspace_screen_width-(20*10))/2, (cyberspace_screen_height+13)/2-30, 100, cyberspace_login_name);
	if(cyberspace_GUI_input_focus==CYBERSPACE_GUI_INPUT_FOCUS_LOGIN) {
		cyberspace_GUI_font_print((cyberspace_screen_width-(20*10))/2+strlen(cyberspace_login_name)*10, (cyberspace_screen_height+13)/2-30+2, 100, "_");
		}

	cyberspace_GUI_font_print((cyberspace_screen_width-(20*10))/2, (cyberspace_screen_height+13)/2+10, 100, "password");

	glBegin(GL_LINE_STRIP);
		glVertex3f((cyberspace_screen_width-20*10)/2-10, (cyberspace_screen_height)/2+6-13-5+30, 0.0);
		glVertex3f((cyberspace_screen_width+20*10)/2+10, (cyberspace_screen_height)/2+6-13-5+30, 0.0);
		glVertex3f((cyberspace_screen_width+20*10)/2+10, (cyberspace_screen_height)/2+6+5+30, 0.0);
		glVertex3f((cyberspace_screen_width-20*10)/2-10, (cyberspace_screen_height)/2+6+5+30, 0.0);
		glVertex3f((cyberspace_screen_width-20*10)/2-10, (cyberspace_screen_height)/2+6-13-5+30, 0.0);
	glEnd();

	//cyberspace_GUI_font_print((cyberspace_screen_width-(20*10))/2, (cyberspace_screen_height+13)/2+30, 100, cyberspace_login_password);
	for(uint i=0;i<strlen(cyberspace_login_password);i++) {
		cyberspace_GUI_font_print((cyberspace_screen_width-(20*10))/2+(i*10), (cyberspace_screen_height+13)/2+30, 100, "*");
		}
	if(cyberspace_GUI_input_focus==CYBERSPACE_GUI_INPUT_FOCUS_PASSWORD) {
		cyberspace_GUI_font_print((cyberspace_screen_width-(20*10))/2+strlen(cyberspace_login_password)*10, (cyberspace_screen_height+13)/2+30+2, 100, "_");
		}

	cyberspace_GUI_font_print((cyberspace_screen_width-(20*10))/2, (cyberspace_screen_height+13)/2+60, 100, "TAB   : cambia campo\nRETURN: invia dati");

	if(cyberspace_server_message_fade>0 && strlen(cyberspace_server_message)>0) {
		glColor4f(1.0, 0.0, 0.0, 1.0);
		cyberspace_GUI_font_print((cyberspace_screen_width-(strlen(cyberspace_server_message)*10))/2, (cyberspace_screen_height+13)/2+100, 100, cyberspace_server_message);
		}

	if(!CYBERSPACE_GUI_WIREFRAMES) {
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		}
	glEnable(GL_DEPTH_TEST);

	glFlush();
	SDL_GL_SwapBuffers();
	}

void cyberspace_connecting_print(void) {
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
	cyberspace_GUI_font_print((cyberspace_screen_width-(20*10))/2, (cyberspace_screen_height+13)/2, 100, "connessione in corso");

	if(!CYBERSPACE_GUI_WIREFRAMES) {
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		}
	glEnable(GL_DEPTH_TEST);

	glFlush();
	SDL_GL_SwapBuffers();
	}

void cyberspace_programming_print(void) {
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
	char* msg_skillname=new char[100];
	int subsyscount;
	int totalsubsyscount;
	int position;
	bool programmed;
	bool programmed_failed;

	switch(cyberspace_programming_subphase) {
		case CYBERSPACE_PHASE_PROGRAMMING_SYS:
			cyberspace_GUI_font_print(10, y_tmp, 100, "programmazione cognitiva - sistemi");
			y_tmp+=20;
			if(cyberspace_server_message_fade>0 && strlen(cyberspace_server_message)>0) {
				glColor4f(1.0, 0.0, 0.0, 1.0);
				cyberspace_GUI_font_print((cyberspace_screen_width-(strlen(cyberspace_server_message)*10))/2, 20, 100, cyberspace_server_message);
				glColor4f(0.0, 0.8, 0.0, 1.0);
				}
			sprintf(msg, "Per potere utilizzare un processo cognitivo artificiale devi avere\nil suo sistema di riferimento caricato nel connettore neurale.\nIl livello del sistema indica la quantita' di processi cognitivi artificiali\nche e' in grado di gestire.\nGrazie alle tue competenze di programmazione sei in grado di caricare sul\nconettore fino a %d sistemi.\nPremi il numero del sistema per caricarlo o scaricarlo dal connettore neurale", cyberspace_personal_data.systems_max);
			cyberspace_GUI_font_print(10, y_tmp, 100, msg);
			y_tmp+=120;
			for(int sys=0;sys<CYBERSPACE_NEURAL_SYSTEM_MAX;sys++) {
				switch(sys) {
					case CYBERSPACE_NEURAL_SYSTEM_MOVEMENT:
						sprintf(msg, "[0] - MOVIMENTO ");
						break;
					case CYBERSPACE_NEURAL_SYSTEM_SCAN:
						sprintf(msg, "[1] - SCANSIONE ");
						break;
					case CYBERSPACE_NEURAL_SYSTEM_OBFUSCATION:
						sprintf(msg, "[2] - OFFUSCAMENTO ");
						break;
					case CYBERSPACE_NEURAL_SYSTEM_ATTACK:
						sprintf(msg, "[3] - ATTACCO ");
						break;
					case CYBERSPACE_NEURAL_SYSTEM_DEFENCE:
						sprintf(msg, "[4] - DIFESA ");
						break;
					default:
						fprintf(stderr, "FATAL ERROR: unknown system %d\n", sys);
						exit(EXIT_FAILURE);
						break;
					}
				switch(cyberspace_personal_data.systems[sys]) {
					case 1:
						sprintf(msg+strlen(msg), "I");
						break;
					case 2:
						sprintf(msg+strlen(msg), "II");
						break;
					case 3:
						sprintf(msg+strlen(msg), "III");
						break;
					case 4:
						sprintf(msg+strlen(msg), "IV");
						break;
					case 5:
						sprintf(msg+strlen(msg), "V");
						break;
					}
				if(cyberspace_personal_data.systems[sys]==0) glColor4f(0.3, 0.3, 0.3, 1.0);
				else {
					if(cyberspace_personal_data.systems_loaded[sys]==0) glColor4f(0.0, 0.5, 0.0, 1.0);
					else glColor4f(0.2, 1.0, 0.2, 1.0);
					}
				cyberspace_GUI_font_print(10, y_tmp, 100, msg);
				y_tmp+=20;
				}
			glColor4f(0.0, 0.8, 0.0, 1.0);
			y_tmp+=10;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [A] per caricare nel connettore un processo cognitivo artificiale");
			y_tmp+=20;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [B] per cercare di programmare un nuovo processo cognitivo artificiale");
			y_tmp+=20;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [SPACE] per allontanarti dalla console di programmazione cognitiva");
			break;
		case CYBERSPACE_PHASE_PROGRAMMING_SUBSYS:
			cyberspace_GUI_font_print(10, y_tmp, 100, "programmazione cognitiva - processi cognitivi artificiali");
			y_tmp+=20;
			if(cyberspace_server_message_fade>0 && strlen(cyberspace_server_message)>0) {
				glColor4f(1.0, 0.0, 0.0, 1.0);
				cyberspace_GUI_font_print((cyberspace_screen_width-(strlen(cyberspace_server_message)*10))/2, 20, 100, cyberspace_server_message);
				glColor4f(0.0, 0.8, 0.0, 1.0);
				}

			cyberspace_GUI_font_print(10, y_tmp, 100, "Seleziona con le frecce un processo cognitivo da caricare.");
			y_tmp+=20+20;

			sonia

			totalsubsyscount=0;
			for(int sys=0;sys<CYBERSPACE_NEURAL_SYSTEM_MAX;sys++) {
				subsyscount=0;
				for(int subsys=0;subsys<CYBERSPACE_NEURAL_SUBSYSTEM_MAX;subsys++) {
					if(cyberspace_personal_data.subsystems[sys][subsys]>0) subsyscount++;
					}
				totalsubsyscount+=subsyscount;
				if(subsyscount==0) continue;
				switch(sys) {
					case CYBERSPACE_NEURAL_SYSTEM_MOVEMENT:
						sprintf(msg, "MOVIMENTO");
						break;
					case CYBERSPACE_NEURAL_SYSTEM_SCAN:
						sprintf(msg, "SCANSIONE");
						break;
					case CYBERSPACE_NEURAL_SYSTEM_OBFUSCATION:
						sprintf(msg, "OFFUSCAMENTO");
						break;
					case CYBERSPACE_NEURAL_SYSTEM_ATTACK:
						sprintf(msg, "ATTACCO");
						break;
					case CYBERSPACE_NEURAL_SYSTEM_DEFENCE:
						sprintf(msg, "DIFESA");
						break;
					default:
						fprintf(stderr, "FATAL ERROR: unknown system %d\n", sys);
						exit(EXIT_FAILURE);
						break;
					}
				if(cyberspace_personal_data.systems[sys]==0) {
					sprintf(msg+strlen(msg), " - sistema NON DISPONIBILE");
					glColor4f(0.3, 0.3, 0.3, 1.0);
					}
				else {
					if(cyberspace_personal_data.systems_loaded[sys]==0) {
						sprintf(msg+strlen(msg), " - sistema NON CARICATO");
						glColor4f(0.0, 0.5, 0.0, 1.0);
						}
					else {
						sprintf(msg+strlen(msg), " - il sistema puo' controllare fino a %d processi cognitivi artificiali", cyberspace_personal_data.systems_loaded[sys]);
						glColor4f(0.2, 1.0, 0.2, 1.0);
						}
					}
				cyberspace_GUI_font_print(10, y_tmp, 100, msg);
				y_tmp+=20;
				for(int subsys=0;subsys<CYBERSPACE_NEURAL_SUBSYSTEM_MAX;subsys++) {
					if(cyberspace_personal_data.subsystems[sys][subsys]==0) continue;
					if(cyberspace_personal_data.subsystems_loaded[sys][subsys]==0) glColor4f(0.0, 0.5, 0.0, 1.0);
					else glColor4f(0.0, 0.5, 0.0, 1.0);
					cyberspace_skills_getname(sys, subsys, cyberspace_personal_data.subsystems_hint[sys][subsys], msg_skillname, false);
					sprintf(msg, "%s", msg_skillname);
					cyberspace_GUI_font_print(10+20, y_tmp, 100, msg);
					y_tmp+=20;
					}
				}

			if(totalsubsyscount==0) {
				cyberspace_GUI_font_print(10, y_tmp, 100, "Non ti sei ancora programmato alcun processo cognitivo artificiale,\npremi [B] per cercare di programmarne uno.");
				y_tmp+=35+20;
				}

			glColor4f(0.0, 0.8, 0.0, 1.0);
			y_tmp+=10;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [SU] e [GIU] per cambiare la selezione del processo congnitivo");
			y_tmp+=20;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [DESTRA] per caricare il processo congnitivo selezionato");
			y_tmp+=20;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [A] per caricare nel connettore un sistema");
			y_tmp+=20;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [B] per cercare di programmare un nuovo processo cognitivo artificiale");
			y_tmp+=20;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [SPACE] per allontanarti dalla console di programmazione cognitiva");
			break;
		case CYBERSPACE_PHASE_PROGRAMMING_DEVELOP:
			cyberspace_GUI_font_print(10, y_tmp, 100, "programmazione cognitiva - programmazione");
			y_tmp+=20;
			if(cyberspace_server_message_fade>0 && strlen(cyberspace_server_message)>0) {
				glColor4f(1.0, 0.0, 0.0, 1.0);
				cyberspace_GUI_font_print((cyberspace_screen_width-(strlen(cyberspace_server_message)*10))/2, 20, 100, cyberspace_server_message);
				glColor4f(0.0, 0.8, 0.0, 1.0);
				}

//			cyberspace_GUI_font_print(10, y_tmp, 100, "Il programma semi-sciente ideato dal professor Felicioli\nche gestiva il server cyberspazio del circolo e' ancora\nin funzione ed interferisce con la console impedendoti\ndi programmare nuovi processi cognitivi.");
//			y_tmp+=80;

			sprintf(msg, "record HEURISTIC        : %15d  (premi [0] per esercitarti)\nrecord FAST ALGORITHMS  : %15d  (premi [1] per esercitarti)\nrecord PATTERN MATCHING : %15d  (premi [2] per esercitarti)", cyberspace_personal_data.heuristic_record, cyberspace_personal_data.fast_record, cyberspace_personal_data.patternmatching_record);
			cyberspace_GUI_font_print(10, y_tmp, 100, msg);
			y_tmp+=50;

			cyberspace_GUI_font_print(10, y_tmp, 100, "Seleziona con le frecce un processo cognitivo da programmare.");
			y_tmp+=40;

			position=0;
			for(int sys=0;sys<CYBERSPACE_NEURAL_SYSTEM_MAX;sys++) {
				for(int subsys=0;subsys<CYBERSPACE_NEURAL_SUBSYSTEM_MAX;subsys++) {
					if(cyberspace_personal_data.subsystems_hint[sys][subsys]>cyberspace_personal_data.subsystems[sys][subsys]) {
						if(position++==cyberspace_programming_position) glColor4f(0.2, 1.0, 0.2, 1.0);
						else glColor4f(0.0, 0.5, 0.0, 1.0);
						cyberspace_skills_getname(sys, subsys, cyberspace_personal_data.subsystems_hint[sys][subsys], msg_skillname, true);
						sprintf(msg, "%d - %s", position, msg_skillname);
						cyberspace_GUI_font_print(10, y_tmp, 100, msg);
						y_tmp+=20;
						}
					}
				}
			if(position==0) {
				cyberspace_GUI_font_print(10, y_tmp, 100, "Non hai idea di come poter programmare ulteriori processi cognitivi artificiali.");
				y_tmp+=20;
				}

			glColor4f(0.0, 0.8, 0.0, 1.0);
			y_tmp+=20;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [SU] e [GIU] per cambiare la selezione del processo congnitivo");
			y_tmp+=20;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [DESTRA] per programmare il processo congnitivo selezionato");
			y_tmp+=20;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [A] per caricare nel connettore un sistema");
			y_tmp+=20;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [B] per caricare nel connettore un processo cognitivo artificiale");
			y_tmp+=20;
			cyberspace_GUI_font_print(10, y_tmp, 100, "premi [SPACE] per allontanarti dalla console di programmazione cognitiva");
			break;
		case CYBERSPACE_PHASE_PROGRAMMING_STEPS:
			cyberspace_skills_getname(cyberspace_programming.mini_cause_progr_sys, cyberspace_programming.mini_cause_progr_subsys, cyberspace_personal_data.subsystems_hint[cyberspace_programming.mini_cause_progr_sys][cyberspace_programming.mini_cause_progr_subsys], msg_skillname, false);
			cyberspace_GUI_font_print(10, y_tmp, 100, "programmazione cognitiva - programmazione");
			y_tmp+=20;
			if(cyberspace_server_message_fade>0 && strlen(cyberspace_server_message)>0) {
				glColor4f(1.0, 0.0, 0.0, 1.0);
				cyberspace_GUI_font_print((cyberspace_screen_width-(strlen(cyberspace_server_message)*10))/2, 20, 100, cyberspace_server_message);
				glColor4f(0.0, 0.8, 0.0, 1.0);
				}

			cyberspace_GUI_font_print(10, y_tmp, 100, "Per sviluppare il processo cognitivo artificiale devi\nprogrammare tutti i moduli richiesti sotto elencati.\nOgni modulo ha un requisito di punteggio minimo, puoi\nseguire l'ordine che preferisci ma se fallisci la programmazione\nanche di un solo modulo quelli gia' programmati verranno scartati.");
			y_tmp+=100;

			glColor4f(0.2, 1.0, 0.2, 1.0);
			sprintf(msg, "%s", msg_skillname);
			cyberspace_GUI_font_print(10, y_tmp, 100, msg);
			y_tmp+=20;

			glColor4f(0.0, 0.8, 0.0, 1.0);
			y_tmp+=20;

			programmed_failed=false;
			programmed=true;

			if(cyberspace_programming.mini_req_heuristic>0) {
				if(cyberspace_programming.mini_res_heuristic>=0) {
					if(cyberspace_programming.mini_res_heuristic>=cyberspace_programming.mini_req_heuristic) glColor4f(1.0, 1.0, 0.0, 1.0);
					else {
						programmed_failed=true;
						glColor4f(1.0, 0.2, 0.2, 1.0);
						}
					sprintf(msg, "modulo di euristica : minimo %d - punteggio ottenuto %d", cyberspace_programming.mini_req_heuristic, cyberspace_programming.mini_res_heuristic);
					}
				else {
					programmed=false;
					glColor4f(0.0, 0.8, 0.0, 1.0);
					sprintf(msg, "modulo di euristica : minimo %d - premi [0] per programmarlo", cyberspace_programming.mini_req_heuristic);
					}
				cyberspace_GUI_font_print(10, y_tmp, 100, msg);
				}
			y_tmp+=30;

			if(cyberspace_programming.mini_req_fast>0) {
				if(cyberspace_programming.mini_res_fast>=0) {
					if(cyberspace_programming.mini_res_fast>=cyberspace_programming.mini_req_fast) glColor4f(1.0, 1.0, 0.0, 1.0);
					else {
						programmed_failed=true;
						glColor4f(1.0, 0.2, 0.2, 1.0);
						}
					sprintf(msg, "modulo di algoritmi rapidi : minimo %d - punteggio ottenuto %d", cyberspace_programming.mini_req_fast, cyberspace_programming.mini_res_fast);
					}
				else {
					programmed=false;
					glColor4f(0.0, 0.8, 0.0, 1.0);
					sprintf(msg, "modulo di algoritmi rapidi : minimo %d - premi [1] per programmarlo", cyberspace_programming.mini_req_fast);
					}
				cyberspace_GUI_font_print(10, y_tmp, 100, msg);
				}
			y_tmp+=30;

			if(cyberspace_programming.mini_req_patternmatching>0) {
				if(cyberspace_programming.mini_res_patternmatching>=0) {
					if(cyberspace_programming.mini_res_patternmatching>=cyberspace_programming.mini_req_patternmatching) glColor4f(1.0, 1.0, 0.0, 1.0);
					else {
						programmed_failed=true;
						glColor4f(1.0, 0.2, 0.2, 1.0);
						}
					sprintf(msg, "modulo di riconoscimento di schemi : minimo %d - punteggio ottenuto %d", cyberspace_programming.mini_req_patternmatching, cyberspace_programming.mini_res_patternmatching);
					}
				else {
					programmed=false;
					glColor4f(0.0, 0.8, 0.0, 1.0);
					sprintf(msg, "modulo di riconoscimento di schemi : minimo %d - premi [2] per programmarlo", cyberspace_programming.mini_req_patternmatching);
					}
				cyberspace_GUI_font_print(10, y_tmp, 100, msg);
				}
			y_tmp+=30;

			if(programmed_failed) {
				glColor4f(1.0, 0.2, 0.2, 1.0);
				cyberspace_GUI_font_print(10, y_tmp, 100, "hai fallito nella programmazione!!");
				y_tmp+=30;
				glColor4f(0.0, 0.8, 0.0, 1.0);
				cyberspace_GUI_font_print(10, y_tmp, 100, "premi [SPACE] per continuare");
				}
			else {
				if(programmed) {
					glColor4f(1.0, 1.0, 0.0, 1.0);
					cyberspace_GUI_font_print(10, y_tmp, 100, "sei riuscito nella programmazione!!");
					y_tmp+=30;
					glColor4f(0.0, 0.8, 0.0, 1.0);
					cyberspace_GUI_font_print(10, y_tmp, 100, "premi [SPACE] per continuare");
					}
				else {
					glColor4f(0.0, 0.8, 0.0, 1.0);
					cyberspace_GUI_font_print(10, y_tmp, 100, "premi [SPACE] per abbandonare la programmazione del processo cognitivo artificiale selezionato");
					}
				}
			break;
		default:
			fprintf(stderr, "FATAL ERROR: unknown cyberspace_programming_subphase %d\n", cyberspace_programming_subphase);
			exit(EXIT_FAILURE);
			break;
		}

	if(!CYBERSPACE_GUI_WIREFRAMES) {
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		}
	glEnable(GL_DEPTH_TEST);

	glFlush();
	SDL_GL_SwapBuffers();

	delete[] msg;
	delete[] msg_skillname;
	}

/*
	GLuint* textures=new GLuint[5];
	SDL_Surface* origin;
	SDL_Surface* appoggio;
	SDL_Rect area;
	SDL_Rect area2;
	delete[] textures;



			glGenTextures(5, textures);
			glBindTexture(GL_TEXTURE_2D, textures[0]);

			origin=IMG_Load("img/icons.png");
			if(origin==NULL) {
				fprintf(stderr, "FATAL ERROR: failed to load icons file.\n");
				exit(EXIT_FAILURE);
				}

			appoggio=SDL_CreateRGBSurface(SDL_SWSURFACE, 64, 64, 32,
//			appoggio=SDL_CreateRGBSurface(SDL_SWSURFACE, 64, 64, 32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN // OpenGL RGBA masks 
                               0x000000FF, 
                               0x0000FF00, 
                               0x00FF0000, 
                               0xFF000000
#else
                               0xFF000000,
                               0x00FF0000, 
                               0x0000FF00, 
                               0x000000FF
#endif
  ) ;
			if(appoggio==0) {
				fprintf(stderr, "FATAL ERROR: fallito appoggio\n");
				exit(EXIT_FAILURE);
				}

//  colorkey = SDL_MapRGBA( alphaImage->format, colorKeyRed, colorKeyGreen, colorKeyBlue, 0 ) ;
 // SDL_FillRect( alphaImage, 0, colorkey ) ;
//
//  colorkey = SDL_MapRGBA( sourceSurface->format, colorKeyRed, colorKeyGreen, colorKeyBlue, 0 ) ;
  //SDL_SetColorKey( sourceSurface, SDL_SRCCOLORKEY, colorkey ) ;

 
  // Copy the surface into the GL texture image : 
			area2.x=0;
			area2.y=0;

			area.x=0;
			area.y=0;
			area.w=64;
			area.h=64;
			SDL_BlitSurface(origin, &area, appoggio, &area2);

//			area.x=32;
//			area.y=0;
//			area.w=32;
//			area.h=64;
//			SDL_BlitSurface(origin, &area, appoggio, &area2);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
//			glPixelStorei(GL_UNPACK_ROW_LENGTH, appoggio->pitch);
//			glPixelStorei(GL_UNPACK_ROW_LENGTH, origin->w);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, appoggio->w);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, appoggio->pixels);

!!!			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_BGRA, GL_UNSIGNED_BYTE, appoggio->pixels);


//			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, origin->pixels);
			SDL_FreeSurface(appoggio);

			SDL_FreeSurface(origin);

			glEnable(GL_TEXTURE_2D);
			glColor4f(1.0, 1.0, 1.0, 1.0);
			glBegin(GL_QUADS); 
				glTexCoord2f(0.0, 0.0); glVertex2i(50, 50);
				glTexCoord2f(0.0, 1.0); glVertex2i(200, 50); 
				glTexCoord2f(1.0, 1.0); glVertex2i(200, 200); 
				glTexCoord2f(1.0, 0.0);	glVertex2i(50, 200); 

				glTexCoord2f(0.0, 1.0); glVertex2i(250, 200); 
				glTexCoord2f(1.0, 1.0); glVertex2i(400, 200); 
				glTexCoord2f(1.0, 0.0); glVertex2i(400, 50); 
				glTexCoord2f(0.0, 0.0); glVertex2i(250, 50);
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glDeleteTextures(5, textures);
*/
