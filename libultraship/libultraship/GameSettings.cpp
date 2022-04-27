#include "GameSettings.h"

// Audio
#include <cstddef>
#include <PR/ultra64/types.h>
#include <PR/ultra64/sptask.h>
#include <PR/ultra64/pi.h>
#include <PR/ultra64/message.h>

#include "ConfigFile.h"
#include "Cvar.h"
#include "GlobalCtx2.h"
#include "SohImGuiImpl.h"
#include "stox.h"
#include "../../soh/include/z64audio.h"
#include <string>
#include "SohHooks.h"
#include "../../soh/soh/Enhancements/debugconsole.h"

#define ABS(var) var < 0 ? -(var) : var

using namespace Ship;

namespace Game {
    bool DeSyncAudio = false;
    SoHConfigType Settings;
    const std::string ConfSection = DEBUG_SECTION;
    const std::string AudioSection = AUDIO_SECTION;
    const std::string ControllerSection = CONTROLLER_SECTION;
    const std::string EnhancementSection = ENHANCEMENTS_SECTION;
    const std::string CheatSection = CHEATS_SECTION;
    const std::string CosmeticsSection = COSMETICS_SECTION;
    const std::string LanguagesSection = LANGUAGES_SECTION;

    void UpdateAudio() {
        Audio_SetGameVolume(SEQ_BGM_MAIN, CVar_GetFloat("gMainMusicVolume", 1));
        Audio_SetGameVolume(SEQ_BGM_SUB, CVar_GetFloat("gSubMusicVolume", 1));
        Audio_SetGameVolume(SEQ_FANFARE, CVar_GetFloat("gSFXMusicVolume", 1));
        Audio_SetGameVolume(SEQ_SFX, CVar_GetFloat("gFanfareVolume", 1));
    }

    void LoadSettings() {

        const std::shared_ptr<ConfigFile> pConf = GlobalCtx2::GetInstance()->GetConfig();
        ConfigFile& Conf = *pConf;

        // Debug
        SohImGui::console->opened = stob(Conf[ConfSection]["console"]);
        Settings.debug.menu_bar = stob(Conf[ConfSection]["menu_bar"]);
        Settings.debug.soh = stob(Conf[ConfSection]["soh_debug"]);
    	//Settings.debug.n64mode = stob(Conf[ConfSection]["n64_mode"]);
        //Settings.debug.buildinfos = stob(Conf[ConfSection]["hide_buildinfos"]);
        /*CVar_SetS32("gBuildInfos", Settings.debug.buildinfos);

        // Enhancements
        Settings.enhancements.skip_text = stob(Conf[EnhancementSection]["skip_text"]);
        CVar_SetS32("gSkipText", Settings.enhancements.skip_text);

        Settings.enhancements.text_speed = Ship::stoi(Conf[EnhancementSection]["text_speed"]);
        CVar_SetS32("gTextSpeed", Settings.enhancements.text_speed);

        Settings.enhancements.disable_lod = stob(Conf[EnhancementSection]["disable_lod"]);
        CVar_SetS32("gDisableLOD", Settings.enhancements.disable_lod);

        Settings.enhancements.animated_pause_menu = stob(Conf[EnhancementSection]["animated_pause_menu"]);
        CVar_SetS32("gPauseLiveLink", Settings.enhancements.animated_pause_menu);

        Settings.enhancements.dynamic_wallet_icon = stob(Conf[EnhancementSection]["dynamic_wallet_icon"]);
        CVar_SetS32("gDynamicWalletIcon", Settings.enhancements.dynamic_wallet_icon);

        Settings.enhancements.minimal_ui = stob(Conf[EnhancementSection]["minimal_ui"]);
        CVar_SetS32("gMinimalUI", Settings.enhancements.minimal_ui);

        Settings.enhancements.newdrops = stob(Conf[EnhancementSection]["newdrops"]);
        CVar_SetS32("gNewDrops", Settings.enhancements.newdrops);

        Settings.enhancements.visualagony = stob(Conf[EnhancementSection]["visualagony"]);
        CVar_SetS32("gVisualAgony", Settings.enhancements.visualagony);

        Settings.enhancements.mm_bunny_hood = stob(Conf[EnhancementSection]["mm_bunny_hood"]);
        CVar_SetS32("gMMBunnyHood", Settings.enhancements.mm_bunny_hood);

        Settings.enhancements.uniform_lr = stob(Conf[EnhancementSection]["uniform_lr"]);
        //CVar_SetS32("gUniformLR", Settings.enhancements.uniform_lr);
        CVar_SetS32("gUniformLR", 1);

        // Audio
        Settings.audio.master = Ship::stof(Conf[AudioSection]["master"]);
        CVar_SetFloat("gGameMasterVolume", Settings.audio.master);

        Settings.audio.music_main = Ship::stof(Conf[AudioSection]["music_main"]);
        CVar_SetFloat("gMainMusicVolume", Settings.audio.music_main);

        Settings.audio.music_sub = Ship::stof(Conf[AudioSection]["music_sub"]);
        CVar_SetFloat("gSubMusicVolume", Settings.audio.music_sub);

        Settings.audio.sfx = Ship::stof(Conf[AudioSection]["sfx"]);
        CVar_SetFloat("gSFXMusicVolume", Settings.audio.sfx);

        Settings.audio.fanfare = Ship::stof(Conf[AudioSection]["fanfare"]);
        CVar_SetFloat("gFanfareVolume", Settings.audio.fanfare);

        // Controllers
        Settings.controller.gyro_sensitivity = Ship::stof(Conf[ControllerSection]["gyro_sensitivity"]);
        CVar_SetFloat("gGyroSensitivity", Settings.controller.gyro_sensitivity);

        Settings.controller.rumble_strength = Ship::stof(Conf[ControllerSection]["rumble_strength"]);
        CVar_SetFloat("gRumbleStrength", Settings.controller.rumble_strength);

        Settings.controller.input_scale = Ship::stof(Conf[ControllerSection]["input_scale"]);
        CVar_SetFloat("gInputScale", Settings.controller.input_scale);

        Settings.controller.input_enabled = stob(Conf[ControllerSection]["input_enabled"]);
        CVar_SetS32("gInputEnabled", Settings.controller.input_enabled);

        Settings.controller.dpad_pause_name = stob(Conf[ControllerSection]["dpad_pause_name"]);
        CVar_SetS32("gDpadPauseName", Settings.controller.dpad_pause_name);

        Settings.controller.dpad_ocarina_text = stob(Conf[ControllerSection]["dpad_ocarina_text"]);
        CVar_SetS32("gDpadOcarinaText", Settings.controller.dpad_ocarina_text);

        Settings.controller.dpad_shop = stob(Conf[ControllerSection]["dpad_shop"]);
        CVar_SetS32("gDpadShop", Settings.controller.dpad_shop);

        // Cheats
        Settings.cheats.debug_mode = stob(Conf[CheatSection]["debug_mode"]);
        CVar_SetS32("gDebugEnabled", Settings.cheats.debug_mode);

        Settings.cheats.infinite_money = stob(Conf[CheatSection]["infinite_money"]);
        CVar_SetS32("gInfiniteMoney", Settings.cheats.infinite_money);

        Settings.cheats.infinite_health = stob(Conf[CheatSection]["infinite_health"]);
        CVar_SetS32("gInfiniteHealth", Settings.cheats.infinite_health);

        Settings.cheats.infinite_ammo = stob(Conf[CheatSection]["infinite_ammo"]);
        CVar_SetS32("gInfiniteAmmo", Settings.cheats.infinite_ammo);

        Settings.cheats.infinite_magic = stob(Conf[CheatSection]["infinite_magic"]);
        CVar_SetS32("gInfiniteMagic", Settings.cheats.infinite_magic);

        Settings.cheats.infinite_nayru = stob(Conf[CheatSection]["infinite_nayru"]);
        CVar_SetS32("gInfiniteNayru", Settings.cheats.infinite_nayru);

        Settings.cheats.no_clip = stob(Conf[CheatSection]["no_clip"]);
        CVar_SetS32("gNoClip", Settings.cheats.no_clip);

        Settings.cheats.climb_everything = stob(Conf[CheatSection]["climb_everything"]);
        CVar_SetS32("gClimbEverything", Settings.cheats.climb_everything);

        Settings.cheats.moon_jump_on_l = stob(Conf[CheatSection]["moon_jump_on_l"]);
        CVar_SetS32("gMoonJumpOnL", Settings.cheats.moon_jump_on_l);

        Settings.cheats.super_tunic = stob(Conf[CheatSection]["super_tunic"]);
        CVar_SetS32("gSuperTunic", Settings.cheats.super_tunic);

        Settings.cheats.ez_isg = stob(Conf[CheatSection]["ez_isg"]);
        CVar_SetS32("gEzISG", Settings.cheats.ez_isg);

        Settings.cheats.no_restrict_item = stob(Conf[CheatSection]["no_restrict_item"]);
        CVar_SetS32("gNoRestrictItems", Settings.cheats.no_restrict_item);

        Settings.cheats.freeze_time = stob(Conf[CheatSection]["freeze_time"]);
        CVar_SetS32("gFreezeTime", Settings.cheats.freeze_time);

        //Hud mods
        Settings.cosmetic.hudcolor = stob(Conf[COSMETICS_SECTION]["hud_colors"]);
        CVar_SetS32("gHudColors", Settings.cosmetic.hudcolor);
        //hearts main colors
        Settings.cosmetic.ccheartsprimr = (Conf[COSMETICS_SECTION]["cc_heartsprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_heartsprimr"]) : Settings.cosmetic.ccheartsprimr;
        CVar_SetS32("gCCHeartsPrimR", Settings.cosmetic.ccheartsprimr);
        Settings.cosmetic.ccheartsprimg = (Conf[COSMETICS_SECTION]["cc_heartsprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_heartsprimg"]) : Settings.cosmetic.ccheartsprimg;
        CVar_SetS32("gCCHeartsPrimG", Settings.cosmetic.ccheartsprimg);
        Settings.cosmetic.ccheartsprimb = (Conf[COSMETICS_SECTION]["cc_heartsprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_heartsprimb"]) : Settings.cosmetic.ccheartsprimb;
        CVar_SetS32("gCCHeartsPrimB", Settings.cosmetic.ccheartsprimb);
        //hearts double defense main colors
        Settings.cosmetic.ddccheartsprimr = (Conf[COSMETICS_SECTION]["cc_ddheartsprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_ddheartsprimr"]) : Settings.cosmetic.ddccheartsprimr;
        CVar_SetS32("gDDCCHeartsPrimR", Settings.cosmetic.ddccheartsprimr);
        Settings.cosmetic.ddccheartsprimg = (Conf[COSMETICS_SECTION]["cc_ddheartsprimg"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_ddheartsprimg"]) : Settings.cosmetic.ddccheartsprimg;
        CVar_SetS32("gDDCCHeartsPrimG", Settings.cosmetic.ddccheartsprimg);
        Settings.cosmetic.ddccheartsprimb = (Conf[COSMETICS_SECTION]["cc_ddheartsprimb"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_ddheartsprimb"]) : Settings.cosmetic.ddccheartsprimb;
        CVar_SetS32("gDDCCHeartsPrimB", Settings.cosmetic.ddccheartsprimb);
        //a button
        Settings.cosmetic.ccabtnprimr = (Conf[COSMETICS_SECTION]["cc_abtnprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_abtnprimr"]) : Settings.cosmetic.ccabtnprimr;
        CVar_SetS32("gCCABtnPrimR", Settings.cosmetic.ccabtnprimr);
        Settings.cosmetic.ccabtnprimg = (Conf[COSMETICS_SECTION]["cc_abtnprimg"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_abtnprimg"]) : Settings.cosmetic.ccabtnprimg;
        CVar_SetS32("gCCABtnPrimG", Settings.cosmetic.ccabtnprimg);
        Settings.cosmetic.ccabtnprimb = (Conf[COSMETICS_SECTION]["cc_abtnprimb"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_abtnprimb"]) : Settings.cosmetic.ccabtnprimb;
        CVar_SetS32("gCCABtnPrimB", Settings.cosmetic.ccabtnprimb);
        //b button
        Settings.cosmetic.ccbbtnprimr = (Conf[COSMETICS_SECTION]["cc_bbtnprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_bbtnprimr"]) : Settings.cosmetic.ccbbtnprimr;
        CVar_SetS32("gCCBBtnPrimR", Settings.cosmetic.ccbbtnprimr);
        Settings.cosmetic.ccbbtnprimg = (Conf[COSMETICS_SECTION]["cc_bbtnprimg"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_bbtnprimg"]) : Settings.cosmetic.ccbbtnprimg;
        CVar_SetS32("gCCBBtnPrimG", Settings.cosmetic.ccbbtnprimg);
        Settings.cosmetic.ccbbtnprimb = (Conf[COSMETICS_SECTION]["cc_bbtnprimb"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_bbtnprimb"]) : Settings.cosmetic.ccbbtnprimb;
        CVar_SetS32("gCCBBtnPrimB", Settings.cosmetic.ccbbtnprimb);
        //start button
        Settings.cosmetic.ccstartbtnprimr = (Conf[COSMETICS_SECTION]["cc_startbtnprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_startbtnprimr"]) : Settings.cosmetic.ccstartbtnprimr;
        CVar_SetS32("gCCStartBtnPrimR", Settings.cosmetic.ccstartbtnprimr);
        Settings.cosmetic.ccstartbtnprimg = (Conf[COSMETICS_SECTION]["cc_startbtnprimg"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_startbtnprimg"]) : Settings.cosmetic.ccstartbtnprimg;
        CVar_SetS32("gCCStartBtnPrimG", Settings.cosmetic.ccstartbtnprimg);
        Settings.cosmetic.ccstartbtnprimb = (Conf[COSMETICS_SECTION]["cc_startbtnprimb"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_startbtnprimb"]) : Settings.cosmetic.ccstartbtnprimb;
        CVar_SetS32("gCCStartBtnPrimB", Settings.cosmetic.ccstartbtnprimb);
        //c button
        Settings.cosmetic.cccbtnprimr = (Conf[COSMETICS_SECTION]["cc_cbtnprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_cbtnprimr"]) : Settings.cosmetic.cccbtnprimr;
        CVar_SetS32("gCCCBtnPrimR", Settings.cosmetic.cccbtnprimr);
        Settings.cosmetic.cccbtnprimg = (Conf[COSMETICS_SECTION]["cc_cbtnprimg"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_cbtnprimg"]) : Settings.cosmetic.cccbtnprimg;
        CVar_SetS32("gCCCBtnPrimG", Settings.cosmetic.cccbtnprimg);
        Settings.cosmetic.cccbtnprimb = (Conf[COSMETICS_SECTION]["cc_cbtnprimb"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_cbtnprimb"]) : Settings.cosmetic.cccbtnprimb;
        CVar_SetS32("gCCCBtnPrimB", Settings.cosmetic.cccbtnprimb);
        //magic metter borders
        Settings.cosmetic.ccmagicborderprimr = (Conf[COSMETICS_SECTION]["cc_magicborderprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_magicborderprimr"]) : Settings.cosmetic.ccmagicborderprimr;
        CVar_SetS32("gCCMagicBorderPrimR", Settings.cosmetic.ccmagicborderprimr);
        Settings.cosmetic.ccmagicborderprimg = (Conf[COSMETICS_SECTION]["cc_magicborderprimg"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_magicborderprimg"]) : Settings.cosmetic.ccmagicborderprimg;
        CVar_SetS32("gCCMagicBorderPrimG", Settings.cosmetic.ccmagicborderprimg);
        Settings.cosmetic.ccmagicborderprimb = (Conf[COSMETICS_SECTION]["cc_magicborderprimb"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_magicborderprimb"]) : Settings.cosmetic.ccmagicborderprimb;
        CVar_SetS32("gCCMagicBorderPrimB", Settings.cosmetic.ccmagicborderprimb);
        //magic metter remaining 
        Settings.cosmetic.ccmagicprimr = (Conf[COSMETICS_SECTION]["cc_magicprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_magicprimr"]) : Settings.cosmetic.ccmagicprimr;
        CVar_SetS32("gCCMagicPrimR", Settings.cosmetic.ccmagicprimr);
        Settings.cosmetic.ccmagicprimg = (Conf[COSMETICS_SECTION]["cc_magicprimg"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_magicprimg"]) : Settings.cosmetic.ccmagicprimg;
        CVar_SetS32("gCCMagicPrimG", Settings.cosmetic.ccmagicprimg);
        Settings.cosmetic.ccmagicprimb = (Conf[COSMETICS_SECTION]["cc_magicprimb"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_magicprimb"]) : Settings.cosmetic.ccmagicprimb;
        CVar_SetS32("gCCMagicPrimB", Settings.cosmetic.ccmagicprimb);
        //magic metter being used
        Settings.cosmetic.ccmagicuseprimr = (Conf[COSMETICS_SECTION]["cc_magicuseprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_magicuseprimr"]) : Settings.cosmetic.ccmagicuseprimr;
        CVar_SetS32("gCCMagicUsePrimR", Settings.cosmetic.ccmagicuseprimr);
        Settings.cosmetic.ccmagicuseprimg = (Conf[COSMETICS_SECTION]["cc_magicuseprimg"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_magicuseprimg"]) : Settings.cosmetic.ccmagicuseprimg;
        CVar_SetS32("gCCMagicUsePrimG", Settings.cosmetic.ccmagicuseprimg);
        Settings.cosmetic.ccmagicuseprimb = (Conf[COSMETICS_SECTION]["cc_magicuseprimb"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_magicuseprimb"]) : Settings.cosmetic.ccmagicuseprimb;
        CVar_SetS32("gCCMagicUsePrimB", Settings.cosmetic.ccmagicuseprimb);
        //minimap
        Settings.cosmetic.ccminimapprimr = (Conf[COSMETICS_SECTION]["cc_minimapprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_minimapprimr"]) : Settings.cosmetic.ccminimapprimr;
        CVar_SetS32("gCCMinimapPrimR", Settings.cosmetic.ccminimapprimr);
        Settings.cosmetic.ccminimapprimg = (Conf[COSMETICS_SECTION]["cc_minimapprimg"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_minimapprimg"]) : Settings.cosmetic.ccminimapprimg;
        CVar_SetS32("gCCMinimapPrimG", Settings.cosmetic.ccminimapprimg);
        Settings.cosmetic.ccminimapprimb = (Conf[COSMETICS_SECTION]["cc_minimapprimb"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_minimapprimb"]) : Settings.cosmetic.ccminimapprimb;
        CVar_SetS32("gCCMinimapPrimB", Settings.cosmetic.ccminimapprimb);
        //rupee icon
        Settings.cosmetic.ccrupeeprimr = (Conf[COSMETICS_SECTION]["cc_rupeeprimr"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_rupeeprimr"]) : Settings.cosmetic.ccrupeeprimr;
        CVar_SetS32("gCCRupeePrimR", Settings.cosmetic.ccrupeeprimr);
        Settings.cosmetic.ccrupeeprimg = (Conf[COSMETICS_SECTION]["cc_rupeeprimg"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_rupeeprimg"]) : Settings.cosmetic.ccrupeeprimg;
        CVar_SetS32("gCCRupeePrimG", Settings.cosmetic.ccrupeeprimg);
        Settings.cosmetic.ccrupeeprimb = (Conf[COSMETICS_SECTION]["cc_rupeeprimb"] != "") ? Ship::stoi(Conf[COSMETICS_SECTION]["cc_rupeeprimb"]) : Settings.cosmetic.ccrupeeprimb;
        CVar_SetS32("gCCRupeePrimB", Settings.cosmetic.ccrupeeprimb);

        //Tunics
        Settings.cosmetic.tunic_kokiri_r = (Conf[CosmeticsSection]["tunic_kokiri_r"] != "") ? Ship::stoi(Conf[CosmeticsSection]["tunic_kokiri_r"]) : Settings.cosmetic.tunic_kokiri_r;
        CVar_SetS32("gTunic_Kokiri_R", Settings.cosmetic.tunic_kokiri_r);
        Settings.cosmetic.tunic_kokiri_g = (Conf[CosmeticsSection]["tunic_kokiri_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["tunic_kokiri_g"]) : Settings.cosmetic.tunic_kokiri_g;
        CVar_SetS32("gTunic_Kokiri_G", Settings.cosmetic.tunic_kokiri_g);
        Settings.cosmetic.tunic_kokiri_b = (Conf[CosmeticsSection]["tunic_kokiri_b"] != "") ? Ship::stoi(Conf[CosmeticsSection]["tunic_kokiri_b"]) : Settings.cosmetic.tunic_kokiri_b;
        CVar_SetS32("gTunic_Kokiri_B", Settings.cosmetic.tunic_kokiri_b);

        Settings.cosmetic.tunic_goron_r = (Conf[CosmeticsSection]["tunic_goron_r"] != "") ? Ship::stoi(Conf[CosmeticsSection]["tunic_goron_r"]) : Settings.cosmetic.tunic_goron_r;
        CVar_SetS32("gTunic_Goron_R", Settings.cosmetic.tunic_goron_r);
        Settings.cosmetic.tunic_goron_g = (Conf[CosmeticsSection]["tunic_goron_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["tunic_goron_g"]) : Settings.cosmetic.tunic_goron_g;
        CVar_SetS32("gTunic_Goron_G", Settings.cosmetic.tunic_goron_g);
        Settings.cosmetic.tunic_goron_b = (Conf[CosmeticsSection]["tunic_goron_b"] != "") ? Ship::stoi(Conf[CosmeticsSection]["tunic_goron_b"]) : Settings.cosmetic.tunic_goron_b;
        CVar_SetS32("gTunic_Goron_B", Settings.cosmetic.tunic_goron_b);

        Settings.cosmetic.tunic_zora_r = (Conf[CosmeticsSection]["tunic_zora_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["tunic_zora_r"]) : Settings.cosmetic.tunic_zora_r;
        CVar_SetS32("gTunic_Zora_R", Settings.cosmetic.tunic_zora_r);
        Settings.cosmetic.tunic_zora_g = (Conf[CosmeticsSection]["tunic_zora_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["tunic_zora_g"]) : Settings.cosmetic.tunic_zora_g;
        CVar_SetS32("gTunic_Zora_G", Settings.cosmetic.tunic_zora_g);
        Settings.cosmetic.tunic_zora_b = (Conf[CosmeticsSection]["tunic_zora_b"] != "" ) ? Ship::stoi(Conf[CosmeticsSection]["tunic_zora_b"]) : Settings.cosmetic.tunic_zora_b;
        CVar_SetS32("gTunic_Zora_B", Settings.cosmetic.tunic_zora_b);
        //Navi
        Settings.cosmetic.navi_idle_inner_r = (Conf[CosmeticsSection]["navi_idle_inner_r"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_idle_inner_r"]) : Settings.cosmetic.navi_idle_inner_r;
        CVar_SetS32("gNavi_Idle_Inner_R", Settings.cosmetic.navi_idle_inner_r);
        Settings.cosmetic.navi_idle_inner_g = (Conf[CosmeticsSection]["navi_idle_inner_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_idle_inner_g"]) : Settings.cosmetic.navi_idle_inner_g;
        CVar_SetS32("gNavi_Idle_Inner_G", Settings.cosmetic.navi_idle_inner_g);
        Settings.cosmetic.navi_idle_inner_b = (Conf[CosmeticsSection]["navi_idle_inner_b"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_idle_inner_b"]) : Settings.cosmetic.navi_idle_inner_b;
        CVar_SetS32("gNavi_Idle_Inner_B", Settings.cosmetic.navi_idle_inner_b);
        Settings.cosmetic.navi_idle_outer_r = (Conf[CosmeticsSection]["navi_idle_outer_r"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_idle_outer_r"]) : Settings.cosmetic.navi_idle_outer_r;
        CVar_SetS32("gNavi_Idle_Outer_R", Settings.cosmetic.navi_idle_outer_r);
        Settings.cosmetic.navi_idle_outer_g = (Conf[CosmeticsSection]["navi_idle_outer_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_idle_outer_g"]) : Settings.cosmetic.navi_idle_outer_g;
        CVar_SetS32("gNavi_Idle_Outer_G", Settings.cosmetic.navi_idle_outer_g);
        Settings.cosmetic.navi_idle_outer_b = (Conf[CosmeticsSection]["navi_idle_outer_b"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_idle_outer_b"]) : Settings.cosmetic.navi_idle_outer_b;
        CVar_SetS32("gNavi_Idle_Outer_B", Settings.cosmetic.navi_idle_outer_b);

        Settings.cosmetic.navi_npc_inner_r = (Conf[CosmeticsSection]["navi_npc_inner_r"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_npc_inner_r"]) : Settings.cosmetic.navi_npc_inner_r;
        CVar_SetS32("gNavi_NPC_Inner_R", Settings.cosmetic.navi_npc_inner_r);
        Settings.cosmetic.navi_npc_inner_g = (Conf[CosmeticsSection]["navi_npc_inner_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_npc_inner_g"]) : Settings.cosmetic.navi_npc_inner_g;
        CVar_SetS32("gNavi_NPC_Inner_G", Settings.cosmetic.navi_npc_inner_g);
        Settings.cosmetic.navi_npc_inner_b = (Conf[CosmeticsSection]["navi_npc_inner_b"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_npc_inner_b"]) : Settings.cosmetic.navi_npc_inner_b;
        CVar_SetS32("gNavi_NPC_Inner_B", Settings.cosmetic.navi_npc_inner_b);
        Settings.cosmetic.navi_npc_outer_r = (Conf[CosmeticsSection]["navi_npc_outer_r"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_npc_outer_r"]) : Settings.cosmetic.navi_npc_outer_r;
        CVar_SetS32("gNavi_NPC_Outer_R", Settings.cosmetic.navi_npc_outer_r);
        Settings.cosmetic.navi_npc_outer_g = (Conf[CosmeticsSection]["navi_npc_outer_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_npc_outer_g"]) : Settings.cosmetic.navi_npc_outer_g;
        CVar_SetS32("gNavi_NPC_Outer_G", Settings.cosmetic.navi_npc_outer_g);
        Settings.cosmetic.navi_npc_outer_b = (Conf[CosmeticsSection]["navi_npc_outer_b"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_npc_outer_b"]) : Settings.cosmetic.navi_npc_outer_b;
        CVar_SetS32("gNavi_NPC_Outer_B", Settings.cosmetic.navi_npc_outer_b);

        Settings.cosmetic.navi_enemy_inner_r = (Conf[CosmeticsSection]["navi_enemy_inner_r"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_enemy_inner_r"]) : Settings.cosmetic.navi_enemy_inner_r;
        CVar_SetS32("gNavi_Enemy_Inner_R", Settings.cosmetic.navi_enemy_inner_r);
        Settings.cosmetic.navi_enemy_inner_g = (Conf[CosmeticsSection]["navi_enemy_inner_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_enemy_inner_g"]) : Settings.cosmetic.navi_enemy_inner_g;
        CVar_SetS32("gNavi_Enemy_Inner_G", Settings.cosmetic.navi_enemy_inner_g);
        Settings.cosmetic.navi_enemy_inner_b = (Conf[CosmeticsSection]["navi_enemy_inner_b"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_enemy_inner_b"]) : Settings.cosmetic.navi_enemy_inner_b;
        CVar_SetS32("gNavi_Enemy_Inner_B", Settings.cosmetic.navi_enemy_inner_b);
        Settings.cosmetic.navi_enemy_outer_r = (Conf[CosmeticsSection]["navi_enemy_outer_r"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_enemy_outer_r"]) : Settings.cosmetic.navi_enemy_outer_r;
        CVar_SetS32("gNavi_Enemy_Outer_R", Settings.cosmetic.navi_enemy_outer_r);
        Settings.cosmetic.navi_enemy_outer_g = (Conf[CosmeticsSection]["navi_enemy_outer_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_enemy_outer_g"]) : Settings.cosmetic.navi_enemy_outer_g;
        CVar_SetS32("gNavi_Enemy_Outer_G", Settings.cosmetic.navi_enemy_outer_g);
        Settings.cosmetic.navi_enemy_outer_b = (Conf[CosmeticsSection]["navi_enemy_outer_b"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_enemy_outer_b"]) : Settings.cosmetic.navi_enemy_outer_b;
        CVar_SetS32("gNavi_Enemy_Outer_B", Settings.cosmetic.navi_enemy_outer_b);

        Settings.cosmetic.navi_prop_inner_r = (Conf[CosmeticsSection]["navi_prop_inner_r"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_prop_inner_r"]) : Settings.cosmetic.navi_prop_inner_r;
        CVar_SetS32("gNavi_Prop_Inner_R", Settings.cosmetic.navi_prop_inner_r);
        Settings.cosmetic.navi_prop_inner_g = (Conf[CosmeticsSection]["navi_prop_inner_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_prop_inner_g"]) : Settings.cosmetic.navi_prop_inner_g;
        CVar_SetS32("gNavi_Prop_Inner_G", Settings.cosmetic.navi_prop_inner_g);
        Settings.cosmetic.navi_prop_inner_b = (Conf[CosmeticsSection]["navi_prop_inner_b"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_prop_inner_b"]) : Settings.cosmetic.navi_prop_inner_b;
        CVar_SetS32("gNavi_Prop_Inner_B", Settings.cosmetic.navi_prop_inner_b);
        Settings.cosmetic.navi_prop_outer_r = (Conf[CosmeticsSection]["navi_prop_outer_r"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_prop_outer_r"]) : Settings.cosmetic.navi_prop_outer_r;
        CVar_SetS32("gNavi_Prop_Outer_R", Settings.cosmetic.navi_prop_outer_r);
        Settings.cosmetic.navi_prop_outer_g = (Conf[CosmeticsSection]["navi_prop_outer_g"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_prop_outer_g"]) : Settings.cosmetic.navi_prop_outer_g;
        CVar_SetS32("gNavi_Prop_Outer_G", Settings.cosmetic.navi_prop_outer_g);
        Settings.cosmetic.navi_prop_outer_b = (Conf[CosmeticsSection]["navi_prop_outer_b"] != "") ? Ship::stoi(Conf[CosmeticsSection]["navi_prop_outer_b"]) : Settings.cosmetic.navi_prop_outer_b;
        CVar_SetS32("gNavi_Prop_Outer_B", Settings.cosmetic.navi_prop_outer_b);

        //languages
        Settings.languages.sellanguages = stob(Conf[LanguagesSection]["sel_languages"]);
        CVar_SetS32("gLanguages", Settings.languages.sellanguages);*/

        UpdateAudio();
    }

    void SaveSettings() {
	    const std::shared_ptr<ConfigFile> pConf = GlobalCtx2::GetInstance()->GetConfig();
        ConfigFile& Conf = *pConf;

        // Debug
        Conf[ConfSection]["console"] = std::to_string(SohImGui::console->opened);
        Conf[ConfSection]["menu_bar"] = std::to_string(Settings.debug.menu_bar);
        Conf[ConfSection]["soh_debug"] = std::to_string(Settings.debug.soh);
        /*Conf[ConfSection]["n64_mode"] = std::to_string(Settings.debug.n64mode);
        Conf[ConfSection]["hide_buildinfos"] = std::to_string(Settings.debug.buildinfos);

        // Audio
        Conf[AudioSection]["master"] = std::to_string(Settings.audio.master);
        Conf[AudioSection]["music_main"] = std::to_string(Settings.audio.music_main);
        Conf[AudioSection]["music_sub"] = std::to_string(Settings.audio.music_sub);
        Conf[AudioSection]["sfx"] = std::to_string(Settings.audio.sfx);
        Conf[AudioSection]["fanfare"] = std::to_string(Settings.audio.fanfare);

        // Enhancements
        Conf[EnhancementSection]["skip_text"] = std::to_string(Settings.enhancements.skip_text);
        Conf[EnhancementSection]["text_speed"] = std::to_string(Settings.enhancements.text_speed);
        Conf[EnhancementSection]["disable_lod"] = std::to_string(Settings.enhancements.disable_lod);
        Conf[EnhancementSection]["animated_pause_menu"] = std::to_string(Settings.enhancements.animated_pause_menu);
        Conf[EnhancementSection]["minimal_ui"] = std::to_string(Settings.enhancements.minimal_ui);
        Conf[EnhancementSection]["dynamic_wallet_icon"] = std::to_string(Settings.enhancements.dynamic_wallet_icon);
    	Conf[EnhancementSection]["newdrops"] = std::to_string(Settings.enhancements.newdrops);
        Conf[EnhancementSection]["visualagony"] = std::to_string(Settings.enhancements.visualagony);
    	Conf[EnhancementSection]["mm_bunny_hood"] = std::to_string(Settings.enhancements.mm_bunny_hood);
        Conf[EnhancementSection]["uniform_lr"] = std::to_string(Settings.enhancements.uniform_lr);

        // Controllers
        Conf[ControllerSection]["gyro_sensitivity"] = std::to_string(Settings.controller.gyro_sensitivity);
        Conf[ControllerSection]["rumble_strength"]  = std::to_string(Settings.controller.rumble_strength);
        Conf[ControllerSection]["input_scale"]   = std::to_string(Settings.controller.input_scale);
        Conf[ControllerSection]["input_enabled"] = std::to_string(Settings.controller.input_enabled);
        Conf[ControllerSection]["dpad_pause_name"] = std::to_string(Settings.controller.dpad_pause_name);
        Conf[ControllerSection]["dpad_ocarina_text"] = std::to_string(Settings.controller.dpad_ocarina_text);
        Conf[ControllerSection]["dpad_shop"] = std::to_string(Settings.controller.dpad_shop);

        // Cheats
        Conf[CheatSection]["debug_mode"] = std::to_string(Settings.cheats.debug_mode);
        Conf[CheatSection]["infinite_money"] = std::to_string(Settings.cheats.infinite_money);
        Conf[CheatSection]["infinite_health"] = std::to_string(Settings.cheats.infinite_health);
        Conf[CheatSection]["infinite_ammo"] = std::to_string(Settings.cheats.infinite_ammo);
        Conf[CheatSection]["infinite_magic"] = std::to_string(Settings.cheats.infinite_magic);
        Conf[CheatSection]["no_clip"] = std::to_string(Settings.cheats.no_clip);
        Conf[CheatSection]["climb_everything"] = std::to_string(Settings.cheats.climb_everything);
        Conf[CheatSection]["moon_jump_on_l"] = std::to_string(Settings.cheats.moon_jump_on_l);
        Conf[CheatSection]["super_tunic"] = std::to_string(Settings.cheats.super_tunic);

        //HUD Color
        Conf[CosmeticsSection]["hud_colors"] = std::to_string(Settings.cosmetic.hudcolor);
        Conf[CosmeticsSection]["cc_heartsprimr"] = std::to_string(Settings.cosmetic.ccheartsprimr);
        Conf[CosmeticsSection]["cc_heartsprimg"] = std::to_string(Settings.cosmetic.ccheartsprimg);
        Conf[CosmeticsSection]["cc_heartsprimb"] = std::to_string(Settings.cosmetic.ccheartsprimb);
        Conf[CosmeticsSection]["cc_ddheartsprimr"] = std::to_string(Settings.cosmetic.ddccheartsprimr);
        Conf[CosmeticsSection]["cc_ddheartsprimg"] = std::to_string(Settings.cosmetic.ddccheartsprimg);
        Conf[CosmeticsSection]["cc_ddheartsprimb"] = std::to_string(Settings.cosmetic.ddccheartsprimb);
        Conf[CosmeticsSection]["cc_abtnprimr"] = std::to_string(Settings.cosmetic.ccabtnprimr);
        Conf[CosmeticsSection]["cc_abtnprimg"] = std::to_string(Settings.cosmetic.ccabtnprimg);
        Conf[CosmeticsSection]["cc_abtnprimb"] = std::to_string(Settings.cosmetic.ccabtnprimb);
        Conf[CosmeticsSection]["cc_bbtnprimr"] = std::to_string(Settings.cosmetic.ccbbtnprimr);
        Conf[CosmeticsSection]["cc_bbtnprimg"] = std::to_string(Settings.cosmetic.ccbbtnprimg);
        Conf[CosmeticsSection]["cc_bbtnprimb"] = std::to_string(Settings.cosmetic.ccbbtnprimb);
        Conf[CosmeticsSection]["cc_cbtnprimr"] = std::to_string(Settings.cosmetic.cccbtnprimr);
        Conf[CosmeticsSection]["cc_cbtnprimg"] = std::to_string(Settings.cosmetic.cccbtnprimg);
        Conf[CosmeticsSection]["cc_cbtnprimb"] = std::to_string(Settings.cosmetic.cccbtnprimb);
        Conf[CosmeticsSection]["cc_startbtnprimr"] = std::to_string(Settings.cosmetic.ccstartbtnprimr);
        Conf[CosmeticsSection]["cc_startbtnprimg"] = std::to_string(Settings.cosmetic.ccstartbtnprimg);
        Conf[CosmeticsSection]["cc_startbtnprimb"] = std::to_string(Settings.cosmetic.ccstartbtnprimb);
        Conf[CosmeticsSection]["cc_magicborderprimr"] = std::to_string(Settings.cosmetic.ccmagicborderprimr);
        Conf[CosmeticsSection]["cc_magicborderprimg"] = std::to_string(Settings.cosmetic.ccmagicborderprimg);
        Conf[CosmeticsSection]["cc_magicborderprimb"] = std::to_string(Settings.cosmetic.ccmagicborderprimb);
        Conf[CosmeticsSection]["cc_magicprimr"] = std::to_string(Settings.cosmetic.ccmagicprimr);
        Conf[CosmeticsSection]["cc_magicprimg"] = std::to_string(Settings.cosmetic.ccmagicprimg);
        Conf[CosmeticsSection]["cc_magicprimb"] = std::to_string(Settings.cosmetic.ccmagicprimb);
        Conf[CosmeticsSection]["cc_magicuseprimr"] = std::to_string(Settings.cosmetic.ccmagicuseprimr);
        Conf[CosmeticsSection]["cc_magicuseprimg"] = std::to_string(Settings.cosmetic.ccmagicuseprimg);
        Conf[CosmeticsSection]["cc_magicuseprimb"] = std::to_string(Settings.cosmetic.ccmagicuseprimb);
        Conf[CosmeticsSection]["cc_minimapprimr"] = std::to_string(Settings.cosmetic.ccminimapprimr);
        Conf[CosmeticsSection]["cc_minimapprimg"] = std::to_string(Settings.cosmetic.ccminimapprimg);
        Conf[CosmeticsSection]["cc_minimapprimb"] = std::to_string(Settings.cosmetic.ccminimapprimb);
        Conf[CosmeticsSection]["cc_rupeeprimr"] = std::to_string(Settings.cosmetic.ccrupeeprimr);
        Conf[CosmeticsSection]["cc_rupeeprimg"] = std::to_string(Settings.cosmetic.ccrupeeprimg);
        Conf[CosmeticsSection]["cc_rupeeprimb"] = std::to_string(Settings.cosmetic.ccrupeeprimb);

        // Tunic/Navi colors
        Conf[CosmeticsSection]["tunic_kokiri_r"] = std::to_string(Settings.cosmetic.tunic_kokiri_r);
        Conf[CosmeticsSection]["tunic_kokiri_g"] = std::to_string(Settings.cosmetic.tunic_kokiri_g);
        Conf[CosmeticsSection]["tunic_kokiri_b"] = std::to_string(Settings.cosmetic.tunic_kokiri_b);

        Conf[CosmeticsSection]["tunic_goron_r"] = std::to_string(Settings.cosmetic.tunic_goron_r);
        Conf[CosmeticsSection]["tunic_goron_g"] = std::to_string(Settings.cosmetic.tunic_goron_g);
        Conf[CosmeticsSection]["tunic_goron_b"] = std::to_string(Settings.cosmetic.tunic_goron_b);

        Conf[CosmeticsSection]["tunic_zora_r"] = std::to_string(Settings.cosmetic.tunic_zora_r);
        Conf[CosmeticsSection]["tunic_zora_g"] = std::to_string(Settings.cosmetic.tunic_zora_g);
        Conf[CosmeticsSection]["tunic_zora_b"] = std::to_string(Settings.cosmetic.tunic_zora_b);

        Conf[CosmeticsSection]["navi_idle_inner_r"] = std::to_string(Settings.cosmetic.navi_idle_inner_r);
        Conf[CosmeticsSection]["navi_idle_inner_g"] = std::to_string(Settings.cosmetic.navi_idle_inner_g);
        Conf[CosmeticsSection]["navi_idle_inner_b"] = std::to_string(Settings.cosmetic.navi_idle_inner_b);
        Conf[CosmeticsSection]["navi_idle_outer_r"] = std::to_string(Settings.cosmetic.navi_idle_outer_r);
        Conf[CosmeticsSection]["navi_idle_outer_g"] = std::to_string(Settings.cosmetic.navi_idle_outer_g);
        Conf[CosmeticsSection]["navi_idle_outer_b"] = std::to_string(Settings.cosmetic.navi_idle_outer_b);

        Conf[CosmeticsSection]["navi_npc_inner_r"] = std::to_string(Settings.cosmetic.navi_npc_inner_r);
        Conf[CosmeticsSection]["navi_npc_inner_g"] = std::to_string(Settings.cosmetic.navi_npc_inner_g);
        Conf[CosmeticsSection]["navi_npc_inner_b"] = std::to_string(Settings.cosmetic.navi_npc_inner_b);
        Conf[CosmeticsSection]["navi_npc_outer_r"] = std::to_string(Settings.cosmetic.navi_npc_outer_r);
        Conf[CosmeticsSection]["navi_npc_outer_g"] = std::to_string(Settings.cosmetic.navi_npc_outer_g);
        Conf[CosmeticsSection]["navi_npc_outer_b"] = std::to_string(Settings.cosmetic.navi_npc_outer_b);

        Conf[CosmeticsSection]["navi_enemy_inner_r"] = std::to_string(Settings.cosmetic.navi_enemy_inner_r);
        Conf[CosmeticsSection]["navi_enemy_inner_g"] = std::to_string(Settings.cosmetic.navi_enemy_inner_g);
        Conf[CosmeticsSection]["navi_enemy_inner_b"] = std::to_string(Settings.cosmetic.navi_enemy_inner_b);
        Conf[CosmeticsSection]["navi_enemy_outer_r"] = std::to_string(Settings.cosmetic.navi_enemy_outer_r);
        Conf[CosmeticsSection]["navi_enemy_outer_g"] = std::to_string(Settings.cosmetic.navi_enemy_outer_g);
        Conf[CosmeticsSection]["navi_enemy_outer_b"] = std::to_string(Settings.cosmetic.navi_enemy_outer_b);

        Conf[CosmeticsSection]["navi_prop_inner_r"] = std::to_string(Settings.cosmetic.navi_prop_inner_r);
        Conf[CosmeticsSection]["navi_prop_inner_g"] = std::to_string(Settings.cosmetic.navi_prop_inner_g);
        Conf[CosmeticsSection]["navi_prop_inner_b"] = std::to_string(Settings.cosmetic.navi_prop_inner_b);
        Conf[CosmeticsSection]["navi_prop_outer_r"] = std::to_string(Settings.cosmetic.navi_prop_outer_r);
        Conf[CosmeticsSection]["navi_prop_outer_g"] = std::to_string(Settings.cosmetic.navi_prop_outer_g);
        Conf[CosmeticsSection]["navi_prop_outer_b"] = std::to_string(Settings.cosmetic.navi_prop_outer_b);

        //Languages
        Conf[LanguagesSection]["sel_languages"] = std::to_string(Settings.languages.sellanguages);*/

        Conf.Save();
        DebugConsole_SaveCVars();
    }

    void InitSettings() {
        ModInternal::registerHookListener({ AUDIO_INIT, [](HookEvent ev) {
            UpdateAudio();
        }});
    }

    void SetSeqPlayerVolume(SeqPlayers playerId, float volume) {
        Audio_SetGameVolume(playerId, volume);
    }
}
