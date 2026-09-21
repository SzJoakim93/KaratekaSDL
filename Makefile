#OBJS specifies which files to compile as part of the project
OBJS = main.c \
	dos_sdl_bridge.c \
	assembly_replacements.c \
	C_0FBB.c \
	C_19F5.c \
	C_268A.c \
	C_414A.c \
	C_3233.c \
	tables.c \
	stubs.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -g -std=c99 -DUSE_SDL=1

#LINKER_FLAGS specifies the libraries we're linking against

ifeq ($(OS),Windows_NT)
	LINKER_FLAGS = -mwindows -lMingw32 -lSDLmain -lSDL
else
	LINKER_FLAGS = -lSDL
endif

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = Data/karateka-sdl

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
