#pragma once

#include <SupportDefs.h>


void DispatchSyscall(uint32 op, uint64 *args, uint64 *_returnValue);
