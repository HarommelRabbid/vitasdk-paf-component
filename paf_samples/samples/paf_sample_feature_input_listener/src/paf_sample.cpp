
#ifdef __SNC__
#include <kernel.h>
#else
#include <psp2/kernel/clib.h>
#endif

#include <paf.h>


paf::Framework *g_fw;
paf::ui::Text *g_text_pad;
paf::ui::Text *g_text_touch_front;
paf::ui::Text *g_text_touch_back;
paf::ui::Text *g_text_motion;
paf::ui::Text *g_text_keyboard;
void *g_rootPage;


class my_InputListener : public paf::inputdevice::InputListener
{
public:

	my_InputListener(int32_t type) : InputListener(type){

	}

	~my_InputListener(){

	}

	void OnUpdate(paf::inputdevice::Data *data)
	{
		paf::string stringdata;

		stringdata = paf::common::FormatString(
			"DEVICE_TYPE_PAD:\npaddata: 0x%08X\nlx: %u ly: %u\nrx: %u ry: %u",
			data->m_pad_data->paddata,
			data->m_pad_data->lx,
			data->m_pad_data->ly,
			data->m_pad_data->rx,
			data->m_pad_data->ry);
		g_text_pad->SetString(paf::common::string_util::ToWString(stringdata));
		stringdata = paf::common::FormatString(
			"\n\n\n\n\nDEVICE_TYPE_TOUCHSCREEN_FRONT:\nforce: %d\nstate: %d\nx: %u y: %u\ncenter_orig_x: %d center_orig_y: %d",
			data->m_touch_data[0].force,
			data->m_touch_data[0].state,
			data->m_touch_data[0].x,
			data->m_touch_data[0].y,
			data->m_touch_data[0].center_orig_x,
			data->m_touch_data[0].center_orig_y);
		g_text_touch_front->SetString(paf::common::string_util::ToWString(stringdata));
		stringdata = paf::common::FormatString(
			"\n\n\n\n\n\n\n\n\n\n\nDEVICE_TYPE_TOUCHSCREEN_BACK:\nforce: %d\nstate: %d\nx: %u y: %u\ncenter_orig_x: %d center_orig_y: %d",
			data->m_touch_rear_data[0].force,
			data->m_touch_rear_data[0].state,
			data->m_touch_rear_data[0].x,
			data->m_touch_rear_data[0].y,
			data->m_touch_rear_data[0].center_orig_x,
			data->m_touch_rear_data[0].center_orig_y);
		g_text_touch_back->SetString(paf::common::string_util::ToWString(stringdata));
		stringdata = paf::common::FormatString(
			"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nDEVICE_TYPE_MOTIONSENSOR:\nbasic_orientation x: %d, y: %d, z: %d",
			(int32_t)data->m_motion_data->basic_orientation.extract_x(),
			(int32_t)data->m_motion_data->basic_orientation.extract_y(),
			(int32_t)data->m_motion_data->basic_orientation.extract_z());
		g_text_motion->SetString(paf::common::string_util::ToWString(stringdata));

		stringdata = paf::common::FormatString(
			"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nDEVICE_TYPE_KEYBOARD:\nkeycode: %u",
			data->m_keyboard_data->keycode);
		g_text_keyboard->SetString(paf::common::string_util::ToWString(stringdata));
	}
};

void loadPluginCB(paf::Plugin *plugin) {
	paf::Plugin::PageOpenParam pageOpenParam;
	pageOpenParam.option = paf::Plugin::PageOption_None;

	paf::ui::Scene *pScene = plugin->PageOpen("page_main", pageOpenParam);

	g_text_pad = (paf::ui::Text *)pScene->FindChild("text_pad");
	g_text_touch_front = (paf::ui::Text *)pScene->FindChild("text_touch_front");
	g_text_touch_back = (paf::ui::Text *)pScene->FindChild("text_touch_back");
	g_text_motion = (paf::ui::Text *)pScene->FindChild("text_motion");
	g_text_keyboard = (paf::ui::Text *)pScene->FindChild("text_keyboard");

	g_rootPage = pScene;

	paf::common::SharedPtr<paf::inputdevice::InputListener> listenerPad(new my_InputListener(paf::inputdevice::DEVICE_TYPE_PAD));
	paf::common::SharedPtr<paf::inputdevice::InputListener> listenerTouchFront(new my_InputListener(paf::inputdevice::DEVICE_TYPE_TOUCHSCREEN_FRONT));
	paf::common::SharedPtr<paf::inputdevice::InputListener> listenerTouchBack(new my_InputListener(paf::inputdevice::DEVICE_TYPE_TOUCHSCREEN_BACK));
	paf::common::SharedPtr<paf::inputdevice::InputListener> listenerMotion(new my_InputListener(paf::inputdevice::DEVICE_TYPE_MOTIONSENSOR));
	paf::common::SharedPtr<paf::inputdevice::InputListener> listenerKeyboard(new my_InputListener(paf::inputdevice::DEVICE_TYPE_KEYBOARD));

	paf::inputdevice::AddInputListener(listenerPad);
	paf::inputdevice::AddInputListener(listenerTouchFront);
	paf::inputdevice::AddInputListener(listenerTouchBack);
	paf::inputdevice::AddInputListener(listenerMotion);
	paf::inputdevice::AddInputListener(listenerKeyboard);
}

int paf_sample_main(void) {
	paf::Framework::InitParam fwParam;
	fwParam.mode = paf::Framework::Mode_Normal;
	fwParam.text_surface_pool_size = 16 * 1024 * 1024;

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
