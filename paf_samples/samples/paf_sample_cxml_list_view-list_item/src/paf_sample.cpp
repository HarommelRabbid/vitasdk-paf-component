
/*
 * TODO: make physical keys operation.
 */

#ifdef __SNC__
#include <kernel.h>
#else
#include <psp2/kernel/clib.h>
#endif

#include <paf.h>


paf::Framework *g_fw;
paf::Plugin *g_samplePlugin;
void *g_rootPage;

class my_ItemFactory : public paf::ui::listview::ItemFactory {
public:
	my_ItemFactory(){
	}

	~my_ItemFactory(){
	}

	paf::ui::ListItem *Create(CreateParam& param);

	void Start(StartParam& param){
		param.list_item->Show(paf::common::transition::Type_FadeinSlow);
	}

	void Stop(StopParam& param){
		param.list_item->Hide(paf::common::transition::Type_FadeinSlow);
	}

	// void Dispose(DisposeParam& param){}
};

paf::ui::ListItem *my_ItemFactory::Create(CreateParam& param){

	paf::ui::ListView *list_view = param.list_view;

	paf::Plugin::TemplateOpenParam openParam;
	int res = g_samplePlugin->TemplateOpen(param.parent, "_sample_template_list_item", openParam);
	if(res != 0){
		sceClibPrintf("TemplateOpen 0x%X\n", res);
	}

	paf::ui::ListItem *list_item = (paf::ui::ListItem *)param.parent->GetChild(param.parent->GetChildrenNum() - 1);
	paf::ui::Widget *button = list_item->FindChild("button");

	switch(param.cell_index){
	case 0:
		button->SetString(L"First cell");
		break;
	case 1:
		button->SetString(L"Test cell");
		break;
	case 2:
		button->SetString(L"My cell");
		break;
	case 3:
		button->SetString(L"Middle cell");
		break;
	case 4:
		button->SetString(L"Final cell");
		break;
	case 5:
		button->SetString(L"Last cell");
		break;
	default:
		button->SetString(L"Extra cell");
		break;
	}

	return list_item;
}

void loadPluginCB(paf::Plugin *plugin){

	g_samplePlugin = plugin;

	{
		paf::Plugin::PageOpenParam pageOpenParam;
		pageOpenParam.option = paf::Plugin::PageOption_None;

		paf::ui::Scene *pScene = plugin->PageOpen("page_main", pageOpenParam);

		g_rootPage = pScene;

		paf::ui::Widget *pPlane = pScene->FindChild("plane_sample_black");
		paf::ui::ListView *list_view = (paf::ui::ListView *)pPlane->FindChild("good_list_view");
		list_view->SetItemFactory(new my_ItemFactory());
		list_view->InsertSegment(0, 1);
		list_view->SetCellSizeDefault(0, { 240.0f, 70.0f, 0.0f, 0.0f } );
		list_view->SetSegmentLayoutType(0, paf::ui::ListView::LAYOUT_TYPE_LIST);
		list_view->InsertCell(0, 0, 8);

		list_view = (paf::ui::ListView *)pPlane->FindChild("bad_list_view_1");
		list_view->SetItemFactory(new my_ItemFactory());
		list_view->InsertSegment(0, 1);
		list_view->SetCellSizeDefault(0, { 240.0f, 70.0f, 0.0f, 0.0f } );
		list_view->SetSegmentLayoutType(0, paf::ui::ListView::LAYOUT_TYPE_LIST);
		list_view->InsertCell(0, 0, 8);

		list_view = (paf::ui::ListView *)pPlane->FindChild("bad_list_view_2");
		list_view->SetItemFactory(new my_ItemFactory());
		list_view->InsertSegment(0, 1);
		list_view->SetCellSizeDefault(0, { 240.0f, 70.0f, 0.0f, 0.0f } );
		list_view->SetSegmentLayoutType(0, paf::ui::ListView::LAYOUT_TYPE_LIST);
		list_view->InsertCell(0, 0, 8);
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
