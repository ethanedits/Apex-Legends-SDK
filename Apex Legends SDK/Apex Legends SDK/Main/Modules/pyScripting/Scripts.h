#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>

#include <Python.h>
#include <pybind11/embed.h>

#include "../../../SDK/Entity/BaseEntity.h"


namespace py = pybind11;
namespace fs = std::filesystem;

int BaseEntityAddr(BaseEntity pEntity)
{
	return (int)pEntity.Entity;
}

int LocalEntityAddr(LocalEntity pLocal)
{
	return (int)pLocal.Entity;
}

//Python Apex Entity Classes
PYBIND11_EMBEDDED_MODULE(Entity, m) {
	py::class_<BaseEntity>(m, "BaseEntity")
		.def(py::init<int>())
		.def("Entity", &BaseEntityAddr)
		.def("Glow", &BaseEntity::Glow)
		.def("GlowStyle", &BaseEntity::setGlowStyle)
		.def("isPlayer", &BaseEntity::isPlayer)
		.def("iHealth", &BaseEntity::iHealth)
		.def("iShield", &BaseEntity::iShield)
		.def("iTeam", &BaseEntity::iTeam)
		.def("isKnocked", &BaseEntity::isKnocked)
		.def("distanceToLocal", &BaseEntity::distanceToLocal);

	py::class_<LocalEntity>(m, "LocalEntity")
		.def(py::init())
		.def("Entity", &LocalEntityAddr)
		.def("iHealth", &LocalEntity::iHealth)
		.def("iShield", &LocalEntity::iShield)
		.def("iTeam", &LocalEntity::iTeam);
}

namespace Scripts {

	std::vector<std::string> scriptList{};
	std::string userScriptsPath;

	void InitUserScripts()
	{
		fs::path path = std::filesystem::current_path();
		fs::path file("UserScripts");
		fs::path full_path = path / file; //Get UserScripts folder
		userScriptsPath = full_path.string();

		for (const auto& entry : std::filesystem::directory_iterator(full_path)) { //Loop through all files in folder
			fs::path filename = entry.path().filename();

			if (filename.string() != "__init__.py" && filename.string().find(".py") != std::string::npos) { //Find a valid UserScript
				scriptList.push_back(filename.string()); //Add valid script name to vector
			}
		}

		py::scoped_interpreter guard{};

		for (int i = 0; i < scriptList.size(); i++) //Iterate through valid scripts
		{
			try {
				std::string scriptName = scriptList.at(i);
				std::size_t pos = scriptName.find(".py");
				if (pos != std::string::npos)
					scriptName.erase(pos, 3); //Remove .py from string

				std::string userScriptModule = "UserScripts." + scriptName;
				const char* charScriptName = userScriptModule.c_str();

				auto scriptModule = py::module::import(charScriptName); //Import Module
				std::cout << "[-] UserScript Loaded: " << scriptName << std::endl;

				auto func = scriptModule.attr("main"); //Call function called main in UserScript

				while (!GetAsyncKeyState(Globals::killKey)) {
					if (MenuSettings::userScriptsEnabled)
						func();
				}
			}
			catch (py::error_already_set& e) {
				std::cout << "[!] Python - " << e.what() << std::endl;
			}
		}
	}
}