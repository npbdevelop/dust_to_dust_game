# Variables
VPATH = src:bin:lib
OBJS = vector.o physics.o animation.o assets.o action.o entity.o components.o scenemenu.o sceneplay.o \
	   scene.o entitymanager.o game_engine.o run.o
COMPILER_FLAGS = -std=c++23
LINKER_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -o 
SOURCE = ./src
BIN = ./bin
LIBRARY = ./lib
OUT = $(BIN)/exec.out
LIST = $(addprefix $(LIBRARY)/, $(OBJS))

# Rules
$(OUT): $(LIST)
	g++ $(COMPILER_FLAGS) $(LIST) $(LINKER_FLAGS) $(OUT)

all: $(LIST) 

$(LIBRARY)/%.o: $(SOURCE)/%.cpp
	g++ $(COMPILER_FLAGS) -c $< $(LINKER_FLAGS) $@

clean:
	rm $(OUT) $(LIST)