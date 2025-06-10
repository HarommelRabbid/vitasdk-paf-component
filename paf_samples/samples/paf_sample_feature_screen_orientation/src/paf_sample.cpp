
#ifdef __SNC__
#include <kernel.h>
#else
#include <psp2/kernel/clib.h>
#endif

#include <paf.h>


paf::Framework *g_fw;
paf::ui::Button *g_bt;
int32_t g_oldOrientation = OrientationType_Standard;
void *g_rootPage;

/*
* UpdateOrientation() needs to be called in regular intervals for automatic screen orientation
* Here we call to on every frame from the MainThreadCallList task
*/
void orientationUpdateTask(void *userdata) {
	paf::ui::Environment *env = paf::Framework::Instance()->GetEnvironmentInstance();
	env->UpdateOrientation();
	/*
	* Following code changes button text with the current orientation type
	*/
	int32_t orientation = env->GetOrientation();
	if (g_oldOrientation != orientation)
	{
		paf::wstring orientationString;
		switch (orientation)
		{
		case OrientationType_Standard:
			orientationString = L"OrientationType_Standard";
			break;
		case OrientationType_Upsidedown:
			orientationString = L"OrientationType_Upsidedown";
			break;
		case OrientationType_Rightsidedown:
			orientationString = L"OrientationType_Rightsidedown";
			break;
		case OrientationType_Leftsidedown:
			orientationString = L"OrientationType_Leftsidedown";
			break;
		}
		g_bt->SetString(orientationString);
		g_oldOrientation = orientation;
	}
}

void loadPluginCB(paf::Plugin *plugin) {
	paf::Plugin::PageOpenParam pageOpenParam;
	pageOpenParam.option = paf::Plugin::PageOption_None;

	paf::ui::Scene *pScene = plugin->PageOpen("page_main", pageOpenParam);

	g_bt = (paf::ui::Button *)pScene->FindChild("_sample_widget_button");

	g_rootPage = pScene;

	paf::common::MainThreadCallList::Register(orientationUpdateTask, NULL);
}

int paf_sample_main(void) {
	paf::Framework::InitParam fwParam;
	fwParam.mode = paf::Framework::Mode_Normal;
	/*
	* screen_orientation_mode option only affects initial screen orientation after the application start
	*/
	fwParam.screen_orientation_mode = OrientationType_Standard;
	/*
	* Set allowed screen orientations in EnvironmentParam option
	*/
	fwParam.env_param = new paf::ui::EnvironmentParam(
		paf::ui::EnvironmentParam::RESOLUTION_PSP2 |
		paf::ui::EnvironmentParam::ORIENTATION_LANDSCAPE_STANDARD |
		paf::ui::EnvironmentParam::ORIENTATION_LANDSCAPE_UPSIDEDOWN |
		paf::ui::EnvironmentParam::ORIENTATION_PORTRAIT_RIGHTSIDEDONW |
		paf::ui::EnvironmentParam::ORIENTATION_PORTRAIT_LEFTSIDEDOWN);

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


	sceClibPrintf("[SAMPLE] Failed to run PAF instance\n");

	exit(0);

	return 0;
}