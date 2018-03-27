#include "execute.h"


execute::execute(): traversed{false} {}

void execute::run(const MatchFinder::MatchResult &Result){
	Context = Result.Context;
	Rewrite.setSourceMgr(Context->getSourceManager(), Context->getLangOpts());

	if(!traversed){
		allClasses.clear();
		clases.clear();
 
		HandleTranslationUnit(*Context);

		traversed = true;
	}

	classWithName(Result);
	classWithDefaultConstructor(Result);
	classWithCopyConstructor(Result);
	classWithMoveConstructor(Result);
	classWithDestructor(Result);
	deleteMember(Result);
	numberOfConstructor(Result);
	explicitSpecifiedConstructor(Result);
	takeException(Result);
	classWithListInitializerConstructor(Result);
	classWithMember(Result);
	classWithStaticVariable(Result);
	releaseVariable(Result);
	classWithVariableMembersAccessLevel(Result);
	methodWithName(Result);
	methodWithReferencedVariable(Result);
	methodWithReferencedMethod(Result);
	noExceptMethod(Result);
	inlineMethod(Result);
	defaultArgumentsInMethod(Result);
	deletedMethod(Result);
	defaultedMethod(Result);
	classWithCopyAssignmentOperator(Result);
	classWithMoveAssignmentOperator(Result);
	functionWithReferencedFunction(Result);
	functionWithName(Result);
	includedHeader(Result);
	classWithNotFriendMember(Result);
	classWithFriendFunction(Result);
	classWithFriendClass(Result);
}






/*
**
** Post-run methods
**
*/

void execute::postRun(){
	hasClass();
	hasDefaultConstructor();
	hasCopyConstructor();
	hasMoveConstructor();
	hasDestructor();
	hasDeleteMember();
	howManyConstructors();
	hasExplicitSpecifiedConstructor();
	hasTakeException();
	hasInitializer();
	hasMember();
	hasStaticVariable();
	hasReleasedVariable();
	hasVariableMembersAccessLevel();
	hasMethodWithName();
	hasMethodWithReferencedVariable();
	hasMethodWithReferencedMethod();
	hasNoExceptMethod();
	hasInlineMethod();
	hasDefaultArgumentsInMethod();
	hasDeletedMethod();
	hasDefaultedMethod();
	hasCopyAssignmentOperator();
	hasMoveAssignmentOperator();
	hasFunctionWithReferencedFunction();
	hasFunctionWithName();
	hasIncludedHeader();
	hasFileIncludedHeader();
	hasNotFriendMember();
	hasFriendFunction();
	hasFriendClass();
	hasGuardClauses();
	
	showMessages();

	isCorrect();
}


void execute::HandleTranslationUnit(ASTContext &Context) {
    TranslationUnitDecl *D = Context.getTranslationUnitDecl();
    // Run Recursive AST Visitor
    TraverseDecl(D);
} 

bool execute::VisitCXXRecordDecl(CXXRecordDecl *Declaration) {
    if(traversed)
        return true;
 
    CXXRecordDecl* Definition = 0;
    FullSourceLoc FullLocation;
    if(Declaration->hasDefinition()){
    //Necessary to take the definition though the declaration is saved: this is done so that if a class is  
    //in a system header, it is considered in this way
        if(!Declaration->isThisDeclarationADefinition()){
            Definition = Declaration->getDefinition();
            FullLocation = Context->getFullLoc(Definition->getLocStart());
        }
        else{
            FullLocation = Context->getFullLoc(Declaration->getLocStart());
        }
 
        if (
            FullLocation.isValid()  
            && !Context->getSourceManager().isInSystemHeader(FullLocation)  
        ){
            bool found_inserted = false;
            for(list<const CXXRecordDecl*>::const_iterator i = allClasses.begin(); i != allClasses.end(); i++){
                const CXXRecordDecl *BaseClass = *i;
                 if(check_same_decl_canonical(Declaration, BaseClass)){
                    found_inserted = true;
                    break;
                }
            }
            if(!found_inserted){
                allClasses.push_back(Declaration);
				clases.push_back(Declaration->getNameAsString());
			}
        }
    }
    return true;
}

bool execute::check_same_decl_canonical(const Decl *c1, const Decl *c2){
    return c1 && c2 && c1->getCanonicalDecl() == c2->getCanonicalDecl();
}

/*
**
** Exist Methods
**
*/
bool execute::existClass(const CXXRecordDecl *foundClass){
	
	if (foundClass && foundClass->isThisDeclarationADefinition()){
		FullSourceLoc FullLocation = Context->getFullLoc(foundClass->getLocStart());
		if (FullLocation.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation)){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

bool execute::existConstructor(const CXXConstructorDecl *foundConstructor){
	
	if (foundConstructor && foundConstructor->isThisDeclarationADefinition()){
		
		FullSourceLoc FullLocation = Context->getFullLoc(foundConstructor->getLocStart());
		if (FullLocation.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation)){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

bool execute::existDestructor(const CXXDestructorDecl *foundDestructor){
	if (foundDestructor){
		FullSourceLoc FullLocation = Context->getFullLoc(foundDestructor->getLocStart());
		if (FullLocation.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation)){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

bool execute::existDeleteExpression(const CXXDeleteExpr *foundDeleteExpression){
	if (foundDeleteExpression){
		FullSourceLoc FullLocation = Context->getFullLoc(foundDeleteExpression->getLocStart());
		if (FullLocation.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation)){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

bool execute::existMethod(const CXXMethodDecl *foundMethod){
	if (foundMethod && foundMethod->isThisDeclarationADefinition()){
		FullSourceLoc FullLocation = Context->getFullLoc(foundMethod->getLocStart());
		if (FullLocation.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation)){
			return true;		
		}else{
			return false;
		}
	}else{
		return false;
	}
}

bool execute::existFunction(const FunctionDecl *foundFunction){
	if (foundFunction && foundFunction->isThisDeclarationADefinition()){
		FullSourceLoc FullLocation = Context->getFullLoc(foundFunction->getLocStart());
		if (FullLocation.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation)){
			return true;					
		}else{
			return false;
		}
	}else{
		return false;
	}
}

bool execute::existMember(const FieldDecl *foundMember){
	if (foundMember){
		FullSourceLoc FullLocation = Context->getFullLoc(foundMember->getLocStart());
		if (FullLocation.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation)){
			return true;					
		}else{
			return false;
		}
	}else{
		return false;
	}
}

bool execute::existVariable(const VarDecl *foundVariable){
	if (foundVariable){
		FullSourceLoc FullLocation = Context->getFullLoc(foundVariable->getLocStart());
		if (FullLocation.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation)){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}




/*
**
** Check Params Methods
**
*/

bool execute::checkInitializersConstructor(const CXXConstructorDecl *foundConstructor, vector<string> parameters){
	bool idem = true;
	if (foundConstructor->getNumCtorInitializers() == parameters.size()){
		unsigned int p = 0;
		for (CXXConstructorDecl::init_const_iterator I = foundConstructor->init_begin(), E = foundConstructor->init_end(); I != E; ++I){
			 if((*I)->isWritten() && (*I)->isMemberInitializer()){

				FieldDecl *FD = (*I)->getMember();
				if(FD->getType().getCanonicalType().getAsString() != parameters[p]){ 
			       		idem = false;
		   		}
		   		p++;
			}
		}
	}else{
		idem = false;
	}
	return idem;
}


bool execute::checkParamsFunction(const FunctionDecl *foundFunction, vector<string> parameters){
	bool idem = true;
//	llvm::outs() << foundFunction->getNameAsString() << ": ";
	if (foundFunction->getNumParams() == parameters.size()){
		for(unsigned int p = 0; p < foundFunction->getNumParams(); p++){

//			llvm::outs()<< foundFunction->getParamDecl(p)->getType().getCanonicalType().getAsString() << " ";
	   		if(foundFunction->getParamDecl(p)->getType().getCanonicalType().getAsString() != parameters[p]){ 
	       			idem = false;
	   		}

//			llvm::outs()<< "\n";
		}
	}else{
		idem = false;
	}
	return idem;
}

bool execute::checkParams(const FunctionDecl* function, vector<string> params){
	bool result = false;	
	
	if (checkWithoutParams(params)){
		result = true;					
	}else{
		if (checkParamsFunction(function, params)){
			result = true;
		}
	}

	return result;
}


bool execute::checkWithoutParams(vector<string> params){
	string regExpr = "?";
	if (params.size()>0 && regExpr.compare(params[0])==0){
		return true;
	}else{
		return false;
	}
}


bool execute::markedConst(string c1, bool c2){
	string regExpr = "?";
	string constWord = "const";
	string noConstWord = "noconst";
	if(regExpr.compare(c1) == 0)
		return true;
	else{
		if(constWord == c1 && c2){
			return true;
		}	
		if(noConstWord == c1 && !c2){
			return true;	
		}
	}

	return false;
}

bool execute::checkDefArgs(const FunctionDecl* function, unsigned int numDefArgs, vector<string> defArgs){
	
	bool res = true;

	if(function->getNumParams() >= numDefArgs){

		unsigned int indexDefArgs = 0;

		for(unsigned int p = 0; p < function->getNumParams(); p++){

			if(function->getParamDecl(p)->hasDefaultArg()){
				numDefArgs--;
			
				if(!checkWithoutParams(defArgs)){

					string dfarg = Rewrite.getRewrittenText(function->getParamDecl(p)->getDefaultArg()->getSourceRange()); 
					if(dfarg != defArgs[indexDefArgs]){
						res = false;
						break;
					}

					indexDefArgs++;		
				}
			}
		}

		if(numDefArgs > 0)
			res = false;
	}
	else
		res = false;

	return res;
}


bool execute::checkRegularExpresion(string str1, string str2){

	bool res = false;
	try{
		if((str1 == str2) || std::regex_match(str1, std::regex(str2))){ 
			res = true;
		}    
	} catch (const std::regex_error& e) {}

	return res;
}




/*
**
** Check Correct Verfication
**
*/

void execute::isCorrect(){
	if (correct == true){
		llvm::outs() << getCorrectMessage() << "\n";
		llvm::outs() << "********************\n";
	}else{
		llvm::outs() << "--------------------\n";
		llvm::outs() << getIncorrectMessage() << "\n";
		llvm::outs() << "********************\n";
	}
}

void execute::showMessages(){
	set<string>::iterator it;

	for (it=messages.begin(); it != messages.end(); ++it){
		llvm::outs() << *it << "\n";
	}
}



/*
**
** Found Objects Methods
**
*/

bool execute::foundAll(A a){
	if (!a.empty()){
		for (AI it = a.begin(); it!=a.end(); ++it){
			if (((it->second).getFound() == false && (it->second).getExist()) ||
				((it->second).getFound() == true && !(it->second).getExist())){
				return false;
			}
		}
	}
	return true;
}


bool execute::foundAny(A a){
	for (AI it = a.begin(); it!=a.end(); ++it){
		if (((it->second).getFound() == true && (it->second).getExist()) ||
			((it->second).getFound() == false && !(it->second).getExist())){
			return true;
		}
	}
	return false;
}

bool execute::anyFoundAndInvalid(A a){
	for (AI it = a.begin(); it!=a.end(); ++it){
		if ((it->second).getFound() == true && (it->second).getValid() == false && (it->second).getExist() == true){
			return true;
		}
		else if((it->second).getFound() == true && (it->second).getValid() == true && (it->second).getExist() == false){
			return true;
		}
	}
	return false;
}

bool execute::allFoundAndValid(A a){
	for (AI it = a.begin(); it!=a.end(); ++it){
		if (((it->second).getFound() == false || (it->second).getValid() == false) && ((it->second).getExist() == true)){
			return false;
		}
		else if(((it->second).getFound() == true && (it->second).getValid() == true) && ((it->second).getExist() == false)){
			return false;
		}
	}
	return true;
}

bool execute::allReleasedVariables(){
   	for (map<const VarDecl*, bool>::iterator it=variables_new.begin(); it!=variables_new.end(); it++){
        if( !(it->second) )
	    	return false;    
    }
	return true;
}

void execute::hasClass(){
	if (!foundAll(classes)){
		getMessagesOfNotFound(classes);
		correct = false;
	}
}

void execute::hasDefaultConstructor(){

	for(auto it = clases.begin() ; it!=clases.end(); ++it){
		for(std::pair<const string,Info>& obj : defaultConstructors){
			if(std::regex_match(it->nom, std::regex(obj.first))){
				if((!obj.second.getValid() && !it->isDefault) ||
					(obj.second.getValid() && it->isDefault)){
					obj.second.setFound(true);
				}
			}
		}
	}
	if (!foundAll(defaultConstructors)){
		getMessagesOfNotFound(defaultConstructors);
		correct = false;
	}
}

void execute::hasCopyConstructor(){
	if (!foundAll(copyConstructors)){
		getMessagesOfNotFound(copyConstructors);
		correct = false;
	}
}

void execute::hasMoveConstructor(){
	if (!foundAll(moveConstructors)){
		getMessagesOfNotFound(moveConstructors);
		correct = false;
	}
}

void execute::hasInitializer(){
	if (!foundAll(initializers)){
		getMessagesOfNotFound(initializers);
		correct = false;
	}
}

void execute::hasDestructor(){
	if (!foundAll(destructors)){
		getMessagesOfNotFound(destructors);
		correct = false;
	}
}

void execute::hasDeleteMember(){
	if (!foundAll(deleteMembers)){
		getMessagesOfNotFound(deleteMembers);
		correct = false;
	}
}

void execute::howManyConstructors(){
	for (AI it = constructors.begin(); it!=constructors.end(); ++it){
		
		int numberOfConstructors = (it->second).getNumberOfConstructors();
		int expectedConstructors = (it->second).getExpectedConstructors();
		if (numberOfConstructors == expectedConstructors){
			(it->second).setFound(true);
		}
		else if((it->second).getLenient() && numberOfConstructors >= expectedConstructors){
			(it->second).setFound(true);
		}
	}
	if (!foundAll(constructors)){
		getMessagesOfNotFound(constructors);
		correct = false;
	}
}

void execute::hasExplicitSpecifiedConstructor(){
	if (!allFoundAndValid(explicitSpecifiedConstructors)){
		getMessagesOfNotFoundOrInvalid(explicitSpecifiedConstructors);
		correct = false;
	}
}

void execute::hasTakeException(){
	if (!foundAll(takeExceptions)){
		getMessagesOfNotFound(takeExceptions);
		correct = false;
	}
}

void execute::hasMethodWithName(){
	if (!foundAll(methodsWithName)){
		getMessagesOfNotFound(methodsWithName);
		correct = false;
	}
}

void execute::hasMethodWithReferencedVariable(){
	if (!foundAll(methodsWithReferencedVariable)){
		getMessagesOfNotFound(methodsWithReferencedVariable);
		correct = false;
	}
}

void execute::hasMethodWithReferencedMethod(){
	if (!foundAll(methodsWithReferencedMethod)){
		getMessagesOfNotFound(methodsWithReferencedMethod);
		correct = false;
	}
}

void execute::hasNoExceptMethod(){
	if (!foundAll(noExceptMethods)){
		getMessagesOfNotFound(noExceptMethods);
		correct = false;
	}
}

void execute::hasInlineMethod(){
	if (!foundAll(inlineMethods)){
		getMessagesOfNotFound(inlineMethods);
		correct = false;
	}
}

void execute::hasDefaultArgumentsInMethod(){
	if (!foundAll(defaultArgumentsInMethods)){
		getMessagesOfNotFound(defaultArgumentsInMethods);
		correct = false;
	}
}

void execute::hasDeletedMethod() {
	if(!foundAll(deletedMethods)) {
		getMessagesOfNotFound(deletedMethods);
		correct = false;
	}
}

void execute::hasDefaultedMethod() {
	if(!foundAll(defaultedMethods)) {
		getMessagesOfNotFound(defaultedMethods);
		correct = false;
	}
}

void execute::hasMember(){
	if (!foundAll(members)){
		getMessagesOfNotFound(members);
		correct = false;
	}
}

void execute::hasStaticVariable(){
	if (!foundAll(staticVariables)){
		getMessagesOfNotFound(staticVariables);
		correct = false;
	}
}

void execute::hasReleasedVariable(){
	if (allReleasedVariables()){
		//llvm::outs() << "ALL VARIABLE RELEASED.\n";
	}else{
		//llvm::outs() << releasedVariableMessage << "\n";
		messages.insert(releasedVariableMessage);
		correct = false;
	}
}

void execute::hasVariableMembersAccessLevel(){
        if (!foundAll(variableMembersAccessLevel)){
                getMessagesOfNotFound(variableMembersAccessLevel);
                correct = false;
        }
}


void execute::hasCopyAssignmentOperator(){
	if (!foundAll(copyAssignmentOperators)){
		getMessagesOfNotFound(copyAssignmentOperators);
		correct = false;
	}
}

void execute::hasMoveAssignmentOperator(){
	if (!foundAll(moveAssignmentOperators)){
		getMessagesOfNotFound(moveAssignmentOperators);
		correct = false;
	}
}

void execute::hasFunctionWithReferencedFunction(){
	if (!foundAll(functionsWithReferencedFunction)){
		getMessagesOfNotFound(functionsWithReferencedFunction);
		correct = false;
	}
}

void execute::hasFunctionWithName(){
	if (!foundAll(functionsWithName)){
		getMessagesOfNotFound(functionsWithName);
		correct = false;
	}
}

void execute::hasIncludedHeader(){
	if (anyFoundAndInvalid(includedHeaders)){
                getMessagesOfNotFound(includedHeaders);
                correct = false;
        }
}

void execute::hasFileIncludedHeader(){
	if (!foundAll(fileIncludedHeader)){
		getMessagesOfNotFound(fileIncludedHeader);
		correct = false;
	}
}

void execute::hasGuardClauses() {
	if(!foundAll(guardClauses)){
		getMessagesOfNotFound(guardClauses);
		correct=false;
	}
}

void execute::hasNotFriendMember(){
	if (!notFriendMembers.empty() && foundAny(notFriendMembers)){
		getMessagesOfFound(notFriendMembers);
		correct = false;
	}
}


void execute::hasFriendFunction(){
	if (!foundAll(friendFunctions)){
                getMessagesOfNotFound(friendFunctions);
                correct = false;
        }
}

void execute::hasFriendClass(){
        if (!foundAll(friendClasses)){
                getMessagesOfNotFound(friendClasses);
                correct = false;
        }
}




/*
**
** Get methods
**
*/

string execute::getCorrectMessage(){
	return correctMessage;
}

string execute::getIncorrectMessage(){
	return incorrectMessage;
}

void execute::getMessagesOfNotFound(A a){
	for (AI it = a.begin(); it!=a.end(); ++it){
		if (((it->second).getFound() == false && (it->second).getExist()) ||
			((it->second).getFound() == true && !(it->second).getExist())){
			messages.insert((it->second).getMessage());
		}
	}
}

void execute::getMessagesOfNotFoundOrInvalid(A a){
	for (AI it = a.begin(); it!=a.end(); ++it){
		if ((it->second).getFound() == false || (it->second).getValid() == false){
			messages.insert((it->second).getMessage());
		}
	}
}

void execute::getMessagesOfFound(A a){
	for (AI it = a.begin(); it!=a.end(); ++it){
		if ((it->second).getFound() == true){
			messages.insert((it->second).getMessage());
		}
	}
}









/*
**
** Set methods
**
*/

void execute::setClasses(string className, string message){
	Info info(className, message, false);
	classes.insert(std::make_pair(className, info));
}

void execute::setDefaultConstructors(string className, bool exist, string notFoundMessage){
	Info info(className, notFoundMessage, false);
	info.setValid(exist);
	defaultConstructors.insert(std::make_pair(className, info));
}

void execute::setCopyConstructors(string className, bool exist, string notFoundMessage){
	Info info(className, notFoundMessage, false, exist);
	copyConstructors.insert(std::make_pair(className, info));
}

void execute::setMoveConstructors(string className, bool exist, string notFoundMessage){
	Info info(className, notFoundMessage, false, exist);
	moveConstructors.insert(std::make_pair(className, info));
}

void execute::setInitializers(string className, vector<string> parameters, vector<string> inits, string message){
	Info info(className, message, false);
	info.setParameters(parameters);
	info.setInitializers(inits);
	initializers.insert(std::make_pair(className, info));
}

void execute::setDestructors(string className, bool exist, string message){
	Info info(className, message, false, exist);
	destructors.insert(std::make_pair(className, info));
}

void execute::setDeleteMembers(string className, string memberName, string message){
	Info info(className, message, false);
	deleteMembers.insert(std::make_pair(memberName, info));
}

void execute::setConstructors(string className, unsigned int ctors, bool lenient, string message){
	Info info(className, message, false);
	info.setExpectedConstructors(ctors);
	info.setLenient(lenient);
	constructors.insert(std::make_pair(className, info));
}

void execute::setExplicitSpecifiedConstructors(string className, vector<string> parameters, string message){
	Info info(className, message, false);
	info.setParameters(parameters);
	info.setValid(true);
	explicitSpecifiedConstructors.insert(std::make_pair(className, info));
}

void execute::setTakeExceptions(string exceptionClass, string message){
	Info info(exceptionClass, message, false);
	takeExceptions.insert(std::make_pair(exceptionClass, info));
}

void execute::setMembers(string className, string memberName, string constant, string message){
	Info info(memberName, message, false);
	info.setClass(className);
	info.setConstant(constant);
	members.insert(std::make_pair(memberName, info));
}

void execute::setStaticVariables(string className, string variableName, string constant, string message){
	Info info(variableName, message, false);
	info.setClass(className);
	info.setConstant(constant);
	staticVariables.insert(std::make_pair(variableName, info));
}

void execute::setReleasedVariables(string message){
	releasedVariableMessage = message;
}

void execute::setMembersAccessLevel(string className, string memberName, string level, string message){
	Info info(memberName, message, false); 
	info.setClass(className);
	info.setLevel(level);
	variableMembersAccessLevel.insert(std::make_pair(memberName, info));
}

void execute::setMethodsWithName(string methodName, string className, vector<string> params, string constant, string message){
	Info info(methodName, message, false);
	info.setClass(className);
	info.setParameters(params);
	info.setConstant(constant);
	methodsWithName.insert(std::make_pair(methodName, info));
}

void execute::setMethodsWithReferencedVariable(string methodName, string className, vector<string> params, string constant, string message){
	Info info(methodName, message, false);
	info.setClass(className);
	info.setParameters(params);
	info.setConstant(constant);
	methodsWithReferencedVariable.insert(std::make_pair(methodName, info));
}

void execute::setMethodsWithReferencedMethod(string mainMethodName, vector<string> mainMethodParams, string mainClassName, string mainConstant, string usedMethodName, vector<string> usedMethodParams, string usedConstant, string message){
	Info info(mainMethodName, message, false);
	info.setClass(mainClassName);
	info.setParameters(mainMethodParams);
	info.setUsedFunction(usedMethodName);
	info.setUsedParameters(usedMethodParams);
	info.setConstant(mainConstant);
	info.setUsedConstant(usedConstant);
	methodsWithReferencedMethod.insert(std::make_pair(mainMethodName, info));
}

void execute::setNoExceptMethods(string methodName, vector<string> params, string className, string constant, string message){
	Info info(className, message, false);
	info.setClass(className);
	info.setParameters(params);
	info.setConstant(constant);
	noExceptMethods.insert(std::make_pair(methodName, info));
}

void execute::setInlineMethods(string methodName, vector<string> params, string className, string constant, string message){
	Info info(className, message, false);
	info.setClass(className);
	info.setParameters(params);
	info.setConstant(constant);
	inlineMethods.insert(std::make_pair(methodName, info));
}

void execute::setDefaultArgumentsInMethod(string methodName, vector<string> params, string className, string constant, unsigned int numDefaultArgs, vector<string> defaultArgs, string message){
	Info info(className, message, false);
	info.setClass(className);
	info.setParameters(params);
	info.setConstant(constant);
	info.setNumDefaultArguments(numDefaultArgs);
	info.setDefaultArguments(defaultArgs);
	defaultArgumentsInMethods.insert(std::make_pair(methodName, info));
}

void execute::setDeletedMethods(string methodName, string className, vector<string> params, string constant, string message) {
	Info info(methodName, message, false);
	info.setClass(className);
	info.setParameters(params);
	info.setConstant(constant);
	deletedMethods.insert(std::make_pair(methodName, info));
}

void execute::setDefaultedMethods(string methodName, string className, vector<string> params, string constant, string message) {
	Info info(methodName, message, false);
	info.setClass(className);
	info.setParameters(params);
	info.setConstant(constant);
	defaultedMethods.insert(std::make_pair(methodName, info));
}


void execute::setCopyAssignmentOperators(string className, bool exist, string message){
	Info info(className, message, false, exist);
	copyAssignmentOperators.insert(std::make_pair(className, info));
}

void execute::setMoveAssignmentOperators(string className, bool exist, string message){
	Info info(className, message, false, exist);
	moveAssignmentOperators.insert(std::make_pair(className, info));
}

void execute::setFunctionsWithReferencedFunction(string mainFunctionName, vector<string> mainFunctionParameters, string usedFunctionName, vector<string> usedFunctionParameters, string message){
	Info info(mainFunctionName, message, false);
	info.setParameters(mainFunctionParameters);
	info.setUsedFunction(usedFunctionName);
	info.setUsedParameters(usedFunctionParameters);
	functionsWithReferencedFunction.insert(std::make_pair(mainFunctionName, info));
}

void execute::setFunctionsWithName(string functionName, vector<string> params, string message){
	Info info(functionName, message, false);
	info.setParameters(params);
	functionsWithName.insert(std::make_pair(functionName, info));
}

void execute::setIncludedHeaders(string functionName, string headerName, bool exist, string message){
	Info info(headerName, message, false, exist);
	info.setValid(false);
	includedHeaders.insert(std::make_pair(functionName, info));
}

void execute::setFileIncludeHeader(vector<string> fileNames, string headerName, string message){
	Info info(headerName, message, false);
	bool found = false;
	for (unsigned int i = 0; i < fileNames.size() && !found; ++i){
		ifstream codeFile( fileNames[i] ) ;
		if (codeFile.is_open()){
			string code((istreambuf_iterator<char>(codeFile)) , istreambuf_iterator<char>() ) ;
			codeFile.close() ;

			std::regex comment_regex("(/\\*(.|\n|\r)*?\\*/)|//.*");
			string str = std::regex_replace(code, comment_regex, "");

			std::regex txt_regex("#include\\s*[<\"]\\s*"+headerName+"\\s*[>\"]"); 
			found |= std::regex_search(str, txt_regex);
		}
	}
	info.setFound(found);
	fileIncludedHeader.insert(std::make_pair(headerName, info));
}

void execute::setGuardClauses(string fileName, string message) {
	Info info(fileName, message, false);
	bool found = false;
	ifstream codeFile(fileName);
	if(codeFile.is_open()){
		string code((istreambuf_iterator<char>(codeFile)), istreambuf_iterator<char>() );
		codeFile.close();

		regex comment_regex("(/\\*(.|\n|\r)*?\\*/)|//.*");
		code = regex_replace(code, comment_regex, "");		//Necesario gcc-4.9 o posterior para buen uso de regex_replace 

		// Dividimos el código en tokens. Buscamos primero la palabra "#define" 
		// luego miramos cual es la siguiente palabra y la guardamos. 
		std::regex ws_re("\\s+");
		found = false;
		bool fin = false;
		std::string token;
		std::sregex_token_iterator it(code.begin(), code.end(), ws_re, -1);
		std::sregex_token_iterator end;

		// Primero buscamos la palabra "#ifndef"
		while(it != end && !fin){
			fin = *it == "#ifndef";
			++it;
		}

		// Luego guardamos la siguiente palabra
		if(fin && it != end) token = *it;
		
		if(!token.empty()){
			fin = false;
			// Buscamos la palabra "#define"
			while(it != end && !fin){
				fin = *it == "#define";
				++it;
			}

			// Miramos si coincide la siguiente palabra con la guardada y en ese caso buscamos "#endif"
			if(it != end && *it == token)
				while(it != end && !found){
					found = *(it) == "#endif";
					++it;
				}
		}
	}
	info.setFound(found);
	guardClauses.insert(make_pair(fileName, info));
}

void execute::setNotFriendMembers(string className, string message){
	Info info(className, message, false);
	notFriendMembers.insert(std::make_pair(className, info));
}

void execute::setFriendFunction(string functionName, string className, vector<string> params, string message){
        Info info(functionName, message, false);
        info.setClass(className);
        info.setParameters(params);
        friendFunctions.insert(std::make_pair(functionName, info));
}

void execute::setFriendClass(string className, string friendClassName, string message){
	Info info(className, message, false);
	info.setClass(className);
	info.setFriendClass(friendClassName);
	friendClasses.insert(std::make_pair(className, info));
}

void execute::setCorrectMessage(string message){
	correctMessage = message;
}

void execute::setIncorrectMessage(string message){
	incorrectMessage = message;
}










/*
**
** Execution methods
**
*/


// ----------------------- CHECK CLASSES ---------------------

void execute::classWithName(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithName = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithName");
	if (existClass(foundClassWithName)){
		for (AI it=classes.begin(); it!=classes.end(); ++it){
			if(std::regex_match(foundClassWithName->getNameAsString(), std::regex(it->first))){
				(it->second).setFound(true);
			}
		}
	}
}

// ----------------------- CHECK CONSTRUCTORS ---------------------

void execute::classWithDefaultConstructor(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithDefaultConstructor = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithDefaultConstructor");
	const CXXConstructorDecl *foundDefaultConstructor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("defaultConstructor");
	if (existClass(foundClassWithDefaultConstructor) && existConstructor(foundDefaultConstructor)){
		for (auto c = clases.begin(); c!=clases.end(); ++c){
			if(std::regex_match(foundClassWithDefaultConstructor->getNameAsString(), std::regex(c->nom))){
				c->isDefault = true;
			}
		}
	}
}

void execute::classWithCopyConstructor(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithCopyConstructor = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithCopyConstructor");
	const CXXConstructorDecl *foundCopyConstructor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("copyConstructor");
	if (existClass(foundClassWithCopyConstructor) && existConstructor(foundCopyConstructor)){
		for (AI it=copyConstructors.begin(); it!=copyConstructors.end(); ++it){
			if(std::regex_match(foundClassWithCopyConstructor->getNameAsString(), std::regex(it->first))){
				(it->second).setFound(true);
			}
		}
	}
}

void execute::classWithMoveConstructor(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithMoveConstructor = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithMoveConstructor");
	const CXXConstructorDecl *foundMoveConstructor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("moveConstructor");
	if (existClass(foundClassWithMoveConstructor) && existConstructor(foundMoveConstructor)){
		for (AI it=moveConstructors.begin(); it!=moveConstructors.end(); ++it){
			if(std::regex_match(foundClassWithMoveConstructor->getNameAsString(), std::regex(it->first))){
				(it->second).setFound(true);
			}
		}
	}
}

void execute::classWithListInitializerConstructor(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithListInitializerConstructor = 
				Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithListInitializerConstructor");
	const CXXConstructorDecl *foundListInitializerConstructor = 
				Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("listInitializerConstructor");

	if (existClass(foundClassWithListInitializerConstructor) && existConstructor(foundListInitializerConstructor) 
		&& foundListInitializerConstructor->getNumCtorInitializers() != 0){

		for (AI it=initializers.begin(); it!=initializers.end(); ++it){
			if(std::regex_match(foundClassWithListInitializerConstructor->getNameAsString(), std::regex(it->first))){

				if(checkParams(foundListInitializerConstructor, (it->second).getParameters())){
					vector<string> inits = (it->second).getInitializers();
					if (checkWithoutParams(inits)){
						(it->second).setFound(true);
					}else{
						if (checkInitializersConstructor(foundListInitializerConstructor, inits)){
							(it->second).setFound(true);
						}
					}
				}
			}
		}
	}
}

void execute::classWithDestructor(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithDestructor = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithDestructor");
	const CXXDestructorDecl *foundDestructor = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("Destructor");
	if (existClass(foundClassWithDestructor) && existDestructor(foundDestructor)){
		for (AI it=destructors.begin(); it!=destructors.end(); ++it){
			if(std::regex_match(foundClassWithDestructor->getNameAsString(), std::regex(it->first))){
				(it->second).setFound(true);
			}
		}
	}
}

 void execute::deleteMember(const MatchFinder::MatchResult &Result){
 	const CXXRecordDecl *foundClassWithDeleteMember = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithDeleteMember");
	const CXXDeleteExpr *foundDeleteExpression = Result.Nodes.getNodeAs<clang::CXXDeleteExpr>("deleteExpression");
	const FieldDecl *foundDeleteMember = Result.Nodes.getNodeAs<clang::FieldDecl>("deleteMember");

	if (existClass(foundClassWithDeleteMember) && existDeleteExpression(foundDeleteExpression)){
		
		if(foundDeleteExpression->isArrayForm()){
			for(AI i = deleteMembers.begin(); i!= deleteMembers.end(); i++){
				if (std::regex_match(foundDeleteMember->getNameAsString(), std::regex(i->first))){
					(i->second).setFound(true);
				}
			}	
		}
	}
}

void execute::numberOfConstructor(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithConstructor = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithConstructor");
	const CXXConstructorDecl *foundConstructor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("foundConstructor");
	
	if (existClass(foundClassWithConstructor) && existConstructor(foundConstructor)){
		
		for (AI it=constructors.begin(); it!=constructors.end(); ++it){
			if(std::regex_match(foundClassWithConstructor->getNameAsString(), std::regex(it->first))){
				unsigned int numberOfConstructors = (it->second).getNumberOfConstructors();
				numberOfConstructors++;
				(it->second).setNumberOfConstructors(numberOfConstructors);				
			}
		}
	}
}

void execute::explicitSpecifiedConstructor(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithExplicitSpecifiedConstructor = 
			Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithExplicitSpecifiedConstructor");
	const CXXConstructorDecl *foundExplicitSpecifiedConstructor = 
			Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("explicitSpecifiedConstructor");

	if (existClass(foundClassWithExplicitSpecifiedConstructor)){

		for (AI it=explicitSpecifiedConstructors.begin(); it!=explicitSpecifiedConstructors.end(); ++it){
			if(std::regex_match(foundClassWithExplicitSpecifiedConstructor->getNameAsString(), std::regex(it->first))){
				if(checkParams(foundExplicitSpecifiedConstructor, (it->second).getParameters())){
					if(foundExplicitSpecifiedConstructor->isExplicitSpecified()){
				   		(it->second).setFound(true);
				   	}
				}
			}
		}
	}
}

void execute::takeException(const MatchFinder::MatchResult &Result){
	if (const CXXThrowExpr *foundException = Result.Nodes.getNodeAs<clang::CXXThrowExpr>("exception")){
		FullSourceLoc FullLocation1 = Context->getFullLoc(foundException->getLocStart());
      		const CXXRecordDecl *foundExceptionClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("exceptionClass");
		if (FullLocation1.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation1)){	
			for(AI i = takeExceptions.begin(); i!= takeExceptions.end(); i++){
				if (i->first == foundExceptionClass->getNameAsString()){
					(i->second).setFound(true);
					break;
				}
			}				
		}
	}
}



// ----------------------- CHECK VARIABLES ---------------------
void execute::classWithMember(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("foundClass");
	const FieldDecl *foundMember = Result.Nodes.getNodeAs<clang::FieldDecl>("foundMember");

	if (existClass(foundClass) && existMember(foundMember)){
		for (AI it=members.begin(); it!=members.end(); ++it){
			if(std::regex_match(foundMember->getNameAsString(), std::regex(it->first)) && 
			   std::regex_match(foundClass->getNameAsString(), std::regex((it->second).getClass()))){

				QualType ft =  foundMember->getType().getCanonicalType();
			        if(ft->isPointerType()){
					ft = ft->getPointeeType();					  
			      	}

				if (markedConst((it->second).getConstant(), ft.isConstQualified())){
					(it->second).setFound(true);
				}
			}
		}
	}
}

void execute::classWithStaticVariable(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("foundClass");
	const VarDecl *foundStaticVariable = Result.Nodes.getNodeAs<clang::VarDecl>("foundStaticVariable");
	if (existClass(foundClass) && existVariable(foundStaticVariable)){
		
		for (AI it=staticVariables.begin(); it!=staticVariables.end(); ++it){
			
			if(std::regex_match(foundStaticVariable->getNameAsString(), std::regex(it->first)) && 
			   std::regex_match(foundClass->getNameAsString(), std::regex((it->second).getClass()))){

			   	QualType ft =  foundStaticVariable->getType().getCanonicalType();
			    	if(ft->isPointerType()){
					ft = ft->getPointeeType();					  
			    	}

				if (markedConst((it->second).getConstant(), ft.isConstQualified()) &&
					foundStaticVariable->isStaticDataMember()){
					(it->second).setFound(true);
				}
			}
		}
	}
}

void execute::releaseVariable(const MatchFinder::MatchResult &Result){
	
	if (const VarDecl *FS = Result.Nodes.getNodeAs<clang::VarDecl>("variable_new")){

		FullSourceLoc FullLocation = Context->getFullLoc(FS->getLocStart());
      		
		if (FullLocation.isValid() 
			&& !Context->getSourceManager().isInSystemHeader(FullLocation) 
		){
			bool found = false;
			for (map<const VarDecl*, bool>::iterator it=variables_new.begin(); it!=variables_new.end(); it++){	
				if( it->first->getCanonicalDecl() == FS->getCanonicalDecl() ){	
					found = true;
					break;	
				}
			}

			if(!found){
				variables_new.insert(pair<const VarDecl*, bool>(FS, false));
			}
		}
	}

	if (const VarDecl *FS = Result.Nodes.getNodeAs<clang::VarDecl>("variable_delete")){

		FullSourceLoc FullLocation = Context->getFullLoc(FS->getLocStart());
      		
		if (FullLocation.isValid() 
			&& !Context->getSourceManager().isInSystemHeader(FullLocation) 
		){
			for (map<const VarDecl*, bool>::iterator it=variables_new.begin(); it!=variables_new.end(); it++){	
				if( it->first->getCanonicalDecl() == FS->getCanonicalDecl() ){
					it->second = true;
				}
			}
		}
	}
}

void execute::classWithVariableMembersAccessLevel(const MatchFinder::MatchResult &Result){
        const CXXRecordDecl *foundClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("foundClassAccessLevel");
        const FieldDecl *foundMember = Result.Nodes.getNodeAs<clang::FieldDecl>("foundMemberVariableAccessLevel");

        if (existClass(foundClass) && existMember(foundMember)){

                for (AI it=variableMembersAccessLevel.begin(); it!=variableMembersAccessLevel.end(); ++it){
                        if(std::regex_match(foundMember->getNameAsString(), std::regex(it->first)) &&
                           std::regex_match(foundClass->getNameAsString(), std::regex((it->second).getClass()))){

				bool accessCorrect = false;
				switch(foundMember->getAccess()){	
						case AS_public: 
							accessCorrect = (it->second).getLevel() == "public";
							break;
						case AS_protected: 
							accessCorrect = (it->second).getLevel() == "protected";
							break;
						case AS_none:
						case AS_private: 
							accessCorrect = (it->second).getLevel() == "private";
							break;
				}

				if(accessCorrect)
	                                (it->second).setFound(true);
                        }
                }
        }
}



// ----------------------- CHECK METHODS ---------------------

void execute::methodWithName(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithNameWithMethod = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithMethod");
	const CXXMethodDecl *foundMethodWithName = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("methodWithName");
	if (existClass(foundClassWithNameWithMethod) && existMethod(foundMethodWithName) == true){
		for (AI it=methodsWithName.begin(); it!=methodsWithName.end(); ++it){
			if(checkRegularExpresion(foundMethodWithName->getNameAsString(), it->first)){
				if(checkParams(foundMethodWithName,(it->second).getParameters())){
					if (markedConst((it->second).getConstant(), foundMethodWithName->isConst())){
						(it->second).setFound(true);
					}
				}
			}
		}
	}
}

void execute::methodWithReferencedVariable(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithMethodVariable = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithMethodVariable");
	const CXXMethodDecl *foundMethodWithReferencedVariable = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("methodWithReferencedVariable");
	if (existClass(foundClassWithMethodVariable) && existMethod(foundMethodWithReferencedVariable) == true){
		const FieldDecl *foundReferencedVariable = Result.Nodes.getNodeAs<clang::FieldDecl>("referencedVariable");
		if (foundReferencedVariable){
			for (AI it=methodsWithReferencedVariable.begin(); it!=methodsWithReferencedVariable.end(); ++it){
				if(checkRegularExpresion(foundMethodWithReferencedVariable->getNameAsString(),it->first)){
					if(checkParams(foundMethodWithReferencedVariable,(it->second).getParameters())){
						if (markedConst((it->second).getConstant(), foundMethodWithReferencedVariable->isConst())){
							(it->second).setFound(true);
						}
					}
				}
			}
		}
	}				
}

void execute::methodWithReferencedMethod(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithReferencedMethod = 0;
	const CXXMethodDecl *foundMainMethod = 0;

	const CXXRecordDecl *foundClassOfReferencedMethod = 0; 
	const CXXMethodDecl *foundUsedMethod = 0; 

	foundClassWithReferencedMethod = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithReferencedMethod");
	foundMainMethod = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("mainMethod");
	
	if (existClass(foundClassWithReferencedMethod) && existMethod(foundMainMethod) == true){
		
		foundClassOfReferencedMethod = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classOfReferencedMethod");
		foundUsedMethod = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("usedMethod");

		if(existClass(foundClassOfReferencedMethod) && existMethod(foundUsedMethod) == true){

			//llvm::outs() << foundMainMethod->getNameAsString() << " y " << foundUsedMethod->getNameAsString() << "\n";

			for (AI it=methodsWithReferencedMethod.begin(); it!=methodsWithReferencedMethod.end(); ++it){

				if(checkRegularExpresion(foundMainMethod->getNameAsString(), it->first)){
					if(checkParams(foundMainMethod,(it->second).getParameters())){
					    if (markedConst((it->second).getConstant(), foundMainMethod->isConst())){
					
						if(checkRegularExpresion(foundUsedMethod->getNameAsString(), (it->second).getUsedFunction()))
						{	
							if(checkParams(foundUsedMethod,(it->second).getUsedParameters())){
								if (markedConst((it->second).getUsedConstant(), foundUsedMethod->isConst())){
									(it->second).setFound(true);
								}
							}
						}
					    }
					}
				}
			}
		}
	}				
}

void execute::noExceptMethod(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithNoExceptMethod = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithNoExceptMethod");
	const CXXMethodDecl *foundNoExceptMethod = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("noExceptMethod");

	if (existClass(foundClassWithNoExceptMethod) && existMethod(foundNoExceptMethod) == true){

		for (AI it=noExceptMethods.begin(); it!=noExceptMethods.end(); ++it){

			if(checkRegularExpresion(foundNoExceptMethod->getNameAsString(), it->first)){

				if(checkParams(foundNoExceptMethod,(it->second).getParameters())){
					if (markedConst((it->second).getConstant(), foundNoExceptMethod->isConst())){
						const FunctionProtoType *FP = foundNoExceptMethod->getType()->castAs<FunctionProtoType>();
						if(FP && FP->isNothrow(*Context)){
							(it->second).setFound(true);
						}
					}
				}
			}
		}
	}
}


// ----------------------- METHODS INLINE ---------------------
void execute::inlineMethod(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithInlineMethod = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithInlineMethod");
	const CXXMethodDecl *foundInlineMethod = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("inlineMethod");

	if (existClass(foundClassWithInlineMethod) && existMethod(foundInlineMethod) == true){
				
		if(foundInlineMethod->isInlined()){

			for (AI it=inlineMethods.begin(); it!=inlineMethods.end(); ++it){
					
				if(checkRegularExpresion(foundInlineMethod->getNameAsString(), it->first))
				{
					if(checkParams(foundInlineMethod,(it->second).getParameters())){
						if (markedConst((it->second).getConstant(), foundInlineMethod->isConst())){
							(it->second).setFound(true);
						}
					}
				}
			}
		}
	}
}

// ----------------------- METHODS INLINE ---------------------
void execute::defaultArgumentsInMethod(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithDefArgsInMethod = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithDefaultArgumentsInMethod");
	const CXXMethodDecl *foundDefArgsInMethod = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("defaultArgumentsInMethod");

	if (existClass(foundClassWithDefArgsInMethod) && existMethod(foundDefArgsInMethod) == true){
				
		for (AI it=defaultArgumentsInMethods.begin(); it!=defaultArgumentsInMethods.end(); ++it){
					
			if(checkRegularExpresion(foundDefArgsInMethod->getNameAsString(), it->first))
			{
				if(checkParams(foundDefArgsInMethod,(it->second).getParameters())){
					if (markedConst((it->second).getConstant(), foundDefArgsInMethod->isConst())){

						if(checkDefArgs(foundDefArgsInMethod, (it->second).getNumDefaultArguments(), (it->second).getDefaultArguments())){
							(it->second).setFound(true);
						}
					}
				}
			}
		}
	}
}

// ----------------------- METHODS DELETED ---------------------
void execute::deletedMethod(const MatchFinder::MatchResult &Result) {

	const CXXRecordDecl *foundClassWithDeletedMethod = 
			Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithDeletedMethod");
	const CXXMethodDecl *foundDeletedMethod = 
			Result.Nodes.getNodeAs<clang::CXXMethodDecl>("deletedMethod");

	if (existClass(foundClassWithDeletedMethod) && existMethod(foundDeletedMethod)) {

		for (AI it = deletedMethods.begin(); it != deletedMethods.end(); ++it) {

			if(checkRegularExpresion(foundDeletedMethod->getNameAsString(), it->first) && 
			   checkParams(foundDeletedMethod, (it->second).getParameters()) &&
			   markedConst((it->second).getConstant(), foundDeletedMethod->isConst())) {
				(it->second).setFound(true);
			}
		}
	}
}

// ----------------------- METHODS DEFAULT ---------------------
void execute::defaultedMethod(const MatchFinder::MatchResult &Result) {

	const CXXRecordDecl *foundClassWithDefaultedMethod = 
			Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithDefaultedMethod");
	const CXXMethodDecl *foundDefaultedMethod = 
			Result.Nodes.getNodeAs<clang::CXXMethodDecl>("defaultedMethod");

	if (existClass(foundClassWithDefaultedMethod) && foundDefaultedMethod) {

		FullSourceLoc FullLocation = Context->getFullLoc(foundDefaultedMethod->getLocStart());
        
        if (FullLocation.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation)) {

			for (AI it = defaultedMethods.begin(); it != defaultedMethods.end(); ++it) {

				if(checkRegularExpresion(foundDefaultedMethod->getNameAsString(), it->first) && 
				   checkParams(foundDefaultedMethod, (it->second).getParameters()) &&
				   markedConst((it->second).getConstant(), foundDefaultedMethod->isConst())) {
					(it->second).setFound(true);
				}
			}
		}
	}
}

// ----------------------- CHECK OPERATORS ---------------------
void execute::classWithCopyAssignmentOperator(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithCopyAssignmentOperator = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithCopyAssignmentOperator");
	const CXXMethodDecl *foundCopyAssignmentOperator = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("copyAssignmentOperator");
	if (existClass(foundClassWithCopyAssignmentOperator) && existMethod(foundCopyAssignmentOperator)){
		for (AI it=copyAssignmentOperators.begin(); it!=copyAssignmentOperators.end(); ++it){
			if(std::regex_match(foundClassWithCopyAssignmentOperator->getNameAsString(), std::regex(it->first))){
				(it->second).setFound(true);
			}
		}
	}
}

void execute::classWithMoveAssignmentOperator(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithMoveAssignmentOperator = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithMoveAssignmentOperator");
	const CXXMethodDecl *foundMoveAssignmentOperator = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("moveAssignmentOperator");
	if (existClass(foundClassWithMoveAssignmentOperator) && existMethod(foundMoveAssignmentOperator)){
		for (AI it=moveAssignmentOperators.begin(); it!=moveAssignmentOperators.end(); ++it){
			if(std::regex_match(foundClassWithMoveAssignmentOperator->getNameAsString(), std::regex(it->first))){
				(it->second).setFound(true);
			}
		}
	}
}

void execute::functionWithReferencedFunction(const MatchFinder::MatchResult &Result){
	const FunctionDecl *foundMainFunction = Result.Nodes.getNodeAs<clang::FunctionDecl>("mainFunction");
	const FunctionDecl *foundUsedFunction = Result.Nodes.getNodeAs<clang::FunctionDecl>("usedFunction");

	if (existFunction(foundMainFunction)){

		for (AI it=functionsWithReferencedFunction.begin(); it!=functionsWithReferencedFunction.end(); ++it){
			if(checkRegularExpresion(foundMainFunction->getNameAsString(), it->first)){
				if (checkParams(foundMainFunction, (it->second).getParameters())){		

					if(checkRegularExpresion(foundUsedFunction->getNameAsString(), (it->second).getUsedFunction()))
					{			
						if(checkParams(foundUsedFunction,(it->second).getUsedParameters())){	
							(it->second).setFound(true);
						}
					}					
				}
			}
		}
	}
}

void execute::functionWithName(const MatchFinder::MatchResult &Result){
	const FunctionDecl *foundFunctionWithName = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionWithName");
	if (existFunction(foundFunctionWithName)){ 

	    for (AI it=functionsWithName.begin(); it!=functionsWithName.end(); ++it){

		if(checkRegularExpresion(foundFunctionWithName->getNameAsString(),it->first)){
//			llvm::outs() << foundFunctionWithName->getType().getCanonicalType().getAsString() << "\n";

				if (checkParams(foundFunctionWithName, (it->second).getParameters())){
					(it->second).setFound(true);
			}
		}	
	   }
	}
 }


// ----------------------- CHECK HEADERS ---------------------


void execute::includedHeader(const MatchFinder::MatchResult &Result){
	if (const FunctionDecl *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionCalled")){
		FullSourceLoc FullLocation1 = Context->getFullLoc(FS->getLocStart());
		if (FullLocation1.isValid() ){
			FullSourceLoc FullLocation2 = Context->getFullLoc(Context->getSourceManager().getIncludeLoc(FullLocation1.getFileID()));
			std::pair<AI, AI> rango = includedHeaders.equal_range(FS->getNameAsString());
			for (AI i = rango.first; i != rango.second; ++i){
				if(FullLocation2.isValid()){
					(i->second).setFound(true);
					 string headerName = (i->second).getName();
                                        if( string(Context->getSourceManager().getFileEntryForID(FullLocation2.getFileID())->getName()).find(headerName) != std::string::npos){	
						(i->second).setValid(true);
					}
				}        
			}
		}
	}
}





// ----------------------- FRIEND MEMBERS---------------------
void execute::classWithNotFriendMember(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithFriendMember = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithFriendMember");
	if (existClass(foundClassWithFriendMember)){
		for (AI it=notFriendMembers.begin(); it!=notFriendMembers.end(); ++it){
			if(std::regex_match(foundClassWithFriendMember->getNameAsString(), std::regex(it->first))){
				(it->second).setFound(true);
			}
		}
	}
}

void execute::classWithFriendFunction(const MatchFinder::MatchResult &Result){
	const FunctionDecl *foundFriendFunction = Result.Nodes.getNodeAs<clang::FunctionDecl>("friendFunction");
	const CXXRecordDecl *foundClassWithFriendFunction = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithFriendFunction");

	if (foundFriendFunction && foundClassWithFriendFunction){
                FullSourceLoc FullLocation = Context->getFullLoc(foundFriendFunction->getLocStart());
                if (FullLocation.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation)){

            	    for (AI it=friendFunctions.begin(); it!=friendFunctions.end(); ++it){

	                if(checkRegularExpresion(foundFriendFunction->getNameAsString(),it->first)){
			   if(checkRegularExpresion(foundClassWithFriendFunction->getNameAsString(), (it->second).getClass())){
        	                if (checkParams(foundFriendFunction, (it->second).getParameters())){
                                	        (it->second).setFound(true);
                        	}
			    }
                	}
		    }
           	}
	}
}


void execute::classWithFriendClass(const MatchFinder::MatchResult &Result){
	const CXXRecordDecl *foundClassWithFriendClass = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("classWithFriendClass");

	if(foundClassWithFriendClass){
		FullSourceLoc FullLocation = Context->getFullLoc(foundClassWithFriendClass->getLocStart());
                if (FullLocation.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocation)){

		     for (AI it=friendClasses.begin(); it!=friendClasses.end(); ++it){

			if(checkRegularExpresion(foundClassWithFriendClass->getNameAsString(), it->first)){
	            		if(friend_class(foundClassWithFriendClass, (it->second).getFriendClass())){
					(it->second).setFound(true);
				}
			}
		    }
		}
	}
}

bool execute::friend_class(const CXXRecordDecl* c1, string c2) {

        for(CXXRecordDecl::friend_iterator fi=c1->friend_begin(); fi != c1->friend_end(); fi++){

                if( (*fi)->getFriendType() != NULL){

                        QualType qt = (*fi)->getFriendType()->getType().getCanonicalType();

		//	if(qt->getAsCXXRecordDecl() != NULL){
		//			llvm::outs() << qt->getAsCXXRecordDecl()->getNameAsString();
		//	}

                        if((qt->getAsCXXRecordDecl() != NULL) && checkRegularExpresion(qt->getAsCXXRecordDecl()->getNameAsString(), c2)){      
                                return true;
                        }
                }
        }

        return false;
}

