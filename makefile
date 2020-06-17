### OUTPUT ###
BUILD = Build

### SOURCES ###
SRC = Src/main\
	  Src/Game/Game\
	  Src/Shader/Shader\
	  Src/Texture/Texture\
	  Src/ResourceManager/ResourceManager\
	  Src/SpriteRenderer/SpriteRenderer\
	  Src/StbImage/stb_image\
	  Src/GameLevel/GameLevel\
	  Src/GameObject/GameObject\
	  Src/BallObject/BallObject

### OBJECTS ###
OBJ = $(addsuffix .o, $(addprefix $(BUILD)/, $(SRC)))

### INCLUDES ###
INC = -ID:/Development/OpenGL/Glfw/include/GLFW\
	  -ID:/Development/OpenGL/Glew/glew-2.1.0/include/GL\
	  -ID:/Development/OpenGL/Glm/glm-0.9.9.8/glm\
	  -ISrc/Game\
	  -ISrc/Shader\
	  -ISrc/Texture\
	  -ISrc/ResourceManager\
	  -ISrc/SpriteRenderer\
	  -ISrc/StbImage\
	  -ISrc/GameLevel\
	  -ISrc/GameObject\
	  -ISrc/BallObject

### LINKER FLAGS ###
LDFLAGS = -LD:/Development/OpenGL/Glew/glew-2.1.0/lib/Release/Win32 -lglew32s\
		  -LD:/Development/OpenGL/Glfw/lib-mingw -lglfw3\
		  -lopengl32\
		  -lgdi32

LDLIBS = -lglew32s -lglfw3 -lopengl32 -lgdi32

### COMPILER FLAGS
CFLAGS = $(INC)

### COMPILER ###
CC = g++

all: $(BUILD)/test.exe

$(BUILD)/test.exe: $(OBJ)
	@echo LINKING $^
	@$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
	
$(BUILD)/%.o: %.cpp
	@echo COMPILING $<
	@mkdir -p $(subst /,\,$(dir $@))
	@$(CC) $(CFLAGS) -M -MT $@ -o $(patsubst %.o, %.d, $@) $<
	@$(CC) $(CFLAGS) -o $@ -c $<
	
-include $(OBJ:.o=.d) 
	
.PHONY: clean

clean:
	@echo CLEANING......
	@rm -rf $(BUILD)/*
