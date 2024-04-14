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

#include <map>
#include "ast.hpp"
#include "debug.hpp"

int intLiteralToInt(string &s){
  return stoi(s);
};



using namespace llvm;


namespace ast {

static std::unique_ptr<llvm::LLVMContext> llvm_ctx;
static std::unique_ptr<llvm::Module> llvm_mod;
static std::unique_ptr<llvm::IRBuilder<>> llvm_builder;
static std::map<string, Value *> llvm_st;

llvm::Type* getType(TypeSpecifier ty){
  switch(ty){
    case TS_INT:
      return llvm::Type::getInt32Ty(*llvm_ctx);
    case TS_FLOAT:
      return llvm::Type::getFloatTy(*llvm_ctx);
    default:
      return nullptr;
  }
}

void TranslationUnit::codegen() {
  llvm_ctx = std::make_unique<llvm::LLVMContext>();
  llvm_mod = std::make_unique<llvm::Module>("Code Generator", *llvm_ctx);

  // Create a new builder for the module.
  llvm_builder = std::make_unique<llvm::IRBuilder<>>(*llvm_ctx);

  Function* func_ptr;
  for (auto node_ptr: *nodes){
    if ((func_ptr = dynamic_cast<Function*>(node_ptr))) {
      // cout<<"READ FUNC DEF"<<endl;
      llvm::Function* func_ir = func_ptr->codegen();
      func_ir->print(errs());
    }
  }
}

Value* Expression::codegen(){
  ConstantInt::get(*llvm_ctx, APInt(32, 0));
}

Value* Statement::codegen(){
  cout<<"virtual"<<endl;
  return ConstantInt::get(*llvm_ctx, APInt(32, 0));
}

Value* ExpressionStatement::codegen(){
  return expr->codegen();
}

Value* Literal::codegen() {
  int v;
  switch(ltype) {
        case LT_INT_LIKE:
            v = intLiteralToInt(value);
            return ConstantInt::get(*llvm_ctx, APInt(32, v));
            break;
            // break;
        case LT_INT64:
            v = intLiteralToInt(value);
            return ConstantInt::get(*llvm_ctx, APInt(64, v));
        case LT_STRING:
            return ConstantDataArray::getString(*llvm_ctx, value, true);
            break;
        default:
            return nullptr;
  }
}

Value *BinaryExpression::codegen() {
  Value *L = lhs->codegen();
  Value *R = rhs->codegen();

  // TODO type check. error if types don't match

  if (!L || !R)
    return nullptr;
  switch (op) {
  case OP_ADD:
    return llvm_builder->CreateAdd(L, R, "addtmp");
  case OP_SUB:
    return llvm_builder->CreateSub(L, R, "subtmp");
  case OP_MUL:
    return llvm_builder->CreateMul(L, R, "multmp");
  case OP_DIV:
    return llvm_builder->CreateSDiv(L, R, "divtmp");
  }
}

Value* ReturnStatement::codegen(){
  Value* ret_val = ret_expr->codegen();
  llvm_builder->CreateRet(ret_val);

}


Value* BlockStatement::codegen(){
  Value* v;
  for (auto stmt = begin(); stmt != end(); ++stmt) {
        v = (*stmt)->codegen();
        if(!v){
          return nullptr;
        }
    }
  return v;
}


llvm::Function *Function::codegen() {
  // Uncommenting this causes a segfault !?
  // cdebug << "Generating function code " << func_decl->ident->name << endl;
  int num_args = 0;
  if(params){
    num_args = params->params->size();
  }

  std::vector<llvm::Type*> argtypes(num_args);
  for(int i = 0; i < num_args; i++){
    argtypes[i] = getType(TS_INT);  // TODO
  }
  FunctionType *func_type = FunctionType::get(getType(TS_INT), argtypes, false);
  std::string func_name = func_decl->ident->name;
  llvm::Function *func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, func_name, llvm_mod.get());

  int i=0;
  for (auto &Arg : func->args()){
    Arg.setName((*(params->params))[i]->ident->name);                // TODO, set arg names
    i++;
  }



  BasicBlock *block = BasicBlock::Create(*llvm_ctx, "entry", func);
  llvm_builder->SetInsertPoint(block);
  llvm_st.clear();
  for (auto &Arg : func->args())
    llvm_st[std::string(Arg.getName())] = &Arg;
  if (Value *ret_val = stmts->codegen()) {
    // llvm_builder->CreateRet(ret_val);
    // creating return statement in retrunstatement->codegen() instead.
    verifyFunction(*func);
    return func;
  }
  func->eraseFromParent();
  return nullptr;
}

Value* Identifier::codegen(){
  Value* v = llvm_st[name];
  if(!v){
    cout<<"nullptr ident";
    return nullptr;
  }
  return v;
}

} // namespace ast
