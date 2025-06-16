/* Specs definitions for Atmel AVR back end.

   Copyright (C) 2012-2015 Free Software Foundation, Inc.
   Contributed by Georg-Johann Lay (avr@gjlay.de)

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


/* Default specs layout.  The actual definitions might be superseeded
   by device- or OS- specific files, like musl.h, avrlibc.h, ../rtems.h, etc.
   The specs are repeated in the device specs files.  Subspecs are
   specs known to GCC or specs defined in the device specs files.  */


#undef  CPLUSPLUS_CPP_SPEC
#define CPLUSPLUS_CPP_SPEC                      \
  "%(cpp)"

#undef  CC1_SPEC
#define CC1_SPEC                                \
  "%(cc1_n_flash) "                             \
  "%(cc1_errata_skip) "                         \
  "%(cc1_rmw) "                                 \
  "%(cc1_non_bit_addressable_registers_mask) "  \
  "%(cc1_absdata) "                             \
  "%(cc1_mdiva_spec) "

#undef  CC1PLUS_SPEC
#define CC1PLUS_SPEC                                    \
  "%(cc1) "                                             \
  "%{!frtti:-fno-rtti} "                                \
  "%{!fenforce-eh-specs:-fno-enforce-eh-specs} "        \
  "%{!fexceptions:-fno-exceptions} "

#define ASM_RELAX_SPEC                          \
  "%{mrelax:--mlink-relax} "

#undef  ASM_SPEC
#define ASM_SPEC                                \
  "%(asm_arch) "                                \
  "%(asm_relax) "                               \
  "%(asm_rmw) "                                 \
  "%(asm_errata_skip) "                         \
  "%{!mno-gas-isr-prologues: -mgcc-isr} "        \
  "%{mconst-data-in-config-mapped-progmem:-mconst-data-in-config-mapped-progmem} "

#define LINK_ARCH_SPEC                          \
  "%{mmcu=*:-m%*} "

#define LINK_RELAX_SPEC                         \
  "%{mrelax:--relax} "

#undef  LINK_SPEC
#define LINK_SPEC                               \
  "%(link_arch) "                               \
  "%(link_data_start) "                         \
  "%(link_text_start) "                         \
  "%(link_relax) "                              \
  "%(link_pmem_wrap) "                          \
  "%(link_fuse_action_define_appdata) "         \
  "%(link_fuse_action_detect_illegal_opcode) "  \
  "%{mno-data-init:--no-data-init} "            \
  "%{mpa:%{mshort-calls | \
           mmcu=avr1 | \
           mmcu=avr2 | \
           mmcu=avr25 | \
           mmcu=avr4 | \
           mmcu=avrtiny : -mpa-short-call}} "     \
  "%{mchp-stack-usage:--mchp-stack-usage        \
       %{mmcu=avr6 | mmcu=avrxmega6 |           \
         mmcu=avrxmega7 : --pc-size=3; :--pc-size=2}}"        \
  "%{mconst-data-in-config-mapped-progmem:--const-data-in-config-mapped-progmem} " \
  "%{msafety-device-features:--append-nop} " \
  "%{mafrlcsj:--mafrlcsj}" \
  "%{shared:%eshared is not supported} " \
  "%{mreserve=*:--mreserve=%* } " \
  "%{!mno-fuse-action:%{mfuse-action=*:--fuse-action=%*}} " \
  "%{!mno-fuse-action:%{mfuse-action-param=*:--fuse-action-param=%*}} "


#undef  LIB_SPEC
#define LIB_SPEC " %{!mmcu=avr1:-lc} "

#undef  LIBGCC_SPEC
#define LIBGCC_SPEC " %{!mmcu=avr1:-lgcc} "

#define STARTFILE_SPEC ""
#define ENDFILE_SPEC ""
