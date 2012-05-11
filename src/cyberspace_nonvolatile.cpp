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


#include "cyberspace_nonvolatile.h"

void cyberspace_nonvolatile_save_server(cyberspace_user_connection_data* data) {
	char* path;
	char* newpath;
	FILE* file;
	int filed;

	path=new char[100];
	newpath=new char[100];

	sprintf(path, "server_data.old");
	if((file=fopen(path, "r+"))==NULL) {
		perror("perror:\n");
		fprintf(stderr, "FATAL ERROR: unable to fopen file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if(fwrite(&cyberspace_server_data, sizeof(cyberspace_server_data), 1, file)!=1) {
		fprintf(stderr, "FATAL ERROR: unable to fwrite file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if((filed=fileno(file))==-1) {
		fprintf(stderr, "FATAL ERROR: unable to fileno file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if(fdatasync(filed)!=0) {
		fprintf(stderr, "FATAL ERROR: unable to fdatasync file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if(fclose(file)!=0) {
		fprintf(stderr, "FATAL ERROR: unable to fclose file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}

	sprintf(path, "server_data.old");
	sprintf(newpath, "server_data.new");
	if(rename(path, newpath)!=0) {
		fprintf(stderr, "FATAL ERROR: failed to rename \"%s\" to \"%s\"", path, newpath);
		exit(EXIT_FAILURE);
		}

	sprintf(path, "server_data.now");
	sprintf(newpath, "server_data.old");
	if(rename(path, newpath)!=0) {
		fprintf(stderr, "FATAL ERROR: failed to rename \"%s\" to \"%s\"", path, newpath);
		exit(EXIT_FAILURE);
		}

	sprintf(path, "server_data.new");
	sprintf(newpath, "server_data.now");
	if(rename(path, newpath)!=0) {
		fprintf(stderr, "FATAL ERROR: failed to rename \"%s\" to \"%s\"", path, newpath);
		exit(EXIT_FAILURE);
		}

	delete[] path;
	delete[] newpath;
	}

void cyberspace_nonvolatile_save_personal(cyberspace_user_connection_data* data) {
	char* path;
	char* newpath;
	FILE* file;
	int filed;

	path=new char[100];
	newpath=new char[100];

	sprintf(path, "accounts/%s.old", data->personal.name);
	if((file=fopen(path, "r+"))==NULL) {
		perror("perror:\n");
		fprintf(stderr, "FATAL ERROR: unable to fopen file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if(fwrite(&(data->personal), sizeof(data->personal), 1, file)!=1) {
		fprintf(stderr, "FATAL ERROR: unable to fwrite file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if((filed=fileno(file))==-1) {
		fprintf(stderr, "FATAL ERROR: unable to fileno file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if(fdatasync(filed)!=0) {
		fprintf(stderr, "FATAL ERROR: unable to fdatasync file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}
	if(fclose(file)!=0) {
		fprintf(stderr, "FATAL ERROR: unable to fclose file \"%s\"\n", path);
		exit(EXIT_FAILURE);
		}

	sprintf(path, "accounts/%s.old", data->personal.name);
	sprintf(newpath, "accounts/%s.new", data->personal.name);
	if(rename(path, newpath)!=0) {
		fprintf(stderr, "FATAL ERROR: failed to rename \"%s\" to \"%s\"", path, newpath);
		exit(EXIT_FAILURE);
		}

	sprintf(path, "accounts/%s.now", data->personal.name);
	sprintf(newpath, "accounts/%s.old", data->personal.name);
	if(rename(path, newpath)!=0) {
		fprintf(stderr, "FATAL ERROR: failed to rename \"%s\" to \"%s\"", path, newpath);
		exit(EXIT_FAILURE);
		}

	sprintf(path, "accounts/%s.new", data->personal.name);
	sprintf(newpath, "accounts/%s.now", data->personal.name);
	if(rename(path, newpath)!=0) {
		fprintf(stderr, "FATAL ERROR: failed to rename \"%s\" to \"%s\"", path, newpath);
		exit(EXIT_FAILURE);
		}

	delete[] path;
	delete[] newpath;
	}

