all: show
show: UnicodeBookfyHtmlget.c
	gcc `pkg-config --cflags gtk+-3.0` -o UnicodeBookfyHtmlget UnicodeBookfyHtmlget.c `pkg-config --libs gtk+-3.0` -lm -Wno-deprecated -Wno-deprecated-declarations -lrt
dontshow: UnicodeBookfyHtmlget.c
	gcc `pkg-config --cflags gtk+-3.0` -o UnicodeBookfyHtmlget UnicodeBookfyHtmlget.c `pkg-config --libs gtk+-3.0` -lm -Wno-deprecated -Wno-deprecated-declarations
