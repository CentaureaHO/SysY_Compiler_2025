#include "unused_func_elimination.h"
#include "llvm_ir/instruction.h"
#include "llvm_ir/ir_builder.h"
#include <algorithm>
#include <iostream>
#include <assert.h>
#include <string>
#include <utility>

namespace Transform
{

    using namespace LLVMIR;

    void UnusedFuncEliminationPass::Execute()
    {
        std::map<std::string, IRFunction*> funcname_func_map;
        std::map<IRFunction*, int>         func_vis_map;
        std::map<std::string, int>         out_use_map;
        std::queue<IRFunction*>            func_q;

        out_use_map["llvm.memset.p0.i32"] = 1;
        out_use_map["getint"]             = 1;
        out_use_map["getch"]              = 1;
        out_use_map["getarray"]           = 1;
        out_use_map["putint"]             = 1;
        out_use_map["putch"]              = 1;
        out_use_map["putarray"]           = 1;
        out_use_map["getfloat"]           = 1;
        out_use_map["getfarray"]          = 1;
        out_use_map["putfloat"]           = 1;
        out_use_map["putfarray"]          = 1;
        out_use_map["_sysy_starttime"]    = 1;
        out_use_map["_sysy_stoptime"]     = 1;

        // 记录映射关系
        for (auto func : ir->functions) { funcname_func_map[func->func_def->func_name] = func; }
        // std::cout<<"func names:"<<std::endl;
        for (auto iter : funcname_func_map)
        {
            // std::cout << iter.first << std::endl;
        }
        // std::cout<<"===============================:"<<std::endl;

        assert(funcname_func_map.find("main") != funcname_func_map.end() && "there must be a main");

        // main函数作为起点
        func_q.push(funcname_func_map["main"]);
        func_vis_map[funcname_func_map["main"]] = 1;

        assert(!func_q.empty());

        while (!func_q.empty())
        {
            auto func_now = func_q.front();
            func_q.pop();

            // std::cout << func_now->func_def->func_name << std::endl;

            // 获取当前函数call了的函数
            for (auto block : func_now->blocks)
            {
                for (auto inst : block->insts)
                {
                    if (inst->opcode != IROpCode::CALL) { continue; }
                    auto call_inst = (CallInst*)inst;
                    // std::cout<<call_inst->func_name<<std::endl;
                    auto to_call_func = funcname_func_map[call_inst->func_name];

                    if (to_call_func == nullptr)
                    {  // 调用外部的函数
                        out_use_map[call_inst->func_name] = 1;
                        continue;
                    }

                    if (func_vis_map.find(to_call_func) != func_vis_map.end()) { continue; }

                    func_vis_map[to_call_func] = 1;
                    func_q.push(to_call_func);
                }
            }
        }
        // std::cout<<"out use func: "<<std::endl;
        //  for(auto iter:out_use_map){
        //      std::cout<<iter.first<<std::endl;
        //  }

        // 删除ir中未使用到的函数
        // ir和dom tree不必要更新，删除不影响后续使用
        std::vector<Instruction*> new_function_declare;
        std::vector<IRFunction*>  new_functions;

        for (auto func : ir->function_declare)
        {
            auto funcdecl = (FuncDeclareInst*)func;
            if (func_vis_map[funcname_func_map[funcdecl->func_name]] || out_use_map[funcdecl->func_name])
            {
                new_function_declare.push_back(func);
            }
        }

        for (auto func : ir->functions)
        {
            if (func_vis_map[func] || out_use_map[func->func_def->func_name]) { new_functions.push_back(func); }
        }

        // ir->function_declare.clear();
        // ir->functions.clear();

        ir->function_declare = std::move(new_function_declare);
        ir->functions        = std::move(new_functions);
    }

}  // namespace Transform