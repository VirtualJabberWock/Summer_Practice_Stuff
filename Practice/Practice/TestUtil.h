#pragma once

//TEST


#define EXCEPTED(name) (_excepted_##name) /*value which is excepted at the test function*/
#define EXCEPT(name) /*notaion of except*/

#define TEST_OK 1
#define TEST_FAIL 0

#define IS_TEST_EXPORTED 0

#define TEST_(status, func, ...) \
status &= test_ParseInt_##func(__VA_ARGS__, __LINE__);

#if IS_TEST_EXPORTED
#define EXPORT_TEST __declspec(dllexport)
#else
#define EXPORT_TEST 
#endif