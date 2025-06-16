/* Definitions of target machine for the GNU compiler collection for
   Microchip (formerly Atmel) AVR micro controller if configured for MUSL.
   Copyright (C) 2021 Free Software Foundation, Inc.
   Contributed by Pitchumani Sivanupandi (pitchumani.sivanupandi@microchip.com)

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

/* The specs for musl is same as avrlibc.

   libc.a and libm.a
   - musl will be built to output libc.a and libm.a
   - libm.a will have floating point routines from avrlibc

   lib<device>.a
   - device library will be built from avrlibc, available as part of DFP

   crt<device>.o
   - startup file also built from avrlibc, available as part of DFP
*/

#undef  LIB_SPEC
#define LIB_SPEC                                \
  "%{!mmcu=avr1:-lc} %(avrlibc_devicelib)"

#undef  LIBGCC_SPEC
#define LIBGCC_SPEC                             \
  "%{!mmcu=avr1:-lgcc -lm}"

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC                          \
  " %(avrlibc_startfile) "

#undef  LINK_GCC_C_SEQUENCE_SPEC
#define LINK_GCC_C_SEQUENCE_SPEC \
  "--start-group %G %L --end-group"
