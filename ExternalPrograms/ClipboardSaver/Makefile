all: show
show: ClipboardSaver.c
	gcc `pkg-config --cflags gtk+-3.0` -o ClipboardSaver ClipboardSaver.c `pkg-config --libs gtk+-3.0` -lm -Wno-deprecated -Wno-deprecated-declarations -lrt
dontshow: ClipboardSaver.c
	gcc `pkg-config --cflags gtk+-3.0` -o ClipboardSaver ClipboardSaver.c `pkg-config --libs gtk+-3.0` -lm -Wno-deprecated -Wno-deprecated-declarations

