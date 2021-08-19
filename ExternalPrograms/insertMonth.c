#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#define LINEMAX 100
#define MINYEAR 1900
/*
Example of the structural object oriented programming.
That is small task suitable for procedural programming - structural object oriented approach for demonstrative purposes, ideal for bigger projects. Since C often bits the electronics so weLL - and oop is what you often miss the most. Also on the water pipes in the future?
*/
typedef struct Calendar {
	int intMonth;
	int intYear;
}Calendar;
Calendar* newCalendar(int intMonth, int intYear){
	/*Or you prefer mallocCalendar?*/
	Calendar* that=(Calendar*)malloc(sizeof(Calendar));
	that->intMonth=intMonth;
	that->intYear=intYear;
	return that;
}
void deleteCalendar(Calendar* that){
	/*Or you prefer freeCalendar?*/
	free(that);
}
int Calendar_getLength(Calendar* that,char* strInput){
/*Could be external universal function, but to hold conceptually functional code as an object.*/
	for(int ii=0;ii<LINEMAX;ii++){
		if('\0'==strInput[ii])return ii;
	}
	return -1;
}
struct tm* Calendar_getDate_(Calendar* that,struct tm* tmInput){
	time_t t=mktime(tmInput);
	struct tm *tmDate=localtime(&t);
	return tmDate;
}
struct tm* Calendar_getDate(Calendar* that,int intMonthDay){
	struct tm tmDate={0};
   	tmDate.tm_sec=0;    /* Seconds (0-60) */
        tmDate.tm_min=0;    /* Minutes (0-59) */
        tmDate.tm_hour=0;   /* Hours (0-23) */
        tmDate.tm_mday=intMonthDay;   /* Day of the month (1-31) */
        tmDate.tm_mon=that->intMonth;    /* Month (0-11) */
        tmDate.tm_year=that->intYear;   /* Year - 1900 */
        /*tm_wday;*/   /* Day of the week (0-6, Sunday = 0) */
        /*tm_yday;*/   /* Day in the year (0-365, 1 Jan = 0) */
        /*tm_isdst;*/  /* Daylight saving time */
	struct tm* tmReturn=Calendar_getDate_(that,&tmDate);
	return tmReturn;
}
int Calendar_getLastDay(Calendar* that){
	struct tm* tmDate=Calendar_getDate(that,31);
	int intLastDay=(that->intMonth==(tmDate->tm_wday))?31:30;
	if(2==that->intMonth)intLastDay=28;
	return intLastDay;
}
void Calendar_getWeek(Calendar* that,int intMonthDay,char* strInchySpaces,char* strLine){
	char strWeek[LINEMAX];
	strncpy(strWeek,strInchySpaces,LINEMAX);
	struct tm* dateCounter=Calendar_getDate(that,intMonthDay);
	int intWeekDay=dateCounter->tm_wday;
	int intMonthDayCounter=intMonthDay-intWeekDay;
 	int intInchySpaces=Calendar_getLength(that,strInchySpaces);
	int d,intOffset=intInchySpaces,intEntrySize=1+2+intInchySpaces;
	char strEntry[intEntrySize+1];
	for(d=0;d<intWeekDay;d++){
		dateCounter=Calendar_getDate(that,intMonthDayCounter);
		snprintf(strWeek+intOffset,(size_t)(intEntrySize+1)," %.2d%s ",dateCounter->tm_mday,strInchySpaces);
		intOffset+=intEntrySize;
		intMonthDayCounter++;
	}
	for(d=intWeekDay;d<7;d++){
		dateCounter=Calendar_getDate(that,intMonthDayCounter);
		snprintf(strWeek+intOffset,(size_t)(intEntrySize+1)," %.2d%s ",dateCounter->tm_mday,strInchySpaces);
		intOffset+=intEntrySize;
		intMonthDayCounter++;
	}
	strncpy(strLine,strWeek,LINEMAX);
};
void Calendar_insertMonth(Calendar* that){
	int intWeeks=5;
	struct tm* tmDate=Calendar_getDate(that,1);
	if(1==that->intMonth&&0==tmDate->tm_wday)intWeeks=4;
	char strInchySpaces[LINEMAX];
	snprintf(strInchySpaces,(size_t)4,"   ");
	char strLine[LINEMAX];
	snprintf(strLine,(size_t)LINEMAX,"%sSun%sMon%sTue%sWen%sThu%sFri%sSat%s",strInchySpaces,strInchySpaces,strInchySpaces,strInchySpaces,strInchySpaces,strInchySpaces,strInchySpaces,strInchySpaces);
	printf("%s\n",strLine);
	int intMonthDay=tmDate->tm_mday;
	int intWeekDay;
	for(int w=0;w<intWeeks;w++){
		tmDate=Calendar_getDate(that,intMonthDay);
		intWeekDay=tmDate->tm_wday;
		Calendar_getWeek(that,intMonthDay,strInchySpaces,strLine);
		printf("%s\n",strLine);
		intMonthDay=intMonthDay+7;
	}
}
int main(int argc, char* argv[]){
	time_t t=time(0);
	struct tm* tmNow=localtime(&t);
	/*
	struct tm* tmNow=(struct tm*)malloc(sizeof(struct tm));
	tmNow=localtime_r(&t,tmNow);
	*/
	int intNowMonth=tmNow->tm_mon,intNowYear=tmNow->tm_year;
	//free(tmNow);
	int intMonth=intNowMonth, intYear=intNowYear;
	char* endptrMonth,*endptrYear;
	int intError=0;
	if(3==argc){
		errno=0;
           	intMonth=strtol(argv[1],&endptrMonth,10);
           	/* Check for various possible errors */
           	if((errno==ERANGE&&(intMonth==LONG_MAX||intMonth==LONG_MIN))||(errno!=0&&intMonth==0)){
			intMonth=intNowMonth;
			intError=1;
           	}
           	if(endptrMonth== argv[1]){
			intMonth=intNowMonth;
			intError=1;
           	}
		if(0==intError)intMonth-=1;
		intError=0;
		errno=0;
           	intYear=strtol(argv[2],&endptrYear,10);
           	if((errno==ERANGE&&(intYear==LONG_MAX||intYear==LONG_MIN))||(errno!=0&&intYear==0)){
			intYear=intNowYear;
			intError=1;
           	}
           	if (endptrYear==argv[2]){
			intYear=intNowYear;
			intError=1;
           	}
		if(0==intError)intYear-=MINYEAR;
	}
	Calendar* calDejt=newCalendar(intMonth,intYear);
	Calendar_insertMonth(calDejt);
	deleteCalendar(calDejt);
	return 0;
}
