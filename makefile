VPATH = ./src
CC = g++
MING = i586-mingw32msvc-g++
CFLAGS = -Wall -O3


all: cyberspace_server cyberspace_neural_connector

clear:
	rm -f cyberspace_server
	rm -f cyberspace_neural_connector


cyberspace_server : cyberspace_server.cpp cyberspace_frontdoor.cpp cyberspace_user_connection.cpp cyberspace_statistics.cpp cyberspace_server_global.cpp cyberspace_login.cpp cyberspace_3D.cpp cyberspace_world_loader.cpp cyberspace_programming.cpp cyberspace_minigame_server_testing.cpp cyberspace_minigame_server_heuristic.cpp cyberspace_minigame_server_fast.cpp cyberspace_minigame_server_patternmatching.cpp cyberspace_skills.cpp cyberspace_nonvolatile.cpp
	$(CC) $(CFLAGS) -o $@ `sdl-config --cflags --libs` -lSDL_net $^
#	$(CC) $(CFLAGS) -o $@ -pthread `sdl-config --cflags --libs` -lSDL_net $^
#	$(CC) $(CFLAGS) -o $@ -pthread $^

cyberspace_neural_connector: cyberspace_neural_connector.cpp cyberspace_neural_connector_input.cpp cyberspace_client_global.cpp cyberspace_neural_connector_output.cpp cyberspace_GUI.cpp cyberspace_GUI_materials.cpp cyberspace_GUI_font.cpp cyberspace_minigame_client_testing.cpp cyberspace_minigame_client_heuristic.cpp cyberspace_minigame_client_fast.cpp cyberspace_minigame_client_patternmatching.cpp cyberspace_skills.cpp
	$(CC) $(CFLAGS) -o $@ `sdl-config --cflags --libs` -lSDL_net -lGL -lGLU $^

ming_cyberspace_neural_connector: cyberspace_neural_connector.cpp cyberspace_neural_connector_input.cpp cyberspace_client_global.cpp cyberspace_neural_connector_output.cpp cyberspace_GUI.cpp cyberspace_GUI_materials.cpp cyberspace_GUI_font.cpp cyberspace_minigame_client_testing.cpp cyberspace_minigame_client_heuristic.cpp cyberspace_minigame_client_fast.cpp cyberspace_minigame_client_patternmatching.cpp cyberspace_skills.cpp
	$(MING) $(CFLAGS) -o $@ `sdl-config --cflags --libs` -lSDL_net -lGL -lGLU -I/usr/include $^

mini_heuristic : mini_heuristic_standalone.cpp
	$(CC) $(CFLAGS) -o $@ -pthread `sdl-config --cflags --libs` -lSDL_image $^
#	g++ -o $@ `sdl-config --cflags --libs` -lSDL_ttf -lGL $^ -lSDL_image

win_nc: cyberspace_neural_connector.cpp cyberspace_neural_connector_input.cpp cyberspace_client_global.cpp cyberspace_neural_connector_output.cpp cyberspace_GUI.cpp cyberspace_GUI_materials.cpp cyberspace_GUI_font.cpp cyberspace_minigame_client_testing.cpp cyberspace_minigame_client_heuristic.cpp cyberspace_minigame_client_fast.cpp cyberspace_minigame_client_patternmatching.cpp cyberspace_skills.cpp
	$(CC) $(CFLAGS) -o $@ -I/urs/local/include/SDL -L/usr/local/lib -lSDL_net -lGL -lGLU $^

