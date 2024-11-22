#include <ast/statement.h>
#include <ast/expression.h>
#include <ast/helper.h>
#include <llvm_ir/ir_builder.h>
#include <llvm_ir/function.h>
#include <llvm_ir/build/type_trans.h>
#include <common/type/symtab/semantic_table.h>
#include <common/array/indexing.h>
#include <iostream>
#include <assert.h>
#include <algorithm>
using namespace std;
using namespace LLVMIR;

using DT = DataType;

extern IRTable     irgen_table;
extern IR          builder;
extern IRFunction* ir_func;

#define NEW_BLOCK() builder.createBlock()
#define IRGEN_TAB_CLEAR()                 \
    {                                     \
        irgen_table.regMap.clear();       \
        irgen_table.formalArrTab.clear(); \
    }

void StmtNode::genIRCode()
{
    cerr << "Base StmtNode genIRCode should not be called, occurred at line " << line_num << endl;
    assert(false);
}

void ExprStmt::genIRCode()
{
    if (!exprs) return;

    for (auto expr : *exprs)
        if (expr) expr->genIRCode();
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
            block->insertTypeConvert(expr->attr.val.type->getKind(), baseType->getKind(), ir_func->max_reg);

            int val_reg  = ir_func->max_reg;
            int addr_reg = ++ir_func->max_reg;

            GEPInst* gep = new GEPInst(TYPE2LLVM(baseType->getKind()),
                DT::I32,
                getRegOperand(arrayaddr_reg_no),
                getRegOperand(addr_reg),
                dims);

            gep->idxs.emplace_back(getImmeI32Operand(0));
            std::deque<int> indexes;
            LINEAR_TO_MULTI_INDEX(dims, pos, indexes);
            for (int idx : indexes) gep->idxs.emplace_back(getImmeI32Operand(idx));

            block->insts.push_back(gep);
            block->insertStore(TYPE2LLVM(baseType->getKind()), getRegOperand(val_reg), getRegOperand(addr_reg));

            ++pos;
            continue;
        }

        // 数组
        int max_subBlock_sz = 0;
        int min_dim_step    = FindMinStepForPosition(dims, pos - beginPos, dimsIdx, max_subBlock_sz);
        RecursiveArrayInitIR(
            block, dims, arrayaddr_reg_no, iv, pos, pos + max_subBlock_sz - 1, dimsIdx + min_dim_step, baseType);
        pos += max_subBlock_sz;
    }
}

void VarDeclStmt::genIRCode()
{
    if (!defs) return;

    IRBlock* declare_block = builder.getBlock(0);
    IRBlock* block         = builder.getBlock(ir_func->cur_label);

    DT dtype = TYPE2LLVM(baseType->getKind());

    for (auto& def : *defs)
    {
        LeftValueExpr* lval = static_cast<LeftValueExpr*>(def->lval);
        int            reg  = ++ir_func->max_reg;
        irgen_table.symTab->addSymbol(lval->entry, reg);

        VarAttribute val;
        val.type = baseType;

        if (lval->dims)
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

            block->insts.emplace_back(new CallInst(DT::VOID,  // 修改，使用时再初始化
                "llvm.memset.p0.i32",
                {
                    {DT::PTR, getRegOperand(reg)},
                    {DT::I8, getImmeI32Operand(0)},
                    {DT::I32, getImmeI32Operand(dim_size * sizeof(int))},
                    {DT::I1, getImmeI32Operand(1)},
                },
                nullptr));

            if (IF_IS_POLY(def->rval, InitMulti))
                RecursiveArrayInitIR(block, val.dims, reg, def->rval, 0, dim_size - 1, 0, baseType);
        }
        else
        {
            declare_block->insertAlloc(dtype, reg);
            irgen_table.regMap[reg] = val;

            if (def->rval)
            {
                InitSingle* init = static_cast<InitSingle*>(def->rval);
                init->expr->genIRCode();
                block->insertTypeConvert(init->expr->attr.val.type->getKind(), baseType->getKind(), ir_func->max_reg);
            }
            else if (dtype == DT::I32)
                block->insertArithmeticI32_ImmeAll(IROpCode::ADD, 0, 0, ++ir_func->max_reg);
            else if (dtype == DT::F32)
                block->insertArithmeticF32_ImmeAll(IROpCode::FADD, 0, 0, ++ir_func->max_reg);
            else
            {
                cerr << "Currently only support int and float type" << endl;
                assert(false);
            }

            block->insertStore(dtype, getRegOperand(ir_func->max_reg), getRegOperand(reg));
        }
    }
}

void BlockStmt::genIRCode()
{
    if (!stmts) return;
    irgen_table.symTab->enterScope();
    for (auto& stmt : *stmts) stmt->genIRCode();
    irgen_table.symTab->exitScope();
}

void FuncDeclStmt::genIRCode()
{
    irgen_table.symTab->enterScope();
    IRGEN_TAB_CLEAR()

    DT           llvm_ret_type = TYPE2LLVM(returnType->getKind());
    FuncDefInst* func_def      = new FuncDefInst(llvm_ret_type, entry->getName());
    ir_func                    = new IRFunction(returnType, func_def);
    ir_func->max_reg           = params ? static_cast<int>(params->size() - 1) : -1;

    builder.enterFunc(ir_func);
    IRBlock* block = NEW_BLOCK();
    block->comment = "Func define at line " + to_string(line_num);

    if (params)
    {
        for (size_t i = 0; i < params->size(); ++i)
        {
            VarAttribute val;
            auto&        pdefNode = *(*params)[i];
            val.type              = pdefNode.baseType;
            DT dtype              = TYPE2LLVM(val.type->getKind());

            bool isArray = pdefNode.dims != nullptr;
            func_def->arg_types.push_back(isArray ? DT::PTR : dtype);
            irgen_table.formalArrTab[i] = isArray;

            if (isArray)
            {
                for (size_t j = 1; j < pdefNode.dims->size(); ++j)
                {
                    val.dims.push_back(TO_INT(pdefNode.dims->at(j)->attr.val.value));
                }
                irgen_table.symTab->addSymbol(pdefNode.entry, i);
                irgen_table.regMap[i] = val;
            }
            else
            {
                ++ir_func->max_reg;
                block->insertAlloc(dtype, ir_func->max_reg);
                block->insertStore(dtype, i, getRegOperand(ir_func->max_reg));
                irgen_table.symTab->addSymbol(pdefNode.entry, ir_func->max_reg);
                irgen_table.regMap[ir_func->max_reg] = val;
            }

            func_def->arg_regs.push_back(getRegOperand(i));
        }
    }

    IRBlock* body_block = NEW_BLOCK();
    body_block->comment = "Func body at line " + to_string(line_num);
    ir_func->cur_label  = ir_func->max_label;

    if (body) body->genIRCode();

    block->insertUncondBranch(1);

    for (auto& block : builder.cur_func->blocks)
    {
        if (block->insts.empty() || !(IS_BR(block->insts.back()) || IS_RET(block->insts.back())))
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
    IRBlock* block = builder.getBlock(ir_func->cur_label);

    if (ir_func->ret_type == voidType)
    {
        block->insertRetVoid();
        return;
    }

    expr->genIRCode();
    block = builder.getBlock(ir_func->cur_label);

    block->insertTypeConvert(expr->attr.val.type->getKind(), ir_func->ret_type->getKind(), ir_func->max_reg);
    block->insertRetReg(TYPE2LLVM(ir_func->ret_type->getKind()), ir_func->max_reg);
}

void WhileStmt::genIRCode()
{
    IRBlock* cond_block = NEW_BLOCK();
    cond_block->comment = "While condition at line " + to_string(line_num);
    IRBlock* body_block = nullptr;
    if (body)
    {
        body_block          = NEW_BLOCK();
        body_block->comment = "While body at line " + to_string(line_num);
    }
    IRBlock* end_block = NEW_BLOCK();
    end_block->comment = "While end at line " + to_string(line_num);
    IRBlock* block     = builder.getBlock(ir_func->cur_label);

    int start_label_bak = ir_func->loop_start_label;
    int end_label_bak   = ir_func->loop_end_label;

    ir_func->loop_start_label = cond_block->block_id;
    ir_func->loop_end_label   = end_block->block_id;

    block->insertUncondBranch(cond_block->block_id);
    ir_func->cur_label = cond_block->block_id;

    condition->true_label  = body ? body_block->block_id : cond_block->block_id;
    condition->false_label = end_block->block_id;
    condition->genIRCode();

    block = builder.getBlock(ir_func->cur_label);
    block->insertTypeConvert(condition->attr.val.type->getKind(), TypeKind::Bool, ir_func->max_reg);
    block->insertCondBranch(ir_func->max_reg, body ? body_block->block_id : end_block->block_id, end_block->block_id);

    if (body)
    {
        ir_func->cur_label = body_block->block_id;
        body->genIRCode();
        block = builder.getBlock(ir_func->cur_label);
        block->insertUncondBranch(cond_block->block_id);
    }

    ir_func->cur_label = end_block->block_id;

    ir_func->loop_start_label = start_label_bak;
    ir_func->loop_end_label   = end_label_bak;
}

void IfStmt::genIRCode()
{
    IRBlock* then_block = NEW_BLOCK();
    then_block->comment = "If then at line " + to_string(line_num);
    IRBlock* end_block  = NEW_BLOCK();
    end_block->comment  = "If end at line " + to_string(line_num);

    IRBlock* else_block = nullptr;
    if (elseBody)
    {
        else_block             = NEW_BLOCK();
        else_block->comment    = "If else at line " + to_string(line_num);
        condition->true_label  = then_block->block_id;
        condition->false_label = else_block->block_id;
    }
    else
    {
        condition->true_label  = then_block->block_id;
        condition->false_label = end_block->block_id;
    }

    condition->genIRCode();

    IRBlock* block = builder.getBlock(ir_func->cur_label);
    block->insertTypeConvert(condition->attr.val.type->getKind(), TypeKind::Bool, ir_func->max_reg);
    block->insertCondBranch(
        ir_func->max_reg, then_block->block_id, else_block ? else_block->block_id : end_block->block_id);

    ir_func->cur_label = then_block->block_id;
    if (thenBody) thenBody->genIRCode();
    block = builder.getBlock(ir_func->cur_label);
    block->insertUncondBranch(end_block->block_id);

    if (elseBody)
    {
        ir_func->cur_label = else_block->block_id;
        elseBody->genIRCode();
        block = builder.getBlock(ir_func->cur_label);
        block->insertUncondBranch(end_block->block_id);
    }

    ir_func->cur_label = end_block->block_id;
}

void ForStmt::genIRCode()
{
    int start_label_bak = ir_func->loop_start_label;
    int end_label_bak   = ir_func->loop_end_label;

    IRBlock* init_block = builder.getBlock(ir_func->cur_label);
    init_block->comment = "For init at line " + to_string(line_num);
    IRBlock* cond_block = NEW_BLOCK();
    cond_block->comment = "For condition at line " + to_string(line_num);
    IRBlock* body_block = nullptr;
    if (body)
    {
        body_block          = NEW_BLOCK();
        body_block->comment = "For body at line " + to_string(line_num);
    }
    IRBlock* update_block = NEW_BLOCK();
    update_block->comment = "For update at line " + to_string(line_num);
    IRBlock* end_block    = NEW_BLOCK();
    end_block->comment    = "For end at line " + to_string(line_num);

    ir_func->loop_start_label = update_block->block_id;
    ir_func->loop_end_label   = end_block->block_id;

    if (init) init->genIRCode();
    init_block->insertUncondBranch(cond_block->block_id);

    ir_func->cur_label = cond_block->block_id;
    if (condition)
    {
        condition->true_label  = body_block ? body_block->block_id : update_block->block_id;
        condition->false_label = end_block->block_id;
        condition->genIRCode();

        IRBlock* block = builder.getBlock(ir_func->cur_label);
        block->insertTypeConvert(condition->attr.val.type->getKind(), TypeKind::Bool, ir_func->max_reg);
        block->insertCondBranch(
            ir_func->max_reg, body_block ? body_block->block_id : update_block->block_id, end_block->block_id);
    }
    else
        cond_block->insertUncondBranch(body_block ? body_block->block_id : update_block->block_id);

    if (body)
    {
        ir_func->cur_label = body_block->block_id;
        body->genIRCode();
        IRBlock* block = builder.getBlock(ir_func->cur_label);
        block->insertUncondBranch(update_block->block_id);
    }

    ir_func->cur_label = update_block->block_id;
    if (update) update->genIRCode();
    IRBlock* block = builder.getBlock(ir_func->cur_label);
    block->insertUncondBranch(cond_block->block_id);

    ir_func->cur_label = end_block->block_id;

    ir_func->loop_start_label = start_label_bak;
    ir_func->loop_end_label   = end_label_bak;
}

void BreakStmt::genIRCode()
{
    IRBlock* block = builder.getBlock(ir_func->cur_label);
    block->insertUncondBranch(ir_func->loop_end_label);

    block              = NEW_BLOCK();
    block->comment     = "Break at line " + to_string(line_num);
    ir_func->cur_label = block->block_id;
}

void ContinueStmt::genIRCode()
{
    IRBlock* block = builder.getBlock(ir_func->cur_label);
    block->insertUncondBranch(ir_func->loop_start_label);

    block              = NEW_BLOCK();
    block->comment     = "Continue at line " + to_string(line_num);
    ir_func->cur_label = block->block_id;
}