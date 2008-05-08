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

SRC     = plBrowser.cpp browser.cpp playlist.cpp main.cpp config.cpp nowPlaying.cpp statsBar.cpp commandFactory.cpp popup.cpp scroller.cpp timestamp.cpp helpBar.cpp albumArt.cpp threadFunctions.cpp bookmarks.cpp gp2xregs.cpp overlay.cpp songDb.cpp keyboard.cpp

OBJ = $(SRC:.cpp=.o) libmpdclient.o
#-------------------------------------------------------------------------------
# Libraries being linked with the application
#-------------------------------------------------------------------------------
LIBS = -lSDL -lSDLmain -lSDL_ttf -lSDL_image -lSDL_gfx pc/libsqlite3.a -ldl 

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

plBrowser.o: plBrowser.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
plBrowser.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
plBrowser.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
plBrowser.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
plBrowser.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
plBrowser.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
plBrowser.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
plBrowser.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
plBrowser.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
plBrowser.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
plBrowser.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
plBrowser.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
plBrowser.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
plBrowser.o: /usr/include/SDL/SDL_ttf.h libmpdclient.h scroller.h
plBrowser.o: threadParms.h commandFactory.h playlist.h config.h timer.h
plBrowser.o: guiPos.h /usr/include/SDL/SDL_image.h
browser.o: browser.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
browser.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
browser.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
browser.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
browser.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
browser.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
browser.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
browser.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
browser.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
browser.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
browser.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
browser.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
browser.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
browser.o: /usr/include/SDL/SDL_ttf.h libmpdclient.h scroller.h threadParms.h
browser.o: commandFactory.h config.h guiPos.h /usr/include/SDL/SDL_image.h
playlist.o: playlist.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
playlist.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
playlist.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
playlist.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
playlist.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
playlist.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
playlist.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
playlist.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
playlist.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
playlist.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
playlist.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
playlist.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
playlist.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
playlist.o: /usr/include/SDL/SDL_ttf.h config.h libmpdclient.h scroller.h
playlist.o: timer.h threadParms.h commandFactory.h popup.h timestamp.h
playlist.o: guiPos.h /usr/include/SDL/SDL_image.h
main.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
main.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
main.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
main.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
main.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
main.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
main.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
main.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
main.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
main.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
main.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
main.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
main.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
main.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_image.h config.h
main.o: browser.h libmpdclient.h scroller.h plBrowser.h bookmarks.h
main.o: playlist.h timer.h albumArt.h threadParms.h overlay.h nowPlaying.h
main.o: threadFunctions.h commandFactory.h statsBar.h helpBar.h popup.h
main.o: gp2xregs.h guiPos.h
config.o: config.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
config.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
config.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
config.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
config.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
config.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
config.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
config.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
config.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
config.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
config.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
config.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
config.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
nowPlaying.o: nowPlaying.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
nowPlaying.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
nowPlaying.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
nowPlaying.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
nowPlaying.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
nowPlaying.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
nowPlaying.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
nowPlaying.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
nowPlaying.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
nowPlaying.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
nowPlaying.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
nowPlaying.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
nowPlaying.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
nowPlaying.o: /usr/include/SDL/SDL_ttf.h libmpdclient.h config.h playlist.h
nowPlaying.o: scroller.h timer.h threadParms.h commandFactory.h guiPos.h
nowPlaying.o: /usr/include/SDL/SDL_image.h
statsBar.o: statsBar.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
statsBar.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
statsBar.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
statsBar.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
statsBar.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
statsBar.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
statsBar.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
statsBar.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
statsBar.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
statsBar.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
statsBar.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
statsBar.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
statsBar.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
statsBar.o: /usr/include/SDL/SDL_ttf.h libmpdclient.h config.h playlist.h
statsBar.o: scroller.h timer.h threadParms.h commandFactory.h
commandFactory.o: commandFactory.h libmpdclient.h threadParms.h
commandFactory.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
commandFactory.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
commandFactory.o: /usr/include/SDL/SDL_platform.h
commandFactory.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
commandFactory.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_error.h
commandFactory.o: /usr/include/SDL/SDL_endian.h /usr/include/SDL/SDL_mutex.h
commandFactory.o: /usr/include/SDL/SDL_thread.h /usr/include/SDL/SDL_rwops.h
commandFactory.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_cpuinfo.h
commandFactory.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
commandFactory.o: /usr/include/SDL/SDL_keyboard.h
commandFactory.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
commandFactory.o: /usr/include/SDL/SDL_video.h
commandFactory.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_quit.h
commandFactory.o: /usr/include/SDL/SDL_loadso.h /usr/include/SDL/SDL_timer.h
commandFactory.o: /usr/include/SDL/SDL_version.h
popup.o: popup.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
popup.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
popup.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
popup.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
popup.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
popup.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
popup.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
popup.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
popup.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
popup.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
popup.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
popup.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
popup.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
popup.o: /usr/include/SDL/SDL_ttf.h libmpdclient.h config.h playlist.h
popup.o: scroller.h timer.h threadParms.h commandFactory.h gp2xregs.h
popup.o: guiPos.h /usr/include/SDL/SDL_image.h
scroller.o: scroller.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
scroller.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
scroller.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
scroller.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
scroller.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
scroller.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
scroller.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
scroller.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
scroller.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
scroller.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
scroller.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
scroller.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
scroller.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
scroller.o: /usr/include/SDL/SDL_ttf.h libmpdclient.h config.h
scroller.o: commandFactory.h /usr/include/SDL/SDL_image.h
timestamp.o: timestamp.h
helpBar.o: helpBar.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
helpBar.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
helpBar.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
helpBar.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
helpBar.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
helpBar.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
helpBar.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
helpBar.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
helpBar.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
helpBar.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
helpBar.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
helpBar.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
helpBar.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
helpBar.o: /usr/include/SDL/SDL_ttf.h libmpdclient.h config.h
helpBar.o: /usr/include/SDL/SDL_image.h
albumArt.o: albumArt.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
albumArt.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
albumArt.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
albumArt.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
albumArt.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
albumArt.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
albumArt.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
albumArt.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
albumArt.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
albumArt.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
albumArt.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
albumArt.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
albumArt.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
albumArt.o: /usr/include/SDL/SDL_ttf.h threadParms.h libmpdclient.h config.h
albumArt.o: threadFunctions.h
threadFunctions.o: threadFunctions.h /usr/include/SDL/SDL.h
threadFunctions.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_stdinc.h
threadFunctions.o: /usr/include/SDL/SDL_config.h
threadFunctions.o: /usr/include/SDL/SDL_platform.h
threadFunctions.o: /usr/include/SDL/begin_code.h
threadFunctions.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
threadFunctions.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
threadFunctions.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
threadFunctions.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
threadFunctions.o: /usr/include/SDL/SDL_cpuinfo.h
threadFunctions.o: /usr/include/SDL/SDL_events.h
threadFunctions.o: /usr/include/SDL/SDL_active.h
threadFunctions.o: /usr/include/SDL/SDL_keyboard.h
threadFunctions.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
threadFunctions.o: /usr/include/SDL/SDL_video.h
threadFunctions.o: /usr/include/SDL/SDL_joystick.h
threadFunctions.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
threadFunctions.o: /usr/include/SDL/SDL_timer.h
threadFunctions.o: /usr/include/SDL/SDL_version.h
threadFunctions.o: /usr/include/SDL/SDL_image.h
threadFunctions.o: /usr/include/SDL/SDL_rotozoom.h threadParms.h
threadFunctions.o: libmpdclient.h
bookmarks.o: bookmarks.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
bookmarks.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
bookmarks.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
bookmarks.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
bookmarks.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
bookmarks.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
bookmarks.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
bookmarks.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
bookmarks.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
bookmarks.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
bookmarks.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
bookmarks.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
bookmarks.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
bookmarks.o: /usr/include/SDL/SDL_ttf.h libmpdclient.h scroller.h
bookmarks.o: threadParms.h commandFactory.h playlist.h config.h timer.h
bookmarks.o: statsBar.h guiPos.h /usr/include/SDL/SDL_image.h
gp2xregs.o: gp2xregs.h
overlay.o: overlay.h /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
overlay.o: /usr/include/SDL/SDL_stdinc.h /usr/include/SDL/SDL_config.h
overlay.o: /usr/include/SDL/SDL_platform.h /usr/include/SDL/begin_code.h
overlay.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_audio.h
overlay.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/SDL_endian.h
overlay.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_thread.h
overlay.o: /usr/include/SDL/SDL_rwops.h /usr/include/SDL/SDL_cdrom.h
overlay.o: /usr/include/SDL/SDL_cpuinfo.h /usr/include/SDL/SDL_events.h
overlay.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
overlay.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
overlay.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_joystick.h
overlay.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_loadso.h
overlay.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_version.h
overlay.o: /usr/include/SDL/SDL_ttf.h libmpdclient.h config.h guiPos.h
overlay.o: commandFactory.h playlist.h scroller.h timer.h
overlay.o: /usr/include/SDL/SDL_image.h
