//*****************************************************************************
//
// timers.c - Timers example.
//
// Copyright (c) 2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 9453 of the EK-LM4F120XL Firmware Package.
//
//*****************************************************************************

#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_timer.h"

#include "debug.h"
#include "fpu.h"
#include "gpio.h"
#include "uart.h"
#include "interrupt.h"
#include "pin_map.h"
#include "rom.h"
#include "sysctl.h"
#include "timer.h"

#include "uartstdio.h"
#include "rgb.h"

#include <stdint.h>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Timer (timers)</h1>
//!
//! This example application demonstrates the use of the timers to generate
//! periodic interrupts.  One timer is set up to interrupt once per second and
//! the other to interrupt twice per second; each interrupt handler will toggle
//! its own indicator on the display.
//!
//! UART0, connected to the Stellaris Virtual Serial Port and running at 115,200,
//! 8-N-1, is used to display messages from this application.
//
//*****************************************************************************

//*****************************************************************************
//
// Flags that contain the current value of the interrupt indicator as displayed
// on the UART.
//
//*****************************************************************************
unsigned long g_ulFlags;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

void configUART();
void configPWM();
void configPWMWide();

//*****************************************************************************
//
// This example application demonstrates the use of the timers to generate
// periodic interrupts.
//
//*****************************************************************************
int main(void) {
	//
	// Set the clocking to run directly from the crystal with 80 MHz!!
	//
	SysCtlClockSet(
			SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN
					| SYSCTL_XTAL_16MHZ);

	char tmp[10];
	memset(tmp, 0xFF, 10);

	const char test[] = "Hallo BillyCPP\n";
	std::string *target = new std::string(test);
	//char* target = malloc(sizeof(char) * 30);
	//strncpy(target, test, 30);

	volatile float p1 = 1.3;
	float pt = 0.423 * p1;

	//if (strcmp(target, test) != 0) {
	//	while (1) {
	//	}
	//}

	// Enable processor interrupts.
	IntMasterEnable();

	if (pt > 0) {
		//while (1) {

		RGBInit(1);

		unsigned long color[] = { 0xFF, 0xFF, 0xF };
		RGBColorSet(color);

		//}
	}


	// configure PWM
	configPWMWide();
	configPWM();

	//
	// Loop forever while the timers run.
	//
	configUART();

	while (1) {
		UARTwrite(target->c_str(), strlen(target->c_str()));
		delete target;
		SysCtlDelay(2000000);

		target = new std::string(test);

	}
}

void configUART() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

	// Select UART Configuration for PINS
	GPIOPinConfigure(GPIO_PC4_U1RX);
	GPIOPinConfigure(GPIO_PC5_U1TX);

	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4);
	GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_5);

	UARTStdioInit(1);
}

void configPWM() {
	// activate timer clock
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	// Select PWM Configuration for PINS
	GPIOPinConfigure(GPIO_PB0_T2CCP0);
	GPIOPinConfigure(GPIO_PB1_T2CCP1);

	// set direction and alternative function
	GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_0);
	GPIOPinTypeTimer(GPIO_PORTB_BASE, GPIO_PIN_1);

	// Timer aktivieren
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

	// Disable Timer
	TimerDisable(TIMER2_BASE, TIMER_A | TIMER_B);

	// Enable PWM
	TimerConfigure(TIMER2_BASE,
			TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);

	// invert signal
	HWREG(TIMER2_BASE + TIMER_O_CTL) |= 0x4000;
	HWREG(TIMER2_BASE + TIMER_O_CTL) |= 0x40;

	// Prescaler
	TimerPrescaleSet(TIMER2_BASE, TIMER_A, 0xF);
	TimerPrescaleSet(TIMER2_BASE, TIMER_B, 0xF);

	// Load timer start value
	TimerLoadSet(TIMER2_BASE, TIMER_A, 0xFFFF);
	TimerLoadSet(TIMER2_BASE, TIMER_B, 0xFFFF);

	//Load TimerMatch
	TimerMatchSet(TIMER2_BASE, TIMER_A, 0xFFF);
	TimerMatchSet(TIMER2_BASE, TIMER_B, 0xFFF);

	// Enable Interrupts
	/*	IntEnable(INT_TIMER2A);
	 IntEnable(INT_TIMER2A);
	 TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	 TimerIntEnable(TIMER2_BASE, TIMER_TIMB_TIMEOUT); */

	//Enable Timers
	TimerEnable(TIMER2_BASE, TIMER_BOTH);
}

void configPWMWide() {
	// activate timer clock
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

	// Select PWM Configuration for PINS
	GPIOPinConfigure(GPIO_PC4_WT0CCP0);
	GPIOPinConfigure(GPIO_PC5_WT0CCP1);

	// set direction and alternative function
	GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_4);
	GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);

	// Timer aktivieren
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);

	// Disable Timer
	TimerDisable(WTIMER0_BASE, TIMER_A | TIMER_B);

	// Enable PWM
	TimerConfigure(WTIMER0_BASE,
			TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);

	// invert signal
	HWREG(WTIMER0_BASE + TIMER_O_CTL) |= 0x4000;
	HWREG(WTIMER0_BASE + TIMER_O_CTL) |= 0x40;

	// DesiredTime * 80 000 0000
	int32_t period = 0x186A00 - 0x1;

	// Load timer start value
	TimerLoadSet(WTIMER0_BASE, TIMER_A, period);
	TimerLoadSet(WTIMER0_BASE, TIMER_B, period);

	int32_t time = 0x3A980;

	//Load TimerMatch
	TimerMatchSet(WTIMER0_BASE, TIMER_A, time);
	TimerMatchSet(WTIMER0_BASE, TIMER_B, time);

	//Enable Timers
	TimerEnable(WTIMER0_BASE, TIMER_BOTH);
}
