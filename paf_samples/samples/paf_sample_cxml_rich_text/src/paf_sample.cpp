
#include <paf.h>


paf::Framework *g_fw;
void *g_rootPage;


void loadPluginCB(paf::Plugin *plugin){

	{
		paf::Plugin::PageOpenParam pageOpenParam;
		pageOpenParam.option = paf::Plugin::PageOption_None;

		paf::ui::Scene *pScene = plugin->PageOpen(paf::IDParam("page_main"), pageOpenParam);

		g_rootPage = pScene;

		paf::ui::Widget *pPlane = pScene->FindChild(paf::IDParam("plane_sample_black"));
		paf::ui::RichText *pRichText = (paf::ui::RichText *)pPlane->FindChild(paf::IDParam("_sample_widget_rich_text"));

		static const char rich_text[] = "<h1>This is <font color=\"#ff0000\">Rich</font><font color=\"#00ff00\">Text</font><br/><br/><br/><br/><br/><br/><br/><br/>Last line text</h1>";

		pRichText->SetCText(rich_text, sce_paf_strlen(rich_text));
	}
}

int paf_sample_main(void){

	paf::Framework::InitParam fwParam;
	fwParam.mode = paf::Framework::Mode_Normal;

	paf::Framework *paf_fw = new paf::Framework(fwParam);
	if(paf_fw == NULL){
		return -1;
	}

	g_fw = paf_fw;

	paf_fw->LoadCommonResourceAsync();

	paf::Plugin::InitParam pluginParam;

	pluginParam.name          = "sample_plugin";
	pluginParam.caller_name   = "__main__";
	pluginParam.resource_file = "app0:/sample_plugin.rco";
	pluginParam.init_func     = NULL;
	pluginParam.start_func    = loadPluginCB;
	pluginParam.stop_func     = NULL;
	pluginParam.exit_func     = NULL;

	paf::Plugin::LoadSync(pluginParam);
	paf_fw->Run();

	return 0;
}
