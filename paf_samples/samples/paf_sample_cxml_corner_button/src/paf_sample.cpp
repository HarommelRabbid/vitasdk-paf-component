
#ifdef __SNC__
#include <kernel.h>
#else
#include <psp2/kernel/clib.h>
#endif

#include <paf.h>


paf::Framework *g_fw;
void *g_rootPage;


static void button_handler(int32_t type, paf::ui::Handler *self, paf::ui::Event *e, void *userdata){
	sceClibPrintf("Button pressed!\n");
}

void loadPluginCB(paf::Plugin *plugin){

	{
		paf::Plugin::PageOpenParam pageOpenParam;
		pageOpenParam.option = paf::Plugin::PageOption_None;

		paf::ui::Scene *pScene = plugin->PageOpen("page_main", pageOpenParam);

		g_rootPage = pScene;
	}
}

int paf_sample_main(void){

	{
		paf::Framework::InitParam fwParam;
		fwParam.mode = paf::Framework::Mode_Normal;

		paf::Framework *paf_fw = new paf::Framework(fwParam);
		if(paf_fw != NULL){
			g_fw = paf_fw;

			paf_fw->LoadCommonResourceSync();

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
		}
	}

	sceClibPrintf("[SAMPLE] Failed to run PAF instance\n");

	exit(0);

	return 0;
}
