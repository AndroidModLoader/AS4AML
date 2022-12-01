#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include <dlfcn.h>

#include <angelscript.h>

MYMODCFG(net.rusjj.as4aml, AngelScript for AML, 1.0, RusJJ)

asIScriptEngine *engine;
void MessageCallback(const asSMessageInfo *msg, void *param)
{
    if(msg->type == asMSGTYPE_WARNING || msg->type == asMSGTYPE_INFORMATION) 
        logger->Info("%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
    else
        logger->Error("%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
}
extern "C" void OnModPreLoad()
{
    logger->SetTag("AngelScript AML");
    
    engine = asCreateScriptEngine();
    if(!engine)
    {
        logger->Error("Failed to create AngelScript engine!");
        return;
    }
    engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
    RegisterStdString(engine);
    
    RegisterInterface("AngelScript", engine);
}
