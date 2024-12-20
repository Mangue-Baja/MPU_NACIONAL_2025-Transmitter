#include "StateMachine.h"

CircularBuffer<state_t, BUFFER_SIZE> state_buffer;
Ticker ticker1Hz;

radio_packet_t volatile_packet;
state_t current_state = IDLE_ST;
uint8_t count_for_gps = 0;
bool buffer_full = false, g = false;

void CircularBuffer_Ticker_Init()
{
    memset(&volatile_packet, 0, sizeof(radio_packet_t));
    ticker1Hz.attach(1.0, ticker1HzISR);
}

void CircularBuffer_CurrentState()
{
    if (state_buffer.isFull())
    {
        // buffer_full = true;
        current_state = state_buffer.pop();
    }
    else
    {
        // buffer_full = false;
        if (!state_buffer.isEmpty())
            current_state = state_buffer.pop();
        else
            current_state = IDLE_ST;
    }

    switch (current_state)
    {
    case IDLE_ST:
        // Serial.println("i");
        break;

    case RADIO_ST:
        //Serial.println("Radio");

        LORA_SendStruct(&volatile_packet, sizeof(volatile_packet));
        // if(LORA_SendStruct(&volatile_packet, sizeof(volatile_packet)))
        // Serial.println("ok");

        break;

    case GPS_ST:
        //Serial.println("GPS");

        g = gpsInfo(&volatile_packet);

        /* Send latitude message */
        if (Send_GPS_data(volatile_packet.latitude, LAT_ID))
        {
            /* Send longitude message if latitude is successful */
            Send_GPS_data(volatile_packet.longitude, LNG_ID);
        }

        break;

    case DEBUG_ST:
        // Serial.println("Debug state");
        // Serial.printf("Latitude (LAT) = %lf\r\n", volatile_packet.latitude);
        // Serial.printf("Longitude (LNG) = %lf\r\n", volatile_packet.longitude);

        // PRINT_GPS_TIME();

        // PRINT_GPS_DATE();

        // Serial.printf("Satellites = %d\r\n", volatile_packet.sat);
        // Serial.println("\n\n");
        break;
    }
}

void ticker1HzISR()
{
    count_for_gps++;

    state_buffer.push(RADIO_ST);

    if (count_for_gps == 4) // 4 seconds
    {
        count_for_gps = 0;
        state_buffer.push(GPS_ST);
    }

    // state_buffer.push(DEBUG_ST);
}
