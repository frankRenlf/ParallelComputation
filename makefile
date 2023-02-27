#
# A simple makefile that compiles GLFW on Linux or Macs.
#
EXE = Mandelbrot
CC = gcc
CCFLAGS = -Wall -fopenmp -DGL_SILENCE_DEPRECATION

OS = $(shell uname)

ifeq ($(OS), Linux)
	CCFLAGS += -lGL -lglfw 
endif

ifeq ($(OS), Darwin)
	MSG = Requires GLFW\; current include/lib dirs work for GLFW installed via homebrew but may need to be altered for other distributions.
	CCFLAGS += -l glfw -framework OpenGL -L /usr/local/lib -I /usr/local/include
endif

all:
	@echo $(MSG)
	@echo
	$(CC) -o $(EXE) Mandelbrot.c $(CCFLAGS) 
