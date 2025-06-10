
#ifndef _EMD2YML_H_
#define _EMD2YML_H_

#include <sys/stat.h>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <map>

class emd2yml {
public:
	emd2yml(){
	}

	~emd2yml(){
	}

	class ent {
	public:
		ent(){
		}

		~ent(){
		}

		std::string m_nid;
		std::string m_name;
	};

	class function : public ent {
	public:
		function(){
		}

		~function(){
		}
	};

	class variable : public ent {
	public:
		variable(){
		}

		~variable(){
		}
	};

	class library {
	public:
		library(){
		}

		~library(){
		}

		void SetLibraryName(const char *_name){
			m_name = _name;
		}

		void SetLibraryNID(const char *_nid){
			m_nid = _nid;
		}

		void SetLibrarySuffix(const char *_suffix){
			m_suffix = _suffix;
		}

		void SetLibraryVersion(const char *_version){
			m_version = _version;
		}

		function& SetFunction(const char *name){
			m_function.push_back(function());

			function& r = m_function[m_function.size() - 1];
			r.m_name = name;
			return r;
		}

		variable& SetVariable(const char *name){
			m_variable.push_back(variable());

			variable& r = m_variable[m_variable.size() - 1];
			r.m_name = name;
			return r;
		}

		std::string m_name;
		std::string m_nid;
		std::string m_suffix;
		std::string m_version;
		std::vector<function> m_function;
		std::vector<variable> m_variable;
	};

	class module {
	public:
		module(){
		}

		~module(){
		}

		void SetName(const char *name){
			m_name = name;
		}

		class search {
		public:
			search(const char *pKey){
				m_target = pKey;
			}

			bool operator()(const library& _library) const {
				return strcmp(m_target.c_str(), _library.m_name.c_str()) == 0;
			}

			std::string m_target;
		};

		library& GetLibrary(const char *name){

			std::vector<library>::iterator res = std::find_if(m_library.begin(), m_library.end(), search(name));
			if(res != m_library.end()){
				return *res;
			}

			m_library.push_back(library());

			library& r = m_library[m_library.size() - 1];
			r.m_name = name;
			return r;
		}

		std::string m_name;
		std::vector<library> m_library;
	};

	class store {
	public:
		store() :
			m_module_current(NULL),
			m_library_current(NULL),
			m_function_current(NULL),
			m_variable_current(NULL),
			m_last_ent(NULL)
		{
		}

		~store(){
		}

		void SetModule(const char *name){
			m_module_current = &m_module_map[name];
			m_module_current->SetName(name);
		}

		void SetLibrary(const char *name){
			m_library_current = &m_module_current->GetLibrary(name);
			m_library_current->SetLibraryName(name);
		}

		void SetLibraryName(const char *_name){
			m_library_current->SetLibraryName(_name);
		}

		void SetLibraryNID(const char *_nid){
			m_library_current->SetLibraryNID(_nid);
		}

		void SetLibrarySuffix(const char *_suffix){
			m_library_current->SetLibrarySuffix(_suffix);
		}

		void SetLibraryVersion(const char *_version){
			m_library_current->SetLibraryVersion(_version);
		}

		void SetFunction(const char *name){
			m_function_current = &m_library_current->SetFunction(name);
			m_last_ent = m_function_current;

			SHA1Context ctx;
			uint8_t hash[0x14];
			char nid_str[0x20];

			std::string ent_name = m_last_ent->m_name + m_library_current->m_suffix;

			// printf("%s\n", ent_name.c_str());

			SHA1Reset(&ctx);
			SHA1Input(&ctx, ent_name.c_str(), strlen(ent_name.c_str()));
			SHA1Result(&ctx, hash);

			snprintf(nid_str, sizeof(nid_str), "0x%02X%02X%02X%02X", hash[3], hash[2], hash[1], hash[0]);

			m_last_ent->m_nid = nid_str;
		}

		void SetVariable(const char *name){
			m_variable_current = &m_library_current->SetVariable(name);
			m_last_ent = m_variable_current;

			SHA1Context ctx;
			uint8_t hash[0x14];
			char nid_str[0x20];

			std::string ent_name = m_last_ent->m_name + m_library_current->m_suffix;

			SHA1Reset(&ctx);
			SHA1Input(&ctx, ent_name.c_str(), strlen(ent_name.c_str()));
			SHA1Result(&ctx, hash);

			snprintf(nid_str, sizeof(nid_str), "0x%02X%02X%02X%02X", hash[3], hash[2], hash[1], hash[0]);
			m_last_ent->m_nid = nid_str;
		}

		void SetNIDtoLastEnt(const char *nidvalue){
			m_last_ent->m_nid = nidvalue;
		}

		std::map<std::string, module> m_module_map;
		module *m_module_current;
		library *m_library_current;
		function *m_function_current;
		variable *m_variable_current;
		ent *m_last_ent;
	private:
	};

	class LoadEmd {
	public:
		LoadEmd(const char *path){
			emd_path = path;
			emd_text = NULL;
		}

		~LoadEmd(){
			free(emd_text);
		}

		int Load(store& _store){

			int res;
			struct stat _stat;

			res = stat(emd_path.c_str(), &_stat);
			if(res < 0){
				return res;
			}

			emd_text = (char *)malloc(_stat.st_size + 1);
			emd_text[_stat.st_size] = 0;

			FILE *fp = fopen(emd_path.c_str(), "rb");
			fread((void *)emd_text, _stat.st_size, 1, fp);
			fclose(fp);

			std::string emd_data = emd_text;

			std::vector<std::string> v = split_core(emd_data, '\n');

			for(std::vector<std::string>::iterator it=v.begin();it!=v.end();it++){
				std::vector<std::string> line = split_core(*it, ' ');
				for(std::vector<std::string>::iterator it2=line.begin();it2!=line.end();){
					std::string& l = *it2;

					const char *piece = l.c_str();
					if(strncmp(piece, "//", 2) == 0){ // comment
						it2++;

						// printf("comment");

						if(piece[2] != 0){
							// printf(" %s", &(piece[2]));
						}

						while(it2 != line.end()){
							// printf(" %s", (*it2).c_str());
							it2++;
						}
						// printf("\n");
					}else if(strcmp(piece, "Library:") == 0){
						it2++;
						_store.SetLibrary((*it2).c_str());
						// printf("Library %s\n", (*it2).c_str());
						it2++;
					}else if(strcmp(piece, "function:") == 0){
						it2++;
						_store.SetFunction((*it2).c_str());
						// printf("function %s\n", (*it2).c_str());
						it2++;
					}else if(strcmp(piece, "variable:") == 0){
						it2++;
						_store.SetVariable((*it2).c_str());
						// printf("variable %s\n", (*it2).c_str());
						it2++;
					}else if(strcmp(piece, "nidvalue:") == 0){
						it2++;
						_store.SetNIDtoLastEnt((*it2).c_str());
						// printf("nid %s\n", (*it2).c_str());
						it2++;
					}else if(strcmp(piece, "nidsuffix:") == 0){
						it2++;

						const char *suffix = (*it2).c_str();

						if(suffix[0] == '"' && suffix[(*it2).length() - 1] == '"'){
							std::string _suffix(suffix + 1, (*it2).length() - 2);
							_store.SetLibrarySuffix(_suffix.c_str());
						}else{
							_store.SetLibrarySuffix((*it2).c_str());
						}

						// printf("nidsuffix %s\n", (*it2).c_str());
						it2++;
					}else if(strcmp(piece, "emd:") == 0){
						it2++;
						// printf("emd %s\n", (*it2).c_str());
						it2++;
					}else if(strcmp(piece, "attr:") == 0){
						it2++;
						// printf("attr");
						while(it2 != line.end()){
							// printf(" %s", (*it2).c_str());
							it2++;
						}
						// printf("\n");
					}else if(strcmp(piece, "version:") == 0){
						it2++;
						_store.SetLibraryVersion((*it2).c_str());
						// printf("version %s\n", (*it2).c_str());
						it2++;
					}else if(strcmp(piece, "stubfile:") == 0){
						it2++;
						// printf("stubfile %s\n", (*it2).c_str());
						it2++;
					}else if(strcmp(piece, "libnamenid:") == 0){
						it2++;
						_store.SetLibraryNID((*it2).c_str());
						// printf("libnamenid %s\n", (*it2).c_str());
						it2++;
					}else{
						it2++;
						// printf("unk %s\n", piece);
					}
				}
			}

			return 0;
		}

	private:
		static std::vector<std::string> split_core(const std::string &s, char delim){

			std::vector<std::string> elems;
			std::stringstream ss(s);
			std::string item;

			while(getline(ss, item, delim)){
				if(!item.empty()){
					// item.erase(std::remove(item.begin(), item.end(), ' '), item.end());
					elems.push_back(item);
				}
			}

			return elems;
		}

		std::string emd_path;
		struct stat _stat;
		char *emd_text;
	};

	static void Load(store& _store, const char *emd_path){
		emd2yml::LoadEmd emd = emd_path;
		emd.Load(_store);
	}
};

#endif   /* _EMD2YML_H_ */
