#ifndef BLE_data_receive_H
#define BLE_data_receive_H

#include <Arduino.h>
#include "BajaDefs/packets.h"
#include <CANmsg.h>

void Send_debug_request(bool req_msg, CANmsg txMsg)
{
  
  txMsg.clear(MPU_ID);
  txMsg << req_msg;
  txMsg.write();
}

#endif