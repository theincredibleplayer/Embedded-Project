#include <stdlib.h>
#include <string.h>
#include "UART_CONFIG.h"
#include "stdio.h"

char GPS_ReqName[]="$GPRMC,";
char GPS[80];
char GPS_Array[20][20];
char *token;
float My_Longitude, My_Latitude;

void GPS_ReadData(){
	char Inputchar;
	char i=0;
	char flag=1;
	
	do{
		flag=1;
		for(i=0;i<7;i++){
	if(UART5_ReceiveChar()!=GPS_ReqName[i]){
		flag=0;
		break;
		}
	}
	}while(flag==0);
	
	strcpy(GPS,"");
	
	do{
	char GPScounter=0;
		Inputchar=UART5_ReceiveChar();
		GPS[GPScounter++]=Inputchar;
		
	}while(Inputchar!='*');
	
		
}

void GPS_list(){

char No_tokens=0;
	token=strtok(GPS,",");
	do{
	strcpy(GPS_Array[No_tokens],token);
		token=strtok(NULL,",");
		No_tokens++;
	
	}while(token!=NULL);
	
	if(strcmp(GPS_Array[1],"A")==0){
		
	if(strcmp(GPS_Array[3],"N")==0)
		My_Latitude=atof(GPS_Array[2]);
	else
		My_Latitude=-atof(GPS_Array[2]);
	
	if(strcmp(GPS_Array[5],"E")==0)
			My_Longitude=atof(GPS_Array[4]);
	else
		My_Longitude=-atof(GPS_Array[4]);
	
	}



}














