CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include
LDFLAGS = -lsqlite3 -lcurl

SRC = src/main.cpp \
      src/ai.cpp \
      src/memory.cpp \
      src/web.cpp \
      src/format.cpp \
      src/utils.cpp

OBJ = $(SRC:.cpp=.o)
TARGET = build/bot

# сюда вставь свои реальные ключи
GOOGLE_API_KEY = твой_ключ
GOOGLE_CX      = твой_cx

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	GOOGLE_API_KEY=$(GOOGLE_API_KEY) GOOGLE_CX=$(GOOGLE_CX) ./build/bot