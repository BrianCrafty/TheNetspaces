#include <gtk/gtk.h>
GtkWidget* window;
GtkWidget* button;//tested and great window resizing algorithm with the button
#include <stdlib.h>
//comment it out if you don't want to show any action
#define SHOWSECONDS 1
#ifdef SHOWSECONDS
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
char* strTextSystem;
#endif
void appendToLog(char* strText){
	FILE* logFile=fopen("/home/user/UnicodeBookfyHtmlget.log","a");
	fprintf(logFile,"%s\n",strText);
	fclose(logFile);
}
static void activate(GtkApplication* app,gpointer user_data){
#ifdef SHOWSECONDS
	window=gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window),"UnicodeBookfyHtmlget");
	gtk_window_set_default_size(GTK_WINDOW(window),200,100);
	gtk_widget_show_all(window);
#endif
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

#ifndef SHOWSECONDS
	char* strTextSystem;
#endif
	char* strTextSystemLog;
	size_t intStrTextSystemLength=0;
	size_t intStrTextSystemLogLength=0;
	char strPrefix[]="google-chrome file:///home/user/soft/TheNetspaces/TheNetspaces-master/UnicodeBookfyHtmlget.html?";
	size_t intStrPrefix=strlen(strPrefix);
	int intParseInt=atoi(strText);
	char* strParseInt=(char*)malloc(intStrTextLen*sizeof(char));
	snprintf(strParseInt,intStrTextLen,"%d",intParseInt);
	int blnRecognized=0;
	if(1==strlen(strText)){
		intStrTextSystemLength=intStrTextLen+intStrPrefix+strlen("encode=");
		intStrTextSystemLogLength=intStrTextLen+strlen("encode=");
		strTextSystem=(char*)malloc(intStrTextSystemLength*sizeof(char));
		strTextSystemLog=(char*)malloc(intStrTextSystemLogLength*sizeof(char));
		snprintf(strTextSystem,intStrTextSystemLength,"%sencode=%s",strPrefix,strText);
		snprintf(strTextSystemLog,intStrTextSystemLogLength,"encode=%s",strText);
		blnRecognized=1;
	}
	size_t wcslen;
	wcslen=mbstowcs(NULL,strText,0);
	if(0==blnRecognized&&1==wcslen){
		intStrTextSystemLength=intStrTextLen+intStrPrefix+strlen("encode=");
		intStrTextSystemLogLength=intStrTextLen+strlen("encode=");
		strTextSystem=(char*)malloc(intStrTextSystemLength*sizeof(char));
		strTextSystemLog=(char*)malloc(intStrTextSystemLogLength*sizeof(char));
		snprintf(strTextSystem,intStrTextSystemLength,"%sencode=%s",strPrefix,strText);
		snprintf(strTextSystemLog,intStrTextSystemLogLength,"encode=%s",strText);
		blnRecognized=1;
	}
	if(0==blnRecognized&&0==strcmp(strText,strParseInt)){
		intStrTextSystemLength=intStrTextLen+intStrPrefix+strlen("decode=");
		intStrTextSystemLogLength=intStrTextLen+strlen("decode=");
		strTextSystem=(char*)malloc(intStrTextSystemLength*sizeof(char));
		strTextSystemLog=(char*)malloc(intStrTextSystemLogLength*sizeof(char));
		snprintf(strTextSystem,intStrTextSystemLength,"%sdecode=%s",strPrefix,strText);
		snprintf(strTextSystemLog,intStrTextSystemLogLength,"decode=%s",strText);
		blnRecognized=1;
	}
	if(0==blnRecognized){
		intStrTextSystemLength=intStrTextLen+intStrPrefix+strlen("discreetBreacketAndSetIntSandcorn=");
		intStrTextSystemLogLength=intStrTextLen+strlen("discreetBreacketAndSetInSandcorn=");
		//intStrTextSystemLength=intStrTextLen+intStrPrefix+strlen("discreetBreacket=");
		//intStrTextSystemLogLength=intStrTextLen+strlen("discreetBreacket=");
		strTextSystem=(char*)malloc(intStrTextSystemLength*sizeof(char));
		strTextSystemLog=(char*)malloc(intStrTextSystemLogLength*sizeof(char));
		snprintf(strTextSystem,intStrTextSystemLength,"%sdiscreetBreacketAndSetInSandcorn=%s",strPrefix,strText);
		//snprintf(strTextSystem,intStrTextSystemLength,"%sdiscreetBreacket=%s",strPrefix,strText);
		snprintf(strTextSystemLog,intStrTextSystemLogLength,"discreetBreacketAndSetInSandcorn=%s",strText);
		//snprintf(strTextSystemLog,intStrTextSystemLogLength,"discreetBreacket=%s",strText);
	}
#ifdef SHOWSECONDS
	GtkWidget* grid=gtk_grid_new();
	size_t intStrRecomputedLength=50+intStrTextLen;
	GtkWidget* textView=gtk_text_view_new();
	int MAX_BUTTON_TEXT_SIZE=250;	
	int MAX_TEXT_VIEW_WRAP_TEXT_SIZE=63;	
	//setting for the multiline text with GTK_WRAP_WORD_CHAR
	//int MAX_BIGFONT_TEXT_VIEW_TEXT_SIZE=2500/2;
	int MAX_BIGFONT_TEXT_VIEW_TEXT_SIZE=MAX_TEXT_VIEW_WRAP_TEXT_SIZE;
	if(MAX_BUTTON_TEXT_SIZE<intStrTextLen){
		char* strShorterText=(char*)malloc((MAX_BUTTON_TEXT_SIZE+1)*sizeof(char));
		strncpy(strShorterText,strTextSystem,MAX_BUTTON_TEXT_SIZE);
		strShorterText[MAX_BUTTON_TEXT_SIZE-1]=0;
		button=gtk_button_new_with_label(strShorterText);
		free(strShorterText);
	}else button=gtk_button_new_with_label(strTextSystem);
	if(MAX_TEXT_VIEW_WRAP_TEXT_SIZE<intStrTextLen)
		gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView),GTK_WRAP_WORD_CHAR);
	gtk_grid_attach(GTK_GRID(grid),GTK_WIDGET(button),0,1,1,1);
	/*
	if(24<strlen(strTextSystem)){
		gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView),GTK_WRAP_WORD_CHAR);
	}
	*/
	GtkTextBuffer* textBuffer=gtk_text_buffer_new(NULL);
	gtk_text_buffer_set_text(textBuffer,strTextSystem,strlen(strTextSystem));
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(textView),textBuffer);
	if(MAX_BIGFONT_TEXT_VIEW_TEXT_SIZE>intStrTextSystemLength){
		gtk_widget_override_font(textView,pango_font_description_from_string("DejaVu Sans Mono 30"));
	}
	//gtk_widget_set_size_request(textView,200,100);
	gtk_grid_attach(GTK_GRID(grid),GTK_WIDGET(textView),0,0,1,1);
	GtkWidget* vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,1);
	gtk_container_add(GTK_CONTAINER(vbox),grid);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	gtk_widget_show(window);
	gtk_widget_show(grid);
	gtk_widget_show(vbox);
	gtk_widget_show(textView);
	gtk_widget_show(button);
#endif
#ifndef SHOWSECONDS
	system(strTextSystem);
#endif
	appendToLog(strTextSystemLog);
	free(strTextSystemLog);
	free(strText);
	free(strParseInt);
#ifndef SHOWSECONDS
	free(strTextSystem);
#endif
}
#ifdef SHOWSECONDS
static void handlerHide(int sig, siginfo_t *si, void *uc){
	gtk_widget_hide(button);
	signal(sig, SIG_IGN);
}
static void handlerClose(int sig, siginfo_t *si, void *uc){
	gtk_window_close(GTK_WINDOW(window));
	signal(sig, SIG_IGN);
	system(strTextSystem);
	free(strTextSystem);
}
#endif
int main(int argc,char** argv){
	GtkApplication *app;
	int status;
	gtk_init(&argc,&argv);
	app=gtk_application_new((const gchar*)"org.gtk.UnicodeBookfyHtmlget",G_APPLICATION_NON_UNIQUE);
	g_signal_connect(app,"activate",G_CALLBACK (activate),NULL);
	g_signal_connect(app,"destroy",G_CALLBACK(gtk_main_quit),NULL);
	//clipboard content
	//GtkClipboard* clipboard=gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
	//what is currently selected under the cursor
	GtkClipboard* clipboard=gtk_clipboard_get(GDK_SELECTION_PRIMARY);
	gtk_clipboard_request_text(clipboard,clipboard_callback,NULL);
#ifdef SHOWSECONDS
	/*traditional signaling way to go*/
	//hide
	timer_t timeridHide;
	struct sigevent sevHide;
	struct itimerspec itsHide;
	sigset_t maskHide;
	struct sigaction saHide;

	saHide.sa_flags=SA_SIGINFO;
	saHide.sa_sigaction=handlerHide;
	sigemptyset(&saHide.sa_mask);
	if(sigaction(SIGRTMIN,&saHide,NULL)==-1){
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	sigemptyset(&maskHide);
	sigaddset(&maskHide,SIGRTMIN);
	if(sigprocmask(SIG_SETMASK,&maskHide,NULL)==-1){
		perror("sigprocmask");
		exit(EXIT_FAILURE);
	}

	sevHide.sigev_notify=SIGEV_SIGNAL;
	sevHide.sigev_signo=SIGRTMIN;
	sevHide.sigev_value.sival_ptr=&timeridHide;
	if(timer_create(CLOCK_REALTIME,&sevHide,&timeridHide)==-1){
		perror("timer_create");
		exit(EXIT_FAILURE);
	}

	itsHide.it_value.tv_sec=0;
	itsHide.it_value.tv_nsec=100000000;
	itsHide.it_interval.tv_sec=itsHide.it_value.tv_sec;
	itsHide.it_interval.tv_nsec=itsHide.it_value.tv_nsec;

	if(timer_settime(timeridHide,0,&itsHide,NULL)==-1){
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}
	//close
	timer_t timeridClose;
	struct sigevent sevClose;
	struct itimerspec itsClose;
	sigset_t maskClose;
	struct sigaction saClose;

	saClose.sa_flags=SA_SIGINFO;
	saClose.sa_sigaction=handlerClose;
	sigemptyset(&saClose.sa_mask);
	if(sigaction(SIGRTMIN+1,&saClose,NULL)==-1){
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	sigemptyset(&maskClose);
	sigaddset(&maskClose,SIGRTMIN+1);
	if(sigprocmask(SIG_SETMASK,&maskClose,NULL)==-1){
		perror("sigprocmask");
		exit(EXIT_FAILURE);
	}

	sevClose.sigev_notify=SIGEV_SIGNAL;
	sevClose.sigev_signo=SIGRTMIN+1;
	sevClose.sigev_value.sival_ptr=&timeridClose;
	if(timer_create(CLOCK_REALTIME,&sevClose,&timeridClose)==-1){
		perror("timer_create");
		exit(EXIT_FAILURE);
	}

	itsClose.it_value.tv_sec=SHOWSECONDS;
	itsClose.it_value.tv_nsec=0;
	itsClose.it_interval.tv_sec=itsClose.it_value.tv_sec;
	itsClose.it_interval.tv_nsec=itsClose.it_value.tv_nsec;

	if(timer_settime(timeridClose,0,&itsClose,NULL)==-1){
		perror("timer_settime");
		exit(EXIT_FAILURE);
	}
#endif
	status=g_application_run(G_APPLICATION(app),argc,argv);
	g_object_unref(app);
	return status;
}
