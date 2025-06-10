
#ifdef __SNC__
#include <kernel.h>
#else
#include <psp2/kernel/clib.h>
#endif

#include <paf.h>


paf::Plugin *g_samplePlugin;
paf::Framework *g_fw;


void loadPluginCB(paf::Plugin *plugin){

	g_samplePlugin = plugin;

	paf::Plugin::PageOpenParam pageOpenParam;
	plugin->PageOpen("page_main", pageOpenParam);
}

int paf_sample_main(void){

	paf::Framework::InitParam fwParam;
	fwParam.mode = paf::Framework::Mode_Normal;

	paf::Framework *paf_fw = new paf::Framework(fwParam);
	if (paf_fw != NULL) {
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

	exit(0);

	return 0;
}
