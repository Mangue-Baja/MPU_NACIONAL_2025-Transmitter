#ifndef GPS_H
#define GPS_H

#include <TinyGPSPlus.h>
#include "MPU_defs.h"

void GPS_init(void);
bool gpsInfo(radio_packet_t *data);
bool get_GPS_data(radio_packet_t *data);

// DEBUG FUNCTIONS
void PRINT_GPS_TIME(void);
void PRINT_GPS_DATE(void);

#endif