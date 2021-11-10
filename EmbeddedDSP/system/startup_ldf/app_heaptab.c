/*
** ADSP-21479 user heap source file generated on Nov 10, 2021 at 15:57:04.
*/
/*
** Copyright (C) 2000-2020 Analog Devices Inc., All Rights Reserved.
**
** This file is generated automatically based upon the options selected
** in the System Configuration utility. Changes to the Heap configuration
** should be made by modifying the appropriate options rather than editing
** this file. To access the System Configuration utility, double-click the
** system.svc file from a navigation view.
**
** Custom additions can be inserted within the user-modifiable sections,
** these are bounded by comments that start with "$VDSG". Only changes
** placed within these sections are preserved when this file is re-generated.
**
** Product      : CrossCore Embedded Studio
** Tool Version : 6.2.1.6
*/

#ifdef _MISRA_RULES
#pragma diag(push)
#pragma diag(suppress:misra_rule_1_1)
#pragma diag(suppress:misra_rule_6_3)
#pragma diag(suppress:misra_rule_8_10)
#pragma diag(suppress:misra_rule_10_1_a)
#pragma diag(suppress:misra_rule_11_3)
#endif /* _MISRA_RULES */

#pragma file_attr("libData=HeapTable")

#include <stdlib.h>
#include <limits.h>

extern "asm" unsigned long ldf_heap_space;
extern "asm" unsigned long ldf_heap_length;
extern "asm" int slot1_space;
extern "asm" int slot1_length;
extern "asm" int slot2_space;
extern "asm" int slot2_length;

const __heap_desc_t heap_table[4] =
{

  { &ldf_heap_space, (unsigned long) &ldf_heap_length, 0 },
  { &slot1_space, (unsigned long) &slot1_length, 1 },
  { &slot2_space, (unsigned long) &slot2_length, 2 },

  { (void *)0, (size_t)0, 0 } /* This terminates the table. */
};

#ifdef _MISRA_RULES
#pragma diag(pop)
#endif /* _MISRA_RULES */

