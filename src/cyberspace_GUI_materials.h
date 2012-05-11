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


#ifndef CYBERSPACE_GUI_MATERIALS
#define CYBERSPACE_GUI_MATERIALS

#include <stdio.h>
#include <stdlib.h>
#include <SDL_opengl.h>

#define CYBERSPACE_GUI_MATERIALS_STEEL 0
#define CYBERSPACE_GUI_MATERIALS_RUBBER_RED 1
#define CYBERSPACE_GUI_MATERIALS_BRASS 2
#define CYBERSPACE_GUI_MATERIALS_USER_GREEN 3
#define CYBERSPACE_GUI_MATERIALS_OBSIDIAN 4
#define CYBERSPACE_GUI_MATERIALS_FORCEFIELD_BLUE 5
#define CYBERSPACE_GUI_MATERIALS_GLASS_STEEL 6
#define CYBERSPACE_GUI_MATERIALS_RUBY 7

void cyberspace_GUI_materials_set(int material);

#endif /* CYBERSPACE_GUI_MATERIALS */

