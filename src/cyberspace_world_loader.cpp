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


#include "cyberspace_world_loader.h"

int cyberspace_world_load_wall_type;

void cyberspace_world_load_wall(cyberspace_world_world* world, char* a) {
	cyberspace_world_point p0;
	cyberspace_world_point p1;
	int h_parameter;
	double h;
	double L;
	double alpha_asin;
	double alpha_acos;
	double alpha;

	p0.y=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;
	p1.y=p0.y;
	h_parameter=atoi(strtok_r(NULL, " ", &a));
	p0.x=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;
	p0.z=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;
	p1.x=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;
	p1.z=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;
	L=sqrt((p0.x-p1.x)*(p0.x-p1.x)+(p0.z-p1.z)*(p0.z-p1.z));
	alpha_asin=-1.0*asin((p1.z-p0.z)/L);
	alpha_acos=acos((p1.x-p0.x)/L);
	if(alpha_acos<=M_PI/2.0) {
		if(alpha_asin>=0.0) alpha=alpha_asin;
		else alpha=(M_PI*2.0)+alpha_asin;
		}
	else {
		if(alpha_asin>=0.0) alpha=alpha_acos;
		else alpha=(M_PI*2.0)-alpha_acos;
		}

	/*switch(h_parameter) {
		case 5:
			h=50.0;
			break;
		case 40:
			h=400.0;
			break;
		default:
			fprintf(stderr, "FATAL ERROR: unexpected h_parameter: %d\n", h_parameter);
			exit(EXIT_FAILURE);
			break;
		}*/
	h=h_parameter*10.0;

	world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_WALL;
	world->static_obj[world->static_size].point.position.x=(p0.x+p1.x)/2.0;
	world->static_obj[world->static_size].point.position.y=(p0.y+p1.y)/2.0;
	world->static_obj[world->static_size].point.position.z=(p0.z+p1.z)/2.0;
	world->static_obj[world->static_size].point.angle.alpha=alpha;
	world->static_obj[world->static_size].point.angle.beta=0.0;
	world->static_obj[world->static_size].point.angle.gamma=0.0;
	world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_HEIGHT]=h;
	world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_LENGTH]=L;
	world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE]=cyberspace_world_load_wall_type;
//	printf("wall (%f - %f) - (%f - %f) center (%f - %f) L=%f alpha=%f (asin:%f acos:%f)\n", p0.x, p0.z, p1.x, p1.z, world->static_obj[world->static_size].point.position.x, world->static_obj[world->static_size].point.position.z, L, alpha, alpha_asin, alpha_acos);
	world->static_size++;

	world->red_line[world->red_line_size].p0=p0;
	world->red_line[world->red_line_size].p1=p1;
	world->red_line[world->red_line_size].p1.y+=h;
	world->red_line_size++;
	}

void cyberspace_world_load_floor(cyberspace_world_world* world, char* a) {
	cyberspace_world_point p0;
	cyberspace_world_point p1;
	cyberspace_world_point p2;
	double h;

	h=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;
	p0.y=h;
	p1.y=h;
	p2.y=h;
	p0.x=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;
	p0.z=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;
	p1.x=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;
	p1.z=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;
	p2.x=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;
	p2.z=atof(strtok_r(NULL, " ", &a))*CYBERSPACE_WORLD_MAP_SCALE;

	world->static_obj[world->static_size].type=CYBERSPACE_WORLD_OBJECT_FLOOR;
	world->static_obj[world->static_size].point.position.x=(p0.x+p1.x+p2.x)/3.0;;
	world->static_obj[world->static_size].point.position.y=(p0.y+p1.y+p2.y)/3.0;;
	world->static_obj[world->static_size].point.position.z=(p0.z+p1.z+p2.z)/3.0;;
	world->static_obj[world->static_size].point.angle.alpha=0.0;
	world->static_obj[world->static_size].point.angle.beta=0.0;
	world->static_obj[world->static_size].point.angle.gamma=0.0;
	world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_X0]=p0.x-world->static_obj[world->static_size].point.position.x;
	world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_Z0]=p0.z-world->static_obj[world->static_size].point.position.z;
	world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_X1]=p1.x-world->static_obj[world->static_size].point.position.x;
	world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_Z1]=p1.z-world->static_obj[world->static_size].point.position.z;
	world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_X2]=p2.x-world->static_obj[world->static_size].point.position.x;
	world->static_obj[world->static_size].parameters[CYBERSPACE_WORLD_OBJECT_FLOOR_PARAMETER_Z2]=p2.z-world->static_obj[world->static_size].point.position.z;
	world->static_size++;

	world->floor[world->floor_size].p0=p0;
	world->floor[world->floor_size].p1=p1;
	world->floor[world->floor_size].p2=p2;
	world->floor_size++;
	}

void cyberspace_world_loader(FILE* world_file, cyberspace_world_world* world) {
	world->static_size=0;
	world->dynamic_size=0;
	world->red_line_size=0;
	world->floor_size=0;
	char *line=new char[10000];
	char* token;
	char* a;
	int type;
	int line_n=0;
	cyberspace_world_load_wall_type=CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_STEEL;
	while(!feof(world_file)) {
		line_n++;
		fgets(line, 9999, world_file);
		line[9999]='\0';
		if(strlen(line)<=1) continue;
		if(line[0]=='#') continue;
		line[strlen(line)-1]='\0';
		printf("read \"%s\"\n", line);
		token=strtok_r(line, " ", &a);
		type=-1;
		if(type==-1 && strcmp(token, "wall")==0) type=CYBERSPACE_WORLD_OBJECT_WALL;
		if(type==-1 && strcmp(token, "floor")==0) type=CYBERSPACE_WORLD_OBJECT_FLOOR;
		if(type==-1 && strcmp(token, "walltype")==0) {
			token=strtok_r(NULL, " ", &a);
			if(strcmp(token, "steel")==0) cyberspace_world_load_wall_type=CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_STEEL;
			if(strcmp(token, "glass_steel")==0) cyberspace_world_load_wall_type=CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_GLASS_STEEL;
			if(strcmp(token, "wire")==0) cyberspace_world_load_wall_type=CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_WIRE;
			if(strcmp(token, "forcefield")==0) cyberspace_world_load_wall_type=CYBERSPACE_WORLD_OBJECT_WALL_PARAMETER_SUBTYPE_FORCEFIELD;
			type=CYBERSPACE_WORLD_OBJECT_NONE;
			}
		switch(type) {
			case CYBERSPACE_WORLD_OBJECT_NONE:
				break;
			case CYBERSPACE_WORLD_OBJECT_WALL:
				cyberspace_world_load_wall(world, a);
				break;
			case CYBERSPACE_WORLD_OBJECT_FLOOR:
				cyberspace_world_load_floor(world, a);
				break;
			default:
				fprintf(stderr, "FATAL ERROR: parsing error in world loader line %d\n", line_n);
				exit(EXIT_FAILURE);
				break;
			}
		}
	delete[] line;
	}

