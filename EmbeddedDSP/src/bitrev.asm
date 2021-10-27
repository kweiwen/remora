#include <def21479.h>
#include <asm_sprt.h>

.global _bitrev;
.extern _command;

.section/pm seg_pmco;
_bitrev:
entry;

i1=dm(_command);
bitrev(i1,0);
dm(_command)=i1;


exit;
_bitrev.end:
