#pragma once

struct SoHConfigType {
    // Debug
    struct {
        bool soh = false;
        bool menu_bar = false;
        bool soh_sink = true;
    } debug;

    // Controller
    struct {
        float gyroDriftX = 0.0f;
        float gyroDriftY = 0.0f;
    } controller;

    struct {
        bool show = false;
    } graphics;
};

enum SeqPlayers {
    /* 0 */ SEQ_BGM_MAIN,
    /* 1 */ SEQ_FANFARE,
    /* 2 */ SEQ_SFX,
    /* 3 */ SEQ_BGM_SUB,
    /* 4 */ SEQ_MAX
 };

#define DEBUG_SECTION "DEBUG SETTINGS"
#define AUDIO_SECTION "AUDIO SETTINGS"
#define CONTROLLER_SECTION "CONTROLLER SECTION"
#define ENHANCEMENTS_SECTION "ENHANCEMENT SETTINGS"
#define CHEATS_SECTION "CHEATS SETTINGS"
#define COSMETICS_SECTION "COSMETIC SETTINGS"
#define LANGUAGES_SECTION "LANGUAGES SETTINGS"

namespace Game {
    extern SoHConfigType Settings;
    void InitSettings();
    void LoadSettings();
    void SaveSettings();
    void SetSeqPlayerVolume(SeqPlayers playerId, float volume);
}
