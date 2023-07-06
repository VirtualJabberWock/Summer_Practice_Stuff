#pragma once
#include <filesystem>
#include <iostream>
#include <Windows.h>
#include <map>
#include <intrin.h>
#include "StringUtil.h"

#if _WIN32 || _WIN64
#if _WIN64
typedef __int64 machine_reg;
typedef union {
	__int64 d;
	void* ptr;
	char* charPtr;
} machine_unit;
#else
typedef __int32 machine_reg;
typedef union {
	__int32 d;
	void* ptr;
	char* charPtr;
} machine_unit;
#endif
#endif


typedef int (*testFunction)(...);


class ITestModule
{
public:

	ITestModule(std::string pathToModule) {
		exportedFunctions = std::map<std::string, testFunction>();
		exePath = pathToModule;
		std::vector<std::string> v = StringUtil::split(exePath, "\\");
		moduleName = v[v.size() - 1];
		loadModule();
		linkExportedFunctions();
	}

	virtual bool startTests();

private:

	std::string exePath;
	std::string moduleName;
	HMODULE module;

	std::map<std::string, testFunction> exportedFunctions;

	virtual void linkExportedFunctions();

	void* getExportedFunction(std::string funcName);
	void loadModule();

	void TestFunc(std::string name, std::vector<machine_unit>* args);

};

