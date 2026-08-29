#include <windows.h>
#include <timeapi.h>
#include <math.h>

#pragma comment(lib, "winmm.lib")

constexpr LONG framebufferWidth = 320;
constexpr LONG framebufferHeight = 180;
LONG roomSizeStage = 2;
LONG worldWidth = 320;
LONG worldHeight = 180;
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
constexpr LONG roomCount = 6;
constexpr LONG openRoomType = 0;
constexpr LONG pillarRoomType = 1;
constexpr LONG mazeRoomType = 2;
constexpr LONG maxRoomWalls = 24;
constexpr LONG enemyCount = 3;
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
constexpr LONG maxNavigationColumns = 100;
constexpr LONG maxNavigationRows = 55;
constexpr LONG maxNavigationNodeCount = maxNavigationColumns * maxNavigationRows;
LONG navigationColumns = 40;
LONG navigationRows = 22;
LONG navigationNodeCount = navigationColumns * navigationRows;
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
constexpr LONG gameOverEndState = 1;
constexpr LONG runClearEndState = 2;
constexpr DWORD saveMagic = 0x56535344;
constexpr DWORD saveVersion = 3;
constexpr wchar_t saveFileName[] = L"DeadSignal.sav";
LONG applicationState = titleMainState;
LONG menuSelection = 0;
LONG titleStatus = 0;
bool newGameRequested = false;
bool loadGameRequested = false;
LONG currentRoom = 0;
DWORD runSeed = 0;
LONG runKillCount = 0;
LONG currentEnemyRemaining = enemyCount;
LONG currentRoomType = openRoomType;
LONG currentLayoutVariant = 0;
LONG currentExitSide = 1;
LONG currentWallCount = 0;
LONG currentWallLeft[maxRoomWalls]{};
LONG currentWallTop[maxRoomWalls]{};
LONG currentWallRight[maxRoomWalls]{};
LONG currentWallBottom[maxRoomWalls]{};
LONG currentExitLeft = exitLeft;
LONG currentExitTop = exitTop;
LONG currentExitRight = exitRight;
LONG currentExitBottom = exitBottom;
float currentPlayerStartX = static_cast<float>(playerCenterX);
float currentPlayerStartY = static_cast<float>(playerCenterY);
float currentEnemyStartX[enemyCount]{};
float currentEnemyStartY[enemyCount]{};
float currentPatrolLeft[enemyCount]{};
float currentPatrolRight[enemyCount]{};
bool currentPatrolStartsRight[enemyCount]{};
LONG runEndState = 0;
LONG runEndSelection = 0;
bool upgradeMenuActive = false;
LONG upgradeSelection = 0;
LONG upgradeOptionA = 0;
LONG upgradeOptionB = 1;
bool rerollUsed = false;
bool upgradeConfirmRequested = false;
bool gameplayMenuActive = false;
LONG gameplayMenuSelection = 0;
bool gameplayMenuSettingsStatus = false;
bool saveAndTitleRequested = false;
bool escapePressed = false;
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
short navigationParent[maxNavigationNodeCount];
unsigned short navigationScore[maxNavigationNodeCount];
BYTE navigationState[maxNavigationNodeCount];
unsigned short navigationPath[enemyCount][maxNavigationNodeCount];

struct EnemyRuntime
{
    float x;
    float y;
    float facingAngle;
    float patrolReturnX;
    float lastSeenPlayerX;
    float lastSeenPlayerY;
    float alertLostElapsed;
    float searchTargetX;
    float searchTargetY;
    float scanElapsed;
    float scanBaseFacing;
    float attackCooldownRemaining;
    float hitRemaining;
    float executeFeedbackRemaining;
    float detectionProgress;
    float lostSightElapsed;
    DWORD searchRandomState;
    LONG searchPathCount;
    LONG searchPathIndex;
    LONG hp;
    bool patrolRight;
    bool returningToPatrol;
    bool playerInVision;
    bool lastSeenPlayerValid;
    bool searchTargetValid;
    bool scanning;
    bool alert;
    bool alive;
};

struct SaveCheckpoint
{
    DWORD magic;
    DWORD version;
    DWORD seed;
    LONG room;
    LONG playerHP;
    float moveSpeed;
    float slashCooldown;
    float dashCooldown;
    DWORD reroll;
    LONG runKills;
};

static_assert(sizeof(SaveCheckpoint) == 40);

bool ReadCheckpoint(SaveCheckpoint* checkpoint)
{
    HANDLE file = CreateFileW(saveFileName, GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD bytesRead = 0;
    bool valid = GetFileSize(file, nullptr) == sizeof(*checkpoint)
        && ReadFile(file, checkpoint, sizeof(*checkpoint), &bytesRead, nullptr)
        && bytesRead == sizeof(*checkpoint);
    CloseHandle(file);
    return valid && checkpoint->magic == saveMagic && checkpoint->version == saveVersion
        && checkpoint->seed != 0 && checkpoint->room >= 0 && checkpoint->room < roomCount
        && checkpoint->playerHP > 0 && checkpoint->playerHP <= 10
        && (checkpoint->moveSpeed == playerMoveSpeed
            || checkpoint->moveSpeed == playerMoveSpeed * 1.1f)
        && (checkpoint->slashCooldown == slashCooldownDuration
            || checkpoint->slashCooldown == slashCooldownDuration * 0.8f)
        && (checkpoint->dashCooldown == dashCooldownDuration
            || checkpoint->dashCooldown == dashCooldownDuration * 0.8f)
        && checkpoint->reroll <= 1 && checkpoint->runKills >= 0;
}

void WriteCheckpoint(DWORD seed, LONG room, LONG playerHP, float moveSpeed,
    float slashCooldown, float dashCooldown, bool reroll, LONG runKills)
{
    SaveCheckpoint checkpoint
    {
        saveMagic,
        saveVersion,
        seed,
        room,
        playerHP,
        moveSpeed,
        slashCooldown,
        dashCooldown,
        static_cast<DWORD>(reroll),
        runKills
    };
    HANDLE file = CreateFileW(saveFileName, GENERIC_WRITE, 0, nullptr,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file != INVALID_HANDLE_VALUE)
    {
        DWORD bytesWritten = 0;
        WriteFile(file, &checkpoint, sizeof(checkpoint), &bytesWritten, nullptr);
        CloseHandle(file);
    }
}

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

DWORD RoomRandom(DWORD seed, LONG room)
{
    DWORD value = seed ^ (0x9E3779B9u * static_cast<DWORD>(room + 1));
    value ^= value >> 16;
    value *= 0x7FEB352Du;
    value ^= value >> 15;
    value *= 0x846CA68Bu;
    return value ^ (value >> 16);
}

DWORD NextRoomRandom(DWORD& state)
{
    state = state * 1664525u + 1013904223u;
    return state;
}

void AddRoomWall(LONG left, LONG top, LONG right, LONG bottom)
{
    LONG wall = currentWallCount++;
    currentWallLeft[wall] = left;
    currentWallTop[wall] = top;
    currentWallRight[wall] = right;
    currentWallBottom[wall] = bottom;
}

bool RectangleOverlapsRoomWall(float left, float top, float right, float bottom)
{
    for (LONG wall = 0; wall < currentWallCount; ++wall)
    {
        if (left < currentWallRight[wall] && right > currentWallLeft[wall]
            && top < currentWallBottom[wall] && bottom > currentWallTop[wall])
        {
            return true;
        }
    }
    return false;
}

void SetRoomSizeStage(LONG stage)
{
    constexpr LONG roomWidths[5] = { 160, 320, 480, 640, 800 };
    constexpr LONG roomHeights[5] = { 90, 180, 270, 360, 450 };
    if (stage < 1)
    {
        stage = 1;
    }
    else if (stage > 5)
    {
        stage = 5;
    }
    roomSizeStage = stage;
    worldWidth = roomWidths[stage - 1];
    worldHeight = roomHeights[stage - 1];
    navigationColumns = (worldWidth - enemyWidth) / navigationCellSize + 1;
    navigationRows = (worldHeight - enemyHeight) / navigationCellSize + 1;
    navigationNodeCount = navigationColumns * navigationRows;
}

void SetupCurrentRoom()
{
    SetRoomSizeStage(currentRoom / 2 + 2);

    DWORD state = RoomRandom(runSeed, currentRoom);
    currentRoomType = NextRoomRandom(state) % 3;
    currentLayoutVariant = currentRoomType == openRoomType
        ? 0 : NextRoomRandom(state) >> 31;
    currentExitSide = NextRoomRandom(state) >> 31;
    currentWallCount = 0;

    if (currentRoomType == openRoomType)
    {
        LONG structureCount = roomSizeStage == 1 ? 1 : (roomSizeStage - 1) * 2 - 1;
        for (LONG structure = 0; structure < structureCount; ++structure)
        {
            LONG column = structure & 1 ? 3 : 2;
            LONG row = structure / 2 + 1;
            LONG left = worldWidth * column / 5 - 8;
            LONG top = worldHeight * row / ((structureCount + 1) / 2 + 1) - 12;
            AddRoomWall(left, top, left + 16, top + 24);
        }
    }
    else if (currentRoomType == pillarRoomType)
    {
        LONG columns = roomSizeStage;
        LONG rows = roomSizeStage < 3 ? roomSizeStage : roomSizeStage - 1;
        for (LONG row = 0; row < rows; ++row)
        {
            for (LONG column = 0; column < columns; ++column)
            {
                LONG left = worldWidth * (column + 1) / (columns + 1) - 8;
                if (currentLayoutVariant && (row & 1))
                {
                    left += worldWidth / (columns + 1) / 3;
                }
                if (left + 16 > worldWidth - 32)
                {
                    left = worldWidth - 48;
                }
                LONG top = worldHeight * (row + 1) / (rows + 1) - 12;
                AddRoomWall(left, top, left + 16, top + 24);
            }
        }
    }
    else if (currentRoomType == mazeRoomType)
    {
        if (!currentLayoutVariant)
        {
            if (worldHeight < 100)
            {
                AddRoomWall(worldWidth / 2 - 60, 12,
                    worldWidth / 2 + 60, 28);
            }
            else
            {
                LONG columns = roomSizeStage + 1;
                LONG segmentsPerColumn = worldHeight >= 270 ? 2 : 1;
                for (LONG column = 0; column < columns; ++column)
                {
                    LONG left = worldWidth * (column + 1) / (columns + 1) - 8;
                    for (LONG segment = 0; segment < segmentsPerColumn; ++segment)
                    {
                        LONG top = segmentsPerColumn == 1
                            ? worldHeight / 2 - 50
                            : (segment ? worldHeight - 116 : 16);
                        if ((column + segment) & 1)
                        {
                            top += segmentsPerColumn == 1 ? 20 : 32;
                        }
                        if (top + 100 > worldHeight)
                        {
                            top = worldHeight - 100;
                        }
                        AddRoomWall(left, top, left + 16, top + 100);
                    }
                }
            }
        }
        else
        {
            LONG columns = (worldWidth - 64) / 144;
            if (!columns)
            {
                columns = 1;
            }
            LONG rows = worldHeight / 96;
            if (!rows)
            {
                rows = 1;
            }
            for (LONG row = 0; row < rows; ++row)
            {
                for (LONG column = 0; column < columns; ++column)
                {
                    LONG left = 32 + (worldWidth - 64) * (column * 2 + 1)
                        / (columns * 2) - 60;
                    LONG top = worldHeight < 100 ? 12
                        : worldHeight * (row + 1) / (rows + 1) - 8;
                    if ((row + column) & 1)
                    {
                        left += 24;
                    }
                    if (worldWidth < 200)
                    {
                        left = 20;
                    }
                    else if (left < 32)
                    {
                        left = 32;
                    }
                    else if (left + 120 > worldWidth - 32)
                    {
                        left = worldWidth - 152;
                    }
                    AddRoomWall(left, top, left + 120, top + 16);
                }
            }
        }
    }

    if (currentExitSide)
    {
        currentExitLeft = worldWidth - 16;
        currentExitRight = worldWidth;
        currentPlayerStartX = 28.0f;
    }
    else
    {
        currentExitLeft = 0;
        currentExitRight = 16;
        currentPlayerStartX = worldWidth - 28.0f;
    }
    currentExitTop = worldHeight / 2 - 12;
    currentExitBottom = worldHeight / 2 + 12;
    currentPlayerStartY = worldHeight / 2.0f;

    constexpr LONG enemyCandidateX[12] = { 2, 4, 6, 2, 4, 6, 2, 4, 6, 3, 5, 3 };
    constexpr LONG enemyCandidateY[12] = { 4, 4, 4, 2, 2, 2, 6, 6, 6, 3, 5, 5 };
    bool candidateUsed[12]{};
    for (LONG enemy = 0; enemy < enemyCount; ++enemy)
    {
        LONG firstCandidate = NextRoomRandom(state) % 12;
        for (LONG attempt = 0; attempt < 12; ++attempt)
        {
            LONG candidate = (firstCandidate + attempt) % 12;
            float candidateX = static_cast<float>(worldWidth * enemyCandidateX[candidate] / 8);
            float candidateY = static_cast<float>(worldHeight * enemyCandidateY[candidate] / 8);
            LONG candidateColumn = static_cast<LONG>((candidateX - enemyHalfWidth
                + navigationCellSize * 0.5f) / navigationCellSize);
            LONG candidateRow = static_cast<LONG>((candidateY - enemyHalfHeight
                + navigationCellSize * 0.5f) / navigationCellSize);
            float cellX = enemyHalfWidth + candidateColumn * navigationCellSize;
            float cellY = enemyHalfHeight + candidateRow * navigationCellSize;
            if (candidateUsed[candidate]
                || RectangleOverlapsRoomWall(candidateX - enemyHalfWidth,
                    candidateY - enemyHalfHeight, candidateX + enemyHalfWidth,
                    candidateY + enemyHalfHeight)
                || candidateColumn < 0 || candidateColumn >= navigationColumns
                || candidateRow < 0 || candidateRow >= navigationRows
                || RectangleOverlapsRoomWall(cellX - enemyHalfWidth,
                    cellY - enemyHalfHeight, cellX + enemyHalfWidth,
                    cellY + enemyHalfHeight)
                || (candidateX - enemyHalfWidth < currentExitRight
                    && candidateX + enemyHalfWidth > currentExitLeft
                    && candidateY - enemyHalfHeight < currentExitBottom
                    && candidateY + enemyHalfHeight > currentExitTop)
                || (candidateX - enemyHalfWidth < currentPlayerStartX + playerHalfWidth
                    && candidateX + enemyHalfWidth > currentPlayerStartX - playerHalfWidth
                    && candidateY - enemyHalfHeight < currentPlayerStartY + playerHalfHeight
                    && candidateY + enemyHalfHeight > currentPlayerStartY - playerHalfHeight))
            {
                continue;
            }
            candidateUsed[candidate] = true;
            currentEnemyStartX[enemy] = candidateX;
            currentEnemyStartY[enemy] = candidateY;
            break;
        }
        float patrolHalfLength = 24.0f + enemy * 8.0f;
        currentPatrolLeft[enemy] = currentEnemyStartX[enemy] - patrolHalfLength;
        currentPatrolRight[enemy] = currentEnemyStartX[enemy] + patrolHalfLength;
        if (currentPatrolLeft[enemy] < enemyHalfWidth)
        {
            currentPatrolLeft[enemy] = enemyHalfWidth;
        }
        if (currentPatrolRight[enemy] > worldWidth - enemyHalfWidth)
        {
            currentPatrolRight[enemy] = worldWidth - enemyHalfWidth;
        }
        currentPatrolStartsRight[enemy] = (NextRoomRandom(state) >> 31) != 0;
    }
}

bool SingleWallBlocksSegment(LONG wall, float startX, float startY, float endX, float endY)
{
    float enter = 0.0f;
    float exit = 1.0f;
    float difference = endX - startX;
    if (difference == 0.0f)
    {
        if (startX < currentWallLeft[wall] || startX >= currentWallRight[wall])
        {
            return false;
        }
    }
    else
    {
        float first = (currentWallLeft[wall] - startX) / difference;
        float second = (currentWallRight[wall] - startX) / difference;
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
        if (startY < currentWallTop[wall] || startY >= currentWallBottom[wall])
        {
            return false;
        }
    }
    else
    {
        float first = (currentWallTop[wall] - startY) / difference;
        float second = (currentWallBottom[wall] - startY) / difference;
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

bool WallBlocksSegment(float startX, float startY, float endX, float endY)
{
    for (LONG wall = 0; wall < currentWallCount; ++wall)
    {
        if (SingleWallBlocksSegment(wall, startX, startY, endX, endY))
        {
            return true;
        }
    }
    return false;
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
    return !RectangleOverlapsRoomWall(centerX - enemyHalfWidth,
        centerY - enemyHalfHeight, centerX + enemyHalfWidth, centerY + enemyHalfHeight);
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

LONG FindEnemyPath(float startX, float startY, LONG targetColumn, LONG targetRow,
    unsigned short* path)
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
                path[count++] = static_cast<unsigned short>(current);
                current = navigationParent[current];
            }
            for (LONG index = 0; index < count / 2; ++index)
            {
                unsigned short swap = path[index];
                path[index] = path[count - index - 1];
                path[count - index - 1] = swap;
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
        else if (nextX > worldWidth - enemyHalfWidth)
        {
            nextX = worldWidth - enemyHalfWidth;
        }
        for (LONG wall = 0; movementDeltaX != 0.0f && wall < currentWallCount; ++wall)
        {
            if (nextX - enemyHalfWidth < currentWallRight[wall]
                && nextX + enemyHalfWidth > currentWallLeft[wall]
                && enemyY - enemyHalfHeight < currentWallBottom[wall]
                && enemyY + enemyHalfHeight > currentWallTop[wall])
            {
                nextX = movementDeltaX > 0.0f
                    ? currentWallLeft[wall] - enemyHalfWidth
                    : currentWallRight[wall] + enemyHalfWidth;
            }
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
        else if (nextY > worldHeight - enemyHalfHeight)
        {
            nextY = worldHeight - enemyHalfHeight;
        }
        for (LONG wall = 0; movementDeltaY != 0.0f && wall < currentWallCount; ++wall)
        {
            if (enemyX - enemyHalfWidth < currentWallRight[wall]
                && enemyX + enemyHalfWidth > currentWallLeft[wall]
                && nextY - enemyHalfHeight < currentWallBottom[wall]
                && nextY + enemyHalfHeight > currentWallTop[wall])
            {
                nextY = movementDeltaY > 0.0f
                    ? currentWallTop[wall] - enemyHalfHeight
                    : currentWallBottom[wall] + enemyHalfHeight;
            }
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

void CloseGameplayMenu()
{
    gameplayMenuActive = false;
    gameplayMenuSettingsStatus = false;
    gameplayInputBlocked = upPressed || downPressed || leftPressed || rightPressed
        || zPressed || xPressed || cPressed || spacePressed;
}

void ConfirmGameplayMenu(HWND window)
{
    if (gameplayMenuSelection == 0)
    {
        CloseGameplayMenu();
    }
    else if (gameplayMenuSelection == 1)
    {
        gameplayMenuSettingsStatus = true;
    }
    else
    {
        saveAndTitleRequested = true;
    }
    InvalidateRect(window, nullptr, FALSE);
}

LRESULT CALLBACK WindowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if ((message == WM_KEYDOWN || message == WM_KEYUP) && wParam == VK_ESCAPE)
    {
        bool pressed = message == WM_KEYDOWN;
        if (applicationState == gameplayState && !runEndState && !upgradeMenuActive
            && pressed && !escapePressed)
        {
            if (gameplayMenuActive)
            {
                CloseGameplayMenu();
            }
            else
            {
                gameplayMenuActive = true;
                gameplayMenuSelection = 0;
                gameplayMenuSettingsStatus = false;
            }
            InvalidateRect(window, nullptr, FALSE);
        }
        escapePressed = pressed;
        return 0;
    }

    if (message == WM_LBUTTONDOWN && applicationState == gameplayState
        && !runEndState && !upgradeMenuActive)
    {
        RECT client{};
        GetClientRect(window, &client);
        LONG clientWidth = client.right;
        LONG clientHeight = client.bottom;
        LONG destinationWidth = clientWidth;
        LONG destinationHeight = clientWidth * framebufferHeight / framebufferWidth;
        if (destinationHeight > clientHeight)
        {
            destinationHeight = clientHeight;
            destinationWidth = clientHeight * framebufferWidth / framebufferHeight;
        }
        LONG destinationX = (clientWidth - destinationWidth) / 2;
        LONG destinationY = (clientHeight - destinationHeight) / 2;
        LONG mouseX = static_cast<short>(LOWORD(lParam));
        LONG mouseY = static_cast<short>(HIWORD(lParam));
        LONG logicalX = destinationWidth
            ? (mouseX - destinationX) * framebufferWidth / destinationWidth : -1;
        LONG logicalY = destinationHeight
            ? (mouseY - destinationY) * framebufferHeight / destinationHeight : -1;
        if (!gameplayMenuActive && logicalX >= 302 && logicalX < 316
            && logicalY >= 3 && logicalY < 15)
        {
            gameplayMenuActive = true;
            gameplayMenuSelection = 0;
            gameplayMenuSettingsStatus = false;
            InvalidateRect(window, nullptr, FALSE);
        }
        else if (gameplayMenuActive && mouseX >= clientWidth / 2 - 120
            && mouseX <= clientWidth / 2 + 120)
        {
            for (LONG item = 0; item < 3; ++item)
            {
                LONG top = clientHeight / 5 + 50 + item * 30;
                if (mouseY >= top && mouseY < top + 24)
                {
                    gameplayMenuSelection = item;
                    ConfirmGameplayMenu(window);
                    break;
                }
            }
        }
        return 0;
    }

    if ((message == WM_KEYDOWN || message == WM_KEYUP) && wParam == 'C')
    {
        bool pressed = message == WM_KEYDOWN;
        if (applicationState == gameplayState && !runEndState
            && !upgradeMenuActive && !gameplayMenuActive && !gameplayInputBlocked
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
        if (applicationState == gameplayState && !runEndState
            && !upgradeMenuActive && !gameplayMenuActive && !gameplayInputBlocked
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
            if (pressed && !zPressed)
            {
                if (gameplayMenuActive)
                {
                    ConfirmGameplayMenu(window);
                }
                else if (upgradeMenuActive)
                {
                    upgradeConfirmRequested = true;
                }
                else if (runEndState)
                {
                    if (runEndSelection == 0)
                    {
                        newGameRequested = true;
                    }
                    else
                    {
                        applicationState = titleMainState;
                        menuSelection = 0;
                        titleStatus = 0;
                        runEndState = 0;
                        runEndSelection = 0;
                        InvalidateRect(window, nullptr, FALSE);
                    }
                }
                else if (!gameplayInputBlocked)
                {
                    slashRequested = true;
                }
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
                    loadGameRequested = true;
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
                loadGameRequested = true;
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
        if (applicationState == gameplayState && !runEndState && !gameplayMenuActive
            && !gameplayInputBlocked
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

        if (gameplayMenuActive && newlyPressed
            && (wParam == VK_UP || wParam == VK_DOWN))
        {
            LONG previousSelection = gameplayMenuSelection;
            if (wParam == VK_UP && gameplayMenuSelection > 0)
            {
                --gameplayMenuSelection;
            }
            else if (wParam == VK_DOWN && gameplayMenuSelection < 2)
            {
                ++gameplayMenuSelection;
            }
            gameplayMenuSettingsStatus = false;
            if (gameplayMenuSelection != previousSelection)
            {
                InvalidateRect(window, nullptr, FALSE);
            }
        }
        else if (runEndState && newlyPressed
            && (wParam == VK_UP || wParam == VK_DOWN))
        {
            LONG previousSelection = runEndSelection;
            if (wParam == VK_UP && runEndSelection > 0)
            {
                --runEndSelection;
            }
            else if (wParam == VK_DOWN && runEndSelection < 1)
            {
                ++runEndSelection;
            }
            if (runEndSelection != previousSelection)
            {
                InvalidateRect(window, nullptr, FALSE);
            }
        }
        else if (upgradeMenuActive && newlyPressed
            && (wParam == VK_UP || wParam == VK_DOWN))
        {
            LONG previousSelection = upgradeSelection;
            if (wParam == VK_UP && upgradeSelection > 0)
            {
                --upgradeSelection;
            }
            else if (wParam == VK_DOWN && upgradeSelection < 2)
            {
                ++upgradeSelection;
            }
            if (upgradeSelection != previousSelection)
            {
                InvalidateRect(window, nullptr, FALSE);
            }
        }
        else if (applicationState != gameplayState && newlyPressed
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

    if (message == WM_ERASEBKGND)
    {
        return 1;
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
                        ? L"\uC800\uC7A5 \uC5C6\uC74C"
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

        SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
        SetBkMode(deviceContext, TRANSPARENT);
        SetTextColor(deviceContext, RGB(220, 220, 220));
        wchar_t hudText[32];
        RECT hudLine
        {
            destinationX + 8,
            destinationY + 3,
            destinationX + destinationWidth / 3,
            destinationY + 24
        };
        wsprintfW(hudText, L"%ld/%ld", currentEnemyRemaining, enemyCount);
        DrawTextW(deviceContext, hudText, -1, &hudLine,
            DT_LEFT | DT_TOP | DT_SINGLELINE);

        const wchar_t* roomName = currentRoomType == openRoomType ? L"Open"
            : (currentRoomType == pillarRoomType ? L"Pillar" : L"Maze");
        wsprintfW(hudText, L"%02ld.%s", currentRoom + 1, roomName);
        hudLine.left = destinationX + destinationWidth / 2 - 60;
        hudLine.right = destinationX + destinationWidth / 2 + 60;
        DrawTextW(deviceContext, hudText, -1, &hudLine,
            DT_CENTER | DT_TOP | DT_SINGLELINE);
        wsprintfW(hudText, L"Kill:%ld", runKillCount);
        hudLine.left = destinationX + destinationWidth / 2 + 64;
        hudLine.right = destinationX + destinationWidth - 34;
        DrawTextW(deviceContext, hudText, -1, &hudLine,
            DT_LEFT | DT_TOP | DT_SINGLELINE);

        for (LONG bar = 0; bar < 3; ++bar)
        {
            RECT iconBar
            {
                destinationX + 302 * destinationWidth / framebufferWidth,
                destinationY + (3 + bar * 4) * destinationHeight / framebufferHeight,
                destinationX + 316 * destinationWidth / framebufferWidth,
                destinationY + (5 + bar * 4) * destinationHeight / framebufferHeight
            };
            if (iconBar.bottom <= iconBar.top)
            {
                iconBar.bottom = iconBar.top + 1;
            }
            FillRect(deviceContext, &iconBar,
                reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
        }

        if (gameplayMenuActive)
        {
            RECT line = clientArea;
            line.top = clientHeight / 5;
            line.bottom = line.top + 30;
            SetTextColor(deviceContext, RGB(220, 220, 220));
            DrawTextW(deviceContext, L"MENU", -1, &line,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            for (LONG item = 0; item < 3; ++item)
            {
                const wchar_t* text = item == 0 ? L"\uAC8C\uC784\uC73C\uB85C \uB3CC\uC544\uAC00\uAE30"
                    : (item == 1 ? L"\uC124\uC815"
                        : L"\uC800\uC7A5 \uD6C4 \uD0C0\uC774\uD2C0\uB85C \uAC00\uAE30");
                line.top = clientHeight / 5 + 50 + item * 30;
                line.bottom = line.top + 24;
                SetTextColor(deviceContext, item == gameplayMenuSelection
                    ? RGB(255, 216, 0) : RGB(160, 160, 160));
                DrawTextW(deviceContext, text, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            if (gameplayMenuSettingsStatus)
            {
                line.top = clientHeight / 5 + 150;
                line.bottom = line.top + 24;
                SetTextColor(deviceContext, RGB(220, 220, 220));
                DrawTextW(deviceContext, L"\uC124\uC815 \uBBF8\uAD6C\uD604", -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        }
        else if (upgradeMenuActive)
        {
            SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
            SetBkMode(deviceContext, TRANSPARENT);
            RECT line = clientArea;
            line.top = clientHeight / 5;
            line.bottom = line.top + 30;
            SetTextColor(deviceContext, RGB(220, 220, 220));
            DrawTextW(deviceContext, L"UPGRADE", -1, &line,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            for (LONG item = 0; item < 3; ++item)
            {
                LONG upgrade = item ? upgradeOptionB : upgradeOptionA;
                const wchar_t* text = item == 2
                    ? (rerollUsed ? L"REROLL (USED)" : L"REROLL")
                    : (upgrade == 0 ? L"MOVE+"
                        : (upgrade == 1 ? L"SLASH+" : L"DASH+"));
                line.top = clientHeight / 5 + 50 + item * 30;
                line.bottom = line.top + 24;
                SetTextColor(deviceContext, item == upgradeSelection
                    ? (item == 2 && rerollUsed ? RGB(96, 96, 96) : RGB(255, 216, 0))
                    : (item == 2 && rerollUsed ? RGB(64, 64, 64) : RGB(160, 160, 160)));
                DrawTextW(deviceContext, text, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        }
        else if (runEndState)
        {
            SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
            SetBkMode(deviceContext, TRANSPARENT);
            RECT line = clientArea;
            if (runEndState == gameOverEndState)
            {
                line.top = clientHeight / 2 - 50;
                line.bottom = line.top + 30;
                SetTextColor(deviceContext, RGB(220, 220, 220));
                DrawTextW(deviceContext, L"GAME OVER", -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            for (LONG item = 0; item < 2; ++item)
            {
                line.top = clientHeight / 2 + 20 + item * 30;
                line.bottom = line.top + 24;
                SetTextColor(deviceContext,
                    item == runEndSelection ? RGB(255, 216, 0) : RGB(160, 160, 160));
                DrawTextW(deviceContext,
                    item == 0 ? L"\uB2E4\uC2DC \uC2DC\uC791" : L"\uD0C0\uC774\uD2C0\uB85C",
                    -1, &line, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        }
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
    EnemyRuntime enemies[enemyCount]{};
    LONG playerHP = 10;
    bool playerAlive = true;
    float playerHitRemaining = 0.0f;
    LONG dashDirectionX = 0;
    LONG dashDirectionY = 0;
    float dashDistanceRemaining = 0.0f;
    float dashCooldownRemaining = 0.0f;
    bool dashActive = false;
    float playerMoveSpeedCurrent = playerMoveSpeed;
    float slashCooldownDurationCurrent = slashCooldownDuration;
    float dashCooldownDurationCurrent = dashCooldownDuration;
    DWORD upgradeRandomState = 0x2468ACE1;
    LONG slashLeft = 0;
    LONG slashTop = 0;
    LONG slashRight = 0;
    LONG slashBottom = 0;
    float slashVisualRemaining = 0.0f;
    float slashCooldownRemaining = 0.0f;
    bool exitUnlocked = false;
    bool roomComplete = false;
    bool sequenceComplete = false;

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

        bool checkpointLoaded = false;
        if (loadGameRequested)
        {
            SaveCheckpoint checkpoint{};
            if (ReadCheckpoint(&checkpoint))
            {
                applicationState = gameplayState;
                runSeed = checkpoint.seed;
                currentRoom = checkpoint.room;
                playerHP = checkpoint.playerHP;
                playerMoveSpeedCurrent = checkpoint.moveSpeed;
                slashCooldownDurationCurrent = checkpoint.slashCooldown;
                dashCooldownDurationCurrent = checkpoint.dashCooldown;
                rerollUsed = checkpoint.reroll != 0;
                runKillCount = checkpoint.runKills;
                upgradeRandomState = 0x2468ACE1;
                upgradeOptionA = 0;
                upgradeOptionB = 1;
                titleStatus = 0;
                checkpointLoaded = true;
            }
            else
            {
                titleStatus = 1;
                InvalidateRect(window, nullptr, FALSE);
            }
            loadGameRequested = false;
        }

        if (newGameRequested || checkpointLoaded
            || (roomComplete && currentRoom < roomCount - 1))
        {
            if (newGameRequested)
            {
                LARGE_INTEGER seedTime{};
                QueryPerformanceCounter(&seedTime);
                runSeed = seedTime.LowPart ^ seedTime.HighPart ^ GetTickCount();
                if (!runSeed)
                {
                    runSeed = 0xA341316Cu;
                }
                currentRoom = 0;
                playerHP = 10;
                playerMoveSpeedCurrent = playerMoveSpeed;
                slashCooldownDurationCurrent = slashCooldownDuration;
                dashCooldownDurationCurrent = dashCooldownDuration;
                upgradeRandomState = 0x2468ACE1;
                rerollUsed = false;
                runKillCount = 0;
                upgradeOptionA = 0;
                upgradeOptionB = 1;
            }
            else if (!checkpointLoaded)
            {
                ++currentRoom;
            }
            SetupCurrentRoom();
            sequenceComplete = false;
            runEndState = 0;
            runEndSelection = 0;
            upgradeMenuActive = false;
            gameplayMenuActive = false;
            gameplayMenuSelection = 0;
            gameplayMenuSettingsStatus = false;
            upgradeSelection = 0;
            upgradeConfirmRequested = false;
            playerX = currentPlayerStartX;
            playerY = currentPlayerStartY;
            facingX = 1;
            facingY = 0;
            for (LONG enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
            {
                EnemyRuntime& enemy = enemies[enemyIndex];
                enemy = {};
                enemy.x = currentEnemyStartX[enemyIndex];
                enemy.y = currentEnemyStartY[enemyIndex];
                enemy.patrolReturnX = enemy.x;
                enemy.searchRandomState = RoomRandom(runSeed, currentRoom)
                    ^ (0x13579BDFu + 0x9E3779B9u * enemyIndex);
                enemy.hp = 3;
                enemy.patrolRight = currentPatrolStartsRight[enemyIndex];
                enemy.alive = true;
            }
            playerAlive = true;
            currentEnemyRemaining = enemyCount;
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
            exitUnlocked = false;
            roomComplete = false;
            slashRequested = false;
            dashRequested = false;
            executeRequested = false;
            gameplayInputBlocked = upPressed || downPressed || leftPressed || rightPressed
                || zPressed || xPressed || cPressed || spacePressed;
            if (!checkpointLoaded)
            {
                WriteCheckpoint(runSeed, currentRoom, playerHP, playerMoveSpeedCurrent,
                    slashCooldownDurationCurrent, dashCooldownDurationCurrent, rerollUsed,
                    runKillCount);
            }
            newGameRequested = false;
            QueryPerformanceCounter(&previousUpdate);
            InvalidateRect(window, nullptr, FALSE);
        }

        if (saveAndTitleRequested && applicationState == gameplayState)
        {
            WriteCheckpoint(runSeed, currentRoom, playerHP, playerMoveSpeedCurrent,
                slashCooldownDurationCurrent, dashCooldownDurationCurrent, rerollUsed,
                runKillCount);
            applicationState = titleMainState;
            menuSelection = 0;
            titleStatus = 0;
            gameplayMenuActive = false;
            gameplayMenuSelection = 0;
            gameplayMenuSettingsStatus = false;
            gameplayInputBlocked = upPressed || downPressed || leftPressed || rightPressed
                || zPressed || xPressed || cPressed || spacePressed;
            saveAndTitleRequested = false;
            QueryPerformanceCounter(&previousUpdate);
            InvalidateRect(window, nullptr, FALSE);
        }

        if (applicationState != gameplayState)
        {
            QueryPerformanceCounter(&previousUpdate);
            Sleep(1);
            continue;
        }

        if (runEndState)
        {
            QueryPerformanceCounter(&previousUpdate);
            Sleep(1);
            continue;
        }

        if (gameplayMenuActive)
        {
            QueryPerformanceCounter(&previousUpdate);
            Sleep(1);
            continue;
        }

        LARGE_INTEGER currentTime{};
        QueryPerformanceCounter(&currentTime);
        if (upgradeMenuActive)
        {
            previousUpdate = currentTime;
            if (upgradeConfirmRequested)
            {
                if (upgradeSelection == 2)
                {
                    if (!rerollUsed)
                    {
                        LONG missingUpgrade = 3 - upgradeOptionA - upgradeOptionB;
                        upgradeRandomState = upgradeRandomState * 1664525 + 1013904223;
                        if (upgradeRandomState & 1)
                        {
                            upgradeOptionB = upgradeOptionA;
                            upgradeOptionA = missingUpgrade;
                        }
                        else
                        {
                            upgradeOptionA = upgradeOptionB;
                            upgradeOptionB = missingUpgrade;
                        }
                        rerollUsed = true;
                        upgradeSelection = 0;
                        InvalidateRect(window, nullptr, FALSE);
                    }
                }
                else
                {
                    LONG upgrade = upgradeSelection ? upgradeOptionB : upgradeOptionA;
                    if (upgrade == 0)
                    {
                        playerMoveSpeedCurrent = playerMoveSpeed * 1.1f;
                    }
                    else if (upgrade == 1)
                    {
                        slashCooldownDurationCurrent = slashCooldownDuration * 0.8f;
                    }
                    else
                    {
                        dashCooldownDurationCurrent = dashCooldownDuration * 0.8f;
                    }
                    upgradeMenuActive = false;
                    if (currentRoom == roomCount - 1)
                    {
                        sequenceComplete = true;
                    }
                    else
                    {
                        roomComplete = true;
                    }
                    gameplayInputBlocked = upPressed || downPressed || leftPressed
                        || rightPressed || zPressed || xPressed || cPressed || spacePressed;
                }
                upgradeConfirmRequested = false;
            }
            if (upgradeMenuActive)
            {
                Sleep(1);
                continue;
            }
        }

        if (currentTime.QuadPart - previousUpdate.QuadPart >= updateInterval)
        {
            float deltaTime = static_cast<float>(currentTime.QuadPart - previousUpdate.QuadPart)
                / static_cast<float>(performanceFrequency.QuadPart);
            previousUpdate = currentTime;
            LONG movementX = gameplayInputBlocked || !playerAlive
                || roomComplete || sequenceComplete || upgradeMenuActive ? 0
                : static_cast<LONG>(rightPressed) - static_cast<LONG>(leftPressed);
            LONG movementY = gameplayInputBlocked || !playerAlive
                || roomComplete || sequenceComplete || upgradeMenuActive ? 0
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
                if (playerAlive && !roomComplete && !sequenceComplete
                    && !upgradeMenuActive && !dashActive
                    && dashCooldownRemaining <= 0.0f)
                {
                    dashDirectionX = facingX;
                    dashDirectionY = facingY;
                    dashDistanceRemaining = dashDistance;
                    dashCooldownRemaining = dashCooldownDurationCurrent;
                    dashActive = true;
                }
                dashRequested = false;
            }

            bool playerDashingThisUpdate = dashActive;
            float movementScale = movementX && movementY ? 0.70710678f : 1.0f;
            float movementDeltaX = movementX * playerMoveSpeedCurrent
                * movementScale * deltaTime;
            float movementDeltaY = movementY * playerMoveSpeedCurrent
                * movementScale * deltaTime;
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
                else if (nextPlayerX > worldWidth - playerHalfWidth)
                {
                    nextPlayerX = worldWidth - playerHalfWidth;
                }

                for (LONG wall = 0; movementDeltaX != 0.0f
                    && wall < currentWallCount; ++wall)
                {
                    if (nextPlayerX - playerHalfWidth < currentWallRight[wall]
                        && nextPlayerX + playerHalfWidth > currentWallLeft[wall]
                        && playerY - playerHalfHeight < currentWallBottom[wall]
                        && playerY + playerHalfHeight > currentWallTop[wall])
                    {
                        nextPlayerX = movementDeltaX > 0.0f
                            ? currentWallLeft[wall] - playerHalfWidth
                            : currentWallRight[wall] + playerHalfWidth;
                    }
                }
                playerX = nextPlayerX;

                float nextPlayerY = playerY + movementDeltaY;
                if (nextPlayerY < playerHalfHeight)
                {
                    nextPlayerY = playerHalfHeight;
                }
                else if (nextPlayerY > worldHeight - playerHalfHeight)
                {
                    nextPlayerY = worldHeight - playerHalfHeight;
                }

                for (LONG wall = 0; movementDeltaY != 0.0f
                    && wall < currentWallCount; ++wall)
                {
                    if (playerX - playerHalfWidth < currentWallRight[wall]
                        && playerX + playerHalfWidth > currentWallLeft[wall]
                        && nextPlayerY - playerHalfHeight < currentWallBottom[wall]
                        && nextPlayerY + playerHalfHeight > currentWallTop[wall])
                    {
                        nextPlayerY = movementDeltaY > 0.0f
                            ? currentWallTop[wall] - playerHalfHeight
                            : currentWallBottom[wall] + playerHalfHeight;
                    }
                }
                playerY = nextPlayerY;
            }

            for (LONG enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
            {
                EnemyRuntime& enemy = enemies[enemyIndex];
                float& enemyX = enemy.x;
                float& enemyY = enemy.y;
                float& enemyFacingAngle = enemy.facingAngle;
                bool& enemyPatrolRight = enemy.patrolRight;
                bool& enemyReturningToPatrol = enemy.returningToPatrol;
                float& enemyPatrolReturnX = enemy.patrolReturnX;
                bool& playerInVision = enemy.playerInVision;
                float& lastSeenPlayerX = enemy.lastSeenPlayerX;
                float& lastSeenPlayerY = enemy.lastSeenPlayerY;
                bool& lastSeenPlayerValid = enemy.lastSeenPlayerValid;
                float& alertLostElapsed = enemy.alertLostElapsed;
                DWORD& searchRandomState = enemy.searchRandomState;
                float& searchTargetX = enemy.searchTargetX;
                float& searchTargetY = enemy.searchTargetY;
                bool& searchTargetValid = enemy.searchTargetValid;
                LONG& searchPathCount = enemy.searchPathCount;
                LONG& searchPathIndex = enemy.searchPathIndex;
                bool& enemyScanning = enemy.scanning;
                float& scanElapsed = enemy.scanElapsed;
                float& scanBaseFacing = enemy.scanBaseFacing;
                float& detectionProgress = enemy.detectionProgress;
                float& lostSightElapsed = enemy.lostSightElapsed;
                bool& enemyAlert = enemy.alert;
                bool& enemyAlive = enemy.alive;

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
                            if (enemyPatrolReturnX < currentPatrolLeft[enemyIndex])
                            {
                                enemyPatrolReturnX = currentPatrolLeft[enemyIndex];
                            }
                            else if (enemyPatrolReturnX > currentPatrolRight[enemyIndex])
                            {
                                enemyPatrolReturnX = currentPatrolRight[enemyIndex];
                            }
                            enemyReturningToPatrol
                                = (enemyX - enemyPatrolReturnX) * (enemyX - enemyPatrolReturnX)
                                + (enemyY - currentEnemyStartY[enemyIndex])
                                * (enemyY - currentEnemyStartY[enemyIndex]) > 0.25f;
                            if (enemyReturningToPatrol)
                            {
                                searchPathCount = FindEnemyPath(enemyX, enemyY,
                                    NavigationColumn(enemyPatrolReturnX),
                                    NavigationRow(currentEnemyStartY[enemyIndex]),
                                    navigationPath[enemyIndex]);
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
                                        || candidateX > worldWidth - enemyHalfWidth
                                        || candidateY < enemyHalfHeight
                                        || candidateY > worldHeight - enemyHalfHeight
                                        || !NavigationCellValid(targetColumn, targetRow)
                                        || RectangleOverlapsRoomWall(
                                            candidateX - enemyHalfWidth,
                                            candidateY - enemyHalfHeight,
                                            candidateX + enemyHalfWidth,
                                            candidateY + enemyHalfHeight))
                                    {
                                        continue;
                                    }
                                    searchTargetX = candidateX;
                                    searchTargetY = candidateY;
                                    searchPathCount = FindEnemyPath(enemyX, enemyY,
                                        targetColumn, targetRow, navigationPath[enemyIndex]);
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
                                    LONG node = navigationPath[enemyIndex][searchPathIndex];
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
                        ? currentPatrolRight[enemyIndex] : currentPatrolLeft[enemyIndex];
                    float movementTargetX = enemyReturningToPatrol
                        ? enemyPatrolReturnX : patrolTargetX;
                    float movementTargetY = currentEnemyStartY[enemyIndex];
                    if (enemyReturningToPatrol && searchPathIndex < searchPathCount)
                    {
                        LONG node = navigationPath[enemyIndex][searchPathIndex];
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
            }

            if (slashVisualRemaining > 0.0f)
            {
                slashVisualRemaining -= deltaTime;
            }
            if (slashCooldownRemaining > 0.0f)
            {
                slashCooldownRemaining -= deltaTime;
            }
            if (playerHitRemaining > 0.0f)
            {
                playerHitRemaining -= deltaTime;
            }
            for (LONG enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
            {
                EnemyRuntime& enemy = enemies[enemyIndex];
                if (enemy.hitRemaining > 0.0f)
                {
                    enemy.hitRemaining -= deltaTime;
                }
                if (enemy.attackCooldownRemaining > 0.0f)
                {
                    enemy.attackCooldownRemaining -= deltaTime;
                }
                if (enemy.executeFeedbackRemaining > 0.0f)
                {
                    enemy.executeFeedbackRemaining -= deltaTime;
                }
            }

            if (slashRequested)
            {
                if (playerAlive && !roomComplete && !sequenceComplete && !upgradeMenuActive
                    && !playerDashingThisUpdate
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
                    slashCooldownRemaining = slashCooldownDurationCurrent;
                    for (LONG enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
                    {
                        EnemyRuntime& enemy = enemies[enemyIndex];
                        LONG enemyLeft = static_cast<LONG>(enemy.x) - enemyWidth / 2;
                        LONG enemyTop = static_cast<LONG>(enemy.y) - enemyHeight / 2;
                        if (enemy.alive
                            && slashLeft < enemyLeft + enemyWidth
                            && slashRight > enemyLeft
                            && slashTop < enemyTop + enemyHeight
                            && slashBottom > enemyTop)
                        {
                            enemy.hitRemaining = slashVisualDuration;
                            --enemy.hp;
                            if (enemy.hp <= 0)
                            {
                                enemy.hp = 0;
                                enemy.alive = false;
                                --currentEnemyRemaining;
                                ++runKillCount;
                                slashCooldownRemaining = 0.0f;
                                dashCooldownRemaining = 0.0f;
                                enemy.playerInVision = false;
                                enemy.searchTargetValid = false;
                                enemy.searchPathCount = 0;
                                enemy.searchPathIndex = 0;
                                enemy.scanning = false;
                            }
                        }
                    }
                }
                slashRequested = false;
            }

            if (executeRequested)
            {
                if (playerAlive && !roomComplete && !sequenceComplete
                    && !upgradeMenuActive)
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

                    LONG selectedEnemy = -1;
                    float nearestDistanceSquared = 3.402823466e+38F;
                    for (LONG enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
                    {
                        EnemyRuntime& enemy = enemies[enemyIndex];
                        LONG enemyLeft = static_cast<LONG>(enemy.x) - enemyWidth / 2;
                        LONG enemyTop = static_cast<LONG>(enemy.y) - enemyHeight / 2;
                        float differenceX = enemy.x - playerX;
                        float differenceY = enemy.y - playerY;
                        float distanceSquared = differenceX * differenceX
                            + differenceY * differenceY;
                        if (enemy.alive && enemy.detectionProgress <= 0.0f
                            && executeLeft < enemyLeft + enemyWidth
                            && executeRight > enemyLeft
                            && executeTop < enemyTop + enemyHeight
                            && executeBottom > enemyTop
                            && distanceSquared < nearestDistanceSquared)
                        {
                            selectedEnemy = enemyIndex;
                            nearestDistanceSquared = distanceSquared;
                        }
                    }
                    if (selectedEnemy >= 0)
                    {
                        EnemyRuntime& enemy = enemies[selectedEnemy];
                        enemy.hp = 0;
                        enemy.alive = false;
                        --currentEnemyRemaining;
                        ++runKillCount;
                        enemy.executeFeedbackRemaining = slashVisualDuration;
                        slashCooldownRemaining = 0.0f;
                        dashCooldownRemaining = 0.0f;
                        enemy.playerInVision = false;
                        enemy.searchTargetValid = false;
                        enemy.searchPathCount = 0;
                        enemy.searchPathIndex = 0;
                        enemy.scanning = false;
                    }
                }
                executeRequested = false;
            }

            for (LONG enemyIndex = 0; enemyIndex < enemyCount && playerAlive; ++enemyIndex)
            {
                EnemyRuntime& enemy = enemies[enemyIndex];
                if (enemy.alive && enemy.alert && enemy.attackCooldownRemaining <= 0.0f
                    && enemy.x - enemyHalfWidth <= playerX + playerHalfWidth
                        + enemyAttackContactTolerance
                    && enemy.x + enemyHalfWidth + enemyAttackContactTolerance
                        >= playerX - playerHalfWidth
                    && enemy.y - enemyHalfHeight <= playerY + playerHalfHeight
                        + enemyAttackContactTolerance
                    && enemy.y + enemyHalfHeight + enemyAttackContactTolerance
                        >= playerY - playerHalfHeight)
                {
                    --playerHP;
                    playerHitRemaining = playerHitFeedbackDuration;
                    enemy.attackCooldownRemaining = enemyAttackCooldownDuration;
                    if (playerHP <= 0)
                    {
                        playerHP = 0;
                        playerAlive = false;
                        dashActive = false;
                        dashDistanceRemaining = 0.0f;
                        slashRequested = false;
                        dashRequested = false;
                        executeRequested = false;
                        for (LONG otherEnemy = 0; otherEnemy < enemyCount; ++otherEnemy)
                        {
                            enemies[otherEnemy].playerInVision = false;
                        }
                        runEndState = gameOverEndState;
                        runEndSelection = 0;
                        DeleteFileW(saveFileName);
                    }
                }
            }

            exitUnlocked = playerAlive && currentEnemyRemaining == 0;
            if (playerAlive && exitUnlocked && !roomComplete && !sequenceComplete
                && playerX - playerHalfWidth < currentExitRight
                && playerX + playerHalfWidth > currentExitLeft
                && playerY - playerHalfHeight < currentExitBottom
                && playerY + playerHalfHeight > currentExitTop)
            {
                if ((currentRoom & 1) == 0)
                {
                    roomComplete = true;
                }
                else
                {
                    upgradeRandomState = RoomRandom(runSeed, currentRoom) ^ 0x2468ACE1u;
                    upgradeRandomState = upgradeRandomState * 1664525 + 1013904223;
                    upgradeOptionA = upgradeRandomState % 3;
                    upgradeRandomState = upgradeRandomState * 1664525 + 1013904223;
                    upgradeOptionB = (upgradeOptionA + 1 + (upgradeRandomState & 1)) % 3;
                    upgradeMenuActive = true;
                    upgradeSelection = 0;
                    upgradeConfirmRequested = false;
                    gameplayInputBlocked = upPressed || downPressed || leftPressed
                        || rightPressed || zPressed || xPressed || cPressed || spacePressed;
                }
                dashActive = false;
                dashDistanceRemaining = 0.0f;
                slashRequested = false;
                dashRequested = false;
                executeRequested = false;
            }
            LONG cameraX = static_cast<LONG>(playerX) - framebufferWidth / 2;
            LONG cameraY = static_cast<LONG>(playerY) - framebufferHeight / 2;
            LONG maximumCameraX = worldWidth - framebufferWidth;
            LONG maximumCameraY = worldHeight - framebufferHeight;
            if (maximumCameraX < 0)
            {
                maximumCameraX = 0;
            }
            if (maximumCameraY < 0)
            {
                maximumCameraY = 0;
            }
            if (cameraX < 0)
            {
                cameraX = 0;
            }
            else if (cameraX > maximumCameraX)
            {
                cameraX = maximumCameraX;
            }
            if (cameraY < 0)
            {
                cameraY = 0;
            }
            else if (cameraY > maximumCameraY)
            {
                cameraY = maximumCameraY;
            }

            for (LONG pixel = 0; pixel < framebufferWidth * framebufferHeight; ++pixel)
            {
                framebuffer[pixel] = 0x00101018;
            }

            LONG worldCenterScreenY = worldHeight / 2 - cameraY;
            if (worldCenterScreenY >= 0 && worldCenterScreenY < framebufferHeight)
            {
                for (LONG x = 0; x < framebufferWidth; ++x)
                {
                    framebuffer[worldCenterScreenY * framebufferWidth + x] = 0x00404040;
                }
            }
            LONG worldCenterScreenX = worldWidth / 2 - cameraX;
            if (worldCenterScreenX >= 0 && worldCenterScreenX < framebufferWidth)
            {
                for (LONG y = 0; y < framebufferHeight; ++y)
                {
                    framebuffer[y * framebufferWidth + worldCenterScreenX] = 0x00404040;
                }
            }
            if (cameraY == 0)
            {
                for (LONG x = 0; x < framebufferWidth; ++x)
                {
                    framebuffer[x] = 0x00800000;
                }
            }
            if (cameraY == maximumCameraY)
            {
                LONG boundaryY = worldHeight - 1 - cameraY;
                for (LONG x = 0; x < framebufferWidth; ++x)
                {
                    if (boundaryY >= 0 && boundaryY < framebufferHeight)
                    {
                        framebuffer[boundaryY * framebufferWidth + x] = 0x00000080;
                    }
                }
            }
            if (cameraX == 0)
            {
                for (LONG y = 0; y < framebufferHeight; ++y)
                {
                    framebuffer[y * framebufferWidth] = 0x00008000;
                }
            }
            if (cameraX == maximumCameraX)
            {
                LONG boundaryX = worldWidth - 1 - cameraX;
                if (boundaryX >= 0 && boundaryX < framebufferWidth)
                {
                    for (LONG y = 0; y < framebufferHeight; ++y)
                    {
                        framebuffer[y * framebufferWidth + boundaryX] = 0x00808000;
                    }
                }
            }

            for (LONG enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
            {
                EnemyRuntime& enemy = enemies[enemyIndex];
                LONG enemyCenterX = static_cast<LONG>(enemy.x);
                LONG enemyCenterY = static_cast<LONG>(enemy.y);
                if (!enemy.alive || enemy.alert)
                {
                    continue;
                }
                float redDistance = enemyVisionRange * enemy.detectionProgress;
                float enemyFacingX = cosf(enemy.facingAngle);
                float enemyFacingY = sinf(enemy.facingAngle);
                LONG visionLeft = enemyCenterX - enemyVisionRange - cameraX;
                LONG visionRight = enemyCenterX + enemyVisionRange - cameraX;
                LONG visionTop = enemyCenterY - enemyVisionRange - cameraY;
                LONG visionBottom = enemyCenterY + enemyVisionRange - cameraY;
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
                        float worldPixelX = static_cast<float>(pixelX + cameraX);
                        float worldPixelY = static_cast<float>(pixelY + cameraY);
                        float visionX = worldPixelX - enemy.x;
                        float visionY = worldPixelY - enemy.y;
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
                            && !WallBlocksSegment(enemy.x, enemy.y,
                                worldPixelX, worldPixelY))
                        {
                            framebuffer[pixelY * framebufferWidth + pixelX]
                                = visionForward <= redDistance ? 0x00401818 : 0x00182040;
                        }
                    }
                }
            }

            for (LONG wall = 0; wall < currentWallCount; ++wall)
            {
                for (LONG y = currentWallTop[wall]; y < currentWallBottom[wall]; ++y)
                {
                    for (LONG x = currentWallLeft[wall]; x < currentWallRight[wall]; ++x)
                    {
                        LONG screenX = x - cameraX;
                        LONG screenY = y - cameraY;
                        if (screenX >= 0 && screenX < framebufferWidth
                            && screenY >= 0 && screenY < framebufferHeight)
                        {
                            framebuffer[screenY * framebufferWidth + screenX] = 0x00606070;
                        }
                    }
                }
            }

            for (LONG y = currentExitTop; y < currentExitBottom; ++y)
            {
                for (LONG x = currentExitLeft; x < currentExitRight; ++x)
                {
                    LONG screenX = x - cameraX;
                    LONG screenY = y - cameraY;
                    if (screenX >= 0 && screenX < framebufferWidth
                        && screenY >= 0 && screenY < framebufferHeight)
                    {
                        framebuffer[screenY * framebufferWidth + screenX]
                            = exitUnlocked ? 0x0040E080 : 0x00282830;
                    }
                }
            }

            if (slashVisualRemaining > 0.0f)
            {
                for (LONG y = slashTop; y < slashBottom; ++y)
                {
                    for (LONG x = slashLeft; x < slashRight; ++x)
                    {
                        LONG screenX = x - cameraX;
                        LONG screenY = y - cameraY;
                        if (screenX >= 0 && screenX < framebufferWidth
                            && screenY >= 0 && screenY < framebufferHeight
                            && (x == slashLeft || x == slashRight - 1
                                || y == slashTop || y == slashBottom - 1))
                        {
                            framebuffer[screenY * framebufferWidth + screenX] = 0x00E0E0E0;
                        }
                    }
                }
            }

            for (LONG enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
            {
                EnemyRuntime& enemy = enemies[enemyIndex];
                LONG enemyCenterX = static_cast<LONG>(enemy.x);
                LONG enemyCenterY = static_cast<LONG>(enemy.y);
                LONG enemyLeft = enemyCenterX - enemyWidth / 2;
                LONG enemyTop = enemyCenterY - enemyHeight / 2;
                if (enemy.alive)
                {
                    for (LONG y = 0; y < enemyHeight; ++y)
                    {
                        for (LONG x = 0; x < enemyWidth; ++x)
                        {
                            bool head = y < 3 && x >= 1 && x < 7;
                            bool body = y >= 3 && y < 9 && x >= 2 && x < 6;
                            bool arm = y >= 4 && y < 8 && (x == 0 || x == 7);
                            bool leg = y >= 9 && (x < 3 || x >= 5);
                            LONG pixelX = enemyLeft + x - cameraX;
                            LONG pixelY = enemyTop + y - cameraY;
                            if ((head || body || arm || leg)
                                && pixelX >= 0 && pixelX < framebufferWidth
                                && pixelY >= 0 && pixelY < framebufferHeight)
                            {
                                framebuffer[pixelY * framebufferWidth + pixelX]
                                    = enemy.hitRemaining > 0.0f
                                        ? 0x00FFFFFF
                                        : (head ? 0x00FF4040 : 0x00A02020);
                            }
                        }
                    }
                }

                if (enemy.alive && enemy.detectionProgress > 0.0f)
                {
                    for (LONG y = 0; y < 5; ++y)
                    {
                        for (LONG x = 0; x < 3; ++x)
                        {
                            bool symbolPixel = enemy.alert
                                ? (x == 1 && y != 3)
                                : ((y == 0 && x < 2) || (y == 1 && x == 2)
                                    || (y == 2 && x == 1) || (y == 4 && x == 1));
                            LONG symbolX = enemyCenterX - 1 + x - cameraX;
                            LONG symbolY = enemyTop - 7 + y - cameraY;
                            if (symbolPixel && symbolX >= 0 && symbolX < framebufferWidth
                                && symbolY >= 0 && symbolY < framebufferHeight)
                            {
                                framebuffer[symbolY * framebufferWidth + symbolX]
                                    = enemy.alert ? 0x00FF4040 : 0x00FFD800;
                            }
                        }
                    }
                }

                if (enemy.executeFeedbackRemaining > 0.0f)
                {
                    for (LONG y = enemyCenterY - 1; y <= enemyCenterY + 1; ++y)
                    {
                        for (LONG x = enemyCenterX - 1; x <= enemyCenterX + 1; ++x)
                        {
                            LONG screenX = x - cameraX;
                            LONG screenY = y - cameraY;
                            if (screenX >= 0 && screenX < framebufferWidth
                                && screenY >= 0 && screenY < framebufferHeight)
                            {
                                framebuffer[screenY * framebufferWidth + screenX] = 0x00FFFF80;
                            }
                        }
                    }
                }
            }

            LONG drawingLeft = static_cast<LONG>(playerX) - playerWidth / 2 - cameraX;
            LONG drawingTop = static_cast<LONG>(playerY) - playerHeight / 2 - cameraY;
            constexpr LONG healthBarWidth = 12;
            constexpr LONG healthBarHeight = 2;
            LONG healthBarLeft = static_cast<LONG>(playerX) - healthBarWidth / 2 - cameraX;
            LONG healthBarTop = static_cast<LONG>(playerY) - playerHeight / 2 - 4 - cameraY;
            LONG healthFillWidth = playerHP * healthBarWidth / 10;
            for (LONG y = 0; y < healthBarHeight; ++y)
            {
                for (LONG x = 0; x < healthBarWidth; ++x)
                {
                    LONG pixelX = healthBarLeft + x;
                    LONG pixelY = healthBarTop + y;
                    if (pixelX >= 0 && pixelX < framebufferWidth
                        && pixelY >= 0 && pixelY < framebufferHeight)
                    {
                        framebuffer[pixelY * framebufferWidth + pixelX]
                            = x < healthFillWidth ? 0x0040D060 : 0x00302028;
                    }
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

            if (sequenceComplete)
            {
                constexpr unsigned short runClearLetters[8]
                {
                    0b101'110'111'101'111,
                    0b111'101'101'101'101,
                    0b101'111'111'111'101,
                    0b111'100'100'100'111,
                    0b111'100'100'100'100,
                    0b111'100'110'100'111,
                    0b101'101'111'101'111,
                    0b101'110'111'101'111
                };
                constexpr LONG runClearLeft = framebufferWidth / 2 - 17;
                constexpr LONG runClearTop = framebufferHeight / 2 - 14;
                for (LONG y = 0; y < 5; ++y)
                {
                    for (LONG x = 0; x < 35; ++x)
                    {
                        LONG letter = x < 12 ? x / 4 : (x >= 16 ? x / 4 - 1 : -1);
                        LONG letterX = x & 3;
                        if (letter >= 0 && letterX < 3
                            && (runClearLetters[letter] & (1 << (y * 3 + (2 - letterX)))))
                        {
                            framebuffer[(runClearTop + y) * framebufferWidth
                                + runClearLeft + x]
                                = 0x0080FFC0;
                        }
                    }
                }
            }

            if (sequenceComplete && !runEndState)
            {
                runEndState = runClearEndState;
                runEndSelection = 0;
                DeleteFileW(saveFileName);
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
