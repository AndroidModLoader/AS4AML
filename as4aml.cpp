#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include <angelscript.h>
#include <dlfcn.h>
#include <string>

// Variables
extern asIScriptEngine *engine;

// Funcs
float GetPtrAddrFloat(uintptr_t ptr) { return *(float*)ptr; }
double GetPtrAddrDouble(uintptr_t ptr) { return *(double*)ptr; }
bool GetPtrAddrBool(uintptr_t ptr) { return *(bool*)ptr; }
int8_t GetPtrAddrInt8(uintptr_t ptr) { return *(int8_t*)ptr; }
uint8_t GetPtrAddrUInt8(uintptr_t ptr) { return *(uint8_t*)ptr; }
int16_t GetPtrAddrInt16(uintptr_t ptr) { return *(int16_t*)ptr; }
uint16_t GetPtrAddrUInt16(uintptr_t ptr) { return *(uint16_t*)ptr; }
int32_t GetPtrAddrInt(uintptr_t ptr) { return *(int32_t*)ptr; }
uint32_t GetPtrAddrUInt(uintptr_t ptr) { return *(uint32_t*)ptr; }
int64_t GetPtrAddrInt64(uintptr_t ptr) { return *(int64_t*)ptr; }
uint64_t GetPtrAddrUInt64(uintptr_t ptr) { return *(uint64_t*)ptr; }

void SetPtrAddrFloat(uintptr_t ptr, float val) { *(float*)ptr = val; }
void SetPtrAddrDouble(uintptr_t ptr, double val) { *(double*)ptr = val; }
void SetPtrAddrBool(uintptr_t ptr, bool val) { *(bool*)ptr = val; }
void SetPtrAddrInt8(uintptr_t ptr, int8_t val) { *(int8_t*)ptr = val; }
void SetPtrAddrUInt8(uintptr_t ptr, uint8_t val) { *(uint8_t*)ptr = val; }
void SetPtrAddrInt16(uintptr_t ptr, int16_t val) { *(int16_t*)ptr = val; }
void SetPtrAddrUInt16(uintptr_t ptr, uint16_t val) { *(uint16_t*)ptr = val; }
void SetPtrAddrInt(uintptr_t ptr, int32_t val) { *(int32_t*)ptr = val; }
void SetPtrAddrUInt(uintptr_t ptr, uint32_t val) { *(uint32_t*)ptr = val; }
void SetPtrAddrInt64(uintptr_t ptr, int64_t val) { *(int64_t*)ptr = val; }
void SetPtrAddrUInt64(uintptr_t ptr, uint64_t val) { *(uint64_t*)ptr = val; }

typedef uint16_t (*voidcallFn)(void*,void*,void*,void*,void*,void*,void*,void*,void*,void*,void*,void*,void*,void*,void*);
uint16_t docallVal = 0;
inline uint16_t DoCallVoid(uintptr_t fnAddr, void* a[])
{
    return ((voidcallFn)fnAddr)(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9], a[10], a[11], a[12], a[13], a[14]);
}
void DoCall(asIScriptGeneric *gen)
{
    uintptr_t addr = *(uintptr_t*)gen->GetArgAddress(0);
    void* args[15] = {0};
    
    for (int i = 1; i < 16; ++i)
    {
        void* ref = gen->GetArgAddress(i);
        int typ = gen->GetArgTypeId(i);
        if(typ == 67108876)
        {
            std::string s = *(std::string*)(ref);
            args[i - 1] = (void*)s.c_str();
        }
        else
        {
            args[i - 1] = *(void**)ref;
        }

        /*switch(typ)
        {
            case 67108876: //string?
            {
                std::string local = *(std::string*)(ref);
                args[i - 1] = (void*)local.c_str();
                break;
            }
            case 2:
            {
                args[i - 1] = (void*)(*(char*)ref);
                break;
            }
            case 3:
            {
                args[i - 1] = (void*)(*(short*)ref);
                break;
            }
            case 4:
            {
                args[i - 1] = (void*)(*(int*)ref);
                break;
            }
            case 5:
            {
                args[i - 1] = (void*)(*(long long*)ref);
                break;
            }
            case 6:
            {
                args[i - 1] = (void*)(*(unsigned char*)ref);
                break;
            }
            case 7:
            {
                args[i - 1] = (void*)(*(unsigned short*)ref);
                break;
            }
            case 8:
            {
                args[i - 1] = (void*)(*(unsigned int*)ref);
                break;
            }
            case 9:
            {
                args[i - 1] = (void*)(*(unsigned long long*)ref);
                break;
            }
            case 10:
            {
                //args[i - 1] = (void*)(*(float*)ref);
                args[i - 1] = (void*)0;
                break;
            }
            case 11:
            {
                //args[i - 1] = (void*)(*(double*)ref);
                args[i - 1] = (void*)0;
				break;
            }
        }*/
    }
    docallVal = DoCallVoid(addr, args);
}
int32_t GetDoCallValueInt()
{
    return (int32_t)(docallVal);
}
int16_t GetDoCallValueInt16()
{
    return (int16_t)(docallVal);
}
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
                logger->Error("An exception occurred:\n%s", ctx->GetExceptionString());
            }
            ctx->Release();
        }
    }
}
bool HasMod(std::string &name)
{
    return aml->HasMod(name.c_str());
}
bool HasModOfVer(std::string &name, std::string &ver)
{
    return aml->HasModOfVersion(name.c_str(), ver.c_str());
}
uintptr_t GetLibraryAddress(std::string &lib)
{
    return aml->GetLib(lib.c_str());
}
uintptr_t GetLibraryHandle(std::string &lib)
{
    return (uintptr_t)dlopen(lib.c_str(), RTLD_LAZY);
}
uintptr_t GetLibraryHandleSymbol(uintptr_t &hndl, std::string &sym)
{
    return aml->GetSym((void*)hndl, sym.c_str());
}
void UnprotectMemory(uintptr_t &mem, uint32_t &count)
{
    aml->Unprot(mem, count);
}
void WriteMemory(uintptr_t &mem, std::string &writeWhat, uint32_t &count)
{
    aml->Write(mem, (uintptr_t)writeWhat.c_str(), count);
}
std::string ReadMemory(uintptr_t &mem, uint32_t &count)
{
    char* newchar = new char[count + 1];
    aml->Read(mem, (uintptr_t)&newchar, count);
    newchar[count] = 0;
    std::string ch = newchar;
    delete[] newchar;
    return ch;
}
void PlaceNOP(uintptr_t &mem, uint32_t &count)
{
    aml->PlaceNOP(mem, count);
}
void Redirect(uintptr_t &from, uintptr_t &to)
{
    aml->Redirect(from, to);
}
void PlaceRET(uintptr_t &mem)
{
    aml->PlaceRET(mem);
}

std::string GetMyName()
{
    asIScriptContext *ctx = asGetActiveContext();
    return ctx->GetFunction(0)->GetModuleName();
}
std::string GetCurrentGame()
{
    aml->GetCurrentGame();
}

// Kinda main thing here
void AddAS4AMLFuncs()
{
    // "Pointers interface"!
    #ifdef AS_64BIT_PTR
    engine->RegisterGlobalFunction("float AML_GetPtrAddrFloat(const uint64 &in)", asFUNCTION(GetPtrAddrFloat), asCALL_STDCALL);
    engine->RegisterGlobalFunction("double AML_GetPtrAddrDouble(const uint64 &in)", asFUNCTION(GetPtrAddrDouble), asCALL_STDCALL);
    engine->RegisterGlobalFunction("bool AML_GetPtrAddrBool(const uint64 &in)", asFUNCTION(GetPtrAddrBool), asCALL_STDCALL);
    engine->RegisterGlobalFunction("int8 AML_GetPtrAddrInt8(const uint64 &in)", asFUNCTION(GetPtrAddrInt8), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint8 AML_GetPtrAddrUInt8(const uint64 &in)", asFUNCTION(GetPtrAddrUInt8), asCALL_STDCALL);
    engine->RegisterGlobalFunction("int16 AML_GetPtrAddrInt16(const uint64 &in)", asFUNCTION(GetPtrAddrInt16), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint16 AML_GetPtrAddrUInt16(const uint64 &in)", asFUNCTION(GetPtrAddrUInt16), asCALL_STDCALL);
    engine->RegisterGlobalFunction("int AML_GetPtrAddrInt(const uint64 &in)", asFUNCTION(GetPtrAddrInt), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint AML_GetPtrAddrUInt(const uint64 &in)", asFUNCTION(GetPtrAddrUInt), asCALL_STDCALL);
    engine->RegisterGlobalFunction("int64 AML_GetPtrAddrInt64(const uint64 &in)", asFUNCTION(GetPtrAddrInt64), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint64 AML_GetPtrAddrUInt64(const uint64 &in)", asFUNCTION(GetPtrAddrUInt64), asCALL_STDCALL);
    
    engine->RegisterGlobalFunction("void AML_SetPtrAddrFloat(const uint64 &in, const float &in)", asFUNCTION(SetPtrAddrFloat), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrDouble(const uint64 &in, const double &in)", asFUNCTION(SetPtrAddrDouble), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrBool(const uint64 &in, const bool &in)", asFUNCTION(SetPtrAddrBool), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrInt8(const uint64 &in, const int8 &in)", asFUNCTION(SetPtrAddrInt8), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrUInt8(const uint64 &in, const uint8 &in)", asFUNCTION(SetPtrAddrUInt8), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrInt16(const uint64 &in, const int16 &in)", asFUNCTION(SetPtrAddrInt16), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrUInt16(const uint64 &in, const uint16 &in)", asFUNCTION(SetPtrAddrUInt16), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrInt(const uint64 &in, const int &in)", asFUNCTION(SetPtrAddrInt), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrUInt(const uint64 &in, const uint &in)", asFUNCTION(SetPtrAddrUInt), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrInt64(const uint64 &in, const int64 &in)", asFUNCTION(SetPtrAddrInt64), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrUInt64(const uint64 &in, const uint64 &in)", asFUNCTION(SetPtrAddrUInt64), asCALL_CDECL);
    #else
    engine->RegisterGlobalFunction("float AML_GetPtrAddrFloat(const uint &in)", asFUNCTION(GetPtrAddrFloat), asCALL_STDCALL);
    engine->RegisterGlobalFunction("double AML_GetPtrAddrDouble(const uint &in)", asFUNCTION(GetPtrAddrDouble), asCALL_STDCALL);
    engine->RegisterGlobalFunction("bool AML_GetPtrAddrBool(const uint &in)", asFUNCTION(GetPtrAddrBool), asCALL_STDCALL);
    engine->RegisterGlobalFunction("int8 AML_GetPtrAddrInt8(const uint &in)", asFUNCTION(GetPtrAddrInt8), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint8 AML_GetPtrAddrUInt8(const uint &in)", asFUNCTION(GetPtrAddrUInt8), asCALL_STDCALL);
    engine->RegisterGlobalFunction("int16 AML_GetPtrAddrInt16(const uint &in)", asFUNCTION(GetPtrAddrInt16), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint16 AML_GetPtrAddrUInt16(const uint &in)", asFUNCTION(GetPtrAddrUInt16), asCALL_STDCALL);
    engine->RegisterGlobalFunction("int AML_GetPtrAddrInt(const uint &in)", asFUNCTION(GetPtrAddrInt), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint AML_GetPtrAddrUInt(const uint &in)", asFUNCTION(GetPtrAddrUInt), asCALL_STDCALL);
    engine->RegisterGlobalFunction("int64 AML_GetPtrAddrInt64(const uint &in)", asFUNCTION(GetPtrAddrInt64), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint64 AML_GetPtrAddrUInt64(const uint &in)", asFUNCTION(GetPtrAddrUInt64), asCALL_STDCALL);
    
    engine->RegisterGlobalFunction("void AML_SetPtrAddrFloat(const uint &in, const float &in)", asFUNCTION(SetPtrAddrFloat), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrDouble(const uint &in, const double &in)", asFUNCTION(SetPtrAddrDouble), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrBool(const uint &in, const bool &in)", asFUNCTION(SetPtrAddrBool), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrInt8(const uint &in, const int8 &in)", asFUNCTION(SetPtrAddrInt8), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrUInt8(const uint &in, const uint8 &in)", asFUNCTION(SetPtrAddrUInt8), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrInt16(const uint &in, const int16 &in)", asFUNCTION(SetPtrAddrInt16), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrUInt16(const uint &in, const uint16 &in)", asFUNCTION(SetPtrAddrUInt16), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrInt(const uint &in, const int &in)", asFUNCTION(SetPtrAddrInt), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrUInt(const uint &in, const uint &in)", asFUNCTION(SetPtrAddrUInt), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrInt64(const uint &in, const int64 &in)", asFUNCTION(SetPtrAddrInt64), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_SetPtrAddrUInt64(const uint &in, const uint64 &in)", asFUNCTION(SetPtrAddrUInt64), asCALL_CDECL);
    #endif // !AS_64BIT_PTR
    
    // A bit of... "Calling interface"! Doesn`t sound that good...
    #ifdef AS_64BIT_PTR
    
    #else
    engine->RegisterGlobalFunction("void AML_DoCall(const uint &in, ?&in var1 = 0, ?&in var2 = 0, ?&in var3 = 0, ?&in var4 = 0, ?&in var5 = 0, ?&in var6 = 0, ?&in var7 = 0, ?&in var8 = 0, ?&in var9 = 0, ?&in var10 = 0, ?&in var11 = 0, ?&in var12 = 0, ?&in var13 = 0, ?&in var14 = 0, ?&in var15 = 0)", asFUNCTION(DoCall), asCALL_GENERIC);
    #endif // !AS_64BIT_PTR
    engine->RegisterGlobalFunction("int AML_GetCallValueInt()", asFUNCTION(GetDoCallValueInt), asCALL_STDCALL);
    engine->RegisterGlobalFunction("int AML_GetCallValueInt16()", asFUNCTION(GetDoCallValueInt16), asCALL_STDCALL);
    engine->RegisterGlobalFunction("void AML_DoScriptsCall(const string &in)", asFUNCTION(DoScriptsCall), asCALL_CDECL);
    
    // AML Interface
    engine->RegisterGlobalFunction("bool AML_HasMod(const string &in)", asFUNCTION(HasMod), asCALL_STDCALL);
    engine->RegisterGlobalFunction("bool AML_HasMod(const string &in, const string &in)", asFUNCTION(HasModOfVer), asCALL_STDCALL);
    #ifdef AS_64BIT_PTR
    engine->RegisterGlobalFunction("uint64 AML_GetLibraryAddress(const string &in)", asFUNCTION(GetLibraryAddress), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint64 AML_GetLibraryHandle(const string &in)", asFUNCTION(GetLibraryHandle), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint64 AML_GetLibraryHandleSymbol(const uint64 &in, const string &in)", asFUNCTION(GetLibraryHandleSymbol), asCALL_STDCALL);
    engine->RegisterGlobalFunction("void AML_UnprotectMemory(const uint64 &in, const uint &in)", asFUNCTION(UnprotectMemory), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_WriteMemory(const uint64 &in, const string &in, const uint &in)", asFUNCTION(WriteMemory), asCALL_CDECL);
    engine->RegisterGlobalFunction("string AML_ReadMemory(const uint64 &in, const string &in, const uint &in)", asFUNCTION(ReadMemory), asCALL_STDCALL);
    engine->RegisterGlobalFunction("void AML_PlaceNOP(const uint64 &in, const uint &in)", asFUNCTION(PlaceNOP), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_Redirect(const uint64 &in, const uint64 &in)", asFUNCTION(Redirect), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_PlaceRET(const uint64 &in)", asFUNCTION(PlaceRET), asCALL_CDECL);
    #else
    engine->RegisterGlobalFunction("uint AML_GetLibraryAddress(const string &in)", asFUNCTION(GetLibraryAddress), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint AML_GetLibraryHandle(const string &in)", asFUNCTION(GetLibraryHandle), asCALL_STDCALL);
    engine->RegisterGlobalFunction("uint AML_GetLibraryHandleSymbol(const uint &in, const string &in)", asFUNCTION(GetLibraryHandleSymbol), asCALL_STDCALL);
    engine->RegisterGlobalFunction("void AML_UnprotectMemory(const uint &in, const uint &in)", asFUNCTION(UnprotectMemory), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_WriteMemory(const uint &in, const string &in, const uint &in)", asFUNCTION(WriteMemory), asCALL_CDECL);
    engine->RegisterGlobalFunction("string AML_ReadMemory(const uint &in, const string &in, const uint &in)", asFUNCTION(ReadMemory), asCALL_STDCALL);
    engine->RegisterGlobalFunction("void AML_PlaceNOP(const uint &in, const uint &in)", asFUNCTION(PlaceNOP), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_Redirect(const uint &in, const uint &in)", asFUNCTION(Redirect), asCALL_CDECL);
    engine->RegisterGlobalFunction("void AML_PlaceRET(const uint &in)", asFUNCTION(PlaceRET), asCALL_CDECL);
    #endif // !AS_64BIT_PTR
    engine->RegisterGlobalFunction("string AML_GetMyName()", asFUNCTION(GetMyName), asCALL_STDCALL);
    engine->RegisterGlobalFunction("string AML_GetCurrentGame()", asFUNCTION(GetCurrentGame), asCALL_STDCALL);
    
}
