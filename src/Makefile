all: movie-encoder

movie-encoder: main.c
	gcc -Wall -Werror --pedantic --std=gnu99 main.c `pkg-config --libs --cflags glib-2.0` -o movie-encoder

clean:
	rm -rf movie-encoder

