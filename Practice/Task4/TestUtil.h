#pragma once

#define EXCEPTED(name) (_excepted_##name) /*value which is excepted at the test function*/
#define _ARGS  /*notaion of except*/
#define _EXCEPT /*notaion of except*/

#define TEST_OK 1
#define TEST_FAIL 0


#define TEST_(status, moduleName, func, ...) \
status &= test_##moduleName##_##func(__VA_ARGS__, __LINE__);

#define MAKE_TEST(moduleName, funcName, ...) \
int test_##moduleName##_##funcName(__VA_ARGS__, int __line__)


#define IS_TEST_EXPORTED 0

#if IS_TEST_EXPORTED
#define EXPORT_TEST __declspec(dllexport)
#else
#define EXPORT_TEST 
#endif