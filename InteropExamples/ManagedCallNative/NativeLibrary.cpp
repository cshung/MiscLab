#include "NativeInterface.h"

class CNativeComponent : public INativeComponent
{
public:
    CNativeComponent();

    virtual ULONG STDMETHODCALLTYPE AddRef();

    virtual ULONG STDMETHODCALLTYPE Release();

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void **ppvObject);

    virtual HRESULT STDMETHODCALLTYPE NativeMethod(
        /* [in] */ long input,
        /* [out] */ long *output);
private:
    long m_cRef;
};

CNativeComponent::CNativeComponent() : m_cRef(0)
{
}

ULONG STDMETHODCALLTYPE CNativeComponent::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

ULONG STDMETHODCALLTYPE CNativeComponent::Release()
{
    ULONG i = InterlockedDecrement(&m_cRef);
    if (i == 0)
        delete this;
    return i;
}

HRESULT STDMETHODCALLTYPE CNativeComponent::QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ void **ppvObject)
{
    if (riid == __uuidof(IUnknown))
    {
        AddRef();
        *ppvObject = this;
        return S_OK;
    }
    else if (riid == __uuidof(INativeComponent))
    {
        AddRef();
        *ppvObject = static_cast<INativeComponent*>(this);
        return S_OK;
    }

    *ppvObject = nullptr;
    return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE CNativeComponent::NativeMethod(
    /* [in] */ long input,
    /* [out] */ long *output)
{
    *output = input + 1;
    return S_OK;
}

extern "C" __declspec(dllexport) INativeComponent* GetNativeComponent()
{
    INativeComponent* nativeComponent = new CNativeComponent();
    nativeComponent->AddRef();
    return nativeComponent;
}