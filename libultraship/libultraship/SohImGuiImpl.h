#pragma once

#include "SohConsole.h"

struct GameAsset {
    uint32_t textureId;
    int width;
    int height;
};

namespace SohImGui {
    enum class Backend {
        DX11,
        SDL
    };

    enum class Dialogues {
        dConsole,
        dMenubar,
        dLoadSettings,
    };

    typedef struct {
        Backend backend;
        union {
            struct {
                void* window;
                void* device_context;
                void* device;
            } dx11;
            struct {
                void* window;
                void* context;
            } sdl;
        };
    } WindowImpl;

    typedef union {
        struct {
            void* handle;
            int msg;
            int wparam;
            int lparam;
        } win32;
        struct {
            void* event;
        } sdl;
    } EventImpl;

    extern Console* console;
    void Init(WindowImpl window_impl);
    void Update(EventImpl event);
    void EnhancementColorEdit3(std::string text, std::string cvarName, float ColorRGB[3]);
    void EnhancementRadioButton(std::string text, std::string cvarName, int value);
    void EnhancementCheckbox(std::string text, std::string cvarName);
    void EnhancementSliderInt(std::string text, std::string id, std::string cvarName, int min, int max, std::string format);
    void EnhancementSliderFloat(std::string text, std::string id, std::string cvarName, float min, float max, std::string format, float defaultValue);
    int ClampFloatToInt(float value, int min, int max);
    void DrawMainMenuAndCalculateGameSize(void);
    void DrawFramebufferAndGameInput(void);
    void Render(void);
    void CancelFrame(void);
    void ShowCursor(bool hide, Dialogues w);
    void BindCmd(const std::string& cmd, CommandEntry entry);
    void* GetTextureByID(int id);
    void LoadCosmeticColors();
}
