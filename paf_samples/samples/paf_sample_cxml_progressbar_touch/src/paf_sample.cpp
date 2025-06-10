
#ifdef __SNC__
#include <kernel.h>
#else
#include <psp2/kernel/clib.h>
#endif

#include <paf.h>


paf::Framework *g_fw;
void *g_rootPage;


static void button_handler(int32_t type, paf::ui::Handler *self, paf::ui::Event *e, void *userdata){
	paf::ui::ProgressBarTouch *pProgressBar = (paf::ui::ProgressBarTouch *)self;
	if (type == paf::ui::ProgressBarTouch::CB_PROGRESSBAR_DRAG_END){
		sceClibPrintf("Progress bar interaction end!\n");
		sceClibPrintf("Progress bar value: %f\n", pProgressBar->GetValue());
	}
	else if (type == paf::ui::Handler::CB_TOUCH_MOVE || type == paf::ui::Handler::CB_TOUCH_OUT){
		sceClibPrintf("Progress bar interaction begin!\n");
	}
}

void loadPluginCB(paf::Plugin *plugin){

	{
		paf::Plugin::PageOpenParam pageOpenParam;
		pageOpenParam.option = paf::Plugin::PageOption_None;

		paf::ui::Scene *pScene = plugin->PageOpen("page_main", pageOpenParam);

		paf::ui::Widget *pPlane = pScene->FindChild("plane_sample_black");

		paf::ui::Widget *pProgressBar = pPlane->FindChild("_sample_widget_progressbar");

		pProgressBar->SetEventCallback(paf::ui::ProgressBarTouch::CB_PROGRESSBAR_DRAG_END, button_handler);
		pProgressBar->SetEventCallback(paf::ui::Handler::CB_TOUCH_MOVE, button_handler);
		pProgressBar->SetEventCallback(paf::ui::Handler::CB_TOUCH_OUT, button_handler);

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
