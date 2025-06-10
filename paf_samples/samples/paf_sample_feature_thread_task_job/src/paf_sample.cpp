
#ifdef __SNC__
#include <kernel.h>
#else
#include <psp2/kernel/clib.h>
#include <psp2/kernel/processmgr.h>

#ifndef SCE_KERNEL_HIGHEST_PRIORITY_USER
#define SCE_KERNEL_HIGHEST_PRIORITY_USER (SCE_KERNEL_PROCESS_PRIORITY_USER_HIGH)
#endif

#endif

#include <paf.h>


paf::Framework *g_fw;
paf::ui::Plane *g_plane;
void *g_rootPage;

/*
* Jobs are advanced execution units that run on top of the threads and use queue-type scheduling
*/
class my_Job : public paf::job::JobItem
{
public:

	my_Job() : paf::job::JobItem("my_Job", NULL){

	}

	~my_Job(){

	}

	void Run(){
		sceClibPrintf("Hello from my_Job()\n");

		/*
		* Since jobs run independently from the rendering, you need to block
		* the main thread before changing any widget properties from your job.
		*/
		paf::thread::RMutex::main_thread_mutex.Lock();
		g_plane->SetSize({ 100.0f, 100.0f });
		paf::thread::RMutex::main_thread_mutex.Unlock();
	}

	void Finish(){

	}
};

/*
* PAF thread wrap around normal C kernel threads, providing convinient C++ thread implementation.
*/
class my_Thread : public paf::thread::Thread
{
public:

	my_Thread(Option *option) : paf::thread::Thread(SCE_KERNEL_HIGHEST_PRIORITY_USER, SCE_KERNEL_4KiB, "my_Thread", option){

	}

	void EntryFunction(){
		sceClibPrintf("Hello from my_Thread()\n");

		/*
		* Since threads run independently from the rendering, you need to block
		* the main thread before changing any widget properties from your thread.
		*/
		paf::thread::RMutex::main_thread_mutex.Lock();
		g_plane->SetSize({ 100.0f, 100.0f });
		paf::thread::RMutex::main_thread_mutex.Unlock();

		paf::common::SharedPtr<paf::job::JobItem> itemParam(new my_Job());
		paf::job::JobQueue::default_queue->Enqueue(itemParam);

		Cancel();
	}
};

/*
* Tasks (MainThreadCall) are executed on each frame from the main thread after the rendering stage.
* Since tasks are executed from the rendering thread, you shouldn't do any long blocking processing in them.
*/
void myTask(void *userdata){
	sceClibPrintf("Hello from myTask()\n");

	g_plane->SetSize({ 100.0f, 100.0f });

	auto *thrd = new my_Thread(NULL);
	thrd->Start();

	//Tasks can recursively unregister themselves
	paf::common::MainThreadCallList::Unregister(myTask, NULL);
}

void loadPluginCB(paf::Plugin *plugin){
	paf::Plugin::PageOpenParam pageOpenParam;
	pageOpenParam.option = paf::Plugin::PageOption_None;

	paf::ui::Scene *pScene = plugin->PageOpen("page_main", pageOpenParam);

	g_plane = (paf::ui::Plane *)pScene->FindChild("plane_sample_white");

	g_rootPage = pScene;

	paf::common::MainThreadCallList::Register(myTask, NULL);
}

int paf_sample_main(void){
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
	

	sceClibPrintf("[SAMPLE] Failed to run PAF instance\n");

	exit(0);

	return 0;
}
