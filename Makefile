# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11 -shared -Os -I./src/lua -I./src/discord

# Linker flags
LDFLAGS = -L./discord

# Libraries to link against
LDLIBS = -ldiscord_game_sdk

# Target DLL name
TARGET = darktide_discord_pluginw64.dll

# Source file
SRC = src/darktide_discord.cpp

LIBFILES=$(wildcard src/discord/*.cpp)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) -o $(TARGET) $(LIBFILES) $(SRC)

clean:
	del /F /Q "$(TARGET)"
