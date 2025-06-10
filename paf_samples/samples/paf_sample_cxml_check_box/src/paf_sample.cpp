
#include <paf.h>


paf::Framework *g_fw;
paf::ui::Scene *g_rootPage;

void OnPressCB(int eventID, paf::ui::Handler *caller, paf::ui::Event *evt, void *pUserData) {
    auto checkBox = (paf::ui::CheckBox *)caller;
    auto text = (paf::ui::Text *)pUserData; // set while calling AddEventCallback

    text->SetString(checkBox->IsChecked() ? L"ON" : L"OFF");
}

void loadPluginCB(paf::Plugin *plugin) {
    paf::Plugin::PageOpenParam pageOpenParam;
    pageOpenParam.option = paf::Plugin::PageOption_None;

    paf::ui::Scene *pScene = plugin->PageOpen("page_main", pageOpenParam);

    g_rootPage = pScene; 

    auto text = (paf::ui::Text *)pScene->FindChild("test_strings_id");
    auto checkBox = (paf::ui::CheckBox *)pScene->FindChild("sample_check_box");
    
    checkBox->AddEventCallback(paf::ui::CheckBox::CB_BTN_DECIDE, (paf::ui::HandlerCB)OnPressCB, text);

    text->SetString(L"OFF");
}

int paf_sample_main(void) {
    paf::Framework::InitParam fwParam;
    fwParam.mode = paf::Framework::Mode_Normal;

    paf::Framework *paf_fw = new paf::Framework(fwParam);
    if (paf_fw != NULL) {
        g_fw = paf_fw;

        paf_fw->LoadCommonResourceSync();

        paf::Plugin::InitParam pluginParam;

        pluginParam.name = "sample_plugin";
        pluginParam.caller_name = "__main__";
        pluginParam.resource_file = "app0:/sample_plugin.rco";
        pluginParam.init_func = NULL;
        pluginParam.start_func = loadPluginCB;
        pluginParam.stop_func = NULL;
        pluginParam.exit_func = NULL;

        paf::Plugin::LoadSync(pluginParam);
        paf_fw->Run();
    }

    exit(0);

    return 0;
}
