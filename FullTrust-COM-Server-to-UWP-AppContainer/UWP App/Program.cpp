#include "pch.h"

using namespace winrt;

// This example code shows how you could implement the required main function
// for a Console UWP Application. You can replace all the code inside main
// with your own custom code.

// You should also change the Alias value in the AppExecutionAlias Extension
// in the package.appxmanifest to a value that you define. To edit this file
// manually, open it with the XML Editor.

static constexpr GUID CLSID_TestFullTrust
{ 0x6a2c629d, 0xf85f, 0x423f, { 0x8f, 0xd8, 0x44, 0xde, 0x57, 0x6b, 0x60, 0x2e } };

int main()
{
    // You can get parsed command-line arguments from the CRT globals.
    wprintf(L"Parsed command-line arguments:\n");
    for (int i = 0; i < __argc; i++)
    {
        wprintf(L"__argv[%d] = %S\n", i, __argv[i]);
    }

    wprintf(L"Press Enter to continue and launch FullTrust Process:");
    getchar();
    // This full trust process registers the COM server that we will try and create an object instance from
    auto fullTrustProcess{ Windows::ApplicationModel::FullTrustProcessLauncher::LaunchFullTrustProcessForCurrentAppAsync() };

    // Wait for the console window for the full trust process to open and display its message before continuing.
    wprintf(L"Press Enter to continue and connect to FullTrust COM server:\n");
    getchar();

    com_ptr<ISequentialStream> tester;
    MULTI_QI desiredInterface
    {
        &CLSID_TestFullTrust,
        nullptr,
        0
    };

    // Try and create and object instance from the full trust process COM server.
    // This fails with REGDB_E_CLASSNOTREG.
    check_hresult(CoCreateInstanceFromApp(CLSID_TestFullTrust, nullptr, CLSCTX_LOCAL_SERVER, nullptr, 1, &desiredInterface));
    check_hresult(desiredInterface.hr);
    *tester.put() = reinterpret_cast<ISequentialStream*>(desiredInterface.pItf);

    wprintf(L"Press Enter to quit\n");
    getchar();
    wprintf(L"Exiting.\n");
}
