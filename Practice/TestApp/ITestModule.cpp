#include "ITestModule.h"

void* ITestModule::getExportedFunction(std::string funcName)
{
	if (module == 0) throw std::exception("Wrong module HANDLE!");
	void* f_addr = GetProcAddress(module, funcName.c_str());
	if(f_addr == 0) throw std::exception(
		std::string("Can't find function with name: "+funcName+" in "+moduleName).c_str()
	);
	return f_addr;
}

void ITestModule::loadModule()
{
	LoadLibraryA(exePath.c_str());
	HMODULE h = GetModuleHandleA(moduleName.c_str());
	if (h == 0) throw std::exception("Wrong module name!");
	module = h;
}

void ITestModule::TestFunc(std::string name, std::vector<machine_unit>* args)
{
	testFunction func = exportedFunctions[name];
	if (func == 0)
		throw std::exception(std::string("FATAL ERROR: function don't linked: "+name).c_str());
	
	func();
}

bool ITestModule::startTests()
{
	return true;
}

void ITestModule::linkExportedFunctions()
{

}
