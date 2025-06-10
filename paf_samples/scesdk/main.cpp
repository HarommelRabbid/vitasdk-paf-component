
#include <kernel.h>
#include <libsysmodule.h>


char    sceUserMainThreadName[]          = "paf_sample";
int     sceUserMainThreadPriority        = 0x10000100;
int     sceUserMainThreadCpuAffinityMask = 0x70000;
SceSize sceUserMainThreadStackSize       = 0x4000;


int paf_sample_main(void);

extern "C" {

typedef struct _ScePafInit { // size is 0x18
	SceSize global_heap_size;
	int a2;
	int a3;
	int cdlg_mode;
	int heap_opt_param1;
	int heap_opt_param2;
} ScePafInit;

typedef struct SceSysmoduleOpt {
	int flags;
	int *result;
	int unused[2];
} SceSysmoduleOpt;

int _main(){

	int load_res;
	ScePafInit init_param;
	SceSysmoduleOpt sysmodule_opt;

	init_param.global_heap_size = SCE_KERNEL_8MiB;//0x1000000;
	init_param.a2               = 0xEA60;
	init_param.a3               = 0x40000;
	init_param.cdlg_mode        = 0;
	init_param.heap_opt_param1  = 0;
	init_param.heap_opt_param2  = 0;

	load_res = 0xDEADBEEF;
	sysmodule_opt.flags  = 0;
	sysmodule_opt.result = &load_res;

	int res = sceSysmoduleLoadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, sizeof(init_param), &init_param, &sysmodule_opt);
	if((res | load_res) != 0){
		sceClibPrintf("[PAF PRX Loader] Failed to load the PAF prx. (return value 0x%x, result code 0x%x )\n", res, load_res);
	}

	paf_sample_main();

	// sceKernelExitProcess(0);

	return 0;
}

void __cxa_set_dso_handle_main(void *dso){

}

int _sceLdTlsRegisterModuleInfo(){
	return 0;
}

int __at_quick_exit(){
	return 0;
}

}

int main() {
	_main();
}
