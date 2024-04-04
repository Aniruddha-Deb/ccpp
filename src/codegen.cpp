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

  for(auto func_ptr: *functions){
    cout<<"READ FUNC DEF"<<endl;
    llvm::Function* func_ir = func_ptr->codegen();
    func_ir->print(errs());
    
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
  cout<<"exprstmt"<<endl;
  return expr->codegen();
}

Value* Literal::codegen() {
  int v;
  cout<<ltype<<endl;
  switch(ltype) {
        case LT_INT_LIKE:
            cout<<32<<endl;
            v = intLiteralToInt(value);
            return ConstantInt::get(*llvm_ctx, APInt(32, v));
            break;
            // break;
        case LT_INT64:
            cout<<64<<endl;
            v = intLiteralToInt(value);
            return ConstantInt::get(*llvm_ctx, APInt(64, v));
        case LT_STRING:
            return ConstantDataArray::getString(*llvm_ctx, value, true);
            break;
        default:
            cout<<-1<<endl;
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
  cout<<"block"<<endl;
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
  int num_args = 0;
  if(params){
    num_args = params->params->size();
  }

  cout<<"READ FUNC DEF"<<endl;
  std::vector<llvm::Type*> argtypes(num_args);
  for(int i = 0; i < num_args; i++){
    argtypes[i] = getType(TS_INT);  // TODO
  }
  cout<<"READ FUNC DEF"<<endl;
  FunctionType *func_type = FunctionType::get(getType(TS_INT), argtypes, false);
  std::string func_name = func_decl->ident->name;
  llvm::Function *func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, func_name, llvm_mod.get());
  cout<<"READ FUNC DEF"<<endl;

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
  cout<<"READ FUNC DEF"<<endl;
  if (Value *ret_val = stmts->codegen()) {
    // llvm_builder->CreateRet(ret_val);
    // creating return statement in retrunstatement->codegen() instead.
    verifyFunction(*func);
    return func;
  }
  cout<<"RECKED"<<endl;
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
