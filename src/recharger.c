/*
 * recharger.c
 *
 *  Created on: 20 нояб. 2016 г.
 *      Author: vasimv
 */

#include "stm32f0xx_hal.h"
// #include "cmsis/device/stm32f030x6.h"
#include "recharger.h"
#include <stdio.h>
#include <stdarg.h>

// Data calculated from ADC readings
// Supply voltage
volatile int32_t Vdd = 0;
// Voltage of USB / input battery
volatile int32_t Vin = 0;
// Voltage of main battery
volatile int32_t Vout = 0;
// Voltage of balance input
volatile int32_t Vbal = 0;
// Incoming battery's current sense voltage
volatile int32_t Vcurr = 0;
// Chip's temperature
volatile int32_t Tchip = 0;

// Pause before charging counter
int PauseCounter = 0;

// Pause before sleep
int SleepTimer = 0;

// Buck/boost current PWM lengths
uint16_t CurrBuck = 0;
uint16_t CurrBoost = 0;

#ifdef SERIAL_DEBUG
char seroutbuf[256];

va_list args;
int nlen;

void debug(char *fmt, ...) {
	va_start(args, fmt);
	nlen = vsnprintf(seroutbuf, sizeof(seroutbuf), fmt, args);
	va_end(args);
	HAL_UART_Transmit(&huart1, seroutbuf, nlen, 2000);
}
#else
void debug(char *fmt, ...) {
}
#endif

// ADC DMA buffer
volatile uint16_t AdcData[6];

// ADC sum buffer and counter
volatile uint32_t AdcSumData[6];
volatile uint32_t AdcCount = 0;

// Calculated averages for all ADCs
uint16_t AdcAvgData[6];

// Read values from ADC buffer and calculate voltages
void UpdateThings() {
	for (int i = 0; i < sizeof(AdcSumData) / sizeof(AdcSumData[0]); i ++) {
		AdcAvgData[i] = AdcSumData[i] / AdcCount;
		AdcSumData[i] = 0;
	}
	AdcCount = 0;
	Vdd = 3300 * VREFINT_CAL / AdcAvgData[5];
	Vin = (Vdd * AdcAvgData[0]) / (uint16_t) (4095 / DIVIDER_RATIO);
	Vout = (Vdd * AdcAvgData[1]) / (uint16_t) (4095 / DIVIDER_RATIO);
	Vbal = (Vdd * AdcAvgData[3]) / (uint16_t) (4095 / DIVIDER_RATIO);
	Vcurr = (Vdd * AdcAvgData[2]) / 4095;
	Tchip = (((int32_t) AdcAvgData[4]) *  Vdd / (int32_t) 3300) - (int32_t) TS_CAL1;
	Tchip *= (int32_t)(110000 - 30000);
	Tchip = Tchip / (int32_t)( TS_CAL2 - TS_CAL1);
	Tchip += 30000;
#ifdef SERIAL_DEBUG
//	debug("A0: %d, A1: %d, A2: %d, A3: %d, A4: %d, A5: %d\n",
//				AdcData[0], AdcData[1], AdcData[2], AdcData[3], AdcData[4], AdcData[5]);
	debug("Cnt: %u, Boost: %u, Buck: %u, Vdd: %u, Tchip: %u, Vin: %u, Vout: %u, Vbal: %u, Ibatt: %u\n", AdcCount, CurrBoost, CurrBuck, Vdd, Tchip, Vin, Vout, Vbal, Vcurr);
#endif
} // void UpdateThings()

// On/Off DC-DC 5V module (USB out)
void SetDc(int set) {
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_1, set ? GPIO_PIN_SET : GPIO_PIN_RESET);
} // void SetDc(int set)

// On/Off Red LED
void SetRedLed(int set) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, set ? GPIO_PIN_RESET : GPIO_PIN_SET);
} // void SetRedLed()

// On/Off Green LED (1 - light, -1 - flash)
void SetGreenLed(int set) {
	TIM_OC_InitTypeDef sConfigOC;
	uint16_t pulse;

	switch (set) {
	case 1:
		pulse = 474;
		break;
	case 2:
		pulse = 499;
		break;
	case -1:
		pulse = 249;
		break;
	case -2:
		pulse = 24;
		break;
	default:
		pulse = 0;
		break;
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = pulse;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim14, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
} // void SetRedLed()

// Start 12-bit ADC (4 ADC channels, temperature, vrefint)
void StartAdc() {
	HAL_ADC_Start_DMA(&hadc, (uint32_t*)AdcData, sizeof(AdcData));
	HAL_ADC_Start(&hadc);
} // void StartAdc()

// Start debug serial interface
void StartSerial() {
	// Actually do nothing, everything is configured by STMCubeMX
	debug("Serial debug started!\n");
} // void StartSerial()

// Set up duty cycle on a timer
void PWM_Timer_Set(TIM_HandleTypeDef *phtim, uint32_t channel, uint32_t pulse) {
	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = pulse;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(phtim, &sConfigOC, channel);
	HAL_TIM_PWM_Start(phtim, channel);
} // PWM_Timer_Set

// Set boost PWM channel
void SetBoost(int pwm) {
	PWM_Timer_Set(&htim3, TIM_CHANNEL_2, pwm);
} // void SetBoost(int pwm)

// Set buck PWM channel
void SetBuck(int pwm) {
	PWM_Timer_Set(&htim3, TIM_CHANNEL_1, pwm);
} // void SetBuck(int pwm)

// Check if everything is ok, returns 1 if ready to go
int CheckFailsafes() {
	if ((Tchip > 80000) || (Vout < 4000) || (Vin < CHARGE_MIN_USB))
		return 0;
	return 1;
} // int CheckFailsafes()

// Check if we're ready to charge
int CheckCharge() {
	if ((Vout < 4000) || (Vout > CHARGE_LIMIT) || (Vbal > CHARGE_BALANCE_LIMIT) || (Vin < CHARGE_MIN_USB))
		return 0;
	return 1;
} // int CheckCharge()

// Interrupts counter
volatile unsigned long cycles = 0;

// Zero current (when battery connected just)
volatile int ZeroCurrent = 0;

// Max current reference
int MaxCurrent = 0;
uint16_t MaxPulse = 0;

// Reset variables when start charging
void ResetCharge() {
	MaxCurrent = 0;
	MaxPulse = 0;
	ZeroCurrent = Vcurr - 20;
} // void ResetCharge()

// Calculate charge PWM pulse
uint16_t CalculateCharge(uint16_t CurrPulse) {
	int Current;

	// Crude calculation
	Current = Vcurr - 20;

	if (ZeroCurrent == 0)
		ZeroCurrent = Current;

	// Lower pulse if charging current is too high
	if (Current > CHARGE_CURRENT)
		return CurrPulse - 1;

	// Increase pulse if current is too small
	if ((Current < 50) && (CurrPulse < (PWMRANGE / 3)))
		return CurrPulse + 1;

	if (CurrPulse == MaxPulse)
		MaxCurrent = Current+1;

	// Periodically increase/decrease pulse to find best value
	switch (cycles % 100) {
	case 1:
	case 3:
	case 5:
	case 7:
		if (Current > MaxCurrent) {
			MaxCurrent = Current;
			MaxPulse = CurrPulse;
		}
		if (Current > (CHARGE_CURRENT - 10))
			return CurrPulse;
		if (CurrPulse < (PWMRANGE - PWMRANGE/4))
			return CurrPulse+1;
		break;
	case 51:
	case 53:
	case 55:
	case 57:
		if (Current >= MaxCurrent) {
			MaxCurrent = Current;
			MaxPulse = CurrPulse;
		}
		if (CurrPulse > 1)
			return CurrPulse-1;
		break;
	case 9:
	case 59:
		if (Current > MaxCurrent) {
			MaxCurrent = Current;
			MaxPulse = CurrPulse;
		}
		break;
	case 10:
	case 60:
		return MaxPulse;
	default:
		break;
	}

	return CurrPulse;
} // uint16_t CalculateCharge(uint16_t CurrPulse)

// DMA completion routine (sum all ADCs readings for averaging later)
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	AdcCount++;
	for (int i = 0; i < sizeof(AdcData) / sizeof(AdcData[0]); i++) {
		AdcSumData[i] += AdcData[i];
	}
} // void DmaInt()

// Counter for empty cycles before starting charge
uint16_t EmptyCycles = 0;

// Charge flag
volatile int FlagCharge = 0;
volatile int FlagNeedDc = 0;

// Counter for how long DC turned off
volatile uint32_t CounterNoDc = 0;

// Interrupt timer
void TimerInt() {
	uint16_t Pulse;

	cycles++;
	UpdateThings();
	if (CheckCharge() && CheckFailsafes()) {
		FlagCharge = 1;
		// Make 0.5s pause before start
		if (EmptyCycles > 25) {
			// Calculating charging PWMs
			Pulse = CalculateCharge((CurrBuck == PWMRANGE) ? CurrBoost : CurrBuck);
			// Switch to boost if input voltage is not enough
			if (Vout > (Vin + 100)) {
				CurrBoost = Pulse;
				CurrBuck =  PWMRANGE;
			} else {
				CurrBuck = Pulse;
				CurrBoost = 0;
			}
		} else {
			// Don't start charge if the battery is too high already
			if (Vin > CHARGE_START)
				EmptyCycles = 0;
			EmptyCycles++;
		}
	} else {
		CurrBuck = 0;
		CurrBoost = 0;
		EmptyCycles = 0;
		ResetCharge();
		FlagCharge = 0;
	}
	SetBuck(CurrBuck);
	SetBoost(CurrBoost);
	// Turn on/off USB out module
	if (!FlagNeedDc || (CurrBuck > 0) || (CurrBoost > 0) || (Vout < MIN_VOLTAGE)) {
		SetDc(0);
		CounterNoDc++;
	} else {
		SetDc(1);
		CounterNoDc = 0;
	}
} // void TimerInt()

// Start timers
void StartTimers() {
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start(&htim3);
	SetBoost(0);
	SetBuck(0);
} // void StartTimers()

// Setup things before run
void Setup() {
#ifdef SERIAL_DEBUG
	StartSerial();
#endif
	StartAdc();
	SetRedLed(0);
	SetGreenLed(2);
	StartTimers();
} // void Setup()

// Main program loop
void loop() {
	int ButtonAlready = 0;

	HAL_Delay(2000);
	debug("Checking button\n");
	// Switching DC-DC state if button pressed
	if ((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET) && !ButtonAlready) {
		ButtonAlready = 1;
		FlagNeedDc = FlagNeedDc ? 0 : 1;
	} else
		ButtonAlready = 0;

	debug("Setting LEDs\n");
	if (!CheckFailsafes())
		SetGreenLed(FlagNeedDc ? -1 : -2);
	else
		SetGreenLed(FlagNeedDc ? 2 : 1);
	if ((CurrBuck > 0) || (CurrBoost > 0))
		SetRedLed(1);
	else
		SetRedLed(0);

	// Check sleep condition
	if (!FlagCharge && (CounterNoDc > SLEEP_TIMER)) {
		// Going to sleep, flash LEDs
		debug("Preparing for standby mode\n");
		SetGreenLed(0);
		SetRedLed(0);
		HAL_Delay(2000);
		SetRedLed(1);
		HAL_Delay(500);

		debug("Entering standby mode\n");
		HAL_Delay(1);

		// Not sure why i can't do this through HAL functions...
        RCC->APB1ENR |= RCC_APB1ENR_PWREN;
        PWR->CR |= PWR_CR_CWUF;
        PWR->CSR |= PWR_CSR_EWUP1;

        HAL_PWR_EnterSTANDBYMode();

		// Going out of sleep by wake up button, just resetting everything
		HAL_NVIC_SystemReset();
	}
} // void loop()
