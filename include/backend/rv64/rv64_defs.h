#ifndef __BACKEND_RV64_DEFS_H__
#define __BACKEND_RV64_DEFS_H__

#include <vector>
#include <utility>
#include <map>
#include <string>
#include <iostream>
#include <cassert>

#ifndef ERROR
#define ERROR(...)                                                                \
    do {                                                                          \
        char message[256];                                                        \
        sprintf(message, __VA_ARGS__);                                            \
        std::cerr << "\033[;31;1m";                                               \
        std::cerr << "ERROR: ";                                                   \
        std::cerr << "\033[0;37;1m";                                              \
        std::cerr << message << "\n";                                             \
        std::cerr << "\033[0;33;1m";                                              \
        std::cerr << "File: \033[4;37;1m" << __FILE__ << ":" << __LINE__ << "\n"; \
        std::cerr << "\033[0m";                                                   \
        assert(false);                                                            \
    } while (0)
#endif

#ifndef Assert
#define Assert(EXP)                                          \
    do {                                                     \
        if (!(EXP)) { ERROR("Assertion failed: %s", #EXP); } \
    } while (0)
#endif

#define RV64_INST_TYPES           \
    X(R)  /* R rd lhs rhs */      \
    X(I)  /* I rd base imme */    \
    X(S)  /* S val base shift */  \
    X(B)  /* B lhs rhs tar */     \
    X(U)  /* U rd val */          \
    X(J)  /* J rd tar */          \
    X(R2) /* R2 rd rs */          \
    X(R4) /* R4 rd rs1 rs2 rs3 */ \
    X(CALL)

// (name, type, asm)
#define RV64_INSTS            \
    X(ADD, R, add)            \
    X(ADDW, R, addw)          \
    X(SUB, R, sub)            \
    X(SUBW, R, subw)          \
    X(MUL, R, mul)            \
    X(MULW, R, mulw)          \
    X(DIV, R, div)            \
    X(DIVW, R, divw)          \
    X(FADD_S, R, fadd.s)      \
    X(FSUB_S, R, fsub.s)      \
    X(FMUL_S, R, fmul.s)      \
    X(FDIV_S, R, fdiv.s)      \
    X(REM, R, rem)            \
    X(REMW, R, remw)          \
    X(SLL, R, sll)            \
    X(SRL, R, srl)            \
    X(SRA, R, sra)            \
    X(AND, R, and)            \
    X(OR, R, or)              \
    X(XOR, R, xor)            \
    X(SLT, R, slt)            \
    X(SLTU, R, sltu)          \
    X(FEQ_S, R, feq.s)        \
    X(FLT_S, R, flt.s)        \
    X(FLE_S, R, fle.s)        \
                              \
    X(ADDI, I, addi)          \
    X(SLLI, I, slli)          \
    X(SRLI, I, srli)          \
    X(SRAI, I, srai)          \
    X(SLLIW, I, slliw)        \
    X(SRLIW, I, srliw)        \
    X(SRAIW, I, sraiw)        \
    X(ANDI, I, andi)          \
    X(ORI, I, ori)            \
    X(XORI, I, xori)          \
    X(SLTI, I, slti)          \
    X(SLTIU, I, sltiu)        \
    X(JALR, I, jalr)          \
    X(LW, I, lw)              \
    X(LD, I, ld)              \
    X(FLW, I, flw)            \
    X(FLD, I, fld)            \
                              \
    X(LI, U, li)              \
    X(LUI, U, lui)            \
    X(LA, U, la)              \
                              \
    X(SW, S, sw)              \
    X(SD, S, sd)              \
    X(FSW, S, fsw)            \
    X(FSD, S, fsd)            \
                              \
    X(BEQ, B, beq)            \
    X(BNE, B, bne)            \
    X(BLT, B, blt)            \
    X(BGE, B, bge)            \
    X(BLTU, B, bltu)          \
    X(BGEU, B, bgeu)          \
    X(BGT, B, bgt)            \
    X(BLE, B, ble)            \
    X(BGTU, B, bgtu)          \
    X(BLEU, B, bleu)          \
                              \
    X(JAL, J, jal)            \
                              \
    X(FMV_W_X, R2, fmv.w.x)   \
    X(FMV_X_W, R2, fmv.x.w)   \
    X(FCVT_S_W, R2, fcvt.s.w) \
    X(FCVT_W_S, R2, fcvt.w.s) \
    X(FMV_S, R2, fmv.s)       \
    X(FMV_D, R2, fmv.d)       \
    X(ZEXT_W, R2, zext.w)     \
    X(FNEG_S, R2, fneg.s)     \
                              \
    X(FMADD_S, R4, fmadd.s)   \
    X(FMSUB_S, R4, fmsub.s)   \
    X(FNMADD_S, R4, fnmadd.s) \
    X(FNMSUB_S, R4, fnmsub.s) \
                              \
    X(SH1ADD, R, sh1add)      \
    X(SH2ADD, R, sh2add)      \
    X(SH3ADD, R, sh3add)      \
    X(SH1ADDUW, R, sh1add.uw) \
    X(SH2ADDUW, R, sh2add.uw) \
    X(SH3ADDUW, R, sh3add.uw) \
                              \
    X(CALL, CALL, call)

// (name, alias, saver)
// saver: 0: caller-saved, 1: callee-saved, 2: other
#define RV64_REGS         \
    X(x0, x0, 2)          \
    X(x1, ra, 0)          \
    X(x2, sp, 2)          \
    X(x3, gp, 2)          \
    X(x4, tp, 2)          \
    X(x5, t0, 0)          \
    X(x6, t1, 0)          \
    X(x7, t2, 0)          \
    X(x8, fp, 1) /* s0 */ \
    X(x9, s1, 1)          \
    X(x10, a0, 0)         \
    X(x11, a1, 0)         \
    X(x12, a2, 0)         \
    X(x13, a3, 0)         \
    X(x14, a4, 0)         \
    X(x15, a5, 0)         \
    X(x16, a6, 0)         \
    X(x17, a7, 0)         \
    X(x18, s2, 1)         \
    X(x19, s3, 1)         \
    X(x20, s4, 1)         \
    X(x21, s5, 1)         \
    X(x22, s6, 1)         \
    X(x23, s7, 1)         \
    X(x24, s8, 1)         \
    X(x25, s9, 1)         \
    X(x26, s10, 1)        \
    X(x27, s11, 1)        \
    X(x28, t3, 0)         \
    X(x29, t4, 0)         \
    X(x30, t5, 0)         \
    X(x31, t6, 0)         \
    X(f0, ft0, 0)         \
    X(f1, ft1, 0)         \
    X(f2, ft2, 0)         \
    X(f3, ft3, 0)         \
    X(f4, ft4, 0)         \
    X(f5, ft5, 0)         \
    X(f6, ft6, 0)         \
    X(f7, ft7, 0)         \
    X(f8, fs0, 1)         \
    X(f9, fs1, 1)         \
    X(f10, fa0, 0)        \
    X(f11, fa1, 0)        \
    X(f12, fa2, 0)        \
    X(f13, fa3, 0)        \
    X(f14, fa4, 0)        \
    X(f15, fa5, 0)        \
    X(f16, fa6, 0)        \
    X(f17, fa7, 0)        \
    X(f18, fs2, 1)        \
    X(f19, fs3, 1)        \
    X(f20, fs4, 1)        \
    X(f21, fs5, 1)        \
    X(f22, fs6, 1)        \
    X(f23, fs7, 1)        \
    X(f24, fs8, 1)        \
    X(f25, fs9, 1)        \
    X(f26, fs10, 1)       \
    X(f27, fs11, 1)       \
    X(f28, ft8, 0)        \
    X(f29, ft9, 0)        \
    X(f30, ft10, 0)       \
    X(f31, ft11, 0)

#define MAT2(T) std::vector<std::vector<T>>

namespace Backend::RV64
{
    enum class REG
    {
#define X(name, alias, saver) name,
        RV64_REGS
#undef X
    };

    // ref: https://github.com/yuhuifishash/NKU-Compilers2024-RV64GC/blob/main/target/common/MachineBaseInstruction.h
    struct DataType
    {
        enum
        {
            INT,
            FLOAT
        };
        enum
        {
            B32,
            B64,
            B128
        };
        unsigned data_type;
        unsigned data_length;
        DataType() {}
        DataType(const DataType& other)
        {
            this->data_type   = other.data_type;
            this->data_length = other.data_length;
        }
        DataType operator=(const DataType& other)
        {
            this->data_type   = other.data_type;
            this->data_length = other.data_length;
            return *this;
        }
        DataType(unsigned data_type, unsigned data_length) : data_type(data_type), data_length(data_length) {}
        bool operator==(const DataType& other) const
        {
            return this->data_type == other.data_type && this->data_length == other.data_length;
        }
        int getDataWidth()
        {
            switch (data_length)
            {
                case B32: return 4;
                case B64: return 8;
                case B128: return 16;
            }
            return 0;
        }
        std::string toString()
        {
            std::string ret;
            if (data_type == INT) ret += 'i';
            if (data_type == FLOAT) ret += 'f';
            if (data_length == B32) ret += "32";
            if (data_length == B64) ret += "64";
            if (data_length == B128) ret += "128";
            return ret;
        }
    };

    extern DataType *INT32, *INT64, *INT128, *FLOAT32, *FLOAT64, *FLOAT128;

    enum class OperandType
    {
        REG     = 0,
        IMMEI32 = 1,
        IMMEF32 = 2,
        IMMEF64 = 3
    };

    enum class InstType
    {
        RV64 = 0,
        PHI  = 1,
        MOVE = 2
    };

    enum class RV64InstType
    {
#define X(name, type, _asm) name,
        RV64_INSTS
#undef X
    };

    enum class RV64OpType
    {
#define X(t) t,
        RV64_INST_TYPES
#undef X
    };

    struct OpInfo
    {
        std::string _asm;
        RV64OpType type;
        int        latency;  // Instruction latency for WAW optimization

        OpInfo();
        OpInfo(std::string a, RV64OpType t, int lat = 1);
    };

    class Register
    {
      public:
        int       reg_num;
        DataType* data_type;

        bool is_virtual;

      public:
        Register();
        Register(int reg, DataType* dt, bool is_v);

      public:
        bool operator<(Register other) const;
        bool operator==(Register other) const;
    };

    class Operand
    {
      public:
        DataType*   data_type;
        OperandType operand_type;

      public:
        Operand(OperandType ot);
        virtual ~Operand() = default;

      public:
        virtual std::string to_string() = 0;
    };

    class RegOperand : public Operand
    {
      public:
        Register reg;

      public:
        RegOperand(Register r);

      public:
        std::string to_string() override;
    };

    class ImmeI32Operand : public Operand
    {
      public:
        int val;

      public:
        ImmeI32Operand(int i32);

      public:
        std::string to_string() override;
    };

    class ImmeF32Operand : public Operand
    {
      public:
        float val;

      public:
        ImmeF32Operand(float f32);

      public:
        std::string to_string() override;
    };

    class ImmeF64Operand : public Operand
    {
      public:
        double val;

      public:
        ImmeF64Operand(double f64);

      public:
        std::string to_string() override;
    };

    class Instruction
    {
      public:
        InstType inst_type;

        int ins_id;

      public:
        Instruction(InstType it);
        virtual ~Instruction() = default;

      public:
        virtual std::vector<Register*> getReadRegs()                                                 = 0;
        virtual std::vector<Register*> getWriteRegs()                                                = 0;
        virtual void                   replaceAllOperands(const std::map<int, int>& reg_replace_map) = 0;
    };

    class RV64Label
    {
      public:
        std::string name;
        bool        is_data_addr;
        bool        is_hi;
        int         jmp_label;
        int         seq_label;
        bool        is_la;

        RV64Label(bool la = false);
        RV64Label(std::string n, bool hi, bool la = false);
        RV64Label(int jmp, int seq, bool la = false);
        RV64Label(int jmp, bool la = false);
    };

    class RV64Inst : public Instruction
    {
      public:
        RV64InstType op;
        bool         use_label;
        int          imme;
        RV64Label    label;
        Register     rd, rs1, rs2, rs3;

        int call_ireg_cnt, call_freg_cnt;

        /*
         * 0: void
         * 1: i32
         * 2: f32
         */
        int ret_type;

      public:
        RV64Inst();

      public:
        std::vector<Register*> getReadRegs() override;
        std::vector<Register*> getWriteRegs() override;
        void                   replaceAllOperands(const std::map<int, int>& reg_replace_map) override;
        int                    getLatency() const;
    };

    class PhiInst : public Instruction
    {
      public:
        Register res_reg;
        // std::map<Register, int> vals_for_labels;
        std::map<int, Operand*> phi_list;

      public:
        PhiInst(Register r);
        ~PhiInst();

      public:
        std::vector<Register*> getReadRegs() override;
        std::vector<Register*> getWriteRegs() override;
        void                   replaceAllOperands(const std::map<int, int>& reg_replace_map) override;
    };

    class MoveInst : public Instruction
    {
      public:
        DataType* data_type;
        Operand * src, *dst;

      public:
        MoveInst(DataType* dt, Operand* s, Operand* d);
        ~MoveInst();

      public:
        std::vector<Register*> getReadRegs() override;
        std::vector<Register*> getWriteRegs() override;
        void                   replaceAllOperands(const std::map<int, int>& reg_replace_map) override;
    };

    extern std::map<RV64InstType, OpInfo> opInfoTable;
    extern std::map<int, std::string>     rv64_reg_name_map;

    DataType* getPhyRegType(int reg_num);
    Register  getPhyReg(int reg_num);

#define X(name, alias, saver) extern Register preg_##alias;
    RV64_REGS
#undef X
}  // namespace Backend::RV64

#endif  // __BACKEND_RV64_DEFS_H__
