#include <windows.h>
#include <timeapi.h>

#pragma comment(lib, "winmm.lib")

constexpr LONG framebufferWidth = 320;
constexpr LONG framebufferHeight = 180;
constexpr DWORD toneSampleRate = 8000;
constexpr DWORD toneFrequency = 440;
constexpr DWORD toneDurationMilliseconds = 250;
constexpr DWORD toneSampleCount = toneSampleRate * toneDurationMilliseconds / 1000;

DWORD framebuffer[framebufferWidth * framebufferHeight];

bool wPressed = false;
bool spacePressed = false;
bool tonePlaying = false;
BYTE toneSamples[toneSampleCount];
HWAVEOUT audioOutput = nullptr;
WAVEHDR toneHeader{};

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
    if ((message == WM_KEYDOWN || message == WM_KEYUP) && wParam == VK_SPACE)
    {
        bool pressed = message == WM_KEYDOWN;
        if (pressed && !spacePressed && !tonePlaying
            && waveOutWrite(audioOutput, &toneHeader, sizeof(toneHeader)) == MMSYSERR_NOERROR)
        {
            tonePlaying = true;
        }

        spacePressed = pressed;
        return 0;
    }

    if ((message == WM_KEYDOWN || message == WM_KEYUP) && wParam == 'W')
    {
        wPressed = message == WM_KEYDOWN;
        return 0;
    }

    if (message == WM_PAINT)
    {
        PAINTSTRUCT paint{};
        HDC deviceContext = BeginPaint(window, &paint);
        RECT clientArea{};
        GetClientRect(window, &clientArea);
        FillRect(deviceContext, &clientArea, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

        int clientWidth = clientArea.right - clientArea.left;
        int clientHeight = clientArea.bottom - clientArea.top;
        int destinationWidth = clientWidth;
        int destinationHeight = clientWidth * framebufferHeight / framebufferWidth;
        if (destinationHeight > clientHeight)
        {
            destinationHeight = clientHeight;
            destinationWidth = clientHeight * framebufferWidth / framebufferHeight;
        }

        int destinationX = (clientWidth - destinationWidth) / 2;
        int destinationY = (clientHeight - destinationHeight) / 2;
        StretchDIBits(
            deviceContext,
            destinationX,
            destinationY,
            destinationWidth,
            destinationHeight,
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

    for (LONG pixel = 0; pixel < framebufferWidth * framebufferHeight; ++pixel)
    {
        framebuffer[pixel] = 0x00101018;
    }

    for (LONG x = 0; x < framebufferWidth; ++x)
    {
        framebuffer[(framebufferHeight / 2) * framebufferWidth + x] = 0x00404040;
        framebuffer[x] = 0x00800000;
        framebuffer[(framebufferHeight - 1) * framebufferWidth + x] = 0x00000080;
    }

    for (LONG y = 0; y < framebufferHeight; ++y)
    {
        framebuffer[y * framebufferWidth + framebufferWidth / 2] = 0x00404040;
        framebuffer[y * framebufferWidth] = 0x00008000;
        framebuffer[y * framebufferWidth + framebufferWidth - 1] = 0x00808000;
    }

    for (LONG y = 8; y < 16; ++y)
    {
        for (LONG x = 8; x < 16; ++x)
        {
            framebuffer[y * framebufferWidth + x] = 0x00FF0000;
        }

        for (LONG x = 24; x < 32; ++x)
        {
            framebuffer[y * framebufferWidth + x] = 0x0000FF00;
        }
    }

    for (DWORD sample = 0; sample < toneSampleCount; ++sample)
    {
        toneSamples[sample] = ((sample * toneFrequency * 2 / toneSampleRate) & 1) ? 64 : 192;
    }

    WAVEFORMATEX toneFormat{};
    toneFormat.wFormatTag = WAVE_FORMAT_PCM;
    toneFormat.nChannels = 1;
    toneFormat.nSamplesPerSec = toneSampleRate;
    toneFormat.nAvgBytesPerSec = toneSampleRate;
    toneFormat.nBlockAlign = 1;
    toneFormat.wBitsPerSample = 8;

    if (waveOutOpen(&audioOutput, WAVE_MAPPER, &toneFormat, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR)
    {
        return 0;
    }

    toneHeader.lpData = reinterpret_cast<LPSTR>(toneSamples);
    toneHeader.dwBufferLength = sizeof(toneSamples);
    if (waveOutPrepareHeader(audioOutput, &toneHeader, sizeof(toneHeader)) != MMSYSERR_NOERROR)
    {
        waveOutClose(audioOutput);
        return 0;
    }

    ShowWindow(window, showCommand);

    timeBeginPeriod(1);
    LARGE_INTEGER performanceFrequency{};
    QueryPerformanceFrequency(&performanceFrequency);
    LARGE_INTEGER previousUpdate{};
    QueryPerformanceCounter(&previousUpdate);
    constexpr LONGLONG updatesPerSecond = 60;
    const LONGLONG updateInterval = performanceFrequency.QuadPart / updatesPerSecond;

    MSG message{};
    bool running = true;
    while (running)
    {
        while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
            {
                running = false;
                break;
            }

            TranslateMessage(&message);
            DispatchMessageW(&message);
        }

        if (!running)
        {
            break;
        }

        if (tonePlaying && (toneHeader.dwFlags & WHDR_DONE))
        {
            tonePlaying = false;
        }

        LARGE_INTEGER currentTime{};
        QueryPerformanceCounter(&currentTime);
        if (currentTime.QuadPart - previousUpdate.QuadPart >= updateInterval)
        {
            previousUpdate = currentTime;
            DWORD updateColor = (framebuffer[8 * framebufferWidth + 8] + 0x00050000) & 0x00FF0000;
            DWORD inputColor = wPressed ? 0x00FFFF00 : 0x0000FF00;
            for (LONG y = 8; y < 16; ++y)
            {
                for (LONG x = 8; x < 16; ++x)
                {
                    framebuffer[y * framebufferWidth + x] = updateColor;
                }

                for (LONG x = 24; x < 32; ++x)
                {
                    framebuffer[y * framebufferWidth + x] = inputColor;
                }
            }

            InvalidateRect(window, nullptr, FALSE);
        }
        else
        {
            Sleep(1);
        }
    }

    timeEndPeriod(1);
    waveOutReset(audioOutput);
    waveOutUnprepareHeader(audioOutput, &toneHeader, sizeof(toneHeader));
    waveOutClose(audioOutput);
    return static_cast<int>(message.wParam);
}
