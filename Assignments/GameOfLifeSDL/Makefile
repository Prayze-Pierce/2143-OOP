CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iincludes $(shell sdl2-config --cflags)
LDFLAGS  = $(shell sdl2-config --libs)

SRC = \
    src/ConwayLife.cpp \
    src/ArgsToJson.cpp \
    src/SdlScreen.cpp \
    src/SDL_GOL_main.cpp

OBJ    = $(SRC:.cpp=.o)
TARGET = SDL_GOL_main.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
