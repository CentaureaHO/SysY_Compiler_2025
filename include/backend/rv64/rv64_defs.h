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

#ifdef RV64_ENABLE_ZBA
#undef RV64_ENABLE_ZBA
#endif
#ifdef RV64_ENABLE_ZBB
#undef RV64_ENABLE_ZBB
#endif
#ifdef RV64_ENABLE_ZICSR
#undef RV64_ENABLE_ZICSR
#endif
#ifdef RV64_ENABLE_ZIFENCEI
#undef RV64_ENABLE_ZIFENCEI
#endif
#ifdef RV64_ENABLE_ZICOND
#undef RV64_ENABLE_ZICOND
#endif

#define RV64_ENABLE_ZBA 0  // 评测平台上超时
#define RV64_ENABLE_ZBB 1
#define RV64_ENABLE_ZICSR 0
#define RV64_ENABLE_ZIFENCEI 0
#define RV64_ENABLE_ZICOND 0  // 性能不如位操作，吗？至少目前评测来看是不如

// (name, type, _asm, latency)
#define RV64_INSTS_BASE          \
    X(ADD, R, add, 1)            \
    X(ADDW, R, addw, 1)          \
    X(SUB, R, sub, 1)            \
    X(SUBW, R, subw, 1)          \
    X(MUL, R, mul, 3)            \
    X(MULW, R, mulw, 3)          \
    X(DIV, R, div, 30)           \
    X(DIVW, R, divw, 30)         \
    X(FADD_S, R, fadd.s, 5)      \
    X(FSUB_S, R, fsub.s, 5)      \
    X(FMUL_S, R, fmul.s, 5)      \
    X(FDIV_S, R, fdiv.s, 30)     \
    X(REM, R, rem, 30)           \
    X(REMW, R, remw, 30)         \
    X(SLL, R, sll, 1)            \
    X(SRL, R, srl, 1)            \
    X(SRA, R, sra, 1)            \
    X(AND, R, and, 1)            \
    X(OR, R, or, 1)              \
    X(XOR, R, xor, 1)            \
    X(SLT, R, slt, 1)            \
    X(SLTU, R, sltu, 1)          \
    X(FEQ_S, R, feq.s, 4)        \
    X(FLT_S, R, flt.s, 4)        \
    X(FLE_S, R, fle.s, 4)        \
    X(FMIN_S, R, fmin.s, 4)      \
    X(FMAX_S, R, fmax.s, 4)      \
                                 \
    X(ADDI, I, addi, 1)          \
    X(ADDIW, I, addiw, 1)        \
    X(SLLI, I, slli, 1)          \
    X(SRLI, I, srli, 1)          \
    X(SRAI, I, srai, 1)          \
    X(SLLIW, I, slliw, 1)        \
    X(SRLIW, I, srliw, 1)        \
    X(SRAIW, I, sraiw, 1)        \
    X(ANDI, I, andi, 1)          \
    X(ORI, I, ori, 1)            \
    X(XORI, I, xori, 1)          \
    X(SLTI, I, slti, 1)          \
    X(SLTIU, I, sltiu, 1)        \
    X(JALR, I, jalr, 1)          \
    X(LW, I, lw, 3)              \
    X(LD, I, ld, 3)              \
    X(FLW, I, flw, 2)            \
    X(FLD, I, fld, 2)            \
                                 \
    X(LI, U, li, 1)              \
    X(LUI, U, lui, 1)            \
    X(LA, U, la, 1)              \
                                 \
    X(SW, S, sw, 1)              \
    X(SD, S, sd, 1)              \
    X(FSW, S, fsw, 4)            \
    X(FSD, S, fsd, 4)            \
                                 \
    X(BEQ, B, beq, 1)            \
    X(BNE, B, bne, 1)            \
    X(BLT, B, blt, 1)            \
    X(BGE, B, bge, 1)            \
    X(BLTU, B, bltu, 1)          \
    X(BGEU, B, bgeu, 1)          \
    X(BGT, B, bgt, 1)            \
    X(BLE, B, ble, 1)            \
    X(BGTU, B, bgtu, 1)          \
    X(BLEU, B, bleu, 1)          \
                                 \
    X(JAL, J, jal, 1)            \
                                 \
    X(FMV_W_X, R2, fmv.w.x, 2)   \
    X(FMV_X_W, R2, fmv.x.w, 1)   \
    X(FCVT_S_W, R2, fcvt.s.w, 2) \
    X(FCVT_W_S, R2, fcvt.w.s, 4) \
    X(FMV_S, R2, fmv.s, 2)       \
    X(FMV_D, R2, fmv.d, 2)       \
    X(ZEXT_W, R2, zext.w, 1)     \
    X(FNEG_S, R2, fneg.s, 2)     \
                                 \
    X(FMADD_S, R4, fmadd.s, 5)   \
    X(FMSUB_S, R4, fmsub.s, 5)   \
    X(FNMADD_S, R4, fnmadd.s, 5) \
    X(FNMSUB_S, R4, fnmsub.s, 5) \
                                 \
    X(CALL, CALL, call, 1)

#if RV64_ENABLE_ZBA
#define RV64_INSTS_ZBA           \
    X(SH1ADD, R, sh1add, 1)      \
    X(SH2ADD, R, sh2add, 1)      \
    X(SH3ADD, R, sh3add, 1)      \
    X(SH1ADDUW, R, sh1add.uw, 1) \
    X(SH2ADDUW, R, sh2add.uw, 1) \
    X(SH3ADDUW, R, sh3add.uw, 1)
#else
#define RV64_INSTS_ZBA
#endif

#if RV64_ENABLE_ZBB
#define RV64_INSTS_ZBB  \
    X(MIN, R, min, 1)   \
    X(MAX, R, max, 1)   \
    X(MINU, R, minu, 1) \
    X(MAXU, R, maxu, 1)
#else
#define RV64_INSTS_ZBB
#endif

#if RV64_ENABLE_ZICOND
#define RV64_INSTS_ZICOND         \
    X(CZERO_EQZ, R, czero.eqz, 1) \
    X(CZERO_NEZ, R, czero.nez, 1)
#else
#define RV64_INSTS_ZICOND
#endif

#define RV64_INSTS  \
    RV64_INSTS_BASE \
    RV64_INSTS_ZBA  \
    RV64_INSTS_ZBB  \
    RV64_INSTS_ZICOND

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
    extern bool can_schedule;
    extern bool force_no_schedule;

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
        RV64   = 0,
        PHI    = 1,
        MOVE   = 2,
        SELECT = 3
    };

    enum class RV64InstType
    {
#define X(name, type, _asm, latency) name,
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

        bool schedule_flag;

      public:
        static void delInst(Instruction* i) { delete i; }

      protected:
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
        static RV64Inst* getInstance()
        {
            RV64Inst* ret      = new RV64Inst();
            ret->schedule_flag = can_schedule;

            if (force_no_schedule) ret->schedule_flag = false;

            return ret;
        }

      private:
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
        static PhiInst* getInstance(Register r)
        {
            PhiInst* ret       = new PhiInst(r);
            ret->schedule_flag = can_schedule;

            if (force_no_schedule) ret->schedule_flag = false;

            return ret;
        }

      private:
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
        static MoveInst* getInstance(DataType* dt, Operand* s, Operand* d)
        {
            MoveInst* ret      = new MoveInst(dt, s, d);
            ret->schedule_flag = can_schedule;

            if (force_no_schedule) ret->schedule_flag = false;

            return ret;
        }

      private:
        MoveInst(DataType* dt, Operand* s, Operand* d);
        ~MoveInst();

      public:
        std::vector<Register*> getReadRegs() override;
        std::vector<Register*> getWriteRegs() override;
        void                   replaceAllOperands(const std::map<int, int>& reg_replace_map) override;
    };

    class SelectInst : public Instruction
    {
      public:
        Register cond_reg;
        Register dst_reg;
        Operand* true_val;
        Operand* false_val;

      public:
        static SelectInst* getInstance(Register cond, Register dst, Operand* tv, Operand* fv)
        {
            SelectInst* ret    = new SelectInst(cond, dst, tv, fv);
            ret->schedule_flag = can_schedule;

            if (force_no_schedule) ret->schedule_flag = false;

            return ret;
        }

      private:
        SelectInst(Register cond, Register dst, Operand* tv, Operand* fv);
        ~SelectInst();

      public:
        std::vector<Register*> getReadRegs() override;
        std::vector<Register*> getWriteRegs() override;
        void                   replaceAllOperands(const std::map<int, int>& reg_replace_map) override;
    };

    class NopInst : public Instruction
    {
      private:
        NopInst();
        ~NopInst();
    };

    extern std::map<RV64InstType, OpInfo> opInfoTable;
    extern std::map<int, std::string>     rv64_reg_name_map;

    DataType* getPhyRegType(int reg_num);
    Register  getPhyReg(int reg_num);

#define X(name, alias, saver) extern Register preg_##alias;
    RV64_REGS
#undef X

    inline std::string getRV64ArchString()
    {
        std::string arch = "rv64i2p1";

        arch += "_m2p0";  // M
        arch += "_a2p1";  // A
        arch += "_f2p2";  // F
        arch += "_d2p2";  // D
        arch += "_c2p0";  // C

#if RV64_ENABLE_ZICSR
        arch += "_zicsr2p0";
#endif
#if RV64_ENABLE_ZIFENCEI
        arch += "_zifencei2p0";
#endif
#if RV64_ENABLE_ZBA
        arch += "_zba1p0";
#endif
#if RV64_ENABLE_ZBB
        arch += "_zbb1p0";
#endif
#if RV64_ENABLE_ZICOND
        arch += "_zicond1p0";
#endif
        return arch;
    }
}  // namespace Backend::RV64

#endif  // __BACKEND_RV64_DEFS_H__
