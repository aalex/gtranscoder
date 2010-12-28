all: movie-transcoder

movie-transcoder: main.c
	gcc -Wall -Werror --pedantic --std=gnu99 main.c `pkg-config --libs --cflags glib-2.0` -o movie-transcoder

clean:
	rm -rf movie-transcoder

