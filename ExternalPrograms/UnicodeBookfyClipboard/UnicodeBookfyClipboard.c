#include <string.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
double epsilon=0.00001;
struct wideIntArray{
	unsigned int* intArray;
	unsigned int intLength;
};
struct wideIntArray* newWideIntArray(int intLength){
	struct wideIntArray* theArray=(struct wideIntArray*)malloc(sizeof(struct wideIntArray));
	theArray->intArray=(unsigned int*)malloc(intLength*sizeof(unsigned int));
	memset(theArray->intArray,0,intLength*sizeof(unsigned int));
	theArray->intLength=intLength;
	return theArray;
}
struct wideIntArray* deleteWideIntArray(struct wideIntArray* this){
	free(this->intArray);
	free(this);
}
struct theFloatArray{
	float* fltArray;
	unsigned int intLength;
};
struct theFloatArray* newTheFloatArray(int intLength){
	struct theFloatArray* theArray=(struct theFloatArray*)malloc(sizeof(struct theFloatArray));
	theArray->fltArray=(float*)malloc(intLength*sizeof(float));
	theArray->intLength=intLength;
	return theArray;
}
struct wideIntArray* deleteTheFloatArray(struct theFloatArray* this){
	free(this->fltArray);
	free(this);
}
GtkWidget* window;
GtkWidget* textView;
void appendToLog(){
	GtkTextBuffer* textBuffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
	char *strText;
	GtkTextIter start;
	GtkTextIter end;
	gtk_text_buffer_get_start_iter(textBuffer,&start);
	gtk_text_buffer_get_end_iter(textBuffer,&end);
	strText=gtk_text_buffer_get_text(textBuffer,&start,&end,TRUE);
	unsigned int intLength=strlen(strText);
	FILE* logFile=fopen("/home/user/UnicodeBookfyClipboard.log","a");
	fprintf(logFile,"%s",strText);
	fclose(logFile);
	free(strText);
}
gboolean on_key_press(GtkWidget* widget,GdkEventKey* event,gpointer user_data){
	if(event->keyval==GDK_KEY_Escape){
		gtk_window_close(GTK_WINDOW(window));
	}
	if(event->keyval==GDK_KEY_Return){
		appendToLog();
		gtk_window_close(GTK_WINDOW(window));
	}
	return FALSE;
}
static void activate(GtkApplication* app,gpointer user_data){
	window=gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window),"UnicodeBookfyClipboard");
	gtk_window_set_default_size(GTK_WINDOW(window),200,100);
	g_signal_connect(G_OBJECT(window),"key_press_event",G_CALLBACK(on_key_press),NULL);
	gtk_widget_show_all(window);
}
void buttonClicked(GtkWidget* widget,gpointer data){
	appendToLog();
	gtk_window_close(GTK_WINDOW(window));
}
float parseTwoDigitInt(int* strInput,int intStart){
	int int0=(intStart+1>=0)?strInput[intStart+1]:0;
	int int1=(intStart>=0)?10*(strInput[intStart]):0;
	return (float)int1+int0;
}
struct theFloatArray* extractStrings(struct wideIntArray* strInput){
	float fltLength=((float)strInput->intLength)/2.;
	struct theFloatArray* discreetBreacket=newTheFloatArray((unsigned int)ceil(fltLength));
	int theCounter=strInput->intLength-2;
	for(int theInternalCounter=(unsigned int)(ceil(fltLength)-1);theInternalCounter>=0;theInternalCounter--){
		discreetBreacket->fltArray[theInternalCounter]=parseTwoDigitInt(strInput->intArray,theCounter);
		theCounter-=2;
	}
	deleteWideIntArray(strInput);
	return discreetBreacket;
}
struct wideIntArray* encodeData(wchar_t* strText,size_t intStrTextSize){
	unsigned int ii;
	size_t intLen=intStrTextSize;
	unsigned int intCharcode=0;
	int jj=0;
	int intPow,kk,ll,mm;
	int intSize=(int)ceil(log(65535)/log(10));
	int intMax;
	unsigned int* intN=(unsigned int*)malloc(intSize*sizeof(unsigned int));
	struct wideIntArray* theIntText=newWideIntArray(intSize*intLen);
	for(ii=0;ii<intLen;ii++){
		intCharcode=(unsigned int)strText[ii];
		intMax=(int)floor(log(intCharcode)/log(10));	
			for(ll=intMax;ll>=0;ll--){
				intPow=pow(10,ll);
				intN[ll]=(int)floor((double)intCharcode/intPow);
					for(mm=intMax;mm>=ll+1;mm--){
						intN[ll]=intN[ll]-(int)(pow(10,mm)/intPow)*intN[mm];
					}
			}
		for(kk=0;kk<=intMax;kk++){
			theIntText->intArray[jj++]=intN[intMax-kk];
		}
	}
	free(intN);
	theIntText->intLength=jj;
	return theIntText;
}
int maxValue(struct theFloatArray* dB){
	unsigned int theMaxValue=0;
	unsigned int theCounter;
	for(theCounter=0;theCounter<dB->intLength;theCounter++)
	{
		if(dB->fltArray[theCounter]>theMaxValue){
			theMaxValue=dB->fltArray[theCounter];
		}
	}
	return theMaxValue;
}
void flushBraecket(struct theFloatArray* dB,struct wideIntArray* dBrests,unsigned int* dBrestsCounter0,unsigned int* dBrestsCounter1){
	float fltRest=0.;
	unsigned int theCounter=0;
	for(theCounter=0;theCounter<dB->intLength;theCounter++){
		fltRest=dB->fltArray[theCounter]-floor(dB->fltArray[theCounter]);
		if(fltRest>epsilon){
			if(theCounter<dB->intLength-1){
				dB->fltArray[theCounter+1]=dB->fltArray[theCounter+1]+10*pow(10,2-1)*(fltRest);
				dB->fltArray[theCounter]=floor(dB->fltArray[theCounter]);

			}else{
				if(*dBrestsCounter0<16)dBrests->intArray[(*dBrestsCounter0)++]=((int)(10*fltRest))%2;
				(*dBrestsCounter1)++;
				
				dB->fltArray[theCounter]=floor(dB->fltArray[theCounter]);
			}
		}
		if(fltRest<=epsilon){
			if(theCounter>=dB->intLength-1){
				if(*dBrestsCounter0<16)dBrests->intArray[(*dBrestsCounter0)++]=0;
				(*dBrestsCounter1)++;
			}
		}
	}
}
struct wideIntArray* secondStage(struct theFloatArray* dB){
	struct wideIntArray* varResult=newWideIntArray(16+1);
	unsigned int theCounter;
	unsigned int pow1,pow2;
	pow1=1;
	pow2=pow(10,2-1);
	unsigned int dBrestsCounter0=0,dBrestsCounter1=0;
	struct wideIntArray* dBrests=newWideIntArray(16);
	while(maxValue(dB)>epsilon){
		for(theCounter=0;theCounter<dB->intLength;theCounter++)
			dB->fltArray[theCounter]=dB->fltArray[theCounter]/2.;
		flushBraecket(dB,dBrests,&dBrestsCounter0,&dBrestsCounter1);
		for(theCounter=0;theCounter<dB->intLength;theCounter++){
			dB->fltArray[theCounter]=(floor(pow2*dB->fltArray[theCounter]))/pow2;
		}
	}	
	for(theCounter=0;theCounter<dBrests->intLength;theCounter++){
		varResult->intArray[dBrests->intLength-1-theCounter]=dBrests->intArray[theCounter];
	}
	varResult->intArray[16]=dBrestsCounter1;
	deleteTheFloatArray(dB);
	deleteWideIntArray(dBrests);
	return varResult;
}
struct wideIntArray* encode_dB_number(struct wideIntArray* strNumber){
	struct wideIntArray* strResult=secondStage(extractStrings(strNumber));
	struct wideIntArray* arrReturn=newWideIntArray(2);
	arrReturn->intArray[1]=strResult->intArray[16];
	arrReturn->intArray[0]=0;
	for(unsigned int ii=0;ii<16;ii++){
		arrReturn->intArray[0]+=pow(2,ii)*strResult->intArray[16-1-ii];
	}
	deleteWideIntArray(strResult);
	return arrReturn;
}
struct wideIntArray* recompute(char* strText){
	/*text multibyte string*/
	wchar_t* wcs;/*wide character string*/
	wchar_t* wp;
	size_t wcslen;
	wcslen=mbstowcs(NULL,strText,0);
	if(wcslen==(size_t)-1){
		perror("mbstowcs");
		exit(EXIT_FAILURE);
	}
	wcs=(wchar_t*)malloc((wcslen+1)*sizeof(wchar_t));
	if(wcs==NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if(mbstowcs(wcs,strText,wcslen+1)==(size_t)-1){
		perror("mbstowcs");
		exit(EXIT_FAILURE);
	}
	struct wideIntArray* strInput=encodeData(wcs,wcslen+1);
	free(wcs);
	struct wideIntArray* arrReturn=encode_dB_number(strInput);
	return arrReturn;
}
int parseIntegerFromStringInBrackets(char* strText){
	int intStrTextLen=strlen(strText);
	if(intStrTextLen<3)return -1;
	if('('!=strText[0])return -1;
	if(')'!=strText[strlen(strText)-1])return -1;
	char* strSubText=(char*)malloc((intStrTextLen-2+1)*sizeof(char));
	strncpy(strSubText,strText+1,intStrTextLen-2);
	strSubText[intStrTextLen-2]='\0';
	int intParseInt=atoi(strSubText);
	char* strParseInt=(char*)malloc((intStrTextLen-2)*sizeof(char));
	snprintf(strParseInt,intStrTextLen,"%d",intParseInt);
	if(0==strcmp(strSubText,strParseInt)){
		free(strSubText);
		free(strParseInt);
		return intParseInt;
	}
	free(strSubText);
	free(strParseInt);
	return -1;
}
void clipboard_callback(GtkClipboard* clipboard,const gchar* gtext,gpointer data){
	size_t intStrTextLen;
	char* strText;
	if(NULL==gtext){
		strText=(char*)malloc(1*sizeof(char));
		strText[0]=0;
		intStrTextLen=1;
	}else{
		intStrTextLen=strlen(gtext);
		if(0==intStrTextLen){
			strText=(char*)malloc(1*sizeof(char));
			strText[0]=0;
			intStrTextLen=1;
		}else{
			intStrTextLen++;
			strText=(char*)malloc(intStrTextLen*sizeof(char));
			strncpy(strText,gtext,intStrTextLen);
			strText[intStrTextLen-1]=0;
		}
	}
	gtk_window_set_title(GTK_WINDOW(window),"UnicodeBookfyClipboard");
	/*
	GdkGeometry hints;
	hints.base_width=200;
	hints.base_height=100;
	hints.min_width=10;
	hints.min_height=10;
	hints.max_width=gdk_screen_width();
	hints.max_height=gdk_screen_height();
	hints.width_inc=1;
	hints.height_inc=1;
	gtk_window_set_geometry_hints(GTK_WINDOW(window),NULL,&hints,GDK_HINT_MIN_SIZE|GDK_HINT_MAX_SIZE|GDK_HINT_BASE_SIZE|GDK_HINT_RESIZE_INC);
	*/
	GtkWidget* grid=gtk_grid_new();
	GtkWidget* button;
	size_t intStrRecomputedLength=50+intStrTextLen;
	textView=gtk_text_view_new();
	int MAX_BUTTON_TEXT_SIZE=250;	
	int MAX_TEXT_VIEW_WRAP_TEXT_SIZE=63;	
	int MAX_BIGFONT_TEXT_VIEW_TEXT_SIZE=2500/2;
	if(MAX_BUTTON_TEXT_SIZE<intStrTextLen){
		char* strShorterText=(char*)malloc((MAX_BUTTON_TEXT_SIZE+1)*sizeof(char));
		strncpy(strShorterText,strText,MAX_BUTTON_TEXT_SIZE);
		strShorterText[MAX_BUTTON_TEXT_SIZE-1]=0;
		button=gtk_button_new_with_label(strShorterText);
		free(strShorterText);
	}else button=gtk_button_new_with_label(strText);
	if(MAX_TEXT_VIEW_WRAP_TEXT_SIZE<intStrTextLen)
		gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView),GTK_WRAP_WORD_CHAR);
	gtk_grid_attach(GTK_GRID(grid),GTK_WIDGET(button),0,0,1,1);
	/*
	if(24<intStrTextLen){
		gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView),GTK_WRAP_WORD_CHAR);
	}
	*/
	GtkTextBuffer* textBuffer=gtk_text_buffer_new(NULL);
	char* strRecomputed;
	int intRecomputed;
	int integerFromStringInBrackets=parseIntegerFromStringInBrackets(strText);
	struct wideIntArray* arrReturn;
	wchar_t* wideChar0;
	wchar_t* wideChar1;
	if(-1==integerFromStringInBrackets){
		arrReturn=recompute(strText);
		strRecomputed=(char*)malloc(intStrRecomputedLength*sizeof(char));
		wideChar0=(wchar_t*)malloc(2*sizeof(wchar_t));
		wideChar0[0]=(wchar_t)arrReturn->intArray[0];
		wideChar0[1]=0;
		wideChar1=(wchar_t*)malloc(2*sizeof(wchar_t));
		wideChar1[0]=(wchar_t)arrReturn->intArray[1];
		wideChar1[1]=0;
		if(0==wideChar0[0]){
			wideChar0[0]=1;
			wideChar0[1]=0;
		}
		intRecomputed=snprintf(strRecomputed,intStrRecomputedLength,"(%s)==[%d,%ls,%d,%ls]\n",strText,arrReturn->intArray[0],wideChar0,arrReturn->intArray[1],wideChar1);
		if(-1==intRecomputed)
			snprintf(strRecomputed,intStrRecomputedLength,"(%s)==[%d, ,%d,%ls]\n",strText,arrReturn->intArray[0],arrReturn->intArray[1],wideChar1);
	}else{
		wideChar0=(wchar_t*)malloc(2*sizeof(wchar_t));
		wideChar0[0]=(wchar_t)integerFromStringInBrackets;
		wideChar0[1]=0;
		strRecomputed=(char*)malloc(intStrRecomputedLength*sizeof(char));
		intRecomputed=snprintf(strRecomputed,intStrRecomputedLength,"%s==%ls\n",strText,wideChar0);
	}
	gtk_text_buffer_set_text(textBuffer,strRecomputed,strlen(strRecomputed));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(textView),textBuffer);
	gtk_grid_attach(GTK_GRID(grid),GTK_WIDGET(textView),0,1,1,1);
	GtkWidget* vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,1);
	gtk_container_add(GTK_CONTAINER(vbox),grid);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(buttonClicked),"button");

	if(MAX_BIGFONT_TEXT_VIEW_TEXT_SIZE>intStrTextLen){
		/*
		PangoContext* pangoContext=gtk_widget_create_pango_context(textView);
		PangoFontDescription* pangoFontDescription=pango_context_get_font_description(pangoContext);
		char* strPangoFontDescription=pango_font_description_to_string(pangoFontDescription);
		char* strFontSize=strstr(strPangoFontDescription,"1");	
		strFontSize[0]='3';
		strFontSize[1]='0';
		gtk_widget_override_font(textView,pango_font_description_from_string(strPangoFontDescription));
		gtk_widget_override_font(textView,pangoFontDescription);
		//font can be non-scallable, just use something very well known
		*/
		gtk_widget_override_font(textView,pango_font_description_from_string("DejaVu Sans Mono 30"));
	}
	gtk_widget_show_all(window);
	free(wideChar0);
	if(-1==integerFromStringInBrackets)free(wideChar1);
	free(strRecomputed);
	free(strText);
	if(-1==integerFromStringInBrackets)deleteWideIntArray(arrReturn);
}

int main(int argc,char** argv){
	GtkApplication *app;
	int status;
	gtk_init(&argc,&argv);
	app=gtk_application_new((const gchar*)"org.gtk.UnicodeBookfyClipboard",G_APPLICATION_NON_UNIQUE);
	g_signal_connect(app,"activate",G_CALLBACK(activate),NULL);
	g_signal_connect(app,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	//clipboard content
	//GtkClipboard* clipboard=gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	//what is currently selected under the cursor
	GtkClipboard* clipboard=gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	gtk_clipboard_request_text(clipboard,clipboard_callback,NULL);
	status=g_application_run(G_APPLICATION(app),argc,argv);
	g_object_unref(app);
	return status;
}
