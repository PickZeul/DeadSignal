#include <windows.h>
#include <timeapi.h>
#include <math.h>

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
constexpr LONG exitLeft = 304;
constexpr LONG exitTop = 78;
constexpr LONG exitRight = 320;
constexpr LONG exitBottom = 102;
constexpr LONG enemyWidth = 8;
constexpr LONG enemyHeight = 12;
constexpr float enemyInitialX = 120.0f;
constexpr float enemyInitialY = 90.0f;
constexpr LONG enemyInitialCenterX = 120;
constexpr LONG enemyInitialCenterY = 90;
constexpr LONG enemyInitialLeft = enemyInitialCenterX - enemyWidth / 2;
constexpr LONG enemyInitialTop = enemyInitialCenterY - enemyHeight / 2;
constexpr float enemyHalfWidth = enemyWidth / 2.0f;
constexpr float enemyHalfHeight = enemyHeight / 2.0f;
constexpr float enemyPatrolLeftPoint = 80.0f;
constexpr float enemyPatrolRightPoint = 140.0f;
constexpr float enemyPatrolSpeed = 24.0f;
constexpr float enemyAlertSpeed = 58.0f;
constexpr LONG enemyVisionRange = 70;
constexpr float enemyVisionSlope = 0.520567f;
constexpr float enemyReacquireRangeSquared = 42.0f * 42.0f;
constexpr float enemyFacingTurnSpeed = 2.0943951f;
constexpr float enemyScanAngle = 0.47996554f;
constexpr float enemyAlertSearchDuration = 10.0f;
constexpr float enemyScanDuration = 2.0f;
constexpr float enemyAttackCooldownDuration = 1.0f;
constexpr float enemyAttackContactTolerance = 1.0f;
constexpr float playerHitFeedbackDuration = 0.10f;
constexpr float detectionFillDuration = 3.0f;
constexpr float lostSightHoldDuration = 0.5f;
constexpr LONG navigationCellSize = 8;
constexpr LONG navigationColumns = 40;
constexpr LONG navigationRows = 22;
constexpr LONG navigationNodeCount = navigationColumns * navigationRows;
constexpr LONG slashReach = 8;
constexpr LONG slashWidth = 8;
constexpr float slashVisualDuration = 0.10f;
constexpr float slashCooldownDuration = 0.5f;
constexpr LONG executeReach = 4;
constexpr float dashDistance = 32.0f;
constexpr float dashDuration = 0.12f;
constexpr float dashSpeed = dashDistance / dashDuration;
constexpr float dashCooldownDuration = 1.0f;
constexpr DWORD toneSampleRate = 8000;
constexpr DWORD toneFrequency = 440;
constexpr DWORD toneDurationMilliseconds = 250;
constexpr DWORD toneSampleCount = toneSampleRate * toneDurationMilliseconds / 1000;

DWORD framebuffer[framebufferWidth * framebufferHeight];

constexpr LONG titleMainState = 0;
constexpr LONG gameStartMenuState = 1;
constexpr LONG gameplayState = 2;
LONG applicationState = titleMainState;
LONG menuSelection = 0;
LONG titleStatus = 0;
bool newGameRequested = false;
bool gameplayInputBlocked = false;
bool upPressed = false;
bool downPressed = false;
bool leftPressed = false;
bool rightPressed = false;
bool zPressed = false;
bool slashRequested = false;
bool xPressed = false;
bool dashRequested = false;
bool cPressed = false;
bool executeRequested = false;
bool spacePressed = false;
bool tonePlaying = false;
BYTE toneSamples[toneSampleCount];
HWAVEOUT audioOutput = nullptr;
WAVEHDR toneHeader{};
short navigationParent[navigationNodeCount];
unsigned short navigationScore[navigationNodeCount];
BYTE navigationState[navigationNodeCount];
unsigned short navigationPath[navigationNodeCount];

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

bool WallBlocksSegment(float startX, float startY, float endX, float endY)
{
    float enter = 0.0f;
    float exit = 1.0f;
    float difference = endX - startX;
    if (difference == 0.0f)
    {
        if (startX < wallLeft || startX >= wallRight)
        {
            return false;
        }
    }
    else
    {
        float first = (wallLeft - startX) / difference;
        float second = (wallRight - startX) / difference;
        if (first > second)
        {
            float swap = first;
            first = second;
            second = swap;
        }
        if (first > enter)
        {
            enter = first;
        }
        if (second < exit)
        {
            exit = second;
        }
        if (enter > exit)
        {
            return false;
        }
    }

    difference = endY - startY;
    if (difference == 0.0f)
    {
        if (startY < wallTop || startY >= wallBottom)
        {
            return false;
        }
    }
    else
    {
        float first = (wallTop - startY) / difference;
        float second = (wallBottom - startY) / difference;
        if (first > second)
        {
            float swap = first;
            first = second;
            second = swap;
        }
        if (first > enter)
        {
            enter = first;
        }
        if (second < exit)
        {
            exit = second;
        }
    }

    return enter <= exit && exit >= 0.0f && enter <= 1.0f;
}

float TurnToward(float angle, float target, float amount)
{
    constexpr float pi = 3.14159265f;
    constexpr float twoPi = pi * 2.0f;
    float difference = target - angle;
    while (difference > pi)
    {
        difference -= twoPi;
    }
    while (difference < -pi)
    {
        difference += twoPi;
    }
    if (difference > amount)
    {
        difference = amount;
    }
    else if (difference < -amount)
    {
        difference = -amount;
    }
    return angle + difference;
}

bool NavigationCellValid(LONG column, LONG row)
{
    if (column < 0 || column >= navigationColumns || row < 0 || row >= navigationRows)
    {
        return false;
    }
    float centerX = enemyHalfWidth + column * navigationCellSize;
    float centerY = enemyHalfHeight + row * navigationCellSize;
    return !(centerX - enemyHalfWidth < wallRight
        && centerX + enemyHalfWidth > wallLeft
        && centerY - enemyHalfHeight < wallBottom
        && centerY + enemyHalfHeight > wallTop);
}

LONG NavigationColumn(float x)
{
    LONG column = static_cast<LONG>((x - enemyHalfWidth + navigationCellSize * 0.5f)
        / navigationCellSize);
    if (column < 0)
    {
        return 0;
    }
    if (column >= navigationColumns)
    {
        return navigationColumns - 1;
    }
    return column;
}

LONG NavigationRow(float y)
{
    LONG row = static_cast<LONG>((y - enemyHalfHeight + navigationCellSize * 0.5f)
        / navigationCellSize);
    if (row < 0)
    {
        return 0;
    }
    if (row >= navigationRows)
    {
        return navigationRows - 1;
    }
    return row;
}

LONG FindEnemyPath(float startX, float startY, LONG targetColumn, LONG targetRow)
{
    LONG startColumn = NavigationColumn(startX);
    LONG startRow = NavigationRow(startY);
    LONG start = startRow * navigationColumns + startColumn;
    LONG target = targetRow * navigationColumns + targetColumn;
    for (LONG node = 0; node < navigationNodeCount; ++node)
    {
        navigationParent[node] = -1;
        navigationScore[node] = 0xFFFF;
        navigationState[node] = 0;
    }
    navigationScore[start] = 0;
    navigationState[start] = 1;

    for (;;)
    {
        LONG current = -1;
        LONG best = 0x7FFFFFFF;
        for (LONG node = 0; node < navigationNodeCount; ++node)
        {
            if (navigationState[node] == 1)
            {
                LONG column = node % navigationColumns;
                LONG row = node / navigationColumns;
                LONG heuristic = column > targetColumn
                    ? column - targetColumn : targetColumn - column;
                heuristic += row > targetRow ? row - targetRow : targetRow - row;
                LONG estimate = navigationScore[node] + heuristic;
                if (estimate < best)
                {
                    best = estimate;
                    current = node;
                }
            }
        }
        if (current < 0)
        {
            return 0;
        }
        if (current == target)
        {
            LONG count = 0;
            while (current != start && count < navigationNodeCount)
            {
                navigationPath[count++] = static_cast<unsigned short>(current);
                current = navigationParent[current];
            }
            for (LONG index = 0; index < count / 2; ++index)
            {
                unsigned short swap = navigationPath[index];
                navigationPath[index] = navigationPath[count - index - 1];
                navigationPath[count - index - 1] = swap;
            }
            return count;
        }

        navigationState[current] = 2;
        LONG currentColumn = current % navigationColumns;
        LONG currentRow = current / navigationColumns;
        constexpr LONG neighborX[4] = { -1, 1, 0, 0 };
        constexpr LONG neighborY[4] = { 0, 0, -1, 1 };
        for (LONG neighbor = 0; neighbor < 4; ++neighbor)
        {
            LONG column = currentColumn + neighborX[neighbor];
            LONG row = currentRow + neighborY[neighbor];
            if (!NavigationCellValid(column, row))
            {
                continue;
            }
            LONG next = row * navigationColumns + column;
            unsigned short score = navigationScore[current] + 1;
            if (score < navigationScore[next])
            {
                navigationScore[next] = score;
                navigationParent[next] = static_cast<short>(current);
                navigationState[next] = 1;
            }
        }
    }
}

bool MoveEnemyToward(float& enemyX, float& enemyY, float targetX, float targetY,
    float speed, float deltaTime, float playerX, float playerY)
{
    float differenceX = targetX - enemyX;
    float differenceY = targetY - enemyY;
    float distance = sqrtf(differenceX * differenceX + differenceY * differenceY);
    if (distance < 0.01f)
    {
        return true;
    }
    float movement = speed * deltaTime;
    if (movement > distance)
    {
        movement = distance;
    }
    float movementDeltaX = differenceX / distance * movement;
    float movementDeltaY = differenceY / distance * movement;
    float largestDelta = movementDeltaX < 0.0f ? -movementDeltaX : movementDeltaX;
    float absoluteY = movementDeltaY < 0.0f ? -movementDeltaY : movementDeltaY;
    if (absoluteY > largestDelta)
    {
        largestDelta = absoluteY;
    }
    LONG movementSteps = static_cast<LONG>(largestDelta) + 1;
    movementDeltaX /= movementSteps;
    movementDeltaY /= movementSteps;
    for (LONG step = 0; step < movementSteps; ++step)
    {
        float nextX = enemyX + movementDeltaX;
        if (nextX < enemyHalfWidth)
        {
            nextX = enemyHalfWidth;
        }
        else if (nextX > framebufferWidth - enemyHalfWidth)
        {
            nextX = framebufferWidth - enemyHalfWidth;
        }
        if (movementDeltaX != 0.0f
            && nextX - enemyHalfWidth < wallRight
            && nextX + enemyHalfWidth > wallLeft
            && enemyY - enemyHalfHeight < wallBottom
            && enemyY + enemyHalfHeight > wallTop)
        {
            nextX = movementDeltaX > 0.0f
                ? wallLeft - enemyHalfWidth : wallRight + enemyHalfWidth;
        }
        if (movementDeltaX != 0.0f
            && nextX - enemyHalfWidth < playerX + playerHalfWidth
            && nextX + enemyHalfWidth > playerX - playerHalfWidth
            && enemyY - enemyHalfHeight < playerY + playerHalfHeight
            && enemyY + enemyHalfHeight > playerY - playerHalfHeight)
        {
            nextX = enemyX;
        }
        enemyX = nextX;

        float nextY = enemyY + movementDeltaY;
        if (nextY < enemyHalfHeight)
        {
            nextY = enemyHalfHeight;
        }
        else if (nextY > framebufferHeight - enemyHalfHeight)
        {
            nextY = framebufferHeight - enemyHalfHeight;
        }
        if (movementDeltaY != 0.0f
            && enemyX - enemyHalfWidth < wallRight
            && enemyX + enemyHalfWidth > wallLeft
            && nextY - enemyHalfHeight < wallBottom
            && nextY + enemyHalfHeight > wallTop)
        {
            nextY = movementDeltaY > 0.0f
                ? wallTop - enemyHalfHeight : wallBottom + enemyHalfHeight;
        }
        if (movementDeltaY != 0.0f
            && enemyX - enemyHalfWidth < playerX + playerHalfWidth
            && enemyX + enemyHalfWidth > playerX - playerHalfWidth
            && nextY - enemyHalfHeight < playerY + playerHalfHeight
            && nextY + enemyHalfHeight > playerY - playerHalfHeight)
        {
            nextY = enemyY;
        }
        enemyY = nextY;
    }
    differenceX = targetX - enemyX;
    differenceY = targetY - enemyY;
    return differenceX * differenceX + differenceY * differenceY < 0.25f;
}

LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if ((message == WM_KEYDOWN || message == WM_KEYUP) && wParam == 'C')
    {
        bool pressed = message == WM_KEYDOWN;
        if (applicationState == gameplayState && !gameplayInputBlocked
            && pressed && !cPressed)
        {
            executeRequested = true;
        }

        cPressed = pressed;
        if (gameplayInputBlocked && !upPressed && !downPressed && !leftPressed && !rightPressed
            && !zPressed && !xPressed && !cPressed && !spacePressed)
        {
            gameplayInputBlocked = false;
        }
        return 0;
    }

    if ((message == WM_KEYDOWN || message == WM_KEYUP) && wParam == 'X')
    {
        bool pressed = message == WM_KEYDOWN;
        if (applicationState == gameplayState && !gameplayInputBlocked
            && pressed && !xPressed)
        {
            dashRequested = true;
        }

        xPressed = pressed;
        if (gameplayInputBlocked && !upPressed && !downPressed && !leftPressed && !rightPressed
            && !zPressed && !xPressed && !cPressed && !spacePressed)
        {
            gameplayInputBlocked = false;
        }
        return 0;
    }

    if ((message == WM_KEYDOWN || message == WM_KEYUP) && wParam == 'Z')
    {
        bool pressed = message == WM_KEYDOWN;
        if (applicationState == gameplayState)
        {
            if (!gameplayInputBlocked && pressed && !zPressed)
            {
                slashRequested = true;
            }
        }
        else if (pressed && !zPressed)
        {
            titleStatus = 0;
            if (applicationState == titleMainState)
            {
                if (menuSelection == 0)
                {
                    applicationState = gameStartMenuState;
                    menuSelection = 0;
                }
                else if (menuSelection == 1)
                {
                    titleStatus = 1;
                }
                else if (menuSelection == 2)
                {
                    titleStatus = 2;
                }
                else
                {
                    DestroyWindow(window);
                }
            }
            else if (menuSelection == 0)
            {
                applicationState = gameplayState;
                newGameRequested = true;
            }
            else if (menuSelection == 1)
            {
                titleStatus = 1;
            }
            else
            {
                applicationState = titleMainState;
                menuSelection = 0;
            }

            InvalidateRect(window, nullptr, FALSE);
        }

        zPressed = pressed;
        if (gameplayInputBlocked && !upPressed && !downPressed && !leftPressed && !rightPressed
            && !zPressed && !xPressed && !cPressed && !spacePressed)
        {
            gameplayInputBlocked = false;
        }
        return 0;
    }

    if ((message == WM_KEYDOWN || message == WM_KEYUP) && wParam == VK_SPACE)
    {
        bool pressed = message == WM_KEYDOWN;
        if (applicationState == gameplayState && !gameplayInputBlocked
            && pressed && !spacePressed && !tonePlaying
            && waveOutWrite(audioOutput, &toneHeader, sizeof(toneHeader)) == MMSYSERR_NOERROR)
        {
            tonePlaying = true;
        }

        spacePressed = pressed;
        if (gameplayInputBlocked && !upPressed && !downPressed && !leftPressed && !rightPressed
            && !zPressed && !xPressed && !cPressed && !spacePressed)
        {
            gameplayInputBlocked = false;
        }
        return 0;
    }

    if ((message == WM_KEYDOWN || message == WM_KEYUP)
        && (wParam == VK_UP || wParam == VK_DOWN || wParam == VK_LEFT || wParam == VK_RIGHT))
    {
        bool pressed = message == WM_KEYDOWN;
        bool newlyPressed = pressed;
        if (wParam == VK_UP)
        {
            newlyPressed = pressed && !upPressed;
            upPressed = pressed;
        }
        else if (wParam == VK_DOWN)
        {
            newlyPressed = pressed && !downPressed;
            downPressed = pressed;
        }
        else if (wParam == VK_LEFT)
        {
            newlyPressed = pressed && !leftPressed;
            leftPressed = pressed;
        }
        else
        {
            newlyPressed = pressed && !rightPressed;
            rightPressed = pressed;
        }

        if (applicationState != gameplayState && newlyPressed
            && (wParam == VK_UP || wParam == VK_DOWN))
        {
            if (wParam == VK_UP && menuSelection > 0)
            {
                --menuSelection;
            }
            else if (wParam == VK_DOWN
                && menuSelection < (applicationState == titleMainState ? 3 : 2))
            {
                ++menuSelection;
            }
            titleStatus = 0;
            InvalidateRect(window, nullptr, FALSE);
        }
        else if (gameplayInputBlocked && !upPressed && !downPressed
            && !leftPressed && !rightPressed && !zPressed && !xPressed
            && !cPressed && !spacePressed)
        {
            gameplayInputBlocked = false;
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

        if (applicationState != gameplayState)
        {
            SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
            SetBkMode(deviceContext, TRANSPARENT);
            SetTextColor(deviceContext, RGB(220, 220, 220));
            int clientHeight = clientArea.bottom - clientArea.top;
            RECT line = clientArea;
            line.top = clientHeight / 5;
            line.bottom = line.top + 30;
            DrawTextW(deviceContext, L"DEAD SIGNAL", -1, &line,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            for (LONG item = 0; item < (applicationState == titleMainState ? 4 : 3); ++item)
            {
                const wchar_t* text;
                if (applicationState == titleMainState)
                {
                    text = item == 0 ? L"\uAC8C\uC784 \uC2DC\uC791"
                        : (item == 1 ? L"\uC774\uC5B4\uD558\uAE30"
                            : (item == 2 ? L"\uC124\uC815" : L"\uC885\uB8CC"));
                }
                else
                {
                    text = item == 0 ? L"\uC0C8 \uAC8C\uC784"
                        : (item == 1 ? L"\uBD88\uB7EC\uC624\uAE30" : L"\uB4A4\uB85C");
                }

                line.top = clientHeight / 5 + 50 + item * 30;
                line.bottom = line.top + 24;
                SetTextColor(deviceContext,
                    item == menuSelection ? RGB(255, 216, 0) : RGB(160, 160, 160));
                DrawTextW(deviceContext, text, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }

            if (titleStatus)
            {
                line.top = clientHeight / 5 + 190;
                line.bottom = line.top + 24;
                SetTextColor(deviceContext, RGB(220, 220, 220));
                DrawTextW(deviceContext,
                    titleStatus == 1
                        ? L"\uC800\uC7A5 \uAE30\uB2A5 \uBBF8\uAD6C\uD604"
                        : L"\uC124\uC815 \uC900\uBE44 \uC911",
                    -1, &line, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }

            EndPaint(window, &paint);
            return 0;
        }

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
            LONG pixelX = enemyInitialCenterX + x;
            LONG pixelY = enemyInitialCenterY + y;
            bool visionBlocked = false;
            if (pixelX > wallLeft)
            {
                float wallAmount = static_cast<float>(wallLeft - enemyInitialCenterX)
                    / static_cast<float>(x);
                float yAtWall = static_cast<float>(enemyInitialCenterY)
                    + static_cast<float>(y) * wallAmount;
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
                framebuffer[(enemyInitialTop + y) * framebufferWidth + enemyInitialLeft + x]
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
    float enemyX = enemyInitialX;
    float enemyY = enemyInitialY;
    float enemyFacingAngle = 0.0f;
    bool enemyPatrolRight = true;
    bool enemyReturningToPatrol = false;
    float enemyPatrolReturnX = enemyInitialX;
    bool playerInVision = false;
    float lastSeenPlayerX = 0.0f;
    float lastSeenPlayerY = 0.0f;
    bool lastSeenPlayerValid = false;
    float alertLostElapsed = 0.0f;
    DWORD searchRandomState = 0x13579BDF;
    float searchTargetX = 0.0f;
    float searchTargetY = 0.0f;
    bool searchTargetValid = false;
    LONG searchPathCount = 0;
    LONG searchPathIndex = 0;
    bool enemyScanning = false;
    float scanElapsed = 0.0f;
    float scanBaseFacing = 0.0f;
    LONG playerHP = 10;
    bool playerAlive = true;
    LONG enemyHP = 3;
    float enemyAttackCooldownRemaining = 0.0f;
    float playerHitRemaining = 0.0f;
    LONG dashDirectionX = 0;
    LONG dashDirectionY = 0;
    float dashDistanceRemaining = 0.0f;
    float dashCooldownRemaining = 0.0f;
    bool dashActive = false;
    LONG slashLeft = 0;
    LONG slashTop = 0;
    LONG slashRight = 0;
    LONG slashBottom = 0;
    float slashVisualRemaining = 0.0f;
    float slashCooldownRemaining = 0.0f;
    float enemyHitRemaining = 0.0f;
    float executeFeedbackRemaining = 0.0f;
    float detectionProgress = 0.0f;
    float lostSightElapsed = 0.0f;
    bool enemyAlert = false;
    bool enemyAlive = true;
    bool exitUnlocked = false;
    bool roomComplete = false;
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

        if (newGameRequested)
        {
            playerX = static_cast<float>(playerCenterX);
            playerY = static_cast<float>(playerCenterY);
            facingX = 1;
            facingY = 0;
            enemyX = enemyInitialX;
            enemyY = enemyInitialY;
            enemyFacingAngle = 0.0f;
            enemyPatrolRight = true;
            enemyReturningToPatrol = false;
            enemyPatrolReturnX = enemyInitialX;
            playerInVision = false;
            lastSeenPlayerX = 0.0f;
            lastSeenPlayerY = 0.0f;
            lastSeenPlayerValid = false;
            alertLostElapsed = 0.0f;
            searchRandomState = 0x13579BDF;
            searchTargetX = 0.0f;
            searchTargetY = 0.0f;
            searchTargetValid = false;
            searchPathCount = 0;
            searchPathIndex = 0;
            enemyScanning = false;
            scanElapsed = 0.0f;
            scanBaseFacing = 0.0f;
            playerHP = 10;
            playerAlive = true;
            enemyHP = 3;
            enemyAttackCooldownRemaining = 0.0f;
            playerHitRemaining = 0.0f;
            dashDirectionX = 0;
            dashDirectionY = 0;
            dashDistanceRemaining = 0.0f;
            dashCooldownRemaining = 0.0f;
            dashActive = false;
            slashLeft = 0;
            slashTop = 0;
            slashRight = 0;
            slashBottom = 0;
            slashVisualRemaining = 0.0f;
            slashCooldownRemaining = 0.0f;
            enemyHitRemaining = 0.0f;
            executeFeedbackRemaining = 0.0f;
            detectionProgress = 0.0f;
            lostSightElapsed = 0.0f;
            enemyAlert = false;
            enemyAlive = true;
            exitUnlocked = false;
            roomComplete = false;
            updateColor = 0x00FF0000;
            slashRequested = false;
            dashRequested = false;
            executeRequested = false;
            gameplayInputBlocked = upPressed || downPressed || leftPressed || rightPressed
                || zPressed || xPressed || cPressed || spacePressed;
            newGameRequested = false;
            QueryPerformanceCounter(&previousUpdate);
            InvalidateRect(window, nullptr, FALSE);
        }

        if (applicationState != gameplayState)
        {
            QueryPerformanceCounter(&previousUpdate);
            Sleep(1);
            continue;
        }

        LARGE_INTEGER currentTime{};
        QueryPerformanceCounter(&currentTime);
        if (currentTime.QuadPart - previousUpdate.QuadPart >= updateInterval)
        {
            float deltaTime = static_cast<float>(currentTime.QuadPart - previousUpdate.QuadPart)
                / static_cast<float>(performanceFrequency.QuadPart);
            previousUpdate = currentTime;
            LONG movementX = gameplayInputBlocked || !playerAlive || roomComplete ? 0
                : static_cast<LONG>(rightPressed) - static_cast<LONG>(leftPressed);
            LONG movementY = gameplayInputBlocked || !playerAlive || roomComplete ? 0
                : static_cast<LONG>(downPressed) - static_cast<LONG>(upPressed);
            if (movementX || movementY)
            {
                facingX = movementX;
                facingY = movementY;
            }
            if (dashCooldownRemaining > 0.0f)
            {
                dashCooldownRemaining -= deltaTime;
            }
            if (dashRequested)
            {
                if (playerAlive && !roomComplete && !dashActive
                    && dashCooldownRemaining <= 0.0f)
                {
                    dashDirectionX = facingX;
                    dashDirectionY = facingY;
                    dashDistanceRemaining = dashDistance;
                    dashCooldownRemaining = dashCooldownDuration;
                    dashActive = true;
                }
                dashRequested = false;
            }

            bool playerDashingThisUpdate = dashActive;
            float movementScale = movementX && movementY ? 0.70710678f : 1.0f;
            float movementDeltaX = movementX * playerMoveSpeed * movementScale * deltaTime;
            float movementDeltaY = movementY * playerMoveSpeed * movementScale * deltaTime;
            LONG movementSteps = 1;
            if (dashActive)
            {
                float dashFrameDistance = dashSpeed * deltaTime;
                if (dashFrameDistance > dashDistanceRemaining)
                {
                    dashFrameDistance = dashDistanceRemaining;
                }

                float dashScale = dashDirectionX && dashDirectionY ? 0.70710678f : 1.0f;
                movementDeltaX = dashDirectionX * dashFrameDistance * dashScale;
                movementDeltaY = dashDirectionY * dashFrameDistance * dashScale;
                float largestDashDelta = movementDeltaX;
                if (largestDashDelta < 0.0f)
                {
                    largestDashDelta = -largestDashDelta;
                }
                float dashDeltaY = movementDeltaY;
                if (dashDeltaY < 0.0f)
                {
                    dashDeltaY = -dashDeltaY;
                }
                if (dashDeltaY > largestDashDelta)
                {
                    largestDashDelta = dashDeltaY;
                }
                movementSteps = static_cast<LONG>(largestDashDelta) + 1;
                dashDistanceRemaining -= dashFrameDistance;
                if (dashDistanceRemaining <= 0.0f)
                {
                    dashActive = false;
                }
            }

            movementDeltaX /= movementSteps;
            movementDeltaY /= movementSteps;
            for (LONG movementStep = 0; movementStep < movementSteps; ++movementStep)
            {
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
            }

            playerInVision = false;
            if (enemyAlive && playerAlive && !enemyAlert)
            {
                float facingX = cosf(enemyFacingAngle);
                float facingY = sinf(enemyFacingAngle);
                float visionX = playerX - enemyX;
                float visionY = playerY - enemyY;
                float visionForward = visionX * facingX + visionY * facingY;
                float visionLateral = visionX * -facingY + visionY * facingX;
                if (visionLateral < 0.0f)
                {
                    visionLateral = -visionLateral;
                }
                playerInVision = visionForward > 0.0f
                    && visionForward <= enemyVisionRange
                    && visionLateral <= visionForward * enemyVisionSlope
                    && !WallBlocksSegment(enemyX, enemyY, playerX, playerY);

                if (playerInVision)
                {
                    lastSeenPlayerX = playerX;
                    lastSeenPlayerY = playerY;
                    lastSeenPlayerValid = true;
                    lostSightElapsed = 0.0f;
                    enemyFacingAngle = TurnToward(enemyFacingAngle,
                        atan2f(playerY - enemyY, playerX - enemyX),
                        enemyFacingTurnSpeed * deltaTime);
                    detectionProgress += deltaTime / detectionFillDuration;
                    if (detectionProgress >= 1.0f)
                    {
                        detectionProgress = 1.0f;
                        enemyAlert = true;
                        alertLostElapsed = 0.0f;
                        searchTargetValid = false;
                        searchPathCount = 0;
                        searchPathIndex = 0;
                        enemyScanning = false;
                    }
                }
                else if (detectionProgress > 0.0f)
                {
                    float decayTime = lostSightElapsed + deltaTime - lostSightHoldDuration;
                    lostSightElapsed += deltaTime;
                    if (decayTime > 0.0f)
                    {
                        if (decayTime > deltaTime)
                        {
                            decayTime = deltaTime;
                        }
                        detectionProgress -= decayTime / detectionFillDuration;
                        if (detectionProgress <= 0.0f)
                        {
                            detectionProgress = 0.0f;
                            lostSightElapsed = 0.0f;
                            enemyPatrolReturnX = enemyX;
                            if (enemyPatrolReturnX < enemyPatrolLeftPoint)
                            {
                                enemyPatrolReturnX = enemyPatrolLeftPoint;
                            }
                            else if (enemyPatrolReturnX > enemyPatrolRightPoint)
                            {
                                enemyPatrolReturnX = enemyPatrolRightPoint;
                            }
                            enemyReturningToPatrol
                                = (enemyX - enemyPatrolReturnX) * (enemyX - enemyPatrolReturnX)
                                + (enemyY - enemyInitialY) * (enemyY - enemyInitialY) > 0.25f;
                            if (enemyReturningToPatrol)
                            {
                                searchPathCount = FindEnemyPath(enemyX, enemyY,
                                    NavigationColumn(enemyPatrolReturnX),
                                    NavigationRow(enemyInitialY));
                                searchPathIndex = 0;
                            }
                        }
                    }
                }
            }

            if (enemyAlive && playerAlive)
            {
                if (enemyAlert)
                {
                    float playerDifferenceX = playerX - enemyX;
                    float playerDifferenceY = playerY - enemyY;
                    bool playerReacquired
                        = playerDifferenceX * playerDifferenceX
                        + playerDifferenceY * playerDifferenceY <= enemyReacquireRangeSquared
                        && !WallBlocksSegment(enemyX, enemyY, playerX, playerY);
                    playerInVision = playerReacquired;
                    if (playerReacquired)
                    {
                        alertLostElapsed = 0.0f;
                        searchTargetValid = false;
                        searchPathCount = 0;
                        searchPathIndex = 0;
                        enemyScanning = false;
                        enemyFacingAngle = atan2f(playerDifferenceY, playerDifferenceX);
                        MoveEnemyToward(enemyX, enemyY, playerX, playerY,
                            enemyAlertSpeed, deltaTime, playerX, playerY);
                    }
                    else
                    {
                        alertLostElapsed += deltaTime;
                        if (alertLostElapsed >= enemyAlertSearchDuration)
                        {
                            enemyAlert = false;
                            lostSightElapsed = lostSightHoldDuration;
                            searchTargetValid = false;
                            searchPathCount = 0;
                            searchPathIndex = 0;
                            enemyScanning = false;
                        }
                        else if (enemyScanning)
                        {
                            scanElapsed += deltaTime;
                            if (scanElapsed < 0.5f)
                            {
                                enemyFacingAngle = scanBaseFacing
                                    - enemyScanAngle * (scanElapsed / 0.5f);
                            }
                            else if (scanElapsed < 1.0f)
                            {
                                enemyFacingAngle = scanBaseFacing - enemyScanAngle
                                    * (1.0f - (scanElapsed - 0.5f) / 0.5f);
                            }
                            else if (scanElapsed < 1.5f)
                            {
                                enemyFacingAngle = scanBaseFacing + enemyScanAngle
                                    * ((scanElapsed - 1.0f) / 0.5f);
                            }
                            else if (scanElapsed < enemyScanDuration)
                            {
                                enemyFacingAngle = scanBaseFacing + enemyScanAngle
                                    * (1.0f - (scanElapsed - 1.5f) / 0.5f);
                            }
                            else
                            {
                                enemyFacingAngle = scanBaseFacing;
                                enemyScanning = false;
                                searchTargetValid = false;
                            }
                        }
                        else
                        {
                            if (!searchTargetValid)
                            {
                                constexpr float targetOffsetX[4]
                                    = { 10.0f, -10.0f, 0.0f, 0.0f };
                                constexpr float targetOffsetY[4]
                                    = { 0.0f, 0.0f, 12.0f, -12.0f };
                                searchRandomState = searchRandomState * 1664525u + 1013904223u;
                                LONG firstCandidate = (searchRandomState >> 30) & 3;
                                for (LONG attempt = 0; attempt < 4 && !searchTargetValid; ++attempt)
                                {
                                    LONG candidate = (firstCandidate + attempt) & 3;
                                    float candidateX = playerX + targetOffsetX[candidate];
                                    float candidateY = playerY + targetOffsetY[candidate];
                                    LONG targetColumn = NavigationColumn(candidateX);
                                    LONG targetRow = NavigationRow(candidateY);
                                    if (candidateX < enemyHalfWidth
                                        || candidateX > framebufferWidth - enemyHalfWidth
                                        || candidateY < enemyHalfHeight
                                        || candidateY > framebufferHeight - enemyHalfHeight
                                        || !NavigationCellValid(targetColumn, targetRow)
                                        || (candidateX - enemyHalfWidth < wallRight
                                            && candidateX + enemyHalfWidth > wallLeft
                                            && candidateY - enemyHalfHeight < wallBottom
                                            && candidateY + enemyHalfHeight > wallTop))
                                    {
                                        continue;
                                    }
                                    searchTargetX = candidateX;
                                    searchTargetY = candidateY;
                                    searchPathCount = FindEnemyPath(enemyX, enemyY,
                                        targetColumn, targetRow);
                                    searchPathIndex = 0;
                                    searchTargetValid = true;
                                }
                            }

                            if (searchTargetValid)
                            {
                                float movementTargetX = searchTargetX;
                                float movementTargetY = searchTargetY;
                                if (searchPathIndex < searchPathCount)
                                {
                                    LONG node = navigationPath[searchPathIndex];
                                    movementTargetX = enemyHalfWidth
                                        + (node % navigationColumns) * navigationCellSize;
                                    movementTargetY = enemyHalfHeight
                                        + (node / navigationColumns) * navigationCellSize;
                                }
                                enemyFacingAngle = atan2f(movementTargetY - enemyY,
                                    movementTargetX - enemyX);
                                if (MoveEnemyToward(enemyX, enemyY,
                                    movementTargetX, movementTargetY, enemyAlertSpeed,
                                    deltaTime, playerX, playerY))
                                {
                                    if (searchPathIndex < searchPathCount)
                                    {
                                        ++searchPathIndex;
                                    }
                                    else
                                    {
                                        enemyScanning = true;
                                        scanElapsed = 0.0f;
                                        scanBaseFacing = enemyFacingAngle;
                                    }
                                }
                            }
                        }
                    }
                }
                else if (detectionProgress > 0.0f)
                {
                    if (!playerInVision && lastSeenPlayerValid)
                    {
                        enemyFacingAngle = TurnToward(enemyFacingAngle,
                            atan2f(lastSeenPlayerY - enemyY, lastSeenPlayerX - enemyX),
                            enemyFacingTurnSpeed * deltaTime);
                        MoveEnemyToward(enemyX, enemyY, lastSeenPlayerX, lastSeenPlayerY,
                            enemyPatrolSpeed, deltaTime, playerX, playerY);
                    }
                }
                else
                {
                    float patrolTargetX = enemyPatrolRight
                        ? enemyPatrolRightPoint : enemyPatrolLeftPoint;
                    float movementTargetX = enemyReturningToPatrol
                        ? enemyPatrolReturnX : patrolTargetX;
                    float movementTargetY = enemyInitialY;
                    if (enemyReturningToPatrol && searchPathIndex < searchPathCount)
                    {
                        LONG node = navigationPath[searchPathIndex];
                        movementTargetX = enemyHalfWidth
                            + (node % navigationColumns) * navigationCellSize;
                        movementTargetY = enemyHalfHeight
                            + (node / navigationColumns) * navigationCellSize;
                    }
                    enemyFacingAngle = atan2f(movementTargetY - enemyY,
                        movementTargetX - enemyX);
                    if (MoveEnemyToward(enemyX, enemyY, movementTargetX, movementTargetY,
                        enemyPatrolSpeed, deltaTime, playerX, playerY))
                    {
                        enemyX = movementTargetX;
                        enemyY = movementTargetY;
                        if (enemyReturningToPatrol && searchPathIndex < searchPathCount)
                        {
                            ++searchPathIndex;
                        }
                        else if (enemyReturningToPatrol)
                        {
                            enemyReturningToPatrol = false;
                        }
                        else
                        {
                            enemyPatrolRight = !enemyPatrolRight;
                        }
                    }
                }
            }

            LONG enemyCenterX = static_cast<LONG>(enemyX);
            LONG enemyCenterY = static_cast<LONG>(enemyY);
            LONG enemyLeft = enemyCenterX - enemyWidth / 2;
            LONG enemyTop = enemyCenterY - enemyHeight / 2;

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
            if (playerHitRemaining > 0.0f)
            {
                playerHitRemaining -= deltaTime;
            }
            if (enemyAttackCooldownRemaining > 0.0f)
            {
                enemyAttackCooldownRemaining -= deltaTime;
            }
            if (executeFeedbackRemaining > 0.0f)
            {
                executeFeedbackRemaining -= deltaTime;
            }

            if (slashRequested)
            {
                if (playerAlive && !roomComplete && !playerDashingThisUpdate
                    && slashCooldownRemaining <= 0.0f)
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
                    if (enemyAlive
                        && slashLeft < enemyLeft + enemyWidth
                        && slashRight > enemyLeft
                        && slashTop < enemyTop + enemyHeight
                        && slashBottom > enemyTop)
                    {
                        enemyHitRemaining = slashVisualDuration;
                        --enemyHP;
                        if (enemyHP <= 0)
                        {
                            enemyHP = 0;
                            enemyAlive = false;
                            slashCooldownRemaining = 0.0f;
                            dashCooldownRemaining = 0.0f;
                            playerInVision = false;
                            searchTargetValid = false;
                            searchPathCount = 0;
                            searchPathIndex = 0;
                            enemyScanning = false;
                        }
                    }
                }
                slashRequested = false;
            }

            if (executeRequested)
            {
                if (playerAlive && !roomComplete && enemyAlive
                    && detectionProgress <= 0.0f)
                {
                    LONG executeCenterX = static_cast<LONG>(playerX);
                    LONG executeCenterY = static_cast<LONG>(playerY);
                    LONG executeLeft;
                    LONG executeRight;
                    LONG executeTop;
                    LONG executeBottom;
                    if (facingX < 0)
                    {
                        executeRight = executeCenterX - playerWidth / 2;
                        executeLeft = executeRight - executeReach;
                    }
                    else if (facingX > 0)
                    {
                        executeLeft = executeCenterX + playerWidth / 2;
                        executeRight = executeLeft + executeReach;
                    }
                    else
                    {
                        executeLeft = executeCenterX - playerWidth / 2;
                        executeRight = executeLeft + playerWidth;
                    }

                    if (facingY < 0)
                    {
                        executeBottom = executeCenterY - playerHeight / 2;
                        executeTop = executeBottom - executeReach;
                    }
                    else if (facingY > 0)
                    {
                        executeTop = executeCenterY + playerHeight / 2;
                        executeBottom = executeTop + executeReach;
                    }
                    else
                    {
                        executeTop = executeCenterY - playerWidth / 2;
                        executeBottom = executeTop + playerWidth;
                    }

                    if (executeLeft < enemyLeft + enemyWidth
                        && executeRight > enemyLeft
                        && executeTop < enemyTop + enemyHeight
                        && executeBottom > enemyTop)
                    {
                        enemyHP = 0;
                        enemyAlive = false;
                        executeFeedbackRemaining = slashVisualDuration;
                        slashCooldownRemaining = 0.0f;
                        dashCooldownRemaining = 0.0f;
                        playerInVision = false;
                        searchTargetValid = false;
                        searchPathCount = 0;
                        searchPathIndex = 0;
                        enemyScanning = false;
                    }
                }
                executeRequested = false;
            }

            if (playerAlive && enemyAlive && enemyAlert
                && enemyAttackCooldownRemaining <= 0.0f
                && enemyX - enemyHalfWidth <= playerX + playerHalfWidth
                    + enemyAttackContactTolerance
                && enemyX + enemyHalfWidth + enemyAttackContactTolerance
                    >= playerX - playerHalfWidth
                && enemyY - enemyHalfHeight <= playerY + playerHalfHeight
                    + enemyAttackContactTolerance
                && enemyY + enemyHalfHeight + enemyAttackContactTolerance
                    >= playerY - playerHalfHeight)
            {
                --playerHP;
                playerHitRemaining = playerHitFeedbackDuration;
                enemyAttackCooldownRemaining = enemyAttackCooldownDuration;
                if (playerHP <= 0)
                {
                    playerHP = 0;
                    playerAlive = false;
                    dashActive = false;
                    dashDistanceRemaining = 0.0f;
                    slashRequested = false;
                    dashRequested = false;
                    executeRequested = false;
                    playerInVision = false;
                }
            }

            if (playerAlive && !enemyAlive)
            {
                exitUnlocked = true;
            }
            if (playerAlive && exitUnlocked && !roomComplete
                && playerX - playerHalfWidth < exitRight
                && playerX + playerHalfWidth > exitLeft
                && playerY - playerHalfHeight < exitBottom
                && playerY + playerHalfHeight > exitTop)
            {
                roomComplete = true;
                dashActive = false;
                dashDistanceRemaining = 0.0f;
                slashRequested = false;
                dashRequested = false;
                executeRequested = false;
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

            if (enemyAlive && !enemyAlert)
            {
                float redDistance = enemyVisionRange * detectionProgress;
                float enemyFacingX = cosf(enemyFacingAngle);
                float enemyFacingY = sinf(enemyFacingAngle);
                LONG visionLeft = enemyCenterX - enemyVisionRange;
                LONG visionRight = enemyCenterX + enemyVisionRange;
                LONG visionTop = enemyCenterY - enemyVisionRange;
                LONG visionBottom = enemyCenterY + enemyVisionRange;
                if (visionLeft < 0)
                {
                    visionLeft = 0;
                }
                if (visionRight >= framebufferWidth)
                {
                    visionRight = framebufferWidth - 1;
                }
                if (visionTop < 0)
                {
                    visionTop = 0;
                }
                if (visionBottom >= framebufferHeight)
                {
                    visionBottom = framebufferHeight - 1;
                }

                for (LONG pixelY = visionTop; pixelY <= visionBottom; ++pixelY)
                {
                    for (LONG pixelX = visionLeft; pixelX <= visionRight; ++pixelX)
                    {
                        float visionX = pixelX - enemyX;
                        float visionY = pixelY - enemyY;
                        float visionForward
                            = visionX * enemyFacingX + visionY * enemyFacingY;
                        float visionLateral
                            = visionX * -enemyFacingY + visionY * enemyFacingX;
                        if (visionLateral < 0.0f)
                        {
                            visionLateral = -visionLateral;
                        }

                        if (visionForward > 0.0f && visionForward <= enemyVisionRange
                            && visionLateral <= visionForward * enemyVisionSlope
                            && !WallBlocksSegment(enemyX, enemyY,
                                static_cast<float>(pixelX), static_cast<float>(pixelY)))
                        {
                            framebuffer[pixelY * framebufferWidth + pixelX]
                                = visionForward <= redDistance ? 0x00401818 : 0x00182040;
                        }
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

            for (LONG y = exitTop; y < exitBottom; ++y)
            {
                for (LONG x = exitLeft; x < exitRight; ++x)
                {
                    framebuffer[y * framebufferWidth + x]
                        = exitUnlocked ? 0x0040E080 : 0x00282830;
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

            if (enemyAlive)
            {
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
            }

            if (enemyAlive && detectionProgress > 0.0f)
            {
                for (LONG y = 0; y < 5; ++y)
                {
                    for (LONG x = 0; x < 3; ++x)
                    {
                        bool symbolPixel = enemyAlert
                            ? (x == 1 && y != 3)
                            : ((y == 0 && x < 2) || (y == 1 && x == 2)
                                || (y == 2 && x == 1) || (y == 4 && x == 1));
                        LONG symbolX = enemyCenterX - 1 + x;
                        LONG symbolY = enemyTop - 7 + y;
                        if (symbolPixel && symbolX >= 0 && symbolX < framebufferWidth
                            && symbolY >= 0 && symbolY < framebufferHeight)
                        {
                            framebuffer[symbolY * framebufferWidth + symbolX]
                                = enemyAlert ? 0x00FF4040 : 0x00FFD800;
                        }
                    }
                }
            }

            if (executeFeedbackRemaining > 0.0f)
            {
                for (LONG y = enemyCenterY - 1; y <= enemyCenterY + 1; ++y)
                {
                    for (LONG x = enemyCenterX - 1; x <= enemyCenterX + 1; ++x)
                    {
                        framebuffer[y * framebufferWidth + x] = 0x00FFFF80;
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
                            = head ? 0x00FFFFFF
                            : (!playerAlive ? 0x00404050
                                : (playerHitRemaining > 0.0f ? 0x00FFFFFF
                                    : (playerDashingThisUpdate
                                        ? 0x0080FFFF : 0x0000A0FF)));
                    }
                }
            }

            if (!playerAlive)
            {
                constexpr LONG deadLeft = framebufferWidth / 2 - 8;
                constexpr LONG deadTop = framebufferHeight / 2 - 14;
                for (LONG y = 0; y < 5; ++y)
                {
                    for (LONG x = 0; x < 15; ++x)
                    {
                        LONG letter = x / 4;
                        LONG letterX = x & 3;
                        bool deadPixel = false;
                        if (letterX < 3)
                        {
                            if (letter == 0 || letter == 3)
                            {
                                deadPixel = letterX == 0
                                    || (letterX == 2 && y > 0 && y < 4)
                                    || ((y == 0 || y == 4) && letterX < 2);
                            }
                            else if (letter == 1)
                            {
                                deadPixel = letterX == 0
                                    || (letterX < 3 && (y == 0 || y == 2 || y == 4));
                            }
                            else
                            {
                                deadPixel = (y == 0 && letterX == 1)
                                    || (y > 0 && (letterX == 0 || letterX == 2))
                                    || y == 2;
                            }
                        }
                        if (deadPixel)
                        {
                            framebuffer[(deadTop + y) * framebufferWidth + deadLeft + x]
                                = 0x00FF4040;
                        }
                    }
                }
            }

            if (roomComplete)
            {
                constexpr LONG clearLeft = framebufferWidth / 2 - 9;
                constexpr LONG clearTop = framebufferHeight / 2 - 14;
                for (LONG y = 0; y < 5; ++y)
                {
                    for (LONG x = 0; x < 19; ++x)
                    {
                        LONG letter = x / 4;
                        LONG letterX = x & 3;
                        bool clearPixel = false;
                        if (letterX < 3)
                        {
                            if (letter == 0)
                            {
                                clearPixel = letterX == 0 || y == 0 || y == 4;
                            }
                            else if (letter == 1)
                            {
                                clearPixel = letterX == 0 || y == 4;
                            }
                            else if (letter == 2)
                            {
                                clearPixel = letterX == 0 || y == 0 || y == 2 || y == 4;
                            }
                            else if (letter == 3)
                            {
                                clearPixel = y == 0 || y == 2
                                    || (letterX == 0 && y > 0)
                                    || (letterX == 2 && y > 0);
                            }
                            else
                            {
                                clearPixel = letterX == 0 || y == 0 || y == 2
                                    || (letterX == 2 && y < 3)
                                    || (letterX == 1 && y == 3)
                                    || (letterX == 2 && y == 4);
                            }
                        }
                        if (clearPixel)
                        {
                            framebuffer[(clearTop + y) * framebufferWidth + clearLeft + x]
                                = 0x0080FFC0;
                        }
                    }
                }
            }

            if (enemyAlive)
            {
                DWORD visionMarkerColor = playerInVision ? 0x0000FF80 : 0x00202028;
                for (LONG y = 8; y < 12; ++y)
                {
                    for (LONG x = 24; x < 28; ++x)
                    {
                        framebuffer[y * framebufferWidth + x] = visionMarkerColor;
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
