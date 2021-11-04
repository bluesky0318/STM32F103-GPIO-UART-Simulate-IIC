#ifndef __OZ37210_H
#define __OZ37210_H

#include "iic.h"
void WriteWord(u8 addr ,u16 udata);
u16 ReadWord(u8 addr);
#endif
