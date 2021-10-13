/**
* Insert hours of the day. Part of the calendar extension.
* Default commandline call:
*	insertDay
* inserts hours from 00:00 to 23:00.
* With optional arguments:
*	insertDay intMinH intMaxH
* it inserts hours from intMinH to intMaxH, also if intMinH>intMaxH - then the next day hours are inserted.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#define LINEMAX 100
/*
Example of the structural object oriented programming.
That is small task suitable for procedural programming - structural object oriented approach for demonstrative purposes, ideal for bigger projects. Since C often bits the electronics so weLL - and oop is what you often miss the most. Also on the water pipes in the future?
*/
typedef struct Calendar {
	int intMinH;
	int intMaxH;
}Calendar;
Calendar* newCalendar(int intMinH, int intMaxH){
	/*Or you prefer mallocCalendar?*/
	Calendar* that=(Calendar*)malloc(sizeof(Calendar));
	that->intMinH=intMinH;
	that->intMaxH=intMaxH;
	return that;
}
void deleteCalendar(Calendar* that){
	/*Or you prefer freeCalendar?*/
	free(that);
}
int Calendar_insertGreatDay(Calendar* that,int intFrom,int intTo){
	/*habitual that unused here*/
	char strLine[LINEMAX];
	for(int h=intFrom;h<intTo+1;h++)
		printf("%.2d:00 \n",h);
}
void Calendar_insertDay(Calendar* that){
	if(that->intMinH<=that->intMaxH)
		Calendar_insertGreatDay(that,that->intMinH,that->intMaxH);
	else{
		Calendar_insertGreatDay(that,that->intMinH,23);
		Calendar_insertGreatDay(that,0,that->intMaxH);
	}
}
int main(int argc, char* argv[]){
	int intMinH=0, intMaxH=23;
	char* endptrMin,*endptrMax;
	if(3==argc){
		errno=0;
           	intMinH=strtol(argv[1],&endptrMin,10);
           	/* Check for various possible errors */
           	if((errno==ERANGE&&(intMinH==LONG_MAX||intMinH==LONG_MIN))||(errno!=0&&intMinH==0)){
			intMinH=0;
           	}
           	if(endptrMin == argv[1]){
			intMinH=0;
           	}
		errno=0;
           	intMaxH=strtol(argv[2],&endptrMax,10);
           	if((errno==ERANGE&&(intMaxH==LONG_MAX||intMaxH==LONG_MIN))||(errno!=0&&intMaxH==0)){
			intMaxH=0;
           	}
           	if (endptrMax==argv[2]){
			intMaxH=23;
           	}
	}
	Calendar* calDejt=newCalendar(intMinH,intMaxH);
	Calendar_insertDay(calDejt);
	deleteCalendar(calDejt);
	return 0;
}
