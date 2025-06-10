
#include <sys/stat.h>
#include <algorithm>
#include <map>
#include <cstring>
#include <string>
#include <vector>
#include <sstream>
#include "sha1.h"
#include "emd2yml.h"


int main(int argc, char *argv[]){

	emd2yml::store _store;
	_store.SetModule("ScePaf");

/*
	emd2yml::Load(_store, "../build/ScePaf-RE/lib/ScePafAutoTestTty.emd");
	emd2yml::Load(_store, "../build/ScePaf-RE/lib/ScePafCommon.emd");
	emd2yml::Load(_store, "../build/ScePaf-RE/lib/ScePafGraphics.emd");
	emd2yml::Load(_store, "../build/ScePaf-RE/lib/ScePafLowlayer.emd");
	emd2yml::Load(_store, "../build/ScePaf-RE/lib/ScePafMisc.emd");
	emd2yml::Load(_store, "../build/ScePaf-RE/lib/ScePafResource.emd");
	emd2yml::Load(_store, "../build/ScePaf-RE/lib/ScePafStdc.emd");
	emd2yml::Load(_store, "../build/ScePaf-RE/lib/ScePafThread.emd");
	emd2yml::Load(_store, "../build/ScePaf-RE/lib/ScePafToplevel.emd");
	emd2yml::Load(_store, "../build/ScePaf-RE/lib/ScePafWidget.emd");
*/

	emd2yml::Load(_store, "../ScePaf-RE/lib/ScePafCommon.emd");
	emd2yml::Load(_store, "../ScePaf-RE/lib/ScePafThread.emd");
	emd2yml::Load(_store, "../ScePaf-RE/lib/ScePafAutoTestTty.emd");
	emd2yml::Load(_store, "../ScePaf-RE/lib/ScePafMisc.emd");
	emd2yml::Load(_store, "../ScePaf-RE/lib/ScePafLowlayer.emd");
	emd2yml::Load(_store, "../ScePaf-RE/lib/ScePafWidget.emd");
	emd2yml::Load(_store, "../ScePaf-RE/lib/ScePafStdc.emd");
	emd2yml::Load(_store, "../ScePaf-RE/lib/ScePafGraphics.emd");
	emd2yml::Load(_store, "../ScePaf-RE/lib/ScePafResource.emd");
	emd2yml::Load(_store, "../ScePaf-RE/lib/ScePafToplevel.emd");

	_store.SetModule("SceAppSettings");
	emd2yml::Load(_store, "../ScePaf-RE/lib/SceAppSettings.emd");

	_store.SetModule("SceCommonGuiDialog");
	emd2yml::Load(_store, "../ScePaf-RE/lib/SceCommonGuiDialog.emd");

	_store.SetModule("ScePafWebMapView");
	emd2yml::Load(_store, "../ScePaf-RE/lib/ScePafWebMapView.emd");

	_store.SetModule("SceWebUIPlugin");
	emd2yml::Load(_store, "../ScePaf-RE/lib/SceWebUIPlugin.emd");

	printf("version: %d\n", 2);
	printf("firmware: %s\n", "3.60");
	printf("modules:\n");

	for(std::map<std::string, emd2yml::module>::iterator it_module=_store.m_module_map.begin();it_module!=_store.m_module_map.end();it_module++){
		emd2yml::module& _module = (*it_module).second;

		printf("  %s:\n", _module.m_name.c_str());
		printf("    libraries:\n");

		for(std::vector<emd2yml::library>::iterator it=_module.m_library.begin();it!=_module.m_library.end();it++){
			emd2yml::library& library = (*it);

			printf("      %s:\n", library.m_name.c_str());
			printf("        kernel: false\n");
			printf("        nid: %s\n", library.m_nid.c_str());

			if(library.m_function.begin() != library.m_function.end()){
				printf("        functions:\n");
				for(std::vector<emd2yml::function>::iterator it2=library.m_function.begin();it2!=library.m_function.end();it2++){
					emd2yml::function& function = *it2;

					std::string ent_name = function.m_name + library.m_suffix;

					SHA1Context ctx;
					uint8_t hash[0x14];
					SHA1Reset(&ctx);
					SHA1Input(&ctx, ent_name.c_str(), ent_name.length());
					SHA1Result(&ctx, hash);

					char nid_str[0x10];
					snprintf(nid_str, sizeof(nid_str), "0x%02X%02X%02X%02X", hash[3], hash[2], hash[1], hash[0]);

					if(strcmp(function.m_nid.c_str(), nid_str) == 0){
						printf("          %s: %s\n", function.m_name.c_str(), function.m_nid.c_str());
					}else{
						printf("          %s: %s # wrong name\n", function.m_name.c_str(), function.m_nid.c_str());
					}
				}
			}

			if(library.m_variable.begin() != library.m_variable.end()){
				printf("        variables:\n");
				for(std::vector<emd2yml::variable>::iterator it2=library.m_variable.begin();it2!=library.m_variable.end();it2++){
					emd2yml::variable& variable = *it2;
					// printf("          %s: %s\n", variable.m_name.c_str(), variable.m_nid.c_str());


					std::string ent_name = variable.m_name + library.m_suffix;

					SHA1Context ctx;
					uint8_t hash[0x14];
					SHA1Reset(&ctx);
					SHA1Input(&ctx, ent_name.c_str(), ent_name.length());
					SHA1Result(&ctx, hash);

					char nid_str[0x10];
					snprintf(nid_str, sizeof(nid_str), "0x%02X%02X%02X%02X", hash[3], hash[2], hash[1], hash[0]);

					if(strcmp(variable.m_nid.c_str(), nid_str) == 0){
						printf("          %s: %s\n", variable.m_name.c_str(), variable.m_nid.c_str());
					}else{
						printf("          %s: %s # wrong name\n", variable.m_name.c_str(), variable.m_nid.c_str());
					}
				}
			}
		}
	}

	return EXIT_SUCCESS;
}
