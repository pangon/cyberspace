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


#include "cyberspace_GUI_materials.h"

GLfloat* cyberspace_GUI_materials_ambient=new GLfloat[4];
GLfloat* cyberspace_GUI_materials_diffuse=new GLfloat[4];
GLfloat* cyberspace_GUI_materials_specular=new GLfloat[4];
GLfloat cyberspace_GUI_materials_shininess;
bool z_buffer_write_disabled=false;

void cyberspace_GUI_materials_set(int material) {

	switch(material) {
		case CYBERSPACE_GUI_MATERIALS_STEEL:
			if(z_buffer_write_disabled) {
				glDepthMask(GL_TRUE);
				z_buffer_write_disabled=!z_buffer_write_disabled;
				}
			cyberspace_GUI_materials_ambient[0]=0.05;
			cyberspace_GUI_materials_ambient[1]=0.05;
			cyberspace_GUI_materials_ambient[2]=0.05;
			cyberspace_GUI_materials_ambient[3]=1.0;

			cyberspace_GUI_materials_diffuse[0]=0.2;
			cyberspace_GUI_materials_diffuse[1]=0.2;
			cyberspace_GUI_materials_diffuse[2]=0.2;
			cyberspace_GUI_materials_diffuse[3]=1.0;

			cyberspace_GUI_materials_specular[0]=0.1;
			cyberspace_GUI_materials_specular[1]=0.1;
			cyberspace_GUI_materials_specular[2]=0.1;
			cyberspace_GUI_materials_specular[3]=1.0;

			cyberspace_GUI_materials_shininess=50.0;
/*			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, {0.2, 0.2, 0.2, 1.0});
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, {1.0, 1.0, 0.8, 1.0});
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, {1.0, 1.0, 0.8, 1.0});
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 100.0);*/
			break;
		case CYBERSPACE_GUI_MATERIALS_BRASS:
			if(z_buffer_write_disabled) {
				glDepthMask(GL_TRUE);
				z_buffer_write_disabled=!z_buffer_write_disabled;
				}
			cyberspace_GUI_materials_ambient[0]=0.329412;
			cyberspace_GUI_materials_ambient[1]=0.223529;
			cyberspace_GUI_materials_ambient[2]=0.027451;
			cyberspace_GUI_materials_ambient[3]=1.0;

			cyberspace_GUI_materials_diffuse[0]=0.780392;
			cyberspace_GUI_materials_diffuse[1]=0.568627;
			cyberspace_GUI_materials_diffuse[2]=0.113725;
			cyberspace_GUI_materials_diffuse[3]=1.0;

			cyberspace_GUI_materials_specular[0]=0.992157;
			cyberspace_GUI_materials_specular[1]=0.941176;
			cyberspace_GUI_materials_specular[2]=0.807843;
			cyberspace_GUI_materials_specular[3]=1.0;

			cyberspace_GUI_materials_shininess=27.8974;
			break;
		case CYBERSPACE_GUI_MATERIALS_RUBBER_RED:
			if(z_buffer_write_disabled) {
				glDepthMask(GL_TRUE);
				z_buffer_write_disabled=!z_buffer_write_disabled;
				}
			cyberspace_GUI_materials_ambient[0]=0.2;
			cyberspace_GUI_materials_ambient[1]=0.2;
			cyberspace_GUI_materials_ambient[2]=0.2;
			cyberspace_GUI_materials_ambient[3]=1.0;

			cyberspace_GUI_materials_diffuse[0]=1.0;
			cyberspace_GUI_materials_diffuse[1]=0.5;
			cyberspace_GUI_materials_diffuse[2]=0.5;
			cyberspace_GUI_materials_diffuse[3]=1.0;

			cyberspace_GUI_materials_specular[0]=1.0;
			cyberspace_GUI_materials_specular[1]=0.5;
			cyberspace_GUI_materials_specular[2]=0.5;
			cyberspace_GUI_materials_specular[3]=1.0;

			cyberspace_GUI_materials_shininess=20.0;
			break;
		case CYBERSPACE_GUI_MATERIALS_GLASS_STEEL:
			if(z_buffer_write_disabled) {
				glDepthMask(GL_TRUE);
				z_buffer_write_disabled=!z_buffer_write_disabled;
				}
/*			if(!z_buffer_write_disabled) {
				glDepthMask(GL_FALSE);
				z_buffer_write_disabled=!z_buffer_write_disabled;
				}*/
			cyberspace_GUI_materials_ambient[0]=0.1;
			cyberspace_GUI_materials_ambient[1]=0.1;
			cyberspace_GUI_materials_ambient[2]=0.1;
			cyberspace_GUI_materials_ambient[3]=0.75;

			cyberspace_GUI_materials_diffuse[0]=0.4;
			cyberspace_GUI_materials_diffuse[1]=0.4;
			cyberspace_GUI_materials_diffuse[2]=0.4;
			cyberspace_GUI_materials_diffuse[3]=0.75;

			cyberspace_GUI_materials_specular[0]=0.2;
			cyberspace_GUI_materials_specular[1]=0.2;
			cyberspace_GUI_materials_specular[2]=0.2;
			cyberspace_GUI_materials_specular[3]=0.5;

			cyberspace_GUI_materials_shininess=50.0;
			break;
		case CYBERSPACE_GUI_MATERIALS_FORCEFIELD_BLUE:
			if(z_buffer_write_disabled) {
				glDepthMask(GL_TRUE);
				z_buffer_write_disabled=!z_buffer_write_disabled;
				}
/*			if(!z_buffer_write_disabled) {
				glDepthMask(GL_FALSE);
				z_buffer_write_disabled=!z_buffer_write_disabled;
				}*/
			cyberspace_GUI_materials_ambient[0]=0.1;
			cyberspace_GUI_materials_ambient[1]=0.1;
			cyberspace_GUI_materials_ambient[2]=0.5;
			cyberspace_GUI_materials_ambient[3]=0.2;

			cyberspace_GUI_materials_diffuse[0]=0.2;
			cyberspace_GUI_materials_diffuse[1]=0.2;
			cyberspace_GUI_materials_diffuse[2]=1.0;
			cyberspace_GUI_materials_diffuse[3]=0.2;

			cyberspace_GUI_materials_specular[0]=0.2;
			cyberspace_GUI_materials_specular[1]=0.2;
			cyberspace_GUI_materials_specular[2]=1.0;
			cyberspace_GUI_materials_specular[3]=0.2;

			cyberspace_GUI_materials_shininess=30.0;
			break;
		case CYBERSPACE_GUI_MATERIALS_USER_GREEN:
			if(z_buffer_write_disabled) {
				glDepthMask(GL_TRUE);
				z_buffer_write_disabled=!z_buffer_write_disabled;
				}
			cyberspace_GUI_materials_ambient[0]=0.3;
			cyberspace_GUI_materials_ambient[1]=0.3;
			cyberspace_GUI_materials_ambient[2]=0.3;
			cyberspace_GUI_materials_ambient[3]=1.0;

			cyberspace_GUI_materials_diffuse[0]=0.2;
			cyberspace_GUI_materials_diffuse[1]=1.0;
			cyberspace_GUI_materials_diffuse[2]=0.2;
			cyberspace_GUI_materials_diffuse[3]=1.0;

			cyberspace_GUI_materials_specular[0]=0.2;
			cyberspace_GUI_materials_specular[1]=1.0;
			cyberspace_GUI_materials_specular[2]=0.2;
			cyberspace_GUI_materials_specular[3]=1.0;

			cyberspace_GUI_materials_shininess=5.0;
			break;
		case CYBERSPACE_GUI_MATERIALS_OBSIDIAN:
			if(z_buffer_write_disabled) {
				glDepthMask(GL_TRUE);
				z_buffer_write_disabled=!z_buffer_write_disabled;
				}
			cyberspace_GUI_materials_ambient[0]=0.05375;
			cyberspace_GUI_materials_ambient[1]=0.05;
			cyberspace_GUI_materials_ambient[2]=0.06625;
			cyberspace_GUI_materials_ambient[3]=1.0;

			cyberspace_GUI_materials_diffuse[0]=0.18275;
			cyberspace_GUI_materials_diffuse[1]=0.17;
			cyberspace_GUI_materials_diffuse[2]=0.22525;
			cyberspace_GUI_materials_diffuse[3]=1.0;

			cyberspace_GUI_materials_specular[0]=0.332741;
			cyberspace_GUI_materials_specular[1]=1.328634;
			cyberspace_GUI_materials_specular[2]=0.346435;
			cyberspace_GUI_materials_specular[3]=1.0;

			cyberspace_GUI_materials_shininess=38.4;
			break;
		case CYBERSPACE_GUI_MATERIALS_RUBY:
			if(z_buffer_write_disabled) {
				glDepthMask(GL_TRUE);
				z_buffer_write_disabled=!z_buffer_write_disabled;
				}
			cyberspace_GUI_materials_ambient[0]=0.1745;
			cyberspace_GUI_materials_ambient[1]=0.01175;
			cyberspace_GUI_materials_ambient[2]=0.01175;
			cyberspace_GUI_materials_ambient[3]=1.0;

			cyberspace_GUI_materials_diffuse[0]=0.81424;
			cyberspace_GUI_materials_diffuse[1]=0.24136;
			cyberspace_GUI_materials_diffuse[2]=0.24136;
			cyberspace_GUI_materials_diffuse[3]=1.0;

			cyberspace_GUI_materials_specular[0]=0.927811;
			cyberspace_GUI_materials_specular[1]=0.726959;
			cyberspace_GUI_materials_specular[2]=0.726959;
			cyberspace_GUI_materials_specular[3]=1.0;

			cyberspace_GUI_materials_shininess=76.8;
			break;
		default:
			fprintf(stderr, "ERROR: cyberspace_GUI_materials_set unknown material %d.\n", material);
			exit(EXIT_FAILURE);
			break;
		}
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, cyberspace_GUI_materials_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cyberspace_GUI_materials_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, cyberspace_GUI_materials_specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, cyberspace_GUI_materials_shininess);
	}

/*
const GltMaterial blackPlasticMaterial
00155 (
00156     GltColor(0.00, 0.00, 0.00), // Ambient
00157     GltColor(0.01, 0.01, 0.01), // Diffuse
00158     GltColor(0.50, 0.50, 0.50), // Specular
00159     GltColor(0.00, 0.00, 0.00), // Emission
00160     32.0f                       // Shininess
00161 );
00162 
00164 
00165 const GltMaterial blackRubberMaterial
00166 (
00167     GltColor(0.02, 0.02, 0.02), // Ambient
00168     GltColor(0.01, 0.01, 0.01), // Diffuse
00169     GltColor(0.40, 0.40, 0.40), // Specular
00170     GltColor(0.00, 0.00, 0.00), // Emission
00171     10.0f                       // Shininess
00172 );
00173 
00175 
00186 
00187 const GltMaterial bronzeMaterial
00188 (
00189     GltColor(0.2125  , 0.1275  , 0.054   ), // Ambient
00190     GltColor(0.714   , 0.4284  , 0.18144 ), // Diffuse
00191     GltColor(0.393548, 0.271906, 0.166721), // Specular
00192     GltColor(0.0,0.0,0.0),                  // Emission
00193     25.6f                                   // Shininess
00194 );
00195 
00197 
00198 const GltMaterial chromeMaterial
00199 (
00200     GltColor(0.25,     0.25,     0.25    ), // Ambient
00201     GltColor(0.4,      0.4,      0.4     ), // Diffuse
00202     GltColor(0.774597, 0.774597, 0.774597), // Specular
00203     GltColor(0.0,0.0,0.0),                  // Emission
00204     76.8f                                   // Shininess
00205 );
00206 
00208 
00209 const GltMaterial copperMaterial
00210 (
00211     GltColor(0.19125,  0.0735,   0.0225  ), // Ambient
00212     GltColor(0.7038,   0.27048,  0.0828  ), // Diffuse
00213     GltColor(0.256777, 0.137622, 0.086014), // Specular
00214     GltColor(0.00,0.00,0.00),               // Emission
00215     12.8f                                   // Shininess
00216 );
00217 
00219 
00220 const GltMaterial emeraldMaterial
00221 (
00222     GltColor(0.0215,  0.1745,   0.0215 ),   // Ambient
00223     GltColor(0.07568, 0.61424,  0.07568),   // Diffuse
00224     GltColor(0.633,   0.727811, 0.633  ),   // Specular
00225     GltColor(0.00,0.00,0.00),               // Emission
00226     76.8f                                   // Shininess
00227 );
00228 
00230 
00231 const GltMaterial goldMaterial
00232 (
00233     GltColor(0.24725,  0.1995,   0.0745),   // Ambient
00234     GltColor(0.75164,  0.60648,  0.22648),  // Diffuse
00235     GltColor(0.628281, 0.555802, 0.366065), // Specular
00236     GltColor(0.00,0.00,0.00),               // Emission
00237     51.2f                                   // Shininess
00238 );
00239 
00241 
00242 const GltMaterial jadeMaterial
00243 (
00244     GltColor(0.135,    0.2225,   0.1575  ), // Ambient
00245     GltColor(0.135,    0.2225,   0.1575  ), // Diffuse
00246     GltColor(0.316228, 0.316228, 0.316228), // Specular
00247     GltColor(0.00,0.00,0.00),               // Emission
00248     12.8f                                   // Shininess
00249 );
00250 


00261 
00263 
00264 const GltMaterial pearlMaterial
00265 (
00266     GltColor(0.25,     0.20725,  0.20725 ), // Ambient
00267     GltColor(1.0,      0.829,    0.829   ), // Diffuse
00268     GltColor(0.296648, 0.296648, 0.296648), // Specular
00269     GltColor(0.00,0.00,0.00),               // Emission
00270     11.264f                                 // Shininess
00271 );
00272 
00274 
00275 const GltMaterial pewterMaterial
00276 (
00277     GltColor(0.105882, 0.058824, 0.113725), // Ambient
00278     GltColor(0.427451, 0.470588, 0.541176), // Diffuse
00279     GltColor(0.333333, 0.333333, 0.521569), // Specular
00280     GltColor(0.00,0.00,0.00),               // Emission
00281     9.84615f                                // Shininess
00282 );
00283 
00285 
00286 const GltMaterial polishedBronzeMaterial
00287 (
00288     GltColor(0.25    , 0.148   , 0.06475 ), // Ambient
00289     GltColor(0.4     , 0.2368  , 0.1036  ), // Diffuse
00290     GltColor(0.774597, 0.458561, 0.200621), // Specular
00291     GltColor(0.0,0.0,0.0),                  // Emission
00292     76.8f                                   // Shininess
00293 );
00294 
00296 
00297 const GltMaterial polishedCopperMaterial
00298 (
00299     GltColor(0.2295,   0.08825,  0.0275   ),    // Ambient
00300     GltColor(0.5508,   0.2118,   0.066    ),    // Diffuse
00301     GltColor(0.580594, 0.223257, 0.0695701),    // Specular
00302     GltColor(0.0,0.0,0.0),                      // Emission
00303     51.2f                                       // Shininess
00304 );
00305 
00307 
00308 const GltMaterial polishedGoldMaterial
00309 (
00310     GltColor(0.24725,  0.2245,   0.0645  ), // Ambient
00311     GltColor(0.34615,  0.3143,   0.0903  ), // Diffuse
00312     GltColor(0.797357, 0.723991, 0.208006), // Specular
00313     GltColor(0.0,0.0,0.0),                  // Emission
00314     83.2f                                   // Shininess
00315 );
00316 
00318 
00319 const GltMaterial polishedSilverMaterial
00320 (
00321     GltColor(0.23125, 0.23125, 0.23125),    // Ambient
00322     GltColor(0.2775,  0.2775,  0.2775),     // Diffuse
00323     GltColor(0.773911, 0.773911, 0.773911), // Specular
00324     GltColor(0.0,0.0,0.0),                  // Emission
00325     89.6f                                   // Shininess
00326 );
00327 
00329 
00330 const GltMaterial rubyMaterial
00331 (
00332     GltColor(0.1745,   0.01175,  0.01175),  // Ambient
00333     GltColor(0.61424,  0.04136,  0.04136),  // Diffuse
00334     GltColor(0.727811, 0.626959, 0.626959), // Specular
00335     GltColor(0.00,0.00,0.00),               // Emission
00336     76.8f                                   // Shininess
00337 );
00338 
00340 
00341 const GltMaterial silverMaterial
00342 (
00343     GltColor(0.19225,  0.19225,  0.19225 ), // Ambient
00344     GltColor(0.50754,  0.50754,  0.50754 ), // Diffuse
00345     GltColor(0.508273, 0.508273, 0.508273), // Specular
00346     GltColor(0.00,0.00,0.00),               // Emission
00347     51.2f                                   // Shininess
00348 );
00349 
00351 
00352 const GltMaterial turquoiseMaterial
00353 (
00354     GltColor(0.1,      0.18725, 0.1745  ),  // Ambient
00355     GltColor(0.396,    0.74151, 0.69102 ),  // Diffuse
00356     GltColor(0.297254, 0.30829, 0.306678),  // Specular
00357     GltColor(0.00,0.00,0.00),               // Emission
00358     12.8f                                   // Shininess
00359 );
00360 
00362 
00363 const GltMaterial whitePlasticMaterial
00364 (
00365     GltColor(0.00, 0.00, 0.00), // Ambient
00366     GltColor(0.55, 0.55, 0.55), // Diffuse
00367     GltColor(0.70, 0.70, 0.70), // Specular
00368     GltColor(0.00, 0.00, 0.00), // Emission
00369     32.0f                       // Shininess
00370 );
*/

