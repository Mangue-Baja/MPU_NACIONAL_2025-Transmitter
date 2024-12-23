#include <Arduino.h>
#include <StateMachine.h>
#include <BLE.h>
#include <CAN.h>  

/* Task Management */
TaskHandle_t StateMachine_Task = NULL, BLEtask = NULL;

uint8_t status;

/* Tasks */
void StateMachineTask(void *pvParameters);
void BLE_Data_Task(void *pvParameters);

void setup() 
{
  Serial.begin(115200);  
  
  pinMode(EMBEDDED_LED, OUTPUT);

  /* CAN-BUS initialize */
  if (!CAN_start_device())
    esp_restart();
  
  status = LORA_init();
  
  GPS_init();
  
  Init_BLE_Server();

  CircularBuffer_Ticker_Init();

  xTaskCreatePinnedToCore(StateMachineTask, "StateMachineTask", 4096, NULL, 5, &StateMachine_Task, 0);
  xTaskCreatePinnedToCore(BLE_Data_Task, "BLE_Data_Task", 4096, NULL, 4, &BLEtask, 1);
}

void loop() {/**/}

void StateMachineTask(void *pvParameters)
{
  while (1)
  {
    CircularBuffer_CurrentState();

    vTaskDelay(1);
  }
  
  vTaskDelay(1);
}

void BLE_Data_Task(void *arg)
{
  for (;;)
  { 
    if (BLE_connected() && App_MPU_data_request())
    {
      Save_LORA_init_status(status);
      Send_MPU_REQUEST(true);
      
      vTaskDelay(300); // Wait the response
      
      Send_BLE_msg();
      CLEAR_BLE_MSG();
    } 

    vTaskDelay(MAX_BLE_DELAY);
  }

  vTaskDelay(1);
}
