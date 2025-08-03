#ifndef __BACKEND_RV64_PASSES_OPTIMIZE_RV64_CSE_H__
#define __BACKEND_RV64_PASSES_OPTIMIZE_RV64_CSE_H__

#include <backend/base_pass.h>
#include <backend/rv64/rv64_function.h>
#include <backend/rv64/rv64_defs.h>
#include <vector>
#include <map>
#include <set>
#include <string>

namespace Backend::RV64::Passes::Optimize
{
    struct RV64InstCSEInfo
    {
        Backend::RV64::InstType     inst_type;
        Backend::RV64::RV64InstType rv64_opcode;

        std::vector<std::string> operand_list;

        std::string move_src_type;
        std::string move_dst_type;

        bool        use_label;
        std::string label_name;
        int         imme_value;

        bool operator<(const RV64InstCSEInfo& other) const
        {
            if (inst_type != other.inst_type) return inst_type < other.inst_type;

            if (inst_type == Backend::RV64::InstType::RV64)
            {
                if (rv64_opcode != other.rv64_opcode) return rv64_opcode < other.rv64_opcode;
                if (use_label != other.use_label) return use_label < other.use_label;
                if (use_label && label_name != other.label_name) return label_name < other.label_name;
                if (!use_label && imme_value != other.imme_value) return imme_value < other.imme_value;
            }
            else if (inst_type == Backend::RV64::InstType::MOVE)
            {
                if (move_src_type != other.move_src_type) return move_src_type < other.move_src_type;
                if (move_dst_type != other.move_dst_type) return move_dst_type < other.move_dst_type;
            }

            return operand_list < other.operand_list;
        }

        bool operator==(const RV64InstCSEInfo& other) const { return !(*this < other) && !(other < *this); }
    };

    class RV64CSEPass : public Backend::BasePass
    {
      public:
        explicit RV64CSEPass(std::vector<Backend::RV64::Function*>& functions);
        ~RV64CSEPass() override = default;

        bool        run() override;
        const char* getName() const override { return "RV64CSE"; }

      private:
        std::vector<Backend::RV64::Function*>& functions_;

        std::set<Backend::RV64::Instruction*> inst_cse_map;
        std::map<int, int>                    reg_replace_map;
        std::set<Backend::RV64::Instruction*> erase_set;

        void optimizeFunction(Backend::RV64::Function* func);
        bool basicBlockCSE(Backend::RV64::Block* block);
        void domTreeWalkCSE(Backend::RV64::Function* func);
        void dfsDomTree(Backend::RV64::Function* func, int block_id);

        RV64InstCSEInfo getCSEInfo(Backend::RV64::Instruction* inst);
        bool            isCSECandidate(Backend::RV64::Instruction* inst);
        bool            isSafeToCSE(Backend::RV64::Instruction* inst, int existing_reg, int new_reg);
        bool            hasPhysicalReg(Backend::RV64::Instruction* inst);
        bool            isSameInstruction(Backend::RV64::Instruction* inst1, Backend::RV64::Instruction* inst2);
        std::string     registerToString(Backend::RV64::Register reg);

        void applyCSE(Backend::RV64::Function* func);
        void applyRegisterReplacement(Backend::RV64::Function* func);
        void eraseInstructions(Backend::RV64::Function* func);
        void eliminateDeadCode(Backend::RV64::Function* func);
        void replaceRegisters(Backend::RV64::Function* func);
    };

}  // namespace Backend::RV64::Passes::Optimize

#endif  // __BACKEND_RV64_PASSES_OPTIMIZE_RV64_CSE_H__
