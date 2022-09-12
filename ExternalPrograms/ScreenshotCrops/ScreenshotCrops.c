#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
char chrQuotationMark='"';
//MODIFF
char strScreenshotsPath[]="/home/user/Pictures/";
//char strScreenshotsPath[]="/home/user/";
typedef struct ActualState{
	char* strFullDestinationFilename;
	char* strFullSourceFilename;
	char* strFilename;
	char* strDestinationFilename;
	char* strSourceFilename;
	char* strDestinationExtension;
	char* strSourceExtension;
	char* strDestinationDirectories;
	char* strSourceDirectories;
	int intStartX;
	int intStartY;
	int intEndX;
	int intEndY;
	gboolean blnMousePressed;
	int intWidth,intHeight;
	GdkPixbuf* pixbufCleanImage;
	GtkWidget* drawingArea;
	uint uintEventCount;
	gboolean blnRenamed;
}ActualState;
void actualState_setFullFilenames(){

}
typedef struct ScreenshotCrops{
	GtkWidget* window;
	GtkWidget* quickRenameRenameBackButton;
}ScreenshotCrops;
ScreenshotCrops screenshotCrops;
void readOrCreateConfigurationFile(ActualState* actualState){

}
void changeDestinationDirectoryStarted(ActualState* actualState){
}
gboolean on_renamingSchemeChanged(GtkWidget* button,ActualState* actualState){
	printf("renamingSchemeChanged");
}
void renamingSchemeStarted(ActualState* actualState){
	GtkWidget* dialogWindow=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(dialogWindow),"Change Renaming Scheme");
	gtk_window_set_position(GTK_WINDOW(dialogWindow),GTK_WIN_POS_MOUSE);
	g_signal_connect(G_OBJECT(dialogWindow),"destroy",G_CALLBACK(on_renamingSchemeChanged),(gpointer)actualState);
	gtk_widget_show(dialogWindow);
}
void quickRename(GtkWidget* button,ActualState* actualState){
	if(FALSE==actualState->blnRenamed){
		gtk_button_set_label(GTK_BUTTON(button),"Quick rename back to Name.xtn (r)");
		actualState->blnRenamed=TRUE;
	}else{
		gtk_button_set_label(GTK_BUTTON(button),"Quick rename to prefNameSuf.xtn (r)");
		actualState->blnRenamed=FALSE;
	}
}
void deleteImage(char* strFilename){
	char strRmPrefix[]="rm ";
	size_t intStrFilename=strlen(strFilename);
	size_t intStrRm=strlen(strRmPrefix)+intStrFilename+3;
	char* strRm=(char*)malloc(intStrRm*sizeof(char));
	snprintf(strRm,intStrRm,"%s%c%s%c",strRmPrefix,chrQuotationMark,strFilename,chrQuotationMark);
	printf("%s\n",strRm);
	system(strRm);
	free(strRm);
}
void resetImage(ActualState* actualState){
	if(FALSE==actualState->blnMousePressed){
		actualState->intStartX=-1;
		actualState->intStartY=-1;
		actualState->intEndX=-1;
		actualState->intEndY=-1;
		GtkWidget* widget=actualState->drawingArea;
		gdk_draw_pixbuf(GDK_DRAWABLE(widget->window),widget->style->white_gc,actualState->pixbufCleanImage,0,0,0,0,-1,-1,GDK_RGB_DITHER_NONE,0,0);
	}
}
void finalizeImage(ActualState* actualState){
	if(-1!=actualState->intStartX&&-1!=actualState->intStartY&&-1!=actualState->intEndX&&-1!=actualState->intEndY){
		int intStartX=actualState->intStartX;
		int intStartY=actualState->intStartY;
		int intEndX=actualState->intEndX;
		int intEndY=actualState->intEndY;
		int intWidth=intEndX-intStartX;
		int intHeight=intEndY-intStartY;
		int intTmp;

		if(intWidth<0){
			intTmp=intEndX;
			intEndX=intStartX;
			intStartX=intTmp;
			intWidth*=-1;
		}
		if(intHeight<0){
			intTmp=intEndY;
			intEndY=intStartY;
			intStartY=intTmp;
			intHeight*=-1;
		}

		char chrWidth='+',chrHeight='+';
		char strPrefix[]="convert ";
		size_t intStrPrefixLength=strlen(strPrefix);
		char strCrop[]="-crop ";
		size_t intStrCropLength=strlen(strCrop);
		int intStrConvertLength=intStrPrefixLength+2*strlen(actualState->strFilename)+intStrCropLength+50;
		size_t intStrFilename=strlen(actualState->strFilename);
		char* strFinalFilename=(char*)malloc(intStrFilename*sizeof(char));
		snprintf(strFinalFilename,intStrFilename-3,"%s",actualState->strFilename);
		snprintf(strFinalFilename+intStrFilename-4,5,".jpg");
		printf("strFinalFilename=|%s|\n",strFinalFilename);
		char* strConvert=(char*)malloc(intStrConvertLength*sizeof(char));
		//snprintf(strConvert,intStrConvertLength,"%s %c%s%c %s %dx%d%c%d%c%d %c%s%c ",strPrefix,chrQuotationMark,actualState->strFilename,chrQuotationMark,strCrop,intWidth,intHeight,chrWidth,intStartX,chrHeight,intStartY,chrQuotationMark,actualState->strFilename,chrQuotationMark);
		snprintf(strConvert,intStrConvertLength,"%s %c%s%c %s %dx%d%c%d%c%d %c%s%c ",strPrefix,chrQuotationMark,actualState->strFilename,chrQuotationMark,strCrop,intWidth,intHeight,chrWidth,intStartX,chrHeight,intStartY,chrQuotationMark,strFinalFilename,chrQuotationMark);
		printf("%s\n",strConvert);
		system(strConvert);
		if(0!=strcmp(actualState->strFilename,strFinalFilename)){
			char strRmPrefix[]="rm ";
			size_t intStrRm=strlen(strRmPrefix)+intStrFilename+3;
			char* strRm=(char*)malloc(intStrRm*sizeof(char));
			snprintf(strRm,intStrRm,"%s%c%s%c",strRmPrefix,chrQuotationMark,actualState->strFilename,chrQuotationMark);
			printf("%s\n",strRm);
			system(strRm);
			free(strRm);
		}
		free(strConvert);
		free(strFinalFilename);
	}
}
void drawRectangle(GtkWidget* widget,int intStartX,int intStartY,int intEndX,int intEndY,ActualState* actualState){
	GdkGC* gc;	
	gc=gdk_gc_new(GDK_DRAWABLE(actualState->drawingArea->window));
	GdkColor colOrange;
	//colOrange.red=255;
	colOrange.red=65535;
	//colOrange.green=100;
	colOrange.green=25700;
	colOrange.blue=0;
	gdk_gc_set_rgb_fg_color(gc,&colOrange);
	int intWidth=intEndX-intStartX;
	int intHeight=intEndY-intStartY;
	int intTmp;
	if(intWidth<0){
		intTmp=intEndX;
		intEndX=intStartX;
		intStartX=intTmp;
		intWidth*=-1;
	}
	if(intHeight<0){
		intTmp=intEndY;
		intEndY=intStartY;
		intStartY=intTmp;
		intHeight*=-1;
	}
	gdk_draw_rectangle(GDK_DRAWABLE(widget->window),gc,FALSE,intStartX,intStartY,intWidth,intHeight);
}
gboolean on_motion_notify_event(GtkWidget* widget,GdkEventMotion* event, ActualState* actualState){
	actualState->uintEventCount++;
	if(0!=actualState->uintEventCount%10)return TRUE;
	gdk_draw_pixbuf(GDK_DRAWABLE(widget->window),widget->style->white_gc,actualState->pixbufCleanImage,0,0,0,0,-1,-1,GDK_RGB_DITHER_NONE,0,0);
	if(-1!=actualState->intStartX&&-1!=actualState->intStartY)
		drawRectangle(widget,actualState->intStartX,actualState->intStartY,event->x,event->y,actualState);
}
gboolean on_key_press(GtkWidget* widget,GdkEventKey* event,ActualState* actualState){
	switch(event->keyval){
		case GDK_Escape:
			gtk_main_quit();
		break;
		case GDK_Return:
			finalizeImage(actualState);
			gtk_main_quit();
		break;
		case GDK_BackSpace:
			resetImage(actualState);
		break;
		case GDK_Delete:
			deleteImage(actualState->strFilename);
			gtk_main_quit();
		break;
		case GDK_r:
			quickRename(screenshotCrops.quickRenameRenameBackButton,actualState);
		break;
		case GDK_f:
			renamingSchemeStarted(actualState);
		break;
		case GDK_d:
			changeDestinationDirectoryStarted(actualState);
		break;
	}
	return FALSE;
}
gboolean on_button_event(GtkWidget* drawingArea,GdkEventButton* event,ActualState* actualState){
	switch(event->type){
	case GDK_BUTTON_PRESS:
		actualState->intStartX=event->x;
		actualState->intStartY=event->y;
		actualState->blnMousePressed=TRUE;
		break;
	case GDK_BUTTON_RELEASE:
		actualState->intEndX=event->x;
		actualState->intEndY=event->y;
		actualState->blnMousePressed=FALSE;
		GtkWidget* widget=actualState->drawingArea;
		gdk_draw_pixbuf(GDK_DRAWABLE(widget->window),widget->style->white_gc,actualState->pixbufCleanImage,0,0,0,0,-1,-1,GDK_RGB_DITHER_NONE,0,0);
		if(-1!=actualState->intStartX&&-1!=actualState->intStartY&&-1!=actualState->intEndX&&-1!=actualState->intEndY)
			drawRectangle(widget,actualState->intStartX,actualState->intStartY,actualState->intEndX,actualState->intEndY,actualState);

		break;
	case GDK_2BUTTON_PRESS:
		break;
	case GDK_3BUTTON_PRESS:
		break;
	}
	return TRUE;
}
gboolean on_cropButton_clicked_event(GtkWidget* button,ActualState* actualState){
	finalizeImage(actualState);
	gtk_main_quit();
	return TRUE;
}
gboolean on_resetButton_clicked_event(GtkWidget* button,ActualState* actualState){
	resetImage(actualState);
	return TRUE;
}
gboolean on_cancelButton_clicked_event(GtkWidget* button,ActualState* actualState){
	gtk_main_quit();
	return TRUE;
}
gboolean on_deleteButton_clicked_event(GtkWidget* button,ActualState* actualState){
	deleteImage(actualState->strFilename);
	gtk_main_quit();
	return TRUE;
}
gboolean on_quickRenameButton_clicked_event(GtkWidget* button,ActualState* actualState){
	quickRename(button,actualState);
	return TRUE;
}
gboolean on_changeRenamingSchemeButton_clicked_event(GtkWidget* button,ActualState* actualState){
	renamingSchemeStarted(actualState);
	return TRUE;
}
gboolean on_changeDestinationDirectoryButton_clicked_event(GtkWidget* button,ActualState* actualState){
	changeDestinationDirectoryStarted(actualState);
	return TRUE;
}
gboolean on_expose_event(GtkWidget* widget,GdkEventExpose* event,ActualState* actualState){
	int intWidth=gdk_pixbuf_get_width(actualState->pixbufCleanImage);
	int intHeight=gdk_pixbuf_get_height(actualState->pixbufCleanImage);
	gdk_draw_pixbuf(GDK_DRAWABLE(widget->window),widget->style->white_gc,actualState->pixbufCleanImage,event->area.x,event->area.y,event->area.x,event->area.y,intWidth,intHeight,GDK_RGB_DITHER_NONE,0,0);
	if(-1!=actualState->intStartX&&-1!=actualState->intStartY&&-1!=actualState->intEndX&&-1!=actualState->intEndY)
		drawRectangle(widget,actualState->intStartX,actualState->intStartY,actualState->intEndX,actualState->intEndY,actualState);
	return TRUE;
}
char* makeScreenshotAndExtractFilename(){
	printf("1==argc \n");
	char strLsPrefix[]="ls ";
	char strLsPrevFilename[]="lsPrev.txt";
	char strLsNextFilename[]="lsNext.txt";
	int intStrOutputFilenameLength=strlen(strLsPrevFilename);
	int intStrScreenshotsPathLength=strlen(strScreenshotsPath);
	int intStrLsPrefixLength=strlen(strLsPrefix);
	int intStrLsLength=intStrLsPrefixLength+2*intStrScreenshotsPathLength+intStrOutputFilenameLength+8;
	char* strPrevLs=(char*)malloc(intStrLsLength*sizeof(char));
	char* strNextLs=(char*)malloc(intStrLsLength*sizeof(char));
	snprintf(strPrevLs,intStrLsLength,"%s%c%s%c > %c%s%s%c",strLsPrefix,chrQuotationMark,strScreenshotsPath,chrQuotationMark,chrQuotationMark,strScreenshotsPath,strLsPrevFilename,chrQuotationMark);
	snprintf(strNextLs,intStrLsLength,"%s%c%s%c > %c%s%s%c",strLsPrefix,chrQuotationMark,strScreenshotsPath,chrQuotationMark,chrQuotationMark,strScreenshotsPath,strLsNextFilename,chrQuotationMark);
	printf("%s\n",strPrevLs);
	system(strPrevLs);
	printf("gnome-screenshot\n");
	system("gnome-screenshot");
	system(strNextLs);
	printf("%s\n",strNextLs);
	char strDiffPreffix[]="diff ";
	char strLsDiffFilename[]="lsDiff.txt";
	int intStrDiffPreffixLength=strlen(strDiffPreffix);
	int intStrDiff=intStrDiffPreffixLength+3*intStrScreenshotsPathLength+3*intStrOutputFilenameLength+11;
	char* strDiff=(char*)malloc(intStrDiff*sizeof(char));
	snprintf(strDiff,intStrDiff,"%s%c%s%s%c %c%s%s%c > %c%s%s%c",strDiffPreffix,chrQuotationMark,strScreenshotsPath,strLsPrevFilename,chrQuotationMark,chrQuotationMark,strScreenshotsPath,strLsNextFilename,chrQuotationMark,chrQuotationMark,strScreenshotsPath,strLsDiffFilename,chrQuotationMark);
	printf("%s\n",strDiff);
	system(strDiff);
	int intStrDifffilePath=intStrScreenshotsPathLength+intStrOutputFilenameLength+1;
	char* strDifffilePath=(char*)malloc(intStrDifffilePath*sizeof(char));
	snprintf(strDifffilePath,intStrDifffilePath,"%s%s",strScreenshotsPath,strLsDiffFilename);
	printf("fopen(%s \n",strDifffilePath);
	FILE* diffFile=fopen(strDifffilePath,"r");
	size_t intStrLineSize=100;
	char* strLine=(char*)malloc(intStrLineSize*sizeof(char));
	char* strPrevLine=(char*)malloc(intStrLineSize*sizeof(char));
	printf("getline");
	size_t nread=0;
	size_t intPrevNread=0;
	int intLinesRead=0;
	while(-1!=(nread=getline(&strLine,&intStrLineSize,diffFile))){
		printf("Retrieved line of length %zu:\n", nread);
		printf("%s\n",strLine);
		intLinesRead++;
		//MODIFF: filter by the file extension e.x. *.png, not the line number
		//almost sure, that in different systems it will be differently
		if(2==intLinesRead){//dependent on ls and diff
			intPrevNread=nread;
			strncpy(strPrevLine,strLine,intPrevNread);
			printf("strncpy(%s,%s,%ld);",strPrevLine,strLine,intPrevNread);
		}
	}
	printf("EOF, intPrevNread=%ld \n",intPrevNread);
	printf("%s\n",strPrevLine);
	char* strScreenshotFilename=(char*)malloc(intPrevNread*sizeof(char));
	snprintf(strScreenshotFilename,intPrevNread-2,"%s",strPrevLine+2);
	fclose(diffFile);
	printf("filename=|%s|\n",strScreenshotFilename);
	size_t intStrScreenshotPathLength=intPrevNread-2+intStrScreenshotsPathLength;
	char* strScreenshotPath=(char*)malloc(intStrScreenshotPathLength*sizeof(char));
	snprintf(strScreenshotPath,intStrScreenshotPathLength,"%s%s",strScreenshotsPath,strScreenshotFilename);
	printf("screenshot path=|%s|\n",strScreenshotPath);

	char strRmPrefix[]="rm ";
	size_t intStrRm=strlen(strRmPrefix)+intStrScreenshotsPathLength+intStrOutputFilenameLength+3;
	char* strRm=(char*)malloc(intStrRm*sizeof(char));
	snprintf(strRm,intStrRm,"%s%c%s%s%c",strRmPrefix,chrQuotationMark,strScreenshotsPath,strLsPrevFilename,chrQuotationMark);
	printf("%s\n",strRm);
	system(strRm);
	snprintf(strRm,intStrRm,"%s%c%s%s%c",strRmPrefix,chrQuotationMark,strScreenshotsPath,strLsNextFilename,chrQuotationMark);
	printf("%s\n",strRm);
	system(strRm);
	snprintf(strRm,intStrRm,"%s%c%s%s%c",strRmPrefix,chrQuotationMark,strScreenshotsPath,strLsDiffFilename,chrQuotationMark);
	printf("%s\n",strRm);
	system(strRm);
	free(strPrevLs);
	free(strNextLs);
	free(strDiff);
	free(strDifffilePath);
	free(strLine);
	free(strPrevLine);
	free(strScreenshotFilename);
	free(strRm);
	return strScreenshotPath;
}
int main(int argc,char* argv[]){
	gtk_init(&argc,&argv);
	ActualState actualState;
	actualState.intStartX=-1;
	actualState.intStartY=-1;
	actualState.intEndX=-1;
	actualState.intEndY=-1;
	actualState.blnMousePressed=FALSE;
	actualState.uintEventCount=0;
	actualState.blnRenamed=FALSE;
	if(1==argc){
		actualState.strFilename=makeScreenshotAndExtractFilename();
	}else{
		actualState.strFilename=argv[1];
		printf("argc=%d\n",argc);
	}
	GtkWidget* eventbox,*evbButtons;
	GtkWidget* drawingArea;
	GtkWidget* vbox;
	GtkWidget* cropButton,*resetButton,*cancelButton,*deleteButton,*quickRenameRenameBackButton,*changeRenamingSchemeButton,*changeDestinationDirectoryButton;
	actualState.pixbufCleanImage=gdk_pixbuf_new_from_file(actualState.strFilename,NULL);
	
	screenshotCrops.window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(screenshotCrops.window),actualState.strFilename);

	gtk_container_set_border_width(GTK_CONTAINER(screenshotCrops.window),10);
	
	cropButton=gtk_button_new_with_label("Crop&Overwrite (Enter)");
	resetButton=gtk_button_new_with_label("Reset (Backspace)");
	cancelButton=gtk_button_new_with_label("Leave uncropped (Escape)");
	deleteButton=gtk_button_new_with_label("No new file (Delete)");
	screenshotCrops.quickRenameRenameBackButton=gtk_button_new_with_label("Quick rename to prefNameSuf.xtn (r)");
	changeRenamingSchemeButton=gtk_button_new_with_label("Change renaming scheme (f)");
	changeDestinationDirectoryButton=gtk_button_new_with_label("Change destination directory (d)");
	GtkWidget* hbox=gtk_hbox_new(TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox),cropButton,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox),resetButton,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox),cancelButton,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox),deleteButton,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox),screenshotCrops.quickRenameRenameBackButton,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox),changeRenamingSchemeButton,TRUE,TRUE,0);
	gtk_box_pack_start(GTK_BOX(hbox),changeDestinationDirectoryButton,TRUE,TRUE,0);
	
	vbox=gtk_vbox_new(FALSE,0);
	eventbox=gtk_event_box_new();
	evbButtons=gtk_event_box_new();
	//drawingArea=gtk_drawing_area_new();

	/*
	we are using image instead of drawingArea only because 
	it resizes automatically to the image file size when loaded on the form
	*/

	//drawingArea=gtk_image_new();
	drawingArea=gtk_image_new_from_file(actualState.strFilename);
	//gtk_window_set_resizable(GTK_WINDOW(screenshotCrops.window),FALSE);


	actualState.drawingArea=drawingArea;

	GtkWidget *align = gtk_alignment_new(0.5, 0.5, 0, 0);
	gtk_container_add(GTK_CONTAINER(eventbox),drawingArea);
	gtk_container_add(GTK_CONTAINER(align), eventbox);
	gtk_container_add(GTK_CONTAINER(evbButtons),hbox);
	gtk_box_pack_start(GTK_BOX(vbox),evbButtons,TRUE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(vbox),align,FALSE,FALSE,0);
	gtk_container_add(GTK_CONTAINER(screenshotCrops.window),vbox);
	
	g_signal_connect(G_OBJECT(screenshotCrops.window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	g_signal_connect(G_OBJECT(eventbox),"button_press_event",G_CALLBACK(on_button_event),(gpointer)&actualState);
	g_signal_connect(G_OBJECT(eventbox),"button_release_event",G_CALLBACK(on_button_event),(gpointer)&actualState);
	g_signal_connect(G_OBJECT(eventbox),"motion_notify_event",G_CALLBACK(on_motion_notify_event),(gpointer)&actualState);
	g_signal_connect(G_OBJECT(drawingArea),"expose_event",G_CALLBACK(on_expose_event),(gpointer)&actualState);
	g_signal_connect(G_OBJECT(cropButton),"clicked",G_CALLBACK(on_cropButton_clicked_event),(gpointer)&actualState);
	g_signal_connect(G_OBJECT(resetButton),"clicked",G_CALLBACK(on_resetButton_clicked_event),(gpointer)&actualState);
	g_signal_connect(G_OBJECT(cancelButton),"clicked",G_CALLBACK(on_cancelButton_clicked_event),(gpointer)&actualState);
	g_signal_connect(G_OBJECT(deleteButton),"clicked",G_CALLBACK(on_deleteButton_clicked_event),(gpointer)&actualState);
	g_signal_connect(G_OBJECT(screenshotCrops.quickRenameRenameBackButton),"clicked",G_CALLBACK(on_quickRenameButton_clicked_event),(gpointer)&actualState);
	g_signal_connect(G_OBJECT(changeRenamingSchemeButton),"clicked",G_CALLBACK(on_changeRenamingSchemeButton_clicked_event),(gpointer)&actualState);
	g_signal_connect(G_OBJECT(changeDestinationDirectoryButton),"clicked",G_CALLBACK(on_changeDestinationDirectoryButton_clicked_event),(gpointer)&actualState);
	g_signal_connect(G_OBJECT(screenshotCrops.window),"key_press_event",G_CALLBACK(on_key_press),(gpointer)&actualState);
	
	gtk_widget_set_events(eventbox,GDK_EXPOSURE_MASK|GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK|GDK_BUTTON_MOTION_MASK);
	//gtk_window_set_position(GTK_WINDOW(screenshotCrops.window),GTK_WIN_POS_CENTER);
	gtk_widget_show_all(screenshotCrops.window);
	GdkWindow* gdk_window_drawingarea=gtk_widget_get_window(drawingArea);
	GdkWindow* gdk_hbox_window=gtk_widget_get_window(hbox);
	GdkWindow* gdk_vbox_window=vbox->window;
	gdk_window_set_cursor(gdk_window_drawingarea,gdk_cursor_new(GDK_CROSSHAIR));
	gdk_window_set_cursor(gdk_vbox_window,gdk_cursor_new(GDK_LEFT_PTR));
	gdk_window_set_cursor(evbButtons->window,gdk_cursor_new(GDK_RIGHT_PTR));

	gtk_main();
	if(1==argc)
		free(actualState.strFilename);
	return 0;
}
