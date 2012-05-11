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


#include "cyberspace_programming.h"


void cyberspace_programming_message2server(cyberspace_user_connection_data* data) {
	if(data->phase!=CYBERSPACE_PHASE_PROGRAMMING) {
		fprintf(stderr, "ERROR: data->phase!=CYBERSPACE_PHASE_PROGRAMMING (%d).\n", data->phase);
		exit(EXIT_FAILURE);
		}

	char* errormessage;
	int sys;
	int subsys;
	int installed_sys;
	bool programmed_failed;
	switch(data->in_command.type) {
		case CYBERSPACE_COMMAND_USER_PROGRAMMING_STOP:
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_3D;
			cyberspace_user_connection_output(data, &(data->out_command));
			data->phase=CYBERSPACE_PHASE_3D;
			break;
		case CYBERSPACE_COMMAND_USER_PROGRAMMING_SYSTEM_LOAD:
			memcpy(&sys, data->in_command.data, sizeof(int));
			SDL_mutexP(data->disk_mutex);

			if(data->personal.systems[sys]==0) {
				SDL_mutexV(data->disk_mutex);
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "non possiedi il sistema selezionato");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}

			installed_sys=0;
			for(int s=0;s<CYBERSPACE_NEURAL_SYSTEM_MAX;s++) if(data->personal.systems_loaded[s]!=0) installed_sys++;
			if(installed_sys+1>data->personal.systems_max) {
				SDL_mutexV(data->disk_mutex);
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "hai gia' %d sistemi installati", installed_sys);
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}

			data->personal.systems_loaded[sys]=data->personal.systems[sys];

			cyberspace_nonvolatile_save_personal(data);
			SDL_mutexV(data->disk_mutex);
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_USER;
			memcpy(data->out_command.data, &(data->personal), sizeof(data->personal));
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		case CYBERSPACE_COMMAND_USER_PROGRAMMING_SYSTEM_UNLOAD:
			memcpy(&sys, data->in_command.data, sizeof(int));
			SDL_mutexP(data->disk_mutex);

			if(data->personal.systems_loaded[sys]==0) {
				SDL_mutexV(data->disk_mutex);
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "il sistema selezionato non e' caricato");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}

			data->personal.systems_loaded[sys]=0;

			cyberspace_nonvolatile_save_personal(data);
			SDL_mutexV(data->disk_mutex);
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_USER;
			memcpy(data->out_command.data, &(data->personal), sizeof(data->personal));
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		case CYBERSPACE_COMMAND_USER_PROGRAMMING_CODE:
			memcpy(&sys, data->in_command.data, sizeof(int));
			memcpy(&subsys, data->in_command.data+sizeof(int), sizeof(int));

			printf("sys: %d - subsys: %d\n", sys, subsys);

			if((sys<0 || sys>=CYBERSPACE_NEURAL_SYSTEM_MAX) || (subsys<0 || subsys>=CYBERSPACE_NEURAL_SUBSYSTEM_MAX)) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "subsystem (%d - %d) incorretto", sys, subsys);
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}

			if(data->personal.subsystems_hint[sys][subsys]==0) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "non hai idea di come programmare il subsystem (%d - %d)", sys, subsys);
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}

			if(data->personal.subsystems[sys][subsys]>=data->personal.subsystems_hint[sys][subsys]) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "subsystem (%d - %d) gia' noto", sys, subsys);
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}


			data->programming.mini_cause_progr_sys=sys;
			data->programming.mini_cause_progr_subsys=subsys;
			data->programming.mini_res_heuristic=-1;
			data->programming.mini_res_fast=-1;
			data->programming.mini_res_patternmatching=-1;
			cyberspace_skills_getreq(sys, subsys, data->personal.subsystems_hint[sys][subsys], &(data->programming.mini_req_heuristic), &(data->programming.mini_req_fast), &(data->programming.mini_req_patternmatching));

			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PROGRAMMING;
			memcpy(data->out_command.data, &(data->programming), sizeof(data->programming));
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		case CYBERSPACE_COMMAND_USER_PROGRAMMING_HEURISTIC:
			programmed_failed=false;
			if(!programmed_failed && data->programming.mini_req_heuristic>0 && data->programming.mini_res_heuristic!=-1 && data->programming.mini_req_heuristic>data->programming.mini_res_heuristic) programmed_failed=true;
			if(!programmed_failed && data->programming.mini_req_fast>0 && data->programming.mini_res_fast!=-1 && data->programming.mini_req_fast>data->programming.mini_res_fast) programmed_failed=true;
			if(!programmed_failed && data->programming.mini_req_patternmatching>0 && data->programming.mini_res_patternmatching!=-1 && data->programming.mini_req_patternmatching>data->programming.mini_res_patternmatching) programmed_failed=true;
			if(programmed_failed) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "non puoi procedere con la programmazione: hai gia' fallito");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			if(data->programming.mini_req_heuristic==0 || data->programming.mini_res_heuristic>=0) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "hai gia' programmato il modulo di euristica oppure tale modulo non e' richiesto");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			data->programming.mini_cause=CYBERSPACE_COMMAND_MINIGAME_CAUSE_PROGRAMMING;
			//init the minigame data
			cyberspace_minigame_heuristic_init(data);

			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_HEURISTIC;
			cyberspace_user_connection_output(data, &(data->out_command));
			data->phase=CYBERSPACE_PHASE_MINIGAME_HEURISTIC;
			break;
		case CYBERSPACE_COMMAND_USER_PROGRAMMING_FAST:
			programmed_failed=false;
			if(!programmed_failed && data->programming.mini_req_heuristic>0 && data->programming.mini_res_heuristic!=-1 && data->programming.mini_req_heuristic>data->programming.mini_res_heuristic) programmed_failed=true;
			if(!programmed_failed && data->programming.mini_req_fast>0 && data->programming.mini_res_fast!=-1 && data->programming.mini_req_fast>data->programming.mini_res_fast) programmed_failed=true;
			if(!programmed_failed && data->programming.mini_req_patternmatching>0 && data->programming.mini_res_patternmatching!=-1 && data->programming.mini_req_patternmatching>data->programming.mini_res_patternmatching) programmed_failed=true;
			if(programmed_failed) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "non puoi procedere con la programmazione: hai gia' fallito");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			if(data->programming.mini_req_fast==0 || data->programming.mini_res_fast>=0) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "hai gia' programmato il modulo di algoritmica veloce oppure tale modulo non e' richiesto");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			data->programming.mini_cause=CYBERSPACE_COMMAND_MINIGAME_CAUSE_PROGRAMMING;
			//init the minigame data
			cyberspace_minigame_fast_init(data);

			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_FAST;
			cyberspace_user_connection_output(data, &(data->out_command));
			data->phase=CYBERSPACE_PHASE_MINIGAME_FAST;
			break;
		case CYBERSPACE_COMMAND_USER_PROGRAMMING_PATTERNMATCHING:
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
			errormessage=new char[100];
			sprintf(errormessage, "il modulo di pattern matching non e' funzionale");
			memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
			programmed_failed=false;
			if(!programmed_failed && data->programming.mini_req_heuristic>0 && data->programming.mini_res_heuristic!=-1 && data->programming.mini_req_heuristic>data->programming.mini_res_heuristic) programmed_failed=true;
			if(!programmed_failed && data->programming.mini_req_fast>0 && data->programming.mini_res_fast!=-1 && data->programming.mini_req_fast>data->programming.mini_res_fast) programmed_failed=true;
			if(!programmed_failed && data->programming.mini_req_patternmatching>0 && data->programming.mini_res_patternmatching!=-1 && data->programming.mini_req_patternmatching>data->programming.mini_res_patternmatching) programmed_failed=true;
			if(programmed_failed) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "non puoi procedere con la programmazione: hai gia' fallito");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			if(data->programming.mini_req_patternmatching==0 || data->programming.mini_res_patternmatching>=0) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
				errormessage=new char[100];
				sprintf(errormessage, "hai gia' programmato il modulo di algoritmica veloce oppure tale modulo non e' richiesto");
				memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
				delete[] errormessage;
				cyberspace_user_connection_output(data, &(data->out_command));
				break;
				}
			data->programming.mini_cause=CYBERSPACE_COMMAND_MINIGAME_CAUSE_PROGRAMMING;
			//init the minigame data
			cyberspace_minigame_patternmatching_init(data);

			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_PATTERNMATCHING;
			cyberspace_user_connection_output(data, &(data->out_command));
			data->phase=CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING;
			break;
		case CYBERSPACE_COMMAND_USER_PROGRAMMING_TRAINING_HEURISTIC:
			data->programming.mini_cause=CYBERSPACE_COMMAND_MINIGAME_CAUSE_TRAINING_HEURISTIC;
			//init the minigame data
			cyberspace_minigame_heuristic_init(data);

			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_HEURISTIC;
			cyberspace_user_connection_output(data, &(data->out_command));
			data->phase=CYBERSPACE_PHASE_MINIGAME_HEURISTIC;
			break;
		case CYBERSPACE_COMMAND_USER_PROGRAMMING_TRAINING_FAST:
			data->programming.mini_cause=CYBERSPACE_COMMAND_MINIGAME_CAUSE_TRAINING_FAST;
			//init the minigame data
			cyberspace_minigame_fast_init(data);

			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_FAST;
			cyberspace_user_connection_output(data, &(data->out_command));
			data->phase=CYBERSPACE_PHASE_MINIGAME_FAST;
			break;
		case CYBERSPACE_COMMAND_USER_PROGRAMMING_TRAINING_PATTERNMATCHING:
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
			errormessage=new char[100];
			sprintf(errormessage, "il modulo di pattern matching non e' funzionale");
			memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
			data->programming.mini_cause=CYBERSPACE_COMMAND_MINIGAME_CAUSE_TRAINING_PATTERNMATCHING;
			//init the minigame data
			cyberspace_minigame_patternmatching_init(data);

			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_PATTERNMATCHING;
			cyberspace_user_connection_output(data, &(data->out_command));
			data->phase=CYBERSPACE_PHASE_MINIGAME_PATTERNMATCHING;
			break;
		case CYBERSPACE_COMMAND_USER_LOGOUT:
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
			errormessage=new char[100];
			sprintf(errormessage, "non puoi effetturare il logout dalla console di programmazione");
			memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		default:
			fprintf(stderr, "WARNING: unexpected message in programming (%d) by connection %d (%d).\n", data->in_command.type, data->personal.id_static, data->id_dynamic);
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
			errormessage=new char[100];
			sprintf(errormessage, "unexpected message in programming");
			memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		}

	}

