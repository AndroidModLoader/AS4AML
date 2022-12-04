#include "amlscriptbuilder.h"
#include <mod/logger.h>
#include <vector>

std::vector<ScriptRegisteredCB> g_vecSRCB;
std::vector<asIScriptFunction*> g_vecFunc_GameStart;
std::vector<asIScriptFunction*> g_vecFunc_GamePause;
std::vector<asIScriptFunction*> g_vecFunc_GameResume;
std::vector<asIScriptFunction*> g_vecFunc_GameStop;
std::vector<asIScriptFunction*> g_vecFunc_Update;

asIScriptModule* AMLScriptBuilder::BuildAMLModule()
{
    asIScriptFunction* f;
    BuildModule();
    f = module->GetFunctionByDecl("void GameStart()");
    if(f) g_vecFunc_GameStart.push_back(f);
    f = module->GetFunctionByDecl("void GamePause()");
    if(f) g_vecFunc_GamePause.push_back(f);
    f = module->GetFunctionByDecl("void GameResume()");
    if(f) g_vecFunc_GameResume.push_back(f);
    f = module->GetFunctionByDecl("void GameStop()");
    if(f) g_vecFunc_GameStop.push_back(f);
    f = module->GetFunctionByDecl("void Update()");
    if(f) g_vecFunc_Update.push_back(f);
    
    int count = g_vecSRCB.size();
    for(int i = 0; i < count; ++i) g_vecSRCB[i](module);
    
    return module;
}

void AMLScriptBuilder::Scripts_GameStart()
{
    asIScriptContext *ctx;
    int count = g_vecFunc_GameStart.size();
    for(int i = 0; i < count; ++i)
    {
        ctx = engine->CreateContext();
        ctx->Prepare(g_vecFunc_GameStart[i]);
        int r = ctx->Execute();
        if(r == asEXECUTION_EXCEPTION) logger->Error("An exception occurred:\n%s", ctx->GetExceptionString());
        ctx->Release();
    }
}

void AMLScriptBuilder::Scripts_GamePause()
{
    asIScriptContext *ctx;
    int count = g_vecFunc_GamePause.size();
    for(int i = 0; i < count; ++i)
    {
        ctx = engine->CreateContext();
        ctx->Prepare(g_vecFunc_GamePause[i]);
        int r = ctx->Execute();
        if(r == asEXECUTION_EXCEPTION) logger->Error("An exception occurred:\n%s", ctx->GetExceptionString());
        ctx->Release();
    }
}

void AMLScriptBuilder::Scripts_GameResume()
{
    asIScriptContext *ctx;
    int count = g_vecFunc_GameResume.size();
    for(int i = 0; i < count; ++i)
    {
        ctx = engine->CreateContext();
        ctx->Prepare(g_vecFunc_GameResume[i]);
        int r = ctx->Execute();
        if(r == asEXECUTION_EXCEPTION) logger->Error("An exception occurred:\n%s", ctx->GetExceptionString());
        ctx->Release();
    }
}

void AMLScriptBuilder::Scripts_GameStop()
{
    asIScriptContext *ctx;
    int count = g_vecFunc_GameStop.size();
    for(int i = 0; i < count; ++i)
    {
        ctx = engine->CreateContext();
        ctx->Prepare(g_vecFunc_GameStop[i]);
        int r = ctx->Execute();
        if(r == asEXECUTION_EXCEPTION) logger->Error("An exception occurred:\n%s", ctx->GetExceptionString());
        ctx->Release();
    }
}

void AMLScriptBuilder::Scripts_Update()
{
    asIScriptContext *ctx;
    int count = g_vecFunc_Update.size();
    for(int i = 0; i < count; ++i)
    {
        ctx = engine->CreateContext();
        ctx->Prepare(g_vecFunc_Update[i]);
        int r = ctx->Execute();
        if(r == asEXECUTION_EXCEPTION) logger->Error("An exception occurred:\n%s", ctx->GetExceptionString());
        ctx->Release();
    }
}

class AS4AML : public IAS4AML
{
    void OnScriptRegisteredCallback(ScriptRegisteredCB cb)
    {
        if(cb) g_vecSRCB.push_back(cb);
    }
    
    void Scripts_GameStart() { builder->Scripts_GameStart(); }
    void Scripts_GamePause() { builder->Scripts_GamePause(); }
    void Scripts_GameResume() { builder->Scripts_GameResume(); }
    void Scripts_GameStop() { builder->Scripts_GameStop(); }
    void Scripts_Update() { builder->Scripts_Update(); }
};
static AS4AML as4amlLocal;
IAS4AML* as4aml = &as4amlLocal;
