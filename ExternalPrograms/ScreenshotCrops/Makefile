all: screenshotcrops 
screenshotcrops: ScreenshotCrops.c
	gcc `pkg-config --cflags gtk+-2.0` -o ScreenshotCrops ScreenshotCrops.c `pkg-config --libs gtk+-2.0` -lm -Wno-deprecated -Wno-deprecated-declarations
