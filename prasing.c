#include "prasing.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "UART_CONFIG.h"
#include "stdio.h"
#include "parsing.h"
#include "math.h" // to use mathematical functions to calculate distance
#define pi 3.14159265359



char GPS_ReqName[]="$GPRMC,";// the log name we want 
char GPS[80];
char GPS_Array[12][15];
char *token;
float My_Longitude, My_Latitude;//longitude and latitude from the gps in float form 
double Loc_Longitude[6]={31.280205285233116, 31.278886300580226, 31.28018823888521, 31.27840725207929 , 31.278885790685383};
double Loc_Latitude[6]= {30.064155268417625,30.064881099300123,30.06527454805054,30.06351032021168,30.064092858202084}; //the lat. & long. of pre-saved locations
char margin_index = 5; //index to tell whether i am near to any of the locations by less than 25 
float R = 6378000; // radius of the globe
float Distance_Arr[5];
int nearest_index;
char Mark_Rejection[] = "Already  Defined";
char Mark_Approval[] = "    Mark Set    ";
char Locations_Lenght = 5;

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
	while(i < Locations_Lenght){
		double Loc_Rad_Longitude = Loc_Longitude[i] * pi / 180;	
		double Loc_Rad_Latitude = Loc_Latitude[i] * pi / 180;	
		// using Harvsine law
		double a = pow(sin((My_Rad_Latitude-Loc_Rad_Latitude)/2),2) + cos(Loc_Rad_Latitude) * cos(My_Rad_Latitude)*pow(sin((My_Rad_Longitude-Loc_Rad_Longitude)/2),2);
		double c = 2 * atan2(sqrt(a),sqrt(1-a));
		float distance = R * c;
		Distance_Arr[i] = distance;
		if (distance < min_dis){
			nearest_index = i ;
			min_dis = distance;
		}
		i++;
	}

	switch (nearest_index)
	{
	case  0 :
		write_LCD_String("    HALL A&B    ",16);
		break;
	case  1 :
		write_LCD_String("  OLD BUILDING  ",16);
		break;
	case  2 :
		write_LCD_String("     LIBRARY    ",16);
		break;
	case  3 :
		write_LCD_String("CREDIT  BUILDING",16);
		break;
	case  4 :
		write_LCD_String("MECHA   WORKSHOP",16);
		break;
	case 5 :
		write_LCD_String("      MARK      ",16);
	default:
		break;
	}
}

char* Mark_Location(void){
	char j;
	char Margin_flag = 1;
	float margin_dis = 0;
	for(j =0; j < strlen(Distance_Arr);j++)
	{
		if(Distance_Arr[j] < 25)
		{
			Margin_flag = 0;
			break;
		}
	}
	if (Margin_flag){
	Loc_Longitude[5] = CoorInDegree(My_Longitude); // marked location Longitude
	Loc_Latitude[5] = CoorInDegree(My_Latitude); // marked location latitude
	Locations_Lenght = 6;
	return Mark_Approval;
	}
	else 
	return Mark_Rejection;
}

void Mark_Removal(void){
	Loc_Latitude[5] = 0;
	Loc_Longitude[5] = 0;
	Locations_Lenght = 5;
}