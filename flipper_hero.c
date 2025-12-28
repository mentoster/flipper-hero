    #include <furi.h>
    #include <gui/gui.h>
    #include <gui/elements.h>
    #include "data/data.h"
    #include "view/arrows.h"
    #include "helpers/storage.h"
    #include "flipper_hero_icons.h"

    #define MIN_ARROWS 3
    #define MAX_ARROWS 8
    #define SCORE_MULTIPLIER 5
    #define TIMER_INCREMENT_PER_ARROW 25
    #define TIMER_MAX 1000

    int generate_numArrows() {
        return MIN_ARROWS + rand() % (MAX_ARROWS - MIN_ARROWS + 1);
    }
    void generate_arrows(PluginState* plugin_state) {
        // Generate a new set of arrows, 3 to 8
        // plugin_state->numArrows = generate_numArrows();
        //
        // for(int i = 0; i < plugin_state->numArrows; i++) {
        //     plugin_state->arrowDirections[i] = "UDLR"[rand() % 4];
        //     plugin_state->arrowFilled[i] = false;
        // }
        // plugin_state->nextArrowToFill = 0;
        // char chosen_stratagem[8] = "LLLLLLLL";
        // int chosen_index = rand() % 81;
        char current_stratagem[9] = "XXXXXXXXX";
        for (int i = 0; i < 8; i++) {
            current_stratagem[i] = plugin_state->stratagems[plugin_state->stratagem_queue[0]][i];
        }
        int i = 0;
        while(current_stratagem[i] != 'X') {
            plugin_state->arrowDirections[i] = current_stratagem[i];
            plugin_state->arrowFilled[i] = false;
            i++;
        }
        plugin_state->numArrows = i;
        plugin_state->nextArrowToFill = 0;
    }

    void update_score_and_timer(PluginState* plugin_state) {
        plugin_state->timer += TIMER_INCREMENT_PER_ARROW * plugin_state->numArrows;
        plugin_state->score += SCORE_MULTIPLIER * plugin_state->numArrows;
        plugin_state->round++;
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

        // Check if the game should end
        if(plugin_state->timer <= 0) end_game(plugin_state);
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
            memset(plugin_state->arrowFilled, false, sizeof(plugin_state->arrowFilled));
            plugin_state->nextArrowToFill = 0;
        }
    }

    void init_game_state(PluginState* plugin_state) {
        plugin_state->isGameStarted = false;
        plugin_state->isGameOver = false;
        plugin_state->score = 0;
        plugin_state->round = 1;
        plugin_state->timer = TIMER_MAX;
        char stratagem_list[81][8] = {
            {'D','L','D','U','R','X','X','X'}, // MG-43 Machine Gun
            {'D','L','R','U','D','X','X','X'}, // Anti Material Rifle
            {'D','L','D','U','U','L','X','X'}, // Stalwart
            {'D','D','L','U','R','X','X','X'}, // EAT
            {'D','L','R','R','L','X','X','X'}, // Recoilless Rifle
            {'D','L','U','D','U','X','X','X'}, // Flamethrower
            {'D','L','D','U','U','R','X','X'}, // Autocannon
            {'D','L','U','D','D','X','X','X'}, // Heavy Machine Gun
            {'D','U','U','L','R','X','X','X'}, // AirburstRocketLauncher
            {'D','L','U','D','R','X','X','X'}, // Commando
            {'D','R','D','U','L','R','X','X'}, // Railgun
            {'D','D','U','D','D','X','X','X'}, // Spear
            {'D','D','U','D','R','X','X','X'}, // W.A.S.P
            {'D','L','U','L','D','X','X','X'}, // GrenadeLauncher
            {'D','L','D','U','L','X','X','X'}, // Laser Cannon
            {'D','R','D','U','L','L','X','X'}, // Arc Thrower
            {'D','D','U','L','R','X','X','X'}, // Quasar Cannon
            {'D','L','U','D','L','X','X','X'}, // Sterilizer
            {'D','L','R','R','U','X','X','X'}, // One True Flag
            {'D','R','U','L','R','X','X','X'}, // De-Escalator
            {'D','L','U','L','R','X','X','X'}, // Epoch
            {'D','R','D','L','U','R','X','X'}, // Speargun
            {'D','D','L','U','L','X','X','X'}, // Expendable Napalm
            {'D','U','R','D','D','X','X','X'}, // Solo Silo
            {'D','L','R','D','U','U','X','X'}, // Maxigun
            {'D','L','R','R','D','X','X','X'}, //Defoliation Tool
            // ORBITALS
            {'R','D','L','U','U','X','X','X'}, // Gatling Barrage
            {'R','R','R','X','X','X','X','X'}, // Airburst
            {'R','R','D','L','R','D','X','X'}, // 120mm HE Barrage
            {'R','D','U','U','L','D','D','X'}, // 380mm HE Barrage
            {'R','D','R','D','R','D','X','X'}, // Walking Barrage
            {'R','D','U','R','D','X','X','X'}, // Laser
            {'R','R','D','L','R','U','X','X'}, // Napalm Barrage
            {'R','U','D','D','R','X','X','X'}, //Railcannon Strike
            {'R','R','U','X','X','X','X','X'}, // Precision Strike
            {'R','R','D','R','X','X','X','X'}, // Gas Strike
            {'R','R','L','D','X','X','X','X'}, // EMS Strike
            {'R','R','D','U','X','X','X','X'}, // Smoke Strike
            // EAGLE
            {'U','R','R','X','X','X','X','X'}, // StrafingRun
            {'U','R','D','R','X','X','X','X'}, // Airstrike
            {'U','R','D','D','R','X','X','X'}, // Cluster Bomb
            {'U','R','D','U','X','X','X','X'}, // Napalm Airstrike
            {'U','R','U','D','X','X','X','X'}, // Smoke Strike
            {'U','R','U','L','X','X','X','X'}, // 110mmRocket Pods
            {'U','R','D','D','D','X','X','X'}, // 500kg
            // BACKPACKS
            {'D','U','U','D','U','X','X','X'}, // Jump
            {'D','U','U','D','L','R','X','X'}, // Hover
            {'D','L','R','D','L','R','X','X'}, // Warp
            {'D','L','D','U','U','D','X','X'}, // Supply
            {'D','U','L','R','L','R','X','X'}, // Shield Generator
            {'D','L','D','D','U','L','X','X'}, // Ballistic Shield
            {'D','U','L','R','U','U','X','X'}, //Directional Shield
            {'D','U','L','U','R','D','X','X'}, // GuardDog
            {'D','U','L','U','R','R','X','X'}, // GuardDogRover
            {'D','U','L','U','R','U','X','X'}, // GuardDogDog Breath
            {'D','U','L','U','R','L','X','X'}, // GuardDog K-9
            {'D','U','L','U','L','L','X','X'}, // Guard Dog Hot Dog
            {'D','R','U','U','U','X','X','X'}, // Portable Hellbomb
            // MINES
            {'D','L','U','R','X','X','X','X'}, // Anti-Personnel
            {'D','L','L','D','X','X','X','X'}, // Incendiary
            {'D','L','U','U','X','X','X','X'}, // Anti-Tank
            {'D','L','L','R','X','X','X','X'}, // Gas
            // SENTRIES/EMPLACEMENTS
            {'D','U','R','U','L','R','X','X'}, // Tesla Tower
            {'D','U','L','R','R','L','X','X'}, // HMG Emplacement
            {'D','U','L','R','R','R','X','X'}, // Anti-Tank Emplacement
            {'D','R','D','L','R','X','X','X'}, // Grenadier Battlement
            {'D','D','L','R','L','R','X','X'}, // Shield Generator Relay
            {'D','U','R','R','U','X','X','X'}, // Machine Gun
            {'D','U','R','L','X','X','X','X'}, // Gatling
            {'D','U','R','R','D','X','X','X'}, // Mortar
            {'D','U','R','U','L','U','X','X'}, // Autocannon
            {'D','U','R','R','L','X','X','X'}, // Rocket
            {'D','U','R','D','R','X','X','X'}, // EMS Mortar
            {'D','U','R','D','U','U','X','X'}, // Flame
            {'D','U','R','D','U','R','X','X'}, // Laser
            // VEHICLES
            {'L','D','R','D','R','D','U','X'}, // Fast Recon Vehicle
            {'L','D','R','U','L','D','D','X'}, // Patriot
            {'L','D','R','U','L','D','U','X'}, // Emancipator
            // MISSION STRATAGEMS
            {'U','D','R','L','U','X','X','X'}, // Reinforce
            {'D','D','U','R','X','X','X','X'}, // Resupply
            {'D','U','L','D','U','R','D','U'} // Hellbomb
        };
        for (int s = 0; s < 81; s++) {
            for (int c = 0; c < 8; c++) {
                plugin_state->stratagems[s][c] = stratagem_list[s][c];
            }
            // for (int c = 0; c < 32; c++) {
            //     plugin_state->stratagem_name_list[s][c] = stratagem_names[s][c];
            // }
        }
        for (int i=0; i < 6; i++) {
            plugin_state->stratagem_queue[i] = rand() % 81;
        }
    }

    void start_game(PluginState* plugin_state, FuriTimer* timer) {
        init_game_state(plugin_state);
        plugin_state->isGameStarted = true;
        generate_arrows(plugin_state);
        furi_timer_start(timer, 10);
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


    void draw_game_ui(Canvas* const canvas, int round, int score, int ind) {
        char roundText[32], scoreText[32], nameText[32];



        canvas_set_font(canvas, FontPrimary);
        snprintf(roundText, sizeof(roundText), "Round %d", round);
        snprintf(scoreText, sizeof(scoreText), "%d score", score);


        // This awful mess is the only way I was able to avoid a stack overflow.

        switch(ind) {
            case 0:
                snprintf(nameText, sizeof(nameText), "MG-43 Machine Gun");
                break;
            case 1:
                snprintf(nameText, sizeof(nameText), "APW-1 Anti Materiel Rifle");
                break;
            case 2:
                snprintf(nameText, sizeof(nameText), "M-105 Stalwart");
                break;
            case 3:
                snprintf(nameText, sizeof(nameText), "EAT-17 Expandable Anti-Tank");
                break;
            case 4:
                snprintf(nameText, sizeof(nameText), "GR-8 Recoilless Rifle");
                break;
            case 5:
                snprintf(nameText, sizeof(nameText), "FLAM-40 Flamethrower");
                break;
            case 6:
                snprintf(nameText, sizeof(nameText), "AC-8 Autocannon");
                break;
            case 7:
                snprintf(nameText, sizeof(nameText), "MG-206 Heavy Machine Gun");
                break;
            case 8:
                snprintf(nameText, sizeof(nameText), "RL-77 Airburst Rocket Launcher");
                break;
            case 9:
                snprintf(nameText, sizeof(nameText), "MLS-4X Commando");
                break;
            case 10:
                snprintf(nameText, sizeof(nameText), "RS-422 Railgun");
                break;
            case 11:
                snprintf(nameText, sizeof(nameText), "FAF-14 Spear");
                break;
            case 12:
                snprintf(nameText, sizeof(nameText), "StA-X3 W.A.S.P. Launcher");
                break;
            case 13:
                snprintf(nameText, sizeof(nameText), "GL-21 Grenade Launcher");
                break;
            case 14:
                snprintf(nameText, sizeof(nameText), "LAS-98 Laser Cannon");
                break;
            case 15:
                snprintf(nameText, sizeof(nameText), "ARC-3 Arc Thrower");
                break;
            case 16:
                snprintf(nameText, sizeof(nameText), "LAS-99 Quasar Cannon");
                break;
            case 17:
                snprintf(nameText, sizeof(nameText), "TX-41 Sterilizer");
                break;
            case 18:
                snprintf(nameText, sizeof(nameText), "CQC-1 One True Flag");
                break;
            case 19:
                snprintf(nameText, sizeof(nameText), "GL-52 De-Escalator");
                break;
            case 20:
                snprintf(nameText, sizeof(nameText), "PLAS-45 Epoch");
                break;
            case 21:
                snprintf(nameText, sizeof(nameText), "S-11 Speargun");
                break;
            case 22:
                snprintf(nameText, sizeof(nameText), "EAT-700 Expendable Napalm");
                break;
            case 23:
                snprintf(nameText, sizeof(nameText), "MS-11 Solo Silo");
                break;
            case 24:
                snprintf(nameText, sizeof(nameText), "M-1000 Maxigun");
                break;
            case 25:
                snprintf(nameText, sizeof(nameText), "CQC-9 Defoliation Tool");
                break;
            case 26:
                snprintf(nameText, sizeof(nameText), "Orbital Gatling Barrage");
                break;
            case 27:
                snprintf(nameText, sizeof(nameText), "Orbital Airburst Strike");
                break;
            case 28:
                snprintf(nameText, sizeof(nameText), "Orbital 120mm HE Barrage");
                break;
            case 29:
                snprintf(nameText, sizeof(nameText), "Orbital 380mm HE Barrage");
                break;
            case 30:
                snprintf(nameText, sizeof(nameText), "Orbital Walking Barrage");
                break;
            case 31:
                snprintf(nameText, sizeof(nameText), "Orbital Laser");
                break;
            case 32:
                snprintf(nameText, sizeof(nameText), "Orbital Napalm Barrage");
                break;
            case 33:
                snprintf(nameText, sizeof(nameText), "Orbital Railcannon Strike");
                break;
            case 34:
                snprintf(nameText, sizeof(nameText), "Orbital Precision Strike");
                break;
            case 35:
                snprintf(nameText, sizeof(nameText), "Orbital Gas Strike");
                break;
            case 36:
                snprintf(nameText, sizeof(nameText), "Orbital EMS Strike");
                break;
            case 37:
                snprintf(nameText, sizeof(nameText), "Orbital Smoke Strike");
                break;
            case 38:
                snprintf(nameText, sizeof(nameText), "Eagle Strafing Run");
                break;
            case 39:
                snprintf(nameText, sizeof(nameText), "Eagle Airstrike");
                break;
            case 40:
                snprintf(nameText, sizeof(nameText), "Eagle Cluster Bomb");
                break;
            case 41:
                snprintf(nameText, sizeof(nameText), "Eagle Napalm Airstrike");
                break;
            case 42:
                snprintf(nameText, sizeof(nameText), "Eagle Smoke Strike");
                break;
            case 43:
                snprintf(nameText, sizeof(nameText), "Eagle 110mm Rocket Pods");
                break;
            case 44:
                snprintf(nameText, sizeof(nameText), "Eagle 500kg Bomb");
                break;
            case 45:
                snprintf(nameText, sizeof(nameText), "LIFT-850 Jump Pack");
                break;
            case 46:
                snprintf(nameText, sizeof(nameText), "LIFT-860 Hover Pack");
                break;
            case 47:
                snprintf(nameText, sizeof(nameText), "LIFT-182 Warp Pack");
                break;
            case 48:
                snprintf(nameText, sizeof(nameText), "B-1 Supply Pack");
                break;
            case 49:
                snprintf(nameText, sizeof(nameText), "SH-32 Shield Generator Pack");
                break;
            case 50:
                snprintf(nameText, sizeof(nameText), "SH-20 Ballistic Shield Backpack");
                break;
            case 51:
                snprintf(nameText, sizeof(nameText), "SH-51 Directional Shield");
                break;
            case 52:
                snprintf(nameText, sizeof(nameText), "AX/AR-23 'Guard Dog'");
                break;
            case 53:
                snprintf(nameText, sizeof(nameText), "AX/LAS-5 'Guard Dog' Rover");
                break;
            case 54:
                snprintf(nameText, sizeof(nameText), "AX/TX-13 'Guard Dog' Dog Breath");
                break;
            case 55:
                snprintf(nameText, sizeof(nameText), "AX/ARC-3 'Guard Dog' K-9");
                break;
            case 56:
                snprintf(nameText, sizeof(nameText), "AX-FLAM-75 'Guard Dog' Hot Dog");
                break;
            case 57:
                snprintf(nameText, sizeof(nameText), "B-100 Portable Hellbomb");
                break;
            case 58:
                snprintf(nameText, sizeof(nameText), "MD-6 Anti-Personnel Minefield");
                break;
            case 59:
                snprintf(nameText, sizeof(nameText), "MD-I4 Incendiary Mines");
                break;
            case 60:
                snprintf(nameText, sizeof(nameText), "MD-17 Anti-Tank Mines");
                break;
            case 61:
                snprintf(nameText, sizeof(nameText), "MD-8 Gas Mines");
                break;
            case 62:
                snprintf(nameText, sizeof(nameText), "A/ARC-3 Tesla Tower");
                break;
            case 63:
                snprintf(nameText, sizeof(nameText), "E/MG-101 HMG Emplacement");
                break;
            case 64:
                snprintf(nameText, sizeof(nameText), "E/AT-12 Anti-Tank Emplacement");
                break;
            case 65:
                snprintf(nameText, sizeof(nameText), "E/GL-21 Grenadier Battlement");
                break;
            case 66:
                snprintf(nameText, sizeof(nameText), "FX-12 Shield Generator Relay");
                break;
            case 67:
                snprintf(nameText, sizeof(nameText), "A/MG-43 Machine Gun Sentry");
                break;
            case 68:
                snprintf(nameText, sizeof(nameText), "A/G-16 Gatling Sentry");
                break;
            case 69:
                snprintf(nameText, sizeof(nameText), "A/M-12 Mortar Sentry");
                break;
            case 70:
                snprintf(nameText, sizeof(nameText), "A/AC-8 Autocannon Sentry");
                break;
            case 71:
                snprintf(nameText, sizeof(nameText), "A/MLS-4X Rocket Sentry");
                break;
            case 72:
                snprintf(nameText, sizeof(nameText), "A/M-23 EMS Mortar Sentry");
                break;
            case 73:
                snprintf(nameText, sizeof(nameText), "A/FLAM-40 Flame Sentry");
                break;
            case 74:
                snprintf(nameText, sizeof(nameText), "A/LAS-98 Laser Sentry");
                break;
            case 75:
                snprintf(nameText, sizeof(nameText), "M-102 Fast Recon Vehicle");
                break;
            case 76:
                snprintf(nameText, sizeof(nameText), "EXO-45 Patriot Exosuit");
                break;
            case 77:
                snprintf(nameText, sizeof(nameText), "EXO-49 Emancipator Exosuit");
                break;
            case 78:
                snprintf(nameText, sizeof(nameText), "Reinforce");
                break;
            case 79:
                snprintf(nameText, sizeof(nameText), "Resupply");
                break;
            case 80:
                snprintf(nameText, sizeof(nameText), "Hellbomb");
                break;
            default:
                break;
        }
        canvas_draw_str_aligned(canvas, 4, 12, AlignLeft, AlignBottom, roundText);
        canvas_draw_str_aligned(canvas, 124, 12, AlignRight, AlignBottom, scoreText);

        canvas_set_font(canvas, FontKeyboard);
        canvas_draw_box(canvas, 0, 32, 128, 9);
        canvas_invert_color(canvas);
        canvas_draw_str_aligned(canvas, 2, 37, AlignLeft, AlignCenter, nameText);
        canvas_invert_color(canvas);
        canvas_draw_frame(canvas, 0, 0, 128, 64);
    }

    void draw_icon(
        Canvas* const canvas,
        int8_t pos_x,
        int8_t pos_y,
        int8_t icon_index) {
        const int width = 15;
        const int height = 15;
        pos_x -= width / 2;
        pos_y -= height / 2;

        switch(icon_index) {
            case 0:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_mg_15x15);
                break;
            case 1:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_amr_15x15);
                break;
            case 2:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_stalwart_15x15);
                break;
            case 3:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_eat_15x15);
                break;
            case 4:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_recoilless_15x15);
                break;
            case 5:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_flamethrower_15x15);
                break;
            case 6:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_autocannon_15x15);
                break;
            case 7:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_hmg_15x15);
                break;
            case 8:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_airburst_15x15);
                break;
            case 9:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_commando_15x15);
                break;
            case 10:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_railgun_15x15);
                break;
            case 11:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_spear_15x15);
                break;
            case 12:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_wasp_15x15);
                break;
            case 13:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_grenade_launcher_15x15);
                break;
            case 14:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_laser_cannon_15x15);
                break;
            case 15:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_arc_15x15);
                break;
            case 16:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_quasar_15x15);
                break;
            case 17:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_sterilizer_15x15);
                break;
            case 18:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_flag_15x15);
                break;
            case 19:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_de_escalator_15x15);
                break;
            case 20:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_epoch_15x15);
                break;
            case 21:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_speargun_15x15);
                break;
            case 22:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_eat_napalm_15x15);
                break;
            case 23:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_e_silo_15x15);
                break;
            case 24:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_maxigun_15x15);
                break;
            case 25:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_w_tool_15x15);
                break;
            case 26:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_gatling_15x15);
                break;
            case 27:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_airburst_15x15);
                break;
            case 28:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_120mm_15x15);
                break;
            case 29:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_380mm_15x15);
                break;
            case 30:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_walking_15x15);
                break;
            case 31:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_laser_15x15);
                break;
            case 32:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_napalm_15x15);
                break;
            case 33:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_railcannon_15x15);
                break;
            case 34:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_precision_15x15);
                break;
            case 35:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_gas_15x15);
                break;
            case 36:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_ems_15x15);
                break;
            case 37:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_o_smoke_15x15);
                break;
            case 38:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_e_strafing_15x15);
                break;
            case 39:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_e_airstrike_15x15);
                break;
            case 40:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_e_cluster_15x15);
                break;
            case 41:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_e_napalm_15x15);
                break;
            case 42:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_e_smoke_15x15);
                break;
            case 43:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_e_110mm_15x15);
                break;
            case 44:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_e_500kg_15x15);
                break;
            case 45:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_p_jump_15x15);
                break;
            case 46:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_p_hover_15x15);
                break;
            case 47:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_p_warp_15x15);
                break;
            case 48:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_p_supply_15x15);
                break;
            case 49:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_p_s_generator_15x15);
                break;
            case 50:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_p_s_ballistic_15x15);
                break;
            case 51:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_p_s_directional_15x15);
                break;
            case 52:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_gd_15x15);
                break;
            case 53:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_gd_las_15x15);
                break;
            case 54:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_gd_gas_15x15);
                break;
            case 55:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_gd_arc_15x15);
                break;
            case 56:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_gd_flam_15x15);
                break;
            case 57:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_p_hellbomb_15x15);
                break;
            case 58:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_m_ap_15x15);
                break;
            case 59:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_m_incendiary_15x15);
                break;
            case 60:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_m_antitank_15x15);
                break;
            case 61:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_m_gas_15x15);
                break;
            case 62:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_m_tesla_15x15);
                break;
            case 63:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_emp_hmg_15x15);
                break;
            case 64:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_emp_antitank_15x15);
                break;
            case 65:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_m_battlement_15x15);
                break;
            case 66:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_m_relay_15x15);
                break;
            case 67:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_s_mg_15x15);
                break;
            case 68:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_s_gatling_15x15);
                break;
            case 69:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_s_mortar_15x15);
                break;
            case 70:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_s_autocannon_15x15);
                break;
            case 71:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_s_rocket_15x15);
                break;
            case 72:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_s_ems_mortar_15x15);
                break;
            case 73:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_s_flame_15x15);
                break;
            case 74:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_s_laser_15x15);
                break;
            case 75:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_v_frv_15x15);
                break;
            case 76:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_v_patriot_15x15);
                break;
            case 77:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_v_emancipator_15x15);
                break;
            case 78:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_m_reinforce_15x15);
                break;
            case 79:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_m_resupply_15x15);
                break;
            case 80:
                canvas_draw_icon(canvas, pos_x, pos_y, &I_m_hellbomb_15x15);
                break;
            default:
                break;
        }

    }

    void draw_queue(Canvas* const canvas, PluginState* plugin_state) {
        int spaceX = 15;
        int totalIconsWidth = 5 * spaceX;
        int startX = (128 - totalIconsWidth) / 2 + 2;
        for(int i = 0; i < 6; i++) {
            draw_icon(
                canvas,
                startX + (i * spaceX),
                24,
                plugin_state->stratagem_queue[i]);
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
                50,
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
        } else {
            draw_game_ui(canvas, plugin_state->round, plugin_state->score, plugin_state->stratagem_queue[0]);
            draw_arrows(canvas, plugin_state);
            draw_queue(canvas, plugin_state);
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
                // Update Queue
                for (int i = 0; i < 5; i++) {
                    plugin_state->stratagem_queue[i] = plugin_state->stratagem_queue[i+1];
                }
                plugin_state->stratagem_queue[5] = rand() % 81;
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
