#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <CircularBuffer.hpp>
#include <Ticker.h>
#include "LORA.h"
#include "CAN.h"
#include "GPS.h"
#include "MPU_defs.h"

void CircularBuffer_Ticker_Init(void);
void CircularBuffer_CurrentState(void);

/* Tickers */
void ticker1HzISR(void);

#endif
