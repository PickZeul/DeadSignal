#include <windows.h>
#include <timeapi.h>
#include <math.h>
#if defined(DEAD_SIGNAL_B01_VALIDATION) || defined(DEAD_SIGNAL_B02_VALIDATION) \
    || defined(DEAD_SIGNAL_B03_VALIDATION) || defined(DEAD_SIGNAL_B101_VALIDATION)
#include <stdio.h>
#endif


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
constexpr LONG combatScale = 10;
constexpr LONG RoundCombatRatio(LONG value, LONG numerator, LONG denominator)
{
    return (value * numerator + denominator / 2) / denominator;
}
constexpr LONG basicPlayerMaxHP = 10 * combatScale;
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
constexpr LONG roomCount = 12;
constexpr LONG openRoomType = 0;
constexpr LONG pillarRoomType = 1;
constexpr LONG mazeRoomType = 2;
constexpr LONG trapRoomType = 3;
constexpr LONG mixedRoomType = 4;
constexpr LONG maxRoomWalls = 24;
constexpr LONG maxRoomTraps = 19;
constexpr LONG mazeWallThickness = 8;
constexpr LONG trapWidth = 24;
constexpr LONG trapHeight = 14;
constexpr float trapOffDuration = 1.25f;
constexpr float trapActiveDuration = 0.75f;
constexpr float trapCycleDuration = trapOffDuration + trapActiveDuration;
constexpr float trapDamageCooldownDuration = 0.5f;
constexpr LONG maxEnemyCount = 24;
constexpr LONG enemyRoleCount = 5;
constexpr BYTE patrollerEnemyRole = 0;
constexpr BYTE watcherEnemyRole = 1;
constexpr BYTE hunterEnemyRole = 2;
constexpr BYTE listenerEnemyRole = 3;
constexpr BYTE spinnerEnemyRole = 4;
constexpr BYTE pressureEnemyRole = 5;
constexpr LONG pressureEnemyIndex = maxEnemyCount;
constexpr LONG pressureVisualWidth = 10;
constexpr LONG pressureVisualHeight = 12;
constexpr float pressureMoveSpeed = 32.0f;
constexpr float pressureMinimumSpawnDistanceSquared = 80.0f * 80.0f;
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
constexpr float hunterAlertSpeed = 62.0f;
constexpr LONG enemyVisionRange = 70;
constexpr float enemyVisionSlope = 0.520567f;
constexpr float enemyVisionRangeSquared
    = static_cast<float>(enemyVisionRange * enemyVisionRange);
constexpr float enemyReacquireRangeSquared = 42.0f * 42.0f;
constexpr float hunterReacquireRangeSquared = 70.0f * 70.0f;
constexpr float listenerMovementHearingRangeSquared = 80.0f * 80.0f;
constexpr float listenerDashHearingRangeSquared = 120.0f * 120.0f;
constexpr float slashLocalAlertRangeSquared = 70.0f * 70.0f;
constexpr float listenerAlertRangeSquared = 80.0f * 80.0f;
constexpr float spinnerRotationSpeed = 1.04719755f;
constexpr float enemyFacingTurnSpeed = 2.0943951f;
constexpr float enemyScanAngle = 0.47996554f;
constexpr float enemyAlertSearchDuration = 10.0f;
constexpr float hunterAlertSearchDuration = 15.0f;
constexpr float enemyScanDuration = 2.0f;
constexpr float enemyAttackCooldownDuration = 1.0f;
constexpr float enemyAttackWindupDuration = 0.3f;
constexpr float enemyAttackContactTolerance = 1.0f;
constexpr float playerHitFeedbackDuration = 0.10f;
constexpr float playerInvulnerabilityDuration = 0.5f;
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
constexpr float executeFacingCosineSquared = 0.007596123f;
constexpr LONG moveUpgrade = 0;
constexpr LONG slashUpgrade = 1;
constexpr LONG dashUpgrade = 2;
constexpr LONG silentDashUpgrade = 3;
constexpr LONG executeReachUpgrade = 4;
constexpr LONG fieldMedicUpgrade = 5;
constexpr LONG upgradeCount = 6;
constexpr BYTE silentDashUpgradeFlag = 1;
constexpr BYTE executeReachUpgradeFlag = 2;
constexpr BYTE fieldMedicUpgradeFlag = 4;
constexpr float dashDistance = 32.0f;
constexpr float dashDuration = 0.12f;
constexpr float dashCooldownDuration = 1.5f;
constexpr LONG playerDamageAmount = combatScale;
constexpr LONG healAmount = combatScale;
constexpr LONG enemyMaximumHP = 3 * combatScale;
constexpr LONG piercerSlashDamage = RoundCombatRatio(combatScale, 3, 2);
constexpr BYTE basicCharacter = 0;
constexpr BYTE mobilityCharacter = 1;
constexpr BYTE piercerCharacter = 2;
constexpr BYTE heavyCharacter = 3;
constexpr BYTE rapidCharacter = 4;
constexpr BYTE characterCount = 5;
constexpr LONG characterUnlockCosts[characterCount]{ 0, 5, 10, 15, 20 };
constexpr BYTE characterDashCaps[characterCount]{ 3, 4, 2, 1, 2 };
constexpr LONG runClearCoinBonus = 3;
constexpr BYTE commonUpgradeCount = 3;
constexpr BYTE characterGlobalUpgradeCount = 3;
constexpr BYTE pierceThroughStorageMarker = 0x80;
constexpr BYTE runRerollGlobalUpgrade = 0;
constexpr BYTE fieldRecoveryGlobalUpgrade = 1;
constexpr BYTE coinSenseGlobalUpgrade = 2;
constexpr BYTE commonUpgradeMaximum[commonUpgradeCount]{ 1, 1, 2 };
constexpr LONG commonUpgradeCost[commonUpgradeCount][2]
    { { 10, 0 }, { 10, 0 }, { 5, 10 } };
constexpr BYTE characterGlobalMaximum[characterCount][characterGlobalUpgradeCount]
{
    { 2, 2, 2 },
    { 3, 2, 2 },
    { 1, 2, 1 },
    { 2, 2, 2 },
    { 1, 2, 2 }
};
constexpr LONG characterGlobalCost[3]{ 5, 10, 15 };

struct CharacterProfile
{
    LONG maxHP;
    float moveScale;
    float slashCooldownScale;
    LONG slashReach;
    LONG slashWidth;
    LONG slashDamage;
    LONG slashHitCap;
    float dashDistanceScale;
};

constexpr CharacterProfile characterProfiles[characterCount]
{
    { basicPlayerMaxHP, 1.0f, 1.0f, slashReach, slashWidth, 10, 3, 1.0f },
    { 8 * combatScale, 68.0f / playerMoveSpeed, 1.0f,
        slashReach - 1, slashWidth - 1, 10, 2, 1.1f },
    { 7 * combatScale, 0.95f, 1.2f,
        slashReach * 2, 3, piercerSlashDamage, 1, 1.0f },
    { 14 * combatScale, 0.75f, 1.5f,
        slashReach - 2, slashWidth + 4, 20, 4, 0.9f },
    { 8 * combatScale, 1.1f, 0.5f,
        slashReach - 2, slashWidth - 2, 10, 2, 1.0f }
};
constexpr DWORD toneSampleRate = 8000;
constexpr DWORD toneFrequency = 440;
constexpr DWORD toneDurationMilliseconds = 250;
constexpr DWORD toneSampleCount = toneSampleRate * toneDurationMilliseconds / 1000;

DWORD framebuffer[framebufferWidth * framebufferHeight];

constexpr LONG titleMainState = 0;
constexpr LONG preparationState = 1;
constexpr LONG characterSelectState = 2;
constexpr LONG gameplayState = 3;
constexpr LONG gameOverEndState = 1;
constexpr LONG runClearEndState = 2;
constexpr DWORD saveMagic = 0x56535344;
constexpr DWORD saveVersion = 8;
constexpr wchar_t saveFileName[] = L"DeadSignal.sav";
constexpr DWORD metaMagic = 0x4154454D;
constexpr DWORD metaVersion = 2;
constexpr wchar_t metaFileName[] = L"DeadSignal.meta";
LONG applicationState = titleMainState;
LONG menuSelection = 0;
LONG titleStatus = 0;
bool settingsActive = false;
bool settingsFromGameplay = false;
LONG settingsSelection = 0;
bool helpActive = false;
bool helpFromGameplay = false;
bool audioEnabled = true;
bool newGameRequested = false;
bool loadGameRequested = false;
LONG currentRoom = 0;
DWORD runSeed = 0;
LONG runKillCount = 0;
LONG runEarnedCoin = 0;
BYTE alertEventCount = 0;
bool alertEventActive = false;
LONG currentEnemyCount = 2;
LONG currentEnemyRemaining = 2;
LONG currentRoomType = openRoomType;
LONG currentLayoutVariant = 0;
LONG currentExitSide = 1;
LONG currentWallCount = 0;
LONG currentWallLeft[maxRoomWalls]{};
LONG currentWallTop[maxRoomWalls]{};
LONG currentWallRight[maxRoomWalls]{};
LONG currentWallBottom[maxRoomWalls]{};
LONG currentTrapCount = 0;
LONG currentTrapLeft[maxRoomTraps]{};
LONG currentTrapTop[maxRoomTraps]{};
LONG currentTrapRight[maxRoomTraps]{};
LONG currentTrapBottom[maxRoomTraps]{};
float currentTrapPhaseOffset[maxRoomTraps]{};
LONG currentExitLeft = exitLeft;
LONG currentExitTop = exitTop;
LONG currentExitRight = exitRight;
LONG currentExitBottom = exitBottom;
float currentPlayerStartX = static_cast<float>(playerCenterX);
float currentPlayerStartY = static_cast<float>(playerCenterY);
float currentEnemyStartX[maxEnemyCount]{};
float currentEnemyStartY[maxEnemyCount]{};
float currentEnemyStartFacing[maxEnemyCount]{};
float currentPatrolLeft[maxEnemyCount]{};
float currentPatrolRight[maxEnemyCount]{};
bool currentPatrolStartsRight[maxEnemyCount]{};
BYTE currentEnemyRole[maxEnemyCount]{};
bool currentPressureActive = false;
float currentPressureStartX = 0.0f;
float currentPressureStartY = 0.0f;
LONG runEndState = 0;
LONG runEndSelection = 0;
bool upgradeMenuActive = false;
LONG upgradeSelection = 0;
LONG upgradeOptionA = 0;
LONG upgradeOptionB = 1;
BYTE selectedCharacter = basicCharacter;
LONG globalCoin = 0;
BYTE unlockedCharacterMask = 1 << basicCharacter;
BYTE commonGlobalLevel[commonUpgradeCount]{};
BYTE characterGlobalLevel[characterCount][characterGlobalUpgradeCount]{};
BYTE currentDashCharges = 1;
BYTE currentDashMaxCharges = 1;
bool runRewardGranted = false;
BYTE moveUpgradeStack = 0;
BYTE slashUpgradeStack = 0;
BYTE dashUpgradeStack = 0;
BYTE functionalUpgradeFlags = 0;
BYTE rerollUsed = 0;
bool characterUpgradeFocus = false;
LONG characterUpgradeSelection = 0;
bool upgradeConfirmRequested = false;
bool gameplayMenuActive = false;
LONG gameplayMenuSelection = 0;
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
unsigned short navigationPath[maxEnemyCount + 1][maxNavigationNodeCount];

struct EnemyRuntime
{
    float x;
    float y;
    float facingAngle;
    float surveillanceFacingAngle;
    float patrolReturnX;
    float lastSeenPlayerX;
    float lastSeenPlayerY;
    float alertLostElapsed;
    float searchTargetX;
    float searchTargetY;
    float scanElapsed;
    float scanBaseFacing;
    float attackCooldownRemaining;
    float attackWindupRemaining;
    float hitRemaining;
    float executeFeedbackRemaining;
    float detectionProgress;
    float lostSightElapsed;
    float patrolTurnRemaining;
    float patrolTurnX;
    float patrolTurnY;
    DWORD searchRandomState;
    LONG searchPathCount;
    LONG searchPathIndex;
    LONG hp;
    BYTE role;
    bool patrolRight;
    bool returningToPatrol;
    bool playerInVision;
    bool lastSeenPlayerValid;
    bool heardSuspicion;
    bool searchTargetValid;
    bool scanning;
    bool alert;
    bool alive;
};

bool EnemyVisibleInViewport(const EnemyRuntime& enemy, LONG cameraX, LONG cameraY)
{
    constexpr LONG visibilityMargin = 2;
    LONG left = static_cast<LONG>(enemy.x) - enemyWidth / 2 - cameraX;
    LONG top = static_cast<LONG>(enemy.y) - enemyHeight / 2 - cameraY;
    return left < framebufferWidth + visibilityMargin
        && left + enemyWidth > -visibilityMargin
        && top < framebufferHeight + visibilityMargin
        && top + enemyHeight > -visibilityMargin;
}

void PutLocatorPixel(LONG x, LONG y)
{
    if (x >= 0 && x < framebufferWidth && y >= 0 && y < framebufferHeight)
    {
        framebuffer[y * framebufferWidth + x] = 0x00FFD060;
    }
}

bool DrawEnemyLocatorArrow(const EnemyRuntime& enemy, LONG cameraX, LONG cameraY,
    LONG arrowIndex)
{
    if (!enemy.alive || EnemyVisibleInViewport(enemy, cameraX, cameraY))
    {
        return false;
    }

    constexpr float centerX = framebufferWidth * 0.5f;
    constexpr float centerY = framebufferHeight * 0.5f;
    constexpr LONG leftInset = 10;
    constexpr LONG rightInset = framebufferWidth - 11;
    constexpr LONG topInset = 20;
    constexpr LONG bottomInset = framebufferHeight - 11;
    float dx = enemy.x - cameraX - centerX;
    float dy = enemy.y - cameraY - centerY;
    float edgeScale = 1000000.0f;
    if (dx > 0.0f)
    {
        edgeScale = (rightInset - centerX) / dx;
    }
    else if (dx < 0.0f)
    {
        edgeScale = (leftInset - centerX) / dx;
    }
    if (dy != 0.0f)
    {
        float verticalScale = (dy > 0.0f ? bottomInset - centerY
            : topInset - centerY) / dy;
        if (verticalScale < edgeScale)
        {
            edgeScale = verticalScale;
        }
    }

    LONG directionX = dx < 0.0f ? -1 : 1;
    LONG directionY = dy < 0.0f ? -1 : 1;
    float absoluteX = dx < 0.0f ? -dx : dx;
    float absoluteY = dy < 0.0f ? -dy : dy;
    if (absoluteX > absoluteY * 2.0f)
    {
        directionY = 0;
    }
    else if (absoluteY > absoluteX * 2.0f)
    {
        directionX = 0;
    }

    constexpr LONG offsets[5]{ 0, -4, 4, -8, 8 };
    LONG perpendicularX = -directionY;
    LONG perpendicularY = directionX;
    LONG tipX = static_cast<LONG>(centerX + dx * edgeScale + 0.5f)
        + perpendicularX * offsets[arrowIndex % 5];
    LONG tipY = static_cast<LONG>(centerY + dy * edgeScale + 0.5f)
        + perpendicularY * offsets[arrowIndex % 5];
    if (tipX < leftInset) tipX = leftInset;
    if (tipX > rightInset) tipX = rightInset;
    if (tipY < topInset) tipY = topInset;
    if (tipY > bottomInset) tipY = bottomInset;

    for (LONG pixel = 0; pixel < 5; ++pixel)
    {
        PutLocatorPixel(tipX - directionX * pixel,
            tipY - directionY * pixel);
    }
    for (LONG wing = -2; wing <= 2; ++wing)
    {
        PutLocatorPixel(tipX - directionX * 2 + perpendicularX * wing,
            tipY - directionY * 2 + perpendicularY * wing);
    }
    return true;
}

LONG DrawEnemyLocators(EnemyRuntime* enemies, LONG enemyCount,
    LONG cameraX, LONG cameraY)
{
    if (roomSizeStage < 3 || currentEnemyRemaining <= 0
        || currentEnemyRemaining > 5)
    {
        return 0;
    }
    LONG arrows = 0;
    for (LONG enemyIndex = 0; enemyIndex < enemyCount && arrows < 5; ++enemyIndex)
    {
        if (enemies[enemyIndex].role != pressureEnemyRole
            && DrawEnemyLocatorArrow(enemies[enemyIndex], cameraX, cameraY, arrows))
        {
            ++arrows;
        }
    }
    return arrows;
}

bool SetEnemyAlertState(EnemyRuntime& enemy)
{
    if (!enemy.alive || enemy.alert)
    {
        return false;
    }
    enemy.alert = true;
    enemy.detectionProgress = 1.0f;
    enemy.lostSightElapsed = 0.0f;
    enemy.alertLostElapsed = 0.0f;
    enemy.heardSuspicion = false;
    enemy.searchTargetValid = false;
    enemy.searchPathCount = 0;
    enemy.searchPathIndex = 0;
    enemy.scanning = false;
    enemy.scanElapsed = 0.0f;
    enemy.patrolTurnRemaining = 0.0f;
    return true;
}

void BeginAlertEvent()
{
    if (!alertEventActive)
    {
        if (alertEventCount != 0xff)
        {
            ++alertEventCount;
        }
        alertEventActive = true;
    }
}

LONG EnterEnemyAlert(EnemyRuntime* enemies, LONG enemyCount, LONG firstEnemy)
{
    if (firstEnemy < 0 || firstEnemy >= enemyCount
        || !SetEnemyAlertState(enemies[firstEnemy]))
    {
        return 0;
    }

    BeginAlertEvent();
    LONG entered = 1;
    BYTE listenerQueue[maxEnemyCount]{};
    LONG queueRead = 0;
    LONG queueCount = 0;
    if (enemies[firstEnemy].role == listenerEnemyRole)
    {
        listenerQueue[queueCount++] = static_cast<BYTE>(firstEnemy);
    }

    while (queueRead < queueCount)
    {
        EnemyRuntime& listener = enemies[listenerQueue[queueRead++]];
        for (LONG enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
        {
            EnemyRuntime& enemy = enemies[enemyIndex];
            BYTE role = enemy.role;
            float differenceX = enemy.x - listener.x;
            float differenceY = enemy.y - listener.y;
            if ((role == patrollerEnemyRole || role == hunterEnemyRole
                    || role == listenerEnemyRole)
                && differenceX * differenceX + differenceY * differenceY
                    <= listenerAlertRangeSquared
                && SetEnemyAlertState(enemy))
            {
                ++entered;
                if (role == listenerEnemyRole && queueCount < maxEnemyCount)
                {
                    listenerQueue[queueCount++] = static_cast<BYTE>(enemyIndex);
                }
            }
        }
    }
    return entered;
}

LONG AlertEnemiesNear(EnemyRuntime* enemies, LONG enemyCount, float centerX,
    float centerY)
{
    LONG entered = 0;
    for (LONG enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
    {
        BYTE role = enemies[enemyIndex].role;
        float differenceX = enemies[enemyIndex].x - centerX;
        float differenceY = enemies[enemyIndex].y - centerY;
        if (role < enemyRoleCount
            && differenceX * differenceX + differenceY * differenceY
            <= slashLocalAlertRangeSquared)
        {
            entered += EnterEnemyAlert(enemies, enemyCount, enemyIndex);
        }
    }
    return entered;
}

void UpdateAlertEventState(EnemyRuntime* enemies, LONG enemyCount)
{
    if (!alertEventActive)
    {
        return;
    }
    for (LONG enemyIndex = 0; enemyIndex < enemyCount; ++enemyIndex)
    {
        EnemyRuntime& enemy = enemies[enemyIndex];
        if (enemy.alive && (enemy.alert || enemy.detectionProgress > 0.0f
            || enemy.heardSuspicion))
        {
            return;
        }
    }
    alertEventActive = false;
}

bool PointInsideVisionSector(float originX, float originY, float facingX,
    float facingY, float targetX, float targetY)
{
    float differenceX = targetX - originX;
    float differenceY = targetY - originY;
    float distanceSquared = differenceX * differenceX + differenceY * differenceY;
    float forward = differenceX * facingX + differenceY * facingY;
    float lateral = differenceX * -facingY + differenceY * facingX;
    return forward > 0.0f && distanceSquared <= enemyVisionRangeSquared
        && lateral * lateral <= forward * forward
            * enemyVisionSlope * enemyVisionSlope;
}

bool PointInsideExecuteFacing(float playerX, float playerY, float facingX,
    float facingY, float targetX, float targetY)
{
    float differenceX = targetX - playerX;
    float differenceY = targetY - playerY;
    float distanceSquared = differenceX * differenceX + differenceY * differenceY;
    if (distanceSquared <= 0.0001f)
    {
        return true;
    }
    float forward = differenceX * facingX + differenceY * facingY;
    return forward > 0.0f && forward * forward
        >= distanceSquared * executeFacingCosineSquared;
}

bool EnemyInAttackRange(float enemyX, float enemyY, float playerX, float playerY)
{
    return enemyX - enemyHalfWidth <= playerX + playerHalfWidth
            + enemyAttackContactTolerance
        && enemyX + enemyHalfWidth + enemyAttackContactTolerance
            >= playerX - playerHalfWidth
        && enemyY - enemyHalfHeight <= playerY + playerHalfHeight
            + enemyAttackContactTolerance
        && enemyY + enemyHalfHeight + enemyAttackContactTolerance
            >= playerY - playerHalfHeight;
}

void QueuePlayerDamage(LONG damage, LONG& pendingDamage)
{
    if (damage > pendingDamage)
    {
        pendingDamage = damage;
    }
}

void UpdateEnemyAttack(EnemyRuntime& enemy, bool attackEnabled, float playerX,
    float playerY, float deltaTime, LONG& pendingDamage)
{
    if (!enemy.alive || !attackEnabled)
    {
        enemy.attackWindupRemaining = 0.0f;
        return;
    }
    if (enemy.attackWindupRemaining > 0.0f)
    {
        enemy.attackWindupRemaining -= deltaTime;
        if (enemy.attackWindupRemaining <= 0.0f)
        {
            enemy.attackWindupRemaining = 0.0f;
            if (EnemyInAttackRange(enemy.x, enemy.y, playerX, playerY))
            {
                QueuePlayerDamage(playerDamageAmount, pendingDamage);
            }
            enemy.attackCooldownRemaining = enemyAttackCooldownDuration;
        }
    }
    else if (enemy.attackCooldownRemaining <= 0.0f
        && EnemyInAttackRange(enemy.x, enemy.y, playerX, playerY))
    {
        enemy.attackWindupRemaining = enemyAttackWindupDuration;
    }
}

struct SaveCheckpoint
{
    DWORD magic;
    DWORD version;
    DWORD seed;
    LONG room;
    LONG playerHP;
    LONG runKills;
    BYTE moveStack;
    BYTE slashStack;
    BYTE dashStack;
    BYTE functionalFlags;
    BYTE reroll;
    BYTE character;
    BYTE dashCharges;
    BYTE alertEvents;
};

static_assert(sizeof(SaveCheckpoint) == 32);

struct LegacyMetaProfile
{
    DWORD magic;
    DWORD version;
    LONG coin;
    BYTE unlockedMask;
    BYTE dashGrowth;
    BYTE audioSetting;
    BYTE reserved;
};

static_assert(sizeof(LegacyMetaProfile) == 16);

struct MetaProfile
{
    DWORD magic;
    DWORD version;
    LONG coin;
    BYTE unlockedMask;
    BYTE audioSetting;
    BYTE commonLevel[commonUpgradeCount];
    BYTE characterLevel[characterCount][characterGlobalUpgradeCount];
};

static_assert(sizeof(MetaProfile) == 32);

void ResetMetaProfile()
{
    globalCoin = 0;
    unlockedCharacterMask = 1 << basicCharacter;
    audioEnabled = true;
    for (BYTE upgrade = 0; upgrade < commonUpgradeCount; ++upgrade)
    {
        commonGlobalLevel[upgrade] = 0;
    }
    for (BYTE character = 0; character < characterCount; ++character)
    {
        for (BYTE upgrade = 0; upgrade < characterGlobalUpgradeCount; ++upgrade)
        {
            characterGlobalLevel[character][upgrade] = 0;
        }
    }
}

void WriteMetaProfile();

bool ReadMetaProfile()
{
    HANDLE file = CreateFileW(metaFileName, GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE)
    {
        ResetMetaProfile();
        return false;
    }
    DWORD size = GetFileSize(file, nullptr);
    DWORD bytesRead = 0;
    if (size == sizeof(LegacyMetaProfile))
    {
        LegacyMetaProfile legacy{};
        bool valid = ReadFile(file, &legacy, sizeof(legacy), &bytesRead, nullptr)
            && bytesRead == sizeof(legacy) && legacy.magic == metaMagic
            && legacy.version == 1 && legacy.coin >= 0
            && (legacy.unlockedMask & (1 << basicCharacter)) != 0
            && (legacy.unlockedMask & ~((1 << characterCount) - 1)) == 0
            && legacy.dashGrowth <= 2 && legacy.audioSetting <= 2
            && legacy.reserved == 0;
        CloseHandle(file);
        if (!valid)
        {
            ResetMetaProfile();
            return false;
        }
        ResetMetaProfile();
        globalCoin = legacy.coin;
        unlockedCharacterMask = legacy.unlockedMask;
        audioEnabled = legacy.audioSetting != 2;
        characterGlobalLevel[basicCharacter][0] = legacy.dashGrowth;
        WriteMetaProfile();
        return true;
    }

    MetaProfile profile{};
    bool valid = size == sizeof(profile)
        && ReadFile(file, &profile, sizeof(profile), &bytesRead, nullptr)
        && bytesRead == sizeof(profile);
    CloseHandle(file);
    valid = valid && profile.magic == metaMagic && profile.version == metaVersion
        && profile.coin >= 0
        && (profile.unlockedMask & (1 << basicCharacter)) != 0
        && (profile.unlockedMask & ~((1 << characterCount) - 1)) == 0
        && profile.audioSetting <= 2;
    for (BYTE upgrade = 0; valid && upgrade < commonUpgradeCount; ++upgrade)
    {
        valid = profile.commonLevel[upgrade] <= commonUpgradeMaximum[upgrade];
    }
    bool migratePiercerProgression = false;
    for (BYTE character = 0; valid && character < characterCount; ++character)
    {
        for (BYTE upgrade = 0; valid && upgrade < characterGlobalUpgradeCount;
            ++upgrade)
        {
            BYTE storedLevel = profile.characterLevel[character][upgrade];
            if (character == piercerCharacter && upgrade == 2)
            {
                if (storedLevel <= 2)
                {
                    migratePiercerProgression = true;
                }
                else
                {
                    valid = storedLevel == pierceThroughStorageMarker
                        || storedLevel == pierceThroughStorageMarker + 1;
                }
            }
            else
            {
                valid = storedLevel <= characterGlobalMaximum[character][upgrade];
            }
        }
    }
    if (!valid)
    {
        ResetMetaProfile();
        return false;
    }
    globalCoin = profile.coin;
    unlockedCharacterMask = profile.unlockedMask;
    audioEnabled = profile.audioSetting != 2;
    for (BYTE upgrade = 0; upgrade < commonUpgradeCount; ++upgrade)
    {
        commonGlobalLevel[upgrade] = profile.commonLevel[upgrade];
    }
    for (BYTE character = 0; character < characterCount; ++character)
    {
        for (BYTE upgrade = 0; upgrade < characterGlobalUpgradeCount; ++upgrade)
        {
            BYTE storedLevel = profile.characterLevel[character][upgrade];
            characterGlobalLevel[character][upgrade]
                = character == piercerCharacter && upgrade == 2
                    ? (storedLevel & pierceThroughStorageMarker
                        ? static_cast<BYTE>(storedLevel & 1) : 0)
                    : storedLevel;
        }
    }
    if (migratePiercerProgression)
    {
        WriteMetaProfile();
    }
    return true;
}

void WriteMetaProfile()
{
    MetaProfile profile{};
    profile.magic = metaMagic;
    profile.version = metaVersion;
    profile.coin = globalCoin;
    profile.unlockedMask = unlockedCharacterMask;
    profile.audioSetting = static_cast<BYTE>(audioEnabled ? 1 : 2);
    for (BYTE upgrade = 0; upgrade < commonUpgradeCount; ++upgrade)
    {
        profile.commonLevel[upgrade] = commonGlobalLevel[upgrade];
    }
    for (BYTE character = 0; character < characterCount; ++character)
    {
        for (BYTE upgrade = 0; upgrade < characterGlobalUpgradeCount; ++upgrade)
        {
            profile.characterLevel[character][upgrade]
                = character == piercerCharacter && upgrade == 2
                    ? static_cast<BYTE>(pierceThroughStorageMarker
                        | characterGlobalLevel[character][upgrade])
                    : characterGlobalLevel[character][upgrade];
        }
    }
    HANDLE file = CreateFileW(metaFileName, GENERIC_WRITE, 0, nullptr,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file != INVALID_HANDLE_VALUE)
    {
        DWORD bytesWritten = 0;
        WriteFile(file, &profile, sizeof(profile), &bytesWritten, nullptr);
        CloseHandle(file);
    }
}

BYTE CharacterDashCapacity(BYTE character)
{
    if (character == heavyCharacter)
    {
        return 1;
    }
    BYTE capacity = static_cast<BYTE>(1
        + characterGlobalLevel[character][0]);
    BYTE cap = characterDashCaps[character];
    return capacity < cap ? capacity : cap;
}

BYTE CurrentDashCapacity()
{
    return CharacterDashCapacity(selectedCharacter);
}

LONG CharacterMaximumHP(BYTE character)
{
    LONG maximumHP = characterProfiles[character].maxHP;
    if (character == basicCharacter)
    {
        maximumHP += characterGlobalLevel[character][1] * combatScale;
    }
    else if (character == heavyCharacter)
    {
        maximumHP += characterGlobalLevel[character][0] * 2 * combatScale;
    }
    return maximumHP;
}

float CharacterMoveSpeed(BYTE character)
{
    BYTE level = 0;
    if (character == mobilityCharacter)
    {
        level = characterGlobalLevel[character][1];
    }
    else if (character == heavyCharacter || character == rapidCharacter)
    {
        level = characterGlobalLevel[character][2];
    }
    return playerMoveSpeed * characterProfiles[character].moveScale
        * (1.0f + level * 0.05f);
}

LONG CharacterSlashHitCap(BYTE character)
{
    LONG hitCap = characterProfiles[character].slashHitCap;
    if (character == piercerCharacter
        && characterGlobalLevel[character][2] != 0)
    {
        ++hitCap;
    }
    return hitCap;
}

float CharacterSlashCooldown(BYTE character)
{
    float cooldown = slashCooldownDuration
        * characterProfiles[character].slashCooldownScale;
    if (character == rapidCharacter)
    {
        cooldown *= 1.0f - characterGlobalLevel[character][1] * 0.1f;
    }
    return cooldown;
}

LONG CharacterSlashReach(BYTE character)
{
    LONG reach = characterProfiles[character].slashReach;
    if (character == basicCharacter && characterGlobalLevel[character][2])
    {
        ++reach;
    }
    else if (character == piercerCharacter)
    {
        reach += characterGlobalLevel[character][1] * 2;
    }
    return reach;
}

LONG CharacterSlashWidth(BYTE character)
{
    LONG width = characterProfiles[character].slashWidth;
    if (character == basicCharacter && characterGlobalLevel[character][2] >= 2)
    {
        ++width;
    }
    else if (character == heavyCharacter)
    {
        width += characterGlobalLevel[character][1] * 2;
    }
    return width;
}

float CharacterDashDistance(BYTE character)
{
    float distance = dashDistance * characterProfiles[character].dashDistanceScale;
    if (character == mobilityCharacter)
    {
        distance *= 1.0f + characterGlobalLevel[character][2] * 0.1f;
    }
    return distance;
}

BYTE CurrentRunRerollCapacity()
{
    return static_cast<BYTE>(1 + commonGlobalLevel[runRerollGlobalUpgrade]);
}

void ApplyFieldRecovery(LONG& playerHP, LONG maximumHP)
{
    if (commonGlobalLevel[fieldRecoveryGlobalUpgrade] && playerHP < maximumHP)
    {
        playerHP += healAmount;
        if (playerHP > maximumHP)
        {
            playerHP = maximumHP;
        }
    }
}

bool BuyCommonGlobalUpgrade(BYTE upgrade)
{
    BYTE level = commonGlobalLevel[upgrade];
    if (level >= commonUpgradeMaximum[upgrade])
    {
        titleStatus = 5;
        return false;
    }
    LONG cost = commonUpgradeCost[upgrade][level];
    if (globalCoin < cost)
    {
        titleStatus = 3;
        return false;
    }
    globalCoin -= cost;
    ++commonGlobalLevel[upgrade];
    titleStatus = 4;
    WriteMetaProfile();
    return true;
}

bool BuyCharacterGlobalUpgrade(BYTE character, BYTE upgrade)
{
    BYTE level = characterGlobalLevel[character][upgrade];
    if (level >= characterGlobalMaximum[character][upgrade])
    {
        titleStatus = 5;
        return false;
    }
    LONG cost = characterGlobalCost[level];
    if (globalCoin < cost)
    {
        titleStatus = 3;
        return false;
    }
    globalCoin -= cost;
    ++characterGlobalLevel[character][upgrade];
    titleStatus = 4;
    WriteMetaProfile();
    return true;
}

bool UnlockCharacter(BYTE character)
{
    BYTE characterBit = static_cast<BYTE>(1 << character);
    if (unlockedCharacterMask & characterBit)
    {
        selectedCharacter = character;
        titleStatus = 4;
        return true;
    }
    if (globalCoin < characterUnlockCosts[character])
    {
        titleStatus = 3;
        return false;
    }
    globalCoin -= characterUnlockCosts[character];
    unlockedCharacterMask |= characterBit;
    selectedCharacter = character;
    titleStatus = 4;
    WriteMetaProfile();
    return true;
}

const wchar_t* CharacterName(BYTE character)
{
    return character == basicCharacter ? L"BASIC"
        : (character == mobilityCharacter ? L"MOBILITY"
            : (character == piercerCharacter ? L"PIERCER"
                : (character == heavyCharacter ? L"HEAVY" : L"RAPID")));
}

const wchar_t* CharacterGlobalUpgradeName(BYTE character, BYTE upgrade)
{
    constexpr const wchar_t* names[characterCount][characterGlobalUpgradeCount]
    {
        { L"DASH CAP", L"VITALITY", L"BALANCED EDGE" },
        { L"DASH CAP", L"LIGHT FRAME", L"LONG STEP" },
        { L"DASH CAP", L"LONG POINT", L"PIERCE THROUGH" },
        { L"FORTIFY", L"WIDE SWING", L"STEADY BODY" },
        { L"DASH CAP", L"TEMPO", L"LIGHT BODY" }
    };
    return names[character][upgrade];
}

void ResetDashRecharge(float& rechargeRemaining, float rechargeDuration)
{
    if (currentDashCharges < currentDashMaxCharges)
    {
        ++currentDashCharges;
    }
    rechargeRemaining = currentDashCharges < currentDashMaxCharges
        ? rechargeDuration : 0.0f;
}

void UpdateDashRecharge(float deltaTime, float rechargeDuration,
    float& rechargeRemaining)
{
    if (currentDashCharges >= currentDashMaxCharges)
    {
        rechargeRemaining = 0.0f;
        return;
    }
    if (rechargeRemaining > 0.0f)
    {
        rechargeRemaining -= deltaTime;
    }
    if (rechargeRemaining <= 0.0f)
    {
        ++currentDashCharges;
        rechargeRemaining = currentDashCharges < currentDashMaxCharges
            ? rechargeDuration : 0.0f;
    }
}

bool ConsumeDashCharge(float rechargeDuration, float& rechargeRemaining)
{
    if (!currentDashCharges)
    {
        return false;
    }
    if (currentDashCharges == currentDashMaxCharges)
    {
        rechargeRemaining = rechargeDuration;
    }
    --currentDashCharges;
    return true;
}

void GrantRunCoin(bool cleared)
{
    if (runRewardGranted)
    {
        return;
    }
    LONG reward = runKillCount + (cleared ? runClearCoinBonus : 0);
    reward += reward * commonGlobalLevel[coinSenseGlobalUpgrade] / 10;
    runEarnedCoin = reward > 0x7fffffff - globalCoin
        ? 0x7fffffff - globalCoin : reward;
    globalCoin += runEarnedCoin;
    runRewardGranted = true;
    WriteMetaProfile();
}

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
    valid = valid && checkpoint->magic == saveMagic
        && checkpoint->version == saveVersion
        && checkpoint->seed != 0 && checkpoint->room >= 0 && checkpoint->room < roomCount
        && checkpoint->playerHP > 0
        && checkpoint->runKills >= 0
        && checkpoint->moveStack <= 2 && checkpoint->slashStack <= 2
        && checkpoint->dashStack <= 2
        && (checkpoint->functionalFlags & ~7) == 0 && checkpoint->reroll <= 2
        && checkpoint->character < characterCount
        && checkpoint->dashCharges <= 4
        && checkpoint->playerHP
            <= CharacterMaximumHP(checkpoint->character) / combatScale
        && checkpoint->moveStack + checkpoint->slashStack + checkpoint->dashStack
            + ((checkpoint->functionalFlags & silentDashUpgradeFlag) != 0)
            + ((checkpoint->functionalFlags & executeReachUpgradeFlag) != 0)
            + ((checkpoint->functionalFlags & fieldMedicUpgradeFlag) != 0)
            <= checkpoint->room / 2;
    if (valid)
    {
        checkpoint->playerHP *= combatScale;
    }
    return valid;
}

void WriteCheckpoint(DWORD seed, LONG room, LONG playerHP, LONG runKills)
{
    SaveCheckpoint checkpoint
    {
        saveMagic,
        saveVersion,
        seed,
        room,
        playerHP / combatScale,
        runKills,
        moveUpgradeStack,
        slashUpgradeStack,
        dashUpgradeStack,
        functionalUpgradeFlags,
        static_cast<BYTE>(rerollUsed),
        selectedCharacter,
        currentDashCharges,
        alertEventCount
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

BYTE CurrentUpgradeStack(LONG upgrade)
{
    if (upgrade == moveUpgrade)
    {
        return moveUpgradeStack;
    }
    if (upgrade == slashUpgrade)
    {
        return slashUpgradeStack;
    }
    if (upgrade == dashUpgrade)
    {
        return dashUpgradeStack;
    }
    return (functionalUpgradeFlags & (1 << (upgrade - silentDashUpgrade))) != 0;
}

void GenerateUpgradeOffer(bool reroll)
{
    LONG candidates[upgradeCount]{};
    LONG candidateCount = 0;
    for (LONG upgrade = 0; upgrade < upgradeCount; ++upgrade)
    {
        if (CurrentUpgradeStack(upgrade) < (upgrade <= dashUpgrade ? 2 : 1))
        {
            candidates[candidateCount++] = upgrade;
        }
    }

    DWORD state = RoomRandom(runSeed, currentRoom)
        ^ (reroll ? 0xA511E9B3u : 0x2468ACE1u);
    for (LONG index = candidateCount - 1; index > 0; --index)
    {
        LONG other = NextRoomRandom(state) % (index + 1);
        LONG upgrade = candidates[index];
        candidates[index] = candidates[other];
        candidates[other] = upgrade;
    }

    LONG previousA = upgradeOptionA;
    LONG previousB = upgradeOptionB;
    upgradeOptionA = candidates[0];
    upgradeOptionB = candidates[1];
    if (reroll && candidateCount > 2
        && ((upgradeOptionA == previousA && upgradeOptionB == previousB)
            || (upgradeOptionA == previousB && upgradeOptionB == previousA)))
    {
        upgradeOptionB = candidates[2];
    }
}

void RecalculateAugmentStats(float& moveSpeed, float& slashCooldown,
    float& dashCooldown)
{
    moveSpeed = CharacterMoveSpeed(selectedCharacter)
        * (1.0f + moveUpgradeStack * 0.1f);
    slashCooldown = CharacterSlashCooldown(selectedCharacter)
        * (1.0f - slashUpgradeStack * 0.2f);
    dashCooldown = dashCooldownDuration * (1.0f - dashUpgradeStack * 0.2f);
}

bool PlayerMovementIsAudible(bool moved, bool dashing)
{
    return moved && (!dashing || !(functionalUpgradeFlags & silentDashUpgradeFlag));
}

LONG CurrentExecuteReach()
{
    return functionalUpgradeFlags & executeReachUpgradeFlag
        ? executeReach * 2 : executeReach;
}

void ApplyFieldMedic(LONG& playerHP)
{
    if ((functionalUpgradeFlags & fieldMedicUpgradeFlag)
        && playerHP < CharacterMaximumHP(selectedCharacter))
    {
        playerHP += healAmount;
        LONG maximumHP = CharacterMaximumHP(selectedCharacter);
        if (playerHP > maximumHP)
        {
            playerHP = maximumHP;
        }
    }
}

bool RoomClearsToUpgrade(LONG room)
{
    return (room & 1) != 0 && room < roomCount - 1;
}

bool PiercerSlashHitsEnemy(float originX, float originY, LONG directionX,
    LONG directionY, float enemyX, float enemyY)
{
    float scale = directionX && directionY ? 0.70710678f : 1.0f;
    float forwardX = directionX * scale;
    float forwardY = directionY * scale;
    float differenceX = enemyX - originX;
    float differenceY = enemyY - originY;
    float forward = differenceX * forwardX + differenceY * forwardY;
    float lateral = differenceX * -forwardY + differenceY * forwardX;
    float absoluteForwardX = forwardX < 0.0f ? -forwardX : forwardX;
    float absoluteForwardY = forwardY < 0.0f ? -forwardY : forwardY;
    float start = absoluteForwardX * playerHalfWidth
        + absoluteForwardY * playerHalfHeight;
    float enemyForwardRadius = absoluteForwardX * enemyHalfWidth
        + absoluteForwardY * enemyHalfHeight;
    float enemyLateralRadius = absoluteForwardY * enemyHalfWidth
        + absoluteForwardX * enemyHalfHeight;
    return forward + enemyForwardRadius > start
        && forward - enemyForwardRadius < start + CharacterSlashReach(piercerCharacter)
        && lateral >= -CharacterSlashWidth(piercerCharacter) * 0.5f
            - enemyLateralRadius
        && lateral < CharacterSlashWidth(piercerCharacter) * 0.5f
            + enemyLateralRadius;
}

bool PiercerSlashOutlinePixel(float originX, float originY, LONG directionX,
    LONG directionY, float pointX, float pointY)
{
    float scale = directionX && directionY ? 0.70710678f : 1.0f;
    float forwardX = directionX * scale;
    float forwardY = directionY * scale;
    float differenceX = pointX - originX;
    float differenceY = pointY - originY;
    float forward = differenceX * forwardX + differenceY * forwardY;
    float lateral = differenceX * -forwardY + differenceY * forwardX;
    float absoluteForwardX = forwardX < 0.0f ? -forwardX : forwardX;
    float absoluteForwardY = forwardY < 0.0f ? -forwardY : forwardY;
    float start = absoluteForwardX * playerHalfWidth
        + absoluteForwardY * playerHalfHeight;
    float end = start + CharacterSlashReach(piercerCharacter);
    float halfWidth = CharacterSlashWidth(piercerCharacter) * 0.5f;
    return forward >= start && forward <= end
        && lateral >= -halfWidth && lateral < halfWidth
        && (forward < start + 1.0f || forward > end - 1.0f
            || lateral < -halfWidth + 1.0f || lateral >= halfWidth - 1.0f);
}

bool RectangleOverlapsRoomWall(float left, float top, float right, float bottom);

void AddRoomWall(LONG left, LONG top, LONG right, LONG bottom)
{
    if (currentWallCount >= maxRoomWalls)
    {
        return;
    }
    LONG wall = currentWallCount++;
    currentWallLeft[wall] = left;
    currentWallTop[wall] = top;
    currentWallRight[wall] = right;
    currentWallBottom[wall] = bottom;
}

bool RoomWallPlacementValid(LONG left, LONG top, LONG right, LONG bottom)
{
    if (left < 24 || top < 24 || right > worldWidth - 24 || bottom > worldHeight - 24
        || (left < currentExitRight && right > currentExitLeft
            && top < currentExitBottom && bottom > currentExitTop)
        || (left < currentPlayerStartX + playerHalfWidth + 8.0f
            && right > currentPlayerStartX - playerHalfWidth - 8.0f
            && top < currentPlayerStartY + playerHalfHeight + 8.0f
            && bottom > currentPlayerStartY - playerHalfHeight - 8.0f))
    {
        return false;
    }
    return !RectangleOverlapsRoomWall(static_cast<float>(left - 8),
        static_cast<float>(top - 8), static_cast<float>(right + 8),
        static_cast<float>(bottom + 8));
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

bool RectangleOverlapsRoomTrap(float left, float top, float right, float bottom)
{
    for (LONG trap = 0; trap < currentTrapCount; ++trap)
    {
        if (left < currentTrapRight[trap] && right > currentTrapLeft[trap]
            && top < currentTrapBottom[trap] && bottom > currentTrapTop[trap])
        {
            return true;
        }
    }
    return false;
}

bool TrapIsActive(LONG trap, float cycleElapsed)
{
    float phase = cycleElapsed + currentTrapPhaseOffset[trap];
    while (phase >= trapCycleDuration)
    {
        phase -= trapCycleDuration;
    }
    return phase >= trapOffDuration;
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

LONG ListenerTargetForRoomSize(LONG stage, LONG enemyCount)
{
    LONG target = stage <= 2 ? 2 : (stage == 3 ? 3 : 4);
    return target < enemyCount ? target : enemyCount;
}

bool WallBlocksSegment(float startX, float startY, float endX, float endY);
bool RoomLayoutConnected();

bool GenerateMazeLayout(DWORD& state)
{
    for (LONG layoutAttempt = 0; layoutAttempt < 32; ++layoutAttempt)
    {
        currentWallCount = 0;
        bool complete = true;
        for (LONG quadrant = 0; quadrant < 4 && complete; ++quadrant)
        {
            LONG quadrantLeft = (quadrant & 1) ? worldWidth / 2 + 16 : 24;
            LONG quadrantRight = (quadrant & 1) ? worldWidth - 24 : worldWidth / 2 - 16;
            LONG quadrantTop = (quadrant & 2) ? worldHeight / 2 + 16 : 24;
            LONG quadrantBottom = (quadrant & 2) ? worldHeight - 24
                : worldHeight / 2 - 16;
            LONG zeroWeight = 12 - currentRoom;
            LONG oneWeight = 48 - currentRoom * 2;
            LONG threeWeight = 20 + currentRoom * 2;
            LONG roll = NextRoomRandom(state) & 127;
            LONG segmentCount = roll < zeroWeight ? 0
                : (roll < zeroWeight + oneWeight ? 1
                    : (roll < 128 - threeWeight ? 2 : 3));
            for (LONG segment = 0; segment < segmentCount; ++segment)
            {
                bool placed = false;
                for (LONG attempt = 0; attempt < 128 && !placed; ++attempt)
                {
                    bool horizontal = (NextRoomRandom(state) & 1) != 0;
                    LONG availableWidth = quadrantRight - quadrantLeft;
                    LONG availableHeight = quadrantBottom - quadrantTop;
                    LONG availableLength = horizontal ? availableWidth : availableHeight;
                    LONG length = availableLength
                        * (50 + static_cast<LONG>(NextRoomRandom(state) % 21)) / 100;
                    if (length < mazeWallThickness * 3)
                    {
                        length = mazeWallThickness * 3;
                    }
                    if (length > availableLength)
                    {
                        length = availableLength;
                    }
                    LONG width = horizontal ? length : mazeWallThickness;
                    LONG height = horizontal ? mazeWallThickness : length;
                    LONG left = quadrantLeft + NextRoomRandom(state)
                        % (availableWidth - width + 1);
                    LONG top = quadrantTop + NextRoomRandom(state)
                        % (availableHeight - height + 1);
                    if (left >= quadrantLeft && top >= quadrantTop
                        && left + width <= quadrantRight
                        && top + height <= quadrantBottom
                        && RoomWallPlacementValid(left, top, left + width, top + height))
                    {
                        AddRoomWall(left, top, left + width, top + height);
                        placed = true;
                    }
                }
                complete &= placed;
            }
        }
        if (complete && RoomLayoutConnected())
        {
            return true;
        }
    }
    currentWallCount = 0;
    return false;
}

void SetupCurrentRoom()
{
    DWORD state = RoomRandom(runSeed, currentRoom);
    currentRoomType = NextRoomRandom(state) % 5;
    currentEnemyCount = 2 + currentRoom * 2;
    LONG sizeStage = currentEnemyCount <= 4 ? 2
        : (currentEnemyCount <= 10 ? 3 : (currentEnemyCount <= 18 ? 4 : 5));
    SetRoomSizeStage(sizeStage);

    DWORD roleState = RoomRandom(runSeed, currentRoom) ^ 0xD1B54A35u;
    LONG listenerTarget = ListenerTargetForRoomSize(roomSizeStage,
        currentEnemyCount);
    constexpr BYTE remainingRoles[4]
        = { patrollerEnemyRole, watcherEnemyRole, hunterEnemyRole, spinnerEnemyRole };
    for (LONG enemy = 0; enemy < currentEnemyCount; ++enemy)
    {
        currentEnemyRole[enemy] = enemy < listenerTarget ? listenerEnemyRole
            : remainingRoles[NextRoomRandom(roleState) % 4];
    }

    currentLayoutVariant = NextRoomRandom(state) >> 31;
    currentExitSide = NextRoomRandom(state) & 3;
    currentWallCount = 0;
    if (currentExitSide == 0)
    {
        currentExitLeft = 0;
        currentExitTop = worldHeight / 2 - 12;
        currentExitRight = 16;
        currentExitBottom = worldHeight / 2 + 12;
        currentPlayerStartX = worldWidth - 28.0f;
        currentPlayerStartY = worldHeight / 2.0f;
    }
    else if (currentExitSide == 1)
    {
        currentExitLeft = worldWidth - 16;
        currentExitTop = worldHeight / 2 - 12;
        currentExitRight = worldWidth;
        currentExitBottom = worldHeight / 2 + 12;
        currentPlayerStartX = 28.0f;
        currentPlayerStartY = worldHeight / 2.0f;
    }
    else if (currentExitSide == 2)
    {
        currentExitLeft = worldWidth / 2 - 12;
        currentExitTop = 0;
        currentExitRight = worldWidth / 2 + 12;
        currentExitBottom = 16;
        currentPlayerStartX = worldWidth / 2.0f;
        currentPlayerStartY = worldHeight - 28.0f;
    }
    else
    {
        currentExitLeft = worldWidth / 2 - 12;
        currentExitTop = worldHeight - 16;
        currentExitRight = worldWidth / 2 + 12;
        currentExitBottom = worldHeight;
        currentPlayerStartX = worldWidth / 2.0f;
        currentPlayerStartY = 28.0f;
    }

    if (currentRoomType == pillarRoomType)
    {
        constexpr LONG pillarCountsByStage[5] = { 1, 4, 6, 12, 20 };
        LONG requestedWallCount = pillarCountsByStage[roomSizeStage - 1];
        for (LONG attempt = 0; attempt < 2048 && currentWallCount < requestedWallCount;
            ++attempt)
        {
            LONG left = 24 + NextRoomRandom(state) % (worldWidth - 64);
            LONG top = 24 + NextRoomRandom(state) % (worldHeight - 72);
            if (RoomWallPlacementValid(left, top, left + 16, top + 24))
            {
                AddRoomWall(left, top, left + 16, top + 24);
            }
        }
    }
    else if (currentRoomType == mazeRoomType)
    {
        GenerateMazeLayout(state);
    }
    else if (currentRoomType == mixedRoomType)
    {
        constexpr LONG mixedWallCountsByStage[5] = { 1, 2, 4, 7, 12 };
        LONG requestedWallCount = mixedWallCountsByStage[roomSizeStage - 1];
        for (LONG attempt = 0; attempt < 1024 && currentWallCount < requestedWallCount;
            ++attempt)
        {
            LONG left = 24 + NextRoomRandom(state) % (worldWidth - 64);
            LONG top = 24 + NextRoomRandom(state) % (worldHeight - 72);
            if (RoomWallPlacementValid(left, top, left + 16, top + 24))
            {
                AddRoomWall(left, top, left + 16, top + 24);
            }
        }
    }

    currentTrapCount = 0;
    if (currentRoomType == trapRoomType || currentRoomType == mixedRoomType)
    {
        constexpr LONG trapCountsByStage[5] = { 1, 3, 7, 12, 19 };
        constexpr LONG mixedTrapCountsByStage[5] = { 1, 2, 4, 7, 11 };
        LONG requestedTrapCount = currentRoomType == trapRoomType
            ? trapCountsByStage[roomSizeStage - 1]
            : mixedTrapCountsByStage[roomSizeStage - 1];
        for (LONG attempt = 0; attempt < 2048 && currentTrapCount < requestedTrapCount;
            ++attempt)
        {
            LONG left = 2 + NextRoomRandom(state) % (worldWidth - trapWidth - 3);
            LONG top = 2 + NextRoomRandom(state) % (worldHeight - trapHeight - 3);
            LONG right = left + trapWidth;
            LONG bottom = top + trapHeight;
            if (left < 2 || top < 2 || right > worldWidth - 2
                || bottom > worldHeight - 2
                || RectangleOverlapsRoomTrap(static_cast<float>(left - 2),
                    static_cast<float>(top - 2), static_cast<float>(right + 2),
                    static_cast<float>(bottom + 2))
                || RectangleOverlapsRoomWall(static_cast<float>(left),
                    static_cast<float>(top), static_cast<float>(right),
                    static_cast<float>(bottom))
                || (left < currentExitRight && right > currentExitLeft
                    && top < currentExitBottom && bottom > currentExitTop)
                || (left < currentPlayerStartX + playerHalfWidth
                    && right > currentPlayerStartX - playerHalfWidth
                    && top < currentPlayerStartY + playerHalfHeight
                    && bottom > currentPlayerStartY - playerHalfHeight))
            {
                continue;
            }
            LONG trap = currentTrapCount++;
            currentTrapLeft[trap] = left;
            currentTrapTop[trap] = top;
            currentTrapRight[trap] = right;
            currentTrapBottom[trap] = bottom;
            currentTrapPhaseOffset[trap]
                = static_cast<float>(NextRoomRandom(state) % 501) / 1000.0f;
        }
    }

    for (LONG enemy = 0; enemy < currentEnemyCount; ++enemy)
    {
        LONG firstCell = NextRoomRandom(state) % navigationNodeCount;
        for (LONG attempt = 0; attempt < navigationNodeCount * 3; ++attempt)
        {
            LONG candidate = attempt < navigationNodeCount * 2
                ? NextRoomRandom(state) % navigationNodeCount
                : (firstCell + attempt) % navigationNodeCount;
            LONG candidateColumn = candidate % navigationColumns;
            LONG candidateRow = candidate / navigationColumns;
            float cellX = enemyHalfWidth + candidateColumn * navigationCellSize;
            float cellY = enemyHalfHeight + candidateRow * navigationCellSize;
            bool enemyOverlap = false;
            for (LONG other = 0; other < enemy && !enemyOverlap; ++other)
            {
                enemyOverlap = cellX - enemyHalfWidth
                    < currentEnemyStartX[other] + enemyHalfWidth
                    && cellX + enemyHalfWidth > currentEnemyStartX[other] - enemyHalfWidth
                    && cellY - enemyHalfHeight < currentEnemyStartY[other] + enemyHalfHeight
                    && cellY + enemyHalfHeight > currentEnemyStartY[other] - enemyHalfHeight;
            }
            if (enemyOverlap || RectangleOverlapsRoomWall(cellX - enemyHalfWidth,
                    cellY - enemyHalfHeight, cellX + enemyHalfWidth,
                    cellY + enemyHalfHeight)
                || RectangleOverlapsRoomTrap(cellX - enemyHalfWidth,
                    cellY - enemyHalfHeight, cellX + enemyHalfWidth,
                    cellY + enemyHalfHeight)
                || (cellX - enemyHalfWidth < currentExitRight
                    && cellX + enemyHalfWidth > currentExitLeft
                    && cellY - enemyHalfHeight < currentExitBottom
                    && cellY + enemyHalfHeight > currentExitTop)
                || (cellX - enemyHalfWidth < currentPlayerStartX + playerHalfWidth
                    && cellX + enemyHalfWidth > currentPlayerStartX - playerHalfWidth
                    && cellY - enemyHalfHeight < currentPlayerStartY + playerHalfHeight
                    && cellY + enemyHalfHeight > currentPlayerStartY - playerHalfHeight))
            {
                continue;
            }
            currentEnemyStartX[enemy] = cellX;
            currentEnemyStartY[enemy] = cellY;
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

    constexpr float watcherFacingAngle[4]
        = { 0.0f, 1.57079633f, 3.14159265f, -1.57079633f };
    constexpr LONG watcherFacingX[4] = { 1, 0, -1, 0 };
    constexpr LONG watcherFacingY[4] = { 0, 1, 0, -1 };
    for (LONG enemy = 0; enemy < currentEnemyCount; ++enemy)
    {
        currentEnemyStartFacing[enemy] = 0.0f;
        if (currentEnemyRole[enemy] == watcherEnemyRole)
        {
            LONG firstFacing = NextRoomRandom(state) & 3;
            currentEnemyStartFacing[enemy] = watcherFacingAngle[firstFacing];
            for (LONG attempt = 0; attempt < 4; ++attempt)
            {
                LONG facing = (firstFacing + attempt) & 3;
                float targetX = currentEnemyStartX[enemy]
                    + watcherFacingX[facing] * enemyWidth * 3;
                float targetY = currentEnemyStartY[enemy]
                    + watcherFacingY[facing] * enemyWidth * 3;
                if (targetX >= 0.0f && targetX <= worldWidth
                    && targetY >= 0.0f && targetY <= worldHeight
                    && !WallBlocksSegment(currentEnemyStartX[enemy],
                        currentEnemyStartY[enemy], targetX, targetY))
                {
                    currentEnemyStartFacing[enemy] = watcherFacingAngle[facing];
                    break;
                }
            }
        }
        else if (currentEnemyRole[enemy] == spinnerEnemyRole)
        {
            currentEnemyStartFacing[enemy]
                = watcherFacingAngle[NextRoomRandom(state) & 3];
        }
    }

    currentPressureActive = false;
    currentPressureStartX = 0.0f;
    currentPressureStartY = 0.0f;
    if (currentRoomType == openRoomType)
    {
        LONG firstPressureCell = NextRoomRandom(state) % navigationNodeCount;
        for (LONG attempt = 0; attempt < navigationNodeCount * 3
            && !currentPressureActive; ++attempt)
        {
            LONG candidate = attempt < navigationNodeCount * 2
                ? NextRoomRandom(state) % navigationNodeCount
                : (firstPressureCell + attempt) % navigationNodeCount;
            LONG candidateColumn = candidate % navigationColumns;
            LONG candidateRow = candidate / navigationColumns;
            float candidateX = enemyHalfWidth + candidateColumn * navigationCellSize;
            float candidateY = enemyHalfHeight + candidateRow * navigationCellSize;
            float playerDifferenceX = candidateX - currentPlayerStartX;
            float playerDifferenceY = candidateY - currentPlayerStartY;
            bool enemyOverlap = false;
            for (LONG enemy = 0; enemy < currentEnemyCount && !enemyOverlap; ++enemy)
            {
                enemyOverlap = candidateX - pressureVisualWidth / 2.0f
                    < currentEnemyStartX[enemy] + enemyHalfWidth
                    && candidateX + pressureVisualWidth / 2.0f
                    > currentEnemyStartX[enemy] - enemyHalfWidth
                    && candidateY - pressureVisualHeight / 2.0f
                    < currentEnemyStartY[enemy] + enemyHalfHeight
                    && candidateY + pressureVisualHeight / 2.0f
                    > currentEnemyStartY[enemy] - enemyHalfHeight;
            }
            float cellX = enemyHalfWidth + candidateColumn * navigationCellSize;
            float cellY = enemyHalfHeight + candidateRow * navigationCellSize;
            if (enemyOverlap
                || candidateX < pressureVisualWidth / 2.0f
                || candidateX > worldWidth - pressureVisualWidth / 2.0f
                || candidateY < pressureVisualHeight / 2.0f
                || candidateY > worldHeight - pressureVisualHeight / 2.0f
                || playerDifferenceX * playerDifferenceX
                    + playerDifferenceY * playerDifferenceY
                    < pressureMinimumSpawnDistanceSquared
                || RectangleOverlapsRoomWall(
                    candidateX - pressureVisualWidth / 2.0f,
                    candidateY - pressureVisualHeight / 2.0f,
                    candidateX + pressureVisualWidth / 2.0f,
                    candidateY + pressureVisualHeight / 2.0f)
                || candidateColumn < 0 || candidateColumn >= navigationColumns
                || candidateRow < 0 || candidateRow >= navigationRows
                || RectangleOverlapsRoomWall(cellX - enemyHalfWidth,
                    cellY - enemyHalfHeight, cellX + enemyHalfWidth,
                    cellY + enemyHalfHeight)
                || RectangleOverlapsRoomTrap(
                    candidateX - pressureVisualWidth / 2.0f,
                    candidateY - pressureVisualHeight / 2.0f,
                    candidateX + pressureVisualWidth / 2.0f,
                    candidateY + pressureVisualHeight / 2.0f)
                || (candidateX - pressureVisualWidth / 2.0f < currentExitRight
                    && candidateX + pressureVisualWidth / 2.0f > currentExitLeft
                    && candidateY - pressureVisualHeight / 2.0f < currentExitBottom
                    && candidateY + pressureVisualHeight / 2.0f > currentExitTop))
            {
                continue;
            }
            currentPressureStartX = candidateX;
            currentPressureStartY = candidateY;
            currentPressureActive = true;
        }
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

bool RoomLayoutConnected()
{
    for (LONG node = 0; node < navigationNodeCount; ++node)
    {
        navigationState[node] = 0;
    }
    LONG startColumn = static_cast<LONG>((currentPlayerStartX - enemyHalfWidth
        + navigationCellSize * 0.5f) / navigationCellSize);
    LONG startRow = static_cast<LONG>((currentPlayerStartY - enemyHalfHeight
        + navigationCellSize * 0.5f) / navigationCellSize);
    if (!NavigationCellValid(startColumn, startRow))
    {
        return false;
    }
    LONG read = 0;
    LONG written = 0;
    LONG start = startRow * navigationColumns + startColumn;
    navigationState[start] = 1;
    navigationPath[pressureEnemyIndex][written++] = static_cast<unsigned short>(start);
    constexpr LONG neighborX[4] = { -1, 1, 0, 0 };
    constexpr LONG neighborY[4] = { 0, 0, -1, 1 };
    while (read < written)
    {
        LONG node = navigationPath[pressureEnemyIndex][read++];
        LONG column = node % navigationColumns;
        LONG row = node / navigationColumns;
        for (LONG neighbor = 0; neighbor < 4; ++neighbor)
        {
            LONG nextColumn = column + neighborX[neighbor];
            LONG nextRow = row + neighborY[neighbor];
            if (NavigationCellValid(nextColumn, nextRow))
            {
                LONG next = nextRow * navigationColumns + nextColumn;
                if (!navigationState[next])
                {
                    navigationState[next] = 1;
                    navigationPath[pressureEnemyIndex][written++]
                        = static_cast<unsigned short>(next);
                }
            }
        }
    }
    for (LONG row = 0; row < navigationRows; ++row)
    {
        for (LONG column = 0; column < navigationColumns; ++column)
        {
            if (NavigationCellValid(column, row)
                && !navigationState[row * navigationColumns + column])
            {
                return false;
            }
        }
    }
    return true;
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
            float currentDistance = enemyX - playerX;
            float nextDistance = nextX - playerX;
            if (currentDistance < 0.0f)
            {
                currentDistance = -currentDistance;
            }
            if (nextDistance < 0.0f)
            {
                nextDistance = -nextDistance;
            }
            bool alreadyOverlapping = enemyX - enemyHalfWidth < playerX + playerHalfWidth
                && enemyX + enemyHalfWidth > playerX - playerHalfWidth;
            if (!alreadyOverlapping || nextDistance <= currentDistance)
            {
                nextX = enemyX;
            }
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
            float currentDistance = enemyY - playerY;
            float nextDistance = nextY - playerY;
            if (currentDistance < 0.0f)
            {
                currentDistance = -currentDistance;
            }
            if (nextDistance < 0.0f)
            {
                nextDistance = -nextDistance;
            }
            bool alreadyOverlapping = enemyY - enemyHalfHeight < playerY + playerHalfHeight
                && enemyY + enemyHalfHeight > playerY - playerHalfHeight;
            if (!alreadyOverlapping || nextDistance <= currentDistance)
            {
                nextY = enemyY;
            }
        }
        enemyY = nextY;
    }
    differenceX = targetX - enemyX;
    differenceY = targetY - enemyY;
    return differenceX * differenceX + differenceY * differenceY < 0.25f;
}

bool EnemyWallPositionValid(float centerX, float centerY)
{
    return centerX >= enemyHalfWidth && centerX <= worldWidth - enemyHalfWidth
        && centerY >= enemyHalfHeight && centerY <= worldHeight - enemyHalfHeight
        && !RectangleOverlapsRoomWall(centerX - enemyHalfWidth,
            centerY - enemyHalfHeight, centerX + enemyHalfWidth,
            centerY + enemyHalfHeight);
}

bool BeginPatrolWallTurn(EnemyRuntime& enemy, LONG enemyIndex, float remaining)
{
    float facingX = cosf(enemy.facingAngle);
    float facingY = sinf(enemy.facingAngle);
    float leftX = -facingY;
    float leftY = facingX;
    bool leftValid = EnemyWallPositionValid(enemy.x + leftX * 2.0f,
        enemy.y + leftY * 2.0f);
    bool rightValid = EnemyWallPositionValid(enemy.x - leftX * 2.0f,
        enemy.y - leftY * 2.0f);
    if (!leftValid && !rightValid)
    {
        return false;
    }
    bool useLeft = leftValid && (!rightValid
        || ((RoomRandom(runSeed, currentRoom) + enemyIndex + enemy.patrolRight) & 1) == 0);
    enemy.patrolTurnX = useLeft ? leftX : -leftX;
    enemy.patrolTurnY = useLeft ? leftY : -leftY;
    enemy.patrolTurnRemaining = remaining;
    enemy.facingAngle = atan2f(enemy.patrolTurnY, enemy.patrolTurnX);
    return true;
}

void CloseGameplayMenu()
{
    gameplayMenuActive = false;
    gameplayInputBlocked = upPressed || downPressed || leftPressed || rightPressed
        || zPressed || xPressed || cPressed || spacePressed;
}

void CloseSettings()
{
    settingsActive = false;
    if (settingsFromGameplay)
    {
        settingsFromGameplay = false;
        gameplayMenuSelection = 1;
    }
    else
    {
        applicationState = titleMainState;
        menuSelection = 2;
        titleStatus = 0;
    }
}

void CloseHelp()
{
    helpActive = false;
    if (helpFromGameplay)
    {
        helpFromGameplay = false;
        gameplayMenuSelection = 2;
    }
    else
    {
        applicationState = titleMainState;
        menuSelection = 3;
        titleStatus = 0;
    }
}

void ChangeAudioSetting()
{
    audioEnabled = !audioEnabled;
    WriteMetaProfile();
}

void ConfirmGameplayMenu(HWND window)
{
    if (gameplayMenuSelection == 0)
    {
        CloseGameplayMenu();
    }
    else if (gameplayMenuSelection == 1)
    {
        settingsActive = true;
        settingsFromGameplay = true;
        settingsSelection = 0;
    }
    else if (gameplayMenuSelection == 2)
    {
        helpActive = true;
        helpFromGameplay = true;
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
        if (helpActive && pressed && !escapePressed)
        {
            CloseHelp();
            InvalidateRect(window, nullptr, FALSE);
        }
        else if (settingsActive && pressed && !escapePressed)
        {
            CloseSettings();
            InvalidateRect(window, nullptr, FALSE);
        }
        else if (applicationState == characterSelectState && pressed && !escapePressed)
        {
            applicationState = preparationState;
            menuSelection = 3;
            titleStatus = 0;
            InvalidateRect(window, nullptr, FALSE);
        }
        else if (applicationState == preparationState && pressed && !escapePressed)
        {
            applicationState = titleMainState;
            menuSelection = 0;
            titleStatus = 0;
            InvalidateRect(window, nullptr, FALSE);
        }
        else if (applicationState == gameplayState && !runEndState && !upgradeMenuActive
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
            }
            InvalidateRect(window, nullptr, FALSE);
        }
        escapePressed = pressed;
        return 0;
    }

    if (message == WM_LBUTTONDOWN && applicationState == gameplayState
        && !runEndState && !upgradeMenuActive && !settingsActive && !helpActive)
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
            InvalidateRect(window, nullptr, FALSE);
        }
        else if (gameplayMenuActive && mouseX >= clientWidth / 2 - 120
            && mouseX <= clientWidth / 2 + 120)
        {
            for (LONG item = 0; item < 4; ++item)
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
        if (helpActive)
        {
            if (pressed && !zPressed)
            {
                CloseHelp();
                InvalidateRect(window, nullptr, FALSE);
            }
        }
        else if (settingsActive)
        {
            if (pressed && !zPressed)
            {
                if (settingsSelection == 0)
                {
                    ChangeAudioSetting();
                }
                else
                {
                    CloseSettings();
                }
                InvalidateRect(window, nullptr, FALSE);
            }
        }
        else if (applicationState == gameplayState)
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
                    applicationState = preparationState;
                    menuSelection = 0;
                }
                else if (menuSelection == 1)
                {
                    loadGameRequested = true;
                }
                else if (menuSelection == 2)
                {
                    settingsActive = true;
                    settingsFromGameplay = false;
                    settingsSelection = 0;
                }
                else if (menuSelection == 3)
                {
                    helpActive = true;
                    helpFromGameplay = false;
                }
                else
                {
                    DestroyWindow(window);
                }
            }
            else if (applicationState == preparationState && menuSelection < 3)
            {
                BuyCommonGlobalUpgrade(static_cast<BYTE>(menuSelection));
            }
            else if (applicationState == preparationState && menuSelection == 3)
            {
                applicationState = characterSelectState;
                menuSelection = selectedCharacter;
                characterUpgradeFocus = false;
                characterUpgradeSelection = 0;
            }
            else if (applicationState == preparationState && menuSelection == 4)
            {
                if (unlockedCharacterMask & (1 << selectedCharacter))
                {
                    applicationState = gameplayState;
                    newGameRequested = true;
                }
            }
            else if (applicationState == preparationState)
            {
                applicationState = titleMainState;
                menuSelection = 0;
            }
            else if (applicationState == characterSelectState)
            {
                BYTE character = static_cast<BYTE>(menuSelection);
                if (!characterUpgradeFocus)
                {
                    UnlockCharacter(character);
                }
                else if (characterUpgradeSelection < characterGlobalUpgradeCount)
                {
                    if (unlockedCharacterMask & (1 << character))
                    {
                        selectedCharacter = character;
                        BuyCharacterGlobalUpgrade(character,
                            static_cast<BYTE>(characterUpgradeSelection));
                    }
                    else
                    {
                        titleStatus = 3;
                    }
                }
                else if (characterUpgradeSelection == 3)
                {
                    if (unlockedCharacterMask & (1 << character))
                    {
                        selectedCharacter = character;
                        applicationState = gameplayState;
                        newGameRequested = true;
                    }
                }
                else
                {
                    applicationState = preparationState;
                    menuSelection = 3;
                }
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
            && !gameplayInputBlocked && audioEnabled
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

        if (helpActive)
        {
            return 0;
        }
        if (settingsActive && newlyPressed)
        {
            if (wParam == VK_UP && settingsSelection > 0)
            {
                --settingsSelection;
            }
            else if (wParam == VK_DOWN && settingsSelection < 1)
            {
                ++settingsSelection;
            }
            else if ((wParam == VK_LEFT || wParam == VK_RIGHT)
                && settingsSelection == 0)
            {
                ChangeAudioSetting();
            }
            InvalidateRect(window, nullptr, FALSE);
        }
        else if (gameplayMenuActive && newlyPressed
            && (wParam == VK_UP || wParam == VK_DOWN))
        {
            LONG previousSelection = gameplayMenuSelection;
            if (wParam == VK_UP && gameplayMenuSelection > 0)
            {
                --gameplayMenuSelection;
            }
            else if (wParam == VK_DOWN && gameplayMenuSelection < 3)
            {
                ++gameplayMenuSelection;
            }
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
        else if (applicationState == characterSelectState && newlyPressed)
        {
            if (wParam == VK_LEFT)
            {
                characterUpgradeFocus = false;
            }
            else if (wParam == VK_RIGHT)
            {
                characterUpgradeFocus = true;
            }
            else if (!characterUpgradeFocus && wParam == VK_UP && menuSelection > 0)
            {
                --menuSelection;
            }
            else if (!characterUpgradeFocus && wParam == VK_DOWN
                && menuSelection < characterCount - 1)
            {
                ++menuSelection;
            }
            else if (characterUpgradeFocus && wParam == VK_UP
                && characterUpgradeSelection > 0)
            {
                --characterUpgradeSelection;
            }
            else if (characterUpgradeFocus && wParam == VK_DOWN
                && characterUpgradeSelection < 4)
            {
                ++characterUpgradeSelection;
            }
            titleStatus = 0;
            InvalidateRect(window, nullptr, FALSE);
        }
        else if (applicationState != gameplayState && newlyPressed
            && (wParam == VK_UP || wParam == VK_DOWN))
        {
            if (wParam == VK_UP && menuSelection > 0)
            {
                --menuSelection;
            }
            else if (wParam == VK_DOWN
                && menuSelection < (applicationState == titleMainState ? 4
                    : 5))
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

        if (helpActive)
        {
            SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
            SetBkMode(deviceContext, TRANSPARENT);
            SetTextColor(deviceContext, RGB(220, 220, 220));
            constexpr const wchar_t* helpLines[11]
            {
                L"\uB3C4\uC6C0\uB9D0",
                L"[\uC870\uC791]",
                L"\uBC29\uD5A5\uD0A4 : \uC774\uB3D9",
                L"Z : \uBCA0\uAE30 / \uC120\uD0DD",
                L"X : \uB300\uC2DC",
                L"C : \uCC98\uD615",
                L"ESC : \uBA54\uB274 / \uB4A4\uB85C",
                L"[\uBAA9\uD45C]",
                L"\uC77C\uBC18 \uC801 \uC804\uBA78 \uC2DC \uCD9C\uAD6C\uAC00 \uC5F4\uB9B0\uB2E4.",
                L"\uC5F4\uB9B0 \uCD9C\uAD6C\uB85C \uC774\uB3D9\uD574 \uB2E4\uC74C \uBC29\uC73C\uB85C \uC9C4\uD589\uD55C\uB2E4.",
                L"OPEN \uBB34\uC801 \uC555\uBC15\uD615\uC740 \uCC98\uCE58 \uB300\uC0C1\uC774 \uC544\uB2C8\uB2E4."
            };
            int clientHeight = clientArea.bottom - clientArea.top;
            LONG top = (clientHeight - framebufferHeight) / 2;
            if (top < 0)
            {
                top = 0;
            }
            RECT line = clientArea;
            for (LONG item = 0; item < 11; ++item)
            {
                line.top = top + (item ? 18 + (item - 1) * 14 : 2);
                line.bottom = line.top + 14;
                DrawTextW(deviceContext, helpLines[item], -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            line.top = top + 164;
            line.bottom = line.top + 16;
            SetTextColor(deviceContext, RGB(255, 216, 0));
            DrawTextW(deviceContext, L"\uB4A4\uB85C", -1, &line,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            EndPaint(window, &paint);
            return 0;
        }

        if (settingsActive)
        {
            SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
            SetBkMode(deviceContext, TRANSPARENT);
            RECT line = clientArea;
            int clientHeight = clientArea.bottom - clientArea.top;
            line.top = clientHeight / 5;
            line.bottom = line.top + 30;
            SetTextColor(deviceContext, RGB(220, 220, 220));
            DrawTextW(deviceContext, L"\uC124\uC815", -1, &line,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            for (LONG item = 0; item < 2; ++item)
            {
                const wchar_t* text = item == 0
                    ? (audioEnabled ? L"\uC624\uB514\uC624: ON" : L"\uC624\uB514\uC624: OFF")
                    : L"\uB4A4\uB85C";
                line.top = clientHeight / 5 + 50 + item * 30;
                line.bottom = line.top + 24;
                SetTextColor(deviceContext, item == settingsSelection
                    ? RGB(255, 216, 0) : RGB(160, 160, 160));
                DrawTextW(deviceContext, text, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            EndPaint(window, &paint);
            return 0;
        }

        if (applicationState != gameplayState)
        {
            SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
            SetBkMode(deviceContext, TRANSPARENT);
            int clientWidth = clientArea.right - clientArea.left;
            int clientHeight = clientArea.bottom - clientArea.top;
            RECT line = clientArea;
            wchar_t titleText[64];
            if (applicationState == titleMainState)
            {
                line.top = clientHeight / 5;
                line.bottom = line.top + 30;
                SetTextColor(deviceContext, RGB(220, 220, 220));
                DrawTextW(deviceContext, L"DEAD SIGNAL", -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                for (LONG item = 0; item < 5; ++item)
                {
                    const wchar_t* text = item == 0 ? L"\uAC8C\uC784 \uC2DC\uC791"
                        : (item == 1 ? L"\uC774\uC5B4\uD558\uAE30"
                            : (item == 2 ? L"\uC124\uC815"
                                : (item == 3 ? L"\uB3C4\uC6C0\uB9D0" : L"\uC885\uB8CC")));
                    line.top = clientHeight / 5 + 50 + item * 30;
                    line.bottom = line.top + 24;
                    SetTextColor(deviceContext, item == menuSelection
                        ? RGB(255, 216, 0) : RGB(160, 160, 160));
                    DrawTextW(deviceContext, text, -1, &line,
                        DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
                if (titleStatus)
                {
                    line.top = clientHeight / 5 + 190;
                    line.bottom = line.top + 24;
                    SetTextColor(deviceContext, RGB(220, 220, 220));
                    DrawTextW(deviceContext, L"\uC800\uC7A5 \uC5C6\uC74C", -1, &line,
                        DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
            }
            else if (applicationState == preparationState)
            {
                line.top = clientHeight * 4 / 180;
                line.bottom = clientHeight * 22 / 180;
                SetTextColor(deviceContext, RGB(220, 220, 220));
                DrawTextW(deviceContext, L"COMMON GLOBAL UPGRADE", -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                line.top = clientHeight * 24 / 180;
                line.bottom = clientHeight * 40 / 180;
                wsprintfW(titleText, titleStatus == 3 ? L"COIN %03ld / LOW"
                    : (titleStatus == 4 ? L"COIN %03ld / OK"
                        : (titleStatus == 5 ? L"COIN %03ld / MAX" : L"COIN %03ld")),
                    globalCoin);
                SetTextColor(deviceContext, titleStatus == 3
                    ? RGB(255, 96, 96) : RGB(220, 220, 220));
                DrawTextW(deviceContext, titleText, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                constexpr const wchar_t* commonNames[commonUpgradeCount]
                    { L"RUN REROLL", L"FIELD RECOVERY", L"COIN SENSE" };
                for (LONG item = 0; item < 6; ++item)
                {
                    const wchar_t* text = item == 3 ? L"[ CHARACTER ]"
                        : (item == 4 ? L"[ RUN START ]" : L"[ BACK ]");
                    if (item < commonUpgradeCount)
                    {
                        BYTE level = commonGlobalLevel[item];
                        BYTE maximum = commonUpgradeMaximum[item];
                        if (level >= maximum)
                        {
                            wsprintfW(titleText, L"%s  Lv%u/%u  MAX", commonNames[item],
                                static_cast<UINT>(level), static_cast<UINT>(maximum));
                        }
                        else
                        {
                            wsprintfW(titleText, L"%s  Lv%u/%u  COST %ld",
                                commonNames[item], static_cast<UINT>(level),
                                static_cast<UINT>(maximum), commonUpgradeCost[item][level]);
                        }
                        text = titleText;
                    }
                    LONG logicalTop = item < 3 ? 46 + item * 22 : 120 + (item - 3) * 20;
                    line.top = clientHeight * logicalTop / 180;
                    line.bottom = clientHeight * (logicalTop + 17) / 180;
                    SetTextColor(deviceContext, item == menuSelection
                        ? RGB(255, 216, 0) : RGB(160, 160, 160));
                    DrawTextW(deviceContext, text, -1, &line,
                        DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
            }
            else
            {
                LONG quarter = clientWidth / 4;
                RECT divider{ quarter, 0, quarter + 1, clientHeight };
                FillRect(deviceContext, &divider,
                    reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)));
                divider.left = quarter * 2;
                divider.right = divider.left + 1;
                FillRect(deviceContext, &divider,
                    reinterpret_cast<HBRUSH>(GetStockObject(GRAY_BRUSH)));

                line.top = clientHeight * 3 / 180;
                line.bottom = clientHeight * 20 / 180;
                line.left = 0;
                line.right = quarter;
                SetTextColor(deviceContext, RGB(220, 220, 220));
                DrawTextW(deviceContext, L"CHAR", -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                line.left = quarter;
                line.right = quarter * 2;
                DrawTextW(deviceContext, L"PREVIEW", -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                line.left = quarter * 2;
                line.right = clientWidth;
                DrawTextW(deviceContext, L"CHARACTER GLOBAL UPGRADE", -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);

                line.left = 0;
                line.right = quarter;
                for (LONG item = 0; item < characterCount; ++item)
                {
                    bool unlocked = (unlockedCharacterMask & (1 << item)) != 0;
                    if (unlocked)
                    {
                        wsprintfW(titleText, item == selectedCharacter ? L"> %s" : L"%s",
                            CharacterName(static_cast<BYTE>(item)));
                    }
                    else
                    {
                        wsprintfW(titleText, L"LOCK %ld", characterUnlockCosts[item]);
                    }
                    line.top = clientHeight * (35 + item * 23) / 180;
                    line.bottom = clientHeight * (52 + item * 23) / 180;
                    SetTextColor(deviceContext, !characterUpgradeFocus && item == menuSelection
                        ? RGB(255, 216, 0) : RGB(160, 160, 160));
                    DrawTextW(deviceContext, titleText, -1, &line,
                        DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }

                BYTE previewCharacter = static_cast<BYTE>(menuSelection);
                line.left = quarter;
                line.right = quarter * 2;
                line.top = clientHeight * 30 / 180;
                line.bottom = clientHeight * 47 / 180;
                SetTextColor(deviceContext, RGB(220, 220, 220));
                DrawTextW(deviceContext, CharacterName(previewCharacter), -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                wsprintfW(titleText, L"HP %ld",
                    CharacterMaximumHP(previewCharacter) / combatScale);
                line.top = clientHeight * 93 / 180;
                line.bottom = clientHeight * 108 / 180;
                DrawTextW(deviceContext, titleText, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                wsprintfW(titleText, L"MOVE %ld",
                    static_cast<LONG>(CharacterMoveSpeed(previewCharacter) + 0.5f));
                line.top = clientHeight * 109 / 180;
                line.bottom = clientHeight * 124 / 180;
                DrawTextW(deviceContext, titleText, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                wsprintfW(titleText, L"DASH %u / %u",
                    static_cast<UINT>(CharacterDashCapacity(previewCharacter)),
                    static_cast<UINT>(characterDashCaps[previewCharacter]));
                line.top = clientHeight * 125 / 180;
                line.bottom = clientHeight * 140 / 180;
                DrawTextW(deviceContext, titleText, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                LONG previewDamage = characterProfiles[previewCharacter].slashDamage;
                if (previewDamage % combatScale)
                {
                    wsprintfW(titleText, L"DMG %ld.%ld", previewDamage / combatScale,
                        previewDamage % combatScale);
                }
                else
                {
                    wsprintfW(titleText, L"DMG %ld", previewDamage / combatScale);
                }
                line.top = clientHeight * 141 / 180;
                line.bottom = clientHeight * 155 / 180;
                DrawTextW(deviceContext, titleText, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                wsprintfW(titleText, L"TARGET %ld",
                    CharacterSlashHitCap(previewCharacter));
                line.top = clientHeight * 156 / 180;
                line.bottom = clientHeight * 170 / 180;
                DrawTextW(deviceContext, titleText, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                LONG previewScale = clientHeight / 90;
                if (previewScale < 1)
                {
                    previewScale = 1;
                }
                LONG previewLeft = quarter + quarter / 2 - 4 * previewScale;
                LONG previewTop = clientHeight * 51 / 180;
                for (LONG y = 0; y < playerHeight; ++y)
                {
                    for (LONG x = 0; x < playerWidth; ++x)
                    {
                        bool head = y < 3 && x >= 2 && x < 6;
                        bool body = y >= 3 && y < 8 && x >= 1 && x < 7;
                        bool arm = y >= 4 && y < 7 && (x == 0 || x == 7);
                        bool leg = y >= 8
                            && ((x >= 1 && x < 3) || (x >= 5 && x < 7));
                        if (head || body || arm || leg)
                        {
                            RECT pixel
                            {
                                previewLeft + x * previewScale,
                                previewTop + y * previewScale,
                                previewLeft + (x + 1) * previewScale,
                                previewTop + (y + 1) * previewScale
                            };
                            FillRect(deviceContext, &pixel,
                                reinterpret_cast<HBRUSH>(GetStockObject(
                                    head ? WHITE_BRUSH : LTGRAY_BRUSH)));
                        }
                    }
                }

                line.left = quarter * 2 + 4;
                line.right = clientWidth - 4;
                for (LONG item = 0; item < 5; ++item)
                {
                    const wchar_t* text = item == 3 ? L"[ RUN START ]" : L"[ BACK ]";
                    if (item < characterGlobalUpgradeCount)
                    {
                        BYTE level = characterGlobalLevel[previewCharacter][item];
                        BYTE maximum = characterGlobalMaximum[previewCharacter][item];
                        if (level >= maximum)
                        {
                            wsprintfW(titleText, L"%s %u/%u MAX",
                                CharacterGlobalUpgradeName(previewCharacter,
                                    static_cast<BYTE>(item)),
                                static_cast<UINT>(level), static_cast<UINT>(maximum));
                        }
                        else
                        {
                            wsprintfW(titleText, L"%s %u/%u C%ld",
                                CharacterGlobalUpgradeName(previewCharacter,
                                    static_cast<BYTE>(item)),
                                static_cast<UINT>(level), static_cast<UINT>(maximum),
                                characterGlobalCost[level]);
                        }
                        text = titleText;
                    }
                    LONG logicalTop = item < 3 ? 43 + item * 25 : 128 + (item - 3) * 22;
                    line.top = clientHeight * logicalTop / 180;
                    line.bottom = clientHeight * (logicalTop + 18) / 180;
                    SetTextColor(deviceContext, characterUpgradeFocus
                        && item == characterUpgradeSelection
                        ? RGB(255, 216, 0) : RGB(160, 160, 160));
                    DrawTextW(deviceContext, text, -1, &line,
                        DT_CENTER | DT_VCENTER | DT_SINGLELINE);
                }
                line.left = quarter;
                line.right = clientWidth;
                line.top = clientHeight * 21 / 180;
                line.bottom = clientHeight * 36 / 180;
                wsprintfW(titleText, titleStatus == 3 ? L"COIN %03ld / LOW"
                    : (titleStatus == 4 ? L"COIN %03ld / OK"
                        : (titleStatus == 5 ? L"COIN %03ld / MAX" : L"COIN %03ld")),
                    globalCoin);
                SetTextColor(deviceContext, titleStatus == 3
                    ? RGB(255, 96, 96) : RGB(220, 220, 220));
                DrawTextW(deviceContext, titleText, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
        wsprintfW(hudText, L"%ld/%ld", currentEnemyRemaining, currentEnemyCount);
        DrawTextW(deviceContext, hudText, -1, &hudLine,
            DT_LEFT | DT_TOP | DT_SINGLELINE);

        const wchar_t* roomName = currentRoomType == openRoomType ? L"Open"
            : (currentRoomType == pillarRoomType ? L"Pillar"
                : (currentRoomType == mazeRoomType ? L"Maze"
                    : (currentRoomType == trapRoomType ? L"Trap" : L"Mixed")));
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
            for (LONG item = 0; item < 4; ++item)
            {
                const wchar_t* text = item == 0 ? L"\uAC8C\uC784\uC73C\uB85C \uB3CC\uC544\uAC00\uAE30"
                    : (item == 1 ? L"\uC124\uC815"
                        : (item == 2 ? L"\uB3C4\uC6C0\uB9D0"
                            : L"\uC800\uC7A5 \uD6C4 \uD0C0\uC774\uD2C0\uB85C \uAC00\uAE30"));
                line.top = clientHeight / 5 + 50 + item * 30;
                line.bottom = line.top + 24;
                SetTextColor(deviceContext, item == gameplayMenuSelection
                    ? RGB(255, 216, 0) : RGB(160, 160, 160));
                DrawTextW(deviceContext, text, -1, &line,
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
                bool rerollExhausted = rerollUsed >= CurrentRunRerollCapacity();
                const wchar_t* text = hudText;
                wsprintfW(hudText, L"REROLL %u/%u",
                    static_cast<UINT>(rerollUsed),
                    static_cast<UINT>(CurrentRunRerollCapacity()));
                if (item != 2)
                {
                    const wchar_t* name = upgrade == moveUpgrade ? L"MOVE+"
                        : (upgrade == slashUpgrade ? L"SLASH+"
                            : (upgrade == dashUpgrade ? L"DASH+"
                                : (upgrade == silentDashUpgrade ? L"SILENT DASH"
                                    : (upgrade == executeReachUpgrade
                                        ? L"EXECUTE REACH" : L"FIELD MEDIC"))));
                    wsprintfW(hudText, L"[%s] %s %u/%u",
                        upgrade <= dashUpgrade ? L"A" : L"F", name,
                        static_cast<UINT>(CurrentUpgradeStack(upgrade)),
                        static_cast<UINT>(upgrade <= dashUpgrade ? 2 : 1));
                    text = hudText;
                }
                line.top = clientHeight / 5 + 50 + item * 30;
                line.bottom = line.top + 24;
                SetTextColor(deviceContext, item == upgradeSelection
                    ? (item == 2 && rerollExhausted ? RGB(96, 96, 96) : RGB(255, 216, 0))
                    : (item == 2 && rerollExhausted ? RGB(64, 64, 64) : RGB(160, 160, 160)));
                DrawTextW(deviceContext, text, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
        }
        else if (runEndState)
        {
            SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
            SetBkMode(deviceContext, TRANSPARENT);
            RECT line = clientArea;
            wchar_t resultText[64];
            line.top = clientHeight * 4 / 180;
            line.bottom = clientHeight * 23 / 180;
            SetTextColor(deviceContext, RGB(220, 220, 220));
            DrawTextW(deviceContext, runEndState == runClearEndState
                ? L"RUN CLEAR" : L"GAME OVER", -1, &line,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            constexpr const wchar_t* resultLabels[6]
            {
                L"CHARACTER  %s", L"ROOM  %ld / 12", L"KILL  %ld",
                L"ALERT  %u", L"COIN  %ld", L"TOTAL COIN  %ld"
            };
            for (LONG item = 0; item < 6; ++item)
            {
                if (item == 0)
                {
                    wsprintfW(resultText, resultLabels[item],
                        CharacterName(selectedCharacter));
                }
                else if (item == 1)
                {
                    wsprintfW(resultText, resultLabels[item], currentRoom + 1);
                }
                else if (item == 2)
                {
                    wsprintfW(resultText, resultLabels[item], runKillCount);
                }
                else if (item == 3)
                {
                    wsprintfW(resultText, resultLabels[item],
                        static_cast<UINT>(alertEventCount));
                }
                else if (item == 4)
                {
                    wsprintfW(resultText, resultLabels[item], runEarnedCoin);
                }
                else
                {
                    wsprintfW(resultText, resultLabels[item], globalCoin);
                }
                line.top = clientHeight * (27 + item * 17) / 180;
                line.bottom = clientHeight * (42 + item * 17) / 180;
                DrawTextW(deviceContext, resultText, -1, &line,
                    DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            }
            for (LONG item = 0; item < 2; ++item)
            {
                line.top = clientHeight * (135 + item * 21) / 180;
                line.bottom = clientHeight * (153 + item * 21) / 180;
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
    if (!ReadMetaProfile())
    {
        WriteMetaProfile();
    }

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

    for (LONG y = -enemyVisionRange; y <= enemyVisionRange; ++y)
    {
        for (LONG x = 1; x <= enemyVisionRange; ++x)
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

            if (!visionBlocked && pixelY >= 0 && pixelY < framebufferHeight
                && PointInsideVisionSector(static_cast<float>(enemyInitialCenterX),
                    static_cast<float>(enemyInitialCenterY), 1.0f, 0.0f,
                    static_cast<float>(pixelX), static_cast<float>(pixelY)))
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
    EnemyRuntime enemies[maxEnemyCount]{};
    EnemyRuntime pressureEnemy{};
    LONG playerMaxHP = characterProfiles[basicCharacter].maxHP;
    LONG playerHP = playerMaxHP;
    bool playerAlive = true;
    float playerHitRemaining = 0.0f;
    float playerInvulnerabilityRemaining = 0.0f;
    LONG dashDirectionX = 0;
    LONG dashDirectionY = 0;
    float dashDistanceCurrent = dashDistance;
    float dashDistanceRemaining = 0.0f;
    float dashCooldownRemaining = 0.0f;
    bool dashActive = false;
    float playerMoveSpeedCurrent = playerMoveSpeed;
    float slashCooldownDurationCurrent = slashCooldownDuration;
    float dashCooldownDurationCurrent = dashCooldownDuration;
    LONG slashLeft = 0;
    LONG slashTop = 0;
    LONG slashRight = 0;
    LONG slashBottom = 0;
    float slashOriginX = 0.0f;
    float slashOriginY = 0.0f;
    LONG slashDirectionX = 1;
    LONG slashDirectionY = 0;
    float slashVisualRemaining = 0.0f;
    float slashCooldownRemaining = 0.0f;
    float trapCycleElapsed = 0.0f;
    float trapDamageCooldownRemaining = 0.0f;
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
                selectedCharacter = checkpoint.character;
                playerMaxHP = CharacterMaximumHP(selectedCharacter);
                playerHP = checkpoint.playerHP;
                moveUpgradeStack = checkpoint.moveStack;
                slashUpgradeStack = checkpoint.slashStack;
                dashUpgradeStack = checkpoint.dashStack;
                functionalUpgradeFlags = checkpoint.functionalFlags;
                RecalculateAugmentStats(playerMoveSpeedCurrent,
                    slashCooldownDurationCurrent, dashCooldownDurationCurrent);
                dashDistanceCurrent = CharacterDashDistance(selectedCharacter);
                currentDashMaxCharges = CurrentDashCapacity();
                currentDashCharges = checkpoint.dashCharges < currentDashMaxCharges
                    ? checkpoint.dashCharges : currentDashMaxCharges;
                dashCooldownRemaining = currentDashCharges < currentDashMaxCharges
                    ? dashCooldownDurationCurrent : 0.0f;
                rerollUsed = checkpoint.reroll;
                runKillCount = checkpoint.runKills;
                runEarnedCoin = 0;
                alertEventCount = checkpoint.alertEvents;
                alertEventActive = false;
                runRewardGranted = false;
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
                playerMaxHP = CharacterMaximumHP(selectedCharacter);
                playerHP = playerMaxHP;
                moveUpgradeStack = 0;
                slashUpgradeStack = 0;
                dashUpgradeStack = 0;
                functionalUpgradeFlags = 0;
                RecalculateAugmentStats(playerMoveSpeedCurrent,
                    slashCooldownDurationCurrent, dashCooldownDurationCurrent);
                dashDistanceCurrent = CharacterDashDistance(selectedCharacter);
                currentDashMaxCharges = CurrentDashCapacity();
                currentDashCharges = currentDashMaxCharges;
                dashCooldownRemaining = 0.0f;
                rerollUsed = 0;
                runKillCount = 0;
                runEarnedCoin = 0;
                alertEventCount = 0;
                alertEventActive = false;
                runRewardGranted = false;
                upgradeOptionA = 0;
                upgradeOptionB = 1;
            }
            else if (!checkpointLoaded)
            {
                ++currentRoom;
            }
            SetupCurrentRoom();
            alertEventActive = false;
            sequenceComplete = false;
            runEndState = 0;
            runEndSelection = 0;
            upgradeMenuActive = false;
            gameplayMenuActive = false;
            gameplayMenuSelection = 0;
            upgradeSelection = 0;
            upgradeConfirmRequested = false;
            playerX = currentPlayerStartX;
            playerY = currentPlayerStartY;
            facingX = 1;
            facingY = 0;
            for (LONG enemyIndex = 0; enemyIndex < currentEnemyCount; ++enemyIndex)
            {
                EnemyRuntime& enemy = enemies[enemyIndex];
                enemy = {};
                enemy.x = currentEnemyStartX[enemyIndex];
                enemy.y = currentEnemyStartY[enemyIndex];
                enemy.role = currentEnemyRole[enemyIndex];
                enemy.facingAngle = currentEnemyStartFacing[enemyIndex];
                enemy.surveillanceFacingAngle = enemy.facingAngle;
                enemy.patrolReturnX = enemy.x;
                enemy.searchRandomState = RoomRandom(runSeed, currentRoom)
                    ^ (0x13579BDFu + 0x9E3779B9u * enemyIndex);
                enemy.hp = enemyMaximumHP;
                enemy.patrolRight = currentPatrolStartsRight[enemyIndex];
                enemy.alive = true;
            }
            pressureEnemy = {};
            pressureEnemy.x = currentPressureStartX;
            pressureEnemy.y = currentPressureStartY;
            pressureEnemy.role = pressureEnemyRole;
            pressureEnemy.searchRandomState = RoomRandom(runSeed, currentRoom)
                ^ 0xB5297A4Du;
            pressureEnemy.alive = currentPressureActive;
            playerAlive = true;
            currentEnemyRemaining = currentEnemyCount;
            playerHitRemaining = 0.0f;
            playerInvulnerabilityRemaining = 0.0f;
            dashDirectionX = 0;
            dashDirectionY = 0;
            dashDistanceRemaining = 0.0f;
            dashActive = false;
            slashLeft = 0;
            slashTop = 0;
            slashRight = 0;
            slashBottom = 0;
            slashOriginX = playerX;
            slashOriginY = playerY;
            slashDirectionX = facingX;
            slashDirectionY = facingY;
            slashVisualRemaining = 0.0f;
            slashCooldownRemaining = 0.0f;
            trapCycleElapsed = 0.0f;
            trapDamageCooldownRemaining = 0.0f;
            exitUnlocked = false;
            roomComplete = false;
            slashRequested = false;
            dashRequested = false;
            executeRequested = false;
            gameplayInputBlocked = upPressed || downPressed || leftPressed || rightPressed
                || zPressed || xPressed || cPressed || spacePressed;
            if (!checkpointLoaded)
            {
                WriteCheckpoint(runSeed, currentRoom, playerHP, runKillCount);
            }
            newGameRequested = false;
            QueryPerformanceCounter(&previousUpdate);
            InvalidateRect(window, nullptr, FALSE);
        }

        if (saveAndTitleRequested && applicationState == gameplayState)
        {
            WriteCheckpoint(runSeed, currentRoom, playerHP, runKillCount);
            applicationState = titleMainState;
            menuSelection = 0;
            titleStatus = 0;
            gameplayMenuActive = false;
            gameplayMenuSelection = 0;
            gameplayInputBlocked = upPressed || downPressed || leftPressed || rightPressed
                || zPressed || xPressed || cPressed || spacePressed;
            saveAndTitleRequested = false;
            QueryPerformanceCounter(&previousUpdate);
            InvalidateRect(window, nullptr, FALSE);
        }

        if (settingsActive || helpActive)
        {
            QueryPerformanceCounter(&previousUpdate);
            Sleep(1);
            continue;
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
                    if (rerollUsed < CurrentRunRerollCapacity())
                    {
                        GenerateUpgradeOffer(true);
                        ++rerollUsed;
                        upgradeSelection = 0;
                        InvalidateRect(window, nullptr, FALSE);
                    }
                }
                else
                {
                    LONG upgrade = upgradeSelection ? upgradeOptionB : upgradeOptionA;
                    if (upgrade == moveUpgrade)
                    {
                        ++moveUpgradeStack;
                    }
                    else if (upgrade == slashUpgrade)
                    {
                        ++slashUpgradeStack;
                    }
                    else if (upgrade == dashUpgrade)
                    {
                        ++dashUpgradeStack;
                    }
                    else
                    {
                        functionalUpgradeFlags |= 1 << (upgrade - silentDashUpgrade);
                    }
                    RecalculateAugmentStats(playerMoveSpeedCurrent,
                        slashCooldownDurationCurrent, dashCooldownDurationCurrent);
                    ApplyFieldRecovery(playerHP, playerMaxHP);
                    if (dashCooldownRemaining > dashCooldownDurationCurrent)
                    {
                        dashCooldownRemaining = dashCooldownDurationCurrent;
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
            LONG pendingPlayerDamage = 0;
            if (playerInvulnerabilityRemaining > 0.0f)
            {
                playerInvulnerabilityRemaining -= deltaTime;
                if (playerInvulnerabilityRemaining < 0.0f)
                {
                    playerInvulnerabilityRemaining = 0.0f;
                }
            }
            if (currentTrapCount)
            {
                trapCycleElapsed += deltaTime;
                while (trapCycleElapsed >= trapCycleDuration)
                {
                    trapCycleElapsed -= trapCycleDuration;
                }
                if (trapDamageCooldownRemaining > 0.0f)
                {
                    trapDamageCooldownRemaining -= deltaTime;
                }
            }
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
            UpdateDashRecharge(deltaTime, dashCooldownDurationCurrent,
                dashCooldownRemaining);
            if (dashRequested)
            {
                if (playerAlive && !roomComplete && !sequenceComplete
                    && !upgradeMenuActive && !dashActive
                    && ConsumeDashCharge(dashCooldownDurationCurrent,
                        dashCooldownRemaining))
                {
                    dashDirectionX = facingX;
                    dashDirectionY = facingY;
                    dashDistanceRemaining = dashDistanceCurrent;
                    dashActive = true;
                }
                dashRequested = false;
            }

            bool playerDashingThisUpdate = dashActive;
            float playerPreviousX = playerX;
            float playerPreviousY = playerY;
            float movementScale = movementX && movementY ? 0.70710678f : 1.0f;
            float movementDeltaX = movementX * playerMoveSpeedCurrent
                * movementScale * deltaTime;
            float movementDeltaY = movementY * playerMoveSpeedCurrent
                * movementScale * deltaTime;
            LONG movementSteps = 1;
            if (dashActive)
            {
                float dashFrameDistance
                    = dashDistanceCurrent / dashDuration * deltaTime;
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
            bool playerMovedThisUpdate
                = playerX != playerPreviousX || playerY != playerPreviousY;
            bool playerAudibleThisUpdate = PlayerMovementIsAudible(
                playerMovedThisUpdate, playerDashingThisUpdate);

            if (currentTrapCount && playerAlive
                && trapDamageCooldownRemaining <= 0.0f)
            {
                for (LONG trap = 0; trap < currentTrapCount; ++trap)
                {
                    if (TrapIsActive(trap, trapCycleElapsed)
                        && playerX - playerHalfWidth < currentTrapRight[trap]
                        && playerX + playerHalfWidth > currentTrapLeft[trap]
                        && playerY - playerHalfHeight < currentTrapBottom[trap]
                        && playerY + playerHalfHeight > currentTrapTop[trap])
                    {
                        QueuePlayerDamage(playerDamageAmount, pendingPlayerDamage);
                        trapDamageCooldownRemaining = trapDamageCooldownDuration;
                        break;
                    }
                }
            }

            for (LONG enemyIndex = 0; enemyIndex < currentEnemyCount; ++enemyIndex)
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
                bool& heardSuspicion = enemy.heardSuspicion;
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
            if (enemyAlive && playerAlive && enemy.role == spinnerEnemyRole
                && !enemyAlert && detectionProgress <= 0.0f
                && !enemyReturningToPatrol)
            {
                enemyFacingAngle += spinnerRotationSpeed * deltaTime;
                if (enemyFacingAngle > 3.14159265f)
                {
                    enemyFacingAngle -= 6.28318531f;
                }
            }
            if (enemyAlive && playerAlive && !enemyAlert)
            {
                float facingX = cosf(enemyFacingAngle);
                float facingY = sinf(enemyFacingAngle);
                playerInVision = PointInsideVisionSector(enemyX, enemyY,
                    facingX, facingY, playerX, playerY)
                    && !WallBlocksSegment(enemyX, enemyY, playerX, playerY);

                if (enemy.role == listenerEnemyRole && !playerInVision
                    && detectionProgress <= 0.0f && playerAudibleThisUpdate)
                {
                    float hearingDifferenceX = playerX - enemyX;
                    float hearingDifferenceY = playerY - enemyY;
                    float hearingRangeSquared = playerDashingThisUpdate
                        ? listenerDashHearingRangeSquared
                        : listenerMovementHearingRangeSquared;
                    float heardTargetDifferenceX = playerX - lastSeenPlayerX;
                    float heardTargetDifferenceY = playerY - lastSeenPlayerY;
                    if (hearingDifferenceX * hearingDifferenceX
                        + hearingDifferenceY * hearingDifferenceY <= hearingRangeSquared
                        && (!heardSuspicion
                            || heardTargetDifferenceX * heardTargetDifferenceX
                            + heardTargetDifferenceY * heardTargetDifferenceY
                            >= navigationCellSize * navigationCellSize))
                    {
                        constexpr float hearingTargetOffsetX[4]
                            = { 10.0f, -10.0f, 0.0f, 0.0f };
                        constexpr float hearingTargetOffsetY[4]
                            = { 0.0f, 0.0f, 12.0f, -12.0f };
                        LONG firstCandidate = (searchRandomState >> 30) & 3;
                        bool hearingTargetValid = false;
                        float hearingTargetX = 0.0f;
                        float hearingTargetY = 0.0f;
                        LONG hearingTargetColumn = 0;
                        LONG hearingTargetRow = 0;
                        for (LONG attempt = 0; attempt < 4 && !hearingTargetValid; ++attempt)
                        {
                            LONG candidate = (firstCandidate + attempt) & 3;
                            hearingTargetX = playerX + hearingTargetOffsetX[candidate];
                            hearingTargetY = playerY + hearingTargetOffsetY[candidate];
                            hearingTargetColumn = NavigationColumn(hearingTargetX);
                            hearingTargetRow = NavigationRow(hearingTargetY);
                            hearingTargetValid
                                = hearingTargetX >= enemyHalfWidth
                                && hearingTargetX <= worldWidth - enemyHalfWidth
                                && hearingTargetY >= enemyHalfHeight
                                && hearingTargetY <= worldHeight - enemyHalfHeight
                                && NavigationCellValid(hearingTargetColumn, hearingTargetRow)
                                && !RectangleOverlapsRoomWall(
                                    hearingTargetX - enemyHalfWidth,
                                    hearingTargetY - enemyHalfHeight,
                                    hearingTargetX + enemyHalfWidth,
                                    hearingTargetY + enemyHalfHeight);
                        }
                        if (hearingTargetValid)
                        {
                            lastSeenPlayerX = playerX;
                            lastSeenPlayerY = playerY;
                            lastSeenPlayerValid = true;
                            heardSuspicion = true;
                            lostSightElapsed = 0.0f;
                            enemyReturningToPatrol = false;
                            searchTargetX = hearingTargetX;
                            searchTargetY = hearingTargetY;
                            searchTargetValid = true;
                            searchPathCount = FindEnemyPath(enemyX, enemyY,
                                hearingTargetColumn, hearingTargetRow,
                                navigationPath[enemyIndex]);
                            searchPathIndex = 0;
                        }
                    }
                }

                if (playerInVision)
                {
                    if (enemy.role == listenerEnemyRole)
                    {
                        heardSuspicion = false;
                    }
                    searchTargetValid = false;
                    searchPathCount = 0;
                    searchPathIndex = 0;
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
                        EnterEnemyAlert(enemies, currentEnemyCount, enemyIndex);
                    }
                }
                else if (detectionProgress > 0.0f)
                {
                    float decayTime = lostSightElapsed + deltaTime - lostSightHoldDuration;
                    lostSightElapsed += deltaTime;
                    bool investigationPending = lastSeenPlayerValid
                        && (enemy.role == patrollerEnemyRole
                            || enemy.role == hunterEnemyRole);
                    if (decayTime > 0.0f && !investigationPending)
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
                            enemyPatrolReturnX = enemy.role == watcherEnemyRole
                                || enemy.role == spinnerEnemyRole
                                ? currentEnemyStartX[enemyIndex] : enemyX;
                            if (enemy.role != watcherEnemyRole
                                && enemy.role != spinnerEnemyRole
                                && enemyPatrolReturnX < currentPatrolLeft[enemyIndex])
                            {
                                enemyPatrolReturnX = currentPatrolLeft[enemyIndex];
                            }
                            else if (enemy.role != watcherEnemyRole
                                && enemy.role != spinnerEnemyRole
                                && enemyPatrolReturnX > currentPatrolRight[enemyIndex])
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
                if (enemy.attackWindupRemaining > 0.0f)
                {
                    enemyFacingAngle = atan2f(playerY - enemyY, playerX - enemyX);
                }
                else if (enemyAlert)
                {
                    float playerDifferenceX = playerX - enemyX;
                    float playerDifferenceY = playerY - enemyY;
                    bool playerReacquired
                        = playerDifferenceX * playerDifferenceX
                        + playerDifferenceY * playerDifferenceY
                        <= (enemy.role == hunterEnemyRole
                            ? hunterReacquireRangeSquared : enemyReacquireRangeSquared)
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
                            enemy.role == hunterEnemyRole
                                ? hunterAlertSpeed : enemyAlertSpeed,
                            deltaTime, playerX, playerY);
                    }
                    else
                    {
                        alertLostElapsed += deltaTime;
                        if (alertLostElapsed >= (enemy.role == hunterEnemyRole
                            ? hunterAlertSearchDuration : enemyAlertSearchDuration))
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
                                    movementTargetX, movementTargetY,
                                    enemy.role == hunterEnemyRole
                                        ? hunterAlertSpeed : enemyAlertSpeed,
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
                else if (heardSuspicion && searchTargetValid)
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
                    enemyFacingAngle = TurnToward(enemyFacingAngle,
                        atan2f(movementTargetY - enemyY, movementTargetX - enemyX),
                        enemyFacingTurnSpeed * deltaTime);
                    if (MoveEnemyToward(enemyX, enemyY,
                        movementTargetX, movementTargetY,
                        enemyPatrolSpeed, deltaTime, playerX, playerY))
                    {
                        if (searchPathIndex < searchPathCount)
                        {
                            ++searchPathIndex;
                        }
                        else
                        {
                            float hearingDifferenceX = playerX - enemyX;
                            float hearingDifferenceY = playerY - enemyY;
                            float hearingRangeSquared = playerDashingThisUpdate
                                ? listenerDashHearingRangeSquared
                                : listenerMovementHearingRangeSquared;
                            if (!playerAudibleThisUpdate
                                || hearingDifferenceX * hearingDifferenceX
                                + hearingDifferenceY * hearingDifferenceY
                                > hearingRangeSquared)
                            {
                                heardSuspicion = false;
                                lastSeenPlayerValid = false;
                                searchTargetValid = false;
                                searchPathCount = 0;
                                searchPathIndex = 0;
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
                                    = (enemyX - enemyPatrolReturnX)
                                    * (enemyX - enemyPatrolReturnX)
                                    + (enemyY - currentEnemyStartY[enemyIndex])
                                    * (enemyY - currentEnemyStartY[enemyIndex]) > 0.25f;
                                if (enemyReturningToPatrol)
                                {
                                    searchPathCount = FindEnemyPath(enemyX, enemyY,
                                        NavigationColumn(enemyPatrolReturnX),
                                        NavigationRow(currentEnemyStartY[enemyIndex]),
                                        navigationPath[enemyIndex]);
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
                        if ((enemy.role == patrollerEnemyRole
                            || enemy.role == hunterEnemyRole)
                            && lostSightElapsed >= lostSightHoldDuration)
                        {
                            if (!searchTargetValid)
                            {
                                searchTargetX = lastSeenPlayerX;
                                searchTargetY = lastSeenPlayerY;
                                searchPathCount = FindEnemyPath(enemyX, enemyY,
                                    NavigationColumn(searchTargetX),
                                    NavigationRow(searchTargetY),
                                    navigationPath[enemyIndex]);
                                searchPathIndex = 0;
                                searchTargetValid = true;
                            }
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
                            bool reached = MoveEnemyToward(enemyX, enemyY,
                                movementTargetX, movementTargetY,
                                enemyPatrolSpeed, deltaTime, playerX, playerY);
                            bool playerContact = enemyX - enemyHalfWidth
                                <= playerX + playerHalfWidth
                                && enemyX + enemyHalfWidth >= playerX - playerHalfWidth
                                && enemyY - enemyHalfHeight <= playerY + playerHalfHeight
                                && enemyY + enemyHalfHeight >= playerY - playerHalfHeight;
                            if (reached)
                            {
                                if (searchPathIndex < searchPathCount)
                                {
                                    ++searchPathIndex;
                                }
                                else
                                {
                                    lastSeenPlayerValid = false;
                                    searchTargetValid = false;
                                }
                            }
                            else if (searchPathIndex >= searchPathCount && playerContact)
                            {
                                lastSeenPlayerValid = false;
                                searchTargetValid = false;
                            }
                        }
                        else if (enemy.role != watcherEnemyRole
                            && enemy.role != spinnerEnemyRole
                            && enemy.role != patrollerEnemyRole
                            && enemy.role != hunterEnemyRole)
                        {
                            MoveEnemyToward(enemyX, enemyY, lastSeenPlayerX,
                                lastSeenPlayerY, enemyPatrolSpeed, deltaTime,
                                playerX, playerY);
                        }
                    }
                }
                else
                {
                    if ((enemy.role == watcherEnemyRole || enemy.role == spinnerEnemyRole)
                        && !enemyReturningToPatrol)
                    {
                        if (enemy.role == watcherEnemyRole)
                        {
                            enemyFacingAngle = enemy.surveillanceFacingAngle;
                        }
                    }
                    else
                    {
                        if (!enemyReturningToPatrol && enemy.patrolTurnRemaining > 0.0f)
                        {
                            float movement = enemyPatrolSpeed * deltaTime;
                            if (movement > enemy.patrolTurnRemaining)
                            {
                                movement = enemy.patrolTurnRemaining;
                            }
                            float previousX = enemyX;
                            float previousY = enemyY;
                            MoveEnemyToward(enemyX, enemyY,
                                enemyX + enemy.patrolTurnX * movement,
                                enemyY + enemy.patrolTurnY * movement,
                                enemyPatrolSpeed, deltaTime, playerX, playerY);
                            float movedX = enemyX - previousX;
                            float movedY = enemyY - previousY;
                            float moved = sqrtf(movedX * movedX + movedY * movedY);
                            if (moved > 0.001f)
                            {
                                enemy.patrolTurnRemaining -= moved;
                            }
                            else if (!EnemyWallPositionValid(
                                enemyX + enemy.patrolTurnX * 2.0f,
                                enemyY + enemy.patrolTurnY * 2.0f))
                            {
                                BeginPatrolWallTurn(enemy, enemyIndex,
                                    enemy.patrolTurnRemaining);
                            }
                            if (enemy.patrolTurnRemaining <= 0.01f)
                            {
                                enemy.patrolTurnRemaining = 0.0f;
                                enemyPatrolRight = !enemyPatrolRight;
                                enemyPatrolReturnX = enemyX;
                                if (enemyPatrolReturnX < currentPatrolLeft[enemyIndex])
                                {
                                    enemyPatrolReturnX = currentPatrolLeft[enemyIndex];
                                }
                                else if (enemyPatrolReturnX
                                    > currentPatrolRight[enemyIndex])
                                {
                                    enemyPatrolReturnX = currentPatrolRight[enemyIndex];
                                }
                                enemyReturningToPatrol = true;
                                searchPathCount = FindEnemyPath(enemyX, enemyY,
                                    NavigationColumn(enemyPatrolReturnX),
                                    NavigationRow(currentEnemyStartY[enemyIndex]),
                                    navigationPath[enemyIndex]);
                                searchPathIndex = 0;
                            }
                        }
                        else
                        {
                            float patrolTargetX = enemyPatrolRight
                                ? currentPatrolRight[enemyIndex]
                                : currentPatrolLeft[enemyIndex];
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
                            float remainingX = movementTargetX - enemyX;
                            float remainingY = movementTargetY - enemyY;
                            float remaining = sqrtf(remainingX * remainingX
                                + remainingY * remainingY);
                            float previousX = enemyX;
                            float previousY = enemyY;
                            if (MoveEnemyToward(enemyX, enemyY,
                                movementTargetX, movementTargetY,
                                enemyPatrolSpeed, deltaTime, playerX, playerY))
                            {
                                enemyX = movementTargetX;
                                enemyY = movementTargetY;
                                if (enemyReturningToPatrol
                                    && searchPathIndex < searchPathCount)
                                {
                                    ++searchPathIndex;
                                }
                                else if (enemyReturningToPatrol)
                                {
                                    enemyReturningToPatrol = false;
                                    if (enemy.role == watcherEnemyRole)
                                    {
                                        enemyFacingAngle = enemy.surveillanceFacingAngle;
                                    }
                                }
                                else
                                {
                                    enemyPatrolRight = !enemyPatrolRight;
                                }
                            }
                            else if (!enemyReturningToPatrol
                                && enemy.role != watcherEnemyRole
                                && enemy.role != spinnerEnemyRole
                                && previousX == enemyX && previousY == enemyY
                                && !EnemyWallPositionValid(
                                    enemyX + cosf(enemyFacingAngle) * 2.0f,
                                    enemyY + sinf(enemyFacingAngle) * 2.0f))
                            {
                                BeginPatrolWallTurn(enemy, enemyIndex, remaining);
                            }
                        }
                    }
                }
            }
            }

            if (pressureEnemy.alive && playerAlive)
            {
                float playerDifferenceX = playerX - pressureEnemy.x;
                float playerDifferenceY = playerY - pressureEnemy.y;
                pressureEnemy.facingAngle = atan2f(playerDifferenceY, playerDifferenceX);
                if (pressureEnemy.attackWindupRemaining > 0.0f)
                {
                }
                else if (!WallBlocksSegment(pressureEnemy.x, pressureEnemy.y,
                    playerX, playerY))
                {
                    pressureEnemy.searchTargetValid = false;
                    pressureEnemy.searchPathCount = 0;
                    pressureEnemy.searchPathIndex = 0;
                    MoveEnemyToward(pressureEnemy.x, pressureEnemy.y, playerX, playerY,
                        pressureMoveSpeed, deltaTime, playerX, playerY);
                }
                else
                {
                    float targetDifferenceX = playerX - pressureEnemy.lastSeenPlayerX;
                    float targetDifferenceY = playerY - pressureEnemy.lastSeenPlayerY;
                    if (!pressureEnemy.searchTargetValid
                        || targetDifferenceX * targetDifferenceX
                        + targetDifferenceY * targetDifferenceY
                        >= navigationCellSize * navigationCellSize)
                    {
                        constexpr float targetOffsetX[4]
                            = { 10.0f, -10.0f, 0.0f, 0.0f };
                        constexpr float targetOffsetY[4]
                            = { 0.0f, 0.0f, 12.0f, -12.0f };
                        pressureEnemy.searchRandomState
                            = pressureEnemy.searchRandomState * 1664525u + 1013904223u;
                        LONG firstCandidate = (pressureEnemy.searchRandomState >> 30) & 3;
                        pressureEnemy.searchTargetValid = false;
                        for (LONG attempt = 0; attempt < 4
                            && !pressureEnemy.searchTargetValid; ++attempt)
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
                                    candidateY + enemyHalfHeight)
                                || WallBlocksSegment(
                                    candidateX, candidateY, playerX, playerY))
                            {
                                continue;
                            }
                            pressureEnemy.lastSeenPlayerX = playerX;
                            pressureEnemy.lastSeenPlayerY = playerY;
                            pressureEnemy.searchTargetX = candidateX;
                            pressureEnemy.searchTargetY = candidateY;
                            pressureEnemy.searchPathCount = FindEnemyPath(
                                pressureEnemy.x, pressureEnemy.y, targetColumn, targetRow,
                                navigationPath[pressureEnemyIndex]);
                            pressureEnemy.searchPathIndex = 0;
                            pressureEnemy.searchTargetValid = true;
                        }
                    }

                    if (pressureEnemy.searchTargetValid)
                    {
                        float movementTargetX = pressureEnemy.searchTargetX;
                        float movementTargetY = pressureEnemy.searchTargetY;
                        if (pressureEnemy.searchPathIndex < pressureEnemy.searchPathCount)
                        {
                            LONG node = navigationPath[pressureEnemyIndex]
                                [pressureEnemy.searchPathIndex];
                            movementTargetX = enemyHalfWidth
                                + (node % navigationColumns) * navigationCellSize;
                            movementTargetY = enemyHalfHeight
                                + (node / navigationColumns) * navigationCellSize;
                        }
                        if (MoveEnemyToward(pressureEnemy.x, pressureEnemy.y,
                            movementTargetX, movementTargetY, pressureMoveSpeed,
                            deltaTime, playerX, playerY))
                        {
                            if (pressureEnemy.searchPathIndex
                                < pressureEnemy.searchPathCount)
                            {
                                ++pressureEnemy.searchPathIndex;
                            }
                            else
                            {
                                pressureEnemy.searchTargetValid = false;
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
            for (LONG enemyIndex = 0; enemyIndex < currentEnemyCount; ++enemyIndex)
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
            if (pressureEnemy.attackCooldownRemaining > 0.0f)
            {
                pressureEnemy.attackCooldownRemaining -= deltaTime;
            }

            if (slashRequested)
            {
                if (playerAlive && !roomComplete && !sequenceComplete && !upgradeMenuActive
                    && !playerDashingThisUpdate
                    && slashCooldownRemaining <= 0.0f)
                {
                    LONG slashCenterX = static_cast<LONG>(playerX);
                    LONG slashCenterY = static_cast<LONG>(playerY);
                    const CharacterProfile& profile = characterProfiles[selectedCharacter];
                    LONG currentSlashReach = CharacterSlashReach(selectedCharacter);
                    LONG currentSlashWidth = CharacterSlashWidth(selectedCharacter);
                    slashOriginX = playerX;
                    slashOriginY = playerY;
                    slashDirectionX = facingX;
                    slashDirectionY = facingY;
                    if (selectedCharacter == piercerCharacter)
                    {
                        LONG extent = currentSlashReach + playerHeight;
                        slashLeft = slashCenterX - extent;
                        slashRight = slashCenterX + extent + 1;
                        slashTop = slashCenterY - extent;
                        slashBottom = slashCenterY + extent + 1;
                    }
                    else
                    {
                        if (facingX < 0)
                        {
                            slashRight = slashCenterX - playerWidth / 2;
                            slashLeft = slashRight - currentSlashReach;
                        }
                        else if (facingX > 0)
                        {
                            slashLeft = slashCenterX + playerWidth / 2;
                            slashRight = slashLeft + currentSlashReach;
                        }
                        else
                        {
                            slashLeft = slashCenterX - currentSlashWidth / 2;
                            slashRight = slashLeft + currentSlashWidth;
                        }

                        if (facingY < 0)
                        {
                            slashBottom = slashCenterY - playerHeight / 2;
                            slashTop = slashBottom - currentSlashReach;
                        }
                        else if (facingY > 0)
                        {
                            slashTop = slashCenterY + playerHeight / 2;
                            slashBottom = slashTop + currentSlashReach;
                        }
                        else
                        {
                            slashTop = slashCenterY - currentSlashWidth / 2;
                            slashBottom = slashTop + currentSlashWidth;
                        }
                    }

                    slashVisualRemaining = slashVisualDuration;
                    slashCooldownRemaining = slashCooldownDurationCurrent;
                    LONG slashHitCount = 0;
                    for (LONG enemyIndex = 0; enemyIndex < currentEnemyCount;
                        ++enemyIndex)
                    {
                        EnemyRuntime& enemy = enemies[enemyIndex];
                        LONG enemyLeft = static_cast<LONG>(enemy.x) - enemyWidth / 2;
                        LONG enemyTop = static_cast<LONG>(enemy.y) - enemyHeight / 2;
                        bool slashOverlap = selectedCharacter == piercerCharacter
                            ? PiercerSlashHitsEnemy(slashOriginX, slashOriginY,
                                slashDirectionX, slashDirectionY, enemy.x, enemy.y)
                            : slashLeft < enemyLeft + enemyWidth
                                && slashRight > enemyLeft
                                && slashTop < enemyTop + enemyHeight
                                && slashBottom > enemyTop;
                        if (enemy.alive && slashOverlap
                            && slashHitCount < CharacterSlashHitCap(selectedCharacter))
                        {
                            ++slashHitCount;
                            enemy.hitRemaining = slashVisualDuration;
                            float hitX = enemy.x;
                            float hitY = enemy.y;
                            enemy.hp -= profile.slashDamage;
                            if (enemy.hp <= 0)
                            {
                                enemy.hp = 0;
                                enemy.alive = false;
                                --currentEnemyRemaining;
                                ++runKillCount;
                                slashCooldownRemaining = 0.0f;
                                ResetDashRecharge(dashCooldownRemaining,
                                    dashCooldownDurationCurrent);
                                enemy.playerInVision = false;
                                enemy.searchTargetValid = false;
                                enemy.searchPathCount = 0;
                                enemy.searchPathIndex = 0;
                                enemy.scanning = false;
                                enemy.attackWindupRemaining = 0.0f;
                            }
                            else
                            {
                                EnterEnemyAlert(enemies, currentEnemyCount,
                                    enemyIndex);
                            }
                            AlertEnemiesNear(enemies, currentEnemyCount, hitX, hitY);
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
                    LONG currentExecuteReach = CurrentExecuteReach();
                    LONG executeLeft = executeCenterX - playerWidth / 2
                        - currentExecuteReach;
                    LONG executeRight = executeCenterX + playerWidth / 2
                        + currentExecuteReach;
                    LONG executeTop = executeCenterY - playerHeight / 2
                        - currentExecuteReach;
                    LONG executeBottom = executeCenterY + playerHeight / 2
                        + currentExecuteReach;
                    float executeFacingScale = facingX && facingY
                        ? 0.70710678f : 1.0f;
                    float executeFacingX = facingX * executeFacingScale;
                    float executeFacingY = facingY * executeFacingScale;

                    LONG selectedEnemy = -1;
                    float nearestDistanceSquared = 3.402823466e+38F;
                    for (LONG enemyIndex = 0; enemyIndex < currentEnemyCount;
                        ++enemyIndex)
                    {
                        EnemyRuntime& enemy = enemies[enemyIndex];
                        LONG enemyLeft = static_cast<LONG>(enemy.x) - enemyWidth / 2;
                        LONG enemyTop = static_cast<LONG>(enemy.y) - enemyHeight / 2;
                        float differenceX = enemy.x - playerX;
                        float differenceY = enemy.y - playerY;
                        float distanceSquared = differenceX * differenceX
                            + differenceY * differenceY;
                        if (enemy.alive && enemy.detectionProgress <= 0.0f
                            && !enemy.heardSuspicion
                            && executeLeft < enemyLeft + enemyWidth
                            && executeRight > enemyLeft
                            && executeTop < enemyTop + enemyHeight
                            && executeBottom > enemyTop
                            && PointInsideExecuteFacing(playerX, playerY,
                                executeFacingX, executeFacingY, enemy.x, enemy.y)
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
                        ApplyFieldMedic(playerHP);
                        enemy.executeFeedbackRemaining = slashVisualDuration;
                        slashCooldownRemaining = 0.0f;
                        ResetDashRecharge(dashCooldownRemaining,
                            dashCooldownDurationCurrent);
                        enemy.playerInVision = false;
                        enemy.searchTargetValid = false;
                        enemy.searchPathCount = 0;
                        enemy.searchPathIndex = 0;
                        enemy.scanning = false;
                        enemy.attackWindupRemaining = 0.0f;
                    }
                }
                executeRequested = false;
            }

            UpdateAlertEventState(enemies, currentEnemyCount);

            for (LONG enemyIndex = 0; enemyIndex < currentEnemyCount && playerAlive;
                ++enemyIndex)
            {
                EnemyRuntime& enemy = enemies[enemyIndex];
                UpdateEnemyAttack(enemy, enemy.alert, playerX, playerY,
                    deltaTime, pendingPlayerDamage);
            }
            if (pressureEnemy.alive && playerAlive)
            {
                UpdateEnemyAttack(pressureEnemy, true, playerX, playerY,
                    deltaTime, pendingPlayerDamage);
            }

            if (playerAlive && pendingPlayerDamage > 0
                && playerInvulnerabilityRemaining <= 0.0f)
            {
                playerHP -= pendingPlayerDamage;
                playerHitRemaining = playerHitFeedbackDuration;
                playerInvulnerabilityRemaining = playerInvulnerabilityDuration;
                if (playerHP <= 0)
                {
                    playerHP = 0;
                    playerAlive = false;
                    dashActive = false;
                    dashDistanceRemaining = 0.0f;
                    slashRequested = false;
                    dashRequested = false;
                    executeRequested = false;
                    for (LONG enemyIndex = 0; enemyIndex < currentEnemyCount;
                        ++enemyIndex)
                    {
                        enemies[enemyIndex].playerInVision = false;
                    }
                    runEndState = gameOverEndState;
                    runEndSelection = 0;
                    GrantRunCoin(false);
                    DeleteFileW(saveFileName);
                }
            }

            exitUnlocked = playerAlive && currentEnemyRemaining == 0;
            if (playerAlive && exitUnlocked && !roomComplete && !sequenceComplete
                && playerX - playerHalfWidth < currentExitRight
                && playerX + playerHalfWidth > currentExitLeft
                && playerY - playerHalfHeight < currentExitBottom
                && playerY + playerHalfHeight > currentExitTop)
            {
                if (currentRoom == roomCount - 1)
                {
                    sequenceComplete = true;
                }
                else if (!RoomClearsToUpgrade(currentRoom))
                {
                    roomComplete = true;
                }
                else
                {
                    GenerateUpgradeOffer(false);
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

            for (LONG enemyIndex = 0; enemyIndex < currentEnemyCount; ++enemyIndex)
            {
                EnemyRuntime& enemy = enemies[enemyIndex];
                LONG enemyCenterX = static_cast<LONG>(enemy.x);
                LONG enemyCenterY = static_cast<LONG>(enemy.y);
                if (!enemy.alive || enemy.alert)
                {
                    continue;
                }
                float redDistance = enemyVisionRange * enemy.detectionProgress;
                float redDistanceSquared = redDistance * redDistance;
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
                        float visionDistanceSquared
                            = visionX * visionX + visionY * visionY;
                        if (PointInsideVisionSector(enemy.x, enemy.y,
                            enemyFacingX, enemyFacingY, worldPixelX, worldPixelY)
                            && !WallBlocksSegment(enemy.x, enemy.y,
                                worldPixelX, worldPixelY))
                        {
                            framebuffer[pixelY * framebufferWidth + pixelX]
                                = visionDistanceSquared <= redDistanceSquared
                                    ? 0x00401818 : 0x00182040;
                        }
                    }
                }
            }

            for (LONG trap = 0; trap < currentTrapCount; ++trap)
            {
                bool active = TrapIsActive(trap, trapCycleElapsed);
                for (LONG y = currentTrapTop[trap]; y < currentTrapBottom[trap]; ++y)
                {
                    for (LONG x = currentTrapLeft[trap]; x < currentTrapRight[trap]; ++x)
                    {
                        LONG screenX = x - cameraX;
                        LONG screenY = y - cameraY;
                        if (screenX >= 0 && screenX < framebufferWidth
                            && screenY >= 0 && screenY < framebufferHeight)
                        {
                            bool outline = x == currentTrapLeft[trap]
                                || x == currentTrapRight[trap] - 1
                                || y == currentTrapTop[trap]
                                || y == currentTrapBottom[trap] - 1;
                            DWORD color = outline ? 0x00484858 : 0x00242830;
                            if (active)
                            {
                                color = ((x + y) & 3) < 2 ? 0x00F06030 : 0x00FFB040;
                            }
                            framebuffer[screenY * framebufferWidth + screenX] = color;
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
                        bool outline = selectedCharacter == piercerCharacter
                            ? PiercerSlashOutlinePixel(slashOriginX, slashOriginY,
                                slashDirectionX, slashDirectionY,
                                x + 0.5f, y + 0.5f)
                            : x == slashLeft || x == slashRight - 1
                                || y == slashTop || y == slashBottom - 1;
                        if (screenX >= 0 && screenX < framebufferWidth
                            && screenY >= 0 && screenY < framebufferHeight
                            && outline)
                        {
                            framebuffer[screenY * framebufferWidth + screenX] = 0x00E0E0E0;
                        }
                    }
                }
            }

            for (LONG enemyIndex = 0; enemyIndex < currentEnemyCount; ++enemyIndex)
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
                            bool watcherEye = enemy.role == watcherEnemyRole
                                && y == 1 && x >= 2 && x < 6;
                            bool hunterMark = enemy.role == hunterEnemyRole
                                && y >= 4 && y < 8 && x == 3;
                            bool listenerMark = enemy.role == listenerEnemyRole
                                && y == 1 && (x == 1 || x == 6);
                            bool spinnerMark = enemy.role == spinnerEnemyRole
                                && y == 4 && x >= 2 && x < 6;
                            LONG pixelX = enemyLeft + x - cameraX;
                            LONG pixelY = enemyTop + y - cameraY;
                            if ((head || body || arm || leg)
                                && pixelX >= 0 && pixelX < framebufferWidth
                                && pixelY >= 0 && pixelY < framebufferHeight)
                            {
                                framebuffer[pixelY * framebufferWidth + pixelX]
                                    = enemy.hitRemaining > 0.0f
                                        ? 0x00FFFFFF
                                        : (enemy.attackWindupRemaining > 0.0f
                                            ? (((x + y) & 1)
                                                ? 0x00FFF060 : 0x00FF8020)
                                        : (watcherEye ? 0x00FFD060
                                            : (hunterMark ? 0x00C060FF
                                                : (listenerMark ? 0x0040E0C0
                                                    : (spinnerMark ? 0x00FF9040
                                                        : (head ? 0x00FF4040 : 0x00A02020))))));
                            }
                        }
                    }
                }

                if (enemy.alive
                    && (enemy.detectionProgress > 0.0f || enemy.heardSuspicion))
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

            if (pressureEnemy.alive)
            {
                LONG pressureLeft = static_cast<LONG>(pressureEnemy.x)
                    - pressureVisualWidth / 2 - cameraX;
                LONG pressureTop = static_cast<LONG>(pressureEnemy.y)
                    - pressureVisualHeight / 2 - cameraY;
                for (LONG y = 0; y < pressureVisualHeight; ++y)
                {
                    for (LONG x = 0; x < pressureVisualWidth; ++x)
                    {
                        bool head = y < 3 && x >= 1 && x < 9;
                        bool body = y >= 3 && y < 9 && x >= 1 && x < 9;
                        bool arm = y >= 4 && y < 8 && (x == 0 || x == 9);
                        bool leg = y >= 9 && (x < 4 || x >= 6);
                        bool pressureMark = (y == 4 || y == 6) && x >= 3 && x < 7;
                        LONG pixelX = pressureLeft + x;
                        LONG pixelY = pressureTop + y;
                        if ((head || body || arm || leg)
                            && pixelX >= 0 && pixelX < framebufferWidth
                            && pixelY >= 0 && pixelY < framebufferHeight)
                        {
                            framebuffer[pixelY * framebufferWidth + pixelX]
                                = pressureEnemy.attackWindupRemaining > 0.0f
                                    ? (((x + y) & 1) ? 0x00FFF060 : 0x00FF8020)
                                    : (pressureMark ? 0x00FFF080
                                        : (head ? 0x00D0A060 : 0x00705030));
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
            LONG healthFillWidth = playerHP * healthBarWidth / playerMaxHP;
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
                    bool mobilityMark = selectedCharacter == mobilityCharacter
                        && y == 4 && (x == 1 || x == 6);
                    bool piercerMark = selectedCharacter == piercerCharacter
                        && x == 3 && y >= 3 && y < 8;
                    bool heavyMark = selectedCharacter == heavyCharacter
                        && y == 6 && x >= 1 && x < 7;
                    bool rapidMark = selectedCharacter == rapidCharacter
                        && (y == 4 || y == 6) && (x == 3 || x == 4);
                    LONG pixelX = drawingLeft + x;
                    LONG pixelY = drawingTop + y;
                    if ((head || body || arm || leg)
                        && pixelX >= 0 && pixelX < framebufferWidth
                        && pixelY >= 0 && pixelY < framebufferHeight)
                    {
                        framebuffer[pixelY * framebufferWidth + pixelX]
                            = !playerAlive ? 0x00404050
                            : (playerHitRemaining > 0.0f ? 0x00FFFFFF
                                : (playerDashingThisUpdate ? 0x0080FFFF
                                    : (head ? 0x00FFFFFF
                                        : (mobilityMark ? 0x0040E0C0
                                            : (piercerMark ? 0x00C060FF
                                                : (heavyMark ? 0x00FF9040
                                                    : (rapidMark ? 0x00FFF060
                                                        : 0x0000A0FF)))))));
                    }
                }
            }

            LONG dashIndicatorWidth = currentDashMaxCharges * 4 - 1;
            LONG dashIndicatorLeft = healthBarLeft
                + (healthBarWidth - dashIndicatorWidth) / 2;
            LONG dashIndicatorTop = healthBarTop - 4;
            if (dashIndicatorLeft < 0)
            {
                dashIndicatorLeft = 0;
            }
            else if (dashIndicatorLeft + dashIndicatorWidth > framebufferWidth)
            {
                dashIndicatorLeft = framebufferWidth - dashIndicatorWidth;
            }
            if (dashIndicatorTop < 0)
            {
                dashIndicatorTop = drawingTop + playerHeight + 1;
            }
            for (LONG charge = 0; charge < currentDashMaxCharges; ++charge)
            {
                for (LONG y = 0; y < 3; ++y)
                {
                    for (LONG x = 0; x < 3; ++x)
                    {
                        LONG pixelX = dashIndicatorLeft + charge * 4 + x;
                        LONG pixelY = dashIndicatorTop + y;
                        if (pixelY >= 0 && pixelY < framebufferHeight)
                        {
                            bool filled = charge < currentDashCharges;
                            bool outline = x == 0 || x == 2 || y == 0 || y == 2;
                            framebuffer[pixelY * framebufferWidth + pixelX]
                                = filled ? 0x0080FFFF
                                    : (outline ? 0x00405060 : 0x00101018);
                        }
                    }
                }
            }

            DrawEnemyLocators(enemies, currentEnemyCount, cameraX, cameraY);

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
                GrantRunCoin(true);
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

#ifdef DEAD_SIGNAL_B01_VALIDATION
DWORD ValidationHashValue(DWORD hash, DWORD value)
{
    return (hash ^ value) * 16777619u;
}

DWORD ValidationHashFloat(DWORD hash, float value)
{
    return ValidationHashValue(hash, *reinterpret_cast<DWORD*>(&value));
}

DWORD CurrentGenerationHash()
{
    DWORD hash = 2166136261u;
    hash = ValidationHashValue(hash, currentRoomType);
    hash = ValidationHashValue(hash, currentEnemyCount);
    hash = ValidationHashValue(hash, roomSizeStage);
    hash = ValidationHashValue(hash, currentExitSide);
    hash = ValidationHashValue(hash, currentExitLeft);
    hash = ValidationHashValue(hash, currentExitTop);
    hash = ValidationHashValue(hash, currentExitRight);
    hash = ValidationHashValue(hash, currentExitBottom);
    hash = ValidationHashFloat(hash, currentPlayerStartX);
    hash = ValidationHashFloat(hash, currentPlayerStartY);
    hash = ValidationHashValue(hash, currentWallCount);
    for (LONG wall = 0; wall < currentWallCount; ++wall)
    {
        hash = ValidationHashValue(hash, currentWallLeft[wall]);
        hash = ValidationHashValue(hash, currentWallTop[wall]);
        hash = ValidationHashValue(hash, currentWallRight[wall]);
        hash = ValidationHashValue(hash, currentWallBottom[wall]);
    }
    hash = ValidationHashValue(hash, currentTrapCount);
    for (LONG trap = 0; trap < currentTrapCount; ++trap)
    {
        hash = ValidationHashValue(hash, currentTrapLeft[trap]);
        hash = ValidationHashValue(hash, currentTrapTop[trap]);
        hash = ValidationHashValue(hash, currentTrapRight[trap]);
        hash = ValidationHashValue(hash, currentTrapBottom[trap]);
        hash = ValidationHashFloat(hash, currentTrapPhaseOffset[trap]);
    }
    for (LONG enemy = 0; enemy < currentEnemyCount; ++enemy)
    {
        hash = ValidationHashFloat(hash, currentEnemyStartX[enemy]);
        hash = ValidationHashFloat(hash, currentEnemyStartY[enemy]);
        hash = ValidationHashFloat(hash, currentEnemyStartFacing[enemy]);
        hash = ValidationHashFloat(hash, currentPatrolLeft[enemy]);
        hash = ValidationHashFloat(hash, currentPatrolRight[enemy]);
        hash = ValidationHashValue(hash, currentPatrolStartsRight[enemy]);
        hash = ValidationHashValue(hash, currentEnemyRole[enemy]);
    }
    hash = ValidationHashValue(hash, currentPressureActive);
    hash = ValidationHashFloat(hash, currentPressureStartX);
    return ValidationHashFloat(hash, currentPressureStartY);
}

bool ValidationOverlap(float leftA, float topA, float rightA, float bottomA,
    float leftB, float topB, float rightB, float bottomB)
{
    return leftA < rightB && rightA > leftB && topA < bottomB && bottomA > topB;
}

void MarkValidationReachable()
{
    for (LONG node = 0; node < navigationNodeCount; ++node)
    {
        navigationState[node] = 0;
    }
    LONG startColumn = NavigationColumn(currentPlayerStartX);
    LONG startRow = NavigationRow(currentPlayerStartY);
    LONG start = startRow * navigationColumns + startColumn;
    LONG read = 0;
    LONG written = 0;
    if (NavigationCellValid(startColumn, startRow))
    {
        navigationState[start] = 1;
        navigationPath[pressureEnemyIndex][written++] = static_cast<unsigned short>(start);
    }
    constexpr LONG neighborX[4] = { -1, 1, 0, 0 };
    constexpr LONG neighborY[4] = { 0, 0, -1, 1 };
    while (read < written)
    {
        LONG node = navigationPath[pressureEnemyIndex][read++];
        LONG column = node % navigationColumns;
        LONG row = node / navigationColumns;
        for (LONG neighbor = 0; neighbor < 4; ++neighbor)
        {
            LONG nextColumn = column + neighborX[neighbor];
            LONG nextRow = row + neighborY[neighbor];
            if (NavigationCellValid(nextColumn, nextRow))
            {
                LONG next = nextRow * navigationColumns + nextColumn;
                if (!navigationState[next])
                {
                    navigationState[next] = 1;
                    navigationPath[pressureEnemyIndex][written++]
                        = static_cast<unsigned short>(next);
                }
            }
        }
    }
}

int main()
{
    unsigned long long wrongEnemyCount = 0;
    unsigned long long wrongRoomSize = 0;
    unsigned long long enemyCapacityOverflow = 0;
    unsigned long long invalidRole = 0;
    unsigned long long listenerCountFailure = 0;
    unsigned long long listenerSpawnOverlap = 0;
    unsigned long long deterministicMismatch = 0;
    unsigned long long playerOverlap = 0;
    unsigned long long enemyOverlap = 0;
    unsigned long long wallOverlap = 0;
    unsigned long long trapOverlap = 0;
    unsigned long long exitOverlap = 0;
    unsigned long long pressureInvalidSpawn = 0;
    unsigned long long wallTrapOverlap = 0;
    unsigned long long exitBlocked = 0;
    unsigned long long unreachablePlayerExit = 0;
    unsigned long long unreachableEnemy = 0;
    unsigned long long mazeQuadrantCount = 0;
    unsigned long long mazeOrientationFailure = 0;
    unsigned long long mazeThicknessFailure = 0;
    unsigned long long mazeQuadrantBoundsFailure = 0;
    unsigned long long mazeZeroQuadrants[roomCount]{};
    unsigned long long mazeQuadrants[roomCount]{};
    unsigned long long mazeLengthTotal[5][2]{};
    unsigned long long mazeLengthCount[5][2]{};
    unsigned long long mazeRoomCount[5]{};
    LONG mazeOrientationMask = 0;
    unsigned long long densityCountFailure = 0;
    unsigned long long invalidExitSide = 0;
    bool exitSeen[4]{};
    bool roleSeen[enemyRoleCount]{};
    bool duplicateRoleSeen = false;

    for (DWORD seed = 1; seed <= 100000; ++seed)
    {
        for (currentRoom = 0; currentRoom < roomCount; ++currentRoom)
        {
            runSeed = seed;
            SetupCurrentRoom();
            LONG expectedEnemyCount = 2 + currentRoom * 2;
            LONG expectedSize = expectedEnemyCount <= 4 ? 2
                : (expectedEnemyCount <= 10 ? 3 : (expectedEnemyCount <= 18 ? 4 : 5));
            wrongEnemyCount += currentEnemyCount != expectedEnemyCount;
            wrongRoomSize += roomSizeStage != expectedSize;
            enemyCapacityOverflow += currentEnemyCount > maxEnemyCount;
            invalidExitSide += currentExitSide < 0 || currentExitSide > 3;
            if (currentExitSide >= 0 && currentExitSide < 4)
            {
                exitSeen[currentExitSide] = true;
            }

            LONG expectedWalls = -1;
            LONG expectedTraps = 0;
            constexpr LONG pillarCounts[5] = { 1, 4, 6, 12, 20 };
            constexpr LONG trapCounts[5] = { 1, 3, 7, 12, 19 };
            constexpr LONG mixedWallCounts[5] = { 1, 2, 4, 7, 12 };
            constexpr LONG mixedTrapCounts[5] = { 1, 2, 4, 7, 11 };
            if (currentRoomType == openRoomType)
            {
                expectedWalls = 0;
            }
            else if (currentRoomType == pillarRoomType)
            {
                expectedWalls = pillarCounts[roomSizeStage - 1];
            }
            else if (currentRoomType == trapRoomType)
            {
                expectedTraps = trapCounts[roomSizeStage - 1];
            }
            else if (currentRoomType == mixedRoomType)
            {
                expectedWalls = mixedWallCounts[roomSizeStage - 1];
                expectedTraps = mixedTrapCounts[roomSizeStage - 1];
            }
            densityCountFailure += (expectedWalls >= 0 && currentWallCount != expectedWalls)
                || currentTrapCount != expectedTraps;

            bool roomHasDuplicateRole = false;
            LONG generatedListenerCount = 0;
            for (LONG enemy = 0; enemy < currentEnemyCount; ++enemy)
            {
                BYTE role = currentEnemyRole[enemy];
                if (role >= enemyRoleCount)
                {
                    ++invalidRole;
                }
                else
                {
                    roleSeen[role] = true;
                    generatedListenerCount += role == listenerEnemyRole;
                    for (LONG other = 0; other < enemy; ++other)
                    {
                        roomHasDuplicateRole |= role == currentEnemyRole[other];
                    }
                }
            }
            listenerCountFailure += generatedListenerCount
                != ListenerTargetForRoomSize(roomSizeStage, currentEnemyCount);
            duplicateRoleSeen |= roomHasDuplicateRole;

            MarkValidationReachable();
            LONG exitX = currentExitSide == 0 ? 4
                : (currentExitSide == 1 ? worldWidth - 4 : worldWidth / 2);
            LONG exitY = currentExitSide == 2 ? 6
                : (currentExitSide == 3 ? worldHeight - 6 : worldHeight / 2);
            LONG exitNode = NavigationRow(static_cast<float>(exitY)) * navigationColumns
                + NavigationColumn(static_cast<float>(exitX));
            unreachablePlayerExit += !navigationState[exitNode];

            bool playerInvalid = RectangleOverlapsRoomWall(
                currentPlayerStartX - playerHalfWidth,
                currentPlayerStartY - playerHalfHeight,
                currentPlayerStartX + playerHalfWidth,
                currentPlayerStartY + playerHalfHeight)
                || RectangleOverlapsRoomTrap(currentPlayerStartX - playerHalfWidth,
                    currentPlayerStartY - playerHalfHeight,
                    currentPlayerStartX + playerHalfWidth,
                    currentPlayerStartY + playerHalfHeight)
                || ValidationOverlap(currentPlayerStartX - playerHalfWidth,
                    currentPlayerStartY - playerHalfHeight,
                    currentPlayerStartX + playerHalfWidth,
                    currentPlayerStartY + playerHalfHeight,
                    static_cast<float>(currentExitLeft), static_cast<float>(currentExitTop),
                    static_cast<float>(currentExitRight), static_cast<float>(currentExitBottom));
            playerOverlap += playerInvalid;

            for (LONG wall = 0; wall < currentWallCount; ++wall)
            {
                bool invalidWall = currentWallLeft[wall] < 0 || currentWallTop[wall] < 0
                    || currentWallRight[wall] > worldWidth
                    || currentWallBottom[wall] > worldHeight;
                wallOverlap += invalidWall;
                exitBlocked += ValidationOverlap(static_cast<float>(currentWallLeft[wall]),
                    static_cast<float>(currentWallTop[wall]),
                    static_cast<float>(currentWallRight[wall]),
                    static_cast<float>(currentWallBottom[wall]),
                    static_cast<float>(currentExitLeft), static_cast<float>(currentExitTop),
                    static_cast<float>(currentExitRight), static_cast<float>(currentExitBottom));
                for (LONG other = wall + 1; other < currentWallCount; ++other)
                {
                    wallOverlap += ValidationOverlap(
                        static_cast<float>(currentWallLeft[wall]),
                        static_cast<float>(currentWallTop[wall]),
                        static_cast<float>(currentWallRight[wall]),
                        static_cast<float>(currentWallBottom[wall]),
                        static_cast<float>(currentWallLeft[other]),
                        static_cast<float>(currentWallTop[other]),
                        static_cast<float>(currentWallRight[other]),
                        static_cast<float>(currentWallBottom[other]));
                }
            }
            for (LONG trap = 0; trap < currentTrapCount; ++trap)
            {
                trapOverlap += currentTrapLeft[trap] < 0 || currentTrapTop[trap] < 0
                    || currentTrapRight[trap] > worldWidth
                    || currentTrapBottom[trap] > worldHeight;
                exitOverlap += ValidationOverlap(static_cast<float>(currentTrapLeft[trap]),
                    static_cast<float>(currentTrapTop[trap]),
                    static_cast<float>(currentTrapRight[trap]),
                    static_cast<float>(currentTrapBottom[trap]),
                    static_cast<float>(currentExitLeft), static_cast<float>(currentExitTop),
                    static_cast<float>(currentExitRight), static_cast<float>(currentExitBottom));
                for (LONG other = trap + 1; other < currentTrapCount; ++other)
                {
                    trapOverlap += ValidationOverlap(
                        static_cast<float>(currentTrapLeft[trap]),
                        static_cast<float>(currentTrapTop[trap]),
                        static_cast<float>(currentTrapRight[trap]),
                        static_cast<float>(currentTrapBottom[trap]),
                        static_cast<float>(currentTrapLeft[other]),
                        static_cast<float>(currentTrapTop[other]),
                        static_cast<float>(currentTrapRight[other]),
                        static_cast<float>(currentTrapBottom[other]));
                }
                for (LONG wall = 0; wall < currentWallCount; ++wall)
                {
                    wallTrapOverlap += ValidationOverlap(
                        static_cast<float>(currentTrapLeft[trap]),
                        static_cast<float>(currentTrapTop[trap]),
                        static_cast<float>(currentTrapRight[trap]),
                        static_cast<float>(currentTrapBottom[trap]),
                        static_cast<float>(currentWallLeft[wall]),
                        static_cast<float>(currentWallTop[wall]),
                        static_cast<float>(currentWallRight[wall]),
                        static_cast<float>(currentWallBottom[wall]));
                }
            }

            for (LONG enemy = 0; enemy < currentEnemyCount; ++enemy)
            {
                float left = currentEnemyStartX[enemy] - enemyHalfWidth;
                float top = currentEnemyStartY[enemy] - enemyHalfHeight;
                float right = currentEnemyStartX[enemy] + enemyHalfWidth;
                float bottom = currentEnemyStartY[enemy] + enemyHalfHeight;
                bool invalidSpawn = left < 0.0f || top < 0.0f || right > worldWidth
                    || bottom > worldHeight
                    || RectangleOverlapsRoomWall(left, top, right, bottom)
                    || RectangleOverlapsRoomTrap(left, top, right, bottom);
                wallOverlap += invalidSpawn;
                playerOverlap += ValidationOverlap(left, top, right, bottom,
                    currentPlayerStartX - playerHalfWidth,
                    currentPlayerStartY - playerHalfHeight,
                    currentPlayerStartX + playerHalfWidth,
                    currentPlayerStartY + playerHalfHeight);
                exitOverlap += ValidationOverlap(left, top, right, bottom,
                    static_cast<float>(currentExitLeft), static_cast<float>(currentExitTop),
                    static_cast<float>(currentExitRight), static_cast<float>(currentExitBottom));
                LONG node = NavigationRow(currentEnemyStartY[enemy]) * navigationColumns
                    + NavigationColumn(currentEnemyStartX[enemy]);
                unreachableEnemy += !navigationState[node];
                for (LONG other = enemy + 1; other < currentEnemyCount; ++other)
                {
                    bool overlaps = ValidationOverlap(left, top, right, bottom,
                        currentEnemyStartX[other] - enemyHalfWidth,
                        currentEnemyStartY[other] - enemyHalfHeight,
                        currentEnemyStartX[other] + enemyHalfWidth,
                        currentEnemyStartY[other] + enemyHalfHeight);
                    enemyOverlap += overlaps;
                    listenerSpawnOverlap += overlaps
                        && currentEnemyRole[enemy] == listenerEnemyRole
                        && currentEnemyRole[other] == listenerEnemyRole;
                }
            }

            if (currentPressureActive)
            {
                float left = currentPressureStartX - pressureVisualWidth / 2.0f;
                float top = currentPressureStartY - pressureVisualHeight / 2.0f;
                float right = currentPressureStartX + pressureVisualWidth / 2.0f;
                float bottom = currentPressureStartY + pressureVisualHeight / 2.0f;
                bool invalid = currentRoomType != openRoomType || left < 0.0f || top < 0.0f
                    || right > worldWidth || bottom > worldHeight
                    || RectangleOverlapsRoomWall(left, top, right, bottom)
                    || RectangleOverlapsRoomTrap(left, top, right, bottom)
                    || ValidationOverlap(left, top, right, bottom,
                        currentPlayerStartX - playerHalfWidth,
                        currentPlayerStartY - playerHalfHeight,
                        currentPlayerStartX + playerHalfWidth,
                        currentPlayerStartY + playerHalfHeight)
                    || ValidationOverlap(left, top, right, bottom,
                        static_cast<float>(currentExitLeft), static_cast<float>(currentExitTop),
                        static_cast<float>(currentExitRight),
                        static_cast<float>(currentExitBottom));
                for (LONG enemy = 0; enemy < currentEnemyCount; ++enemy)
                {
                    invalid |= ValidationOverlap(left, top, right, bottom,
                        currentEnemyStartX[enemy] - enemyHalfWidth,
                        currentEnemyStartY[enemy] - enemyHalfHeight,
                        currentEnemyStartX[enemy] + enemyHalfWidth,
                        currentEnemyStartY[enemy] + enemyHalfHeight);
                }
                LONG node = NavigationRow(currentPressureStartY) * navigationColumns
                    + NavigationColumn(currentPressureStartX);
                invalid |= !navigationState[node];
                pressureInvalidSpawn += invalid;
            }
            else
            {
                pressureInvalidSpawn += currentRoomType == openRoomType;
            }

            if (currentRoomType == mazeRoomType)
            {
                ++mazeRoomCount[roomSizeStage - 1];
                LONG quadrantWalls[4]{};
                for (LONG wall = 0; wall < currentWallCount; ++wall)
                {
                    LONG centerX = (currentWallLeft[wall] + currentWallRight[wall]) / 2;
                    LONG centerY = (currentWallTop[wall] + currentWallBottom[wall]) / 2;
                    LONG quadrant = (centerX >= worldWidth / 2)
                        | ((centerY >= worldHeight / 2) << 1);
                    ++quadrantWalls[quadrant];
                    LONG quadrantLeft = (quadrant & 1) ? worldWidth / 2 + 16 : 24;
                    LONG quadrantRight = (quadrant & 1) ? worldWidth - 24
                        : worldWidth / 2 - 16;
                    LONG quadrantTop = (quadrant & 2) ? worldHeight / 2 + 16 : 24;
                    LONG quadrantBottom = (quadrant & 2) ? worldHeight - 24
                        : worldHeight / 2 - 16;
                    mazeQuadrantBoundsFailure += currentWallLeft[wall] < quadrantLeft
                        || currentWallTop[wall] < quadrantTop
                        || currentWallRight[wall] > quadrantRight
                        || currentWallBottom[wall] > quadrantBottom;
                    LONG width = currentWallRight[wall] - currentWallLeft[wall];
                    LONG height = currentWallBottom[wall] - currentWallTop[wall];
                    LONG orientation = width > height ? 0 : 1;
                    LONG length = orientation ? height : width;
                    mazeOrientationMask |= orientation ? 2 : 1;
                    mazeThicknessFailure += (orientation ? width : height)
                        != mazeWallThickness || length <= mazeWallThickness;
                    mazeLengthTotal[roomSizeStage - 1][orientation] += length;
                    ++mazeLengthCount[roomSizeStage - 1][orientation];
                }
                for (LONG quadrant = 0; quadrant < 4; ++quadrant)
                {
                    mazeQuadrantCount += quadrantWalls[quadrant] > 3;
                    mazeZeroQuadrants[currentRoom] += quadrantWalls[quadrant] == 0;
                    ++mazeQuadrants[currentRoom];
                }
            }

            DWORD firstHash = CurrentGenerationHash();
            SetupCurrentRoom();
            deterministicMismatch += firstHash != CurrentGenerationHash();
        }
    }

    LONG missingExitSide = 0;
    LONG missingRole = 0;
    for (LONG side = 0; side < 4; ++side)
    {
        missingExitSide += !exitSeen[side];
    }
    for (LONG role = 0; role < enemyRoleCount; ++role)
    {
        missingRole += !roleSeen[role];
    }
    LONG upgradeCompatibilityFailure = 0;
    LONG runFlowFailure = 0;
    for (LONG room = 0; room < roomCount; ++room)
    {
        bool expectedUpgrade = room == 1 || room == 3 || room == 5
            || room == 7 || room == 9;
        runFlowFailure += RoomClearsToUpgrade(room) != expectedUpgrade;
    }
    for (LONG move = 0; move <= 2; ++move)
    {
        for (LONG slash = 0; slash <= 2; ++slash)
        {
            for (LONG dash = 0; dash <= 2; ++dash)
            {
                for (LONG flags = 0; flags < 8; ++flags)
                {
                    LONG selected = move + slash + dash + ((flags & 1) != 0)
                        + ((flags & 2) != 0) + ((flags & 4) != 0);
                    if (selected <= 5)
                    {
                        LONG available = (move < 2) + (slash < 2) + (dash < 2)
                            + ((flags & 1) == 0) + ((flags & 2) == 0)
                            + ((flags & 4) == 0);
                        upgradeCompatibilityFailure += available < 2;
                    }
                }
            }
        }
    }

    unsigned long long mazeZeroProbabilityFailure = 0;
    unsigned long long earlyZero = 0;
    unsigned long long earlyQuadrants = 0;
    unsigned long long middleZero = 0;
    unsigned long long middleQuadrants = 0;
    unsigned long long lateZero = 0;
    unsigned long long lateQuadrants = 0;
    for (LONG room = 0; room < roomCount; ++room)
    {
        if (room < 4)
        {
            earlyZero += mazeZeroQuadrants[room];
            earlyQuadrants += mazeQuadrants[room];
        }
        else if (room < 8)
        {
            middleZero += mazeZeroQuadrants[room];
            middleQuadrants += mazeQuadrants[room];
        }
        else
        {
            lateZero += mazeZeroQuadrants[room];
            lateQuadrants += mazeQuadrants[room];
        }
    }
    mazeZeroProbabilityFailure += !earlyZero || !middleZero || !lateZero
        || earlyZero * middleQuadrants <= middleZero * earlyQuadrants
        || middleZero * lateQuadrants <= lateZero * middleQuadrants;
    unsigned long long mazeLengthGrowthFailure = 0;
    unsigned long long mazeDensityGrowthFailure = 0;
    for (LONG stage = 2; stage < 5; ++stage)
    {
        unsigned long long previousWallCount = mazeLengthCount[stage - 1][0]
            + mazeLengthCount[stage - 1][1];
        unsigned long long currentStageWallCount = mazeLengthCount[stage][0]
            + mazeLengthCount[stage][1];
        mazeDensityGrowthFailure += !mazeRoomCount[stage - 1] || !mazeRoomCount[stage]
            || currentStageWallCount * mazeRoomCount[stage - 1]
                <= previousWallCount * mazeRoomCount[stage];
        for (LONG orientation = 0; orientation < 2; ++orientation)
        {
            mazeLengthGrowthFailure += !mazeLengthCount[stage - 1][orientation]
                || !mazeLengthCount[stage][orientation]
                || mazeLengthTotal[stage][orientation]
                    * mazeLengthCount[stage - 1][orientation]
                    <= mazeLengthTotal[stage - 1][orientation]
                        * mazeLengthCount[stage][orientation];
        }
    }
    mazeOrientationFailure = mazeOrientationMask != 3;

    printf("rooms=1200000 wrong_enemy=%llu wrong_size=%llu capacity=%llu invalid_role=%llu listener_count=%llu determinism=%llu\n",
        wrongEnemyCount, wrongRoomSize, enemyCapacityOverflow, invalidRole,
        listenerCountFailure, deterministicMismatch);
    printf("player_overlap=%llu enemy_overlap=%llu listener_overlap=%llu wall_invalid=%llu trap_overlap=%llu exit_overlap=%llu pressure_invalid=%llu\n",
        playerOverlap, enemyOverlap, listenerSpawnOverlap, wallOverlap, trapOverlap,
        exitOverlap, pressureInvalidSpawn);
    printf("wall_trap=%llu exit_blocked=%llu player_exit_unreachable=%llu enemy_unreachable=%llu density=%llu\n",
        wallTrapOverlap, exitBlocked, unreachablePlayerExit, unreachableEnemy,
        densityCountFailure);
    printf("maze_quadrant=%llu maze_bounds=%llu maze_thickness=%llu maze_orientation=%llu maze_zero_weight=%llu maze_density=%llu maze_length_growth=%llu\n",
        mazeQuadrantCount, mazeQuadrantBoundsFailure, mazeThicknessFailure,
        mazeOrientationFailure, mazeZeroProbabilityFailure, mazeDensityGrowthFailure,
        mazeLengthGrowthFailure);
    printf("invalid_exit=%llu missing_exit=%ld duplicate_role_seen=%d missing_role=%ld upgrade_compat=%ld run_flow=%ld\n",
        invalidExitSide, missingExitSide, duplicateRoleSeen, missingRole,
        upgradeCompatibilityFailure, runFlowFailure);
    return wrongEnemyCount || wrongRoomSize || enemyCapacityOverflow || invalidRole
        || listenerCountFailure || deterministicMismatch || playerOverlap
        || enemyOverlap || listenerSpawnOverlap || wallOverlap
        || trapOverlap || exitOverlap || pressureInvalidSpawn || wallTrapOverlap
        || exitBlocked || unreachablePlayerExit || unreachableEnemy || densityCountFailure
        || mazeQuadrantCount || mazeQuadrantBoundsFailure || mazeThicknessFailure
        || mazeOrientationFailure || mazeZeroProbabilityFailure || mazeDensityGrowthFailure
        || mazeLengthGrowthFailure
        || invalidExitSide || missingExitSide || !duplicateRoleSeen || missingRole
        || upgradeCompatibilityFailure || runFlowFailure;
}

extern "C" __declspec(dllexport) void CALLBACK RunB01Validation(HWND, HINSTANCE,
    LPSTR, int)
{
    FILE* output = nullptr;
    freopen_s(&output, "B01Validation.txt", "w", stdout);
    main();
    if (output)
    {
        fclose(output);
    }
}
#endif

#ifdef DEAD_SIGNAL_B02_VALIDATION
int main()
{
    LONG failures = 0;
    failures += maxEnemyCount != 24 || pressureEnemyIndex != 24;
    failures += pressureMoveSpeed != 32.0f || hunterAlertSpeed != 62.0f
        || enemyAlertSpeed != 58.0f || enemyPatrolSpeed != 24.0f;
    failures += listenerMovementHearingRangeSquared != 80.0f * 80.0f
        || listenerDashHearingRangeSquared != 120.0f * 120.0f
        || hunterReacquireRangeSquared != 70.0f * 70.0f
        || hunterAlertSearchDuration != 15.0f || lostSightHoldDuration != 0.5f;
    failures += saveVersion != 8 || sizeof(SaveCheckpoint) != 32
        || metaVersion != 2 || sizeof(LegacyMetaProfile) != 16
        || sizeof(MetaProfile) != 32;

    EnemyRuntime transition{};
    transition.alive = true;
    transition.role = patrollerEnemyRole;
    transition.searchTargetValid = true;
    transition.searchPathCount = 9;
    transition.searchPathIndex = 4;
    transition.scanning = true;
    transition.alertLostElapsed = 3.0f;
    transition.heardSuspicion = true;
    transition.patrolTurnRemaining = 12.0f;
    failures += !SetEnemyAlertState(transition) || !transition.alert
        || transition.detectionProgress != 1.0f || transition.searchTargetValid
        || transition.searchPathCount || transition.searchPathIndex
        || transition.scanning || transition.alertLostElapsed != 0.0f
        || transition.heardSuspicion || transition.patrolTurnRemaining != 0.0f;
    failures += SetEnemyAlertState(transition);

    EnemyRuntime propagation[maxEnemyCount]{};
    LONG expectedPropagation = 0;
    for (LONG index = 0; index < maxEnemyCount; ++index)
    {
        propagation[index].alive = index != 5;
        propagation[index].role = static_cast<BYTE>(index % enemyRoleCount);
        BYTE role = propagation[index].role;
        expectedPropagation += propagation[index].alive
            && (role == patrollerEnemyRole || role == hunterEnemyRole
                || role == listenerEnemyRole);
    }
    alertEventCount = 0;
    alertEventActive = false;
    LONG firstPropagation = EnterEnemyAlert(propagation, maxEnemyCount,
        listenerEnemyRole);
    for (LONG index = 0; index < maxEnemyCount; ++index)
    {
        BYTE role = propagation[index].role;
        bool expected = propagation[index].alive && (role == patrollerEnemyRole
            || role == hunterEnemyRole || role == listenerEnemyRole);
        failures += propagation[index].alert != expected;
    }
    failures += firstPropagation != expectedPropagation
        || EnterEnemyAlert(propagation, maxEnemyCount, listenerEnemyRole) != 0;

    EnemyRuntime local[5]{};
    constexpr BYTE localRoles[5]
        = { watcherEnemyRole, spinnerEnemyRole, patrollerEnemyRole,
            hunterEnemyRole, listenerEnemyRole };
    constexpr float localX[5] = { 100.0f, 140.0f, 139.0f, 140.1f, 110.0f };
    for (LONG index = 0; index < 5; ++index)
    {
        local[index].alive = index != 4;
        local[index].role = localRoles[index];
        local[index].x = localX[index];
        local[index].y = 100.0f;
    }
    LONG localEntered = AlertEnemiesNear(local, 5, 100.0f, 100.0f);
    failures += localEntered != 4 || !local[0].alert || !local[1].alert
        || !local[2].alert || !local[3].alert || local[4].alert;

    currentWallCount = 0;
    SetRoomSizeStage(2);
    float overlapX = 100.0f;
    float overlapY = 100.0f;
    for (LONG update = 0; update < 200; ++update)
    {
        MoveEnemyToward(overlapX, overlapY, 50.0f, 100.0f,
            enemyPatrolSpeed, 0.016f, 100.0f, 100.0f);
    }
    failures += overlapX > 50.5f;

    currentWallCount = 1;
    currentWallLeft[0] = 55;
    currentWallTop[0] = 38;
    currentWallRight[0] = 63;
    currentWallBottom[0] = 62;
    currentRoom = 0;
    runSeed = 1;
    EnemyRuntime turn{};
    turn.alive = true;
    turn.x = 50.0f;
    turn.y = 50.0f;
    turn.patrolRight = true;
    failures += !BeginPatrolWallTurn(turn, 0, 20.0f)
        || turn.patrolTurnX < -0.001f || turn.patrolTurnX > 0.001f
        || (turn.patrolTurnY < 0.999f && turn.patrolTurnY > -0.999f);
    float turnStartY = turn.y;
    for (LONG update = 0; update < 100 && turn.patrolTurnRemaining > 0.01f; ++update)
    {
        float movement = enemyPatrolSpeed * 0.016f;
        if (movement > turn.patrolTurnRemaining)
        {
            movement = turn.patrolTurnRemaining;
        }
        float previousX = turn.x;
        float previousY = turn.y;
        MoveEnemyToward(turn.x, turn.y, turn.x + turn.patrolTurnX * movement,
            turn.y + turn.patrolTurnY * movement, enemyPatrolSpeed, 0.016f,
            300.0f, 150.0f);
        float movedX = turn.x - previousX;
        float movedY = turn.y - previousY;
        turn.patrolTurnRemaining -= sqrtf(movedX * movedX + movedY * movedY);
    }
    failures += turn.patrolTurnRemaining > 0.01f
        || (turn.y - turnStartY < 19.0f && turnStartY - turn.y < 19.0f);

    printf("b02_failures=%ld capacity=%ld listener_first=%ld local_radius=%d overlap_escape=%d patrol_turn=%d\n",
        failures, maxEnemyCount, firstPropagation, !local[3].alert,
        overlapX <= 50.5f, turn.patrolTurnRemaining <= 0.01f);
    return failures;
}

extern "C" __declspec(dllexport) void CALLBACK RunB02Validation(HWND, HINSTANCE,
    LPSTR, int)
{
    FILE* output = nullptr;
    freopen_s(&output, "B02Validation.txt", "w", stdout);
    main();
    if (output)
    {
        fclose(output);
    }
}
#endif

#ifdef DEAD_SIGNAL_B03_VALIDATION
int main()
{
    LONG failures = 0;
    failures += slashLocalAlertRangeSquared != 70.0f * 70.0f;
    failures += listenerMovementHearingRangeSquared != 80.0f * 80.0f
        || listenerDashHearingRangeSquared != 120.0f * 120.0f;
    failures += enemyVisionRange != 70 || enemyAttackWindupDuration != 0.3f
        || playerInvulnerabilityDuration != 0.5f;
    failures += saveVersion != 8 || sizeof(SaveCheckpoint) != 32
        || metaVersion != 2 || sizeof(LegacyMetaProfile) != 16
        || sizeof(MetaProfile) != 32;

    failures += !PointInsideVisionSector(0.0f, 0.0f, 1.0f, 0.0f,
        70.0f, 0.0f);
    failures += PointInsideVisionSector(0.0f, 0.0f, 1.0f, 0.0f,
        70.0f, 1.0f);
    float insideAngle = 27.0f * 3.14159265f / 180.0f;
    float outsideAngle = 28.0f * 3.14159265f / 180.0f;
    failures += !PointInsideVisionSector(0.0f, 0.0f, 1.0f, 0.0f,
        cosf(insideAngle) * 69.0f, sinf(insideAngle) * 69.0f);
    failures += PointInsideVisionSector(0.0f, 0.0f, 1.0f, 0.0f,
        cosf(outsideAngle) * 69.0f, sinf(outsideAngle) * 69.0f);

    currentWallCount = 1;
    currentWallLeft[0] = 30;
    currentWallTop[0] = 85;
    currentWallRight[0] = 38;
    currentWallBottom[0] = 115;
    bool rawSector = PointInsideVisionSector(10.0f, 100.0f, 1.0f, 0.0f,
        60.0f, 100.0f);
    bool clippedSector = rawSector
        && !WallBlocksSegment(10.0f, 100.0f, 60.0f, 100.0f);
    failures += !rawSector || clippedSector;

    float executeInside = 84.0f * 3.14159265f / 180.0f;
    float executeOutside = 86.0f * 3.14159265f / 180.0f;
    failures += !PointInsideExecuteFacing(0.0f, 0.0f, 1.0f, 0.0f,
        cosf(executeInside) * 8.0f, sinf(executeInside) * 8.0f);
    failures += PointInsideExecuteFacing(0.0f, 0.0f, 1.0f, 0.0f,
        cosf(executeOutside) * 8.0f, sinf(executeOutside) * 8.0f);
    failures += PointInsideExecuteFacing(0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f);

    LONG pendingDamage = 0;
    QueuePlayerDamage(10, pendingDamage);
    QueuePlayerDamage(20, pendingDamage);
    QueuePlayerDamage(10, pendingDamage);
    failures += pendingDamage != 20;

    EnemyRuntime local[maxEnemyCount]{};
    for (LONG index = 0; index < maxEnemyCount; ++index)
    {
        local[index].alive = true;
        local[index].role = watcherEnemyRole;
        local[index].x = index & 1 ? 170.0f : 169.0f;
        local[index].y = 100.0f;
    }
    local[maxEnemyCount - 2].x = 170.1f;
    local[maxEnemyCount - 1].alive = false;
    local[maxEnemyCount - 1].x = 100.0f;
    AlertEnemiesNear(local, maxEnemyCount, 100.0f, 100.0f);
    for (LONG index = 0; index < maxEnemyCount; ++index)
    {
        bool expected = index != maxEnemyCount - 2
            && index != maxEnemyCount - 1;
        failures += local[index].alert != expected;
    }

    EnemyRuntime attacker{};
    attacker.alive = true;
    attacker.alert = true;
    attacker.x = 100.0f;
    attacker.y = 100.0f;
    LONG windupDamage = 0;
    UpdateEnemyAttack(attacker, true, 108.0f, 100.0f, 0.016f, windupDamage);
    failures += attacker.attackWindupRemaining != enemyAttackWindupDuration
        || windupDamage;
    float stationaryX = attacker.x;
    float stationaryY = attacker.y;
    for (LONG update = 0; update < 19; ++update)
    {
        UpdateEnemyAttack(attacker, true, 120.0f, 100.0f, 0.016f,
            windupDamage);
    }
    failures += windupDamage || attacker.attackWindupRemaining > 0.0f
        || attacker.attackCooldownRemaining != enemyAttackCooldownDuration
        || attacker.x != stationaryX || attacker.y != stationaryY;

    attacker.attackCooldownRemaining = 0.0f;
    UpdateEnemyAttack(attacker, true, 108.0f, 100.0f, 0.016f, windupDamage);
    for (LONG update = 0; update < 19; ++update)
    {
        UpdateEnemyAttack(attacker, true, 108.0f, 100.0f, 0.016f,
            windupDamage);
    }
    failures += windupDamage != playerDamageAmount;
    attacker.attackCooldownRemaining = 0.0f;
    UpdateEnemyAttack(attacker, true, 108.0f, 100.0f, 0.016f, windupDamage);
    attacker.alive = false;
    UpdateEnemyAttack(attacker, true, 108.0f, 100.0f, 0.30f, windupDamage);
    failures += attacker.attackWindupRemaining != 0.0f
        || windupDamage != playerDamageAmount;

#ifdef DEAD_SIGNAL_B03_SOAK_VALIDATION
    EnemyRuntime soakEnemies[maxEnemyCount]{};
    for (LONG index = 0; index < maxEnemyCount; ++index)
    {
        soakEnemies[index].alive = true;
        soakEnemies[index].alert = true;
        soakEnemies[index].role = static_cast<BYTE>(index % enemyRoleCount);
        soakEnemies[index].x = 100.0f + static_cast<float>(index % 3);
        soakEnemies[index].y = 100.0f + static_cast<float>(index % 5);
    }
    ULONGLONG soakStart = GetTickCount64();
    ULONGLONG soakUpdates = 0;
    LONG soakFailures = 0;
    while (GetTickCount64() - soakStart < 90000)
    {
        LONG soakDamage = 0;
        for (LONG index = 0; index < maxEnemyCount; ++index)
        {
            EnemyRuntime& soakEnemy = soakEnemies[index];
            if (soakEnemy.attackCooldownRemaining > 0.0f)
            {
                soakEnemy.attackCooldownRemaining -= 1.0f / 60.0f;
            }
            UpdateEnemyAttack(soakEnemy, soakEnemy.alert, 108.0f, 100.0f,
                1.0f / 60.0f, soakDamage);
            float facing = static_cast<float>((soakUpdates + index) % 360)
                * 3.14159265f / 180.0f;
            PointInsideVisionSector(soakEnemy.x, soakEnemy.y,
                cosf(facing), sinf(facing), 108.0f, 100.0f);
            soakFailures += soakEnemy.attackWindupRemaining < 0.0f
                || soakEnemy.attackWindupRemaining > enemyAttackWindupDuration;
        }
        soakFailures += soakDamage < 0 || soakDamage > playerDamageAmount;
        if ((soakUpdates & 255) == 0)
        {
            for (LONG index = 0; index < maxEnemyCount; ++index)
            {
                soakEnemies[index].alert = false;
            }
            AlertEnemiesNear(soakEnemies, maxEnemyCount, 100.0f, 100.0f);
        }
        ++soakUpdates;
    }
    failures += soakFailures;
    printf("soak_ms=%llu soak_updates=%llu soak_failures=%ld\n",
        GetTickCount64() - soakStart, soakUpdates, soakFailures);
#endif

    printf("b03_failures=%ld slash70=%d sector=%d wall_clip=%d execute170=%d highest=%ld windup=%d capacity=%ld\n",
        failures, local[0].alert, rawSector, !clippedSector,
        !PointInsideExecuteFacing(0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f),
        pendingDamage, windupDamage, maxEnemyCount);
    return failures;
}

extern "C" __declspec(dllexport) void CALLBACK RunB03Validation(HWND, HINSTANCE,
    LPSTR, int)
{
    FILE* output = nullptr;
    freopen_s(&output, "B03Validation.txt", "w", stdout);
    main();
    if (output)
    {
        fclose(output);
    }
}
#endif

#ifdef DEAD_SIGNAL_B101_VALIDATION
#if 0
int main()
{
    LONG failures = 0;
    failures += slashLocalAlertRangeSquared != 70.0f * 70.0f
        || listenerAlertRangeSquared != 80.0f * 80.0f;
    failures += ListenerTargetForRoomSize(1, 24) != 2
        || ListenerTargetForRoomSize(2, 24) != 2
        || ListenerTargetForRoomSize(3, 24) != 3
        || ListenerTargetForRoomSize(4, 24) != 4
        || ListenerTargetForRoomSize(5, 24) != 4
        || ListenerTargetForRoomSize(2, 1) != 1;
    failures += saveVersion != 8 || sizeof(SaveCheckpoint) != 32
        || metaVersion != 1 || sizeof(MetaProfile) != 16;

    EnemyRuntime slashEnemies[10]{};
    constexpr BYTE slashRoles[10]
        = { watcherEnemyRole, patrollerEnemyRole, hunterEnemyRole,
            listenerEnemyRole, watcherEnemyRole, spinnerEnemyRole,
            watcherEnemyRole, spinnerEnemyRole, pressureEnemyRole,
            patrollerEnemyRole };
    constexpr float slashX[10]
        = { 100.0f, 169.0f, 170.0f, 100.0f, 31.0f, 30.0f,
            171.0f, 29.0f, 100.0f, 100.0f };
    constexpr float slashY[10]
        = { 100.0f, 100.0f, 100.0f, 170.0f, 100.0f, 100.0f,
            100.0f, 100.0f, 100.0f, 100.0f };
    for (LONG index = 0; index < 10; ++index)
    {
        slashEnemies[index].alive = index != 9;
        slashEnemies[index].role = slashRoles[index];
        slashEnemies[index].x = slashX[index];
        slashEnemies[index].y = slashY[index];
    }
    alertEventCount = 0;
    alertEventActive = false;
    failures += EnterEnemyAlert(slashEnemies, 10, 0) != 1
        || !slashEnemies[0].alert;
    AlertEnemiesNear(slashEnemies, 10, 100.0f, 100.0f);
    for (LONG index = 0; index < 6; ++index)
    {
        failures += !slashEnemies[index].alert;
    }
    failures += slashEnemies[6].alert || slashEnemies[7].alert
        || slashEnemies[8].alert || slashEnemies[9].alert
        || alertEventCount != 1;

    EnemyRuntime relay[10]{};
    constexpr BYTE relayRoles[10]
        = { listenerEnemyRole, listenerEnemyRole, listenerEnemyRole,
            patrollerEnemyRole, hunterEnemyRole, watcherEnemyRole,
            spinnerEnemyRole, pressureEnemyRole, patrollerEnemyRole,
            hunterEnemyRole };
    constexpr float relayX[10]
        = { 0.0f, 79.0f, 158.0f, 0.0f, 158.0f,
            10.0f, 20.0f, 30.0f, 0.0f, 5.0f };
    constexpr float relayY[10]
        = { 0.0f, 0.0f, 0.0f, 80.0f, 80.0f,
            0.0f, 0.0f, 0.0f, 81.0f, 0.0f };
    for (LONG index = 0; index < 10; ++index)
    {
        relay[index].alive = index != 9;
        relay[index].role = relayRoles[index];
        relay[index].x = relayX[index];
        relay[index].y = relayY[index];
    }
    alertEventCount = 0;
    alertEventActive = false;
    LONG relayEntered = EnterEnemyAlert(relay, 10, 0);
    failures += relayEntered != 5 || !relay[0].alert || !relay[1].alert
        || !relay[2].alert || !relay[3].alert || !relay[4].alert
        || relay[5].alert || relay[6].alert || relay[7].alert
        || relay[8].alert || relay[9].alert;
    failures += EnterEnemyAlert(relay, 10, 1) != 0 || alertEventCount != 1;

    EnemyRuntime listenerBoundary[2]{};
    listenerBoundary[0].alive = listenerBoundary[1].alive = true;
    listenerBoundary[0].role = listenerBoundary[1].role = listenerEnemyRole;
    listenerBoundary[1].x = 81.0f;
    failures += EnterEnemyAlert(listenerBoundary, 2, 0) != 1
        || listenerBoundary[1].alert;
    listenerBoundary[0] = {};
    listenerBoundary[1] = {};
    listenerBoundary[0].alive = listenerBoundary[1].alive = true;
    listenerBoundary[0].role = listenerBoundary[1].role = listenerEnemyRole;
    listenerBoundary[1].x = 80.0f;
    failures += EnterEnemyAlert(listenerBoundary, 2, 0) != 2
        || !listenerBoundary[1].alert;

    for (LONG index = 0; index < 10; ++index)
    {
        relay[index].alert = false;
        relay[index].detectionProgress = 0.0f;
        relay[index].heardSuspicion = false;
    }
    relay[3].detectionProgress = 0.5f;
    UpdateAlertEventState(relay, 10);
    failures += !alertEventActive;
    relay[3].detectionProgress = 0.0f;
    UpdateAlertEventState(relay, 10);
    failures += alertEventActive;
    failures += EnterEnemyAlert(relay, 10, 5) != 1
        || alertEventCount != 2 || !alertEventActive;

    constexpr LONG expectedHP[characterCount]{ 100, 80, 70, 140, 80 };
    constexpr float expectedMoveSpeed[characterCount]
        { 60.0f, 72.0f, 57.0f, 45.0f, 66.0f };
    constexpr float expectedSlashCooldown[characterCount]
        { 0.5f, 0.5f, 0.6f, 0.75f, 0.25f };
    constexpr LONG expectedSlashReach[characterCount]{ 8, 7, 16, 6, 6 };
    constexpr LONG expectedSlashWidth[characterCount]{ 8, 7, 3, 12, 6 };
    constexpr LONG expectedSlashDamage[characterCount]{ 1, 1, 1, 2, 1 };
    constexpr LONG expectedSlashHitCap[characterCount]{ 3, 2, 1, 4, 2 };
    constexpr float expectedDashDistance[characterCount]
        { 32.0f, 35.2f, 32.0f, 28.8f, 32.0f };
    constexpr BYTE expectedCharacterDashCap[characterCount]{ 3, 3, 2, 2, 3 };
    constexpr LONG b07MaxHP[characterCount]{ 10, 9, 8, 12, 9 };

    for (BYTE character = 0; character < characterCount; ++character)
    {
        const CharacterProfile& profile = characterProfiles[character];
        failures += profile.maxHP != expectedHP[character]
            || fabsf(playerMoveSpeed * profile.moveScale
                - expectedMoveSpeed[character]) > 0.0001f
            || fabsf(slashCooldownDuration * profile.slashCooldownScale
                - expectedSlashCooldown[character]) > 0.0001f
            || profile.slashReach != expectedSlashReach[character]
            || profile.slashWidth != expectedSlashWidth[character]
            || profile.slashDamage != expectedSlashDamage[character]
            || profile.slashHitCap != expectedSlashHitCap[character]
            || fabsf(dashDistance * profile.dashDistanceScale
                - expectedDashDistance[character]) > 0.0001f
            || characterDashCaps[character] != expectedCharacterDashCap[character];

        selectedCharacter = character;
        moveUpgradeStack = 0;
        slashUpgradeStack = 0;
        dashUpgradeStack = 0;
        functionalUpgradeFlags = 0;
        float profileMove = 0.0f;
        float profileSlashCooldown = 0.0f;
        float profileDashCooldown = 0.0f;
        RecalculateAugmentStats(profileMove, profileSlashCooldown,
            profileDashCooldown);
        failures += fabsf(profileMove - expectedMoveSpeed[character]) > 0.0001f
            || fabsf(profileSlashCooldown - expectedSlashCooldown[character])
                > 0.0001f
            || profileDashCooldown != dashCooldownDuration
            || CurrentExecuteReach() != executeReach;
        printf("character=%u hp=%ld move=%.3f slash_cd=%.3f damage=%ld reach=%ld width=%ld target_cap=%ld dash=%.3f dash_cap=%u\n",
            character, profile.maxHP, profileMove, profileSlashCooldown,
            profile.slashDamage, profile.slashReach, profile.slashWidth,
            profile.slashHitCap, dashDistance * profile.dashDistanceScale,
            characterDashCaps[character]);
    }

    const CharacterProfile& basicProfile = characterProfiles[basicCharacter];
    const CharacterProfile& mobilityProfile = characterProfiles[mobilityCharacter];
    const CharacterProfile& piercerProfile = characterProfiles[piercerCharacter];
    const CharacterProfile& heavyProfile = characterProfiles[heavyCharacter];
    const CharacterProfile& rapidProfile = characterProfiles[rapidCharacter];
    failures += !(mobilityProfile.moveScale > basicProfile.moveScale
            && mobilityProfile.slashCooldownScale == basicProfile.slashCooldownScale
            && mobilityProfile.dashDistanceScale > basicProfile.dashDistanceScale
            && mobilityProfile.maxHP < basicProfile.maxHP
            && mobilityProfile.slashReach < basicProfile.slashReach
            && mobilityProfile.slashWidth < basicProfile.slashWidth
            && mobilityProfile.slashHitCap < basicProfile.slashHitCap);
    failures += !(piercerProfile.moveScale < basicProfile.moveScale
            && piercerProfile.slashReach > basicProfile.slashReach
            && piercerProfile.maxHP < basicProfile.maxHP
            && piercerProfile.slashCooldownScale > basicProfile.slashCooldownScale
            && piercerProfile.slashWidth < basicProfile.slashWidth
            && piercerProfile.slashHitCap < basicProfile.slashHitCap);
    failures += !(heavyProfile.maxHP > basicProfile.maxHP
            && heavyProfile.slashDamage > basicProfile.slashDamage
            && heavyProfile.slashHitCap > basicProfile.slashHitCap
            && heavyProfile.moveScale < basicProfile.moveScale
            && heavyProfile.slashCooldownScale > basicProfile.slashCooldownScale
            && heavyProfile.slashReach < basicProfile.slashReach
            && heavyProfile.slashWidth > basicProfile.slashWidth
            && heavyProfile.dashDistanceScale < basicProfile.dashDistanceScale);
    failures += !(rapidProfile.moveScale > basicProfile.moveScale
            && rapidProfile.slashCooldownScale < basicProfile.slashCooldownScale
            && rapidProfile.maxHP < basicProfile.maxHP
            && rapidProfile.slashReach < basicProfile.slashReach
            && rapidProfile.slashWidth < basicProfile.slashWidth
            && rapidProfile.slashHitCap < basicProfile.slashHitCap);

    constexpr LONG cardinalX[4]{ 0, 0, -1, 1 };
    constexpr LONG cardinalY[4]{ -1, 1, 0, 0 };
    for (BYTE character = 0; character < characterCount; ++character)
    {
        const CharacterProfile& profile = characterProfiles[character];
        for (LONG direction = 0; direction < 4; ++direction)
        {
            if (character == piercerCharacter)
            {
                LONG outlineLeft = 1000;
                LONG outlineTop = 1000;
                LONG outlineRight = -1000;
                LONG outlineBottom = -1000;
                for (LONG y = 70; y < 130; ++y)
                {
                    for (LONG x = 70; x < 130; ++x)
                    {
                        if (PiercerSlashOutlinePixel(100.0f, 100.0f,
                            cardinalX[direction], cardinalY[direction],
                            x + 0.5f, y + 0.5f))
                        {
                            if (x < outlineLeft)
                            {
                                outlineLeft = x;
                            }
                            if (x > outlineRight)
                            {
                                outlineRight = x;
                            }
                            if (y < outlineTop)
                            {
                                outlineTop = y;
                            }
                            if (y > outlineBottom)
                            {
                                outlineBottom = y;
                            }
                        }
                    }
                }
                LONG expectedX = cardinalX[direction]
                    ? profile.slashReach : profile.slashWidth;
                LONG expectedY = cardinalY[direction]
                    ? profile.slashReach : profile.slashWidth;
                failures += outlineRight - outlineLeft + 1 != expectedX
                    || outlineBottom - outlineTop + 1 != expectedY;
            }
            else
            {
                LONG shapeWidth = cardinalX[direction]
                    ? profile.slashReach : profile.slashWidth;
                LONG shapeHeight = cardinalY[direction]
                    ? profile.slashReach : profile.slashWidth;
                failures += shapeWidth != (cardinalX[direction]
                        ? expectedSlashReach[character]
                        : expectedSlashWidth[character])
                    || shapeHeight != (cardinalY[direction]
                        ? expectedSlashReach[character]
                        : expectedSlashWidth[character]);
            }
        }
    }

    for (BYTE character = 0; character < characterCount; ++character)
    {
        selectedCharacter = character;
        const CharacterProfile& profile = characterProfiles[character];
        EnemyRuntime hitEnemies[maxEnemyCount]{};
        for (LONG enemyIndex = 0; enemyIndex < maxEnemyCount; ++enemyIndex)
        {
            hitEnemies[enemyIndex].alive = enemyIndex != 0;
            hitEnemies[enemyIndex].role
                = static_cast<BYTE>(enemyIndex % enemyRoleCount);
            hitEnemies[enemyIndex].x = 108.0f;
            hitEnemies[enemyIndex].y = 100.0f;
            hitEnemies[enemyIndex].hp = 3;
        }
        LONG hitCount = 0;
        LONG slashLeftTest = 100 + playerWidth / 2;
        LONG slashRightTest = slashLeftTest + profile.slashReach;
        LONG slashTopTest = 100 - profile.slashWidth / 2;
        LONG slashBottomTest = slashTopTest + profile.slashWidth;
        for (LONG enemyIndex = 0; enemyIndex < maxEnemyCount; ++enemyIndex)
        {
            EnemyRuntime& enemy = hitEnemies[enemyIndex];
            LONG enemyLeft = static_cast<LONG>(enemy.x) - enemyWidth / 2;
            LONG enemyTop = static_cast<LONG>(enemy.y) - enemyHeight / 2;
            bool overlap = character == piercerCharacter
                ? PiercerSlashHitsEnemy(100.0f, 100.0f, 1, 0,
                    enemy.x, enemy.y)
                : slashLeftTest < enemyLeft + enemyWidth
                    && slashRightTest > enemyLeft
                    && slashTopTest < enemyTop + enemyHeight
                    && slashBottomTest > enemyTop;
            if (enemy.alive && overlap && hitCount < profile.slashHitCap)
            {
                ++hitCount;
                enemy.hp -= profile.slashDamage;
            }
        }
        failures += hitCount != profile.slashHitCap || hitEnemies[0].hp != 3;
        for (LONG enemyIndex = 1; enemyIndex < maxEnemyCount; ++enemyIndex)
        {
            LONG expectedHPAfterHit = enemyIndex <= profile.slashHitCap
                ? 3 - profile.slashDamage : 3;
            failures += hitEnemies[enemyIndex].hp != expectedHPAfterHit;
        }
        EnemyRuntime pressureTarget{};
        pressureTarget.alive = true;
        pressureTarget.role = pressureEnemyRole;
        pressureTarget.hp = 3;
        failures += pressureTarget.hp != 3;
    }

    failures += !PiercerSlashHitsEnemy(100.0f, 100.0f, 1, 0,
            123.0f, 100.0f)
        || PiercerSlashHitsEnemy(100.0f, 100.0f, 1, 0,
            124.0f, 100.0f)
        || PiercerSlashHitsEnemy(100.0f, 100.0f, 1, 0,
            110.0f, 108.0f)
        || !PiercerSlashHitsEnemy(100.0f, 100.0f, 1, 1,
            110.0f, 110.0f)
        || !PiercerSlashOutlinePixel(100.0f, 100.0f, 1, 0,
            104.5f, 99.5f)
        || PiercerSlashOutlinePixel(100.0f, 100.0f, 1, 0,
            120.5f, 100.5f)
        || PiercerSlashOutlinePixel(100.0f, 100.0f, 1, 0,
            110.5f, 98.5f);

    moveUpgradeStack = 1;
    slashUpgradeStack = 0;
    dashUpgradeStack = 0;
    functionalUpgradeFlags = 0;
    selectedCharacter = mobilityCharacter;
    float combinationMove = 0.0f;
    float combinationSlash = 0.0f;
    float combinationDash = 0.0f;
    RecalculateAugmentStats(combinationMove, combinationSlash,
        combinationDash);
    failures += fabsf(combinationMove - 79.2f) > 0.0001f
        || fabsf(combinationSlash - 0.5f) > 0.0001f;
    selectedCharacter = piercerCharacter;
    moveUpgradeStack = 1;
    functionalUpgradeFlags = executeReachUpgradeFlag;
    RecalculateAugmentStats(combinationMove, combinationSlash,
        combinationDash);
    failures += fabsf(combinationMove - 62.7f) > 0.0001f
        || fabsf(combinationSlash - 0.6f) > 0.0001f
        || CurrentExecuteReach() != 8
        || characterProfiles[selectedCharacter].slashReach != 16;
    selectedCharacter = heavyCharacter;
    moveUpgradeStack = 0;
    slashUpgradeStack = 1;
    functionalUpgradeFlags = 0;
    RecalculateAugmentStats(combinationMove, combinationSlash,
        combinationDash);
    failures += fabsf(combinationMove - 45.0f) > 0.0001f
        || fabsf(combinationSlash - 0.6f) > 0.0001f;
    selectedCharacter = rapidCharacter;
    slashUpgradeStack = 1;
    dashUpgradeStack = 1;
    functionalUpgradeFlags = silentDashUpgradeFlag;
    RecalculateAugmentStats(combinationMove, combinationSlash,
        combinationDash);
    failures += fabsf(combinationSlash - 0.2f) > 0.0001f
        || fabsf(combinationDash - 0.8f) > 0.0001f
        || PlayerMovementIsAudible(true, true)
        || !PlayerMovementIsAudible(true, false);
    slashUpgradeStack = 2;
    RecalculateAugmentStats(combinationMove, combinationSlash,
        combinationDash);
    failures += fabsf(combinationSlash - 0.15f) > 0.0001f
        || combinationSlash <= 0.0f;
    selectedCharacter = heavyCharacter;
    functionalUpgradeFlags = fieldMedicUpgradeFlag;
    LONG medicHP = 13;
    ApplyFieldMedic(medicHP);
    ApplyFieldMedic(medicHP);
    failures += medicHP != heavyProfile.maxHP;

    constexpr BYTE expectedDashCapacity[characterCount][3]
    {
        { 1, 2, 3 },
        { 1, 2, 3 },
        { 1, 2, 2 },
        { 1, 2, 2 },
        { 1, 2, 3 }
    };
    for (BYTE character = 0; character < characterCount; ++character)
    {
        selectedCharacter = character;
        for (BYTE growth = 0; growth <= maximumDashGrowthLevel; ++growth)
        {
            dashGrowthLevel = growth;
            failures += CurrentDashCapacity()
                != expectedDashCapacity[character][growth];
        }
    }
    selectedCharacter = basicCharacter;
    dashGrowthLevel = maximumDashGrowthLevel;
    currentDashMaxCharges = CurrentDashCapacity();
    currentDashCharges = currentDashMaxCharges;
    float recharge = 0.0f;
    failures += !ConsumeDashCharge(1.0f, recharge)
        || !ConsumeDashCharge(1.0f, recharge)
        || !ConsumeDashCharge(1.0f, recharge)
        || ConsumeDashCharge(1.0f, recharge) || currentDashCharges != 0;
    UpdateDashRecharge(1.0f, 1.0f, recharge);
    failures += currentDashCharges != 1 || recharge != 1.0f;
    UpdateDashRecharge(1.0f, 1.0f, recharge);
    failures += currentDashCharges != 2 || recharge != 1.0f;
    UpdateDashRecharge(1.0f, 1.0f, recharge);
    failures += currentDashCharges != 3 || recharge != 0.0f;

    for (BYTE character = 0; character < characterCount; ++character)
    {
        runSeed = 12345 + character;
        currentRoom = 10;
        moveUpgradeStack = 1;
        slashUpgradeStack = 1;
        dashUpgradeStack = 1;
        functionalUpgradeFlags = executeReachUpgradeFlag
            | fieldMedicUpgradeFlag;
        rerollUsed = true;
        selectedCharacter = character;
        dashGrowthLevel = maximumDashGrowthLevel;
        currentDashMaxCharges = CurrentDashCapacity();
        currentDashCharges = currentDashMaxCharges;
        alertEventCount = 7;
        WriteCheckpoint(runSeed, currentRoom,
            characterProfiles[character].maxHP - 1, 11);
        SaveCheckpoint checkpoint{};
        failures += !ReadCheckpoint(&checkpoint)
            || checkpoint.character != character
            || checkpoint.playerHP != characterProfiles[character].maxHP - 1
            || checkpoint.alertEvents != 7
            || checkpoint.dashCharges != currentDashMaxCharges
            || checkpoint.runKills != 11;

        WriteCheckpoint(runSeed, currentRoom, b07MaxHP[character], 11);
        checkpoint = {};
        failures += !ReadCheckpoint(&checkpoint);
        LONG compatibleHP = checkpoint.playerHP
            < characterProfiles[character].maxHP ? checkpoint.playerHP
            : characterProfiles[character].maxHP;
        failures += compatibleHP != (b07MaxHP[character]
                < expectedHP[character] ? b07MaxHP[character]
                : expectedHP[character]);

        selectedCharacter = checkpoint.character;
        moveUpgradeStack = checkpoint.moveStack;
        slashUpgradeStack = checkpoint.slashStack;
        dashUpgradeStack = checkpoint.dashStack;
        functionalUpgradeFlags = checkpoint.functionalFlags;
        float continuedMove = 0.0f;
        float continuedSlash = 0.0f;
        float continuedDash = 0.0f;
        RecalculateAugmentStats(continuedMove, continuedSlash, continuedDash);
        failures += fabsf(continuedMove
                - expectedMoveSpeed[character] * 1.1f) > 0.0001f
            || fabsf(continuedSlash
                - expectedSlashCooldown[character] * 0.8f) > 0.0001f
            || fabsf(continuedDash - 0.8f) > 0.0001f
            || characterProfiles[selectedCharacter].maxHP
                != expectedHP[character];

        moveUpgradeStack = 0;
        slashUpgradeStack = 0;
        dashUpgradeStack = 0;
        functionalUpgradeFlags = 0;
        RecalculateAugmentStats(continuedMove, continuedSlash, continuedDash);
        failures += selectedCharacter != character
            || fabsf(continuedMove - expectedMoveSpeed[character]) > 0.0001f
            || fabsf(continuedSlash - expectedSlashCooldown[character])
                > 0.0001f;
        DeleteFileW(saveFileName);
    }

    globalCoin = 0;
    runKillCount = 4;
    alertEventCount = 0;
    runRewardGranted = false;
    GrantRunCoin(false);
    GrantRunCoin(false);
    failures += globalCoin != 8;
    globalCoin = 0;
    alertEventCount = 1;
    runRewardGranted = false;
    GrantRunCoin(false);
    failures += globalCoin != 4;
    globalCoin = 0;
    runKillCount = 5;
    alertEventCount = 0;
    runRewardGranted = false;
    GrantRunCoin(true);
    failures += globalCoin != 16;

#ifdef DEAD_SIGNAL_B08_SOAK_VALIDATION
    EnemyRuntime soakEnemies[maxEnemyCount]{};
    ULONGLONG soakStart = GetTickCount64();
    ULONGLONG soakUpdates = 0;
    LONG soakFailures = 0;
    while (GetTickCount64() - soakStart < 90000)
    {
        for (LONG index = 0; index < maxEnemyCount; ++index)
        {
            EnemyRuntime& enemy = soakEnemies[index];
            enemy = {};
            enemy.alive = true;
            enemy.role = index < 4 ? listenerEnemyRole
                : static_cast<BYTE>(index % enemyRoleCount);
            enemy.x = index < 4 ? 10.0f + index * 79.0f
                : 100.0f + static_cast<float>(index * 3);
            enemy.y = 100.0f;
        }
        soakEnemies[maxEnemyCount - 1].role = pressureEnemyRole;
        soakEnemies[maxEnemyCount - 1].x = 150.0f;
        alertEventCount = 0;
        alertEventActive = false;
        LONG entered = EnterEnemyAlert(soakEnemies, maxEnemyCount, 0);
        soakFailures += entered < 4 || !soakEnemies[3].alert
            || EnterEnemyAlert(soakEnemies, maxEnemyCount, 1) != 0;
        AlertEnemiesNear(soakEnemies, maxEnemyCount, 150.0f, 100.0f);
        soakFailures += soakEnemies[maxEnemyCount - 1].alert
            || alertEventCount != 1;
        LONG soakDamage = 0;
        EnemyRuntime& attacker = soakEnemies[4];
        attacker.alert = true;
        attacker.x = 100.0f;
        attacker.y = 100.0f;
        for (LONG update = 0; update < 20; ++update)
        {
            UpdateEnemyAttack(attacker, true, 108.0f, 100.0f, 0.016f,
                soakDamage);
        }
        soakFailures += soakDamage != 1
            || !PointInsideVisionSector(100.0f, 100.0f, 1.0f, 0.0f,
                170.0f, 100.0f)
            || PointInsideVisionSector(100.0f, 100.0f, 1.0f, 0.0f,
                171.0f, 100.0f);
        selectedCharacter = static_cast<BYTE>(soakUpdates % characterCount);
        moveUpgradeStack = static_cast<BYTE>(soakUpdates % 3);
        slashUpgradeStack = static_cast<BYTE>((soakUpdates / 3) % 3);
        dashUpgradeStack = static_cast<BYTE>((soakUpdates / 9) % 3);
        functionalUpgradeFlags = (soakUpdates & 1) ? silentDashUpgradeFlag : 0;
        float soakMove = 0.0f;
        float soakSlash = 0.0f;
        float soakDash = 0.0f;
        RecalculateAugmentStats(soakMove, soakSlash, soakDash);
        const CharacterProfile& soakProfile = characterProfiles[selectedCharacter];
        soakFailures += fabsf(soakMove - playerMoveSpeed * soakProfile.moveScale
                * (1.0f + moveUpgradeStack * 0.1f)) > 0.0001f
            || fabsf(soakSlash - slashCooldownDuration
                * soakProfile.slashCooldownScale
                * (1.0f - slashUpgradeStack * 0.2f)) > 0.0001f
            || soakProfile.slashHitCap < 1 || soakProfile.slashHitCap > 4;
        dashGrowthLevel = static_cast<BYTE>((soakUpdates / 27) % 3);
        currentDashMaxCharges = CurrentDashCapacity();
        currentDashCharges = currentDashMaxCharges;
        float soakRecharge = 0.0f;
        soakFailures += !ConsumeDashCharge(1.0f, soakRecharge);
        UpdateDashRecharge(1.0f, 1.0f, soakRecharge);
        soakFailures += currentDashCharges != currentDashMaxCharges;
        for (LONG index = 0; index < maxEnemyCount; ++index)
        {
            soakEnemies[index].alert = false;
            soakEnemies[index].detectionProgress = 0.0f;
            soakEnemies[index].heardSuspicion = false;
        }
        UpdateAlertEventState(soakEnemies, maxEnemyCount);
        soakFailures += alertEventActive;
        ++soakUpdates;
    }
    failures += soakFailures;
    printf("soak_ms=%llu soak_updates=%llu soak_failures=%ld\n",
        GetTickCount64() - soakStart, soakUpdates, soakFailures);
#endif

    DeleteFileW(metaFileName);
    printf("b08_failures=%ld slash70=%d relay80=%ld events=%u dash=%u save=%lu meta=%lu\n",
        failures, slashEnemies[5].alert, relayEntered, alertEventCount,
        currentDashCharges, static_cast<unsigned long>(sizeof(SaveCheckpoint)),
        static_cast<unsigned long>(sizeof(MetaProfile)));
    return failures;
}

extern "C" __declspec(dllexport) void CALLBACK RunB08Validation(HWND, HINSTANCE,
    LPSTR, int)
{
    FILE* output = nullptr;
    freopen_s(&output, "B08Validation.txt", "w", stdout);
    main();
    if (output)
    {
        fclose(output);
    }
}
#endif

int main()
{
    LONG failures = 0;
    DeleteFileW(metaFileName);
    DeleteFileW(saveFileName);
    ResetMetaProfile();

    failures += slashLocalAlertRangeSquared != 70.0f * 70.0f
        || listenerAlertRangeSquared != 80.0f * 80.0f
        || enemyVisionRange != 70 || enemyAttackWindupDuration != 0.3f
        || playerInvulnerabilityDuration != 0.5f
        || hunterAlertSpeed != 62.0f || pressureMoveSpeed != 32.0f;
    failures += saveVersion != 8 || sizeof(SaveCheckpoint) != 32
        || metaVersion != 2 || sizeof(LegacyMetaProfile) != 16
        || sizeof(MetaProfile) != 32;

    constexpr LONG expectedHP[characterCount]{ 100, 80, 70, 140, 80 };
    constexpr float expectedMove[characterCount]{ 60.0f, 68.0f, 57.0f, 45.0f, 66.0f };
    constexpr float expectedCooldown[characterCount]
        { 0.5f, 0.5f, 0.6f, 0.75f, 0.25f };
    constexpr LONG expectedReach[characterCount]{ 8, 7, 16, 6, 6 };
    constexpr LONG expectedWidth[characterCount]{ 8, 7, 3, 12, 6 };
    constexpr LONG expectedDamage[characterCount]{ 10, 10, 15, 20, 10 };
    constexpr LONG expectedCap[characterCount]{ 3, 2, 1, 4, 2 };
    constexpr float expectedDash[characterCount]{ 32.0f, 35.2f, 32.0f, 28.8f, 32.0f };
    for (BYTE character = 0; character < characterCount; ++character)
    {
        const CharacterProfile& profile = characterProfiles[character];
        failures += profile.maxHP != expectedHP[character]
            || fabsf(CharacterMoveSpeed(character) - expectedMove[character]) > 0.0001f
            || fabsf(CharacterSlashCooldown(character) - expectedCooldown[character]) > 0.0001f
            || CharacterSlashReach(character) != expectedReach[character]
            || CharacterSlashWidth(character) != expectedWidth[character]
            || profile.slashDamage != expectedDamage[character]
            || CharacterSlashHitCap(character) != expectedCap[character]
            || fabsf(CharacterDashDistance(character) - expectedDash[character]) > 0.0001f
            || CharacterDashCapacity(character) != 1;
    }
    printf("section_base=%ld\n", failures);

    constexpr LONG expectedKillHits[characterCount]{ 3, 3, 2, 2, 3 };
    constexpr float expectedKillTime[characterCount]
        { 1.0f, 1.0f, 0.6f, 0.75f, 0.5f };
    for (BYTE character = 0; character < characterCount; ++character)
    {
        LONG hp = enemyMaximumHP;
        LONG hits = 0;
        while (hp > 0)
        {
            hp -= characterProfiles[character].slashDamage;
            ++hits;
        }
        failures += hits != expectedKillHits[character]
            || fabsf((hits - 1) * CharacterSlashCooldown(character)
                - expectedKillTime[character]) > 0.0001f;
    }
    failures += combatScale != 10 || enemyMaximumHP != 30
        || playerDamageAmount != 10 || healAmount != 10
        || RoundCombatRatio(164, 1, 10) != 16
        || RoundCombatRatio(165, 1, 10) != 17
        || RoundCombatRatio(166, 1, 10) != 17
        || dashCooldownDuration != 1.5f;
    selectedCharacter = basicCharacter;
    for (BYTE stack = 0; stack < 3; ++stack)
    {
        dashUpgradeStack = stack;
        float dashMove = 0.0f;
        float dashSlash = 0.0f;
        float dashRecharge = 0.0f;
        RecalculateAugmentStats(dashMove, dashSlash, dashRecharge);
        failures += fabsf(dashRecharge - (1.5f - stack * 0.3f)) > 0.0001f
            || dashRecharge <= 0.0f;
    }
    currentDashMaxCharges = 4;
    currentDashCharges = 0;
    float validationRecharge = dashCooldownDuration;
    UpdateDashRecharge(1.49f, dashCooldownDuration, validationRecharge);
    failures += currentDashCharges != 0;
    UpdateDashRecharge(0.02f, dashCooldownDuration, validationRecharge);
    failures += currentDashCharges != 1
        || fabsf(validationRecharge - dashCooldownDuration) > 0.0001f;
    for (LONG charge = 1; charge < 4; ++charge)
    {
        UpdateDashRecharge(dashCooldownDuration, dashCooldownDuration,
            validationRecharge);
    }
    failures += currentDashCharges != 4 || validationRecharge != 0.0f;
    dashUpgradeStack = 0;
    printf("section_damage_dash=%ld\n", failures);

    constexpr LONG cardinalX[4]{ 0, 0, -1, 1 };
    constexpr LONG cardinalY[4]{ -1, 1, 0, 0 };
    for (BYTE character = 0; character < characterCount; ++character)
    {
        for (LONG direction = 0; direction < 4; ++direction)
        {
            LONG geometryX = cardinalX[direction]
                ? CharacterSlashReach(character) : CharacterSlashWidth(character);
            LONG geometryY = cardinalY[direction]
                ? CharacterSlashReach(character) : CharacterSlashWidth(character);
            failures += geometryX != (cardinalX[direction]
                    ? expectedReach[character] : expectedWidth[character])
                || geometryY != (cardinalY[direction]
                    ? expectedReach[character] : expectedWidth[character]);
        }
    }
    for (LONG direction = 0; direction < 4; ++direction)
    {
        LONG outlineLeft = 1000;
        LONG outlineTop = 1000;
        LONG outlineRight = -1000;
        LONG outlineBottom = -1000;
        for (LONG y = 70; y < 130; ++y)
        {
            for (LONG x = 70; x < 130; ++x)
            {
                if (PiercerSlashOutlinePixel(100.0f, 100.0f,
                    cardinalX[direction], cardinalY[direction],
                    x + 0.5f, y + 0.5f))
                {
                    if (x < outlineLeft) outlineLeft = x;
                    if (x > outlineRight) outlineRight = x;
                    if (y < outlineTop) outlineTop = y;
                    if (y > outlineBottom) outlineBottom = y;
                }
            }
        }
        failures += outlineRight - outlineLeft + 1
                != (cardinalX[direction] ? 16 : 3)
            || outlineBottom - outlineTop + 1
                != (cardinalY[direction] ? 16 : 3);
    }
    failures += !PiercerSlashHitsEnemy(100.0f, 100.0f, 1, 0,
            123.9f, 100.0f)
        || PiercerSlashHitsEnemy(100.0f, 100.0f, 1, 0,
            124.0f, 100.0f)
        || !PiercerSlashHitsEnemy(100.0f, 100.0f, 1, 0,
            110.0f, 92.5f)
        || PiercerSlashHitsEnemy(100.0f, 100.0f, 1, 0,
            110.0f, 107.5f);
    printf("section_geometry=%ld\n", failures);

    characterGlobalLevel[basicCharacter][0] = 2;
    characterGlobalLevel[basicCharacter][1] = 2;
    characterGlobalLevel[basicCharacter][2] = 2;
    failures += CharacterDashCapacity(basicCharacter) != 3
        || CharacterMaximumHP(basicCharacter) != 120
        || CharacterSlashReach(basicCharacter) != 9
        || CharacterSlashWidth(basicCharacter) != 9
        || CharacterDashCapacity(rapidCharacter) != 1;
    characterGlobalLevel[mobilityCharacter][0] = 3;
    characterGlobalLevel[mobilityCharacter][1] = 2;
    characterGlobalLevel[mobilityCharacter][2] = 2;
    failures += CharacterDashCapacity(mobilityCharacter) != 4
        || fabsf(CharacterMoveSpeed(mobilityCharacter) - 74.8f) > 0.0001f
        || fabsf(CharacterDashDistance(mobilityCharacter) - 42.24f) > 0.0001f
        || CharacterMaximumHP(mobilityCharacter) != 80;
    characterGlobalLevel[piercerCharacter][0] = 1;
    characterGlobalLevel[piercerCharacter][1] = 2;
    characterGlobalLevel[piercerCharacter][2] = 1;
    failures += CharacterDashCapacity(piercerCharacter) != 2
        || CharacterSlashReach(piercerCharacter) != 20
        || CharacterSlashWidth(piercerCharacter) != 3
        || CharacterSlashHitCap(piercerCharacter) != 2
        || fabsf(CharacterMoveSpeed(piercerCharacter) - 57.0f) > 0.0001f;
    characterGlobalLevel[heavyCharacter][0] = 2;
    characterGlobalLevel[heavyCharacter][1] = 2;
    characterGlobalLevel[heavyCharacter][2] = 2;
    failures += CharacterDashCapacity(heavyCharacter) != 1
        || CharacterMaximumHP(heavyCharacter) != 180
        || CharacterSlashReach(heavyCharacter) != 6
        || CharacterSlashWidth(heavyCharacter) != 16
        || fabsf(CharacterMoveSpeed(heavyCharacter) - 49.5f) > 0.0001f;
    characterGlobalLevel[rapidCharacter][0] = 1;
    characterGlobalLevel[rapidCharacter][1] = 2;
    characterGlobalLevel[rapidCharacter][2] = 2;
    failures += CharacterDashCapacity(rapidCharacter) != 2
        || fabsf(CharacterSlashCooldown(rapidCharacter) - 0.2f) > 0.0001f
        || fabsf(CharacterMoveSpeed(rapidCharacter) - 72.6f) > 0.0001f;

    selectedCharacter = rapidCharacter;
    moveUpgradeStack = 2;
    slashUpgradeStack = 2;
    dashUpgradeStack = 2;
    float runMove = 0.0f;
    float runSlash = 0.0f;
    float runDash = 0.0f;
    RecalculateAugmentStats(runMove, runSlash, runDash);
    failures += fabsf(runMove - 87.12f) > 0.0002f
        || fabsf(runSlash - 0.12f) > 0.0001f || runSlash <= 0.0f
        || fabsf(runDash - 0.9f) > 0.0001f;
    LONG recoveryHP = 70;
    commonGlobalLevel[fieldRecoveryGlobalUpgrade] = 1;
    ApplyFieldRecovery(recoveryHP, 90);
    ApplyFieldRecovery(recoveryHP, 80);
    failures += recoveryHP != 80;
    LONG recoveryClampHP = 75;
    ApplyFieldRecovery(recoveryClampHP, 80);
    failures += recoveryClampHP != 80;
    selectedCharacter = basicCharacter;
    functionalUpgradeFlags = fieldMedicUpgradeFlag;
    LONG medicHP = 100;
    ApplyFieldMedic(medicHP);
    ApplyFieldMedic(medicHP);
    ApplyFieldMedic(medicHP);
    failures += medicHP != 120;
    medicHP = 115;
    ApplyFieldMedic(medicHP);
    failures += medicHP != 120;
    LONG highestDamage = 0;
    QueuePlayerDamage(10, highestDamage);
    QueuePlayerDamage(20, highestDamage);
    QueuePlayerDamage(10, highestDamage);
    LONG damageTestHP = 30 - highestDamage;
    failures += highestDamage != 20 || damageTestHP != 10
        || 70 * 12 / 100 != 7 * 12 / 10;
    commonGlobalLevel[runRerollGlobalUpgrade] = 0;
    failures += CurrentRunRerollCapacity() != 1;
    commonGlobalLevel[runRerollGlobalUpgrade] = 1;
    failures += CurrentRunRerollCapacity() != 2;

    ResetMetaProfile();
    characterGlobalLevel[basicCharacter][0] = 2;
    characterGlobalLevel[basicCharacter][1] = 2;
    characterGlobalLevel[basicCharacter][2] = 2;
    failures += CharacterMoveSpeed(mobilityCharacter) != 68.0f
        || CharacterMaximumHP(mobilityCharacter) != 80
        || CharacterDashCapacity(mobilityCharacter) != 1;
    characterGlobalLevel[mobilityCharacter][0] = 3;
    characterGlobalLevel[mobilityCharacter][1] = 2;
    characterGlobalLevel[mobilityCharacter][2] = 2;
    failures += CharacterMoveSpeed(rapidCharacter) != 66.0f
        || CharacterSlashCooldown(rapidCharacter) != 0.25f
        || CharacterDashCapacity(rapidCharacter) != 1;
    characterGlobalLevel[piercerCharacter][1] = 2;
    characterGlobalLevel[piercerCharacter][2] = 1;
    characterGlobalLevel[heavyCharacter][0] = 2;
    characterGlobalLevel[heavyCharacter][1] = 2;
    characterGlobalLevel[heavyCharacter][2] = 2;
    characterGlobalLevel[rapidCharacter][1] = 2;
    characterGlobalLevel[rapidCharacter][2] = 2;
    selectedCharacter = mobilityCharacter;
    moveUpgradeStack = 1;
    slashUpgradeStack = 0;
    dashUpgradeStack = 0;
    functionalUpgradeFlags = 0;
    RecalculateAugmentStats(runMove, runSlash, runDash);
    failures += fabsf(runMove - 82.28f) > 0.0002f
        || fabsf(runSlash - 0.5f) > 0.0001f
        || fabsf(CharacterDashDistance(mobilityCharacter) - 42.24f) > 0.0001f;
    selectedCharacter = piercerCharacter;
    moveUpgradeStack = 0;
    functionalUpgradeFlags = executeReachUpgradeFlag;
    RecalculateAugmentStats(runMove, runSlash, runDash);
    failures += CharacterSlashReach(piercerCharacter) != 20
        || CharacterSlashWidth(piercerCharacter) != 3
        || CurrentExecuteReach() != 8;
    selectedCharacter = heavyCharacter;
    slashUpgradeStack = 1;
    functionalUpgradeFlags = 0;
    RecalculateAugmentStats(runMove, runSlash, runDash);
    failures += CharacterSlashReach(heavyCharacter) != 6
        || CharacterSlashWidth(heavyCharacter) != 16
        || fabsf(runSlash - 0.6f) > 0.0001f
        || CharacterDashCapacity(heavyCharacter) != 1;
    selectedCharacter = rapidCharacter;
    slashUpgradeStack = 2;
    RecalculateAugmentStats(runMove, runSlash, runDash);
    failures += fabsf(runSlash - 0.12f) > 0.0001f || runSlash <= 0.0f;
    selectedCharacter = basicCharacter;
    slashUpgradeStack = 0;
    failures += CharacterSlashReach(basicCharacter) != 9
        || CharacterSlashWidth(basicCharacter) != 9;
    printf("section_growth=%ld\n", failures);

    ResetMetaProfile();
    globalCoin = 123;
    unlockedCharacterMask = 0x1f;
    audioEnabled = false;
    commonGlobalLevel[0] = 1;
    commonGlobalLevel[1] = 1;
    commonGlobalLevel[2] = 2;
    for (BYTE character = 0; character < characterCount; ++character)
    {
        for (BYTE upgrade = 0; upgrade < characterGlobalUpgradeCount; ++upgrade)
        {
            characterGlobalLevel[character][upgrade]
                = characterGlobalMaximum[character][upgrade];
        }
    }
    WriteMetaProfile();
    ResetMetaProfile();
    failures += !ReadMetaProfile() || globalCoin != 123
        || unlockedCharacterMask != 0x1f || audioEnabled
        || commonGlobalLevel[2] != 2
        || characterGlobalLevel[mobilityCharacter][0] != 3
        || characterGlobalLevel[heavyCharacter][0] != 2
        || characterGlobalLevel[piercerCharacter][2] != 1;
    printf("section_meta_v2=%ld\n", failures);

    MetaProfile b09Profile{};
    b09Profile.magic = metaMagic;
    b09Profile.version = metaVersion;
    b09Profile.coin = 44;
    b09Profile.unlockedMask = 0x1f;
    b09Profile.audioSetting = 1;
    b09Profile.characterLevel[piercerCharacter][2] = 2;
    HANDLE b09File = CreateFileW(metaFileName, GENERIC_WRITE, 0, nullptr,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    DWORD b09Transferred = 0;
    if (b09File != INVALID_HANDLE_VALUE)
    {
        WriteFile(b09File, &b09Profile, sizeof(b09Profile), &b09Transferred, nullptr);
        CloseHandle(b09File);
    }
    ResetMetaProfile();
    failures += b09File == INVALID_HANDLE_VALUE
        || b09Transferred != sizeof(b09Profile) || !ReadMetaProfile()
        || globalCoin != 44 || characterGlobalLevel[piercerCharacter][2] != 0;
    MetaProfile rewrittenProfile{};
    b09File = CreateFileW(metaFileName, GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    b09Transferred = 0;
    if (b09File != INVALID_HANDLE_VALUE)
    {
        ReadFile(b09File, &rewrittenProfile, sizeof(rewrittenProfile),
            &b09Transferred, nullptr);
        CloseHandle(b09File);
    }
    failures += b09Transferred != sizeof(rewrittenProfile)
        || rewrittenProfile.characterLevel[piercerCharacter][2]
            != pierceThroughStorageMarker;
    printf("section_b09_slot_migration=%ld\n", failures);

    LegacyMetaProfile legacy
        { metaMagic, 1, 77, 0x0b, 2, 2, 0 };
    HANDLE file = CreateFileW(metaFileName, GENERIC_WRITE, 0, nullptr,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    DWORD transferred = 0;
    failures += file == INVALID_HANDLE_VALUE;
    if (file != INVALID_HANDLE_VALUE)
    {
        failures += !WriteFile(file, &legacy, sizeof(legacy), &transferred, nullptr)
            || transferred != sizeof(legacy);
        CloseHandle(file);
    }
    ResetMetaProfile();
    bool migrated = ReadMetaProfile();
    file = CreateFileW(metaFileName, GENERIC_READ, FILE_SHARE_READ, nullptr,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    DWORD migratedSize = file == INVALID_HANDLE_VALUE ? 0 : GetFileSize(file, nullptr);
    if (file != INVALID_HANDLE_VALUE)
    {
        CloseHandle(file);
    }
    failures += !migrated || globalCoin != 77
        || unlockedCharacterMask != 0x0b || audioEnabled
        || characterGlobalLevel[basicCharacter][0] != 2
        || characterGlobalLevel[mobilityCharacter][0] != 0
        || migratedSize != 32;
    printf("section_migration=%ld\n", failures);

    MetaProfile corrupt{};
    corrupt.magic = metaMagic;
    corrupt.version = metaVersion;
    corrupt.coin = -1;
    file = CreateFileW(metaFileName, GENERIC_WRITE, 0, nullptr,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    transferred = 0;
    if (file != INVALID_HANDLE_VALUE)
    {
        WriteFile(file, &corrupt, sizeof(corrupt), &transferred, nullptr);
        CloseHandle(file);
    }
    failures += ReadMetaProfile() || globalCoin != 0
        || unlockedCharacterMask != 1 || !audioEnabled;
    printf("section_corrupt=%ld\n", failures);

    ResetMetaProfile();
    globalCoin = 100;
    failures += !BuyCommonGlobalUpgrade(runRerollGlobalUpgrade)
        || !BuyCommonGlobalUpgrade(fieldRecoveryGlobalUpgrade)
        || !BuyCommonGlobalUpgrade(coinSenseGlobalUpgrade)
        || !BuyCommonGlobalUpgrade(coinSenseGlobalUpgrade)
        || BuyCommonGlobalUpgrade(coinSenseGlobalUpgrade)
        || globalCoin != 65;
    failures += !UnlockCharacter(mobilityCharacter)
        || !BuyCharacterGlobalUpgrade(mobilityCharacter, 0)
        || characterGlobalLevel[basicCharacter][0] != 0
        || characterGlobalLevel[mobilityCharacter][0] != 1
        || globalCoin != 55;
    characterGlobalLevel[piercerCharacter][2] = 0;
    failures += !BuyCharacterGlobalUpgrade(piercerCharacter, 2)
        || BuyCharacterGlobalUpgrade(piercerCharacter, 2)
        || CharacterSlashHitCap(piercerCharacter) != 2
        || globalCoin != 50
        || lstrcmpW(CharacterGlobalUpgradeName(piercerCharacter, 2),
            L"PIERCE THROUGH") != 0;
    printf("section_purchase=%ld\n", failures);

    commonGlobalLevel[coinSenseGlobalUpgrade] = 0;
    for (BYTE events = 0; events <= 10; events += events ? 9 : 1)
    {
        globalCoin = 0;
        runKillCount = 10;
        alertEventCount = events;
        runRewardGranted = false;
        runEarnedCoin = 0;
        GrantRunCoin(false);
        GrantRunCoin(false);
        failures += globalCoin != 10 || runEarnedCoin != 10;
    }
    commonGlobalLevel[coinSenseGlobalUpgrade] = 1;
    globalCoin = 0;
    runKillCount = 10;
    alertEventCount = 10;
    runRewardGranted = false;
    GrantRunCoin(false);
    failures += globalCoin != 11 || runEarnedCoin != 11;
    commonGlobalLevel[coinSenseGlobalUpgrade] = 2;
    globalCoin = 0;
    runKillCount = 10;
    alertEventCount = 0;
    runRewardGranted = false;
    GrantRunCoin(true);
    failures += globalCoin != 15 || runEarnedCoin != 15;
    globalCoin = 0x7ffffffe;
    runKillCount = 10;
    runRewardGranted = false;
    GrantRunCoin(false);
    failures += globalCoin != 0x7fffffff || runEarnedCoin != 1;
    printf("section_coin=%ld\n", failures);

    EnemyRuntime alertEnemies[3]{};
    for (LONG index = 0; index < 3; ++index)
    {
        alertEnemies[index].alive = true;
        alertEnemies[index].role = index < 2 ? listenerEnemyRole : patrollerEnemyRole;
        alertEnemies[index].x = index * 80.0f;
    }
    alertEventCount = 0;
    alertEventActive = false;
    failures += EnterEnemyAlert(alertEnemies, 3, 0) != 3
        || alertEventCount != 1 || EnterEnemyAlert(alertEnemies, 3, 1) != 0;
    for (LONG index = 0; index < 3; ++index)
    {
        alertEnemies[index].alert = false;
        alertEnemies[index].detectionProgress = 0.0f;
        alertEnemies[index].heardSuspicion = false;
    }
    UpdateAlertEventState(alertEnemies, 3);
    failures += alertEventActive || EnterEnemyAlert(alertEnemies, 3, 2) != 1
        || alertEventCount != 2;
    printf("section_alert=%ld\n", failures);

    ResetMetaProfile();
    unlockedCharacterMask = 0x1f;
    for (BYTE character = 0; character < characterCount; ++character)
    {
        selectedCharacter = character;
        characterGlobalLevel[character][0]
            = characterGlobalMaximum[character][0];
        runSeed = 1000 + character;
        currentRoom = 10;
        moveUpgradeStack = 1;
        slashUpgradeStack = 1;
        dashUpgradeStack = 1;
        functionalUpgradeFlags = executeReachUpgradeFlag | fieldMedicUpgradeFlag;
        rerollUsed = 2;
        currentDashCharges = CharacterDashCapacity(character);
        alertEventCount = 7;
        LONG savedHP = CharacterMaximumHP(character) - combatScale;
        WriteCheckpoint(runSeed, currentRoom, savedHP, 11);
        SaveCheckpoint diskCheckpoint{};
        HANDLE checkpointFile = CreateFileW(saveFileName, GENERIC_READ,
            FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        DWORD checkpointBytes = 0;
        if (checkpointFile != INVALID_HANDLE_VALUE)
        {
            ReadFile(checkpointFile, &diskCheckpoint, sizeof(diskCheckpoint),
                &checkpointBytes, nullptr);
            CloseHandle(checkpointFile);
        }
        SaveCheckpoint checkpoint{};
        failures += checkpointBytes != sizeof(diskCheckpoint)
            || diskCheckpoint.playerHP != savedHP / combatScale
            || !ReadCheckpoint(&checkpoint)
            || checkpoint.version != 8 || checkpoint.character != character
            || checkpoint.reroll != 2 || checkpoint.alertEvents != 7
            || checkpoint.playerHP != savedHP
            || checkpoint.dashCharges != currentDashCharges;
    }
    SaveCheckpoint existingV8
    {
        saveMagic, saveVersion, 1234, 0, 7, 0,
        0, 0, 0, 0, 0, basicCharacter, 1, 0
    };
    HANDLE checkpointFile = CreateFileW(saveFileName, GENERIC_WRITE, 0, nullptr,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    DWORD checkpointBytes = 0;
    if (checkpointFile != INVALID_HANDLE_VALUE)
    {
        WriteFile(checkpointFile, &existingV8, sizeof(existingV8),
            &checkpointBytes, nullptr);
        CloseHandle(checkpointFile);
    }
    SaveCheckpoint loadedV8{};
    failures += checkpointBytes != sizeof(existingV8)
        || !ReadCheckpoint(&loadedV8) || loadedV8.playerHP != 70;
    printf("section_save=%ld\n", failures);

    constexpr BYTE expectedDashByLevel[characterCount][4]
    {
        { 1, 2, 3, 3 },
        { 1, 2, 3, 4 },
        { 1, 2, 2, 2 },
        { 1, 1, 1, 1 },
        { 1, 2, 2, 2 }
    };
    ResetMetaProfile();
    for (BYTE character = 0; character < characterCount; ++character)
    {
        for (BYTE level = 0; level <= characterGlobalMaximum[character][0]; ++level)
        {
            characterGlobalLevel[character][0] = level;
            BYTE dashCapacity = CharacterDashCapacity(character);
            LONG indicatorWidth = dashCapacity * 4 - 1;
            failures += dashCapacity != expectedDashByLevel[character][level]
                || indicatorWidth < 3 || indicatorWidth > 15;
            for (BYTE charges = 0; charges <= dashCapacity; ++charges)
            {
                LONG filledSlots = 0;
                LONG emptySlots = 0;
                for (BYTE slot = 0; slot < dashCapacity; ++slot)
                {
                    slot < charges ? ++filledSlots : ++emptySlots;
                }
                failures += filledSlots != charges
                    || emptySlots != dashCapacity - charges;
            }
        }
        failures += !CharacterName(character)[0];
    }
    printf("section_dash_hud=%ld\n", failures);

    ResetMetaProfile();
    selectedCharacter = basicCharacter;
    applicationState = titleMainState;
    menuSelection = 0;
    zPressed = false;
    WindowProcedure(nullptr, WM_KEYDOWN, 'Z', 0);
    WindowProcedure(nullptr, WM_KEYUP, 'Z', 0);
    failures += applicationState != preparationState || menuSelection != 0;
    menuSelection = 3;
    WindowProcedure(nullptr, WM_KEYDOWN, 'Z', 0);
    WindowProcedure(nullptr, WM_KEYUP, 'Z', 0);
    failures += applicationState != characterSelectState
        || menuSelection != basicCharacter || characterUpgradeFocus;
    menuSelection = mobilityCharacter;
    globalCoin = 0;
    WindowProcedure(nullptr, WM_KEYDOWN, 'Z', 0);
    WindowProcedure(nullptr, WM_KEYUP, 'Z', 0);
    failures += titleStatus != 3
        || (unlockedCharacterMask & (1 << mobilityCharacter));
    globalCoin = characterUnlockCosts[mobilityCharacter];
    WindowProcedure(nullptr, WM_KEYDOWN, 'Z', 0);
    WindowProcedure(nullptr, WM_KEYUP, 'Z', 0);
    failures += !(unlockedCharacterMask & (1 << mobilityCharacter))
        || selectedCharacter != mobilityCharacter || globalCoin != 0;
    characterUpgradeFocus = true;
    characterUpgradeSelection = 0;
    globalCoin = characterGlobalCost[0];
    WindowProcedure(nullptr, WM_KEYDOWN, 'Z', 0);
    WindowProcedure(nullptr, WM_KEYUP, 'Z', 0);
    failures += characterGlobalLevel[mobilityCharacter][0] != 1
        || globalCoin != 0;
    characterUpgradeSelection = 3;
    WindowProcedure(nullptr, WM_KEYDOWN, 'Z', 0);
    WindowProcedure(nullptr, WM_KEYUP, 'Z', 0);
    failures += applicationState != gameplayState || !newGameRequested
        || selectedCharacter != mobilityCharacter;
    newGameRequested = false;
    printf("section_ui=%ld\n", failures);

    EnemyRuntime locatorEnemies[5]{};
    for (LONG enemyIndex = 0; enemyIndex < 5; ++enemyIndex)
    {
        locatorEnemies[enemyIndex].alive = true;
        locatorEnemies[enemyIndex].role = static_cast<BYTE>(enemyIndex % 5);
    }
    locatorEnemies[0].x = 100.0f;
    locatorEnemies[0].y = 100.0f;
    locatorEnemies[1].x = 400.0f;
    locatorEnemies[1].y = 90.0f;
    locatorEnemies[2].x = -100.0f;
    locatorEnemies[2].y = 90.0f;
    locatorEnemies[3].x = 160.0f;
    locatorEnemies[3].y = 300.0f;
    locatorEnemies[4].alive = false;
    roomSizeStage = 2;
    currentEnemyRemaining = 5;
    failures += DrawEnemyLocators(locatorEnemies, 5, 0, 0) != 0;
    roomSizeStage = 3;
    currentEnemyRemaining = 6;
    failures += DrawEnemyLocators(locatorEnemies, 5, 0, 0) != 0;
    currentEnemyRemaining = 5;
    for (LONG size = 3; size <= 5; ++size)
    {
        roomSizeStage = size;
        failures += DrawEnemyLocators(locatorEnemies, 5, 0, 0) != 3;
    }
    failures += !EnemyVisibleInViewport(locatorEnemies[0], 0, 0)
        || EnemyVisibleInViewport(locatorEnemies[1], 0, 0)
        || !EnemyVisibleInViewport(locatorEnemies[1], 100, 0);
    for (LONG enemyIndex = 0; enemyIndex < 5; ++enemyIndex)
    {
        locatorEnemies[enemyIndex].alive = true;
        locatorEnemies[enemyIndex].x = 400.0f + enemyIndex * 20.0f;
        locatorEnemies[enemyIndex].y = 200.0f + enemyIndex * 10.0f;
    }
    failures += DrawEnemyLocators(locatorEnemies, 5, 0, 0) != 5;
    currentEnemyRemaining = 1;
    failures += DrawEnemyLocators(locatorEnemies, 1, 0, 0) != 1;
    locatorEnemies[0].alive = false;
    currentEnemyRemaining = 0;
    failures += DrawEnemyLocators(locatorEnemies, 1, 0, 0) != 0;
    locatorEnemies[0].alive = true;
    locatorEnemies[0].role = pressureEnemyRole;
    currentEnemyRemaining = 1;
    failures += DrawEnemyLocators(locatorEnemies, 1, 0, 0) != 0;
    printf("section_locator=%ld\n", failures);

    ResetMetaProfile();
    for (BYTE character = 0; character < characterCount; ++character)
    {
        LONG hitTargets = 0;
        LONG appliedDamage = 0;
        for (LONG enemy = 0; enemy < maxEnemyCount; ++enemy)
        {
            if (hitTargets < CharacterSlashHitCap(character))
            {
                ++hitTargets;
                appliedDamage += characterProfiles[character].slashDamage;
            }
        }
        failures += appliedDamage != characterProfiles[character].slashDamage
            * CharacterSlashHitCap(character);
    }
    characterGlobalLevel[piercerCharacter][2] = 1;
    failures += CharacterSlashHitCap(piercerCharacter) != 2;
    printf("section_multitarget=%ld\n", failures);

#ifdef DEAD_SIGNAL_B101_SOAK_VALIDATION
    ULONGLONG soakStart = GetTickCount64();
    ULONGLONG soakUpdates = 0;
    LONG soakFailures = 0;
    EnemyRuntime soakLocator{};
    soakLocator.alive = true;
    soakLocator.role = patrollerEnemyRole;
    soakLocator.x = 400.0f;
    soakLocator.y = 90.0f;
    while (GetTickCount64() - soakStart < 90000)
    {
        BYTE character = static_cast<BYTE>(soakUpdates % characterCount);
        selectedCharacter = character;
        for (BYTE upgrade = 0; upgrade < characterGlobalUpgradeCount; ++upgrade)
        {
            characterGlobalLevel[character][upgrade]
                = static_cast<BYTE>(soakUpdates
                    % (characterGlobalMaximum[character][upgrade] + 1));
        }
        moveUpgradeStack = static_cast<BYTE>(soakUpdates % 3);
        slashUpgradeStack = static_cast<BYTE>((soakUpdates / 3) % 3);
        dashUpgradeStack = static_cast<BYTE>((soakUpdates / 9) % 3);
        RecalculateAugmentStats(runMove, runSlash, runDash);
        roomSizeStage = 3 + static_cast<LONG>(soakUpdates % 3);
        currentEnemyRemaining = (soakUpdates & 1) ? 5 : 6;
        LONG locatorCount = DrawEnemyLocators(&soakLocator, 1, 0, 0);
        soakFailures += runMove <= 0.0f || runSlash <= 0.0f || runDash <= 0.0f
            || CharacterDashCapacity(character) > characterDashCaps[character]
            || CharacterMaximumHP(character) < expectedHP[character]
            || CharacterSlashHitCap(character) < characterProfiles[character].slashHitCap
            || CharacterSlashHitCap(character) > characterProfiles[character].slashHitCap + 1
            || locatorCount != (currentEnemyRemaining == 5 ? 1 : 0);
        ++soakUpdates;
    }
    failures += soakFailures;
    printf("soak_ms=%llu soak_updates=%llu soak_failures=%ld\n",
        GetTickCount64() - soakStart, soakUpdates, soakFailures);
#endif

    DeleteFileW(metaFileName);
    DeleteFileW(saveFileName);
    printf("b101_failures=%ld alert=%u earned=%ld save=%lu legacy=%lu meta=%lu\n",
        failures, alertEventCount, runEarnedCoin,
        static_cast<unsigned long>(sizeof(SaveCheckpoint)),
        static_cast<unsigned long>(sizeof(LegacyMetaProfile)),
        static_cast<unsigned long>(sizeof(MetaProfile)));
    return failures;
}

extern "C" __declspec(dllexport) void CALLBACK RunB101Validation(HWND, HINSTANCE,
    LPSTR, int)
{
    FILE* output = nullptr;
    freopen_s(&output, "B101Validation.txt", "w", stdout);
    main();
    if (output)
    {
        fclose(output);
    }
}
#endif
