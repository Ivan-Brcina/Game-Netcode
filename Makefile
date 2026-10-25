# Makefile for Netcode (cubesServer + cubeClient)
# Put this in the project root, next to CMakeLists.txt.
# Usage:  make            build both
#         make cubesServer
#         make cubeClient
#         make clean

CXX      = g++
CC       = gcc
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
CFLAGS   = -O2
# -MMD -MP makes the compiler write .d files so editing a header rebuilds what depends on it
CPPFLAGS = -ISocket-Packet -ICubes -IRendering -MMD -MP

# ---- Platform-specific libraries ----
UNAME := $(shell uname -s)

GL_LIBS  = -lglfw -lGL -ldl
NET_LIBS =

BUILD = build

# ---- Sources ----
# Used by both programs
NET_SRC  = Socket-Packet/myPacket.cpp \
           Socket-Packet/mySerialization.cpp \
           Socket-Packet/mySocket.cpp \
           Socket-Packet/myStream.cpp

GAME_SRC = Cubes/cubeGame.cpp \
           Cubes/cubePlayer.cpp

# Client only
RENDER_CXX = Rendering/cubeRenderer.cpp \
             Rendering/mesh.cpp \
             Rendering/shaders.cpp
RENDER_C   = Rendering/gl.c

# ---- Object files ----
NET_OBJ    = $(NET_SRC:%.cpp=$(BUILD)/%.o)
GAME_OBJ   = $(GAME_SRC:%.cpp=$(BUILD)/%.o)
RENDER_OBJ = $(RENDER_CXX:%.cpp=$(BUILD)/%.o) $(RENDER_C:%.c=$(BUILD)/%.o)

SERVER_MAIN = $(BUILD)/Cubes/cubesServer.o
CLIENT_MAIN = $(BUILD)/Cubes/cubeClient.o

SERVER_OBJ = $(SERVER_MAIN) $(GAME_OBJ) $(NET_OBJ)
CLIENT_OBJ = $(CLIENT_MAIN) $(GAME_OBJ) $(NET_OBJ) $(RENDER_OBJ)

# ---- Targets ----
all: cubesServer cubeClient

cubesServer: $(SERVER_OBJ)
	$(CXX) $^ -o $@ $(NET_LIBS)

cubeClient: $(CLIENT_OBJ)
	$(CXX) $^ -o $@ $(GL_LIBS) $(NET_LIBS)

# ---- Compile rules ----
$(BUILD)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD) cubesServer cubeClient

.PHONY: all clean

# Pull in the auto-generated header dependencies
-include $(SERVER_OBJ:.o=.d) $(CLIENT_OBJ:.o=.d)
