//# SPDX Apache-V2 Bga <bga.email@gmail.com>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include <!cpp/common.h>

#include <stdio.h>
#include "usb-gadget0.h"

#define ER_DEBUG
#ifdef ER_DEBUG
#define ER_DPRINTF(fmt, ...) \
	do { printf(fmt, ## __VA_ARGS__); } while (0)
#else
#define ER_DPRINTF(fmt, ...) \
	do { } while (0)
#endif

enum MidiCmd {
	MidiCmd_snare,
	MidiCmd_bass0,
	MidiCmd_bass1,
	MidiCmd_tomtom0,
	MidiCmd_tomtom1,
	MidiCmd_cymbal0,
	MidiCmd_cymbal1,
	MidiCmd_cymbal2,
	MidiCmd_cymbal3,
};


//# X(nameArg, portArg, pinNoArg, portModeArg, portClockArg, midiCmdArg)
#define GPIO__FOR_ALL(XArg) \
	XArg(hiHat, GPIOC, GPIO0, GPIO_CNF_OUTPUT_PUSHPULL, GPIO_MODE_OUTPUT_2_MHZ, MidiCmd_hiHat) \
	XArg(lowHat, GPIOC, GPIO1, GPIO_CNF_OUTPUT_PUSHPULL, GPIO_MODE_OUTPUT_2_MHZ, MidiCmd_lowHat) \
;


inline void asm_nop() {
	__asm__("nop");
}
void delay_ticks(unsigned ticksCount) {
	for(unsigned int i = 0; i < ticksCount; ++i) {
		asm_nop();
	}
	
}


int main() {
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_48MHZ]);
	/* LED to indicate boot process */
	#define GPIO__INIT_CLOCK(nameArg, portArg, pinNoArg, portModeArg, portClockArg, midiCmdArg) rcc_periph_clock_enable(RCC_ ## portArg);
		GPIO__FOR_ALL(GPIO__INIT_CLOCK);
	#undef GPIO__INIT_CLOCK
	
	#define GPIO__INIT_PIN(nameArg, portArg, pinNoArg, portModeArg, portClockArg, midiCmdArg) \
		gpio_set_mode(portArg, portClockArg, portModeArg, pinNoArg); \
		gpio_set(portArg, pinNoArg); \
	;
		GPIO__FOR_ALL(GPIO__INIT_PIN);
	#undef GPIO__INIT_PIN

	/*
	 * Vile hack to reenumerate, physically _drag_ d+ low.
	 * do NOT do this if you're board has proper usb pull up control!
	 * (need at least 2.5us to trigger usb disconnect)
	 */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
	gpio_clear(GPIOA, GPIO12);
	delay_ticks(800000);

	rcc_periph_clock_enable(RCC_OTGFS);


	usbd_device *usbd_dev = gadget0_init(&st_usbfs_v1_usb_driver, "stm32f103-generic");

	ER_DPRINTF("bootup complete\n");
	gpio_clear(GPIOC, GPIO13);
	while (1) {
		gadget0_run(usbd_dev);
	}

}

