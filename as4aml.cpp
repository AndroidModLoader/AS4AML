#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include <angelscript.h>
#include <dlfcn.h>
#include <string>

// Variables
extern asIScriptEngine *engine;

// Funcs
void DoScriptsCall(std::string &fnDecl)
{
    asUINT c = engine->GetModuleCount();
    for(asUINT i = 0; i < c; ++i)
    {
        asIScriptContext *ctx = asGetActiveContext();
        
        asIScriptModule* mdl = engine->GetModuleByIndex(i);
        if(!mdl || mdl == ctx->GetFunction(0)->GetModule()) continue;
        
        asIScriptFunction *func = mdl->GetFunctionByDecl(fnDecl.c_str());
        if(func)
        {
            ctx = engine->CreateContext();
            ctx->Prepare(func);
            int r = ctx->Execute();
            if( r == asEXECUTION_EXCEPTION )
            {
                logger->Error("An exception '%s' occurred. Please correct the code and try again.\n", ctx->GetExceptionString());
            }
            ctx->Release();
        }
    }
}
unsigned int GetLibraryAddress(std::string &lib)
{
    return aml->GetLib(lib.c_str());
}
unsigned int GetLibraryHandle(std::string &lib)
{
    return (unsigned int)dlopen(lib.c_str(), RTLD_LAZY);
}
unsigned int GetLibraryHandleSymbol(unsigned int &hndl, std::string &sym)
{
    return (unsigned int)aml->GetSym((void*)hndl, sym.c_str());
}
std::string GetMyName()
{
    asIScriptContext *ctx = asGetActiveContext();
    return ctx->GetFunction(0)->GetModuleName();
}

// Kinda main thing here
void AddAS4AMLFuncs()
{
    engine->RegisterGlobalFunction("void AML_DoScriptsCall(const string &in)", asFUNCTION(DoScriptsCall), asCALL_CDECL);
    engine->RegisterGlobalFunction("uint AML_GetLibraryAddress(const string &in)", asFUNCTION(GetLibraryAddress), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint AML_GetLibraryHandle(const string &in)", asFUNCTION(GetLibraryHandle), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint AML_GetLibraryHandleSymbol(const uint &in, const string &in)", asFUNCTION(GetLibraryHandleSymbol), asCALL_STDCALL);
    engine->RegisterGlobalFunction("string AML_GetMyName()", asFUNCTION(GetMyName), asCALL_STDCALL);
    
}
