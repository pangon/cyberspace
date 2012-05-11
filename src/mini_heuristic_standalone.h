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


#ifndef MINI_HEURISTIC
#define MINI_HEURISTIC

#define CYB_HARDWARE_PC 0
#define CYB_HARDWARE_EEE 1

#define CYB_PHASE_QUIT 0
#define CYB_PHASE_MAIN 1
#define CYB_PHASE_MINI_HEURISTIC 2
#define CYB_PHASE_MINI_FAST 3

#define CYB_GRAPHIC_SCREEN_WIDTH 750
#define CYB_GRAPHIC_SCREEN_HEIGHT 430
#define CYB_GRAPHIC_PHASE_BLACK 0
#define CYB_GRAPHIC_PHASE_MAIN_3D 1
#define CYB_GRAPHIC_PHASE_MINI_HEURISTIC 2
#define CYB_GRAPHIC_PHASE_MINI_HEURISTIC_END 3
#define CYB_GRAPHIC_PHASE_MINI_FAST 4
#define CYB_GRAPHIC_PHASE_MINI_FAST_END 5

#define FONT_SOLID 0
#define FONT_SHADED 1
#define FONT_BLENDED 2
#define FONT_BLACK 0
#define FONT_WHITE 1
#define FONT_GOLD 2
#define FONT_MONO 0
#define FONT_MONOBOLD 1
#define FONT_H 14
#define FONT_W 8


void cyb_mini_heuristic(void);
bool cyb_mini_heuristic_ismorphing(int x, int y);
void cyb_mini_heuristic_morph(int x, int y);
void cyb_mini_heuristic_graphics(void);
void* cyb_graphics(void* voidinfo);

void font_init(void);
void font_setFont(const int font);
void font_setColor(const int color);
void font_print(const char* string, SDL_Surface* screen, const SDL_Rect* to, const int quality=FONT_BLENDED);
void font_free(void);

#endif /* MINI_HEURISTIC */
