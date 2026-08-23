# DEAD SIGNAL Repository Guidelines

## Project Identity and Goal

- Game title: `DEAD SIGNAL`
- Development project: `1.44MB GAME_DEV`
- Final goal: complete the game while keeping the entire final submission at or below exactly `1,474,560 bytes`.
- Do not interpret `1.44MB` using an approximate KB/MB display. Record and compare sizes in exact bytes.

This repository is currently a minimal Visual Studio Windows Native C++ project. `DeadSignal.slnx` is the solution entry point and `DeadSignal/DeadSignal.vcxproj` is the single application target. The project currently contains one source file, `DeadSignal/DeadSignal.cpp`. Both Win32 and x64 Debug/Release configurations exist.

## Decision Priority

When rules or materials conflict, use this order:

1. The user's most recent instruction for the current task
2. The latest `1.44MB GAME_DEV 기본 기획서`
3. The actual current source, framework, and build state
4. Older documents and prior work

Do not silently resolve a conflict by changing the project. Report the difference to the user first. Do not change an approved design rule without user approval. If a change appears necessary, follow: identify the problem, explain the reason, propose the change, and wait for the user's decision.

## Current Development Stage

`B00 Empty Executable Baseline` is complete. The measured executable sizes were `9,216 bytes` for MSVC Release Win32/x86 and `10,752 bytes` for MSVC Release x64.

`B01 Minimal Win32 Window` is complete. The measured executable sizes were `10,240 bytes` for MSVC Release Win32/x86 (`+1,024 bytes` from B00) and `12,288 bytes` for MSVC Release x64 (`+1,536 bytes` from B00). Both architectures passed build and manual execution validation. Window display, movement, resizing, minimize/maximize, and X-button exit behaved normally. The Release subsystem is Windows GUI and the user entry point is `wWinMain`.

`B02 Minimal Software Framebuffer Output` is complete. The test framebuffer is `2x2 pixels` (`초기 기준`) using 32-bit top-down `BI_RGB`, with a red/green/blue/white test pattern. The Win32 GDI `StretchDIBits` output path is an initial prototype candidate and is not the final Renderer specification.

The measured B02 executable sizes were `10,752 bytes` for MSVC Release Win32/x86 (`+512 bytes` from B01) and `12,288 bytes` for MSVC Release x64 (`+0 bytes` from B01). Both architectures passed build and execution validation. Framebuffer output succeeded, resizing stretched the fixed framebuffer across the full client area, and minimize/maximize and X-button exit behaved normally. The system import `GDI32.dll` was added; no external library was added.

`B03 Minimal Real-time Game Loop` is complete. The blocking `GetMessageW` loop was replaced with non-blocking `PeekMessageW(PM_REMOVE)` message processing. High-resolution time measurement uses `QueryPerformanceFrequency` and `QueryPerformanceCounter`. The prototype Update target is approximately 60 Hz (`초기 기준`); current pacing uses `Sleep(1)` with `timeBeginPeriod(1)` and `timeEndPeriod(1)`. The measured Update rate is approximately 56 Hz and is not the final FPS or timing policy.

The B03 test Update changes the framebuffer red component over time and requests redraw with `InvalidateRect`, reusing the existing `WM_PAINT` and `StretchDIBits` output path. The system import `WINMM.dll` was added; no external library was added.

The measured B03 executable sizes were `11,264 bytes` for MSVC Release Win32/x86 (`+512 bytes` from B02) and `13,312 bytes` for MSVC Release x64 (`+1,024 bytes` from B02). Both architectures passed build and execution validation, including continuous Update, window movement, resizing, minimize/maximize, and X-button exit.

`B04 Minimal Keyboard Input` is complete. The only test input is the W key. `WM_KEYDOWN` and `WM_KEYUP` record pressed/released state in one minimal `bool`; no Input abstraction was added. The framebuffer test color changes correctly while W is pressed or held and returns to its original color when W is released. The B03 real-time loop and timing continue to operate normally. Player, Movement, and full WASD input are not implemented.

The measured B04 executable sizes were `11,776 bytes` for MSVC Release Win32/x86 (`+512 bytes` from B03) and `13,312 bytes` for MSVC Release x64 (`+0 bytes` from B03). Both architectures passed build and execution validation, including W press/hold/release, window movement, resizing, minimize/maximize, and X-button exit. No system import or external library was added.

`B05 Minimal Generated Audio` is complete. A square-wave PCM tone is generated in RAM and played without an audio asset. The prototype values (`초기 기준`) are 8,000 Hz, unsigned 8-bit PCM, mono, 440 Hz, 250 ms, and a 2,000-byte PCM buffer. These values are not final game sound specifications. The tone uses callback-free asynchronous `waveOut` playback with `CALLBACK_NULL`. A new Space press plays one tone; additional Space input is ignored while it is playing.

No Audio Thread, Mixer, SoundManager, or Audio abstraction was added, and no Audio Asset exists. The existing `WINMM.dll` dependency is reused with no new DLL dependency or external library.

The measured B05 executable sizes were `11,776 bytes` for MSVC Release Win32/x86 (`+0 bytes` from B04) and `13,312 bytes` for MSVC Release x64 (`+0 bytes` from B04). Both architectures passed build and execution validation. Tone playback and replay after completion succeeded. During playback, the game loop, framebuffer Update, W input, window movement, resizing, minimize/maximize, and X-button exit continued to operate normally.

`P00 Minimal Play Space` is complete. The Logical Resolution is `320x180` (`확정`), the Aspect Ratio is `16:9` (`확정`), and one logical pixel equals one game unit. The current coordinate convention uses the top-left as `(0,0)`, +X to the right, and +Y downward.

The framebuffer is 32-bit top-down `BI_RGB` and uses `230,400 bytes` of runtime RAM as zero-initialized static storage. Large pixel data is not stored in the executable; the required test pixels are generated at runtime. The P00 test screen contains a solid background, directional top/bottom/left/right borders, a center cross, and small markers for the existing real-time and input validation.

The framebuffer is aspect-fitted to the Window client area without distortion, unused space is black, and the framebuffer is centered. Integer scaling and the final pixel-perfect policy remain `미정`.

The measured P00 executable sizes were `12,288 bytes` for MSVC Release Win32/x86 (`+512 bytes` from B05) and `15,872 bytes` for MSVC Release x64 (`+2,560 bytes` from B05). Both architectures passed build and execution validation. The `320x180` framebuffer, 4:3 and wide aspect-ratio preservation, centering, resizing, minimize/maximize, X-button exit, B03 real-time loop, B04 W input, and B05 generated audio operated normally. No external Asset or new abstraction/system was added. Movement, Collision, and Enemy are not implemented.

`P01 Minimal Player Display` is complete. The Player Visual Bounding Size is `8x12 logical pixels` (`확정`). The Player is currently fixed at the framebuffer center `(160,90)`, with drawing bounds X=`156..163` and Y=`84..95`. A two-color prototype pixel silhouette distinguishing the head, body, arms, and legs is generated at runtime without an external Asset, Sprite System, Renderer abstraction, or Animation. This is not the final pixel art or palette.

The measured P01 executable sizes were `12,288 bytes` for MSVC Release Win32/x86 (`+0 bytes` from P00) and `16,384 bytes` for MSVC Release x64 (`+512 bytes` from P00). Both architectures passed build and execution validation. The `8x12` bounds and centered display remained correct at 4:3 and wide Window sizes, and the existing real-time loop, W input, Space generated audio, resizing, minimize/maximize, and close behavior continued to operate normally. Player Movement is not implemented.

`P02 Minimal Player Movement` is complete. The Player Visual Bounding Size remains `8x12 logical pixels` (`확정`), and the initial position is the logical framebuffer center `(160,90)`. WASD provides free directional movement, with each key state stored in a minimal `bool` and the current Player position stored as `float playerX` and `float playerY`. Movement uses QPC delta time for frame-rate independence. Simultaneous movement on both axes applies `0.70710678f` normalization, while W+S and A+D cancel on their respective axes. Player drawing retains framebuffer bounds checks without implementing a Boundary clamp or Collision.

The prototype movement speed is `60 logical pixels per second` (`초기 기준`). It currently feels appropriate to the user, but it is not the final game movement-speed specification and may change when Sneak / Walk / Run is designed.

The measured P02 executable sizes were `13,824 bytes` for MSVC Release Win32/x86 (`+1,536 bytes` from P01) and `16,896 bytes` for MSVC Release x64 (`+512 bytes` from P01). Both architectures passed Release build and execution validation. Four-direction WASD movement, all four diagonals, opposing-direction cancellation, and near-equal straight/diagonal movement speed operated normally. The real-time loop, generated audio, 16:9 aspect-fit, resizing, window movement, minimize/maximize, and X-button exit also remained normal. No external Asset or Library, new abstraction/system, or Release setting change was added.

P02 does not implement Boundary clamp, Wall Collision, Hitbox/Collision specifications, Sneak / Walk / Run, Acceleration / Deceleration, Facing / Animation, Dash, Slash, or Execute.

`P03 Minimal Boundary + Wall Collision` is complete. The Player Visual Bounding Size and Player Collision Bounding Size are both `8x12 logical pixels` (`확정`) and use the same full rectangle. There is no separate Hitbox and no pixel-perfect silhouette collision. The existing center-based `float playerX` and `float playerY` position remain in use. Screen boundary handling keeps the full Player rectangle inside the `320x180` logical framebuffer, allowing Player center coordinates X=`4..316` and Y=`6..174`.

A single runtime-generated rectangular test Wall uses X=`200..207` and Y=`60..119`, or `8x60 logical pixels` (`초기 기준`). Its visual and collision rectangles are identical. Player movement is tested separately on the X and Y axes with direct rectangle comparisons, which preserves movement on an unblocked axis and allows sliding along the Wall. No CollisionManager, Physics system, Collider/AABB/Hitbox class, multiple-Wall system, or continuous collision detection was added. The test Wall dimensions are not the final game Wall or Tile specification.

The measured P03 executable sizes were `14,336 bytes` for MSVC Release Win32/x86 (`+512 bytes` from P02) and `18,944 bytes` for MSVC Release x64 (`+2,048 bytes` from P02). Both architectures passed Release build and execution validation. The four screen boundaries, movement along each boundary, Wall collision from all four directions, diagonal Wall sliding, matching visual/collision bounds, WASD movement, real-time loop, generated audio, 16:9 aspect-fit, resizing, window movement, minimize/maximize, and X-button exit operated normally. No external Asset or Library, new abstraction/system, or Release setting change was added.

The current minimal foundation is Window + Real-time Loop + Software Framebuffer + Keyboard Input + Runtime Generated Audio + Logical Play Space + Player Display + Player Movement + Screen Boundary + Single Test Wall Collision.

The final Architecture, integer-scaling/pixel-perfect policy, Window size policy, final Player pixel art, palette, Animation frame specification, actual Wall/Tile specification, need for a general Collision structure, Room structure, relationship between gameplay width W and visual width, FPS/timing/pacing policy, Audio API, and actual sound specifications remain `미정`. Do not implement `P04` until explicitly requested.

## Development Principles

- Do not use Unity or another general-purpose game engine.
- Build an ultralight custom framework in Windows Native C++.
- Implement only functionality that `DEAD SIGNAL` actually needs.
- Do not add features or abstractions in anticipation of future reuse.
- Reuse the existing structure when it can satisfy the request; do not add a new system without demonstrated need.
- Prefer removing unused functionality over merely disabling it.
- Do not add an external runtime or library without comparing its measured final-build cost against its concrete benefit.
- Do not increase binary size solely for generality or architectural elegance.
- Do not apply a size optimization merely to save a few bytes if it would materially harm development stability or game completeness.
- Treat the C++ standard version, CRT linkage/use, character set, SDL checks, STL use, architecture, compiler options, and linker options as undecided final specifications. Current project settings are only the present baseline state. Adopt final settings only after measuring size and implementation benefit.
- MSVC is the initial compiler baseline. The final choice between Win32 (x86) and x64 is undecided.
- Do not add empty directories or hierarchy for anticipated expansion.

## Do Not Prebuild General Systems

Do not implement or prepare structures for the following until a concrete need is confirmed:

- General-purpose Scene System
- ECS
- Reflection
- General-purpose UI Framework
- General-purpose Animation Framework
- General-purpose Physics Engine
- Plugin System
- Runtime Scripting
- General-purpose Renderer abstraction
- General-purpose Input abstraction
- General-purpose Audio Engine
- General-purpose Asset Manager
- General-purpose Platform abstraction
- Unnecessary Utility or Math Library
- Separate Test Framework or Test Project
- Unnecessary multiplatform structure

## Build-Size Measurement Rules

Base technical decisions on actual final build results, not estimates. Whenever possible, compare each change as:

`current Build Size -> minimal implementation -> new Build Size -> byte increase/decrease`

- Record executable and submission sizes as exact byte counts.
- Define and report exactly which artifacts are included in the measured final submission.
- Record the compiler, target architecture, configuration, compiler options, linker options, and other conditions needed to reproduce each meaningful measurement.
- Compare Win32 and x64 or alternative settings using actual build output before adopting a final choice.
- Distinguish development/debug artifacts from the final submission. Do not count or exclude an artifact without stating the measurement rule.
- Do not assume that the current Debug/Release configurations or current project defaults are the final size baseline.

The solution currently exposes Win32 and x64 configurations. The project currently uses MSVC `v145`, Windows 10 SDK selection, C++20, Unicode, SDL checks, conformance mode, and warning level 3. The Release configurations use the Windows GUI subsystem. These describe the checked-in project state only; they are not approved final specifications.

## Data Principles

Store the minimum data and rules needed to regenerate results rather than storing fully expanded results:

- Graphic = Pattern + Palette + Transform
- Sound = Wave + Frequency + Sequence
- Map = Tile + Chunk + Rule + Seed
- Save = minimum State required for restoration, not the full derived result

Minimize disk use and deliberately trade available RAM and runtime computation for stored bytes when the measured benefit is worthwhile and game stability remains acceptable.

## Required Work Procedure

For implementation work, always proceed in this order:

`inspect current structure -> inspect relevant code -> verify whether the existing structure can be reused -> make the minimum requested implementation -> validate -> measure actual Build Size -> report results`

- Do not add unrequested features, refactors, or structural changes.
- Keep implementation files under `DeadSignal/` and keep a header beside its corresponding `.cpp` when both are genuinely needed.
- When adding a required file, update `DeadSignal.vcxproj` and `DeadSignal.vcxproj.filters` so Visual Studio includes it in the appropriate filter.
- Do not commit generated or machine-local outputs such as `.vs/`, `Debug/`, `Release/`, `x64/`, binaries, or IDE user settings.
- Use four spaces, not tabs, and follow the style already present in nearby code. Do not introduce a naming or ownership abstraction merely to enforce a generic convention.

## Validation

Validation must be proportional to the requested change and the current stage.

- Build and manually exercise only the configurations relevant to the current decision; do not treat Debug x64 and Release x64 as permanently preferred configurations.
- When architecture or build settings are being selected, compare the relevant candidates rather than assuming one.
- No separate test framework or test project is currently authorized. Add one only when concrete need and size cost have been evaluated and the user has requested or approved it.
- Report commands run, whether they succeeded, exact measured byte counts, measurement scope, and any unverified behavior.

## Status Labels for Numeric Values

Always label meaningful design and tuning values as one of:

- `확정`: use as specified by the current approved design
- `초기 기준`: use for the prototype and allow adjustment after playtesting
- `미정`: not decided; do not invent a final value

The `1,474,560 bytes` maximum for the entire final submission is `확정`. Architecture and the final compiler, CRT, language, character-set, SDL, STL, and linker choices are `미정`.

## Change and Reporting Discipline

- Inspect the actual repository state before editing.
- Limit changes to the user's explicit scope.
- Preserve unrelated user changes and machine-specific state.
- If documentation and the actual project differ, report both; do not rewrite one merely to hide the discrepancy.
- Use short, imperative commit subjects if a commit is requested. Keep unrelated changes separate.
- In a pull request, explain intent and behavior, list validation and size-measurement commands, link relevant issues, and include terminal output or screenshots when user-visible behavior changes.
