#include <windows.h>
#include <timeapi.h>

#pragma comment(lib, "winmm.lib")

constexpr LONG framebufferWidth = 320;
constexpr LONG framebufferHeight = 180;
constexpr LONG playerWidth = 8;
constexpr LONG playerHeight = 12;
constexpr LONG playerCenterX = framebufferWidth / 2;
constexpr LONG playerCenterY = framebufferHeight / 2;
constexpr LONG playerLeft = playerCenterX - playerWidth / 2;
constexpr LONG playerTop = playerCenterY - playerHeight / 2;
constexpr float playerMoveSpeed = 60.0f;
constexpr DWORD toneSampleRate = 8000;
constexpr DWORD toneFrequency = 440;
constexpr DWORD toneDurationMilliseconds = 250;
constexpr DWORD toneSampleCount = toneSampleRate * toneDurationMilliseconds / 1000;

DWORD framebuffer[framebufferWidth * framebufferHeight];

bool wPressed = false;
bool sPressed = false;
bool aPressed = false;
bool dPressed = false;
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

    if ((message == WM_KEYDOWN || message == WM_KEYUP)
        && (wParam == 'W' || wParam == 'S' || wParam == 'A' || wParam == 'D'))
    {
        bool pressed = message == WM_KEYDOWN;
        if (wParam == 'W')
        {
            wPressed = pressed;
        }
        else if (wParam == 'S')
        {
            sPressed = pressed;
        }
        else if (wParam == 'A')
        {
            aPressed = pressed;
        }
        else
        {
            dPressed = pressed;
        }

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
    }

    for (LONG y = 0; y < playerHeight; ++y)
    {
        for (LONG x = 0; x < playerWidth; ++x)
        {
            bool head = y < 3 && x >= 2 && x < 6;
            bool body = y >= 3 && y < 8 && x >= 1 && x < 7;
            bool arm = y >= 4 && y < 7 && (x == 0 || x == 7);
            bool leg = y >= 8 && ((x >= 1 && x < 3) || (x >= 5 && x < 7));
            LONG pixelX = playerLeft + x;
            LONG pixelY = playerTop + y;
            if ((head || body || arm || leg)
                && pixelX >= 0 && pixelX < framebufferWidth
                && pixelY >= 0 && pixelY < framebufferHeight)
            {
                framebuffer[pixelY * framebufferWidth + pixelX]
                    = head ? 0x00FFFFFF : 0x0000A0FF;
            }
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
    float playerX = static_cast<float>(playerCenterX);
    float playerY = static_cast<float>(playerCenterY);
    DWORD updateColor = 0x00FF0000;

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
            float deltaTime = static_cast<float>(currentTime.QuadPart - previousUpdate.QuadPart)
                / static_cast<float>(performanceFrequency.QuadPart);
            previousUpdate = currentTime;
            LONG movementX = static_cast<LONG>(dPressed) - static_cast<LONG>(aPressed);
            LONG movementY = static_cast<LONG>(sPressed) - static_cast<LONG>(wPressed);
            float movementScale = movementX && movementY ? 0.70710678f : 1.0f;
            playerX += movementX * playerMoveSpeed * movementScale * deltaTime;
            playerY += movementY * playerMoveSpeed * movementScale * deltaTime;
            updateColor = (updateColor + 0x00050000) & 0x00FF0000;

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
                    framebuffer[y * framebufferWidth + x] = updateColor;
                }
            }

            LONG drawingLeft = static_cast<LONG>(playerX) - playerWidth / 2;
            LONG drawingTop = static_cast<LONG>(playerY) - playerHeight / 2;
            for (LONG y = 0; y < playerHeight; ++y)
            {
                for (LONG x = 0; x < playerWidth; ++x)
                {
                    bool head = y < 3 && x >= 2 && x < 6;
                    bool body = y >= 3 && y < 8 && x >= 1 && x < 7;
                    bool arm = y >= 4 && y < 7 && (x == 0 || x == 7);
                    bool leg = y >= 8 && ((x >= 1 && x < 3) || (x >= 5 && x < 7));
                    LONG pixelX = drawingLeft + x;
                    LONG pixelY = drawingTop + y;
                    if ((head || body || arm || leg)
                        && pixelX >= 0 && pixelX < framebufferWidth
                        && pixelY >= 0 && pixelY < framebufferHeight)
                    {
                        framebuffer[pixelY * framebufferWidth + pixelX]
                            = head ? 0x00FFFFFF : 0x0000A0FF;
                    }
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
