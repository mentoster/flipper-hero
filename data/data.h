#include <input/input.h>
#include <gui/icon.h>
#pragma once
typedef enum {
    EventTypeTick,
    EventTypeKey,
} EventType;

typedef struct {
    EventType type;
    InputEvent input;
} PluginEvent;

typedef struct {
    FuriMutex* mutex;
    char arrowDirections[8]; // Store directions of up to 8 arrows
    bool arrowFilled[8]; // Track whether each arrow is filled
    int numArrows; // Number of arrows currently active
    int nextArrowToFill; // Index of the next arrow to fill
    int queue_spot;
    char name_buf[32];
    char seq_buf[9];
    int stratagem_queue[20];
    int score;
    int round;
    int timer;
    int32_t record_score;
    int32_t record_round;
    int bonuses[3]; // round bonus at 0, time bonus at 1, perfect bonus at 2.
    bool isGameStarted;
    bool isScoreScreen;
    bool isPreRound;
    bool isRoundOver;
    bool perfect;
    bool isGameOver;
} PluginState;



typedef enum {
    ArrowDirectionUp,
    ArrowDirectionRight,
    ArrowDirectionLeft,
    ArrowDirectionDown
} ArrowDirection;
