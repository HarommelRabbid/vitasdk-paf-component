
#ifdef __SNC__
#include <kernel.h>
#include <libsysmodule.h>
#include <appmgr.h>
#else
#include <psp2/kernel/clib.h>
#include <psp2/sysmodule.h>
#include <psp2/appmgr.h>

#ifndef SCE_UID_INVALID_UID
#define SCE_UID_INVALID_UID (-1)
#endif

extern "C" {

typedef struct _SceAppMgrAppStatus { // size is 0x80
	SceUInt32 unk_0;                  // 0x0
	SceUInt32 launchMode;             // 0x4
	SceUInt32 bgm_priority_or_status; // 0x8
	char appName[32];                 // 0xC
	SceUInt32 unk_2C;                 // 0x2C
	SceUID appId;                     // 0x30 - Application ID
	SceUID processId;                 // 0x34 - Process ID
	SceUInt32 status_related_1;       // 0x38
	SceUInt32 status_related_2;       // 0x3C
	SceUInt8 padding[0x40];
} SceAppMgrAppStatus;

#define SCE_APPMGR_APP_ID_THIS (~1)
#define SCE_APPMGR_APP_ID_GAME (~2)

int sceAppMgrGetStatusByAppId(int appId, SceAppMgrAppStatus *appStatus);
int sceAppMgrGetStatusByName(const char *titleid, SceAppMgrAppStatus *appStatus);

typedef struct SceAppMgrLaunchAppOptParam {
	SceSize size; //0x40
	int launchFlags; //seen: 0x20000(NPXS10016 signin), 0x820000(NPXS10002), 0x100000, 0x30000(videostreaming:), 0x40000(prevents using someCB), 0
	int unk_08; //?
	SceUID appLaunchCB; //-1 if not used
	SceUID getParamCB; //-1 if not used
	SceUID appCloseCB; //-1 if not used
	int unk_18; //seen: 0, 2, 3
	int unk_1C;
	char reserved[0x20];
} SceAppMgrLaunchAppOptParam;

};

#endif

#include <paf.h>
#include <paf_web_map_view.h>
#include <paf_web_ui.h>

//#define USE_HIGHMEM_WEBKIT

#ifdef USE_HIGHMEM_WEBKIT
#define WEBKIT_TITLEID "NPXS10083"
#else
#define WEBKIT_TITLEID "NPXS10084"
#endif


paf::Framework *g_fw;
paf::View *g_view;
paf::ui::Button *g_buttonBack;
paf::ui::Button *g_buttonForward;
paf::ui::Text *g_title;
paf::ui::TextBox *g_url;
paf::ui::ProgressBar *g_progressbar;
void *g_rootPage;

void initWebViewGraphics(){
	paf::webview::Context::Create("sample", 0x900000, SCE_KERNEL_MEMBLOCK_TYPE_USER_MAIN_NC_RW, 0x100, 0x100);

	paf::webview::Context *ctx = paf::webview::Context::Get();
	ctx->InitGraphics();
	ctx->InitIPC();

	paf::webview::Context::GraphicsParam param;
	param.gxm_ctx = paf::graph::ScreenSurface::GetRawContext();
	param.viewport_width = 960;
	param.viewport_height = 544;
	paf::webview::Context::SetGraphicsParam(&param);
}

class my_WebViewListener : public paf::ui::IWebViewListener
{
public:

	static int32_t GetAppId(SceUID *pAppId, const char *titleid){
		int32_t ret;
		SceAppMgrAppStatus status;
		sce_paf_memset(&status, 0, sizeof(SceAppMgrAppStatus));
		ret = sceAppMgrGetStatusByName(titleid, &status);
		*pAppId = status.appId;
		return ret;
	}

	static int32_t GetTitleId(char *titleid, int32_t maxlen, SceUID pAppId){
		int32_t ret;
		SceAppMgrAppStatus status;
		sce_paf_memset(&status, 0, sizeof(SceAppMgrAppStatus));
		ret = sceAppMgrGetStatusByAppId(pAppId, &status);
		sce_paf_strncpy(titleid, status.appName, maxlen);
		return ret;
	}

	static paf::string BuildLaunchArg(){
		paf::string launchArg;
		char tmp[0x42];

		SceAppMgrAppStatus appStatus;
		sce_paf_memset(&appStatus, 0, sizeof(SceAppMgrAppStatus));
		sceAppMgrGetStatusByAppId(SCE_APPMGR_APP_ID_THIS, &appStatus);

		launchArg = "Parent_app_id:";
		sce_paf_snprintf(tmp, 0x40, "%lld,", (int64_t)appStatus.appId);
		launchArg += tmp;

		launchArg += "CONNECTION_POSTFIX:";
		sce_paf_snprintf(tmp, 0x40, "%08x,", appStatus.processId);
		launchArg += tmp;

		launchArg += "MANX_INSPECTOR_SERVER_PORT:0";

		sce_paf_memset(&appStatus, 0, sizeof(SceAppMgrAppStatus));
		if (sceAppMgrGetStatusByAppId(SCE_APPMGR_APP_ID_GAME, &appStatus) == SCE_OK && appStatus.appId != SCE_UID_INVALID_UID){
			launchArg += ",Game_app_id:";
			GetTitleId(tmp, 0x10, appStatus.appId);
			launchArg += tmp;
		}

		return launchArg;
	}

	static SceInt32 SceWebCoreProcessWatcher(SceUID notifyId, SceInt32 notifyCount, SceInt32 notifyArg, void *pCommon){
		my_WebViewListener *listener = static_cast<my_WebViewListener *>(pCommon);
		listener->OnFinishCreateProcess();
		return 1;
	}

	my_WebViewListener(){
		m_webkitAppId = SCE_UID_INVALID_UID;
		m_inProgress = false;
	}

	virtual ~my_WebViewListener(){

	}

	virtual int32_t Initialize(paf::ui::WebView *webview){
		int32_t ret = SCE_PAF_OK;
		SceUID webkitAppId = SCE_UID_INVALID_UID;

		sceClibPrintf("%s()\n", __func__);

		if (webview == NULL){
			sceClibPrintf("%s(): bad webview ptr\n", __func__);
			return 0x80AF008F;
		}

		if (m_inProgress){
			return ret;
		}

		m_inProgress = true;

		//First, kill all other unbound webkit process instances if they exist

		sceClibPrintf("step 0\n");

		ret = GetAppId(&webkitAppId, "NPXS10083");
		if (ret == SCE_OK && 0 < webkitAppId){
			sceClibPrintf("step 05\n");
			if (m_webkitAppId == webkitAppId){
				//Our webkit process already exists, all good!
				return ret;
			}
			sceClibPrintf("step 06\n");
			sceAppMgrDestroyAppByAppId(webkitAppId);
			while (GetAppId(&webkitAppId, "NPXS10083") != 0x80802012){
				paf::thread::Sleep(100);
			}
		}

		sceClibPrintf("step 1\n");

		ret = GetAppId(&webkitAppId, "NPXS10084");
		if (ret == SCE_OK && 0 < webkitAppId){
			sceClibPrintf("step 15\n");
			if (m_webkitAppId == webkitAppId){
				//Our webkit process already exists, all good!
				return ret;
			}
			sceClibPrintf("step 16\n");
			sceAppMgrDestroyAppByAppId(webkitAppId);
			while (GetAppId(&webkitAppId, "NPXS10084") != 0x80802012){
				paf::thread::Sleep(100);
			}
		}

		sceClibPrintf("step 2\n");

		//Now launch our own bound webkit process

		SceAppMgrLaunchAppOptParam param;
		sce_paf_memset(&param, 0, sizeof(SceAppMgrLaunchAppOptParam));
		param.size = sizeof(SceAppMgrLaunchAppOptParam);
		param.launchFlags = 0x20000;
		param.appLaunchCB = sceKernelCreateCallback("SceWebCoreProcessWatcher", 0, SceWebCoreProcessWatcher, this);

		paf::string launchArg = BuildLaunchArg();

		return sceAppMgrLaunchAppByName2(WEBKIT_TITLEID, launchArg.c_str(), &param);
	}

	virtual int32_t Term(paf::ui::WebView *webview){
		sceClibPrintf("%s()\n", __func__);
		return 0;
	}

	void OnFinishCreateProcess(){
		int32_t ret = SCE_PAF_OK;
		SceUID webkitAppId = SCE_UID_INVALID_UID;

		m_inProgress = false;

		//Check if webkit actually launched successfully

		ret = GetAppId(&webkitAppId, WEBKIT_TITLEID);
		if (ret == SCE_OK && 0 < webkitAppId){
			m_webkitAppId = webkitAppId;
		}
		else{
			//If not, loop to Initialize() again
			Initialize(NULL);
			return;
		}
	}

private:

	SceUID m_webkitAppId;
	bool m_inProgress;
};

class my_WebViewDelegate : public paf::ui::WebViewDelegate
{
public:

	my_WebViewDelegate(){

	}

	virtual ~my_WebViewDelegate(){

	}

	virtual void OnStartLoad(paf::ui::WebView *view){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void OnFinishLoad(paf::ui::WebView *view, int32_t status){
		sceClibPrintf(__func__);

		sceClibPrintf(" Status: %d\n", status);
	}
	virtual void OnReceivedError(paf::ui::WebView *webview, int32_t error_code, paf::common::String const& error_msg){
		sceClibPrintf("%s: error_code=0x%X\n", __func__, error_code);
	}
	virtual void OnStartLoadWithRequest(paf::ui::WebView *webview){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void OnInitializedBrowser(paf::ui::WebView *webview){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void OnProgressChanged(paf::ui::WebView *view, int32_t progress){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");

		g_progressbar->SetValue(progress);
	}
	virtual void OnTitleChanged(paf::ui::WebView *webview, paf::common::String const& title){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");

		g_title->SetString(title.GetWString());
	}
	virtual void OnUrlChanged(paf::ui::WebView *webview, paf::Url const& url){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");

		g_url->SetString(url.GetUrlWString());
	}
	virtual void unk_fun_10(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_11(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_12(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_13(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_14(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_15(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_16(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_17(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_18(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_19(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void OnDownloadStart(int32_t unk, paf::Url const& url, paf::common::String const& content_type){
		sceClibPrintf(__func__);
	}
	virtual void unk_fun_21(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_22(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_23(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void historyStackStatusChanged(paf::ui::WebView *webview, bool canBack, bool canForward){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");

		if (canBack){
			g_buttonBack->SetShowAlpha(1.0f);
		}
		else{
			g_buttonBack->SetShowAlpha(0.3f);
		}

		if (canForward){
			g_buttonForward->SetShowAlpha(1.0f);
		}
		else{
			g_buttonForward->SetShowAlpha(0.3f);
		}

	}
	virtual void unk_fun_25(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_26(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
	virtual void unk_fun_27(){
		sceClibPrintf(__func__);
		sceClibPrintf("\n");
	}
};

class my_ViewController : public paf::ViewController
{
public:

	virtual ~my_ViewController(){

	}

	virtual void PluginOnStart(paf::Plugin *plugin){
		sceClibPrintf("%s()\n", __func__);

		paf::Plugin::PageOpenParam pageOpenParam;
		pageOpenParam.option = paf::Plugin::PageOption_None;

		paf::ui::Scene *pScene = plugin->PageOpen("page_main", pageOpenParam);

		g_rootPage = pScene;

		paf::ui::Widget *pPlane = pScene->FindChild("plane_sample_black");

		paf::ui::WebView *pWebView = (paf::ui::WebView *)pPlane->FindChild("_sample_widget_webview");

		g_buttonBack = (paf::ui::Button *)pPlane->FindChild("_sample_widget_webview_back");
		g_buttonForward = (paf::ui::Button *)pPlane->FindChild("_sample_widget_webview_forward");
		g_title = (paf::ui::Text *)pPlane->FindChild("_sample_widget_webview_title");
		g_url = (paf::ui::TextBox *)pPlane->FindChild("_sample_widget_webview_url");
		g_progressbar = (paf::ui::ProgressBar *)pPlane->FindChild("_sample_widget_webview_progressbar");

		pPlane->FindChild("_sample_widget_webview_reload")->SetString(L"[]");
		g_buttonBack->SetString(L"<");
		g_buttonForward->SetString(L">");
		g_buttonBack->SetShowAlpha(0.3f);
		g_buttonForward->SetShowAlpha(0.3f);

		g_view->BindWidget(plugin, pWebView);

		pWebView->SetDelegate(new my_WebViewDelegate());
		pWebView->EnableJavaScript(true);
		pWebView->EnableCookie(true);

		paf::Url url("http://google.com");
		pWebView->LoadUrl(url);

		/*
		pWebView->SetCursorParam(NULL, NULL, NULL, NULL);
		pWebView->ScePafWebMapView_48BF783B(0);
		*/
	}
};

int paf_sample_main(void){

	char vpath[0x124];

	sceAppMgrConvertVs0UserDrivePath("vs0:/sys/external/libpaf_web_map_view.suprx", vpath, sizeof(vpath));

	new paf::Module(vpath);

	my_WebViewListener *webViewListener = new my_WebViewListener();

	{
		paf::Framework::InitParam fwParam;
		fwParam.mode = paf::Framework::Mode_Application;
		fwParam.webview_listener = webViewListener;
		fwParam.surface_pool_size = 0x500000;
		fwParam.text_surface_pool_size = 0x80000;

		paf::Framework *paf_fw = new paf::Framework(fwParam);
		if(paf_fw != NULL){

			paf::webview::Context::Initialize();

			g_fw = paf_fw;

			paf_fw->LoadCommonResourceSync();

			sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_INI_FILE_PROCESSOR);

			initWebViewGraphics();

			my_ViewController *controller = new my_ViewController();
			g_view = new paf::View("sample_plugin", "app0:/sample_plugin.rco", controller);
			paf_fw->Run();
		}
	}

	sceClibPrintf("[SAMPLE] Failed to run PAF instance\n");

	exit(0);

	return 0;
}