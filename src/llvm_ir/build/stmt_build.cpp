#include <ast/statement.h>
#include <ast/expression.h>
#include <ast/helper.h>
#include <llvm_ir/ir_builder.h>
#include <llvm_ir/build/type_trans.h>
#include <common/type/symtab/semantic_table.h>
#include <iostream>
using namespace std;
using namespace LLVMIR;

using DT = DataType;

extern SemanticTable::Table* semTable;

extern IRTable irgen_table;
extern IR      builder;

extern FuncDefInst* cur_func;
extern Type*        ret_type;

extern int cur_label;
extern int loop_start_label;  // continue;
extern int loop_end_label;    // break;

extern int max_label;
extern int max_reg;

#define NEW_BLOCK() builder.createBlock(cur_func, ++max_label)

void StmtNode::genIRCode() { cerr << "StmtNode genIRCode not implemented" << endl; }

void ExprStmt::genIRCode()
{
    if (!exprs) return;

    for (auto expr : *exprs)
        if (expr) expr->genIRCode();
}

int FindMinDimStepIR(const std::vector<int> dims, int relativePos, int dimsIdx, int& max_subBlock_sz)
{
    int min_dim_step = 1;
    int blockSz      = 1;
    for (size_t i = dimsIdx + 1; i < dims.size(); ++i) { blockSz *= dims[i]; }
    while (relativePos % blockSz != 0)
    {
        min_dim_step++;
        blockSz /= dims[dimsIdx + min_dim_step - 1];
    }
    max_subBlock_sz = blockSz;
    return min_dim_step;
}

std::vector<int> GetIndexes(std::vector<int> dims, int absoluteIndex)
{
    std::vector<int> ret;
    for (std::vector<int>::reverse_iterator it = dims.rbegin(); it != dims.rend(); ++it)
    {
        int dim = *it;
        ret.insert(ret.begin(), absoluteIndex % dim);
        absoluteIndex /= dim;
    }
    return ret;
}

void RecursiveArrayInitIR(IRBlock* block, const std::vector<int> dims, int arrayaddr_reg_no, InitNode* init,
    int beginPos, int endPos, int dimsIdx, Type* baseType)
{
    InitMulti* init_multi = static_cast<InitMulti*>(init);
    if (!init_multi) return;
    if (!init_multi->exprs) return;

    int pos = beginPos;

    for (InitNode* iv : *(init_multi->exprs))
    {
        InitSingle* init_single = dynamic_cast<InitSingle*>(iv);

        if (init_single)  // 单个数
        {
            ExprNode* expr = init_single->expr;
            if (!expr) continue;
            expr->genIRCode();
            block->insertTypeConvert(expr->attr.val.type->getKind(), baseType->getKind(), max_reg);

            int val_reg  = max_reg;
            int addr_reg = ++max_reg;

            GEPInst* gep = new GEPInst(TYPE2LLVM(baseType->getKind()),
                DT::I32,
                getRegOperand(arrayaddr_reg_no),
                getRegOperand(addr_reg),
                dims);

            gep->idxs.emplace_back(new ImmeI32Operand(0));
            std::vector<int> indexes = GetIndexes(dims, pos);
            for (int idx : indexes) gep->idxs.emplace_back(new ImmeI32Operand(idx));

            block->insts.push_back(gep);
            block->insertStore(TYPE2LLVM(baseType->getKind()), getRegOperand(val_reg), getRegOperand(addr_reg));

            ++pos;
            continue;
        }

        // 数组
        int max_subBlock_sz = 0;
        int min_dim_step    = FindMinDimStepIR(dims, pos - beginPos, dimsIdx, max_subBlock_sz);
        RecursiveArrayInitIR(
            block, dims, arrayaddr_reg_no, iv, pos, pos + max_subBlock_sz - 1, dimsIdx + min_dim_step, baseType);
        pos += max_subBlock_sz;
    }
}

void VarDeclStmt::genIRCode()
{
    if (!defs) return;

    IRBlock* declare_block = builder.getBlock(cur_func, 0);
    IRBlock* block         = builder.getBlock(cur_func, cur_label);

    DT dtype = TYPE2LLVM(baseType->getKind());

    for (auto& def : *defs)
    {
        LeftValueExpr* lval = static_cast<LeftValueExpr*>(def->lval);
        int            reg  = ++max_reg;
        irgen_table.symTab->addSymbol(lval->entry, reg);

        VarAttribute val;
        val.type = baseType;

        if (lval->dims)  // 数组
        {
            int dim_size = 1;
            for (auto& dim : *lval->dims)
            {
                int d = TO_INT(dim->attr.val.value);
                dim_size *= d;
                val.dims.push_back(d);
            }

            declare_block->insertAllocArray(dtype, val.dims, reg);
            irgen_table.regMap[reg] = val;

            // 初始化为0
            declare_block->insts.emplace_back(new CallInst(DT::VOID,
                "llvm.memset.p0.i32",
                {
                    {DT::PTR, getRegOperand(reg)},
                    {DT::I8, new ImmeI32Operand(0)},
                    {DT::I32, new ImmeI32Operand(dim_size * sizeof(int))},
                    {DT::I1, new ImmeI32Operand(1)},
                },
                nullptr));

            // 覆盖初始化
            InitMulti* init = static_cast<InitMulti*>(def->rval);
            if (!init) continue;

            RecursiveArrayInitIR(block, val.dims, reg, def->rval, 0, dim_size - 1, 0, baseType);

            continue;
        }

        // 非数组
        block->insertAlloc(dtype, reg);
        irgen_table.regMap[reg] = val;

        Operand* init_val = nullptr;

        if (def->rval)  // 带初始值
        {
            InitSingle* init = static_cast<InitSingle*>(def->rval);
            init->expr->genIRCode();
            block->insertTypeConvert(init->expr->attr.val.type->getKind(), baseType->getKind(), max_reg);
        }
        else  // 不带初始值，置为0
        {
            if (dtype == DT::I32)
                block->insertArithmeticI32_ImmeAll(IROpCode::ADD, 0, 0, ++max_reg);
            else if (dtype == DT::F32)
                block->insertArithmeticF32_ImmeAll(IROpCode::FADD, 0, 0, ++max_reg);
        }

        init_val = getRegOperand(max_reg);
        block->insertStore(dtype, init_val, getRegOperand(reg));
    }
}

void BlockStmt::genIRCode()
{
    irgen_table.symTab->enterScope();

    if (stmts)
    {
        for (auto& stmt : *stmts) stmt->genIRCode();
    }

    irgen_table.symTab->exitScope();
}

void FuncDeclStmt::genIRCode()
{
    irgen_table.symTab->enterScope();
    irgen_table.regMap.clear();
    irgen_table.formalArrTab.clear();

    DT           llvm_ret_type = TYPE2LLVM(returnType->getKind());
    FuncDefInst* func_def      = new FuncDefInst(llvm_ret_type, entry->getName());

    cur_func = func_def;
    ret_type = returnType;

    cur_label = 0;
    max_label = -1;

    if (params)
        max_reg = static_cast<int>(params->size() - 1);
    else
        max_reg = -1;

    builder.enterFunc(func_def);
    IRBlock* block = NEW_BLOCK();
    block->comment = "Func define at line " + to_string(line_num);

    if (params)
    {
        auto& params_vec = *params;
        for (size_t i = 0; i < params_vec.size(); ++i)
        {
            VarAttribute val;
            auto&        pdefNode = *params_vec[i];
            val.type              = pdefNode.baseType;
            DT dtype              = TYPE2LLVM(val.type->getKind());

            if (pdefNode.dims)
            {
                func_def->arg_types.push_back(DT::PTR);
                irgen_table.formalArrTab[i] = true;
                for (size_t j = 1; j < pdefNode.dims->size(); ++j)
                {
                    val.dims.push_back(TO_INT(pdefNode.dims->at(j)->attr.val.value));
                }

                irgen_table.symTab->addSymbol(pdefNode.entry, i);
                irgen_table.regMap[i] = val;
                func_def->arg_regs.push_back(getRegOperand(func_def->arg_regs.size()));
            }
            else
            {
                func_def->arg_types.push_back(dtype);

                ++max_reg;
                block->insertAlloc(dtype, max_reg);
                block->insertStore(dtype, i, getRegOperand(max_reg));

                irgen_table.symTab->addSymbol(pdefNode.entry, max_reg);
                irgen_table.regMap[max_reg] = val;
                func_def->arg_regs.push_back(getRegOperand(func_def->arg_regs.size()));
            }

            cerr << "In func " << entry->getName() << " add param " << pdefNode.entry->getName() << " at reg "
                 << max_reg << endl;
        }
    }

    if (body) body->genIRCode();

    block->insertUncondBranch(1);

    block          = NEW_BLOCK();
    block->comment = "Func end at line " + to_string(line_num);
    cur_label      = max_label;

    for (auto& [idx, block] : builder.function_block_map[func_def])
    {
        bool broken = false;

        if (block->insts.empty())
            broken = true;
        else
        {
            auto& last_inst = block->insts.back();
            broken          = !(IS_BR(last_inst) || IS_RET(last_inst));
        }

        if (broken)
        {
            if (llvm_ret_type == DT::I32)
                block->insertRetImmI32(DT::I32, 0);
            else if (llvm_ret_type == DT::F32)
                block->insertRetImmF32(DT::F32, 0.3);
            else
                block->insertRetVoid();
        }
    }

    irgen_table.symTab->exitScope();
}

void ReturnStmt::genIRCode()
{
    IRBlock* block = builder.getBlock(cur_func, cur_label);

    if (ret_type == voidType)
    {
        block->insertRetVoid();
        return;
    }

    expr->genIRCode();
    block = builder.getBlock(cur_func, cur_label);

    block->insertTypeConvert(expr->attr.val.type->getKind(), ret_type->getKind(), max_reg);
    block->insertRetReg(TYPE2LLVM(ret_type->getKind()), max_reg);
}

void WhileStmt::genIRCode()
{
    IRBlock* cond_block = NEW_BLOCK();
    cond_block->comment = "While condition at line " + to_string(line_num);
    IRBlock* body_block = NEW_BLOCK();
    body_block->comment = "While body at line " + to_string(line_num);
    IRBlock* end_block  = NEW_BLOCK();
    end_block->comment  = "While end at line " + to_string(line_num);
    IRBlock* block      = builder.getBlock(cur_func, cur_label);

    int start_label_bak = loop_start_label;
    int end_label_bak   = loop_end_label;

    loop_start_label = cond_block->block_id;
    loop_end_label   = end_block->block_id;

    block->insertUncondBranch(cond_block->block_id);
    cur_label = cond_block->block_id;

    condition->true_label  = body_block->block_id;
    condition->false_label = end_block->block_id;
    condition->genIRCode();

    block = builder.getBlock(cur_func, cur_label);
    block->insertTypeConvert(condition->attr.val.type->getKind(), TypeKind::Bool, max_reg);
    block->insertCondBranch(max_reg, body_block->block_id, end_block->block_id);

    cur_label = body_block->block_id;
    if (body) body->genIRCode();
    block = builder.getBlock(cur_func, cur_label);
    block->insertUncondBranch(cond_block->block_id);

    cur_label = end_block->block_id;

    loop_start_label = start_label_bak;
    loop_end_label   = end_label_bak;
}

void IfStmt::genIRCode()
{
    IRBlock* then_block = NEW_BLOCK();
    then_block->comment = "If then at line " + to_string(line_num);
    IRBlock* else_block = NEW_BLOCK();
    else_block->comment = "If else at line " + to_string(line_num);
    IRBlock* end_block  = NEW_BLOCK();
    end_block->comment  = "If end at line " + to_string(line_num);

    condition->true_label  = then_block->block_id;
    condition->false_label = else_block->block_id;
    condition->genIRCode();

    IRBlock* block = builder.getBlock(cur_func, cur_label);
    block->insertTypeConvert(condition->attr.val.type->getKind(), TypeKind::Bool, max_reg);
    block->insertCondBranch(max_reg, then_block->block_id, else_block->block_id);

    cur_label = then_block->block_id;
    if (thenBody) thenBody->genIRCode();
    block = builder.getBlock(cur_func, cur_label);
    block->insertUncondBranch(end_block->block_id);

    cur_label = else_block->block_id;
    if (elseBody) elseBody->genIRCode();
    block = builder.getBlock(cur_func, cur_label);
    block->insertUncondBranch(end_block->block_id);

    cur_label = end_block->block_id;
}

void ForStmt::genIRCode() { cerr << "ForStmt genIRCode not implemented" << endl; }

void BreakStmt::genIRCode()
{
    IRBlock* block = builder.getBlock(cur_func, cur_label);
    block->insertUncondBranch(loop_end_label);

    block          = NEW_BLOCK();
    block->comment = "Break at line " + to_string(line_num);
    cur_label      = block->block_id;
}

void ContinueStmt::genIRCode()
{
    IRBlock* block = builder.getBlock(cur_func, cur_label);
    block->insertUncondBranch(loop_start_label);

    block          = NEW_BLOCK();
    block->comment = "Continue at line " + to_string(line_num);
    cur_label      = block->block_id;
}