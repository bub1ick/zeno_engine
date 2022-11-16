#include <zeno.hpp>

namespace zeno::dx11
{
renderer_t::renderer_t()
{
    //  create DXGI factory to handle DXGI
    m_result = CreateDXGIFactory2(0, IID_IDXGIFactory7, reinterpret_cast<void**>(&m_dxgi.factory));
    if (FAILED(m_result))  //  check for failure
        std::cerr << "Failed on CreateDXGIFactory2!\t" << std::hex << m_result << std::endl;

    std::vector<IDXGIAdapter4*> adapters;  //  vector holding all found adapters

    get_all_available_adapters(adapters);

    //  TODO: wchar to utf-8 automatic converter
    //   print adapter names
    for (auto adapter : adapters)
    {
        std::cout << "adapter description:\t";  //  decorator

        DXGI_ADAPTER_DESC3 adapter_descriptor;   //  variable to store various info about the adapter
        adapter->GetDesc3(&adapter_descriptor);  //  retrieve the descriptor

        std::wstring wide_name   = adapter_descriptor.Description;  //  store the widechar version of the adapter name
        size_t       size_needed = WideCharToMultiByte(             //  get the size of the adapter name string
            CP_UTF8,
            0,
            &wide_name [0],
            wide_name.size(),
            0,
            0,
            0,
            0
        );

        std::string  utf8_name(size_needed, 0);  //  initialize a utf-8 string for storing adapter name
        WideCharToMultiByte(                     //  translate wchar_t to char
            CP_UTF8,
            0,
            &wide_name [0],
            (int) wide_name.size(),
            &utf8_name [0],
            size_needed,
            0,
            0
        );

        std::cout << utf8_name << std::endl;  //  print the name
    }
}

void renderer_t::get_all_available_adapters(std::vector<IDXGIAdapter4*>& out_adapters)
{
    IDXGIAdapter4* adapter;  //  used to temporary store found adapter

    //  getting all the adapters
    uint8_t        index = 0;   //  adapter index
    bool           could_find;  //  to check wether there are adapters left
    while (true)
    {
        could_find =
            m_dxgi.factory->EnumAdapterByGpuPreference(
                index, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_IDXGIAdapter4, reinterpret_cast<void**>(&adapter)
            ) != DXGI_ERROR_NOT_FOUND;  //  get the adapter in order of "highest performance first"

        if (not could_find)  //  finish if couldn't find the next adapter
            break;

        out_adapters.push_back(adapter);  //  store the found adapter

        index++;  //  next adapter
    }
}
}  //  namespace zeno::dx11