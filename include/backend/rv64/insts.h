#ifndef __BACKEND_RV64_INSTS_H__
#define __BACKEND_RV64_INSTS_H__

#include "defs.h"

namespace Backend::RV64
{
    RV64Inst* createRInst(RV64InstType op, Register rd, Register rs1, Register rs2);
    RV64Inst* createR2Inst(RV64InstType op, Register rd, Register rs);

    RV64Inst* createIInst(RV64InstType op, Register rd, Register rs1, int imme);
    RV64Inst* createIInst(RV64InstType op, Register rd, Register rs1, RV64Label label);

    RV64Inst* createSInst(RV64InstType op, Register val, Register ptr, int imme);
    RV64Inst* createSInst(RV64InstType op, Register val, Register ptr, RV64Label label);

    RV64Inst* createBInst(RV64InstType op, Register rs1, Register rs2, RV64Label label);

    RV64Inst* createUInst(RV64InstType op, Register rd, int imme);
    RV64Inst* createUInst(RV64InstType op, Register rd, RV64Label label);

    RV64Inst* createJInst(RV64InstType op, Register rd, RV64Label label);

    RV64Inst* createCallInst(RV64InstType op, RV64Label label);

    MoveInst* createMoveInst(DataType* type, Register dst, Register src);
    MoveInst* createMoveInst(DataType* type, Register dst, int src);
    MoveInst* createMoveInst(DataType* type, Register dst, float src);
    MoveInst* createMoveInst(DataType* type, Register dst, double src);

    RV64Inst* createCallInst(RV64InstType op, std::string name, int ireg_para_cnt, int freg_para_cnt);
}  // namespace Backend::RV64

#endif  // __BACKEND_RV64_INSTS_H__
