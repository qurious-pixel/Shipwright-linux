#pragma once

struct SoHConfigType {
    // Debug
    struct {
        bool soh = false;
        bool menu_bar = false;
        bool soh_sink = true;
        //bool n64mode = false;
        //bool buildinfos = false;
    } debug;

    /*
    // Audio
    struct {
        float master = 1.0f;
        float music_main = 1.0f;
        float fanfare = 1.0f;
        float sfx = 1.0f;
        float music_sub = 1.0f;
    } audio;

    // Enhancements
    struct {
        int text_speed = 1;
        bool skip_text = false;
        bool disable_lod = false;
        bool animated_pause_menu = false;
        bool minimal_ui = false;
        bool reworked_controls = false;
        bool newdrops = false;
        bool dynamic_wallet_icon = false;
        bool mm_bunny_hood = false;
        bool uniform_lr = true;
        bool visualagony = false;
    } enhancements;*/

    // Controller
    struct {
        //float gyro_sensitivity = 1.0f;
        //float rumble_strength = 1.0f;
        //float input_scale = 1.0f;
        float gyroDriftX = 0.0f;
        float gyroDriftY = 0.0f;
        //bool input_enabled = false;
        //bool dpad_pause_name = false;
        //bool dpad_ocarina_text = false;
        //bool dpad_shop = false;
    } controller;

    /*
    // Cheats
    struct {
        bool debug_mode = false;
        bool infinite_money = false;
        bool infinite_health = false;
        bool infinite_ammo = false;
        bool infinite_magic = false;
        bool infinite_nayru = false;
        bool no_clip = false;
        bool climb_everything = false;
        bool moon_jump_on_l = false;
        bool super_tunic = false;
        bool ez_isg = false;
        bool no_restrict_item = false;
        bool freeze_time = false;
    } cheats;
    */
    // Graphics
    struct {
        bool show = false;
    } graphics;
    /*
    // HUD Colors
    struct {
        int hudcolor = 1;
        //hearts colors
        int ccheartsprimr = 255;
        int ccheartsprimg = 70;
        int ccheartsprimb = 50;
        //hearts double defense colors
        int ddccheartsprimr = 255;
        int ddccheartsprimg = 255;
        int ddccheartsprimb = 255;
        //A Button Main colors
        int ccabtnprimr = 90;
        int ccabtnprimg = 90;
        int ccabtnprimb = 255;
        //B Button Main colors
        int ccbbtnprimr = 0;
        int ccbbtnprimg = 150;
        int ccbbtnprimb = 0;
        //C Button Main colors
        int cccbtnprimr = 255;
        int cccbtnprimg = 160;
        int cccbtnprimb = 0;
        //Start Button Main colors
        int ccstartbtnprimr = 200;
        int ccstartbtnprimg = 0;
        int ccstartbtnprimb = 0;
        //Magic Border colors
        int ccmagicborderprimr = 255;
        int ccmagicborderprimg = 255;
        int ccmagicborderprimb = 255;
        //Magic Main colors
        int ccmagicprimr = 10;
        int ccmagicprimg = 255;
        int ccmagicprimb = 10;
        //Magic Being used colors
        int ccmagicuseprimr = 250;
        int ccmagicuseprimg = 250;
        int ccmagicuseprimb = 0;
        //Minimap
        int ccminimapprimr = 10;
        int ccminimapprimg = 10;
        int ccminimapprimb = 180;
        //Rupee icon
        int ccrupeeprimr = 10;
        int ccrupeeprimg = 220;
        int ccrupeeprimb = 10;

        int tunic_kokiri_r = 30;
        int tunic_kokiri_g = 105;
        int tunic_kokiri_b = 27;
        int tunic_goron_r = 100;
        int tunic_goron_g = 20;
        int tunic_goron_b = 0;
        int tunic_zora_r = 0;
        int tunic_zora_g = 60;
        int tunic_zora_b = 100;

        int navi_idle_inner_r = 255;
        int navi_idle_inner_g = 255;
        int navi_idle_inner_b = 255;
        int navi_idle_outer_r = 0;
        int navi_idle_outer_g = 0;
        int navi_idle_outer_b = 255;

        int navi_enemy_inner_r = 255;
        int navi_enemy_inner_g = 255;
        int navi_enemy_inner_b = 0;
        int navi_enemy_outer_r = 200;
        int navi_enemy_outer_g = 155;
        int navi_enemy_outer_b = 0;

        int navi_npc_inner_r = 150;
        int navi_npc_inner_g = 150;
        int navi_npc_inner_b = 255;
        int navi_npc_outer_r = 150;
        int navi_npc_outer_g = 150;
        int navi_npc_outer_b = 255;

        int navi_prop_inner_r = 0;
        int navi_prop_inner_g = 250;
        int navi_prop_inner_b = 0;
        int navi_prop_outer_r = 0;
        int navi_prop_outer_g = 250;
        int navi_prop_outer_b = 0;

    } cosmetic;


    // Languages
    struct {
        int sellanguages = 0;
    } languages;*/
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
