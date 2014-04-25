#include "common.h"
#include "fmod.hpp"
#include "fmod_errors.h"

// Include common_platform from test framework
#define TEST_EXAMPLES
#include "common_test_platform.h"

// Taken from test framework common.h
#define SMOKETEST_ERR_FMOD_ERROR        251

// Declared in common_platform.cpp
extern int Common_Private_Argc;
extern char** Common_Private_Argv;
extern void (*Common_Private_Update)(unsigned int*);
extern void (*Common_Private_Error)(FMOD_RESULT);
extern void (*Common_Private_Close)();

// Main functionality
int Common_AutoTest_QuitCounter = 50;

void Common_AutoTest_Update(unsigned int *buttons)
{
    // For now always auto quit after a number of updates
    Common_AutoTest_QuitCounter--;
    if (Common_AutoTest_QuitCounter <= 0)
    {
        *buttons |= (1<<BTN_QUIT);
        PRINT("Issuing quit command\n");
    }
}

void Common_AutoTest_Exit(int returnCode)
{
    PRINT("EXIT_CODE:%d\n", returnCode);
    EXIT(returnCode);
}

void Common_AutoTest_Close()
{
    PRINT("Finished example, quitting\n");
    Common_AutoTest_Exit(0);
}

void Common_AutoTest_Error(FMOD_RESULT result)
{
    PRINT("ERRCHECK failed: result = %s\n", FMOD_ErrorString(result));
    Common_AutoTest_Exit(SMOKETEST_ERR_FMOD_ERROR);
}


class Common_AutotTest_Startup
{
    public:
        Common_AutotTest_Startup()
        {
            Common_Private_Update = Common_AutoTest_Update;
            Common_Private_Error = Common_AutoTest_Error;
            Common_Private_Close = Common_AutoTest_Close;
            platformInit();
        }
};
Common_AutotTest_Startup gStartup;