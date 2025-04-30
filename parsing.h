#ifndef parsing
#define parsing

extern char GPS_ReqName[];
extern char GPS[80];
extern char GPS_Array[20][20];
extern char *token;
extern float My_Longitude, My_Latitude;
extern float Loc_Longitude[5], Loc_Latitude[5];
extern char Location_Names[6][20];
extern char Location_index;
extern float R;
extern float Distance_Arr[5];


void GPS_ReadData();
void GPS_list();



#endif