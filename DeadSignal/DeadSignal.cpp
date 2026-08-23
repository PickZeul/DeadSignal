#include <windows.h>

constexpr LONG framebufferWidth = 2;
constexpr LONG framebufferHeight = 2;

DWORD framebuffer[framebufferWidth * framebufferHeight]
{
    0x00FF0000, 0x0000FF00,
    0x000000FF, 0x00FFFFFF
};

BITMAPINFO framebufferInfo
{
    {
        sizeof(BITMAPINFOHEADER),
        framebufferWidth,
        -framebufferHeight,
        1,
        32,
        BI_RGB
    }
};

LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_PAINT)
    {
        PAINTSTRUCT paint{};
        HDC deviceContext = BeginPaint(window, &paint);
        RECT clientArea{};
        GetClientRect(window, &clientArea);
        StretchDIBits(
            deviceContext,
            0,
            0,
            clientArea.right,
            clientArea.bottom,
            0,
            0,
            framebufferWidth,
            framebufferHeight,
            framebuffer,
            &framebufferInfo,
            DIB_RGB_COLORS,
            SRCCOPY);
        EndPaint(window, &paint);
        return 0;
    }

    if (message == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(window, message, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int showCommand)
{
    constexpr wchar_t windowClassName[] = L"DeadSignalWindow";

    WNDCLASSW windowClass{};
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WindowProcedure;
    windowClass.hInstance = instance;
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    windowClass.lpszClassName = windowClassName;

    if (!RegisterClassW(&windowClass))
    {
        return 0;
    }

    HWND window = CreateWindowExW(
        0,
        windowClassName,
        L"DEAD SIGNAL",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        nullptr,
        nullptr,
        instance,
        nullptr);

    if (!window)
    {
        return 0;
    }

    ShowWindow(window, showCommand);

    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    return static_cast<int>(message.wParam);
}
