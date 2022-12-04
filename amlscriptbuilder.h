#include <scriptbuilder/scriptbuilder.h>
#include <angelscript.h>
#include <ias4aml.h>

class AMLScriptBuilder : public CScriptBuilder
{
public:
    AMLScriptBuilder() : CScriptBuilder() {}
    asIScriptModule* BuildAMLModule();
    void Scripts_GameStart();
    void Scripts_GamePause();
    void Scripts_GameResume();
    void Scripts_GameStop();
    void Scripts_Update();
};

extern AMLScriptBuilder* builder;
extern IAS4AML *as4aml;
