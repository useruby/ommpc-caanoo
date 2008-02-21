#-------------------------------------------------------------------------------
# Simple Makefile
#
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
# Be sure to use the cxx compiler
#-------------------------------------------------------------------------------
CC = gcc
CXX = g++


#-------------------------------------------------------------------------------
# Desired optimization level
#-------------------------------------------------------------------------------
O_FLAG = -Wall -Werror
#O_FLAG = -O1
OPTIMIZATION_FLAG = -g

#-------------------------------------------------------------------------------
# The directories to look for include files and libraries
#-------------------------------------------------------------------------------
INCDIR = -I. -I/usr/include/SDL
LIB = -L. 

#-------------------------------------------------------------------------------
# Compiler flags
#-------------------------------------------------------------------------------
CXXFLAGS      = $(OPTIMIZATION_FLAG) $(INCDIR) $(LIB)

#-------------------------------------------------------------------------------
# The .cpp files used to compile the client
#-------------------------------------------------------------------------------
.SUFFIXES: .cpp .c

.c.o:
	$(CC) $(CXXFLAGS) -c $<
.c:
	$(CC) $(CXXFLAGS) $< -o pc/$@
	
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $<

.cpp:
	$(CXX) $(CXXFLAGS) $< -o pc/$@

SRC     = plBrowser.cpp browser.cpp playlist.cpp main.cpp config.cpp nowPlaying.cpp statsBar.cpp commandFactory.cpp popup.cpp scroller.cpp timestamp.cpp helpBar.cpp albumArt.cpp threadFunctions.cpp bookmarks.cpp gp2xregs.cpp overlay.cpp

OBJ = $(SRC:.cpp=.o) libmpdclient.o
#-------------------------------------------------------------------------------
# Libraries being linked with the application
#-------------------------------------------------------------------------------
LIBS = -lSDL -lSDLmain -lSDL_ttf -lSDL_image -lSDL_gfx

#-------------------------------------------------------------------------------
# Target for creating simple client
#-------------------------------------------------------------------------------
ommpc: $(OBJ)
	@echo $(OBJ)
	@echo $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS) \
		$(SYSLIBS) $(LIBS)
	@echo "SUCCESS"

watchdog: 
	$(CXX) $(CXXFLAGS) -c watchdog.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o watchdog watchdog.o config.o libmpdclient.o

#-------------------------------------------------------------------------------
# Target for "make all"
#-------------------------------------------------------------------------------
all:
	make clean
	make ommp2x 


#-------------------------------------------------------------------------------
# Target for "make clean"
#-------------------------------------------------------------------------------
clean:
	rm -rf core *.o ommp2x *~ *.*~ .*~ \#* .in* tca.* *vti* *.idl cxx_repository


#---
#Dependecies
#---
depend:
	makedepend $(CXXFLAGS) -Y $(SRC)
# DO NOT DELETE

plBrowser.o: plBrowser.h /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
plBrowser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_ttf.h
plBrowser.o: libmpdclient.h scroller.h threadParms.h commandFactory.h
plBrowser.o: playlist.h config.h timer.h
browser.o: browser.h /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
browser.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_ttf.h
browser.o: libmpdclient.h scroller.h threadParms.h commandFactory.h config.h
playlist.o: playlist.h /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
playlist.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_ttf.h config.h
playlist.o: libmpdclient.h scroller.h timer.h threadParms.h commandFactory.h
playlist.o: popup.h timestamp.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_thread.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_ttf.h
main.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_image.h config.h
main.o: browser.h libmpdclient.h scroller.h plBrowser.h bookmarks.h
main.o: playlist.h timer.h albumArt.h threadParms.h nowPlaying.h
main.o: threadFunctions.h commandFactory.h statsBar.h helpBar.h popup.h
main.o: gp2xregs.h
config.o: config.h /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
config.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
nowPlaying.o: nowPlaying.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
nowPlaying.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_ttf.h
nowPlaying.o: libmpdclient.h config.h playlist.h scroller.h timer.h
nowPlaying.o: threadParms.h
statsBar.o: statsBar.h /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
statsBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_ttf.h
statsBar.o: libmpdclient.h config.h playlist.h scroller.h timer.h
statsBar.o: threadParms.h commandFactory.h
commandFactory.o: commandFactory.h libmpdclient.h threadParms.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
commandFactory.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
popup.o: popup.h /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
popup.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_ttf.h
popup.o: libmpdclient.h config.h playlist.h scroller.h timer.h threadParms.h
popup.o: commandFactory.h gp2xregs.h
scroller.o: scroller.h /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
scroller.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_ttf.h
scroller.o: libmpdclient.h config.h commandFactory.h
timestamp.o: timestamp.h
helpBar.o: helpBar.h /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
helpBar.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_ttf.h
helpBar.o: libmpdclient.h config.h
albumArt.o: albumArt.h /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
albumArt.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_ttf.h
albumArt.o: threadParms.h libmpdclient.h config.h threadFunctions.h
threadFunctions.o: threadFunctions.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_image.h
threadFunctions.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rotozoom.h
threadFunctions.o: threadParms.h libmpdclient.h
bookmarks.o: bookmarks.h /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_main.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_types.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_getenv.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_error.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/begin_code.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/close_code.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_rwops.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_timer.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_audio.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_byteorder.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_cdrom.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_joystick.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_events.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_active.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keyboard.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_keysym.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mouse.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_video.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_mutex.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_quit.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_version.h
bookmarks.o: /opt/open2x/gcc-4.1.1-glibc-2.3.6/include/SDL/SDL_ttf.h
bookmarks.o: libmpdclient.h scroller.h threadParms.h commandFactory.h
bookmarks.o: playlist.h config.h timer.h statsBar.h
gp2xregs.o: gp2xregs.h
