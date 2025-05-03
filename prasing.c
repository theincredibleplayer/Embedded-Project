#include "prasing.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "UART_CONFIG.h"
#include "stdio.h"
#include "parsing.h"
#include "math.h" // to use mathematical functions to calculate distance
#define pi 3.14159



char GPS_ReqName[]="$GPRMC,";// the log name we want 
char GPS[80];
char GPS_Array[12][15];
char *token;
float My_Longitude, My_Latitude;//longitude and latitude from the gps in float form 
float Loc_Longitude[5], Loc_Latitude[5]; //the lat. & long. of pre-saved locations
char Location_Names[6][20]; // 5 locations indexed from 0 to 4 and a sixth array called "you are far" when no place is in range
char Location_index = 5; //index to choose location ,set initially to 5 which is "you are far"
float R = 6378000; // radius of the globe
float Distance_Arr[5];

//function that takes the gps output and checks GPRMC
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
				uint8_t GPScounter = 0 ;
				Inputchar = UART5_ReceiveChar();
				GPS[GPScounter++]=Inputchar;
				
		}while(Inputchar!='*');
}

// transforms string into elements in an array and we use the data we want 
void GPS_list(){

char No_tokens=0;
	token=strtok(GPS,",");
	do{
		strcpy(GPS_Array[No_tokens],token);
		token=strtok(NULL,",");
		No_tokens++;
	
	}while(token!=NULL);
	
	if(strcmp(GPS_Array[2],"A")==0){
		
	if(strcmp(GPS_Array[4],"N")==0)
		My_Latitude=atof(GPS_Array[3]);
	else
		My_Latitude=-atof(GPS_Array[3]);
	
	if(strcmp(GPS_Array[6],"E")==0)
			My_Longitude=atof(GPS_Array[5]);
	else
		My_Longitude=-atof(GPS_Array[5]);
	
	}

}
// this function is to convert decimal lat. & long. to degrees in case input is already in degrees this func will be neglected
/*float CoorInDegree(float angle)
{
	int degree = (int)angle / 100;
	float minutes = angle - (float)degree * 100;
	return(degree + (minutes / 60));
}*/


// calculates the distance and checks which location is near
void Distance(){
	char i = 0;

	// converting degrees to radians
	float My_Rad_Longitude = My_Longitude * pi / 180;
	float My_Rad_Latitude = My_Latitude * pi / 180;
	while(i < 5){
	float Loc_Rad_Longitude = Loc_Longitude[i] * pi / 180;	
	float Loc_Rad_Latitude = Loc_Latitude[i] * pi / 180;	
	// using Harvsine law
	float a = pow(sin((My_Rad_Latitude-Loc_Rad_Latitude)/2),2) + cos(Loc_Rad_Latitude) * cos(My_Rad_Latitude)*pow(sin((My_Rad_Longitude-Loc_Rad_Longitude)/2),2);
	float c = 2 * atan2(sqrt(a),sqrt(1-a));
	float distance = R * c;
	Distance_Arr[i] = distance;
	if (distance <= 10) Location_index = i;
	}
}


// sends the location to the LCD
/*
void Location_Identification(char index){
 char My_Location = strcpy(My_Location,Location_Names[Location_index]);
 UART0_SendString(My_Location);

} */
