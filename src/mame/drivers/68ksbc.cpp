// license:BSD-3-Clause
// copyright-holders:Robbbert
/***************************************************************************

    Skeleton driver for 68k Single Board Computer

    29/03/2011

    http://www.kmitl.ac.th/~kswichit/68k/68k.html

    TODO:
    - Add RTC (type DS12887)

    All of the address and i/o decoding is done by a pair of XC9536
    mask-programmed custom devices.

    There are some chips used for unclear purposes (GPI, GPO, LCD).

    This computer has no sound, and no facility for saving or loading programs.

    When started, press ? key for a list of commands.

    Has 1MB of flash (which is actually just 12k of program),
    and 1MB of RAM. Memory map should probably be corrected.


****************************************************************************/

#include "emu.h"
#include "bus/rs232/rs232.h"
#include "cpu/m68000/m68000.h"
#include "machine/6850acia.h"
#include "machine/clock.h"


class c68ksbc_state : public driver_device
{
public:
	c68ksbc_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
	{ }

	void c68ksbc(machine_config &config);
	void c68ksbc_mem(address_map &map);
private:
	required_device<cpu_device> m_maincpu;
};

void c68ksbc_state::c68ksbc_mem(address_map &map)
{
	map.unmap_value_high();
	map(0x000000, 0x002fff).rom();
	map(0x003000, 0x5fffff).ram();
	map(0x600000, 0x600003).rw("acia", FUNC(acia6850_device::read), FUNC(acia6850_device::write)).umask16(0x00ff);
}


/* Input ports */
static INPUT_PORTS_START( c68ksbc )
INPUT_PORTS_END


MACHINE_CONFIG_START(c68ksbc_state::c68ksbc)
	/* basic machine hardware */
	MCFG_DEVICE_ADD("maincpu", M68000, 8000000) // text says 8MHz, schematic says 10MHz
	MCFG_DEVICE_PROGRAM_MAP(c68ksbc_mem)

	MCFG_DEVICE_ADD("acia", ACIA6850, 0)
	MCFG_ACIA6850_TXD_HANDLER(WRITELINE("rs232", rs232_port_device, write_txd))
	MCFG_ACIA6850_RTS_HANDLER(WRITELINE("rs232", rs232_port_device, write_rts))

	MCFG_DEVICE_ADD("rs232", RS232_PORT, default_rs232_devices, "terminal")
	MCFG_RS232_RXD_HANDLER(WRITELINE("acia", acia6850_device, write_rxd))
	MCFG_RS232_CTS_HANDLER(WRITELINE("acia", acia6850_device, write_cts))

	MCFG_DEVICE_ADD("acia_clock", CLOCK, 153600)
	MCFG_CLOCK_SIGNAL_HANDLER(WRITELINE("acia", acia6850_device, write_txc))
	MCFG_DEVCB_CHAIN_OUTPUT(WRITELINE("acia", acia6850_device, write_rxc))
MACHINE_CONFIG_END

/* ROM definition */
ROM_START( 68ksbc )
	ROM_REGION(0x1000000, "maincpu", 0)
	ROM_LOAD( "t68k.bin", 0x0000, 0x2f78, CRC(20a8d0d0) SHA1(544fd8bd8ed017115388c8b0f7a7a59a32253e43) )
ROM_END

/* Driver */

/*    YEAR  NAME    PARENT  COMPAT  MACHINE  INPUT    CLASS          INIT        COMPANY             FULLNAME                     FLAGS */
COMP( 2002, 68ksbc, 0,      0,      c68ksbc, c68ksbc, c68ksbc_state, empty_init, "Wichit Sirichote", "68k Single Board Computer", MACHINE_NO_SOUND_HW)
