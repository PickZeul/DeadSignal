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
constexpr float playerHalfWidth = playerWidth / 2.0f;
constexpr float playerHalfHeight = playerHeight / 2.0f;
constexpr LONG wallLeft = 170;
constexpr LONG wallTop = 60;
constexpr LONG wallWidth = 8;
constexpr LONG wallHeight = 60;
constexpr LONG wallRight = wallLeft + wallWidth;
constexpr LONG wallBottom = wallTop + wallHeight;
constexpr LONG enemyWidth = 8;
constexpr LONG enemyHeight = 12;
constexpr LONG enemyCenterX = 120;
constexpr LONG enemyCenterY = 90;
constexpr LONG enemyLeft = enemyCenterX - enemyWidth / 2;
constexpr LONG enemyTop = enemyCenterY - enemyHeight / 2;
constexpr LONG enemyVisionRange = 70;
constexpr float enemyVisionSlope = 0.520567f;
constexpr float detectionFillDuration = 3.0f;
constexpr float lostSightHoldDuration = 0.5f;
constexpr LONG slashReach = 8;
constexpr LONG slashWidth = 8;
constexpr float slashVisualDuration = 0.10f;
constexpr float slashCooldownDuration = 0.5f;
constexpr DWORD toneSampleRate = 8000;
constexpr DWORD toneFrequency = 440;
constexpr DWORD toneDurationMilliseconds = 250;
constexpr DWORD toneSampleCount = toneSampleRate * toneDurationMilliseconds / 1000;

DWORD framebuffer[framebufferWidth * framebufferHeight];

bool wPressed = false;
bool sPressed = false;
bool aPressed = false;
bool dPressed = false;
bool jPressed = false;
bool slashRequested = false;
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
    if ((message == WM_KEYDOWN || message == WM_KEYUP) && wParam == 'J')
    {
        bool pressed = message == WM_KEYDOWN;
        if (pressed && !jPressed)
        {
            slashRequested = true;
        }

        jPressed = pressed;
        return 0;
    }

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

    for (LONG x = 1; x <= enemyVisionRange; ++x)
    {
        LONG visionHalfHeight = static_cast<LONG>(x * enemyVisionSlope);
        for (LONG y = -visionHalfHeight; y <= visionHalfHeight; ++y)
        {
            LONG pixelX = enemyCenterX + x;
            LONG pixelY = enemyCenterY + y;
            bool visionBlocked = false;
            if (pixelX > wallLeft)
            {
                float wallAmount = static_cast<float>(wallLeft - enemyCenterX) / static_cast<float>(x);
                float yAtWall = static_cast<float>(enemyCenterY) + static_cast<float>(y) * wallAmount;
                visionBlocked = yAtWall >= wallTop && yAtWall < wallBottom;
            }

            if (!visionBlocked && pixelY >= 0 && pixelY < framebufferHeight)
            {
                framebuffer[pixelY * framebufferWidth + pixelX] = 0x00182040;
            }
        }
    }

    for (LONG y = wallTop; y < wallBottom; ++y)
    {
        for (LONG x = wallLeft; x < wallRight; ++x)
        {
            framebuffer[y * framebufferWidth + x] = 0x00606070;
        }
    }

    for (LONG y = 0; y < enemyHeight; ++y)
    {
        for (LONG x = 0; x < enemyWidth; ++x)
        {
            bool head = y < 3 && x >= 1 && x < 7;
            bool body = y >= 3 && y < 9 && x >= 2 && x < 6;
            bool arm = y >= 4 && y < 8 && (x == 0 || x == 7);
            bool leg = y >= 9 && (x < 3 || x >= 5);
            if (head || body || arm || leg)
            {
                framebuffer[(enemyTop + y) * framebufferWidth + enemyLeft + x]
                    = head ? 0x00FF4040 : 0x00A02020;
            }
        }
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

    for (LONG y = 8; y < 12; ++y)
    {
        for (LONG x = 24; x < 28; ++x)
        {
            framebuffer[y * framebufferWidth + x] = 0x0000FF80;
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
    LONG facingX = 1;
    LONG facingY = 0;
    LONG slashLeft = 0;
    LONG slashTop = 0;
    LONG slashRight = 0;
    LONG slashBottom = 0;
    float slashVisualRemaining = 0.0f;
    float slashCooldownRemaining = 0.0f;
    float enemyHitRemaining = 0.0f;
    float detectionProgress = 0.0f;
    float lostSightElapsed = 0.0f;
    bool enemyAlert = false;
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
            if (movementX || movementY)
            {
                facingX = movementX;
                facingY = movementY;
            }
            float movementScale = movementX && movementY ? 0.70710678f : 1.0f;
            float movementDeltaX = movementX * playerMoveSpeed * movementScale * deltaTime;
            float movementDeltaY = movementY * playerMoveSpeed * movementScale * deltaTime;

            float nextPlayerX = playerX + movementDeltaX;
            if (nextPlayerX < playerHalfWidth)
            {
                nextPlayerX = playerHalfWidth;
            }
            else if (nextPlayerX > framebufferWidth - playerHalfWidth)
            {
                nextPlayerX = framebufferWidth - playerHalfWidth;
            }

            if (movementDeltaX != 0.0f
                && nextPlayerX - playerHalfWidth < wallRight
                && nextPlayerX + playerHalfWidth > wallLeft
                && playerY - playerHalfHeight < wallBottom
                && playerY + playerHalfHeight > wallTop)
            {
                nextPlayerX = movementDeltaX > 0.0f
                    ? wallLeft - playerHalfWidth
                    : wallRight + playerHalfWidth;
            }
            playerX = nextPlayerX;

            float nextPlayerY = playerY + movementDeltaY;
            if (nextPlayerY < playerHalfHeight)
            {
                nextPlayerY = playerHalfHeight;
            }
            else if (nextPlayerY > framebufferHeight - playerHalfHeight)
            {
                nextPlayerY = framebufferHeight - playerHalfHeight;
            }

            if (movementDeltaY != 0.0f
                && playerX - playerHalfWidth < wallRight
                && playerX + playerHalfWidth > wallLeft
                && nextPlayerY - playerHalfHeight < wallBottom
                && nextPlayerY + playerHalfHeight > wallTop)
            {
                nextPlayerY = movementDeltaY > 0.0f
                    ? wallTop - playerHalfHeight
                    : wallBottom + playerHalfHeight;
            }
            playerY = nextPlayerY;

            if (slashVisualRemaining > 0.0f)
            {
                slashVisualRemaining -= deltaTime;
            }
            if (slashCooldownRemaining > 0.0f)
            {
                slashCooldownRemaining -= deltaTime;
            }
            if (enemyHitRemaining > 0.0f)
            {
                enemyHitRemaining -= deltaTime;
            }

            if (slashRequested)
            {
                if (slashCooldownRemaining <= 0.0f)
                {
                    LONG slashCenterX = static_cast<LONG>(playerX);
                    LONG slashCenterY = static_cast<LONG>(playerY);
                    if (facingX < 0)
                    {
                        slashRight = slashCenterX - playerWidth / 2;
                        slashLeft = slashRight - slashReach;
                    }
                    else if (facingX > 0)
                    {
                        slashLeft = slashCenterX + playerWidth / 2;
                        slashRight = slashLeft + slashReach;
                    }
                    else
                    {
                        slashLeft = slashCenterX - slashWidth / 2;
                        slashRight = slashLeft + slashWidth;
                    }

                    if (facingY < 0)
                    {
                        slashBottom = slashCenterY - playerHeight / 2;
                        slashTop = slashBottom - slashReach;
                    }
                    else if (facingY > 0)
                    {
                        slashTop = slashCenterY + playerHeight / 2;
                        slashBottom = slashTop + slashReach;
                    }
                    else
                    {
                        slashTop = slashCenterY - slashWidth / 2;
                        slashBottom = slashTop + slashWidth;
                    }

                    slashVisualRemaining = slashVisualDuration;
                    slashCooldownRemaining = slashCooldownDuration;
                    if (slashLeft < enemyLeft + enemyWidth
                        && slashRight > enemyLeft
                        && slashTop < enemyTop + enemyHeight
                        && slashBottom > enemyTop)
                    {
                        enemyHitRemaining = slashVisualDuration;
                    }
                }
                slashRequested = false;
            }

            float visionX = playerX - enemyCenterX;
            float visionY = playerY - enemyCenterY;
            float visionDistanceY = visionY;
            if (visionDistanceY < 0.0f)
            {
                visionDistanceY = -visionDistanceY;
            }
            bool playerInRawVision = visionX > 0.0f
                && visionX <= enemyVisionRange
                && visionDistanceY <= visionX * enemyVisionSlope;
            bool playerVisionOccluded = false;
            if (playerInRawVision && playerX > wallLeft)
            {
                float wallAmount = static_cast<float>(wallLeft - enemyCenterX) / visionX;
                float yAtWall = enemyCenterY + visionY * wallAmount;
                playerVisionOccluded = yAtWall >= wallTop && yAtWall < wallBottom;
            }
            bool playerInVision = playerInRawVision && !playerVisionOccluded;
            if (playerInVision)
            {
                lostSightElapsed = 0.0f;
                if (enemyAlert)
                {
                    detectionProgress = 1.0f;
                }
                else
                {
                    detectionProgress += deltaTime / detectionFillDuration;
                    if (detectionProgress >= 1.0f)
                    {
                        detectionProgress = 1.0f;
                        enemyAlert = true;
                    }
                }
            }
            else if (detectionProgress > 0.0f)
            {
                float decayTime = lostSightElapsed + deltaTime - lostSightHoldDuration;
                lostSightElapsed += deltaTime;
                if (decayTime > 0.0f)
                {
                    enemyAlert = false;
                    if (decayTime > deltaTime)
                    {
                        decayTime = deltaTime;
                    }

                    detectionProgress -= decayTime / detectionFillDuration;
                    if (detectionProgress <= 0.0f)
                    {
                        detectionProgress = 0.0f;
                        lostSightElapsed = 0.0f;
                    }
                }
            }
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

            float redDistance = enemyVisionRange * detectionProgress;
            for (LONG x = 1; x <= enemyVisionRange; ++x)
            {
                LONG visionHalfHeight = static_cast<LONG>(x * enemyVisionSlope);
                for (LONG y = -visionHalfHeight; y <= visionHalfHeight; ++y)
                {
                    LONG pixelX = enemyCenterX + x;
                    LONG pixelY = enemyCenterY + y;
                    bool visionBlocked = false;
                    if (pixelX > wallLeft)
                    {
                        float wallAmount = static_cast<float>(wallLeft - enemyCenterX) / static_cast<float>(x);
                        float yAtWall = static_cast<float>(enemyCenterY) + static_cast<float>(y) * wallAmount;
                        visionBlocked = yAtWall >= wallTop && yAtWall < wallBottom;
                    }

                    if (!visionBlocked && pixelY >= 0 && pixelY < framebufferHeight)
                    {
                        framebuffer[pixelY * framebufferWidth + pixelX]
                            = x <= redDistance ? 0x00401818 : 0x00182040;
                    }
                }
            }

            for (LONG y = wallTop; y < wallBottom; ++y)
            {
                for (LONG x = wallLeft; x < wallRight; ++x)
                {
                    framebuffer[y * framebufferWidth + x] = 0x00606070;
                }
            }

            if (slashVisualRemaining > 0.0f)
            {
                for (LONG y = slashTop; y < slashBottom; ++y)
                {
                    for (LONG x = slashLeft; x < slashRight; ++x)
                    {
                        if (x >= 0 && x < framebufferWidth && y >= 0 && y < framebufferHeight
                            && (x == slashLeft || x == slashRight - 1
                                || y == slashTop || y == slashBottom - 1))
                        {
                            framebuffer[y * framebufferWidth + x] = 0x00E0E0E0;
                        }
                    }
                }
            }

            for (LONG y = 0; y < enemyHeight; ++y)
            {
                for (LONG x = 0; x < enemyWidth; ++x)
                {
                    bool head = y < 3 && x >= 1 && x < 7;
                    bool body = y >= 3 && y < 9 && x >= 2 && x < 6;
                    bool arm = y >= 4 && y < 8 && (x == 0 || x == 7);
                    bool leg = y >= 9 && (x < 3 || x >= 5);
                    if (head || body || arm || leg)
                    {
                        framebuffer[(enemyTop + y) * framebufferWidth + enemyLeft + x]
                            = enemyHitRemaining > 0.0f
                                ? 0x00FFFFFF
                                : (head ? 0x00FF4040 : 0x00A02020);
                    }
                }
            }

            if (detectionProgress > 0.0f)
            {
                for (LONG y = 0; y < 5; ++y)
                {
                    for (LONG x = 0; x < 3; ++x)
                    {
                        bool symbolPixel = enemyAlert
                            ? (x == 1 && y != 3)
                            : ((y == 0 && x < 2) || (y == 1 && x == 2)
                                || (y == 2 && x == 1) || (y == 4 && x == 1));
                        if (symbolPixel)
                        {
                            framebuffer[(enemyTop - 7 + y) * framebufferWidth + enemyCenterX - 1 + x]
                                = enemyAlert ? 0x00FF4040 : 0x00FFD800;
                        }
                    }
                }
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

            DWORD visionMarkerColor = playerInVision ? 0x0000FF80 : 0x00202028;
            for (LONG y = 8; y < 12; ++y)
            {
                for (LONG x = 24; x < 28; ++x)
                {
                    framebuffer[y * framebufferWidth + x] = visionMarkerColor;
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
