#include <stdbool.h>

unsigned char DayOfWeek(unsigned char day, unsigned char month, unsigned char year);
bool DS3231_IsConnected(void);
bool DS3231_Save(unsigned short int Address, void *data, unsigned char DataCount);
bool DS3231_Load(unsigned short int Address, void *data);
bool DS3231_Load_Temp(void *data);
void Save_Time_All(void);
void Save_Time(unsigned char adress);
unsigned char MonthDay(unsigned char month);
bool SunTime(unsigned char hour, unsigned char day, unsigned char month, unsigned char year);
