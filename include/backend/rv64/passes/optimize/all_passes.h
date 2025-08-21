#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_ALL_PASSES_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_ALL_PASSES_H__

#include "control_flow/control_flow_passes.h"
#include "move/move_passes.h"
#include "arithmetic_strength_reduction.h"
#include "peehole/peehole_passes.h"
#include "rv64_cse.h"
#include "rv64_makedom.h"
#include "instruction_schedule.h"
#include "rv64_licm.h"
#include "redundant_arithmetic_elimination.h"
#include "../rv64_loop_find.h"

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_ALL_PASSES_H__
