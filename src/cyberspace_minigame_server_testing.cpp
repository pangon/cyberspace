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


#include "cyberspace_minigame_server_testing.h"



void cyberspace_minigame_testing_init(cyberspace_user_connection_data* data) {
	data->mini_testing.points=0;
	data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_TESTING_POINTS;
	memcpy(data->out_command.data, &(data->mini_testing.points), sizeof(data->mini_testing.points));
	cyberspace_user_connection_output(data, &(data->out_command));

	}

void cyberspace_minigame_testing_frame(cyberspace_user_connection_data* data, int milliseconds) {
	data->mini_testing.points++;
	if(data->mini_testing.points%10==0) {
		data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_MINIGAME_TESTING_POINTS;
		memcpy(data->out_command.data, &(data->mini_testing.points), sizeof(data->mini_testing.points));
		cyberspace_user_connection_output(data, &(data->out_command));
		}
	}

void cyberspace_minigame_testing_message2server(cyberspace_user_connection_data* data) {
	if(data->phase!=CYBERSPACE_PHASE_MINIGAME_TESTING) {
		fprintf(stderr, "ERROR: data->phase!=CYBERSPACE_PHASE_MINIGAME_TESTING (%d).\n", data->phase);
		exit(EXIT_FAILURE);
		}

	char* errormessage;
	bool resend_personal=false;
	bool success;
	bool error=false;
	int lv;
	switch(data->in_command.type) {
		case CYBERSPACE_COMMAND_USER_MINIGAME_TESTING_DONE:
			switch(data->programming.mini_cause) {
				case CYBERSPACE_COMMAND_MINIGAME_CAUSE_PROGRAMMING:
					lv=data->personal.subsystems_hint[data->programming.mini_cause_progr_sys][data->programming.mini_cause_progr_subsys];
					success=false;
/*
					if(data->personal.heur_record<data->mini_testing.points) {
						data->personal.heur_record=data->mini_testing.points;
						resend_personal=true;
						}
*/
					switch(data->programming.mini_cause_progr_sys) {
						case CYBERSPACE_NEURAL_SYSTEM_MOVEMENT:
							switch(data->programming.mini_cause_progr_subsys) {
								case CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_SPEED:
									if(lv==1) success=data->mini_testing.points>=CYBERSPACE_NEURAL_SUBSYSTEM_MOVEMENT_SPEED_I_FAST;
									else error=true;
									break;
								default:
									error=true;
									break;
								}
							break;
						case CYBERSPACE_NEURAL_SYSTEM_SCAN:
							switch(data->programming.mini_cause_progr_subsys) {
								case CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_MAP:
									if(lv==1) success=data->mini_testing.points>=CYBERSPACE_NEURAL_SUBSYSTEM_SCAN_MAP_I_PATTERN;
									else error=true;
									break;
								default:
									error=true;
									break;
								}
							break;
						default:
							error=true;
							break;
						}
					if(error) {
						fprintf(stderr, "WARNING: done programming of %d - %d (lv.%d) but in mini testing by connection %d (%d).\n", data->programming.mini_cause_progr_sys, data->programming.mini_cause_progr_subsys, lv, data->personal.id_static, data->id_dynamic);
						data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
						errormessage=new char[100];
						sprintf(errormessage, "done programming of %d - %d (lv.%d) but in mini testing", data->programming.mini_cause_progr_sys, data->programming.mini_cause_progr_subsys, lv);
						memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
						delete[] errormessage;
						cyberspace_user_connection_output(data, &(data->out_command));
						}
					if(success) {
						data->personal.subsystems[data->programming.mini_cause_progr_sys][data->programming.mini_cause_progr_subsys]=data->personal.subsystems_hint[data->programming.mini_cause_progr_sys][data->programming.mini_cause_progr_subsys];
						resend_personal=true;
						}
					break;
				case CYBERSPACE_COMMAND_MINIGAME_CAUSE_TRAINING_HEURISTIC:
					if(data->personal.heuristic_record<data->mini_testing.points) {
						data->personal.heuristic_record=data->mini_testing.points;
						resend_personal=true;
						}
					break;
				case CYBERSPACE_COMMAND_MINIGAME_CAUSE_TRAINING_FAST:
					if(data->personal.fast_record<data->mini_testing.points) {
						data->personal.fast_record=data->mini_testing.points;
						resend_personal=true;
						}
					break;
				case CYBERSPACE_COMMAND_MINIGAME_CAUSE_TRAINING_PATTERNMATCHING:
					if(data->personal.patternmatching_record<data->mini_testing.points) {
						data->personal.patternmatching_record=data->mini_testing.points;
						resend_personal=true;
						}
					break;
				default:
					fprintf(stderr, "WARNING: unexpected data->programming.mini_cause in minigame testing (%d) by connection %d (%d).\n", data->programming.mini_cause, data->personal.id_static, data->id_dynamic);
					data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
					errormessage=new char[100];
					sprintf(errormessage, "unexpected data->programming.mini_cause in minigame testing");
					memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
					delete[] errormessage;
					cyberspace_user_connection_output(data, &(data->out_command));
					break;
				}
			if(resend_personal) {
				data->out_command.type=CYBERSPACE_COMMAND_SERVER_USER;
				memcpy(data->out_command.data, &(data->personal), sizeof(data->personal));
				cyberspace_user_connection_output(data, &(data->out_command));
				}
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_PHASE_PROGRAMMING;
			cyberspace_user_connection_output(data, &(data->out_command));
			data->phase=CYBERSPACE_PHASE_PROGRAMMING;
			break;
		default:
			fprintf(stderr, "WARNING: unexpected message in minigame testing (%d) by connection %d (%d).\n", data->in_command.type, data->personal.id_static, data->id_dynamic);
			data->out_command.type=CYBERSPACE_COMMAND_SERVER_ERROR;
			errormessage=new char[100];
			sprintf(errormessage, "unexpected message in minigame testing");
			memcpy(data->out_command.data, errormessage, strlen(errormessage)+1);
			delete[] errormessage;
			cyberspace_user_connection_output(data, &(data->out_command));
			break;
		}

	}

