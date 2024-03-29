/****************************************************************************
 *
 * StarCI2Win driver for TF7700, Fortis, Opticum HD (TS) 9600
 *
 * CI slot driver for the following receivers:
 * Fortis FS9000 (StarCI2Win)
 * Fortis HS8200 (StarCI2Win)
 * Fortis HS7420 (Coreriver CIcore10)
 * Fortis HS7429 (Coreriver CIcore10)
 * Fortis HS7810A (Coreriver CIcore10)
 * Fortis HS7819 (Coreriver CIcore10)
 * CubeRevo (StarCI2Win)
 * CubeRevo mini (StarCI2Win)
 * CubeRevo mini II (StarCI2Win)
 * CubeRevo 2000HD (StarCI2Win)
 * CubeRevo 3000HD (StarCI2Win)
 * CubeRevo 9500HD (StarCI2Win)
 * CubeRevo 200HD (mini_FTA) (StarCI2Win)
 * CubeRevo 250HD (StarCI2Win)
 * Topfield TF77X0 HDPVR (StarCI2Win)
 * Opticum HD (TS) 9600 (StarCI)
 * Opticum HD (TS) 9600 Prima (StarCI)
 *
 * It should be noted that this driver is used for both the StarCI2Win
 * and the StarCI/CIcore10, although these devices are not entirely
 * the same; the StarCI/CIcore10 seems to be downward compatible to
 * the StarCI2Win.
 *
 * Copyright (C) 2007 konfetti <konfetti@ufs910.de>
 * many thanx to jolt for good support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ****************************************************************************
 *
 * Changes
 *
 * Date     By              Description
 * --------------------------------------------------------------------------
 * 20211008 Audioniek       Removed Fortis HS7110/7119.
 * */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/string.h>

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/i2c.h>
#include <linux/i2c-algo-bit.h>
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,17)
#include <linux/stm/pio.h>
#else
#include <linux/stpio.h>
#endif

#include <asm/system.h>
#include <asm/io.h>

#include "dvb_ca_en50221.h"

#include "dvb_ca_core.h"

short paramDebug = 10;
static int extmoduldetect = 0;

#define TAGDEBUG "[starci2win] "

#ifndef dprintk
#define dprintk(level, x...) do \
{ \
	if ((paramDebug) && (paramDebug >= level)|| paramDebug == 0) \
	{ \
		printk(TAGDEBUG x); \
	} \
} while (0)
#endif

#if defined(FS9000) \
 || defined(HS8200) \
 || defined(HS7420) \
 || defined(HS7429) \
 || defined(HS7810A) \
 || defined(HS7819) \
 || defined(OPT9600) \
 || defined(OPT9600PRIMA)
struct stpio_pin *cic_enable_pin = NULL;
struct stpio_pin *module_A_pin = NULL;
struct stpio_pin *module_B_pin = NULL;
#endif

#if defined(CUBEREVO) \
 || defined(CUBEREVO_MINI) \
 || defined(CUBEREVO_MINI2) \
 || defined(CUBEREVO_250HD) \
 || defined(CUBEREVO_9500HD) \
 || defined(CUBEREVO_2000HD) \
 || defined(CUBEREVO_MINI_FTA) \
 || defined(CUBEREVO_3000HD)
#define CUBEBOX
#else
#undef  CUBEBOX
#endif

/* I2C addressing */
#if defined(FS9000) \
 || defined(HS8200)
#define STARCI2_I2C_BUS 2
#elif defined(HS7420) \
 ||   defined(HS7429) \
 ||   defined(HS7810A) \
 ||   defined(HS7819)
#define STARCI2_I2C_BUS 1
#else
#define STARCI2_I2C_BUS 0
#endif

#if defined(HS7420) \
 || defined(HS7429) \
 || defined(HS7810A) \
 || defined(HS7819)
#define STARCI2_ADDR 0x43
#else
#define STARCI2_ADDR 0x40
#endif

/* StarCI2Win register definitions */
#define MODA_CTRL_REG         0x00
#define MODA_MASK_HIGH_REG    0x01
#define MODA_MASK_LOW_REG     0x02
#define MODA_PATTERN_HIGH_REG 0x03
#define MODA_PATTERN_LOW_REG  0x04
#define MODA_TIMING_REG       0x05
#define MODB_CTRL_REG         0x09
#define MODB_MASK_HIGH_REG    0x0a
#define MODB_MASK_LOW_REG     0x0b
#define MODB_PATTERN_HIGH_REG 0x0c
#define MODB_PATTERN_LOW_REG  0x0d
#define MODB_TIMING_REG       0x0e
#define SINGLE_MODE_CTRL_REG  0x10
#define TWIN_MODE_CTRL_REG    0x11
#define DEST_SEL_REG          0x17
#define POWER_CTRL_REG        0x18
#define INT_STATUS_REG        0x1a
#define INT_MASK_REG          0x1b
#define INT_CONFIG_REG        0x1c
#define STARCI_CTRL_REG       0x1f

#if defined(OPT9600)
#define STARCI_INT_PORT      2
#define STARCI_INT_PIN       4
#define STARCI_RESET_PORT    5
#define STARCI_RESET_PIN     5
#define STARCI_MODA_PORT     4
#define STARCI_MODA_PIN      7
#define STARCI_MODB_PORT     5
#define STARCI_MODB_PIN      4  /* hardware specific register values */
#endif

#if defined(OPT9600PRIMA)  // TODO: get values
#define STARCI_INT_PORT      2
#define STARCI_INT_PIN       4
#define STARCI_RESET_PORT    5
#define STARCI_RESET_PIN     5
#define STARCI_MODA_PORT     4
#define STARCI_MODA_PIN      7
#define STARCI_MODB_PORT     5
#define STARCI_MODB_PIN      4  /* hardware specific register values */
#endif

#if defined(TF7700)
unsigned char default_values[33] =
{
	0x00, /* register address for block transfer */
	0x02, 0x00, 0x01, 0x00, 0x00, 0x33, 0x00, 0x00,
	0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x33, 0x00,
	0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00
};
#elif defined(CUBEBOX)
unsigned char default_values[33] =
{
	0x00, /* register address for block transfer */
/*	0x00  0x01  0x02  0x03  0x04  0x05  0x06  0x07 */
	0x00, 0x00, 0x01, 0x00, 0x00, 0x33, 0x00, 0x00,
/*	0x08  0x09  0x0a  0x0b  0x0c  0x0d  0x0e  0x0f */
	0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x33, 0x00,
/*	0x10  0x11  0x12  0x13  0x14  0x15  0x16  0x17 */
	0x02, 0x9a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
/*	0x18  0x19  0x1a  0x1b  0x1c  0x1d  0x1e  0x1f */
	0xc0, 0x00, 0x00, 0x00, 0x00, 0x02, 0x03, 0x00
 };
#elif defined (FS9000)
unsigned char default_values[33] =
{
	0x00, /* register address for block transfer */
	0x00, 0x00, 0x02, 0x00, 0x00, 0x44, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x44, 0x00,
	0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x03, 0x01
};
#elif defined (HS8200)
unsigned char default_values[33] =
{
	0x00,
	0x00, 0x00, 0x02, 0x00, 0x00, 0x44, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x44, 0x00,
	0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x03, 0x01
};
#elif defined(HS7420) \
 ||   defined(HS7429) \
 ||   defined(HS7810A) \
 ||   defined(HS7819)
unsigned char default_values[33] =
{
	0x00,
	0x00, 0x00, 0x02, 0x00, 0x00, 0x44, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x00, 0x02, 0x44, 0x00,
	0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x00, 0x03, 0x06, 0x00, 0x03, 0x01
};
#elif defined(OPT9600)
unsigned char default_values[33] =
{
	0x00,  // start register number
	0x00,  // register 0x00 (MODA_CTRL_REG)
	0x00,  // register 0x01 (MODA_MASK_HIGH_REG)
	0x02,  // register 0x02 (MODA_MASK_LOW_REG)
	0x00,  // register 0x03 (MODA_PATTERN_HIGH_REG)
	0x00,  // register 0x04 (MODA_PATTERN_LOW_REG)
	0x44,  // register 0x05 (MODA_TIMING_REG)
	0x00,  // register 0x06 (MODA_INVERT_INPUT_MASK_REG), reserved on StarCI2Win
	0x00,  // register 0x07 (reserved)
	0x00,  // register 0x08 (reserved)
	0x00,  // register 0x09 (MODB_CTRL_REG)
	0x00,  // register 0x0a (MODB_MASK_HIGH_REG)
	0x02,  // register 0x0b (MODB_MASK_LOW_REG)
	0x00,  // register 0x0c (MODB_PATTERN_HIGH_REG)
	0x02,  // register 0x0d (MODB_PATTERN_LOW_REG)
	0x44,  // register 0x0e (MODB_TIMING_REG)
	0x00,  // register 0x0f (MODB_INVERT_INPUT_MASK_REG), reserved on StarCI2Win
	0x00,  // register 0x10 (SINGLE_MODE_CTRL_REG), reserved on StarCI & CIcore1.0
	0x80,  // register 0x11 (TWIN_MODE_CTRL_REG), reserved on StarCI & CIcore1.0
	0x00,  // register 0x12 (EXT_ACCESS_MASK_HIGH_REG)
	0x00,  // register 0x13 (EXT_ACCESS_MASK_LOW_REG)
	0x00,  // register 0x14 (EXT_ACCESS_PATTERN_HIGH_REG)
	0x00,  // register 0x15 (EXT_ACCESS_PATTERN_LOW_REG)
	0x00,  // register 0x16 (reserved)
	0x01,  // register 0x17 (DEST_SEL_REG)
	0x00,  // register 0x18 (POWER_CTRL_REG)
	0x00,  // register 0x19 (reserved)
	0x00,  // register 0x1a (INT_STATUS_REG)
	0x03,  // register 0x1b (INT_MASK_REG)
	0x06,  // register 0x1c (INT_CONFIG_REG)
	0x00,  // register 0x1d (UP_INTERFACE_CONFIG_REG)
	0x03,  // register 0x1e (UP_INTERFACE_WAITACK_REG)
	0x01   // register 0x1f (STARCI_CTRL_REG)
};
#elif defined(OPT9600PRIMA)
unsigned char default_values[33] =
{
	0x00,  // start register number
	0x00,  // register 0x00 (MODA_CTRL_REG)
	0x00,  // register 0x01 (MODA_MASK_HIGH_REG)
	0x02,  // register 0x02 (MODA_MASK_LOW_REG)
	0x00,  // register 0x03 (MODA_PATTERN_HIGH_REG)
	0x00,  // register 0x04 (MODA_PATTERN_LOW_REG)
	0x44,  // register 0x05 (MODA_TIMING_REG)
	0x00,  // register 0x06 (MODA_INVERT_INPUT_MASK_REG), reserved on StarCI2Win
	0x00,  // register 0x07 (reserved)
	0x00,  // register 0x08 (reserved)
	0x00,  // register 0x09 (MODB_CTRL_REG)
	0x00,  // register 0x0a (MODB_MASK_HIGH_REG)
	0x02,  // register 0x0b (MODB_MASK_LOW_REG)
	0x00,  // register 0x0c (MODB_PATTERN_HIGH_REG)
	0x02,  // register 0x0d (MODB_PATTERN_LOW_REG)
	0x44,  // register 0x0e (MODB_TIMING_REG)
	0x00,  // register 0x0f (MODB_INVERT_INPUT_MASK_REG), reserved on StarCI2Win
	0x00,  // register 0x10 (SINGLE_MODE_CTRL_REG), reserved on StarCI & CIcore1.0
	0x80,  // register 0x11 (TWIN_MODE_CTRL_REG), reserved on StarCI & CIcore1.0
	0x00,  // register 0x12 (EXT_ACCESS_MASK_HIGH_REG)
	0x00,  // register 0x13 (EXT_ACCESS_MASK_LOW_REG)
	0x00,  // register 0x14 (EXT_ACCESS_PATTERN_HIGH_REG)
	0x00,  // register 0x15 (EXT_ACCESS_PATTERN_LOW_REG)
	0x00,  // register 0x16 (reserved)
	0x01,  // register 0x17 (DEST_SEL_REG)
	0x00,  // register 0x18 (POWER_CTRL_REG)
	0x00,  // register 0x19 (reserved)
	0x00,  // register 0x1a (INT_STATUS_REG)
	0x03,  // register 0x1b (INT_MASK_REG)
	0x06,  // register 0x1c (INT_CONFIG_REG)
	0x00,  // register 0x1d (UP_INTERFACE_CONFIG_REG)
	0x03,  // register 0x1e (UP_INTERFACE_WAITACK_REG)
	0x01   // register 0x1f (STARCI_CTRL_REG)
};
#endif

/* EMI configuration */
unsigned long reg_config = 0;
unsigned long reg_buffer = 0;

#if defined(HS8200) \
 || defined(HS7420) \
 || defined(HS7429) \
 || defined(HS7810A) \
 || defined(HS7819)
unsigned long reg_sysconfig = 0;
#endif

#if defined(FS9000) \
 || defined(HS8200) \
 || defined(HS7420) \
 || defined(HS7429) \
 || defined(HS7810A) \
 || defined(HS7819) \
 || defined(OPT9600) \
 || defined(OPT9600PRIMA)
static unsigned char *slot_membase[2];
#else
/* for whatever reason the access has to be done though a short pointer */
static unsigned short *slot_membase[2];
#endif

#define EMI_DATA0_WE_USE_OE(a)    (a << 26)
#define EMI_DATA0_WAIT_POL(a)     (a << 25)
#define EMI_DATA0_LATCH_POINT(a)  (a << 20)
#define EMI_DATA0_DATA_DRIVE(a)   (a << 15)
#define EMI_DATA0_BUS_RELEASE(a)  (a << 11)
#define EMI_DATA0_CS_ACTIVE(a)    (a << 9)
#define EMI_DATA0_OE_ACTIVE(a)    (a << 7)
#define EMI_DATA0_BE_ACTIVE(a)    (a << 5)
#define EMI_DATA0_PORT_SIZE(a)    (a << 3)
#define EMI_DATA0_DEVICE_TYPE(a)  (a << 0)

#define EMI_DATA1_CYCLE(a)        (a << 31)
#define EMI_DATA1_ACCESS_READ(a)  (a << 24)
#define EMI_DATA1_CSE1_READ(a)    (a << 20)
#define EMI_DATA1_CSE2_READ(a)    (a << 16)
#define EMI_DATA1_OEE1_READ(a)    (a << 12)
#define EMI_DATA1_OEE2_READ(a)    (a << 8)
#define EMI_DATA1_BEE1_READ(a)    (a << 4)
#define EMI_DATA1_BEE2_READ(a)    (a << 0)

#define EMI_DATA2_CYCLE(a)        (a << 31)
#define EMI_DATA2_ACCESS_WRITE(a) (a << 24)
#define EMI_DATA2_CSE1_WRITE(a)   (a << 20)
#define EMI_DATA2_CSE2_WRITE(a)   (a << 16)
#define EMI_DATA2_OEE1_WRITE(a)   (a << 12)
#define EMI_DATA2_OEE2_WRITE(a)   (a << 8)
#define EMI_DATA2_BEE1_WRITE(a)   (a << 4)
#define EMI_DATA2_BEE2_WRITE(a)   (a << 0)

#if defined(HS8200) \
 || defined(HS7420) \
 || defined(HS7429) \
 || defined(HS7810A) \
 || defined(HS7819)
#define EMIConfigBaseAddress 0xfe700000
#define SysConfigBaseAddress 0xFE001000
#else
#define EMIConfigBaseAddress 0x1A100000
#endif

#define EMIBufferBaseAddress 0x1A100800

#define EMIBank0 0x100
#define EMIBank1 0x140
#define EMIBank2 0x180
#define EMIBank3 0x1C0
#define EMIBank4 0x200
#define EMIBank5 0x240  /* virtual */

#define EMIBank0BaseAddress EMIConfigBaseAddress + EMIBank0
#define EMIBank1BaseAddress EMIConfigBaseAddress + EMIBank1
#define EMIBank2BaseAddress EMIConfigBaseAddress + EMIBank2
#define EMIBank3BaseAddress EMIConfigBaseAddress + EMIBank3
#define EMIBank4BaseAddress EMIConfigBaseAddress + EMIBank4
#define EMIBank5BaseAddress EMIConfigBaseAddress + EMIBank5

/* konfetti: first base address of EMI. The specification
 * says that all other can be calculated from the top address
 * but this doesnt work for me. So I set the address fix later on
 * and do not waste time on that.
 */
#define EMI_BANK0_BASE_ADDRESS  0x40000000

/* ConfigBase */
#define EMI_STA_CFG 0x0010
#define EMI_STA_LCK 0x0018
#define EMI_LCK     0x0020
/* general purpose config register
 * 32Bit, R/W, reset=0x00
 * Bit 31-5 reserved
 * Bit 4 = PCCB4_EN: Enable PC card bank 4
 * Bit 3 = PCCB3_EN: Enable PC card bank 3
 * Bit 2 = EWAIT_RETIME
 * Bit 1/0 reserved
 */
#define EMI_GEN_CFG 0x0028

#define EMI_FLASH_CLK_SEL 0x0050  /* WO: 00, 10, 01 */
#define EMI_CLK_EN        0x0068  /* WO: must only be set once !!*/

/* BankBase */
#define EMI_CFG_DATA0 0x0000
#define EMI_CFG_DATA1 0x0008
#define EMI_CFG_DATA2 0x0010
#define EMI_CFG_DATA3 0x0018

/* ***************************** */
/* EMIBufferBaseAddress + Offset */
/* ***************************** */
#define EMIB_BANK0_TOP_ADDR 0x000
#define EMIB_BANK1_TOP_ADDR 0x010
#define EMIB_BANK2_TOP_ADDR 0x020
 /* 32Bit, R/W, reset=0xFB
	* Bits 31- 8: reserved
	* Bits 7 - 0: Bits 27-22 off Bufferadresse
	*/
#define EMIB_BANK3_TOP_ADDR 0x030
 /* 32Bit, R/W, reset=0xFB
	* Bits 31- 8: reserved
	* Bits 7 - 0: Bits 27-22 off Bufferadresse
	*/
#define EMIB_BANK4_TOP_ADDR 0x040
#define EMIB_BANK5_TOP_ADDR 0x050

/* 32 Bit, R/W, reset=0x110
 * Enable/Disable the banks
 */
#define EMIB_BANK_EN 	0x0060

static struct dvb_ca_state ca_state;

static int starci_read_attribute_mem(struct dvb_ca_en50221 *ca, int slot, int address);

/* EMI Banks End ************************************ */

/* ************************** */
/*     StarCI2Win control     */
/* ************************** */

/********************************************
 *
 * Write len bytes to controller, data[0]
 * is the first register written to.
 * data[1] through data[len - 1] are the
 * data written in the regsters 0 to len -2.
 *
 */
static int starci_writeregN(struct dvb_ca_state *state, u8 *data, u16 len)
{
	int    ret = -EREMOTEIO;
	struct i2c_msg msg;

	msg.addr  = state->i2c_addr;
	msg.flags = 0;
	msg.buf   = data;
	msg.len   = len;

	if ((ret = i2c_transfer(state->i2c, &msg, 1)) != 1)
	{
		dprintk(1, "%s: writereg error(err == %i, reg == 0x%02x\n", __func__, ret, data[0]);
		ret = -EREMOTEIO;
	}
	return ret;
}

/********************************************
 *
 * Write one byte (data) to register (reg).
 *
 */
static int starci_writereg(struct dvb_ca_state *state, int reg, int data)
{
	u8 buf[] = { reg, data };
	struct i2c_msg msg = { .addr = state->i2c_addr, .flags = 0, .buf = buf, .len = 2 };
	int err;

	dprintk(150, "%s >\n", __func__);
	dprintk(20, "%s: reg=0x%02x, value=0x%02x\n", __func__, reg, data);

	if ((err = i2c_transfer(state->i2c, &msg, 1)) != 1)
	{
		dprintk(1, "%s: error(err=%i, reg=0x%02x, data=0x%02x)\n", __func__, err, reg, data);
		return -EREMOTEIO;
	}
	return 0;
}

/********************************************
 *
 * Read register (reg).
 *
 */
static int starci_readreg(struct dvb_ca_state* state, u8 reg)
{
	int ret;
	u8 b0[] = { reg };
	u8 b1[] = { 0 };
	struct i2c_msg msg[] =
	{
		{ .addr = state->i2c_addr, .flags = 0, .buf = b0, .len = 1 },
		{ .addr = state->i2c_addr, .flags = I2C_M_RD, .buf = b1, .len = 1 }
	};

	dprintk(150, "%s >\n", __func__);
	ret = i2c_transfer(state->i2c, msg, 2);

	if (ret != 2)
	{
		dprintk(1, "%s: reg=0x%x (error=%d)\n", __func__, reg, ret);
		return ret;
	}
	dprintk(20, "%s reg=0x%02x, value=0x%02x\n",__func__, reg, b1[0]);
	dprintk(150, "%s <\n", __func__);
	return b1[0];
}

int starci_detect(struct dvb_ca_state* state)
{
	struct i2c_adapter *i2c;
	int val;
	u8 b0[] = { MODA_CTRL_REG };  // MOD-A CTRL
	u8 b1[] = { 0 };
	struct i2c_msg msg[] =
	{
		{ .addr = state->i2c_addr, .flags = 0, .buf = b0, .len = 1 },
		{ .addr = state->i2c_addr, .flags = I2C_M_RD, .buf = b1, .len = 1 }
	};

	dprintk(150, "%s >\n", __func__);
	dprintk(20, "%s: Using I2C bus %d, I2C address 0x%02x\n", __func__, STARCI2_I2C_BUS, state->i2c_addr);

	val = i2c_transfer(state->i2c, msg, 2);

	if (val < 0)
	{
		return val;
	}
	if (val != 2)
	{
		return -ENODEV;
	}
	dprintk(20, "%s: Found supported device on I2C bus %d, address 0x%02x\n", __func__, STARCI2_I2C_BUS, state->i2c_addr);
	return 0;
}

/* This function gets the CI source
	Params:
		 slot - CI slot number (0|1)
		 source - tuner number (0|1)
*/
void getCiSource(int slot, int* source)
{
#if !defined(HS8200) \
 && !defined(HS7429) \
 && !defined(HS7810A) \
 && !defined(HS7119) \
 && !defined(HS7819)  // These receivers use single mode?
	int val;

	dprintk(150, "%s >\n", __func__);
	val = starci_readreg(&ca_state, TWIN_MODE_CTRL_REG);
	val &= 0x20;

	if (slot == 0)
	{
#if defined(TF7700)
		if (val != 0)
		{
			*source = 1;
		}
		else
		{
			*source = 0;
		}
#else
		if (val != 0)
		{
			*source = 0;
		}
		else
		{
			*source = 1;
		}
#endif
	}

	if (slot == 1)
	{
#if defined(TF7700)
		if (val != 0)
		{
			*source = 0;
		}
		else
		{
			*source = 1;
		}
#else
		if (val != 0)
		{
			*source = 1;
		}
		else
		{
			*source = 0;
		}
#endif
	}
#endif
	dprintk(150, "%s <\n", __func__);
}

/* This function sets the CI source
   Params:
     slot - CI slot number (0|1)
     source - tuner number (0|1)
*/
int setCiSource(int slot, int source)
{
	int val;

	printk("%s> %d %d\n", __func__, slot, source);

	if ((slot < 0)
	||  (slot > 1)
	||  (source < 0)
	||  (source > 1))
	{
		return -1;
	}
#if defined(CUBEBOX)
/* fixme: think on this */
	if (slot == 0)
	{
		   return starci_writereg(&ca_state, TWIN_MODE_CTRL_REG, 0x82);
	}
	else
	{
		   return starci_writereg(&ca_state, TWIN_MODE_CTRL_REG, 0xa2);
	}
#else
	val = starci_readreg(&ca_state, TWIN_MODE_CTRL_REG);
	if (slot != source)
	{
		/* send stream A through module B and stream B through module A */
#if defined(TF7700) \
 || defined(HS8200) \
 || defined(FS9000) \
 || defined(OPT9600) \
 || defined(OPT9600PRIMA)
		val |= 0x20;
#else
		val &= ~0x20;
#endif
	}
	else
	{
		/* enforce direct mapping */
		/* send stream A through module A and stream B through module B */
#if defined(TF7700) \
 || defined(HS8200) \
 || defined(FS9000) \
 || defined(OPT9600) \
 || defined(OPT9600PRIMA)
		val &= ~0x20;
#else
		val |= 0x20;
#endif
	}
	return starci_writereg(&ca_state, TWIN_MODE_CTRL_REG, val);
#endif
}

void setDestination(struct dvb_ca_state *state, int slot)
{
	int result = 0;
	int loop = 0;
	int activationMask[2] = {0x02, 0x04};
	int deactivationMask[2] = {0x04, 0x02};

	dprintk(150, "%s > slot = %d\n", __func__, slot);

	if ((slot < 0)
	||  (slot > 1))
	{
		return;
	}
	/* read destination register */
	result = starci_readreg(state, DEST_SEL_REG);

	while ((result & activationMask[slot]) == 0)
	{
		/* unselect the other slot if this was selected before */
		result = result & ~deactivationMask[slot];
		starci_writereg(state, DEST_SEL_REG, result | activationMask[slot]);

		/* try it first time without sleep */
		if (loop > 0)
		{
			msleep(10);
		}
		/* re-read the destination register */
		result = starci_readreg(state, DEST_SEL_REG);

		dprintk(20, "%s (slot = %d, loop = %d): result = 0x%x\n", __func__, slot,loop, result);

		loop++;
		if (loop >= 10)
		{
			dprintk(1, "->abort setting slot destination\n");
			break;
		}
	}
	dprintk(150, "%s < slot = %d\n", __func__, slot);
}

static int starci_poll_slot_status(struct dvb_ca_en50221 *ca, int slot, int open)
{
	struct dvb_ca_state *state = ca->data;
	int    slot_status = 0;
	int    ctrlReg[2] = {MODA_CTRL_REG, MODB_CTRL_REG};

	dprintk(150, "%s <\n", __func__);
	dprintk(20, "%s (slot %d; open = %d) >\n", __func__, slot, open);

	if ((slot < 0)
	|| (slot > 1))
	{
		return 0;
	}
	slot_status = starci_readreg(state, ctrlReg[slot]) & 0x01;

	if (slot_status)
	{
		if (state->module_status[slot] & SLOTSTATUS_RESET)
		{
			unsigned int result = starci_read_attribute_mem(ca, slot, 0); 

			dprintk(20, "%s: SLOTSTATUS_RESET result = 0x%02x\n", __func__, result);

			if (result == 0x1d)
			{
				state->module_status[slot] = SLOTSTATUS_READY;
				dprintk(50, "%s: status -> SLOTSTATUS_READY\n", __func__);
			}
		}
		else if (state->module_status[slot] & SLOTSTATUS_NONE)
		{
#if defined(HS8200) \
 || defined(FS9000) \
 || defined(OPT9600) \
 || defined(OPT9600PRIMA)
			dprintk(50, "%s: SLOTSTATUS_NONE -> set PIO pin to 1\n", __func__);
			if (slot == 0)
			{
				stpio_set_pin(module_A_pin, 1);
		    }
			else
			{
				stpio_set_pin(module_B_pin, 1);
			}
#endif
			dprintk(20, "CI module present now\n");
			state->module_status[slot] = SLOTSTATUS_PRESENT;
			dprintk(50, "%s: status -> SLOTSTATUS_PRESENT\n", __func__);
		}
	}
	else
	{
		  if (!(state->module_status[slot] & SLOTSTATUS_NONE))
		  {
#if defined(HS8200) \
 || defined(FS9000) \
 || defined(OPT9600) \
 || defined(OPT9600PRIMA)
			if (slot == 0)
			{
				stpio_set_pin(module_A_pin, 0);
			}
			else
			{
				stpio_set_pin(module_B_pin, 0);
			}
#endif
			dprintk(20, "CI module not present now\n");
			state->module_status[slot] = SLOTSTATUS_NONE;
		}
	}
	if (state->module_status[slot] != SLOTSTATUS_NONE)
	{
		slot_status = DVB_CA_EN50221_POLL_CAM_PRESENT;
	}
	else
	{
		slot_status = 0;
	}
	if (state->module_status[slot] & SLOTSTATUS_READY)
	{
		slot_status |= DVB_CA_EN50221_POLL_CAM_READY;
	}
	dprintk(20, "Module %c (%d): result = 0x%02x, status = %d\n", slot ? 'B' : 'A', slot, slot_status, state->module_status[slot]);
	dprintk(150, "%s <\n", __func__);
	return slot_status;
}

static int starci_slot_reset(struct dvb_ca_en50221 *ca, int slot)
{
	struct dvb_ca_state *state = ca->data;
	int reg[2] = {MODA_CTRL_REG, MODB_CTRL_REG};
	int result;

	dprintk(150, "%s >\n", __func__);

	if((slot < 0) || (slot > 1))
	{
		return -1;
	}
	state->module_status[slot] = SLOTSTATUS_RESET;

	result = starci_readreg(state, reg[slot]);

	/* only reset if module is present */
	if (result & 0x01)
	{
#if defined(CUBEBOX)
		starci_writereg(state, reg[slot], 0x80);

		msleep(200);

		/* reset "rst" bit */
		result = starci_readreg(state, reg[slot]);
		starci_writereg(state, reg[slot], 0x00);

		dprintk(KERN_ERR "Reset Module %c\n", slot ? 'B' : 'A');
#else
		result = starci_readreg(state, reg[slot]);
		dprintk(20, "%s: result = 0x%02x\n", __func__, result);
		starci_writereg(state, reg[slot], result | 0x80);

		starci_writereg(state, DEST_SEL_REG, 0x0);
#if defined(HS8200) \
 || defined(FS9000) \
 || defined(HS7420) \
 || defined(HS7429) \
 || defined(HS7810A) \
 || defined(HS7819) \
 || defined(OPT9600) \
 || defined(OPT9600PRIMA)
		msleep(200);
#else
		msleep(60);
#endif
		/* reset "rst" bit */
		result = starci_readreg(state, reg[slot]);
		starci_writereg(state, reg[slot], result & ~0x80);
		dprintk(20, "Reset Module %c\n", slot ? 'B' : 'A');
#endif
	}
	dprintk(150, "%s <\n", __func__);
	return 0;
}

static int starci_read_attribute_mem(struct dvb_ca_en50221 *ca, int slot, int address)
{
	struct dvb_ca_state *state = ca->data;
	int res = 0;
	int result;
	int reg[2] = { MODA_CTRL_REG, MODB_CTRL_REG };

	dprintk(150, "%s >\n", __func__);
	dprintk(20, "%s slot = %d, address = 0x%02x\n", __func__, slot, address);

	if ((slot < 0)
	|| (slot > 1))
	{
		return -1;
	}
	result = starci_readreg(state, reg[slot]);

	if (result & 0xC)
	{
		starci_writereg(state, reg[slot], (result & ~0xC));
	}
	setDestination(state, slot);

	res = slot_membase[slot][address] & 0xFF;

	if (address <= 2)
	{
		dprintk (20, "address = 0x%02x: res = 0x%02x\n", address, res);
	}
	else
	{
		if ((res > 31) && (res < 127))
		{
			dprintk(20, "%s: %c\n", __func__, res);
		}
		else
		{
			dprintk(20, "%s: .\n", __func__);
		}
	}
	return res;
}

static int starci_write_attribute_mem(struct dvb_ca_en50221 *ca, int slot, int address, u8 value)
{
	struct dvb_ca_state *state = ca->data;
	int result;
	int reg[2] = { MODA_CTRL_REG, MODB_CTRL_REG };

	dprintk(150, "%s >\n", __func__);
	dprintk(20, "%s: slot = %d, address = 0x%02x, value = 0x%02x\n", __func__, slot, address, value);

	if ((slot < 0)
	||  (slot > 1))
	{
		return -1;
	}
	result = starci_readreg(state, reg[slot]);

	/* reset bit 3/4 ->access to attribute mem */
	if (result & 0xC)
	{
		starci_writereg(state, reg[slot], (result & ~0xC));
	}
	setDestination(state, slot);
	slot_membase[slot][address] = value;
	return 0;
}

static int starci_read_cam_control(struct dvb_ca_en50221 *ca, int slot, u8 address)
{
	struct dvb_ca_state *state = ca->data;
	int res = 0;
	int result;
	int reg[2] = { MODA_CTRL_REG, MODB_CTRL_REG };

	dprintk(150, "%s > slot = %d, address = %d\n", __func__, slot, address);

	if ((slot < 0)
	||  (slot > 1))
	{
		return -1;
	}
	result = starci_readreg(state, reg[slot]);

	/* FIXME: handle "result" ->is the module really present */

	/* access i/o mem (bit 3) */
	if (!(result & 0x4))
	{
		starci_writereg(state, reg[slot], (result & ~0xC) | 0x4);
	}
	setDestination(state, slot);
	res = slot_membase[slot][address] & 0xFF;

	if (address > 2)
	{
		if ((res > 31) && (res < 127))
		{
			dprintk(20, "%s: %c", __func__, res);
		}
		else
		{
			dprintk(20, "%s: .");
		}
	}
	return res;
}

static int starci_write_cam_control(struct dvb_ca_en50221 *ca, int slot, u8 address, u8 value)
{
	struct dvb_ca_state *state = ca->data;
	int result;
	int reg[2] = { MODA_CTRL_REG, MODB_CTRL_REG };

	dprintk(150, "%s >\n", __func__);
	dprintk(20, "%s: slot = %d, address = 0x%02x, value = 0x%02x\n", __func__, slot, address, value);

	if((slot < 0) || (slot > 1))
	{
		return -1;
	}
	result = starci_readreg(state, reg[slot]);

	/* FIXME: handle "result" ->is the module really present */

	/* access to i/o mem */
	if (!(result & 0x4))
	{
		starci_writereg(state, reg[slot], (result & ~0xC) | 0x4);
	}
	setDestination(state, slot);

	slot_membase[slot][address] = value;

	//without this some modules not working (unicam evo, unicam twin, zetaCam)
	//I have tested with 9 modules an all working with this code
	if (extmoduldetect == 1 && value == 8 && address == 1)
	{
		slot_membase[slot][address] = 0;
	}
	return 0;
}

static int starci_slot_shutdown(struct dvb_ca_en50221 *ca, int slot)
{
	dprintk(150, "%s >\n", __func__);
	dprintk(20,"%s: slot = %d\n", __func__, slot);

	/*Power control : (@18h); quatsch slot shutdown ->0x17*/
	return 0;
}

static int starci_slot_ts_enable(struct dvb_ca_en50221 *ca, int slot)
{
	struct dvb_ca_state *state = ca->data;
	int reg[2] = { MODA_CTRL_REG, MODB_CTRL_REG };
	int result;

	dprintk(150, "%s >\n", __func__);
	dprintk(20, "%s: slot = %d\n", __func__, slot);

	if ((slot < 0) || (slot > 1))
	{
		return -1;
	}
	result = starci_readreg(state, reg[slot]);  // read CTRL register

#if !defined(HS8200) \
 && !defined(FS9000) \
 && !defined(HS7420) \
 && !defined(HS7429) \
 && !defined(HS7810A) \
 && !defined(HS7819) \
 && !defined(CUBEBOX) \
 && !defined(OPT9600) \
 || defined(OPT9600PRIMA)
	starci_writereg(state, reg[slot], 0x23);  // write CTRL register: MPEG stream enabled, module inserted, auto mode
#else
	starci_writereg(state, reg[slot], 0x21);  // write CTRL register: MPEG stream enabled, module inserted, no auto mode
#endif

	/* reading back from the register implements the delay */
	result = starci_readreg(state, reg[slot]);

	dprintk(20, "%s: writing 0x%x\n", __func__, result | 0x40);  // disable bypass
	starci_writereg(state, reg[slot], result | 0x40);
	result = starci_readreg(state, reg[slot]);

	dprintk(20, "%s: result 0x%02x (slot %d)\n", __func__, result, slot);

	if (!(result & 0x40))
	{
		dprintk(1, "%s: Error setting ts enable on slot %d\n", __func__, slot);
	}
	return 0;
}

int init_ci_controller(struct dvb_adapter* dvb_adap)
{
	struct dvb_ca_state *state = &ca_state;
	int result = 0;

	dprintk(150, "%s >\n", __func__);

	state->dvb_adap = dvb_adap;

	state->i2c = i2c_get_adapter(STARCI2_I2C_BUS);

	if (state->i2c == NULL)
	{
		dprintk(1, "%s: Error: Cannot find I2C adapter #%d\n", __func__, STARCI2_I2C_BUS);
		return -ENODEV;
	}
	state->i2c_addr = STARCI2_ADDR;

	if (starci_detect(&ca_state))
	{
		dprintk(1, "%s Error: StarCI2Win chip not detected.\n",__func__);
	}
	memset(&state->ca, 0, sizeof(struct dvb_ca_en50221));

	/* register CI interface */
	state->ca.owner               = THIS_MODULE;

	state->ca.read_attribute_mem  = starci_read_attribute_mem;
	state->ca.write_attribute_mem = starci_write_attribute_mem;
	state->ca.read_cam_control    = starci_read_cam_control;
	state->ca.write_cam_control   = starci_write_cam_control;
	state->ca.slot_shutdown       = starci_slot_shutdown;
	state->ca.slot_ts_enable      = starci_slot_ts_enable;

	state->ca.slot_reset          = starci_slot_reset;
	state->ca.poll_slot_status    = starci_poll_slot_status;

	state->ca.data                = state;

	state->module_status[0]       = SLOTSTATUS_NONE;
	state->module_status[1]       = SLOTSTATUS_NONE;

	reg_config = (unsigned long)ioremap(EMIConfigBaseAddress, 0x7ff);

#if !defined(HS8200) \
 && !defined(HS7420) \
 && !defined(HS7429) \
 && !defined(HS7810A) \
 && !defined(HS7819)
	reg_buffer = (unsigned long)ioremap(EMIBufferBaseAddress, 0x40);
#endif

#if defined(HS8200) \
 || defined(HS7420) \
 || defined(HS7429) \
 || defined(HS7810A) \
 || defined(HS7819)
	reg_sysconfig = (unsigned long)ioremap(SysConfigBaseAddress, 0x200);
#endif

	dprintk(20, "ioremap 0x%.8x -> 0x%.8lx\n", EMIConfigBaseAddress, reg_config);

#if !defined(HS8200) \
 && !defined(HS7420) \
 && !defined(HS7429) \
 && !defined(HS7810A) \
 && !defined(HS7819)
	dprintk(20, "ioremap 0x%.8x -> 0x%.8lx\n", EMIBufferBaseAddress, reg_buffer);
#endif

#if defined(FS9000)
	cic_enable_pin = stpio_request_pin (3, 6, "StarCI", STPIO_OUT);
	stpio_set_pin (cic_enable_pin, 1);
	msleep(250);
	stpio_set_pin (cic_enable_pin, 0);
	msleep(250);

	module_A_pin = stpio_request_pin (1, 2, "StarCI_ModA", STPIO_OUT);
	module_B_pin = stpio_request_pin (2, 7, "StarCI_ModB", STPIO_OUT);

	stpio_set_pin (module_A_pin, 0);
	stpio_set_pin (module_B_pin, 0);
#elif defined(HS8200)
	/* the magic potion - some clkb settings */
	ctrl_outl(0x0000c0de, 0xfe000010);
	ctrl_outl(0x00000008, 0xfe0000b4);
	ctrl_outl(0x0000c1a0, 0xfe000010);

	/* necessary to access i2c register */
	ctrl_outl(0x1c, reg_sysconfig + 0x160);

	module_A_pin = stpio_request_pin (11, 0, "StarCI_ModA", STPIO_OUT);
	module_B_pin = stpio_request_pin (11, 1, "StarCI_ModB", STPIO_OUT);

	cic_enable_pin = stpio_request_pin (15, 1, "StarCI", STPIO_OUT);
	stpio_set_pin (cic_enable_pin, 1);
	msleep(250);
	stpio_set_pin (cic_enable_pin, 0);
	msleep(250);

	stpio_set_pin (module_A_pin, 0);
	stpio_set_pin (module_B_pin, 0);
#elif defined(HS7420) \
 ||   defined(HS7429) \
 ||   defined(HS7810A) \
 ||   defined(HS7819)
	/* the magic potion - some clkb settings */
	ctrl_outl(0x0000c0de, 0xfe000010);
	ctrl_outl(0x00000008, 0xfe0000b4);
	ctrl_outl(0x0000c1a0, 0xfe000010);

	cic_enable_pin = stpio_request_pin (6, 2, "StarCI_RST", STPIO_OUT);
	stpio_set_pin (cic_enable_pin, 1);
	msleep(250);
	stpio_set_pin (cic_enable_pin, 0);
	msleep(250);
#elif defined(OPT9600) \
 ||   defined(OPT9600PRIMA)
	cic_enable_pin = stpio_request_pin (STARCI_RESET_PORT, STARCI_RESET_PIN, "StarCI_RST", STPIO_OUT);
	if (cic_enable_pin == NULL)
	{
		dprintk(1, "%s: Request for STPIO pin %d.%1d (STARCI_RESET) failed; abort\n", __func__, STARCI_RESET_PORT, STARCI_RESET_PIN);
		result = 1;
		goto pio_init_fail3;
	}
	stpio_set_pin (cic_enable_pin, 1);
	msleep(250);
	stpio_set_pin (cic_enable_pin, 0);
	msleep(250);

	module_A_pin = stpio_request_pin (STARCI_MODA_PORT, STARCI_MODA_PIN, "StarCI_ModA", STPIO_OUT);  // power enable for upper slot
	if (module_A_pin == NULL)
	{
		dprintk(1, "%s: Request for STPIO pin %d.%1d (STARCI_MODA) failed; abort\n", __func__, STARCI_MODA_PORT, STARCI_MODA_PIN);
		result = 1;
		goto pio_init_fail2;
	}
	module_B_pin = stpio_request_pin (STARCI_MODB_PORT, STARCI_MODB_PIN, "StarCI_ModB", STPIO_OUT);  // power enable for lower slot
	if (module_B_pin == NULL)
	{
		dprintk(1, "%s: Request for STPIO pin %d.%1d (STARCI_MODB) failed; abort\n", __func__, STARCI_MODB_PORT, STARCI_MODB_PIN);
		result = 1;
		goto pio_init_fail3;
	}
	dprintk(20, "%s: Allocating PIO pins successful\n", __func__);
	stpio_set_pin (module_A_pin, 0);
	stpio_set_pin (module_B_pin, 0);
#endif

	/* reset the chip */
	dprintk(20, "%s: Initializing StarCI2Win\n", __func__);
	starci_writereg(state, STARCI_CTRL_REG, 0x80);  // set RESET bit

	starci_writeregN(state, default_values, sizeof(default_values));  // set registers

	/* lock the configuration */
	starci_writereg(state, STARCI_CTRL_REG, 0x01);  // reset RESET, lock config
	dprintk(20, "%s: Initializing StarCI2Win\n", __func__);

	/* power on (only possible with LOCK = 1)
		 other bits cannot be set when LOCK is = 1 */

#if defined(HS8200) \
 || defined(FS9000) \
 || defined(HS7420) \
 || defined(HS7429) \
 || defined(HS7810A) \
 || defined(HS7819) \
 || defined(CUBEBOX) \
 || defined(OPT9600) \
 || defined(OPT9600PRIMA)
	starci_writereg(state, POWER_CTRL_REG, 0x21);  // ?? data sheet says bit 5 is don't care
#else
	starci_writereg(state, POWER_CTRL_REG, 0x01);
#endif

#if !defined(HS8200) \
 && !defined(FS9000) \
 && !defined(HS7420) \
 && !defined(HS7429) \
 && !defined(HS7810A) \
 && !defined(HS7819) \
 && !defined(OPT9600) \
 || defined(OPT9600PRIMA)
	ctrl_outl(0x0, reg_config + EMI_LCK);
	ctrl_outl(0x0, reg_config + EMI_GEN_CFG);
#endif

#if defined(FS9000)
/* fixme: this is mysterious on FS9000! there is no lock setting EMI_LCK and there is
 * no EMI_CLK_EN, so the settings cant get into effect?
 */
	ctrl_outl(0x008486d9,reg_config + EMIBank1 + EMI_CFG_DATA0);
	ctrl_outl(0x9d220000,reg_config + EMIBank1 + EMI_CFG_DATA1);
	ctrl_outl(0x9d220000,reg_config + EMIBank1 + EMI_CFG_DATA2);
	ctrl_outl(0x00000008,reg_config + EMIBank1 + EMI_CFG_DATA3);

#elif defined(HS8200) \
 ||   defined(HS7420) \
 ||   defined(HS7429) \
 ||   defined(HS7810A) \
 ||   defined(HS7819)
	ctrl_outl(0x008486d9, reg_config + EMIBank3 + EMI_CFG_DATA0);
	ctrl_outl(0x9d220000, reg_config + EMIBank3 + EMI_CFG_DATA2);
	ctrl_outl(0x00000008, reg_config + EMIBank3 + EMI_CFG_DATA3);
	ctrl_outl(0x00000000, reg_config + EMI_GEN_CFG);
#else /* Cuberevo, TF7700 & Opticum HD (TS) 9600 */
	ctrl_outl(EMI_DATA0_WE_USE_OE(0x0) |
	          EMI_DATA0_WAIT_POL(0x0) |
	          EMI_DATA0_LATCH_POINT(30) |
	          EMI_DATA0_DATA_DRIVE(12) |
	          EMI_DATA0_BUS_RELEASE(50) |
	          EMI_DATA0_CS_ACTIVE(0x3) |
	          EMI_DATA0_OE_ACTIVE(0x1) |
	          EMI_DATA0_BE_ACTIVE(0x2) |
	          EMI_DATA0_PORT_SIZE(0x2) |
	          EMI_DATA0_DEVICE_TYPE(0x1), reg_config + EMIBank2 + EMI_CFG_DATA0);
	ctrl_outl(EMI_DATA1_CYCLE(0x1) |
	          EMI_DATA1_ACCESS_READ(100) |
	          EMI_DATA1_CSE1_READ(0) |
	          EMI_DATA1_CSE2_READ(0) |
	          EMI_DATA1_OEE1_READ(10) |
	          EMI_DATA1_OEE2_READ(10) |
	          EMI_DATA1_BEE1_READ(10) |
	          EMI_DATA1_BEE2_READ(10), reg_config + EMIBank2 + EMI_CFG_DATA1);
	ctrl_outl(EMI_DATA2_CYCLE(1) |
	          EMI_DATA2_ACCESS_WRITE(100) |
              EMI_DATA2_CSE1_WRITE(0) |
              EMI_DATA2_CSE2_WRITE(0) |
              EMI_DATA2_OEE1_WRITE(10) |
              EMI_DATA2_OEE2_WRITE(10) |
              EMI_DATA2_BEE1_WRITE(10) |
              EMI_DATA2_BEE2_WRITE(10), reg_config + EMIBank2 + EMI_CFG_DATA2);

	ctrl_outl(0x0, reg_config + EMIBank2 + EMI_CFG_DATA3);

#if defined(CUBEBOX)
	ctrl_outl(0x2, reg_config + EMI_FLASH_CLK_SEL);
#else
	ctrl_outl(0x0, reg_config + EMI_FLASH_CLK_SEL);
#endif

#endif

#if !defined(HS8200) \
 && !defined(FS9000) \
 && !defined(HS7110) \
 && !defined(HS7119) \
 && !defined(HS7810A) \
 && !defined(HS7819) \
 && !defined(OPT9600) \
 || defined(OPT9600PRIMA)
	ctrl_outl(0x1, reg_config + EMI_CLK_EN);
#endif

#if defined(FS9000)
//is [0] = top slot?
	slot_membase[0] = ioremap(0xa2000000, 0x1000);
#elif defined(HS8200)
	slot_membase[0] = ioremap(0x06800000, 0x1000);
#elif defined(HS7420) \
 ||   defined(HS7429) \
 ||   defined(HS7810A) \
 ||   defined(HS7819)
	slot_membase[0] = ioremap(0x06000000, 0x1000);
#elif defined(CUBEBOX)
	slot_membase[0] = ioremap(0x3000000, 0x1000);
	printk("membase-0 0x%08x\n", slot_membase[0]);
#elif defined(OPT9600) \
 || defined(OPT9600PRIMA)
//is [0] = top slot?
	slot_membase[0] = ioremap(0x3000000, 0x1000);
#else
	slot_membase[0] = ioremap(0xa3000000, 0x1000);
#endif
	if (slot_membase[0] == NULL)
	{
		result = 1;
		goto error;
	}
#if defined(TF7700)
	slot_membase[1] = ioremap(0xa3400000, 0x1000);
#elif defined(FS9000)
//is [1] = bottom slot?
	slot_membase[1] = ioremap(0xa2010000, 0x1000);
#elif defined(HS8200)
	slot_membase[1] = ioremap(0x06810000, 0x1000);
#elif defined(HS7420) \
 ||   defined(HS7429) \
 ||   defined(HS7810A) \
 ||   defined(HS7819)
	slot_membase[1] = ioremap(0x06010000, 0x1000);
#elif defined(CUBEBOX)
	slot_membase[1] = ioremap( 0x3010000, 0x1000 );
	printk("membase-1 0x%08x\n", slot_membase[1]);
#elif defined(OP9600) \
 || defined(OPT9600PRIMA)
//is [1] = bottom slot?
	slot_membase[1] = ioremap(0x3010000, 0x1000);
#else
	slot_membase[1] = ioremap(0xa3010000, 0x1000);
#endif
	if (slot_membase[1] == NULL)
	{
		iounmap(slot_membase[0]);
		result = 1;
		goto error;
	}

#if !defined(HS8200) \
 && !defined(FS9000) \
 && !defined(HS7420) \
 && !defined(HS7429) \
 && !defined(HS7810A) \
 && !defined(HS7819) \
 && !defined(OPT9600) \
 || defined(OPT9600PRIMA)
	slot_membase[1] = ioremap(0xa2010000, 0x1000);
	ctrl_outl(0x1F, reg_config + EMI_LCK);
#endif

	dprintk(20, "%s: call dvb_ca_en50221_init\n", __func__);

#if defined(CUBEREVO_250HD) \
 || defined(CUBEREVO_2000HD) \
 || defined(CUBEREVO_MINI_FTA)
	if ((result = dvb_ca_en50221_init(state->dvb_adap, &state->ca, 0, 1)) != 0)
	{
#else
	if ((result = dvb_ca_en50221_init(state->dvb_adap, &state->ca, 0, 2)) != 0)
	{
#endif
		dprintk(1, "%s Error: ca0 initialisation failed.\n", __func__);
		goto error;
	}
	dprintk(20, "ca0 interface initialised.\n");

error:
	dprintk(1, "%s: < Error = %d\n", __func__, result);
	return result;

pio_init_fail3:
	stpio_free_pin(module_B_pin);
pio_init_fail2:
	stpio_free_pin(module_A_pin);
pio_init_fail1:
	stpio_free_pin(cic_enable_pin);
	dprintk(1, "%s Error: Initializing PIO pins failed.\n", __func__);
	return result;
}

EXPORT_SYMBOL(init_ci_controller);
EXPORT_SYMBOL(setCiSource);
EXPORT_SYMBOL(getCiSource);

int starci2win_init(void)
{
		printk("Module starci2win loaded\n");
		return 0;
}

void starci2win_exit(void)
{
	printk("Module starci2win unloaded\n");
}

module_init             (starci2win_init);
module_exit             (starci2win_exit);

MODULE_DESCRIPTION      ("CI Controller driver for StarCI2Win, StarCI & CIcore1.0");
MODULE_AUTHOR           ("");
MODULE_LICENSE          ("GPL");

module_param(paramDebug, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(paramDebug, "Debug Output 0=disabled >0=enabled(debuglevel)");

module_param(extmoduldetect, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(extmoduldetect, "Ext. Modul detect 0=disabled 1=enabled");
// vim:ts=4
