#include "pch.h"

using namespace winrt;
using namespace Windows::Foundation;

// {6A2C629D-F85F-423F-8FD8-44DE576B602E}
static constexpr GUID CLSID_TestFullTrust
{ 0x6a2c629d, 0xf85f, 0x423f, { 0x8f, 0xd8, 0x44, 0xde, 0x57, 0x6b, 0x60, 0x2e } };

// This is a sinple test class. The only thing we want it to do is to
// have its methods return S_OK when invoked to verify the COM server connection
struct TestFullTrust : winrt::implements<TestFullTrust, ISequentialStream>
{
    HRESULT __stdcall Read(
        [[maybe_unused]] void* pv,
        [[maybe_unused]] ULONG cb,
        [[maybe_unused]] ULONG* pcbRead) override
    {
        return S_OK;
    }
    HRESULT __stdcall Write(
        [[maybe_unused]] const void* pv,
        [[maybe_unused]] ULONG cb,
        [[maybe_unused]] ULONG* pcbWritten) override
    {
        return S_OK;
    }
};

struct TestFullTrustFactory : winrt::implements<TestFullTrustFactory, IClassFactory>
{
    HRESULT __stdcall CreateInstance(
        ::IUnknown* outer,
        GUID const& iid,
        void** result) noexcept final
    {
        *result = nullptr;

        if (outer)
        {
            return CLASS_E_NOAGGREGATION;
        }

        return make<TestFullTrust>()->QueryInterface(iid, result);
    }

    HRESULT __stdcall LockServer(BOOL) noexcept final
    {
        return S_OK;
    }
};

int main()
{
    init_apartment();
    
    DWORD registration{};

    winrt::check_hresult(::CoRegisterClassObject(
        CLSID_TestFullTrust,
        make<TestFullTrustFactory>().get(),
        CLSCTX_LOCAL_SERVER,
        REGCLS_SINGLEUSE,
        &registration));

    std::wcout << L"Waiting for COM object request. Press enter to quit.\n";

    std::wstring input;
    std::getline(std::wcin, input);
}
