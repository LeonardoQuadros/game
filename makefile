all: game

game: game.c game.h
	 gcc -Wall game.c -o game $(shell pkg-config allegro-5 allegro_font-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags) 
clean: 
	-rm -f *~ *.o
purge: clean
	-rm -f game
