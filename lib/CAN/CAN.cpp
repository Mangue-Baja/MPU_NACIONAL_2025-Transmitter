#include "CAN.h"

CANmsg txMsg(CAN_RX_id, CAN_TX_id, CAN_BPS_1000K);
radio_packet_t can_receive_packet;
bluetooth bluetooth_packet;
bool mode = false; 

bool CAN_start_device(bool debug_mode)
{
  txMsg.Set_Debug_Mode(debug_mode);
  if (!txMsg.init(canISR))
  {
    Serial.println("CAN ERROR!! SYSTEM WILL RESTART IN 2 SECONDS...");
    // log_e("CAN ERROR!! SYSTEM WILL RESTART IN 2 SECONDS...");
    vTaskDelay(2000);
    return false;
  }
  /* if you needed apply a Mask and id to filtering write this:
   * txMsg.init(canISR, ID); or txMsg.init(canISR, ID, Mask);
   * if you ID is bigger then 0x7FF the extended mode is activated
   * you can set the filter in this function too:
   * txMsg.Set_Filter(uint32_t ID, uint32_t Mask, bool Extended);  */
  memset(&can_receive_packet, 0, sizeof(radio_packet_t));
  CLEAR_BLE_MSG();
  return true;
}

void Save_LORA_init_status(uint8_t st)
{
  bluetooth_packet.lora_init = st;
}

bool Send_GPS_data(double _msg, uint32_t _ID)
{
  txMsg.clear(_ID);
  txMsg << _msg;
  return txMsg.write();
}

bool Send_MPU_REQUEST(bool _msg)
{
  txMsg.clear(MPU_ID);
  txMsg << _msg;
  return txMsg.write();
}

bluetooth update_packet()
{
  return bluetooth_packet;
}

void CLEAR_BLE_MSG()
{
  memset(&bluetooth_packet, 0, sizeof(bluetooth));
}

/* CAN functions */
void canISR(CAN_FRAME *rxMsg)
{
  mode = !mode;
  digitalWrite(EMBEDDED_LED, mode);

  can_receive_packet.timestamp = millis();

  switch (rxMsg->id)
  {
  case TEMPERATURE_ID:
    memcpy(&bluetooth_packet.termistor, (uint8_t *)&rxMsg->data.uint8, sizeof(uint8_t));
    // Serial.printf("Motor = %d\r\n", bluetooth_packet.termistor);
    break;

  case CVT_ID:
    memcpy(&bluetooth_packet.cvt_temperature, (uint8_t *)&rxMsg->data.uint8, sizeof(uint8_t));
    // Serial.printf("CVT = %d\r\n", bluetooth_packet.cvt_temperature);
    break;

  case SOC_ID:
    memcpy(&bluetooth_packet.measure_volt, (uint8_t *)&rxMsg->data.uint8, sizeof(uint8_t));
    // Serial.printf("SOC = %d\r\n", bluetooth_packet.measure_volt);
    break;

  case SPEED_ID:
    memcpy(&bluetooth_packet.speed_pulse_counter, (uint16_t *)&rxMsg->data.uint8, sizeof(uint16_t));
    // Serial.printf("Speed = %d\r\n", bluetooth_packet.speed_pulse_counter);
    break;

  case MMI_ID:
    memcpy(&bluetooth_packet.accel_begin, (uint8_t *)&rxMsg->data.uint8, sizeof(uint8_t));
    // Serial.printf("accel_begin = %d\r\n", bluetooth_packet.accel_begin);
    break;

  case TCU_ID:
    memcpy(&bluetooth_packet.servo_state, (uint16_t *)&rxMsg->data.uint8, sizeof(uint16_t));
    //Serial.printf("servo_state = %s\r\n", bluetooth_packet.servo_state == 4 ? "CHOKE" :  \
    bluetooth_packet.servo_state == 3 ? "MID" : bluetooth_packet.servo_state == 2 ? "RUN" : "ERRO");
    break;

  case SCU_ID:
  {
    uint8_t recv;

    memcpy(&recv, (uint8_t *)&rxMsg->data.uint8, sizeof(uint8_t));
    //Serial.printf("All data: %d\r\n", recv);

    bluetooth_packet.internet_modem = recv & 0x02;
    bluetooth_packet.mqtt_client_connection = (recv >> 2) & 0x02;
    bluetooth_packet.sd_start = (recv >> 4) & 0x02;
    bluetooth_packet.check_sd = (recv >> 6) & 0x02;

    // Serial.printf("internet_modem: %d\r\n", bluetooth_packet.internet_modem);
    // Serial.printf("mqtt_client_connection: %d\r\n", bluetooth_packet.mqtt_client_connection);
    // Serial.printf("sd_start: %d\r\n", bluetooth_packet.sd_start);
    // Serial.printf("check_sd: %d\r\n", bluetooth_packet.check_sd);
    break;
  }
  }
}
