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


#include "cyberspace_skills.h"


void cyberspace_skills_getname(int sys, int subsys, int lv, char* s, bool append_programming_req) {
	switch(sys) {
		case CYBERSPACE_NEURAL_SYSTEM_MOVEMENT:
			switch(subsys) {
				case CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_SPEED:
					sprintf(s, "speed");
					break;
				case CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_JUMP:
					sprintf(s, "jump");
					break;
				default:
					fprintf(stderr, "ERROR: unknown subsys (%d - %d).\n", sys, subsys);
					exit(EXIT_FAILURE);
				}
			break;
		case CYBERSPACE_NEURAL_SYSTEM_SCAN:
			switch(subsys) {
				case CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_MAP:
					sprintf(s, "automap");
					break;
				case CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_SCAN:
					sprintf(s, "deeph scan");
					break;
				case CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_COOR:
					sprintf(s, "coordinate");
					break;
				default:
					fprintf(stderr, "ERROR: unknown subsys (%d - %d).\n", sys, subsys);
					exit(EXIT_FAILURE);
				}
			break;
		case CYBERSPACE_NEURAL_SYSTEM_ATTACK:
			switch(subsys) {
				case CYBERSPACE_NEURAL_SUBSYSTEM_ATTACK_TOUCH:
					sprintf(s, "crash touch");
					break;
				default:
					fprintf(stderr, "ERROR: unknown subsys (%d - %d).\n", sys, subsys);
					exit(EXIT_FAILURE);
				}
		case CYBERSPACE_NEURAL_SYSTEM_DEFENCE:
			switch(subsys) {
				case CYBERSPACE_NEURAL_SUBSYSTEM_DEFENCE_TOUCH:
					sprintf(s, "crash touch armour");
					break;
				default:
					fprintf(stderr, "ERROR: unknown subsys (%d - %d).\n", sys, subsys);
					exit(EXIT_FAILURE);
				}
			break;
		default:
			fprintf(stderr, "ERROR: unknown sys (%d - %d).\n", sys, subsys);
			exit(EXIT_FAILURE);
		}
	switch(lv) {
		case 1:
			sprintf(s+strlen(s), " I");
			break;
		case 2:
			sprintf(s+strlen(s), " II");
			break;
		case 3:
			sprintf(s+strlen(s), " III");
			break;
		case 4:
			sprintf(s+strlen(s), " IV");
			break;
		case 5:
			sprintf(s+strlen(s), " V");
			break;
		}
	if(!append_programming_req) return;
	int heuristic, fast, patternmatching;
	cyberspace_skills_getreq(sys, subsys, lv, &heuristic, &fast, &patternmatching);
	bool needspace=false;
	sprintf(s+strlen(s), " (");
	if(heuristic!=0) {
		if(needspace) sprintf(s+strlen(s), ", ");
		else needspace=true;
		sprintf(s+strlen(s), "heuristics: %d", heuristic);
		}
	if(fast!=0) {
		if(needspace) sprintf(s+strlen(s), ", ");
		else needspace=true;
		sprintf(s+strlen(s), "fast algorithms: %d", fast);
		}
	if(patternmatching!=0) {
		if(needspace) sprintf(s+strlen(s), ", ");
		else needspace=true;
		sprintf(s+strlen(s), "pattern matching: %d", patternmatching);
		}
	sprintf(s+strlen(s), ")");
	}

void cyberspace_skills_getreq(int sys, int subsys, int lv, int* heuristic, int* fast, int* patternmatching) {
	*heuristic=-1;
	*fast=-1;
	*patternmatching=-1;
	switch(sys) {
		case CYBERSPACE_NEURAL_SYSTEM_MOVEMENT:
			switch(subsys) {
				case CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_SPEED:
					if(lv==1) {
						*heuristic=CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_SPEED_I_HEUR;
						*fast=CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_SPEED_I_FAST;
						*patternmatching=CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_SPEED_I_PATTERN;
						}
					break;
				case CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_JUMP:
					if(lv==1) {
						*heuristic=CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_JUMP_I_HEUR;
						*fast=CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_JUMP_I_FAST;
						*patternmatching=CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_JUMP_I_PATTERN;
						}
					break;
				default:
					fprintf(stderr, "ERROR: unknown subsys (%d - %d).\n", sys, subsys);
					exit(EXIT_FAILURE);
				}
			break;
		case CYBERSPACE_NEURAL_SYSTEM_SCAN:
			switch(subsys) {
				case CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_MAP:
					if(lv==1) {
						*heuristic=CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_MAP_I_HEUR;
						*fast=CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_MAP_I_FAST;
						*patternmatching=CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_MAP_I_PATTERN;
						}
					break;
				case CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_SCAN:
					if(lv==1) {
						*heuristic=CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_SCAN_I_HEUR;
						*fast=CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_SCAN_I_FAST;
						*patternmatching=CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_SCAN_I_PATTERN;
						}
					break;
				case CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_COOR:
					if(lv==1) {
						*heuristic=CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_COOR_I_HEUR;
						*fast=CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_COOR_I_FAST;
						*patternmatching=CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_COOR_I_PATTERN;
						}
					break;
				default:
					fprintf(stderr, "ERROR: unknown subsys (%d - %d).\n", sys, subsys);
					exit(EXIT_FAILURE);
				}
			break;
		case CYBERSPACE_NEURAL_SYSTEM_ATTACK:
			switch(subsys) {
				case CYBERSPACE_NEURAL_SUBSYSTEM_ATTACK_TOUCH:
					if(lv==1) {
						*heuristic=CYBERSPACE_NEURAL_SUBSYSTEM_ATTACK_TOUCH_I_HEUR;
						*fast=CYBERSPACE_NEURAL_SUBSYSTEM_ATTACK_TOUCH_I_FAST;
						*patternmatching=CYBERSPACE_NEURAL_SUBSYSTEM_ATTACK_TOUCH_I_PATTERN;
						}
					break;
				default:
					fprintf(stderr, "ERROR: unknown subsys (%d - %d).\n", sys, subsys);
					exit(EXIT_FAILURE);
				}
			break;
		case CYBERSPACE_NEURAL_SYSTEM_DEFENCE:
			switch(subsys) {
				case CYBERSPACE_NEURAL_SUBSYSTEM_DEFENCE_TOUCH:
					if(lv==1) {
						*heuristic=CYBERSPACE_NEURAL_SUBSYSTEM_DEFENCE_TOUCH_I_HEUR;
						*fast=CYBERSPACE_NEURAL_SUBSYSTEM_DEFENCE_TOUCH_I_FAST;
						*patternmatching=CYBERSPACE_NEURAL_SUBSYSTEM_DEFENCE_TOUCH_I_PATTERN;
						}
					break;
				default:
					fprintf(stderr, "ERROR: unknown subsys (%d - %d).\n", sys, subsys);
					exit(EXIT_FAILURE);
				}
			break;
		default:
			fprintf(stderr, "ERROR: unknown sys (%d - %d).\n", sys, subsys);
			exit(EXIT_FAILURE);
		}
	}

