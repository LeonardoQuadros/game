compile com gcc game.c -o game $(pkg-config allegro-5 allegro_font-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags)
