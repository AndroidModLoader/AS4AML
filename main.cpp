#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include <string.h>
#include <dlfcn.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h> // mkdir
#include <sys/sendfile.h> // sendfile
#include <fcntl.h> // "open" flags

#include <angelscript.h>

#include <datetime/datetime.h>
#include <scriptarray/scriptarray.h>
#include <amlscriptbuilder.h>
#include <scripthandle/scripthandle.h>
#include <scripthelper/scripthelper.h>
#include <scriptmath/scriptmath.h>
#include <scriptstdstring/scriptstdstring.h>

MYMODCFG(net.rusjj.as4aml, AngelScript for AML, 0.1, RusJJ)

asIScriptEngine *engine;

void AddAS4AMLFuncs();
void MessageCallback(const asSMessageInfo *msg, void *param)
{
    if(msg->type == asMSGTYPE_WARNING)
        logger->Print(LogP_Warn, "%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
    else if(msg->type == asMSGTYPE_INFORMATION) 
        logger->Info("%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
    else
        logger->Error("%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
}
void SimplePrint(std::string &msg)
{
    asIScriptContext *ctx = asGetActiveContext();
    logger->Info("<%s>: %s", ctx->GetFunction(0)->GetModuleName(), msg.c_str());
}
inline bool EndsWithAS(const char* base)
{
    static int blen;
    blen = strlen(base);
    return (blen >= 3) && (!strcmp(base + blen - 3, ".as"));
}
void LoadAS(const char* path)
{
    char buf[0xFF];
    DIR* dir = opendir(path);
    if (dir != NULL)
    {
        struct dirent *diread; void* handle;
        while ((diread = readdir(dir)) != NULL)
        {
            if(diread->d_name[0] == '.') continue; // Skip . and ..
            if(!EndsWithAS(diread->d_name))
            {
                //logger->Error("File %s is not a script, atleast it is NOT AngelScript file!", diread->d_name);
                continue;
            }
            int r = builder->StartNewModule(engine, diread->d_name);
            if(r < 0)
            {
                logger->Error("Failed to start a script module!");
                continue;
            }
            sprintf(buf, "%s/%s", path, diread->d_name);
            r = builder->AddSectionFromFile(buf);
            if(r < 0)
            {
                logger->Error("Failed to load a script!");
                continue;
            }
            
            asIScriptFunction *func = builder->BuildAMLModule()->GetFunctionByDecl("void main()");
            if(func)
            {
                asIScriptContext *ctx = engine->CreateContext();
                ctx->Prepare(func);
                r = ctx->Execute();
                if( r == asEXECUTION_EXCEPTION )
                {
                    logger->Error("An exception occurred:\n%s", ctx->GetExceptionString());
                }
                ctx->Release();
            }
            
            logger->Info("%s has been loaded! woo!", diread->d_name);
        }
        closedir(dir);
    }
    else
    {
        logger->Error("Failed to load mods: DIR IS NOT OPEN");
    }
}
extern "C" void OnModPreLoad()
{
    logger->SetTag("AngelScript AML");
    
    logger->Info("Launching AngelScript for %s", aml->GetCurrentGame());
    engine = asCreateScriptEngine();
    if(!engine)
    {
        logger->Error("Failed to start AngelScript engine!");
        return;
    }
    logger->Info("AngelScript has been started!");
    engine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
    
    #ifdef AS4AML_NO_LINE_CUES
    engine->SetEngineProperty(asEP_BUILD_WITHOUT_LINE_CUES, true);
    #endif // AS4AML_NO_LINE_CUES
    
    // Add-ons
    RegisterStdString(engine);
    RegisterScriptDateTime(engine);
    RegisterScriptArray(engine, true);
    RegisterStdStringUtils(engine);
    RegisterScriptHandle(engine);
    RegisterExceptionRoutines(engine);
    RegisterScriptMath(engine);
    
    // Some basic funcs
    engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(SimplePrint), asCALL_CDECL);
    AddAS4AMLFuncs();
    
    // Register an interface
    RegisterInterface("AngelScript", engine);
    RegisterInterface("AS4AML", as4aml);
}

extern "C" void OnAllModsLoaded()
{
    // Described below.
    char buf[0xFF];
    bool supportsExtDir = aml->HasModOfVersion("net.rusjj.aml", "1.0.1");
    if(supportsExtDir)
    {
        //sprintf(buf, "%s/%s/%s", g_szInternalStoragePath, szInternalModsDir, g_szAppName);
        //LoadAS(buf);
    }
    sprintf(buf, "/sdcard/Android/data/%s/angelscript", aml->GetCurrentGame());
    logger->Info("Starting to load scripts...");
    mkdir(buf, 0777);
    LoadAS(buf);
    
    asUINT c = engine->GetModuleCount();
    for(asUINT i = 0; i < c; ++i)
    {
        asIScriptModule* mdl = engine->GetModuleByIndex(i);
        if(!mdl) continue;
        
        asIScriptFunction *func = mdl->GetFunctionByDecl("void OnAllScriptsLoaded()");
        if(func)
        {
            asIScriptContext *ctx = engine->CreateContext();
            ctx->Prepare(func);
            int r = ctx->Execute();
            if( r == asEXECUTION_EXCEPTION )
            {
                logger->Error("An exception occurred:\n%s", ctx->GetExceptionString());
            }
            ctx->Release();
        }
    }
}

extern "C" void OnModUnload()
{
    engine->Release();
}

static AMLScriptBuilder builderLocal;
AMLScriptBuilder* builder = &builderLocal;
