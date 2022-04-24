#include "SohImGuiImpl.h"

#include <iostream>
#include <utility>

#include "Archive.h"
#include "Environment.h"
#include "GameSettings.h"
#include "SohConsole.h"
#include "SohHooks.h"
#include "Lib/ImGui/imgui_internal.h"
#include "GlobalCtx2.h"
#include "ResourceMgr.h"
#include "TextureMod.h"
#include "Window.h"
#include "Cvar.h"
#include "../Fast3D/gfx_pc.h"
#include "Lib/stb/stb_image.h"
#include "Lib/Fast3D/gfx_rendering_api.h"
#include "Utils/StringHelper.h"

#ifdef ENABLE_OPENGL
#include "Lib/ImGui/backends/imgui_impl_opengl3.h"
#include "Lib/ImGui/backends/imgui_impl_sdl.h"

#endif

#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
#include "Lib/ImGui/backends/imgui_impl_dx11.h"
#include "Lib/ImGui/backends/imgui_impl_win32.h"
#include <Windows.h>

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif

using namespace Ship;
bool oldCursorState = true;

#define TOGGLE_BTN ImGuiKey_F1
#define HOOK(b) if(b) needs_save = true;
OSContPad* pads;

std::map<std::string, GameAsset*> DefaultAssets;
namespace SohImGui {
    WindowImpl impl;
    ImGuiIO* io;
    Console* console = new Console;
    bool p_open = false;
    bool needs_save = false;

    float hearts_colors[3] = {0,0,0};
    float hearts_dd_colors[3] = {0,0,0};
    float a_btn_colors[3] = {0,0,0};
    float b_btn_colors[3] = {0,0,0};
    float c_btn_colors[3] = {0,0,0};
    float start_btn_colors[3] = {0,0,0};
    float magic_border_colors[3] = {0,0,0};
    float magic_remaining_colors[3] = {0,0,0};
    float magic_use_colors[3] = {0,0,0};
    float minimap_colors[3] = {0,0,0};
    float rupee_colors[3] = {0,0,0};

    void ImGuiWMInit() {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplSDL2_InitForOpenGL(static_cast<SDL_Window*>(impl.sdl.window), impl.sdl.context);
            break;
#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplWin32_Init(impl.dx11.window);
            break;
#endif
        default:
            break;
        }
    }

    void ImGuiBackendInit() {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplOpenGL3_Init("#version 120");
            break;

#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplDX11_Init(static_cast<ID3D11Device*>(impl.dx11.device), static_cast<ID3D11DeviceContext*>(impl.dx11.device_context));
            break;
#endif
        default:
            break;
        }
    }

    void ImGuiProcessEvent(EventImpl event) {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplSDL2_ProcessEvent(static_cast<const SDL_Event*>(event.sdl.event));
            break;
#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplWin32_WndProcHandler(static_cast<HWND>(event.win32.handle), event.win32.msg, event.win32.wparam, event.win32.lparam);
            break;
#endif
        default:
            break;
        }
    }

    void ImGuiWMNewFrame() {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplSDL2_NewFrame(static_cast<SDL_Window*>(impl.sdl.window));
            break;
#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplWin32_NewFrame();
            break;
#endif
        default:
            break;
        }
    }

    void ImGuiBackendNewFrame() {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplOpenGL3_NewFrame();
            break;
#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplDX11_NewFrame();
            break;
#endif
        default:
            break;
        }
    }

    void ImGuiRenderDrawData(ImDrawData* data) {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplOpenGL3_RenderDrawData(data);
            break;
#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplDX11_RenderDrawData(data);
            break;
#endif
        default:
            break;
        }
    }

    /*bool UseInternalRes() {
        switch (impl.backend) {
        case Backend::SDL:
            return true;
        default:
            return false;
        }
    }*/

    bool UseViewports() {
        switch (impl.backend) {
        case Backend::DX11:
            return true;
        default:
            return false;
        }
    }

    void ShowCursor(bool hide, Dialogues d) {
        if (d == Dialogues::dLoadSettings) {
            GlobalCtx2::GetInstance()->GetWindow()->ShowCursor(hide);
            return;
        }

        if (d == Dialogues::dConsole && Game::Settings.debug.menu_bar) {
            return;
        }
        if (!GlobalCtx2::GetInstance()->GetWindow()->IsFullscreen()) {
            oldCursorState = false;
            return;
        }

        if (oldCursorState != hide) {
            oldCursorState = hide;
            GlobalCtx2::GetInstance()->GetWindow()->ShowCursor(hide);
        }
    }

    void LoadTexture(std::string name, std::string path) {
        GfxRenderingAPI* api = gfx_get_current_rendering_api();
        const auto res = GlobalCtx2::GetInstance()->GetResourceManager()->LoadFile(normalize(path));

        const auto asset = new GameAsset{ api->new_texture() };
        uint8_t* img_data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(res->buffer.get()), res->dwBufferSize, &asset->width, &asset->height, nullptr, 4);

        if (img_data == nullptr) {
            std::cout << "Found error: " << stbi_failure_reason() << std::endl;
            return;
        }

        api->select_texture(0, asset->textureId);
        api->set_sampler_parameters(0, false, 0, 0);
        api->upload_texture(img_data, asset->width, asset->height);

        DefaultAssets[name] = asset;
        stbi_image_free(img_data);
    }

    void LoadHudColors(){//This function is necessary as without it IMGui wont load the updated float array.
        hearts_colors[0] = (float)CVar_GetInt("gCCHeartsPrimR", 255)/255;
        hearts_colors[1] = (float)CVar_GetInt("gCCHeartsPrimG", 10)/255;
        hearts_colors[2] = (float)CVar_GetInt("gCCHeartsPrimB", 10)/255;
        hearts_dd_colors[0] = (float)CVar_GetInt("gDDCCHeartsPrimR", 255)/255;
        hearts_dd_colors[1] = (float)CVar_GetInt("gDDCCHeartsPrimG", 255)/255;
        hearts_dd_colors[2] = (float)CVar_GetInt("gDDCCHeartsPrimB", 255)/255;
        a_btn_colors[0] = (float)CVar_GetInt("gCCABtnPrimR", 90)/255;
        a_btn_colors[1] = (float)CVar_GetInt("gCCABtnPrimG", 90)/255;
        a_btn_colors[2] = (float)CVar_GetInt("gCCABtnPrimB", 255)/255;
        b_btn_colors[0] = (float)CVar_GetInt("gCCBBtnPrimR", 0)/255;
        b_btn_colors[1] = (float)CVar_GetInt("gCCBBtnPrimG", 150)/255;
        b_btn_colors[2] = (float)CVar_GetInt("gCCBBtnPrimB", 0)/255;
        c_btn_colors[0] = (float)CVar_GetInt("gCCCBtnPrimR", 255)/255;
        c_btn_colors[1] = (float)CVar_GetInt("gCCCBtnPrimG", 160)/255;
        c_btn_colors[2] = (float)CVar_GetInt("gCCCBtnPrimB", 0)/255;
        start_btn_colors[0] = (float)CVar_GetInt("gCCStartBtnPrimR", 120)/255;
        start_btn_colors[1] = (float)CVar_GetInt("gCCStartBtnPrimG", 120)/255;
        start_btn_colors[2] = (float)CVar_GetInt("gCCStartBtnPrimB", 120)/255;
        magic_border_colors[0] = (float)CVar_GetInt("gCCMagicBorderPrimR", 255)/255;
        magic_border_colors[1] = (float)CVar_GetInt("gCCMagicBorderPrimG", 255)/255;
        magic_border_colors[2] = (float)CVar_GetInt("gCCMagicBorderPrimB", 255)/255;
        magic_remaining_colors[0] = (float)CVar_GetInt("gCCMagicPrimR", 250)/255;
        magic_remaining_colors[1] = (float)CVar_GetInt("gCCMagicPrimG", 250)/255;
        magic_remaining_colors[2] = (float)CVar_GetInt("gCCMagicPrimB", 0)/255;
        magic_remaining_colors[0] = (float)CVar_GetInt("gCCMagicUsePrimR", 0)/255;
        magic_remaining_colors[1] = (float)CVar_GetInt("gCCMagicUsePrimG", 200)/255;
        magic_remaining_colors[2] = (float)CVar_GetInt("gCCMagicUsePrimB", 0)/255;
        minimap_colors[0] = (float)CVar_GetInt("gCCMinimapPrimR", 0)/255;
        minimap_colors[1] = (float)CVar_GetInt("gCCMinimapPrimG", 255)/255;
        minimap_colors[2] = (float)CVar_GetInt("gCCMinimapPrimB", 255)/255;
        rupee_colors[0] = (float)CVar_GetInt("gCCRupeePrimR", 120)/255;
        rupee_colors[1] = (float)CVar_GetInt("gCCRupeePrimG", 120)/255;
        rupee_colors[2] = (float)CVar_GetInt("gCCRupeePrimB", 120)/255;
    } 

    int ClampFloatToInt(float value, int min, int max){
        return fmin(fmax(value,min),max);
    }
    float ClampIntToFloat(int value, float min, float max){
        return fmin(fmax(value,min),max);
    }

    void Init(WindowImpl window_impl) {
        impl = window_impl;
        Game::LoadSettings();
        ImGuiContext* ctx = ImGui::CreateContext();
        ImGui::SetCurrentContext(ctx);
        io = &ImGui::GetIO();
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (UseViewports()) {
            io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        }
        console->Init();
        ImGuiWMInit();
        ImGuiBackendInit();
        LoadHudColors();
        ModInternal::registerHookListener({ GFX_INIT, [](const HookEvent ev) {

            if (GlobalCtx2::GetInstance()->GetWindow()->IsFullscreen())
                ShowCursor(Game::Settings.debug.menu_bar, Dialogues::dLoadSettings);

            LoadTexture("Game_Icon", "assets/ship_of_harkinian/icons/gSohIcon.png");
            LoadTexture("A-Btn", "assets/ship_of_harkinian/buttons/ABtn.png");
            LoadTexture("B-Btn", "assets/ship_of_harkinian/buttons/BBtn.png");
            LoadTexture("L-Btn", "assets/ship_of_harkinian/buttons/LBtn.png");
            LoadTexture("R-Btn", "assets/ship_of_harkinian/buttons/RBtn.png");
            LoadTexture("Z-Btn", "assets/ship_of_harkinian/buttons/ZBtn.png");
            LoadTexture("Start-Btn", "assets/ship_of_harkinian/buttons/StartBtn.png");
            LoadTexture("C-Left", "assets/ship_of_harkinian/buttons/CLeft.png");
            LoadTexture("C-Right", "assets/ship_of_harkinian/buttons/CRight.png");
            LoadTexture("C-Up", "assets/ship_of_harkinian/buttons/CUp.png");
            LoadTexture("C-Down", "assets/ship_of_harkinian/buttons/CDown.png");

        } });

        ModInternal::registerHookListener({ CONTROLLER_READ, [](const HookEvent ev) {
            pads = static_cast<OSContPad*>(ev->baseArgs["cont_pad"]);
        } });
        Game::InitSettings();
        //LoadHUDColors();
    }

    void Update(EventImpl event) {
        if (needs_save) {
            Game::SaveSettings();
            needs_save = false;
        }
        ImGuiProcessEvent(event);
    }

#ifdef ENABLE_DX11
#define BindButton(btn, status) ImGui::Image(impl.backend == Backend::DX11 ? GetTextureByID(DefaultAssets[btn]->textureId) : (ImTextureID)(DefaultAssets[btn]->textureId), ImVec2(16.0f * scale, 16.0f * scale), ImVec2(0, 0), ImVec2(1.0f, 1.0f), ImVec4(255, 255, 255, (status) ? 255 : 0));
#else
#define BindButton(btn, status) ImGui::Image((ImTextureID)(DefaultAssets[btn]->textureId), ImVec2(16.0f * scale, 16.0f * scale), ImVec2(0, 0), ImVec2(1.0f, 1.0f), ImVec4(255, 255, 255, (status) ? 255 : 0));
#endif
    void BindAudioSlider(const char* name, const char* key, float* value, SeqPlayers playerId) {
        ImGui::Text(name, static_cast<int>(100 * *(value)));
        if (ImGui::SliderFloat((std::string("##") + key).c_str(), value, 0.0f, 1.0f, "")) {
            const float volume = floorf(*(value) * 100) / 100;
            CVar_SetFloat(key, volume);
            needs_save = true;
            Game::SetSeqPlayerVolume(playerId, volume);
        }
    }
    //void Draw() {
    void DrawMainMenuAndCalculateGameSize() {

        console->Update();
        ImGuiBackendNewFrame();
        ImGuiWMNewFrame();
        ImGui::NewFrame();
        const std::shared_ptr<Window> wnd = GlobalCtx2::GetInstance()->GetWindow();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoResize;
        if (UseViewports()) {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }
        if (Game::Settings.debug.menu_bar) window_flags |= ImGuiWindowFlags_MenuBar;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(ImVec2(wnd->GetCurrentWidth(), wnd->GetCurrentHeight()));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
        ImGui::Begin("Main - Deck", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        ImVec2 top_left_pos = ImGui::GetWindowPos();

        const ImGuiID dockId = ImGui::GetID("main_dock");

        if (!ImGui::DockBuilderGetNode(dockId)) {
            ImGui::DockBuilderRemoveNode(dockId);
            ImGui::DockBuilderAddNode(dockId, ImGuiDockNodeFlags_NoTabBar);

            ImGui::DockBuilderDockWindow("OoT Master Quest", dockId);

            ImGui::DockBuilderFinish(dockId);
        }

        ImGui::DockSpace(dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        if (ImGui::IsKeyPressed(TOGGLE_BTN)) {
            Game::Settings.debug.menu_bar = !Game::Settings.debug.menu_bar;
            needs_save = true;
            GlobalCtx2::GetInstance()->GetWindow()->dwMenubar = Game::Settings.debug.menu_bar;
            ShowCursor(Game::Settings.debug.menu_bar, Dialogues::dMenubar);
        }

        if (ImGui::BeginMenuBar()) {
            if (DefaultAssets.contains("Game_Icon")) {
                ImGui::SetCursorPos(ImVec2(5, 2.5f));
#ifdef ENABLE_DX11
                ImGui::Image(impl.backend == Backend::DX11 ? GetTextureByID(DefaultAssets["Game_Icon"]->textureId) : reinterpret_cast<ImTextureID>(DefaultAssets["Game_Icon"]->textureId), ImVec2(16.0f, 16.0f));
#else
                ImGui::Image(reinterpret_cast<ImTextureID>(DefaultAssets["Game_Icon"]->textureId), ImVec2(16.0f, 16.0f));
#endif
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(25, 0));
            }
            ImGui::Text("Shipwright");
            ImGui::Separator();

            if (ImGui::BeginMenu("Audio")) {
                const float volume = Game::Settings.audio.master;
                ImGui::Text("Master Volume: %d %%", static_cast<int>(100 * volume));
                if (ImGui::SliderFloat("##Master_Vol", &Game::Settings.audio.master, 0.0f, 1.0f, "")) {
                    CVar_SetFloat("gGameMasterVolume", volume);
                    needs_save = true;
                }

                BindAudioSlider("Main Music Volume: %d %%", "gMainMusicVolume", &Game::Settings.audio.music_main, SEQ_BGM_MAIN);
                BindAudioSlider("Sub Music Volume: %d %%", "gSubMusicVolume", &Game::Settings.audio.music_sub, SEQ_BGM_SUB);
                BindAudioSlider("Sound Effects Volume: %d %%", "gSFXMusicVolume", &Game::Settings.audio.sfx, SEQ_SFX);
                BindAudioSlider("Fanfare Volume: %d %%", "gFanfareVolume", &Game::Settings.audio.fanfare, SEQ_FANFARE);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Controller")) {
                ImGui::Text("Gyro Sensitivity: %d %%", static_cast<int>(100 * Game::Settings.controller.gyro_sensitivity));
                if (ImGui::SliderFloat("##GYROSCOPE", &Game::Settings.controller.gyro_sensitivity, 0.0f, 1.0f, "")) {
                    needs_save = true;
                }

                if (ImGui::Button("Recalibrate Gyro")) {
                    Game::Settings.controller.gyroDriftX = 0;
                    Game::Settings.controller.gyroDriftY = 0;
                }

                ImGui::Separator();

                ImGui::Text("Rumble Strength: %d %%", static_cast<int>(100 * Game::Settings.controller.rumble_strength));
                if (ImGui::SliderFloat("##RUMBLE", &Game::Settings.controller.rumble_strength, 0.0f, 1.0f, "")) {
                    needs_save = true;
                }

                if (ImGui::Checkbox("Show Inputs", &Game::Settings.controller.input_enabled)) {
                    needs_save = true;
                }

                ImGui::Text("Input Scale: %.1f", Game::Settings.controller.input_scale);
                if (ImGui::SliderFloat("##Input", &Game::Settings.controller.input_scale, 1.0f, 3.0f, "")) {
                    needs_save = true;
                }

                ImGui::Separator();

                if (ImGui::Checkbox("Dpad Support on Pause and File Select", &Game::Settings.controller.dpad_pause_name)) {
                    CVar_SetS32("gDpadPauseName", Game::Settings.controller.dpad_pause_name);
                    needs_save = true;
                }

                if (ImGui::Checkbox("DPad Support in Ocarina and Text Choice", &Game::Settings.controller.dpad_ocarina_text)) {
                    CVar_SetS32("gDpadOcarinaText", Game::Settings.controller.dpad_ocarina_text);
                    needs_save = true;
                }

                if (ImGui::Checkbox("DPad Support for Browsing Shop Items", &Game::Settings.controller.dpad_shop)) {
                    CVar_SetS32("gDpadShop", Game::Settings.controller.dpad_shop);
                    needs_save = true;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Enhancements")) {

                ImGui::Text("Gameplay");
                ImGui::Separator();

                if (ImGui::Checkbox("Fast Text", &Game::Settings.enhancements.fast_text)) {
                    CVar_SetS32("gFastText", Game::Settings.enhancements.fast_text);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Minimal UI", &Game::Settings.enhancements.minimal_ui)) {
                    CVar_SetS32("gMinimalUI", Game::Settings.enhancements.minimal_ui);
                    needs_save = true;
                }

                if (ImGui::Checkbox("MM Bunny Hood", &Game::Settings.enhancements.mm_bunny_hood)) {
                    CVar_SetS32("gMMBunnyHood", Game::Settings.enhancements.mm_bunny_hood);
                    needs_save = true;
                }

                /*if (ImGui::Checkbox("Fix L&R Pause menu", &Game::Settings.enhancements.uniform_lr)) {
                    CVar_SetS32("gUniformLR", Game::Settings.enhancements.uniform_lr);
                    needs_save = true;
                } */

                ImGui::Text("Graphics");
                ImGui::Separator();

                HOOK(ImGui::Checkbox("N64 Mode", &Game::Settings.debug.n64mode));

                if (ImGui::Checkbox("Animated Link in Pause Menu", &Game::Settings.enhancements.animated_pause_menu)) {
                    CVar_SetS32("gPauseLiveLink", Game::Settings.enhancements.animated_pause_menu);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Disable LOD", &Game::Settings.enhancements.disable_lod)) {
                    CVar_SetS32("gDisableLOD", Game::Settings.enhancements.disable_lod);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Dynamic Wallet Icon", &Game::Settings.enhancements.dynamic_wallet_icon)) {
                    CVar_SetS32("gDynamicWalletIcon", Game::Settings.enhancements.dynamic_wallet_icon);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Enable 3D Dropped items", &Game::Settings.enhancements.newdrops)) {
                    CVar_SetS32("gNewDrops", Game::Settings.enhancements.newdrops);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Enable Visual/Audio Stone of Agony", &Game::Settings.enhancements.visualagony)) {
                    CVar_SetS32("gVisualAgony", Game::Settings.enhancements.visualagony);
                    needs_save = true;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Developer Tools")) {
                HOOK(ImGui::MenuItem("Stats", nullptr, &Game::Settings.debug.soh));
                HOOK(ImGui::MenuItem("Console", nullptr, &console->opened));

                ImGui::Text("Debug");
                ImGui::Separator();

                if (ImGui::Checkbox("Debug Mode", &Game::Settings.cheats.debug_mode)) {
                    CVar_SetS32("gDebugEnabled", Game::Settings.cheats.debug_mode);
                    needs_save = true;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Cheats")) {
                if (ImGui::Checkbox("Infinite Money", &Game::Settings.cheats.infinite_money)) {
                    CVar_SetS32("gInfiniteMoney", Game::Settings.cheats.infinite_money);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Infinite Health", &Game::Settings.cheats.infinite_health)) {
                    CVar_SetS32("gInfiniteHealth", Game::Settings.cheats.infinite_health);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Infinite Ammo", &Game::Settings.cheats.infinite_ammo)) {
                    CVar_SetS32("gInfiniteAmmo", Game::Settings.cheats.infinite_ammo);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Infinite Magic", &Game::Settings.cheats.infinite_magic)) {
                    CVar_SetS32("gInfiniteMagic", Game::Settings.cheats.infinite_magic);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Nayru's Love", &Game::Settings.cheats.infinite_nayru)) {
                    CVar_SetS32("gInfiniteNayru", Game::Settings.cheats.infinite_nayru);
                    needs_save = true;
                }

                if (ImGui::Checkbox("No Clip", &Game::Settings.cheats.no_clip)) {
                    CVar_SetS32("gNoClip", Game::Settings.cheats.no_clip);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Climb Everything", &Game::Settings.cheats.climb_everything)) {
                    CVar_SetS32("gClimbEverything", Game::Settings.cheats.climb_everything);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Moon Jump on L", &Game::Settings.cheats.moon_jump_on_l)) {
                    CVar_SetS32("gMoonJumpOnL", Game::Settings.cheats.moon_jump_on_l);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Super Tunic", &Game::Settings.cheats.super_tunic)) {
                    CVar_SetS32("gSuperTunic", Game::Settings.cheats.super_tunic);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Easy ISG", &Game::Settings.cheats.ez_isg)) {
                    CVar_SetS32("gEzISG", Game::Settings.cheats.ez_isg);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Unrestricted Items", &Game::Settings.cheats.no_restrict_item)) {
                    CVar_SetS32("gNoRestrictItems", Game::Settings.cheats.no_restrict_item);
                    needs_save = true;
                }

                if (ImGui::Checkbox("Freeze Time", &Game::Settings.cheats.freeze_time)) {
                    CVar_SetS32("gFreezeTime", Game::Settings.cheats.freeze_time);
                    needs_save = true;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Graphics")) {
                HOOK(ImGui::MenuItem("Anti-aliasing", nullptr, &Game::Settings.graphics.show));
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("HUD Colors")) {
                if (ImGui::RadioButton("N64 Colors", CVar_GetS32("gN64Colors", 1))) {
                    Game::Settings.hudcolors.n64_colors = 1;
                    Game::Settings.hudcolors.gc_colors = 0;
                    Game::Settings.hudcolors.custom_colors = 0;
                    CVar_SetS32("gN64Colors", 1);
                    CVar_SetS32("gGameCubeColors", 0);
                    CVar_SetS32("gCustomColors", 0);
                    needs_save = true;
                }
                if (ImGui::RadioButton("Gamecube Colors", CVar_GetS32("gGameCubeColors", 0))) {
                    Game::Settings.hudcolors.n64_colors = 0;
                    Game::Settings.hudcolors.gc_colors = 1;
                    Game::Settings.hudcolors.custom_colors = 0;
                    CVar_SetS32("gN64Colors", 0);
                    CVar_SetS32("gGameCubeColors", 1);
                    CVar_SetS32("gCustomColors", 0);
                    needs_save = true;
                }
                if (ImGui::RadioButton("Custom Colors", CVar_GetS32("gCustomColors", 0))) {
                    Game::Settings.hudcolors.n64_colors = 0;
                    Game::Settings.hudcolors.gc_colors = 0;
                    Game::Settings.hudcolors.custom_colors = 1;
                    CVar_SetS32("gN64Colors", 0);
                    CVar_SetS32("gGameCubeColors", 0);
                    CVar_SetS32("gCustomColors", 1);
                    needs_save = true;
                }
                bool OpenColorEdit = ImGui::SmallButton("Edit HUD Colors");
                bool Hearts_col=false;
                bool Hearts_dd_col=false;
                bool Buttons_col=false;
                bool Magic_col=false;
                bool Minimap_col=false;
                bool Rupee_col=false;
                if (OpenColorEdit) {
                     ImGui::OpenPopup("CustomColors");
                }
                if (ImGui::BeginPopup("CustomColors")){
                    ImGui::Text("Edit custom HUD colors");
                    ImGui::Separator();
                    Hearts_col = ImGui::SmallButton("Hearts colors");
                    ImGui::Separator();
                    Hearts_dd_col = ImGui::SmallButton("Hearts double defense colors");
                    ImGui::Separator();
                    Buttons_col = ImGui::SmallButton("Buttons");
                    ImGui::Separator();
                    Magic_col = ImGui::SmallButton("Magic Bar");
                    ImGui::Separator();
                    Minimap_col = ImGui::SmallButton("Minimap");
                    ImGui::Separator();
                    Rupee_col = ImGui::SmallButton("Rupee icon");
                    ImGui::EndPopup();
                }
                if (Hearts_col) { ImGui::OpenPopup("Hearts_col"); }
                if (Hearts_dd_col) { ImGui::OpenPopup("Hearts_dd_col"); }
                if (Buttons_col) { ImGui::OpenPopup("Buttons_col"); }
                if (Magic_col) { ImGui::OpenPopup("Magic_col"); }
                if (Minimap_col) { ImGui::OpenPopup("Minimap_col"); }
                if (Rupee_col) { ImGui::OpenPopup("Rupee_col"); }
                if (ImGui::BeginPopup("Hearts_col")){
                    if (ImGui::ColorEdit3("Hearts", (float*)&hearts_colors)) {
                        Game::Settings.hudcolors.ccheartsprimr = ClampFloatToInt(hearts_colors[0]*255,0,255);
                        Game::Settings.hudcolors.ccheartsprimg = ClampFloatToInt(hearts_colors[1]*255,0,255);
                        Game::Settings.hudcolors.ccheartsprimb = ClampFloatToInt(hearts_colors[2]*255,0,255);
                        CVar_SetInt("gCCHeartsPrimR", Game::Settings.hudcolors.ccheartsprimr);
                        CVar_SetInt("gCCHeartsPrimG", Game::Settings.hudcolors.ccheartsprimg);
                        CVar_SetInt("gCCHeartsPrimB", Game::Settings.hudcolors.ccheartsprimb);
                        needs_save = true;
                    }
                    ImGui::EndPopup();
                }
                if (ImGui::BeginPopup("Hearts_dd_col")){
                    if (ImGui::ColorEdit3("Hearts", hearts_dd_colors)) {
                        Game::Settings.hudcolors.ddccheartsprimr = ClampFloatToInt(hearts_dd_colors[0]*255,0,255);
                        Game::Settings.hudcolors.ddccheartsprimg = ClampFloatToInt(hearts_dd_colors[1]*255,0,255);
                        Game::Settings.hudcolors.ddccheartsprimb = ClampFloatToInt(hearts_dd_colors[2]*255,0,255);
                        CVar_SetInt("gDDCCHeartsPrimR", Game::Settings.hudcolors.ddccheartsprimr);
                        CVar_SetInt("gDDCCHeartsPrimG", Game::Settings.hudcolors.ddccheartsprimg);
                        CVar_SetInt("gDDCCHeartsPrimB", Game::Settings.hudcolors.ddccheartsprimb);
                        needs_save = true;
                    }
                    ImGui::EndPopup();
                }
                if (ImGui::BeginPopup("Buttons_col")){
                    if (ImGui::ColorEdit3("A Buttons", a_btn_colors)) {
                        Game::Settings.hudcolors.ccabtnprimr = ClampFloatToInt(a_btn_colors[0]*255,0,255);
                        Game::Settings.hudcolors.ccabtnprimg = ClampFloatToInt(a_btn_colors[1]*255,0,255);
                        Game::Settings.hudcolors.ccabtnprimb = ClampFloatToInt(a_btn_colors[2]*255,0,255);
                        CVar_SetInt("gCCABtnPrimR", Game::Settings.hudcolors.ccabtnprimr);
                        CVar_SetInt("gCCABtnPrimG", Game::Settings.hudcolors.ccabtnprimg);
                        CVar_SetInt("gCCABtnPrimB", Game::Settings.hudcolors.ccabtnprimb);
                        needs_save = true;
                    }
                    if (ImGui::ColorEdit3("B Buttons", b_btn_colors)) {
                        Game::Settings.hudcolors.ccbbtnprimr = ClampFloatToInt(b_btn_colors[0]*255,0,255);
                        Game::Settings.hudcolors.ccbbtnprimg = ClampFloatToInt(b_btn_colors[1]*255,0,255);
                        Game::Settings.hudcolors.ccbbtnprimb = ClampFloatToInt(b_btn_colors[2]*255,0,255);
                        CVar_SetInt("gCCBBtnPrimR", Game::Settings.hudcolors.ccbbtnprimr);
                        CVar_SetInt("gCCBBtnPrimG", Game::Settings.hudcolors.ccbbtnprimg);
                        CVar_SetInt("gCCBBtnPrimB", Game::Settings.hudcolors.ccbbtnprimb);
                        needs_save = true;
                    }
                    if (ImGui::ColorEdit3("C Buttons", c_btn_colors)) {
                        Game::Settings.hudcolors.cccbtnprimr = ClampFloatToInt(c_btn_colors[0]*255,0,255);
                        Game::Settings.hudcolors.cccbtnprimg = ClampFloatToInt(c_btn_colors[1]*255,0,255);
                        Game::Settings.hudcolors.cccbtnprimb = ClampFloatToInt(c_btn_colors[2]*255,0,255);
                        CVar_SetInt("gCCCBtnPrimR", Game::Settings.hudcolors.cccbtnprimr);
                        CVar_SetInt("gCCCBtnPrimG", Game::Settings.hudcolors.cccbtnprimg);
                        CVar_SetInt("gCCCBtnPrimB", Game::Settings.hudcolors.cccbtnprimb);
                        needs_save = true;
                    }
                    if (ImGui::ColorEdit3("Start Button", start_btn_colors)) {
                        Game::Settings.hudcolors.ccstartbtnprimr = ClampFloatToInt(start_btn_colors[0]*255,0,255);
                        Game::Settings.hudcolors.ccstartbtnprimg = ClampFloatToInt(start_btn_colors[1]*255,0,255);
                        Game::Settings.hudcolors.ccstartbtnprimb = ClampFloatToInt(start_btn_colors[2]*255,0,255);
                        CVar_SetInt("gCCStartBtnPrimR", Game::Settings.hudcolors.ccstartbtnprimr);
                        CVar_SetInt("gCCStartBtnPrimG", Game::Settings.hudcolors.ccstartbtnprimg);
                        CVar_SetInt("gCCStartBtnPrimB", Game::Settings.hudcolors.ccstartbtnprimb);
                        needs_save = true;
                    }
                    ImGui::EndPopup();
                }
                if (ImGui::BeginPopup("Magic_col")){
                    if (ImGui::ColorEdit3("Magic bar borders", magic_border_colors)) {
                        Game::Settings.hudcolors.ccmagicborderprimr = ClampFloatToInt(magic_border_colors[0]*255,0,255);
                        Game::Settings.hudcolors.ccmagicborderprimg = ClampFloatToInt(magic_border_colors[1]*255,0,255);
                        Game::Settings.hudcolors.ccmagicborderprimb = ClampFloatToInt(magic_border_colors[2]*255,0,255);
                        CVar_SetInt("gCCMagicBorderPrimR", Game::Settings.hudcolors.ccmagicborderprimr);
                        CVar_SetInt("gCCMagicBorderPrimG", Game::Settings.hudcolors.ccmagicborderprimg);
                        CVar_SetInt("gCCMagicBorderPrimB", Game::Settings.hudcolors.ccmagicborderprimb);
                        needs_save = true;
                    }
                    if (ImGui::ColorEdit3("Magic bar main color", magic_remaining_colors)) {
                        Game::Settings.hudcolors.ccmagicprimr = ClampFloatToInt(magic_remaining_colors[0]*255,0,255);
                        Game::Settings.hudcolors.ccmagicprimg = ClampFloatToInt(magic_remaining_colors[1]*255,0,255);
                        Game::Settings.hudcolors.ccmagicprimb = ClampFloatToInt(magic_remaining_colors[2]*255,0,255);
                        CVar_SetInt("gCCMagicPrimR", Game::Settings.hudcolors.ccmagicprimr);
                        CVar_SetInt("gCCMagicPrimG", Game::Settings.hudcolors.ccmagicprimg);
                        CVar_SetInt("gCCMagicPrimB", Game::Settings.hudcolors.ccmagicprimb);
                        needs_save = true;
                    }
                    if (ImGui::ColorEdit3("Magic bar being used", magic_use_colors)) {
                        Game::Settings.hudcolors.ccmagicuseprimr = ClampFloatToInt(magic_use_colors[0]*255,0,255);
                        Game::Settings.hudcolors.ccmagicuseprimg = ClampFloatToInt(magic_use_colors[1]*255,0,255);
                        Game::Settings.hudcolors.ccmagicuseprimb = ClampFloatToInt(magic_use_colors[2]*255,0,255);
                        CVar_SetInt("CCMagicUsePrimR", Game::Settings.hudcolors.ccmagicuseprimr);
                        CVar_SetInt("CCMagicUsePrimG", Game::Settings.hudcolors.ccmagicuseprimg);
                        CVar_SetInt("CCMagicUsePrimB", Game::Settings.hudcolors.ccmagicuseprimb);
                        needs_save = true;
                    }
                    ImGui::EndPopup();
                }
                if (ImGui::BeginPopup("Minimap_col")){
                    if (ImGui::ColorEdit3("Minimap color", minimap_colors)) {
                        Game::Settings.hudcolors.ccminimapprimr = ClampFloatToInt(minimap_colors[0]*255,0,255);
                        Game::Settings.hudcolors.ccminimapprimg = ClampFloatToInt(minimap_colors[1]*255,0,255);
                        Game::Settings.hudcolors.ccminimapprimb = ClampFloatToInt(minimap_colors[2]*255,0,255);
                        CVar_SetInt("gCCMinimapPrimR", Game::Settings.hudcolors.ccminimapprimr);
                        CVar_SetInt("gCCMinimapPrimG", Game::Settings.hudcolors.ccminimapprimg);
                        CVar_SetInt("gCCMinimapPrimB", Game::Settings.hudcolors.ccminimapprimb);
                        needs_save = true;
                    }
                    ImGui::EndPopup();
                }
                if (ImGui::BeginPopup("Rupee_col")){
                    if (ImGui::ColorEdit3("Rupee icon color", rupee_colors)) {
                        Game::Settings.hudcolors.ccrupeeprimr = ClampFloatToInt(rupee_colors[0]*255,0,255);
                        Game::Settings.hudcolors.ccrupeeprimg = ClampFloatToInt(rupee_colors[1]*255,0,255);
                        Game::Settings.hudcolors.ccrupeeprimb = ClampFloatToInt(rupee_colors[2]*255,0,255);
                        CVar_SetInt("gCCRupeePrimR", Game::Settings.hudcolors.ccrupeeprimr);
                        CVar_SetInt("gCCRupeePrimG", Game::Settings.hudcolors.ccrupeeprimg);
                        CVar_SetInt("gCCRupeePrimB", Game::Settings.hudcolors.ccrupeeprimb);
                        needs_save = true;
                    }
                    ImGui::EndPopup();
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Languages")) {
                if (ImGui::RadioButton("English", CVar_GetS32("gSetENG", 1))) {
                    Game::Settings.languages.set_eng = 1;
                    Game::Settings.languages.set_fra = 0;
                    Game::Settings.languages.set_ger = 0;
                    CVar_SetS32("gSetENG", 1);
                    CVar_SetS32("gSetFRA", 0);
                    CVar_SetS32("gSetGER", 0);
                    needs_save = true;
                }
                  if (ImGui::RadioButton("German", CVar_GetS32("gSetGER", 0))) {
                    Game::Settings.languages.set_eng = 0;
                    Game::Settings.languages.set_fra = 0;
                    Game::Settings.languages.set_ger = 1;
                    CVar_SetS32("gSetENG", 0);
                    CVar_SetS32("gSetFRA", 0);
                    CVar_SetS32("gSetGER", 1);
                    needs_save = true;
                }
                if (ImGui::RadioButton("French", CVar_GetS32("gSetFRA", 0))) {
                    Game::Settings.languages.set_eng = 0;
                    Game::Settings.languages.set_fra = 1;
                    Game::Settings.languages.set_ger = 0;
                    CVar_SetS32("gSetENG", 0);
                    CVar_SetS32("gSetFRA", 1);
                    CVar_SetS32("gSetGER", 0);
                    needs_save = true;
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();

        if (Game::Settings.graphics.show) {
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
            ImGui::Begin("Anti-aliasing settings", nullptr, ImGuiWindowFlags_None);
            ImGui::Text("Internal Resolution:");
            ImGui::SliderInt("Mul", reinterpret_cast<int*>(&gfx_current_dimensions.internal_mul), 1, 8);
            ImGui::Text("MSAA:");
            ImGui::SliderInt("MSAA", reinterpret_cast<int*>(&gfx_msaa_level), 1, 8);
            ImGui::End();
            ImGui::PopStyleColor();
        }

        console->Draw();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
        ImGui::Begin("OoT Master Quest", nullptr, flags);
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor();

        ImVec2 main_pos = ImGui::GetWindowPos();
        main_pos.x -= top_left_pos.x;
        main_pos.y -= top_left_pos.y;
        ImVec2 size = ImGui::GetContentRegionAvail();
        ImVec2 pos = ImVec2(0, 0);
        gfx_current_dimensions.width = size.x * gfx_current_dimensions.internal_mul;
        gfx_current_dimensions.height = size.y * gfx_current_dimensions.internal_mul;
        gfx_current_game_window_viewport.x = main_pos.x;
        gfx_current_game_window_viewport.y = main_pos.y;
        gfx_current_game_window_viewport.width = size.x;
        gfx_current_game_window_viewport.height = size.y;
        if (Game::Settings.debug.n64mode) {
            gfx_current_dimensions.width = 320;
            gfx_current_dimensions.height = 240;
            const int sw = size.y * 320 / 240;
            gfx_current_game_window_viewport.x += (size.x - sw) / 2;
            gfx_current_game_window_viewport.width = sw;
            pos = ImVec2(size.x / 2 - sw / 2, 0);
            size = ImVec2(sw, size.y);
        }
    }

    void DrawFramebufferAndGameInput() {
         ImVec2 main_pos = ImGui::GetWindowPos();
         ImVec2 size = ImGui::GetContentRegionAvail();
         ImVec2 pos = ImVec2(0, 0);
         if (Game::Settings.debug.n64mode) {
             const int sw = size.y * 320 / 240;
             pos = ImVec2(size.x / 2 - sw / 2, 0);
             size = ImVec2(sw, size.y);
         }
         std::string fb_str = SohUtils::getEnvironmentVar("framebuffer");
         if (!fb_str.empty()) {
             uintptr_t fbuf = (uintptr_t)std::stoull(fb_str);
             //ImGui::ImageSimple(reinterpret_cast<ImTextureID>(fbuf), pos, size);
             ImGui::SetCursorPos(pos);
             ImGui::Image(reinterpret_cast<ImTextureID>(fbuf), size);
         }

         ImGui::End();

         const float scale = Game::Settings.controller.input_scale;
         ImVec2 BtnPos = ImVec2(160 * scale, 85 * scale);

         if (Game::Settings.controller.input_enabled) {
             ImGui::SetNextWindowSize(BtnPos);
             ImGui::SetNextWindowPos(ImVec2(main_pos.x + size.x - BtnPos.x - 20, main_pos.y + size.y - BtnPos.y - 20));

             if (pads != nullptr && ImGui::Begin("Game Buttons", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground)) {
                 ImGui::SetCursorPosY(32 * scale);

                 ImGui::BeginGroup();
                 const ImVec2 cPos = ImGui::GetCursorPos();
                 ImGui::SetCursorPos(ImVec2(cPos.x + 10 * scale, cPos.y - 20 * scale));
                 BindButton("L-Btn", pads[0].button & BTN_L);
                 ImGui::SetCursorPos(ImVec2(cPos.x + 16 * scale, cPos.y));
                 BindButton("C-Up", pads[0].button & BTN_CUP);
                 ImGui::SetCursorPos(ImVec2(cPos.x, cPos.y + 16 * scale));
                 BindButton("C-Left", pads[0].button & BTN_CLEFT);
                 ImGui::SetCursorPos(ImVec2(cPos.x + 32 * scale, cPos.y + 16 * scale));
                 BindButton("C-Right", pads[0].button & BTN_CRIGHT);
                 ImGui::SetCursorPos(ImVec2(cPos.x + 16 * scale, cPos.y + 32 * scale));
                 BindButton("C-Down", pads[0].button & BTN_CDOWN);
                 ImGui::EndGroup();

                 ImGui::SameLine();

                 ImGui::BeginGroup();
                 const ImVec2 sPos = ImGui::GetCursorPos();
                 ImGui::SetCursorPos(ImVec2(sPos.x + 21, sPos.y - 20 * scale));
                 BindButton("Z-Btn", pads[0].button & BTN_Z);
                 ImGui::SetCursorPos(ImVec2(sPos.x + 22, sPos.y + 16 * scale));
                 BindButton("Start-Btn", pads[0].button & BTN_START);
                 ImGui::EndGroup();

                 ImGui::SameLine();

                 ImGui::BeginGroup();
                 const ImVec2 bPos = ImGui::GetCursorPos();
                 ImGui::SetCursorPos(ImVec2(bPos.x + 20 * scale, bPos.y - 20 * scale));
                 BindButton("R-Btn", pads[0].button & BTN_R);
                 ImGui::SetCursorPos(ImVec2(bPos.x + 12 * scale, bPos.y + 8 * scale));
                 BindButton("B-Btn", pads[0].button & BTN_B);
                 ImGui::SetCursorPos(ImVec2(bPos.x + 28 * scale, bPos.y + 24 * scale));
                 BindButton("A-Btn", pads[0].button & BTN_A);
                 ImGui::EndGroup();

                 ImGui::End();
             }
         }
     }

    void Render() {
        console->Draw();

        ImGui::Render();
        ImGuiRenderDrawData(ImGui::GetDrawData());
        if (UseViewports()) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void CancelFrame() {
        ImGui::EndFrame();
        if (UseViewports()) {
            ImGui::UpdatePlatformWindows();
        }
    }

    void BindCmd(const std::string& cmd, CommandEntry entry) {
        console->Commands[cmd] = std::move(entry);
    }
}
