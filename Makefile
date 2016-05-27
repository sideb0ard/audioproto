CXX = g++
TARGET = audiorrr
CXXFLAGS = -std=c++11 -Wall -Werror -g
LIBS = -lpthread -lportaudio

SRC = main.cpp subsystem_audio.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CPPFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

clean:
	rm -rf *.o *~ $(TARGET)
