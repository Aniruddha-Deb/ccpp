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
    extern std::map<std::string, GlobalVariable*> global_st;

    Value* handleAdd(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
    case I32:
        return llvm_builder->CreateAdd(L, R, "temp");
        
    case I64:
        return llvm_builder->CreateAdd(L, R, "temp");
        
    case U8:
        return llvm_builder->CreateAdd(L, R, "temp");
        
    case FP32:
        return llvm_builder->CreateFAdd(L, R, "temp");
        
    case FP64:
        return llvm_builder->CreateFAdd(L, R, "temp");
        
    default:
        return nullptr;
    }
    }

    Value* handleSub(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
    case I32:
        return llvm_builder->CreateSub(L, R, "temp");
        
    case I64:
        return llvm_builder->CreateSub(L, R, "temp");
        
    case U8:
        return llvm_builder->CreateSub(L, R, "temp");
        
    case FP32:
        return llvm_builder->CreateFSub(L, R, "temp");
        
    case FP64:
        return llvm_builder->CreateFSub(L, R, "temp");
        
    default:
        return nullptr;
    }
    }

    Value* handleMul(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
    case I32:
        return llvm_builder->CreateMul(L, R, "temp");
        
    case I64:
        return llvm_builder->CreateMul(L, R, "temp");
        
    case U8:
        return llvm_builder->CreateMul(L, R, "temp");
        
    case FP32:
        return llvm_builder->CreateFMul(L, R, "temp");
        
    case FP64:
        return llvm_builder->CreateFMul(L, R, "temp");
        
    default:
        return nullptr;
    }
    }

Value* handleDiv(Value* L, Value* R, SymbolType ty){
    switch (ty)
        {
        case I32:
            return llvm_builder->CreateSDiv(L, R, "temp");
            
        case I64:
            return llvm_builder->CreateSDiv(L, R, "temp");
            
        case U8:
            return llvm_builder->CreateUDiv(L, R, "temp");
            
        case FP32:
            return llvm_builder->CreateFDiv(L, R, "temp");
            
        case FP64:
            return llvm_builder->CreateFDiv(L, R, "temp");
            
        default:
            return nullptr;
        }
    }

Value* handleGTE(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            return llvm_builder->CreateICmpSGE(L, R, "temp");
            
        case I64:
            return llvm_builder->CreateICmpSGE(L, R, "temp");
            
        case U8:
            return llvm_builder->CreateICmpUGE(L, R, "temp");
            
        case FP32:
            return llvm_builder->CreateFCmpOGE(L, R, "temp");
            
        case FP64:
            return llvm_builder->CreateFCmpOGE(L, R, "temp");
            
        default:
            return nullptr;
    }
}

Value* handleGT(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            return llvm_builder->CreateICmpSGT(L, R, "temp");
            
        case I64:
            return llvm_builder->CreateICmpSGT(L, R, "temp");
            
        case U8:
            return llvm_builder->CreateICmpUGT(L, R, "temp");
            
        case FP32:
            return llvm_builder->CreateFCmpOGT(L, R, "temp");
            
        case FP64:
            return llvm_builder->CreateFCmpOGT(L, R, "temp");
            
        default:
            return nullptr;
    }
}

Value* handleLTE(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            return llvm_builder->CreateICmpSLE(L, R, "temp");
            
        case I64:
            return llvm_builder->CreateICmpSLE(L, R, "temp");
            
        case U8:
            return llvm_builder->CreateICmpULE(L, R, "temp");
            
        case FP32:
            return llvm_builder->CreateFCmpOLE(L, R, "temp");
            
        case FP64:
            return llvm_builder->CreateFCmpOLE(L, R, "temp");
            
        default:
            return nullptr;
    }
}

Value* handleLT(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            return llvm_builder->CreateICmpSLT(L, R, "temp");
            
        case I64:
            return llvm_builder->CreateICmpSLT(L, R, "temp");
            
        case U8:
            return llvm_builder->CreateICmpULT(L, R, "temp");
            
        case FP32:
            return llvm_builder->CreateFCmpOLT(L, R, "temp");
            
        case FP64:
            return llvm_builder->CreateFCmpOLT(L, R, "temp");
            
        default:
            return nullptr;
    }
}

Value* handleEQ(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            return llvm_builder->CreateICmpEQ(L, R, "temp");
            
        case I64:
            return llvm_builder->CreateICmpEQ(L, R, "temp");
            
        case U8:
            return llvm_builder->CreateICmpEQ(L, R, "temp");
            
        case FP32:
            return llvm_builder->CreateFCmpOEQ(L, R, "temp");
            
        case FP64:
            return llvm_builder->CreateFCmpOEQ(L, R, "temp");
            
        default:
            return nullptr;
    }
}

Value* handleNE(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            return llvm_builder->CreateICmpNE(L, R, "temp");
            
        case I64:
            return llvm_builder->CreateICmpNE(L, R, "temp");
            
        case U8:
            return llvm_builder->CreateICmpNE(L, R, "temp");
            
        case FP32:
            return llvm_builder->CreateFCmpONE(L, R, "temp");
            
        case FP64:
            return llvm_builder->CreateFCmpONE(L, R, "temp");
            
        default:
            return nullptr;
    }
}


Value* handleBAnd(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I1:
            return llvm_builder->CreateAnd(L, R, "temp");
            
        default:
            return nullptr;
    }
}


Value* handleBOr(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I1:
            return llvm_builder->CreateOr(L, R, "temp");
            
        default:
            return nullptr;
    }
}



Value* handleBNot(Value* L, SymbolType ty){
    switch (ty)
    {
        case I1:
            return llvm_builder->CreateNot(L);
            
        default:
            return nullptr;
    }
}



Value* handleAnd(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            return llvm_builder->CreateAnd(L, R, "temp");
            
        case I64:
            return llvm_builder->CreateAnd(L, R, "temp");
            
        case U8:
            return llvm_builder->CreateAnd(L, R, "temp");
            
        case U32:
            return llvm_builder->CreateAnd(L, R, "temp");
            
        case U64:
            return llvm_builder->CreateAnd(L, R, "temp");
            
        case I1:
            return llvm_builder->CreateAnd(L, R, "temp");
            
        default:
            return nullptr;
    }
}


Value* handleOr(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            return llvm_builder->CreateOr(L, R, "temp");
            
        case I64:
            return llvm_builder->CreateOr(L, R, "temp");
            
        case U8:
            return llvm_builder->CreateOr(L, R, "temp");
            
        case U32:
            return llvm_builder->CreateOr(L, R, "temp");
            
        case U64:
            return llvm_builder->CreateOr(L, R, "temp");
            
        case I1:
            return llvm_builder->CreateOr(L, R, "temp");
            
        default:
            return nullptr;
    }
}


Value* handleXor(Value* L, Value* R, SymbolType ty){
    switch (ty)
    {
        case I32:
            return llvm_builder->CreateXor(L, R, "temp");
            
        case I64:
            return llvm_builder->CreateXor(L, R, "temp");
            
        case U8:
            return llvm_builder->CreateXor(L, R, "temp");
            
        case U32:
            return llvm_builder->CreateXor(L, R, "temp");
            
        case U64:
            return llvm_builder->CreateXor(L, R, "temp");
            
        case I1:
            return llvm_builder->CreateXor(L, R, "temp");
            
        default:
            return nullptr;
    }
}




}