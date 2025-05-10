#include <stdint.h>

#ifndef parsing
#define parsing

extern char GPS_ReqName[];
extern char GPS[80];
extern char GPS_Array[12][15];
extern char *token;
extern float My_Longitude, My_Latitude;
extern double Loc_Longitude[6], Loc_Latitude[6];
extern char Location_Names[6][20];
extern char Location_index;
extern float R;
extern float Distance_Arr[5];

//----------------------------------------------------
extern int hh,mm,ss;// hours , minutes ,seconds
extern char hours_str[2];
extern char minutes_str[2];
extern char seconds_str[2];
extern char time_str[8];

//--------------------------------------------------------
extern int total_distance;
extern char flag;
extern double old_lat;
extern double old_long;

//-----------------------------------------------------
extern char time_flag;
extern char distance_flag;
void GPS_ReadData();
void GPS_list();
void Distance();
void Location_Identification(char index);
char* Mark_Location(void);
void Mark_Removal(void);

void SendIntToLCD(int num);
void SendDistanceToLCD(float num,uint8_t precision);



#endif