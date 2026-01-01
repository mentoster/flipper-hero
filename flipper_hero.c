    #include <furi.h>
    #include <gui/gui.h>
    #include <gui/icon.h>
    #include <gui/elements.h>
    #include "data/data.h"
    #include "view/arrows.h"
    #include "helpers/storage.h"
    #include "flipper_hero_icons.h"

    #define SCORE_MULTIPLIER 5
    #define TIMER_INCREMENT_PER_ARROW 25
    #define TIMER_MAX 1000


    // Yes, I know that these are awful. But I tried using arrays.
    // Trust me, I tried. The flipper just doesn't have enough memory for arrays to be used here.
    // So these lovecraftian switch statements are what you get.
    // They have to be separate functions because:
    // get_stratagem_name is called in a unique context, and get_stratagem_icon isn't void. That is because I dont want to have
    // any more headaches with pointers. If you want to try to make this more elegant, be my guest.
    void get_stratagem_name(char name_buf[32], int i) {
        switch(i) {
            case  0: snprintf(name_buf, 32, "MG-43 Machine Gun"); break;
            case  1: snprintf(name_buf, 32, "APW-1 Anti Materiel Rifle"); break;
            case  2: snprintf(name_buf, 32, "M-105 Stalwart"); break;
            case  3: snprintf(name_buf, 32, "EAT-17 Expandable Anti-Tank"); break;
            case  4: snprintf(name_buf, 32, "GR-8 Recoilless Rifle"); break;
            case  5: snprintf(name_buf, 32, "FLAM-40 Flamethrower"); break;
            case  6: snprintf(name_buf, 32, "AC-8 Autocannon"); break;
            case  7: snprintf(name_buf, 32, "MG-206 Heavy Machine Gun"); break;
            case  8: snprintf(name_buf, 32, "RL-77 Airburst Rocket Launcher"); break;
            case  9: snprintf(name_buf, 32, "MLS-4X Commando"); break;
            case 10: snprintf(name_buf, 32, "RS-422 Railgun"); break;
            case 11: snprintf(name_buf, 32, "FAF-14 Spear"); break;
            case 12: snprintf(name_buf, 32, "StA-X3 W.A.S.P. Launcher"); break;
            case 13: snprintf(name_buf, 32, "GL-21 Grenade Launcher"); break;
            case 14: snprintf(name_buf, 32, "LAS-98 Laser Cannon"); break;
            case 15: snprintf(name_buf, 32, "ARC-3 Arc Thrower"); break;
            case 16: snprintf(name_buf, 32, "LAS-99 Quasar Cannon"); break;
            case 17: snprintf(name_buf, 32, "TX-41 Sterilizer"); break;
            case 18: snprintf(name_buf, 32, "CQC-1 One True Flag"); break;
            case 19: snprintf(name_buf, 32, "GL-52 De-Escalator"); break;
            case 20: snprintf(name_buf, 32, "PLAS-45 Epoch"); break;
            case 21: snprintf(name_buf, 32, "S-11 Speargun"); break;
            case 22: snprintf(name_buf, 32, "EAT-700 Expendable Napalm"); break;
            case 23: snprintf(name_buf, 32, "MS-11 Solo Silo"); break;
            case 24: snprintf(name_buf, 32, "M-1000 Maxigun"); break;
            case 25: snprintf(name_buf, 32, "CQC-9 Defoliation Tool"); break;
            case 26: snprintf(name_buf, 32, "Orbital Gatling Barrage"); break;
            case 27: snprintf(name_buf, 32, "Orbital Airburst Strike"); break;
            case 28: snprintf(name_buf, 32, "Orbital 120mm HE Barrage"); break;
            case 29: snprintf(name_buf, 32, "Orbital 380mm HE Barrage"); break;
            case 30: snprintf(name_buf, 32, "Orbital Walking Barrage"); break;
            case 31: snprintf(name_buf, 32, "Orbital Laser"); break;
            case 32: snprintf(name_buf, 32, "Orbital Napalm Barrage"); break;
            case 33: snprintf(name_buf, 32, "Orbital Railcannon Strike"); break;
            case 34: snprintf(name_buf, 32, "Orbital Precision Strike"); break;
            case 35: snprintf(name_buf, 32, "Orbital Gas Strike"); break;
            case 36: snprintf(name_buf, 32, "Orbital EMS Strike"); break;
            case 37: snprintf(name_buf, 32, "Orbital Smoke Strike"); break;
            case 38: snprintf(name_buf, 32, "Eagle Strafing Run"); break;
            case 39: snprintf(name_buf, 32, "Eagle Airstrike"); break;
            case 40: snprintf(name_buf, 32, "Eagle Cluster Bomb"); break;
            case 41: snprintf(name_buf, 32, "Eagle Napalm Airstrike"); break;
            case 42: snprintf(name_buf, 32, "Eagle Smoke Strike"); break;
            case 43: snprintf(name_buf, 32, "Eagle 110mm Rocket Pods"); break;
            case 44: snprintf(name_buf, 32, "Eagle 500kg Bomb"); break;
            case 45: snprintf(name_buf, 32, "LIFT-850 Jump Pack"); break;
            case 46: snprintf(name_buf, 32, "LIFT-860 Hover Pack"); break;
            case 47: snprintf(name_buf, 32, "LIFT-182 Warp Pack"); break;
            case 48: snprintf(name_buf, 32, "B-1 Supply Pack"); break;
            case 49: snprintf(name_buf, 32, "SH-32 Shield Generator Pack"); break;
            case 50: snprintf(name_buf, 32, "SH-20 Ballistic Shield Backpack"); break;
            case 51: snprintf(name_buf, 32, "SH-51 Directional Shield"); break;
            case 52: snprintf(name_buf, 32, "AX/AR-23 'Guard Dog'"); break;
            case 53: snprintf(name_buf, 32, "AX/LAS-5 'Guard Dog' Rover"); break;
            case 54: snprintf(name_buf, 32, "AX/TX-13 'Guard Dog' Dog Breath"); break;
            case 55: snprintf(name_buf, 32, "AX/ARC-3 'Guard Dog' K-9"); break;
            case 56: snprintf(name_buf, 32, "AX-FLAM-75 'Guard Dog' Hot Dog"); break;
            case 57: snprintf(name_buf, 32, "B-100 Portable Hellbomb"); break;
            case 58: snprintf(name_buf, 32, "MD-6 Anti-Personnel Minefield"); break;
            case 59: snprintf(name_buf, 32, "MD-I4 Incendiary Mines"); break;
            case 60: snprintf(name_buf, 32, "MD-17 Anti-Tank Mines"); break;
            case 61: snprintf(name_buf, 32, "MD-8 Gas Mines"); break;
            case 62: snprintf(name_buf, 32, "A/ARC-3 Tesla Tower"); break;
            case 63: snprintf(name_buf, 32, "E/MG-101 HMG Emplacement"); break;
            case 64: snprintf(name_buf, 32, "E/AT-12 Anti-Tank Emplacement"); break;
            case 65: snprintf(name_buf, 32, "E/GL-21 Grenadier Battlement"); break;
            case 66: snprintf(name_buf, 32, "FX-12 Shield Generator Relay"); break;
            case 67: snprintf(name_buf, 32, "A/MG-43 Machine Gun Sentry"); break;
            case 68: snprintf(name_buf, 32, "A/G-16 Gatling Sentry"); break;
            case 69: snprintf(name_buf, 32, "A/M-12 Mortar Sentry"); break;
            case 70: snprintf(name_buf, 32, "A/AC-8 Autocannon Sentry"); break;
            case 71: snprintf(name_buf, 32, "A/MLS-4X Rocket Sentry"); break;
            case 72: snprintf(name_buf, 32, "A/M-23 EMS Mortar Sentry"); break;
            case 73: snprintf(name_buf, 32, "A/FLAM-40 Flame Sentry"); break;
            case 74: snprintf(name_buf, 32, "A/LAS-98 Laser Sentry"); break;
            case 75: snprintf(name_buf, 32, "M-102 Fast Recon Vehicle"); break;
            case 76: snprintf(name_buf, 32, "EXO-45 Patriot Exosuit"); break;
            case 77: snprintf(name_buf, 32, "EXO-49 Emancipator Exosuit"); break;
            case 78: snprintf(name_buf, 32, "Reinforce"); break;
            case 79: snprintf(name_buf, 32, "Resupply"); break;
            case 80: snprintf(name_buf, 32, "Hellbomb"); break;
            case 81: snprintf(name_buf, 32, "SoS Beacon"); break;
            case 82: snprintf(name_buf, 32, "Eagle Rearm"); break;
            case 83: snprintf(name_buf, 32, "SSSD Delivery"); break;
            case 84: snprintf(name_buf, 32, "Prospecting Drill"); break;
            case 85: snprintf(name_buf, 32, "Super Earth Flag"); break;
            case 86: snprintf(name_buf, 32, "Upload Data"); break;
            case 87: snprintf(name_buf, 32, "Seismic Probe"); break;
            case 88: snprintf(name_buf, 32, "SEAF Artillery"); break;
            case 89: snprintf(name_buf, 32, "Dark Fluid Vessel"); break;
            case 90: snprintf(name_buf, 32, "Tectonic Drill"); break;
            case 91: snprintf(name_buf, 32, "Hive Breaker Drill"); break;
            case 92: snprintf(name_buf, 32, "Cargo Container"); break;
            default: snprintf(name_buf, 32, "INVALID STRATAGEM INDEX"); break;
        }
    }
    Icon* get_stratagem_icon(int i, bool large_icon) {
        if (large_icon) {
            switch(i) {
                case  0: return (Icon*)&I_w_mg_30x30;
                case  1: return (Icon*)&I_w_amr_30x30;
                case  2: return (Icon*)&I_w_stalwart_30x30;
                case  3: return (Icon*)&I_w_eat_30x30;
                case  4: return (Icon*)&I_w_recoilless_30x30;
                case  5: return (Icon*)&I_w_flamethrower_30x30;
                case  6: return (Icon*)&I_w_autocannon_30x30;
                case  7: return (Icon*)&I_w_hmg_30x30;
                case  8: return (Icon*)&I_w_airburst_30x30;
                case  9: return (Icon*)&I_w_commando_30x30;
                case 10: return (Icon*)&I_w_railgun_30x30;
                case 11: return (Icon*)&I_w_spear_30x30;
                case 12: return (Icon*)&I_w_wasp_30x30;
                case 13: return (Icon*)&I_w_grenade_launcher_30x30;
                case 14: return (Icon*)&I_w_laser_cannon_30x30;
                case 15: return (Icon*)&I_w_arc_30x30;
                case 16: return (Icon*)&I_w_quasar_30x30;
                case 17: return (Icon*)&I_w_sterilizer_30x30;
                case 18: return (Icon*)&I_w_flag_30x30;
                case 19: return (Icon*)&I_w_de_escalator_30x30;
                case 20: return (Icon*)&I_w_epoch_30x30;
                case 21: return (Icon*)&I_w_speargun_30x30;
                case 22: return (Icon*)&I_w_eat_napalm_30x30;
                case 23: return (Icon*)&I_e_silo_30x30;
                case 24: return (Icon*)&I_w_maxigun_30x30;
                case 25: return (Icon*)&I_w_tool_30x30;
                case 26: return (Icon*)&I_o_gatling_30x30;
                case 27: return (Icon*)&I_o_airburst_30x30;
                case 28: return (Icon*)&I_o_120mm_30x30;
                case 29: return (Icon*)&I_o_380mm_30x30;
                case 30: return (Icon*)&I_o_walking_30x30;
                case 31: return (Icon*)&I_o_laser_30x30;
                case 32: return (Icon*)&I_o_napalm_30x30;
                case 33: return (Icon*)&I_o_railcannon_30x30;
                case 34: return (Icon*)&I_o_precision_30x30;
                case 35: return (Icon*)&I_o_gas_30x30;
                case 36: return (Icon*)&I_o_ems_30x30;
                case 37: return (Icon*)&I_o_smoke_30x30;
                case 38: return (Icon*)&I_e_strafing_30x30;
                case 39: return (Icon*)&I_e_airstrike_30x30;
                case 40: return (Icon*)&I_e_cluster_30x30;
                case 41: return (Icon*)&I_e_napalm_30x30;
                case 42: return (Icon*)&I_e_smoke_30x30;
                case 43: return (Icon*)&I_e_110mm_30x30;
                case 44: return (Icon*)&I_e_500kg_30x30;
                case 45: return (Icon*)&I_p_jump_30x30;
                case 46: return (Icon*)&I_p_hover_30x30;
                case 47: return (Icon*)&I_p_warp_30x30;
                case 48: return (Icon*)&I_p_supply_30x30;
                case 49: return (Icon*)&I_p_s_generator_30x30;
                case 50: return (Icon*)&I_p_s_ballistic_30x30;
                case 51: return (Icon*)&I_p_s_directional_30x30;
                case 52: return (Icon*)&I_gd_30x30;
                case 53: return (Icon*)&I_gd_las_30x30;
                case 54: return (Icon*)&I_gd_gas_30x30;
                case 55: return (Icon*)&I_gd_arc_30x30;
                case 56: return (Icon*)&I_gd_flam_30x30;
                case 57: return (Icon*)&I_p_hellbomb_30x30;
                case 58: return (Icon*)&I_m_ap_30x30;
                case 59: return (Icon*)&I_m_incendiary_30x30;
                case 60: return (Icon*)&I_m_antitank_30x30;
                case 61: return (Icon*)&I_m_gas_30x30;
                case 62: return (Icon*)&I_m_tesla_30x30;
                case 63: return (Icon*)&I_emp_hmg_30x30;
                case 64: return (Icon*)&I_emp_antitank_30x30;
                case 65: return (Icon*)&I_m_battlement_30x30;
                case 66: return (Icon*)&I_m_relay_30x30;
                case 67: return (Icon*)&I_s_mg_30x30;
                case 68: return (Icon*)&I_s_gatling_30x30;
                case 69: return (Icon*)&I_s_mortar_30x30;
                case 70: return (Icon*)&I_s_autocannon_30x30;
                case 71: return (Icon*)&I_s_rocket_30x30;
                case 72: return (Icon*)&I_s_ems_mortar_30x30;
                case 73: return (Icon*)&I_s_flame_30x30;
                case 74: return (Icon*)&I_s_laser_30x30;
                case 75: return (Icon*)&I_v_frv_30x30;
                case 76: return (Icon*)&I_v_patriot_30x30;
                case 77: return (Icon*)&I_v_emancipator_30x30;
                case 78: return (Icon*)&I_m_reinforce_30x30;
                case 79: return (Icon*)&I_m_resupply_30x30;
                case 80: return (Icon*)&I_m_hellbomb_30x30;
                case 81: return (Icon*)&I_m_sos_30x30;
                case 82: return (Icon*)&I_m_rearm_30x30;
                case 83: return (Icon*)&I_m_sssd_30x30;
                case 84: return (Icon*)&I_m_drill_30x30;
                case 85: return (Icon*)&I_m_flag_30x30;
                case 86: return (Icon*)&I_m_sssd_30x30;
                case 87: return (Icon*)&I_m_seismic_probe_30x30;
                case 88: return (Icon*)&I_m_seaf_artillery_30x30;
                case 89: return (Icon*)&I_m_vessel_30x30;
                case 90: return (Icon*)&I_m_drill_30x30;
                case 91: return (Icon*)&I_m_drill_30x30;
                case 92: return (Icon*)&I_m_cargo_30x30;
                default: return (Icon*)&I_eagle_30x30; // Failsafe. It's a blank eagle icon.
            }
        } else {
            switch(i) {
                case  0: return (Icon*)&I_w_mg_15x15;
                case  1: return (Icon*)&I_w_amr_15x15;
                case  2: return (Icon*)&I_w_stalwart_15x15;
                case  3: return (Icon*)&I_w_eat_15x15;
                case  4: return (Icon*)&I_w_recoilless_15x15;
                case  5: return (Icon*)&I_w_flamethrower_15x15;
                case  6: return (Icon*)&I_w_autocannon_15x15;
                case  7: return (Icon*)&I_w_hmg_15x15;
                case  8: return (Icon*)&I_w_airburst_15x15;
                case  9: return (Icon*)&I_w_commando_15x15;
                case 10: return (Icon*)&I_w_railgun_15x15;
                case 11: return (Icon*)&I_w_spear_15x15;
                case 12: return (Icon*)&I_w_wasp_15x15;
                case 13: return (Icon*)&I_w_grenade_launcher_15x15;
                case 14: return (Icon*)&I_w_laser_cannon_15x15;
                case 15: return (Icon*)&I_w_arc_15x15;
                case 16: return (Icon*)&I_w_quasar_15x15;
                case 17: return (Icon*)&I_w_sterilizer_15x15;
                case 18: return (Icon*)&I_w_flag_15x15;
                case 19: return (Icon*)&I_w_de_escalator_15x15;
                case 20: return (Icon*)&I_w_epoch_15x15;
                case 21: return (Icon*)&I_w_speargun_15x15;
                case 22: return (Icon*)&I_w_eat_napalm_15x15;
                case 23: return (Icon*)&I_e_silo_15x15;
                case 24: return (Icon*)&I_w_maxigun_15x15;
                case 25: return (Icon*)&I_w_tool_15x15;
                case 26: return (Icon*)&I_o_gatling_15x15;
                case 27: return (Icon*)&I_o_airburst_15x15;
                case 28: return (Icon*)&I_o_120mm_15x15;
                case 29: return (Icon*)&I_o_380mm_15x15;
                case 30: return (Icon*)&I_o_walking_15x15;
                case 31: return (Icon*)&I_o_laser_15x15;
                case 32: return (Icon*)&I_o_napalm_15x15;
                case 33: return (Icon*)&I_o_railcannon_15x15;
                case 34: return (Icon*)&I_o_precision_15x15;
                case 35: return (Icon*)&I_o_gas_15x15;
                case 36: return (Icon*)&I_o_ems_15x15;
                case 37: return (Icon*)&I_o_smoke_15x15;
                case 38: return (Icon*)&I_e_strafing_15x15;
                case 39: return (Icon*)&I_e_airstrike_15x15;
                case 40: return (Icon*)&I_e_cluster_15x15;
                case 41: return (Icon*)&I_e_napalm_15x15;
                case 42: return (Icon*)&I_e_smoke_15x15;
                case 43: return (Icon*)&I_e_110mm_15x15;
                case 44: return (Icon*)&I_e_500kg_15x15;
                case 45: return (Icon*)&I_p_jump_15x15;
                case 46: return (Icon*)&I_p_hover_15x15;
                case 47: return (Icon*)&I_p_warp_15x15;
                case 48: return (Icon*)&I_p_supply_15x15;
                case 49: return (Icon*)&I_p_s_generator_15x15;
                case 50: return (Icon*)&I_p_s_ballistic_15x15;
                case 51: return (Icon*)&I_p_s_directional_15x15;
                case 52: return (Icon*)&I_gd_15x15;
                case 53: return (Icon*)&I_gd_las_15x15;
                case 54: return (Icon*)&I_gd_gas_15x15;
                case 55: return (Icon*)&I_gd_arc_15x15;
                case 56: return (Icon*)&I_gd_flam_15x15;
                case 57: return (Icon*)&I_p_hellbomb_15x15;
                case 58: return (Icon*)&I_m_ap_15x15;
                case 59: return (Icon*)&I_m_incendiary_15x15;
                case 60: return (Icon*)&I_m_antitank_15x15;
                case 61: return (Icon*)&I_m_gas_15x15;
                case 62: return (Icon*)&I_m_tesla_15x15;
                case 63: return (Icon*)&I_emp_hmg_15x15;
                case 64: return (Icon*)&I_emp_antitank_15x15;
                case 65: return (Icon*)&I_m_battlement_15x15;
                case 66: return (Icon*)&I_m_relay_15x15;
                case 67: return (Icon*)&I_s_mg_15x15;
                case 68: return (Icon*)&I_s_gatling_15x15;
                case 69: return (Icon*)&I_s_mortar_15x15;
                case 70: return (Icon*)&I_s_autocannon_15x15;
                case 71: return (Icon*)&I_s_rocket_15x15;
                case 72: return (Icon*)&I_s_ems_mortar_15x15;
                case 73: return (Icon*)&I_s_flame_15x15;
                case 74: return (Icon*)&I_s_laser_15x15;
                case 75: return (Icon*)&I_v_frv_15x15;
                case 76: return (Icon*)&I_v_patriot_15x15;
                case 77: return (Icon*)&I_v_emancipator_15x15;
                case 78: return (Icon*)&I_m_reinforce_15x15;
                case 79: return (Icon*)&I_m_resupply_15x15;
                case 80: return (Icon*)&I_m_hellbomb_15x15;
                case 81: return (Icon*)&I_m_sos_15x15;
                case 82: return (Icon*)&I_m_rearm_15x15;
                case 83: return (Icon*)&I_m_sssd_15x15;
                case 84: return (Icon*)&I_m_drill_15x15;
                case 85: return (Icon*)&I_m_flag_15x15;
                case 86: return (Icon*)&I_m_sssd_15x15;
                case 87: return (Icon*)&I_m_seismic_probe_15x15;
                case 88: return (Icon*)&I_m_seaf_artillery_15x15;
                case 89: return (Icon*)&I_m_vessel_15x15;
                case 90: return (Icon*)&I_m_drill_15x15;
                case 91: return (Icon*)&I_m_drill_15x15;
                case 92: return (Icon*)&I_m_cargo_15x15;
                default: return (Icon*)&I_eagle_15x15; // Failsafe. It's a blank eagle icon.
            }
        }
        return (Icon*)&I_eagle_15x15; // idk why its making me do this but whatev
    }
    void get_stratagem_seq(PluginState* plugin_state, int i) {
        switch(i) {
            case  0: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLDURXXX"); break; // MG-43 Machine Gun
            case  1: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLRUDXXX"); break; // Anti Material Rifle
            case  2: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLDUULXX"); break; // Stalwart
            case  3: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DDLURXXX"); break; // EAT
            case  4: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLRRLXXX"); break; // Recoilless Rifle
            case  5: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLUDUXXX"); break; // Flamethrower
            case  6: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLDUURXX"); break; // Autocannon
            case  7: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLUDDXXX"); break; // Heavy Machine Gun
            case  8: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DUULRXXX"); break; // AirburstRocketLauncher
            case  9: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLUDRXXX"); break; // Commando
            case 10: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DRDULRXX"); break; // Railgun
            case 11: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DDUDDXXX"); break; // Spear
            case 12: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DDUDRXXX"); break; // W.A.S.P
            case 13: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLULDXXX"); break; // GrenadeLauncher
            case 14: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLDULXXX"); break; // Laser Cannon
            case 15: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DRDULLXX"); break; // Arc Thrower
            case 16: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DDULRXXX"); break; // Quasar Cannon
            case 17: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLUDLXXX"); break; // Sterilizer
            case 18: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLRRUXXX"); break; // One True Flag
            case 19: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DRULRXXX"); break; // De-Escalator
            case 20: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLULRXXX"); break; // Epoch
            case 21: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DRDLURXX"); break; // Speargun
            case 22: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DDLULXXX"); break; // Expendable Napalm
            case 23: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DURDDXXX"); break; // Solo Silo
            case 24: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLRDUUXX"); break; // Maxigun
            case 25: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLRRDXXX"); break; // Defoliation Tool
            case 26: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RDLUUXXX"); break; // Gatling Barrage // ORBITALS
            case 27: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RRRXXXXX"); break; // Airburst
            case 28: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RRDLRDXX"); break; // 120mm HE Barrage
            case 29: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RDUULDDX"); break; // 380mm HE Barrage
            case 30: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RDRDRDXX"); break; // Walking Barrage
            case 31: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RDURDXXX"); break; // Laser
            case 32: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RRDLRUXX"); break; // Napalm Barrage
            case 33: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RUDDRXXX"); break; // Railcannon Strike
            case 34: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RRUXXXXX"); break; // Precision Strike
            case 35: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RRDRXXXX"); break; // Gas Strike
            case 36: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RRLDXXXX"); break; // EMS Strike
            case 37: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RRDUXXXX"); break; // Smoke Strike
            case 38: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "URRXXXXX"); break; // StrafingRun // EAGLE
            case 39: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "URDRXXXX"); break; // Airstrike
            case 40: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "URDDRXXX"); break; // Cluster Bomb
            case 41: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "URDUXXXX"); break; // Napalm Airstrike
            case 42: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "URUDXXXX"); break; // Smoke Strike
            case 43: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "URULXXXX"); break; // 110mmRocket Pods
            case 44: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "URDDDXXX"); break; // 500kg
            case 45: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DUUDUXXX"); break; // Jump // BACKPACKS
            case 46: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DUUDLRXX"); break; // Hover
            case 47: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLRDLRXX"); break; // Warp
            case 48: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLDUUDXX"); break; // Supply
            case 49: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DULRLRXX"); break; // Shield Generator
            case 50: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLDDULXX"); break; // Ballistic Shield
            case 51: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DULRUUXX"); break; // Directional Shield
            case 52: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DULURDXX"); break; // GuardDog
            case 53: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DULURRXX"); break; // GuardDogRover
            case 54: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DULURUXX"); break; // GuardDogDog Breath
            case 55: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DULURLXX"); break; // GuardDog K-9
            case 56: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DULULLXX"); break; // Guard Dog Hot Dog
            case 57: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DRUUUXXX"); break; // Portable Hellbomb
            case 58: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLURXXXX"); break; // Anti-Personnel // MINES
            case 59: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLLDXXXX"); break; // Incendiary
            case 60: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLUUXXXX"); break; // Anti-Tank
            case 61: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DLLRXXXX"); break; // Gas
            case 62: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DURULRXX"); break; // Tesla Tower // SENTRIES/EMPLACEMENTS
            case 63: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DULRRLXX"); break; // HMG Emplacement
            case 64: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DULRRRXX"); break; // Anti-Tank Emplacement
            case 65: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DRDLRXXX"); break; // Grenadier Battlement
            case 66: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DDLRLRXX"); break; // Shield Generator Relay
            case 67: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DURRUXXX"); break; // Machine Gun
            case 68: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DURLXXXX"); break; // Gatling
            case 69: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DURRDXXX"); break; // Mortar
            case 70: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DURULUXX"); break; // Autocannon
            case 71: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DURRLXXX"); break; // Rocket
            case 72: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DURDRXXX"); break; // EMS Mortar
            case 73: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DURDUUXX"); break; // Flame
            case 74: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DURDURXX"); break; // Laser
            case 75: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "LDRDRDUX"); break; // Fast Recon Vehicle // VEHICLES
            case 76: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "LDRULDDX"); break; // Patriot
            case 77: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "LDRULDUX"); break; // Emancipator
            case 78: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "UDRLUXXX"); break; // Reinforce // MISSION STRATAGEMS
            case 79: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DDURXXXX"); break; // Resupply
            case 80: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DULDURDU"); break; // Hellbomb
            case 81: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "UDRUXXXX"); break; // SoS Beacon
            case 82: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "UULURXXX"); break; // Eagle Rearm
            case 83: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DDDUUXXX"); break; // SSSD Delivery
            case 84: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DDLRDDXX"); break; // Prospecting Drill
            case 85: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "DUDUXXXX"); break; // Super Earth Flag
            case 86: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "LRUUUXXX"); break; // Upload Data
            case 87: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "UULRDDXX"); break; // Seismic Probe
            case 88: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "RUUDXXXX"); break; // SEAF Artillery
            case 89: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "ULRDUUXX"); break; // Dark Fluid Vessel
            case 90: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "UDUDUDXX"); break; // Tectonic Drill
            case 91: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "LUDRDDXX"); break; // Hive Breaker Drill
            case 92: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "UUDDRDXX"); break; // Cargo Container
            default: snprintf(plugin_state->seq_buf, sizeof("LLLLLLLL"), "LLLLLLLL"); break; // Failsafe
        }
    }

    void fill_stratagem_queue(PluginState* plugin_state) {
        // Fill stratagem queue with invalid stratagem IDs. The renderer will know to stop counting stratagems when it reaches an invalid one.
        for (int i = 0; i < 20; i++) {
            plugin_state->stratagem_queue[i] = -1;
        }

        // Determine the amount of stratagems to put in the queue. Stratagem amount starts at round 1 with 6, and caps at 16 at round 11.
        int stratagem_amount = 0;
        if (plugin_state->round > 10) {
            stratagem_amount = 16;
        } else {
            stratagem_amount = 5 + plugin_state->round;
        }
        //Populate the queue
        for (int i = 0; i < stratagem_amount; i++) {
            plugin_state->stratagem_queue[i] = rand() % 93;
        }
    }

    void generate_arrows(PluginState* plugin_state) {

        //Initialize an empty stratagem, then fill it with the sequence of the next stratagem in the queue.
        char current_stratagem[9] = "XXXXXXXXX";
        get_stratagem_seq(plugin_state, plugin_state->stratagem_queue[plugin_state->queue_spot]);
        for (int i = 0; i < 8; i++) {
            current_stratagem[i] = plugin_state->seq_buf[i];
        }

        // Generate arrows until an X is reached, indicating the end of the sequence.
        int i = 0;
        while(current_stratagem[i] != 'X') {
            plugin_state->arrowDirections[i] = current_stratagem[i];
            plugin_state->arrowFilled[i] = false;
            i++;
        }
        plugin_state->numArrows = i;
        plugin_state->nextArrowToFill = 0;
    }

    void advance_round(PluginState* plugin_state) {
        plugin_state->isRoundOver = true;
        plugin_state->bonuses[1] = plugin_state->timer;
        plugin_state->timer = TIMER_MAX;
    }

    void update_score_and_timer(PluginState* plugin_state) {
        plugin_state->timer += TIMER_INCREMENT_PER_ARROW * plugin_state->numArrows;
        plugin_state->score += SCORE_MULTIPLIER * plugin_state->numArrows;


        plugin_state->queue_spot++;
        // If there's a -1  where a stratagem number should be, end the round.
        if (plugin_state->stratagem_queue[plugin_state->queue_spot] == -1) {
            advance_round(plugin_state);
        }
        // Removed when switching to more faithful round system
        // plugin_state->round++;
    }

    void end_game(PluginState* plugin_state) {
        bool shouldSave = false;
        if(plugin_state->score > plugin_state->record_score) {
            plugin_state->record_score = plugin_state->score;
            shouldSave = true;
        }
        if(plugin_state->round > plugin_state->record_round) {
            plugin_state->record_round = plugin_state->round;
            shouldSave = true;
        }
        if(shouldSave) save_game_records(plugin_state);
        plugin_state->isGameOver = true;
        plugin_state->isRoundOver = false;
        plugin_state->isPreRound = false;
        plugin_state->isScoreScreen = false;
    }
    void start_round(PluginState* plugin_state) {
        plugin_state->isPreRound = false;
        plugin_state->queue_spot = 0;
        fill_stratagem_queue(plugin_state);
        plugin_state->timer = TIMER_MAX;
        plugin_state->perfect = true;
        generate_arrows(plugin_state);
    }

    static void input_callback(InputEvent* input_event, void* ctx) {

        FuriMessageQueue* event_queue = ctx;
        PluginEvent event = {.type = EventTypeKey, .input = *input_event};
        furi_message_queue_put(event_queue, &event, FuriWaitForever);
    }

    static void timer_callback(void* context) {
        PluginState* plugin_state = (PluginState*)context;
        if(!plugin_state || plugin_state->isGameOver) return;

        // Decrement the timer
        plugin_state->timer--;


        if (plugin_state->isRoundOver) {
            plugin_state->isRoundOver = false;
            plugin_state->timer = 250;
            plugin_state->isScoreScreen = true;
            plugin_state->bonuses[0] = plugin_state->round * 25;
            if (plugin_state->perfect) {
                plugin_state->bonuses[2] = 100;
            } else {
                plugin_state->bonuses[2] = 0;
            }
            plugin_state->score += plugin_state->bonuses[0] + plugin_state->bonuses[1] + plugin_state->bonuses[2];
        }
        // Check if the game should end or change state
        if(plugin_state->timer <= 0) {
            if (plugin_state->isScoreScreen) {
                // switch to pre-round state from round score screen
                plugin_state->timer = 250;
                plugin_state->isPreRound = true;
                plugin_state->isScoreScreen = false;
                plugin_state->round++;
            } else if (plugin_state->isPreRound) {
                // Start a new round from pre-round state
                plugin_state->isPreRound = false;
                start_round(plugin_state);
            }
            else {
                end_game(plugin_state);
            }
        }
    }

    void handle_arrow_input(PluginState* plugin_state, InputEvent input_event) {
        if(input_event.type != InputTypePress) return;
        char expectedDirection = plugin_state->arrowDirections[plugin_state->nextArrowToFill];
        bool isCorrect = false;
        switch(input_event.key) {
        case InputKeyUp:
            isCorrect = expectedDirection == 'U';
            break;
        case InputKeyDown:
            isCorrect = expectedDirection == 'D';
            break;
        case InputKeyLeft:
            isCorrect = expectedDirection == 'L';
            break;
        case InputKeyRight:
            isCorrect = expectedDirection == 'R';
            break;
        default:
            break;
        }
        if(isCorrect) {
            plugin_state->arrowFilled[plugin_state->nextArrowToFill++] = true;
        } else if(input_event.key != InputKeyOk) {
            plugin_state->perfect = false;
            memset(plugin_state->arrowFilled, false, sizeof(plugin_state->arrowFilled));
            plugin_state->nextArrowToFill = 0;
        }
    }

    void init_game_state(PluginState* plugin_state) {
        plugin_state->isGameStarted = false;
        plugin_state->isGameOver = false;
        plugin_state->score = 0;
        plugin_state->round = 1;
        plugin_state->perfect = true;
        plugin_state->queue_spot = 0;
        plugin_state->timer = TIMER_MAX;

        fill_stratagem_queue(plugin_state);
    }

    void start_game(PluginState* plugin_state, FuriTimer* timer) {
        init_game_state(plugin_state);
        plugin_state->isGameStarted = true;
        generate_arrows(plugin_state);
        furi_timer_start(timer, 10);
    }

    void draw_round_score(PluginState* plugin_state, Canvas* const canvas) {
        char bonusTextBuf[32];
        canvas_set_font(canvas, FontPrimary);
        //Round Bonus ~= 25 * round number
        canvas_draw_str_aligned(canvas, 4, 16, AlignLeft, AlignCenter, "Round Bonus");
        snprintf(bonusTextBuf, sizeof(bonusTextBuf), "%d", plugin_state->bonuses[0]);

        canvas_draw_str_aligned(canvas, 124, 16, AlignRight, AlignCenter, bonusTextBuf);

        // Time Bonus
        if (plugin_state->timer < 200) {

            canvas_draw_str_aligned(canvas, 4, 28, AlignLeft, AlignCenter, "Time Bonus");
            snprintf(bonusTextBuf, sizeof(bonusTextBuf), "%d", plugin_state->bonuses[1]);

            canvas_draw_str_aligned(canvas, 124, 28, AlignRight, AlignCenter, bonusTextBuf);
        }
        // Perfect Bonus
        if (plugin_state->timer < 150) {

            canvas_draw_str_aligned(canvas, 4, 40, AlignLeft, AlignCenter, "Perfect Bonus");
            snprintf(bonusTextBuf, sizeof(bonusTextBuf), "%d", plugin_state->bonuses[2]);

            canvas_draw_str_aligned(canvas, 124, 40, AlignRight, AlignCenter, bonusTextBuf);
        }
        // Total Score
        if (plugin_state->timer < 100 ) {

            canvas_set_font(canvas, FontPrimary);
            canvas_draw_str_aligned(canvas, 4, 52, AlignLeft, AlignCenter, "Total Score");
            snprintf(bonusTextBuf, sizeof(bonusTextBuf), "%d", plugin_state->score);

            canvas_draw_str_aligned(canvas, 124, 52, AlignRight, AlignCenter, bonusTextBuf);
        }
    }

    void draw_pre_round(PluginState* plugin_state, Canvas* const canvas) {
        char roundText[32];
        snprintf(roundText, sizeof(roundText), "%d", plugin_state->round);
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, "Get Ready");
        canvas_draw_str_aligned(canvas, 64, 50, AlignCenter, AlignBottom, "Round");
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 51, AlignCenter, AlignTop, roundText);
    }
    void draw_start_screen(Canvas* const canvas) {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_icon(canvas, 0, 0, &I_flipper_hero_128x50);
        elements_button_center(canvas, "Start");
    }
    void draw_game_over_screen(Canvas* const canvas, int score, int round) {
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str_aligned(canvas, 64, 10, AlignCenter, AlignCenter, "Game Over");
        char scoreText[32];
        snprintf(scoreText, sizeof(scoreText), "Score: %d", score);
        canvas_draw_str_aligned(canvas, 64, 25, AlignCenter, AlignCenter, scoreText);
        char roundText[32];
        snprintf(roundText, sizeof(roundText), "Round: %d", round);
        canvas_draw_str_aligned(canvas, 64, 35, AlignCenter, AlignCenter, roundText);

        elements_button_center(canvas, "Restart");
        canvas_draw_frame(canvas, 0, 0, 128, 64);
    }


    void draw_game_ui(Canvas* const canvas, char name_buf[32], int round, int score, int ind) {
        char roundText[32], scoreText[32], nameText[32];
        get_stratagem_name(name_buf, ind);


        snprintf(roundText, sizeof(roundText), "%d", round);
        snprintf(scoreText, sizeof(scoreText), "%d", score);
        snprintf(nameText, sizeof(nameText), "%s", name_buf);


        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str_aligned(canvas, 4, 9, AlignLeft, AlignBottom, "Round");
        canvas_draw_str_aligned(canvas, 124, 9, AlignRight, AlignBottom, "Score");



        canvas_set_font(canvas, FontKeyboard);
        canvas_draw_str_aligned(canvas, 4, 10, AlignLeft, AlignTop, roundText);
        canvas_draw_str_aligned(canvas, 124, 10, AlignRight, AlignTop, scoreText);
        canvas_draw_box(canvas, 0, 34, 128, 11);
        canvas_invert_color(canvas);
        canvas_draw_str_aligned(canvas, 2, 35, AlignLeft, AlignTop, nameText);
        canvas_invert_color(canvas);
        // canvas_draw_frame(canvas, 0, 0, 128, 64);
    }

    void draw_icon(
        Canvas* const canvas,
        int8_t pos_x,
        int8_t pos_y,
        int8_t icon_index,
        bool large_icon) {
        int width = 15;
        int height = 15;
        if (large_icon) {
            width *= 2;
            height *= 2;
        }

        pos_x -= width / 2;
        pos_y -= height / 2;

        canvas_draw_icon(canvas, pos_x, pos_y, get_stratagem_icon(icon_index, large_icon));

    }

    void draw_queue(Canvas* const canvas, PluginState* plugin_state) {
        int spaceX = 15;
        // int totalIconsWidth = 5 * spaceX;
        int queue_stop = plugin_state->queue_spot + 5;
        if (queue_stop > 20) {
            queue_stop = 20;
        }
        int startX = 64;
        for (int i = plugin_state->queue_spot; i < queue_stop; i++) {
            if (plugin_state->stratagem_queue[i] == -1) {
                break;
            }
            if (i == plugin_state->queue_spot) {
                draw_icon(
                    canvas,
                    startX,
                    17,
                    plugin_state->stratagem_queue[i],
                    true);
                canvas_draw_frame(canvas, startX - 16, 1, 32, 32);
            } else {
                draw_icon(
                    canvas,
                    startX + ((i - plugin_state->queue_spot) * (spaceX + 1)) + 8,
                    25,
                    plugin_state->stratagem_queue[i],
                    false);
            }
        }
    }
    void draw_arrows(Canvas* const canvas, PluginState* plugin_state) {
        int spaceX = 15;
        int totalArrowsWidth = (plugin_state->numArrows - 1) * spaceX;
        int startX = (128 - totalArrowsWidth) / 2 + 2; // Assuming canvas width of 128 pixels
        for(int i = 0; i < plugin_state->numArrows; i++) {
            draw_empty_or_filled_arrow(
                canvas,
                startX + i * spaceX,
                53,
                plugin_state->arrowDirections[i],
                plugin_state->arrowFilled[i]);
        }
    }

    void draw_progress_box(Canvas* const canvas, int timer) {
        canvas_draw_frame(canvas, 0, 58, 128, 6);
        canvas_draw_box(
            canvas, 0, 58, timer * 0.128, 6); // Assuming max timer value scales to full width
    }

    static void render_callback(Canvas* const canvas, void* ctx) {
        PluginState* plugin_state = (PluginState*)ctx;
        furi_mutex_acquire(plugin_state->mutex, FuriWaitForever);
        canvas_clear(canvas);
        canvas_set_color(canvas, ColorBlack);

        if(!plugin_state->isGameStarted) {
            draw_start_screen(canvas);
        } else if(plugin_state->isGameOver) {
            draw_game_over_screen(canvas, plugin_state->score, plugin_state->round);
        } else if (plugin_state->isScoreScreen){
            draw_round_score(plugin_state, canvas);
        } else if (plugin_state->isPreRound) {
            draw_pre_round(plugin_state, canvas);
        } else if (!plugin_state->isRoundOver) {
            draw_queue(canvas, plugin_state);
            draw_game_ui(canvas, plugin_state->name_buf, plugin_state->round, plugin_state->score, plugin_state->stratagem_queue[plugin_state->queue_spot]);
            draw_arrows(canvas, plugin_state);
            draw_progress_box(canvas, plugin_state->timer);
        }

        furi_mutex_release(plugin_state->mutex);
    }
    int32_t flipper_hero_app() {
        FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(PluginEvent));
        PluginState* plugin_state = malloc(sizeof(PluginState));

        FuriTimer* timer =
            furi_timer_alloc(timer_callback, FuriTimerTypePeriodic, (void*)plugin_state);
        plugin_state->mutex = furi_mutex_alloc(FuriMutexTypeNormal);

        if(!plugin_state->mutex) {
            FURI_LOG_E("flipper_hero", "cannot create mutex\r\n");
            free(plugin_state);
            return 255;
        }

        init_game_state(plugin_state);
        // load_game_records(plugin_state);

        // Set system callbacks
        ViewPort* view_port = view_port_alloc();
        view_port_draw_callback_set(view_port, render_callback, plugin_state);
        view_port_input_callback_set(view_port, input_callback, event_queue);

        // Open GUI and register view_port
        Gui* gui = furi_record_open("gui");
        gui_add_view_port(gui, view_port, GuiLayerFullscreen);

        PluginEvent event;
        for(bool processing = true; processing;) {
            FuriStatus event_status = furi_message_queue_get(event_queue, &event, 100);

            furi_mutex_acquire(plugin_state->mutex, FuriWaitForever);

            if(event_status == FuriStatusOk) {
                // Pass the event to the new function for processing
                handle_arrow_input(plugin_state, event.input);
            } else {
                FURI_LOG_D("flipper_hero", "FuriMessageQueue: event timeout");
            }

            // Check if all arrows are filled, then regenerate
            if(plugin_state->nextArrowToFill >= plugin_state->numArrows) {
                update_score_and_timer(plugin_state);

                generate_arrows(plugin_state); // Re-initialize arrow states
            }

            view_port_update(view_port);
            furi_mutex_release(plugin_state->mutex);

            if(event.input.key == InputKeyBack) {
                // quit game
                // stop timers
                furi_timer_stop(timer);
                processing = false; // Close the application
            } else if(event.input.key == InputKeyOk) {
                if(!plugin_state->isGameStarted) {
                    start_game(plugin_state, timer);
                } else if(plugin_state->isGameOver) {
                    start_game(plugin_state, timer);
                }
            }
        }
        view_port_enabled_set(view_port, false);
        gui_remove_view_port(gui, view_port);
        furi_record_close("gui");
        view_port_free(view_port);
        furi_message_queue_free(event_queue);
        furi_mutex_free(plugin_state->mutex);
        return 0;
    }
