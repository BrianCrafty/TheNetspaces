#include <string.h>
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//#include <locale.h>
double epsilon=0.00001;
void printfString(unsigned int* strString,int intLen){
	for(int ii=0;ii<intLen;ii++)
		printf("%d",strString[ii]);
	printf("\n");
}
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
	char *text;
	GtkTextIter start;
	GtkTextIter end;
	gtk_text_buffer_get_start_iter (textBuffer,&start);
	gtk_text_buffer_get_end_iter (textBuffer,&end);
	text=gtk_text_buffer_get_text (textBuffer,&start,&end,TRUE);
	unsigned int intLength=strlen(text);
	printf("Savefile: %s\n",text);
	FILE* logFile=fopen("/home/user/UnicodeBookfyClipboard.log","a");
	fprintf(logFile,"%s",text);
	fclose(logFile);
}
gboolean on_key_press(GtkWidget *widget,GdkEventKey *event,gpointer user_data){
	g_print("Key pressed \n");
	if (event->keyval == GDK_KEY_Escape) {
		gtk_window_close((GtkWindow*)window);
		//gtk_main_quit();
	}
	if (event->keyval == GDK_KEY_Return) {
		appendToLog();
		gtk_window_close((GtkWindow*)window);
	}
	return FALSE;
}
static void activate (GtkApplication* app,gpointer user_data)
{
	//printf equivalent in GTK+
	//g_print( "UnicodeBookfyClipboard\n" );
	window=gtk_application_window_new ( app );
	gtk_window_set_title ( GTK_WINDOW ( window ) , "UnicodeBookfyClipboard" );
	gtk_window_set_default_size ( GTK_WINDOW ( window ) , 200 , 100 );
	g_signal_connect (G_OBJECT ( window ) , "key_press_event" , G_CALLBACK ( on_key_press ), NULL );
	gtk_widget_show_all ( window );
}
void buttonClicked(GtkWidget* widget,gpointer data){
	appendToLog();
	//gtk_window_close((GtkWindow*)window);
	gtk_window_close(GTK_WINDOW(window));
}
float parseTwoDigitInt(int* strInput,int intStart){
	int int0=(intStart+1>=0)?strInput[intStart+1]:0;
	int int1=(intStart>=0)?10*(strInput[intStart]):0;
	return (float)int1+int0;
}
//struct wideIntArray* extractStrings(struct wideIntArray* strInput){
struct theFloatArray* extractStrings(struct wideIntArray* strInput){
	printf("005: extractStrings() start\n");
	//struct wideIntArray* discreetBreacket=newWideIntArray(ceil(strInput->intLength/2)+1);
	//struct theFloatArray* discreetBreacket=newTheFloatArray(ceil(strInput->intLength/2)+1);
	float fltLength=((float)strInput->intLength)/2.;
	printf("strInput->intLength=%d\n",strInput->intLength);
	printf("((float)strInput->intLength)/2.=%f\n",fltLength);
	printf("ceil(fltLength)=%f\n",ceil(fltLength));
	printf("(unsigned int)ceil(fltLength)=%d\n",(unsigned int)ceil(fltLength));
	
	//struct theFloatArray* discreetBreacket=newTheFloatArray(ceil(((float)strInput->intLength)/2));
	struct theFloatArray* discreetBreacket=newTheFloatArray((unsigned int)ceil(fltLength));
	//printf("discreetBreacket->intLength=%d \n",(int)ceil(strInput->intLength/2)+1);
	printf("discreetBreacket->intLength=%d \n",discreetBreacket->intLength);
	int theCounter=strInput->intLength-2;
	//for(int theInternalCounter=(int)((ceil(strInput->intLength)/2)-1);theInternalCounter>=0;theInternalCounter--){
	for(int theInternalCounter=(unsigned int)(ceil(fltLength)-1);theInternalCounter>=0;theInternalCounter--){
		//discreetBreacket->intArray[theInternalCounter]=parseTwoDigitInt(strInput->intArray,theCounter);
		discreetBreacket->fltArray[theInternalCounter]=parseTwoDigitInt(strInput->intArray,theCounter);
		printf("theInternalCounter=%d, theCounter=%d, strInput[theCouter]=%d, strInput[theCounter+1]=%d \n",theInternalCounter,theCounter,strInput->intArray[theCounter],strInput->intArray[theCounter+1]);
		theCounter-=2;
	}
	//free(strInput);
	deleteWideIntArray(strInput);
	printf("006: extractStrings() end\n");
	return discreetBreacket;
}
struct wideIntArray* encodeData(wchar_t* strText,size_t intStrTextSize){
	printf("002: encodeData() start\n");
	unsigned int ii;
	//size_t intLen=strlen(strText);
	size_t intLen=intStrTextSize;
	unsigned int intCharcode=0;
	int jj=0;
	int intPow,kk,ll,mm;
	//int intSize=floor(log(65535)/log(10));
	int intSize=(int)ceil(log(65535)/log(10));
	//TODO: intSize in the precomputing loop,jj=0 again
	int intMax;
	unsigned int* intN=(unsigned int*)malloc(intSize*sizeof(unsigned int));
	struct wideIntArray* theIntText=newWideIntArray(intSize*intLen);
	for(ii=0;ii<intLen;ii++){
	//for(ii=intLen-1;ii>=0;ii--){
		intCharcode=(unsigned int)strText[ii];
		printf("intCharCode=%d\n",intCharcode);
		intMax=(int)floor(log(intCharcode)/log(10));	
		//intMax=ceil(log(intCharcode)/log(10));	
			for(ll=intMax;ll>=0;ll--){
				intPow=pow(10,ll);
				intN[ll]=(int)floor((double)intCharcode/intPow);
					for(mm=intMax;mm>=ll+1;mm--){
						intN[ll]=intN[ll]-(int)(pow(10,mm)/intPow)*intN[mm];
					}
			}
		for(kk=0;kk<=intMax;kk++){
			theIntText->intArray[jj++]=intN[intMax-kk];
			//theIntText->intArray[jj]=intN[intMax-kk];
			//printf("jj=%d theIntText->intArray[jj]=%d\n",jj,theIntText->intArray[jj]);
			//jj++;
		}
	}
	free(intN);
	theIntText->intLength=jj;
	printf("003: encodeData() end\n");
	return theIntText;
}
int maxValue(struct theFloatArray* dB){
	unsigned int theMaxValue=0;
	unsigned int theCounter;
	for (theCounter=0;theCounter<dB->intLength; theCounter++)
	{
		if (dB->fltArray[theCounter]>theMaxValue){
			theMaxValue=dB->fltArray[theCounter];
		}
	}
	return theMaxValue;
}
//void flushBraecket(struct wideIntArray* dB, struct wideIntArray* dBrests,int* dBrestsCounter0){
void flushBraecket(struct theFloatArray* dB, struct wideIntArray* dBrests,unsigned int* dBrestsCounter0,unsigned int* dBrestsCounter1){
	float fltRest=0.;
	unsigned int theCounter=0;
	for (theCounter=0;theCounter<dB->intLength; theCounter++){
		//fltRest=dB->intArray[theCounter]-floor(dB->intArray[theCounter]);
		fltRest=dB->fltArray[theCounter]-floor(dB->fltArray[theCounter]);
		if (fltRest>epsilon)
		{
			if(theCounter<dB->intLength-1){
				//dB->intArray[theCounter+1]=dB->intArray[theCounter+1]+10*pow(10,2-1)*(fltRest);
				dB->fltArray[theCounter+1]=dB->fltArray[theCounter+1]+10*pow(10,2-1)*(fltRest);
				//dB->intArray[theCounter]=floor(dB->intArray[theCounter]);
				dB->fltArray[theCounter]=floor(dB->fltArray[theCounter]);
				//printf("flushBraecket0\n");

			}else{
				if(*dBrestsCounter0<16)dBrests->intArray[(*dBrestsCounter0)++]=((int)(10*fltRest))%2;
				//->16dBrests->intArray[(*dBrestsCounter0)++]=((int)(10*fltRest))%2;
				(*dBrestsCounter1)++;
				
				//dB->intArray[theCounter]=floor(dB->intArray[theCounter]);
				dB->fltArray[theCounter]=floor(dB->fltArray[theCounter]);
				//printf("flushBraecket1\n");
			}
		}
		if(fltRest<=epsilon){
			if (theCounter>=dB->intLength-1){
				//dBrests->intArray[dBrests->intLength]=0;
				if(*dBrestsCounter0<16)dBrests->intArray[(*dBrestsCounter0)++]=0;
				//->16dBrests->intArray[(*dBrestsCounter0)++]=0;
				(*dBrestsCounter1)++;
				//printf("flushBraecket2\n");
			}
		}
	}
}
struct wideIntArray* secondStage(struct theFloatArray* dB){
	printf("007: secondStage() start\n");
	printf("extractStrings: \n");
	for(unsigned int ii=0;ii<dB->intLength;ii++){
		printf("%f, ",dB->fltArray[ii]);
	}
	printf("\n");
	printf("007.25: secondStage() after dB written out\n");

	struct wideIntArray* varResult=newWideIntArray(16+1);
	unsigned int theCounter;
	unsigned int pow1,pow2;
	pow1=1;
	pow2=pow(10,2-1);
	unsigned int dBrestsCounter0=0,dBrestsCounter1=0;
	printf("007.25+0.125-0.125/2: secondStage() before dBrests alocated\n");
	struct wideIntArray* dBrests=newWideIntArray(16);
	printf("007.25+0.125: secondStage() after dBrests alocated\n");
	//struct wideIntArray* dBrests=newWideIntArray(dB->intLength);
	//->16struct wideIntArray* dBrests=newWideIntArray(215);
	printf("007.5: secondStage() before loop\n");
	while(maxValue(dB)>epsilon){
		for(theCounter=0;theCounter<dB->intLength;theCounter++)
			//dB->intArray[theCounter]=dB->intArray[theCounter]/2.;
			dB->fltArray[theCounter]=dB->fltArray[theCounter]/2.;
		flushBraecket(dB,dBrests,&dBrestsCounter0,&dBrestsCounter1);
		for (theCounter=0;theCounter<dB->intLength; theCounter++){
				/*
			if (theCounter>=dB-<intLength-1){
				if(dBrestsCounter<16)
					varResult->intArray[dBrestsCounter++]=dBrests[dBrests->intLength-1];
				dB->intArray[theCounter]=(Math.floor(pow1*dB->intArray[theCounter]))/pow1;
			}
				*/
			//dB->intArray[theCounter]=(floor(pow2*dB->intArray[theCounter]))/pow2;
			dB->fltArray[theCounter]=(floor(pow2*dB->fltArray[theCounter]))/pow2;
		}
		//printf("dBrests: dBrestsCounter0=%d \n",dBrestsCounter0);
		printf("dBrests: dBrestsCounter1=%d \n",dBrestsCounter1);
		for(unsigned int ii=0;ii<dBrests->intLength;ii++){
			printf("%d",dBrests->intArray[ii]);
		}
		printf("\n");

	}	
	for(theCounter=0;theCounter<dBrests->intLength;theCounter++){
		varResult->intArray[dBrests->intLength-1-theCounter]=dBrests->intArray[theCounter];
	}

	printf("secondStage dBrests: \n");
	for(unsigned int ii=0;ii<dBrests->intLength;ii++){
		printf("%d, ",dBrests->intArray[ii]);
	}
	printf("\n");
	varResult->intArray[16]=dBrestsCounter1;
	printf("008: secondStage() end\n");
	deleteTheFloatArray(dB);
	deleteWideIntArray(dBrests);
	return varResult;
}
struct wideIntArray* encode_dB_number(struct wideIntArray* strNumber){
	printf("004: encode_dB_number() start\n");
	struct wideIntArray* strResult=secondStage(extractStrings(strNumber));
	printf("encode_dB_number: \n");
	printfString(strResult->intArray,strResult->intLength);
	printf("secondStage: \n");
	for(unsigned int ii=0;ii<strResult->intLength-1;ii++){
		printf("%d, ",strResult->intArray[ii]);
	}
	printf("\n");
	printf("binary length=%d \n",strResult->intArray[16]);
	struct wideIntArray* arrReturn=newWideIntArray(2);
	arrReturn->intArray[1]=strResult->intArray[16];
	arrReturn->intArray[0]=0;
	for(unsigned int ii=0;ii<16;ii++){
		arrReturn->intArray[0]+=pow(2,ii)*strResult->intArray[16-1-ii];
	}

	//free(strResult);
	deleteWideIntArray(strResult);
	//return strResult;
	return arrReturn;
	
}
struct wideIntArray* recompute(char* text){
	printf("001: recomptue() start\n");
	wchar_t *wcs;/*Wide character string*/
	wchar_t *wp;
	size_t mbslen;
	mbslen=mbstowcs(NULL,text,0);
	if(mbslen==(size_t)-1){
		perror("mbstowcs");
		exit(EXIT_FAILURE);
	}
	//wcs=calloc(mbslen+1,sizeof(wchar_t));
	wcs=(wchar_t*)malloc((mbslen+1)*sizeof(wchar_t));
	if(wcs==NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	if(mbstowcs(wcs,text,mbslen+1)==(size_t)-1){
		perror("mbstowcs");
		exit(EXIT_FAILURE);
	}

	
	//struct wideIntArray* strInput=encodeData(text);
	printf("mbslen+1=%ld\n",mbslen+1);
	printf("wcs=%ls\n",wcs);
	struct wideIntArray* strInput=encodeData(wcs,mbslen+1);
	printf("encodeData: \n");
	for(unsigned int ii=0;ii<strInput->intLength;ii++){
		printf("%d, ",strInput->intArray[ii]);
	}
	printf("\n");
	/*
	strInput=extractStrings(strInput);
	printf("extractStrings: \n");
	for(int ii=0;ii<strInput->intLength;ii++){
		printf("%d, ",strInput->intArray[ii]);
	}
	printf("\n");
	*/
	//free(strInput);
	free(wcs);
	struct wideIntArray* arrReturn=encode_dB_number(strInput);
	return arrReturn;
}
void clipboard_callback(GtkClipboard* clip,const gchar* gtext,gpointer data){
	printf("clipboard_callback function\n");
	size_t intStrTextLen;
	char* text;
	if(NULL==gtext){
		printf("Incorrect gtext:%s\n",gtext);
		text=(char*)malloc(1*sizeof(char));
		text[0]=0;
		//strncpy(text,"",1);
		intStrTextLen=1;
	}else{
		intStrTextLen=strlen(gtext);
		printf("intStrTextLen=strlen(gtext)==%ld\n",intStrTextLen);
		if(0==intStrTextLen){
			text=(char*)malloc(1*sizeof(char));
			//strncpy(text,"",1);
			text[0]=0;
			intStrTextLen=1;
		}else{
			printf("Correct gtext:%s, strlen(gtext)=%ld\n",gtext,strlen(gtext));
			intStrTextLen++;
			text=(char*)malloc(intStrTextLen*sizeof(char));
			strncpy(text,gtext,intStrTextLen);
			text[intStrTextLen]=0;
		}
	}
	printf("text in clipboard:\n");
	printf("%s\n",text);
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
	printf("a tu się pewnie jeszcze nie speniał?\n");
	textView=gtk_text_view_new();
	printf("to pewnie tu się speniał?\n");
	int MAX_BUTTON_TEXT_SIZE=250;	
	int MAX_TEXT_VIEW_WRAP_TEXT_SIZE=63;	
	int MAX_BIGFONT_TEXT_VIEW_TEXT_SIZE=2500/2;
	if(MAX_BUTTON_TEXT_SIZE<intStrTextLen){
		char* strShorterText=(char*)malloc((MAX_BUTTON_TEXT_SIZE+1)*sizeof(char));
		strncpy(strShorterText,text,MAX_BUTTON_TEXT_SIZE);
		strShorterText[MAX_BUTTON_TEXT_SIZE-1]=0;
		button=gtk_button_new_with_label(strShorterText);
		printf("before strShorterText removal \n");
		free(strShorterText);
	}else button=gtk_button_new_with_label(text);
	if(MAX_TEXT_VIEW_WRAP_TEXT_SIZE<intStrTextLen)
		gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView),GTK_WRAP_WORD_CHAR);
	gtk_grid_attach(GTK_GRID(grid),GTK_WIDGET(button),0,0,1,1);
	/*
	if(24<strlen(text)){
		gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView),GTK_WRAP_WORD_CHAR);
	}
	*/
	GtkTextBuffer* textBuffer=gtk_text_buffer_new(NULL);
	//gtk_text_buffer_set_text(textBuffer,"Recomputing",strlen("Recomputing"));
	struct wideIntArray* arrReturn=recompute((char*)text);	
	char* strRecomputed=(char*)malloc(intStrRecomputedLength*sizeof(char));
	wchar_t* wideChar0=(wchar_t*)malloc(2*sizeof(wchar_t));
	wideChar0[0]=(wchar_t)arrReturn->intArray[0];
	wideChar0[1]=0;
	wchar_t* wideChar1=(wchar_t*)malloc(2*sizeof(wchar_t));
	wideChar1[0]=(wchar_t)arrReturn->intArray[1];
	wideChar1[1]=0;
	printf("wideChar0[0]=%d\n",(unsigned int)wideChar0[0]);
	if(0==wideChar0[0]){
		printf("0==wideChar0[0]");
		wideChar0[0]=1;
		wideChar0[1]=0;
	}
	int intRecomputed=snprintf(strRecomputed,intStrRecomputedLength,"(%s)==[%d,%ls,%d,%ls]\n",text,arrReturn->intArray[0],wideChar0,arrReturn->intArray[1],wideChar1);
	if(-1==intRecomputed)
		snprintf(strRecomputed,intStrRecomputedLength,"(%s)==[%d, ,%d,%ls]\n",text,arrReturn->intArray[0],arrReturn->intArray[1],wideChar1);
	printf("intRecomputed=%d\n",intRecomputed);
	printf("strRecomputed:\n");
	printf("%s\n",strRecomputed);
	gtk_text_buffer_set_text(textBuffer,strRecomputed,strlen(strRecomputed));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(textView),textBuffer);
	gtk_grid_attach(GTK_GRID(grid),GTK_WIDGET(textView),0,1,1,1);
	GtkWidget* vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,1);
	gtk_container_add (GTK_CONTAINER (vbox),grid);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(buttonClicked),"button");

	if(MAX_BIGFONT_TEXT_VIEW_TEXT_SIZE>intStrTextLen){
		/*
		PangoContext* pangoContext=gtk_widget_create_pango_context(textView);
		PangoFontDescription* pangoFontDescription=pango_context_get_font_description(pangoContext);
		printf("pangoFontDescription=%s\n",pango_font_description_to_string(pangoFontDescription));
		char* strPangoFontDescription=pango_font_description_to_string(pangoFontDescription);
		char* strFontSize=strstr(strPangoFontDescription,"1");	
		printf("strFontSize=%s\n",strFontSize);
		strFontSize[0]='3';
		strFontSize[1]='0';
		printf("changed strPangoFontDescription=%s\n",strPangoFontDescription);
		gtk_widget_override_font(textView,pango_font_description_from_string(strPangoFontDescription));
		gtk_widget_override_font(textView,pangoFontDescription);
		//font can be non-scallable, just use something very well known
		*/
		gtk_widget_override_font(textView,pango_font_description_from_string("DejaVu Sans Mono 30"));
	}
	gtk_widget_show_all(window);
	free(wideChar0);
	free(wideChar1);
	free(strRecomputed);
	free(text);
	deleteWideIntArray(arrReturn);
	//gtk_main_quit();
}

int main(int argc,char** argv){
	GtkApplication *app;
	int status;
	gtk_init(&argc,&argv);
	app=gtk_application_new((const gchar*)"org.gtk.UnicodeBookfyClipboard",G_APPLICATION_NON_UNIQUE);
	g_signal_connect(app,"activate",G_CALLBACK (activate),NULL);
	g_signal_connect(app,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	//clipboard content
	//GtkClipboard* clip=gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	//what is currently selected under the cursor
	GtkClipboard* clip=gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	gtk_clipboard_request_text(clip,clipboard_callback,NULL);
	status=g_application_run(G_APPLICATION(app),argc,argv);
	g_object_unref (app);
	return status;
}
