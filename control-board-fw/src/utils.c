#include <libopencm3/cm3/scb.h>
#include "utils.h"

inline void delay(uint32_t wait)
{
	while (--wait > 0) __asm__("nop");
}

static const char *hexdigits = "0123456789ABCDEF";

void uint32hex(uint32_t num, char *str)
{
	uint32_t i;
	for (i = 0; i < 8; i++) {
		str[i] = hexdigits[(num >> (28 - i * 4)) & 0xF];
	}
}

// converts data to hexa
void data2hex(const void *data, uint32_t len, char *str)
{
	uint32_t i;
	const uint8_t *cdata = (uint8_t *)data;
	for (i = 0; i < len; i++) {
		str[i * 2    ] = hexdigits[(cdata[i] >> 4) & 0xF];
		str[i * 2 + 1] = hexdigits[cdata[i] & 0xF];
	}
	str[len * 2] = 0;
}

uint32_t readprotobufint(uint8_t **ptr)
{
	uint32_t result = (**ptr & 0x7F);
	if (**ptr & 0x80) {
		(*ptr)++;
		result += (**ptr & 0x7F) * 128;
		if (**ptr & 0x80) {
			(*ptr)++;
			result += (**ptr & 0x7F) * 128 * 128;
			if (**ptr & 0x80) {
				(*ptr)++;
				result += (**ptr & 0x7F) * 128 * 128 * 128;
				if (**ptr & 0x80) {
					(*ptr)++;
					result += (**ptr & 0x7F) * 128 * 128 * 128 * 128;
				}
			}
		}
	}
	(*ptr)++;
	return result;
}

void __attribute__((noreturn)) system_halt(void)
{
	for (;;) {} // loop forever
}

void __attribute__((noreturn)) system_reset(void)
{
	scb_reset_system();
}
