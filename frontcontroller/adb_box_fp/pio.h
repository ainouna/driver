/*
 */

#include <asm/io.h>

#define PIO_PORT_SIZE      0x1000
#define PIO_BASE           0xb8020000
#define STPIO_SET_OFFSET   0x4
#define STPIO_CLEAR_OFFSET 0x8
#define STPIO_POUT_OFFSET  0x00
#define STPIO_PIN_OFFSET   0x10

#define STPIO_SET_PIN(PIO_ADDR, PIN, V) \
	writel(1 << PIN, PIO_ADDR + STPIO_POUT_OFFSET + ((V) ? STPIO_SET_OFFSET : STPIO_CLEAR_OFFSET))
#define STPIO_GET_PIN(PIO_ADDR, PIN) \
	((readl(PIO_ADDR + STPIO_PIN_OFFSET) & (1 << PIN)) ? 1 : 0)
#define PIO_PORT(n) \
	(((n)*PIO_PORT_SIZE) + PIO_BASE)
// vim:ts=4
