#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "codegen.hpp"
#include "symtab.hpp"

#include <map>
#include "ast.hpp"
#include "debug.hpp"

using namespace llvm;

namespace ast{

    extern std::unique_ptr<llvm::LLVMContext> llvm_ctx;
    extern std::unique_ptr<llvm::Module> llvm_mod;
    extern std::unique_ptr<llvm::IRBuilder<>> llvm_builder;
    extern std::map<std::string, AllocaInst*> llvm_st;
    extern std::map<std::string, AllocaInst*> global_st;

    Value* handleAdd(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
    case I32:
        llvm_builder->CreateAdd(L, R);
        break;
    case I64:
        llvm_builder->CreateAdd(L, R);
        break;
    case U8:
        llvm_builder->CreateAdd(L, R);
        break;
    case FP32:
        llvm_builder->CreateFAdd(L, R);
        break;
    case FP64:
        llvm_builder->CreateFAdd(L, R);
        break;
    default:
        return nullptr;
    }
    }

    Value* handleSub(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
    case I32:
        llvm_builder->CreateSub(L, R);
        break;
    case I64:
        llvm_builder->CreateSub(L, R);
        break;
    case U8:
        llvm_builder->CreateSub(L, R);
        break;
    case FP32:
        llvm_builder->CreateFSub(L, R);
        break;
    case FP64:
        llvm_builder->CreateFSub(L, R);
        break;
    default:
        return nullptr;
    }
    }

    Value* handleMul(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
    case I32:
        llvm_builder->CreateMul(L, R);
        break;
    case I64:
        llvm_builder->CreateMul(L, R);
        break;
    case U8:
        llvm_builder->CreateMul(L, R);
        break;
    case FP32:
        llvm_builder->CreateFMul(L, R);
        break;
    case FP64:
        llvm_builder->CreateFMul(L, R);
        break;
    default:
        return nullptr;
    }
    }

Value* handleDiv(Value* L, Value* R, SymbolType ty){
    switch (ty)
        {
        case I32:
            llvm_builder->CreateSDiv(L, R);
            break;
        case I64:
            llvm_builder->CreateSDiv(L, R);
            break;
        case U8:
            llvm_builder->CreateUDiv(L, R);
            break;
        case FP32:
            llvm_builder->CreateFDiv(L, R);
            break;
        case FP64:
            llvm_builder->CreateFDiv(L, R);
            break;
        default:
            return nullptr;
        }
    }

Value* handleGTE(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            llvm_builder->CreateICmpSGE(L, R);
            break;
        case I64:
            llvm_builder->CreateICmpSGE(L, R);
            break;
        case U8:
            llvm_builder->CreateICmpUGE(L, R);
            break;
        case FP32:
            llvm_builder->CreateFCmpOGE(L, R);
            break;
        case FP64:
            llvm_builder->CreateFCmpOGE(L, R);
            break;
        default:
            return nullptr;
    }
}

Value* handleGT(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            llvm_builder->CreateICmpSGT(L, R);
            break;
        case I64:
            llvm_builder->CreateICmpSGT(L, R);
            break;
        case U8:
            llvm_builder->CreateICmpUGT(L, R);
            break;
        case FP32:
            llvm_builder->CreateFCmpOGT(L, R);
            break;
        case FP64:
            llvm_builder->CreateFCmpOGT(L, R);
            break;
        default:
            return nullptr;
    }
}

Value* handleLTE(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            llvm_builder->CreateICmpSLE(L, R);
            break;
        case I64:
            llvm_builder->CreateICmpSLE(L, R);
            break;
        case U8:
            llvm_builder->CreateICmpULE(L, R);
            break;
        case FP32:
            llvm_builder->CreateFCmpOLE(L, R);
            break;
        case FP64:
            llvm_builder->CreateFCmpOLE(L, R);
            break;
        default:
            return nullptr;
    }
}

Value* handleLT(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            llvm_builder->CreateICmpSLT(L, R);
            break;
        case I64:
            llvm_builder->CreateICmpSLT(L, R);
            break;
        case U8:
            llvm_builder->CreateICmpULT(L, R);
            break;
        case FP32:
            llvm_builder->CreateFCmpOLT(L, R);
            break;
        case FP64:
            llvm_builder->CreateFCmpOLT(L, R);
            break;
        default:
            return nullptr;
    }
}

Value* handleEQ(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            llvm_builder->CreateICmpEQ(L, R);
            break;
        case I64:
            llvm_builder->CreateICmpEQ(L, R);
            break;
        case U8:
            llvm_builder->CreateICmpEQ(L, R);
            break;
        case FP32:
            llvm_builder->CreateFCmpOEQ(L, R);
            break;
        case FP64:
            llvm_builder->CreateFCmpOEQ(L, R);
            break;
        default:
            return nullptr;
    }
}

Value* handleNE(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            llvm_builder->CreateICmpNE(L, R);
            break;
        case I64:
            llvm_builder->CreateICmpNE(L, R);
            break;
        case U8:
            llvm_builder->CreateICmpNE(L, R);
            break;
        case FP32:
            llvm_builder->CreateFCmpONE(L, R);
            break;
        case FP64:
            llvm_builder->CreateFCmpONE(L, R);
            break;
        default:
            return nullptr;
    }
}


Value* handleBAnd(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I1:
            llvm_builder->CreateAnd(L, R);
            break;
        default:
            return nullptr;
    }
}


Value* handleBOr(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I1:
            llvm_builder->CreateOr(L, R);
            break;
        default:
            return nullptr;
    }
}



Value* handleBNot(Value* L, SymbolType ty){
    switch (ty)
    {
        case I1:
            llvm_builder->CreateNot(L);
            break;
        default:
            return nullptr;
    }
}



Value* handleAnd(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            llvm_builder->CreateAnd(L, R);
            break;
        case I64:
            llvm_builder->CreateAnd(L, R);
            break;
        case U8:
            llvm_builder->CreateAnd(L, R);
            break;
        case U32:
            llvm_builder->CreateAnd(L, R);
            break;
        case U64:
            llvm_builder->CreateAnd(L, R);
            break;
        case I1:
            llvm_builder->CreateAnd(L, R);
            break;
        default:
            return nullptr;
    }
}


Value* handleOr(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            llvm_builder->CreateOr(L, R);
            break;
        case I64:
            llvm_builder->CreateOr(L, R);
            break;
        case U8:
            llvm_builder->CreateOr(L, R);
            break;
        case U32:
            llvm_builder->CreateOr(L, R);
            break;
        case U64:
            llvm_builder->CreateOr(L, R);
            break;
        case I1:
            llvm_builder->CreateOr(L, R);
            break;
        default:
            return nullptr;
    }
}


Value* handleXor(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            llvm_builder->CreateXor(L, R);
            break;
        case I64:
            llvm_builder->CreateXor(L, R);
            break;
        case U8:
            llvm_builder->CreateXor(L, R);
            break;
        case U32:
            llvm_builder->CreateXor(L, R);
            break;
        case U64:
            llvm_builder->CreateXor(L, R);
            break;
        case I1:
            llvm_builder->CreateXor(L, R);
            break;
        default:
            return nullptr;
    }
}




}