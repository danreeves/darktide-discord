# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11 -shared -Os

# Linker flags
LDFLAGS = -L./discord

# Libraries to link against
LDLIBS = -ldiscord_game_sdk

# Target DLL name
TARGET = darktide_discord_pluginw64.dll

# Source file
SRC = darktide_discord/darktide_discord.cpp

LIBFILES=$(wildcard darktide_discord/discord/*.cpp)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(LIBFILES) $(LDFLAGS) $(LDLIBS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
