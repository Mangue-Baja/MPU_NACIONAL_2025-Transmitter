#include "GPS.h"

TinyGPSPlus gps;

void GPS_init()
{
    GPS_uart.begin(GPS_Baud_Rate, SERIAL_8N1, GPS_TX, GPS_RX);
}

bool gpsInfo(radio_packet_t *data)
{
    static bool _g = false;

    while (GPS_uart.available() > 0)
    {
        if (gps.encode(GPS_uart.read()))
            _g = get_GPS_data(data);
    }

    return _g;
}

bool get_GPS_data(radio_packet_t *data)
{

    if (gps.location.isValid())
    {
        data->latitude = gps.location.lat();
        data->longitude = gps.location.lng();

        if (gps.satellites.isValid())
            data->sat = gps.satellites.value();
        else
            data->sat = 0;

        return true;
    }

    //else
    //{
    //    data->latitude = 0;
    //    data->longitude = 0;

        return false;
    //}
}

void PRINT_GPS_TIME(void)
{
    // Time and Data in point 0 of the map
     //(gps.time.isValid() ? Serial.printf("Time: %dh:%dm:%ds\r\n", gps.time.hour(), \
        gps.time.minute(), gps.time.second()) : Serial.println("Time: 0h:0m:0s\r\n"));
}

void PRINT_GPS_DATE(void)
{
    //(gps.date.isValid() ? Serial.printf("Date: %d/%d/%d\r\n", gps.date.day(), gps.date.month(), \
        gps.date.year()) : Serial.println("Date: 0/0/0\r\n"));
}
