
#ifdef __SNC__
#include <kernel.h>
#include <libsysmodule.h>
#include <net.h>
#include <libnetctl.h>
#else
#include <psp2/kernel/clib.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/net/http.h>
#include <psp2/libssl.h>
#include <psp2/sysmodule.h>
#endif

#include <paf.h>


paf::Framework *g_fw;
void *g_rootPage;


void loadTexture(const char *path, paf::ui::Plane *targetPlane) {
	int32_t res = SCE_PAF_OK;
	paf::common::SharedPtr<paf::LocalFile> lfile;
	paf::common::SharedPtr<paf::HttpFile> hfile;
	paf::common::SharedPtr<paf::File> file;

	lfile = paf::LocalFile::Open(path, SCE_O_RDONLY, 0, &res);
	if (res != SCE_PAF_OK)
	{
		sceClibPrintf("[SAMPLE] %s not a local file?..\n", path);

		hfile = paf::HttpFile::Open(path, SCE_O_RDONLY, 0, &res);
		if (res != SCE_PAF_OK)
		{
			sceClibPrintf("[SAMPLE] Failed to open image file\n");
			return;
		}
		else{
			file = (paf::common::SharedPtr<paf::File>&)hfile;
		}
	}
	else{
		file = (paf::common::SharedPtr<paf::File>&)lfile;
	}


	paf::intrusive_ptr<paf::graph::Surface> tex = paf::graph::Surface::Load(paf::gutil::GetDefaultSurfacePool(), file, NULL);
	if (!tex.get())
	{
		sceClibPrintf("[SAMPLE] Failed to load image surface\n");
		return;
	}

	// We can close/dispose of the file at this point if we need more free memory...
	file->Close();
	file.reset();

	targetPlane->SetTexture(tex);

	//We can play with some PlaneObj properties just for an example
	paf::graph::PlaneObj *pPlaneObj = (paf::graph::PlaneObj *)targetPlane->GetDrawObj(paf::ui::Plane::OBJ_PLANE);
	pPlaneObj->SetScaleMode(paf::graph::PlaneObj::SCALE_SIZE, paf::graph::PlaneObj::SCALE_SIZE);
}

void loadPluginCB(paf::Plugin *plugin){
	paf::Plugin::PageOpenParam pageOpenParam;
	pageOpenParam.option = paf::Plugin::PageOption_None;

	paf::ui::Scene *pScene = plugin->PageOpen("page_main", pageOpenParam);

	g_rootPage = pScene;

	loadTexture("http://www.google.com/images/branding/googlelogo/1x/googlelogo_light_color_272x92dp.png", (paf::ui::Plane *)pScene->FindChild("plane_sample_white_1"));
	loadTexture("vs0:data/internal/theme/defaultTheme_homeScreen.png", (paf::ui::Plane *)pScene->FindChild("plane_sample_white_2"));
}

void initNet(bool useHttps){
	int32_t ret = 0;

	sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
	sceSysmoduleLoadModule(SCE_SYSMODULE_HTTPS);

	SceNetInitParam param;
	param.memory = sce_paf_malloc(256 * 1024);
	param.size = 256 * 1024;
	param.flags = 0;
	sceNetInit(&param);
	sceNetCtlInit();
	sceHttpInit(256 * 1024);

	if (useHttps){
		sceSysmoduleLoadModule(SCE_SYSMODULE_SSL);
		sceSslInit(256 * 1024);
	}
}

int paf_sample_main(void){
	paf::Framework::InitParam fwParam;
	fwParam.mode = paf::Framework::Mode_Normal;

	paf::Framework *paf_fw = new paf::Framework(fwParam);
	if (paf_fw != NULL) {
		g_fw = paf_fw;

		initNet(false);

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
