#include "prasing.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "UART_CONFIG.h"
#include "stdio.h"
#include "parsing.h"
#include "math.h" // to use mathematical functions to calculate distance
#include "LCD.h"

#define pi 3.14159265359



char GPS_ReqName[]="$GPRMC,";// the log name we want 
char GPS[80];
char GPS_Array[12][15];
char *token;
float My_Longitude, My_Latitude;//longitude and latitude from the gps in float form 
double Loc_Longitude[5]={31.280205285233116, 31.278886300580226, 31.28018823888521, 31.27840725207929 , 31.278885790685383};
double Loc_Latitude[5]= {30.064155268417625,30.064881099300123,30.06527454805054,30.06351032021168,30.064092858202084}; //the lat. & long. of pre-saved locations
char Location_index = 5; //index to choose location ,set initially to 5 which is "you are far"
float R = 6378000; // radius of the globe
float Distance_Arr[5];
int nearest_index;
int utc;
//
     int hh=0,mm=0,ss=0;// hours , minutes ,seconds
    char hours_str[2]="";
		char minutes_str[2]="";
		char seconds_str[2]="";

//
int total_distance=0;
char flag=0;
double old_lat;
double old_long;

//function that takes the gps output and checks GPRMC
void GPS_ReadData(){
		char Inputchar;
		char i=0;
		char flag=1;
		uint8_t GPScounter = 0 ;

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
	
	
	if(strcmp(GPS_Array[1],"A")==0){
   char time_str[6] ="";
		
		strcpy(time_str,GPS_Array[0]);

   
		 // Copy substrings
    strncpy(hours_str, time_str, 2);
    strncpy(minutes_str, time_str + 2, 2);
    strncpy(seconds_str, time_str + 4, 2);
    // Convert to integers
     hh = atoi(hours_str)+3;
     mm = atoi(minutes_str);
     ss = atoi(seconds_str);
     hours_str=itoa(hh);
		
		minutes_str=itoa(mm);
   seconds_str=itoa(ss);
		
		
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
// this function is to convert decimal lat. & long. to degrees in case input is already in degrees this func will be neglected
double CoorInDegree(float angle)
{
	int degree = (int)angle / 100;
	double minutes = angle - (double)degree * 100;
	return(degree + (minutes / 60));
}


// calculates the distance and checks which location is near
void Distance(){
	char i =0;
	double min_dis = 10000000;
	// converting degrees to radians
	double My_Rad_Longitude = CoorInDegree(My_Longitude) * pi / 180;
	double My_Rad_Latitude = CoorInDegree(My_Latitude) * pi / 180;
	
		//total distance

	if(flag==0){
	old_lat=My_Rad_Latitude;
	old_long=My_Rad_Longitude;
	flag=1;
	}
	 double a = pow(sin((My_Rad_Latitude - old_lat)/2),2) + cos(old_lat) * cos(My_Rad_Latitude)*pow(sin((My_Rad_Longitude - old_long)/2),2);
		double c = 2 * atan2(sqrt(a),sqrt(1-a));
		total_distance += R * c;
	
	old_lat=My_Rad_Latitude;
	old_long=My_Rad_Longitude;

	
	while(i < 5){
		double Loc_Rad_Longitude = Loc_Longitude[i] * pi / 180;	
		double Loc_Rad_Latitude = Loc_Latitude[i] * pi / 180;	
		// using Harvsine law
		 a = pow(sin((My_Rad_Latitude-Loc_Rad_Latitude)/2),2) + cos(Loc_Rad_Latitude) * cos(My_Rad_Latitude)*pow(sin((My_Rad_Longitude-Loc_Rad_Longitude)/2),2);
		 c = 2 * atan2(sqrt(a),sqrt(1-a));
		float distance = R * c;
		Distance_Arr[i] = distance;
		if (distance < min_dis){
			nearest_index = i ;
			min_dis = distance;
		}
		if (distance <= 10) Location_index = i;
		i++;
	}
//0 is hall a and b
//1 is the same
//2 is old building
//3 is credit building
//4 is library
//5 is mecha workshop
	switch (nearest_index)
	{
	case  0 :
		write_LCD_String("    HALL A&B    ",16);
		Audio_PlayTrack(1);
		break;
	case  1 :
		write_LCD_String("  OLD BUILDING  ",16);
		Audio_PlayTrack(2);
		break;
	case  2 :
		write_LCD_String("     LIBRARY    ",16);
		Audio_PlayTrack(4);
		break;
	case  3 :
		write_LCD_String("CREDIT  BUILDING",16);
		Audio_PlayTrack(3);
		break;
	case  4 :
		write_LCD_String("MECHA   WORKSHOP",16);
		Audio_PlayTrack(5);
		break;
	default:
		break;
	}
}


// sends the location to the LCD
/*
void Location_Identification(char index){
 char My_Location = strcpy(My_Location,Location_Names[Location_index]);
 UART0_SendString(My_Location);s

} */
