#ifndef CAN_H
#define CAN_H

#include <Arduino.h>
#include <CANmsg.h>
#include "can_defs.h"
#include "hard_defs.h"
#include "packets.h"

bool CAN_start_device(bool debug_mode = false);
void Save_LORA_init_status(uint8_t st);
bool Send_GPS_data(double _msg, uint32_t _ID);
bool Send_MPU_REQUEST(bool _msg);
bluetooth update_packet(void);

/* Interrupt */
void canISR(CAN_FRAME *rxMsg);

#endif
