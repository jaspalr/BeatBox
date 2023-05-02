# Makefile for building embedded application.
# by Brian Fraser

# Edit this file to compile extra C files into their own programs.
TARGET= wave_player

SOURCES= wave_player.c


PUBDIR = $(HOME)/cmpt433/public/myApps
OUTDIR = $(PUBDIR)
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc

CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror -Wshadow


# Asound process:
# get alibsound2 lib on target:
# 	# apt-get install libasound2
# Copy target's /usr/lib/arm-linux-gnueabihf/libasound.so.2.0.0 
#      to host  ~/public/asound_lib_BBB/libasound.so
# Copy to just base library:

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB


# -pg for supporting gprof profiling.
#CFLAGS += -pg



all: beatbox wav node

beatbox :main.c $(OUTDIR)/audioMixer.o $(OUTDIR)/joystick.o $(OUTDIR)/beat.o $(OUTDIR)/joystickcontrols.o $(OUTDIR)/accelerometer.o $(OUTDIR)/periodTimer.o $(OUTDIR)/terminal.o $(OUTDIR)/udpserver.o
	$(CC_C) $(CFLAGS) main.c $(OUTDIR)/audioMixer.o $(OUTDIR)/joystick.o  $(OUTDIR)/beat.o $(OUTDIR)/joystickcontrols.o $(OUTDIR)/periodTimer.o $(OUTDIR)/accelerometer.o  $(OUTDIR)/terminal.o $(OUTDIR)/udpserver.o -o $(OUTDIR)/$(TARGET)  $(LFLAGS) -lpthread -lasound

# Copy wave files to the shared folder
wav:
	mkdir -p $(PUBDIR)/wave-files/
	cp wave-files/* $(PUBDIR)/wave-files/ 

node: 
	make --directory=beatboxServer

$(OUTDIR)/udpserver.o: udpserver.c udpserver.h shutdown.h
	$(CC_C) $(CFLAGS) -c udpserver.c -o $(OUTDIR)/udpserver.o

$(OUTDIR)/beat.o: beat.c beat.h 
	$(CC_C) $(CFLAGS) -c beat.c -o $(OUTDIR)/beat.o

$(OUTDIR)/joystickcontrols.o: joystickcontrols.c joystickcontrols.h 
	$(CC_C) $(CFLAGS) -c joystickcontrols.c -o $(OUTDIR)/joystickcontrols.o

$(OUTDIR)/joystick.o: joystick.c joystick.h
	$(CC_C) $(CFLAGS) -c joystick.c -o $(OUTDIR)/joystick.o

$(OUTDIR)/accelerometer.o: accelerometer.c accelerometer.h
	$(CC_C) $(CFLAGS) -c accelerometer.c -o $(OUTDIR)/accelerometer.o

$(OUTDIR)/audioMixer.o: audioMixer_template.c audioMixer_template.h
	$(CC_C) $(CFLAGS) -c audioMixer_template.c -o $(OUTDIR)/audioMixer.o

$(OUTDIR)/periodTimer.o: periodTimer.c periodTimer.h
	$(CC_C) $(CFLAGS) -c periodTimer.c -o $(OUTDIR)/periodTimer.o

$(OUTDIR)/terminal.o: terminaloutput.c terminaloutput.h
	$(CC_C) $(CFLAGS) -c terminaloutput.c -o $(OUTDIR)/terminal.o
clean:
	rm -f $(OUTDIR)/$(TARGET)
