/*
 * recharger.h
 *
 *  Created on: 20 нояб. 2016 г.
 *      Author: vasimv
 */

#ifndef RECHARGER_H_
#define RECHARGER_H_

// Debug over serial (115200 8n1) interface
#define SERIAL_DEBUG

// Resistors divider ratio (43K/(1/(1/11K+1/MCU_INPUT_RESISTANCE)))
#define DIVIDER_RATIO 4.93

// Maximum charging current (mA)
#define CHARGE_CURRENT 200

// Maximum voltage during charge (*1000)
#define CHARGE_LIMIT 8600

// Maximum voltage to start the charge process
#define CHARGE_START 8200

// Maximum balance voltage during charge (*1000)
#define CHARGE_BALANCE_LIMIT 4300

// Minimum voltage on the battery to turn on DC-DC
#define MIN_VOLTAGE 6200

// PWM period (32 mHz tick)
#define PWMRANGE 63

// Time before entering sleep mode (in 0.02s)
#define SLEEP_TIMER 1500

// Minimum USB input voltage
#define CHARGE_MIN_USB 3200

// main setup/loop routines
extern void Setup();
extern void loop();

// timer interrupt function (every 0.02s)
extern void TimerInt();

// From main.c
extern ADC_HandleTypeDef hadc;
extern DMA_HandleTypeDef hdma_adc;
extern UART_HandleTypeDef huart1;
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim14;
extern IWDG_HandleTypeDef hiwdg;

// Internal voltage reference raw value at 30 degrees C, VDDA=3.3V
#define VREFINT_CAL (*((uint16_t *) 0x1FFFF7BA))
//Temperature sensor raw value at 30 degrees C, VDDA=3.3V
#define TS_CAL1 (*((uint16_t *) 0x1FFFF7B8))
//Temperature sensor raw value at 110 degrees C, VDDA=3.3V
#define TS_CAL2 (*((uint16_t *) 0x1FFFF7C2))

#endif /* RECHARGER_H_ */
