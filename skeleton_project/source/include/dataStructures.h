#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <stdbool.h>

typedef enum {
    UP,
    DOWN,
    STOP
} Direction;

typedef struct {
    int currentFloor;
    Timer timer;
    bool onFloor;
    bool obstruction;
    bool stopButton;
} State;

typedef struct {
    double startTime;
    bool active;
} Timer;

typedef struct {
    Direction motorDirection;
    Lights lights;
} Output;
typedef struct {
    bool floorLights[4];
    bool buttonLights[4][2]; // [floor][up/down]. 0 is up, 1 is down. [0][1] and [3][0] unused
    bool internalButtonLights[4];
    bool openDoorLight;
    bool stopLight;
} Lights;


typedef struct {
    int targetFloor;
    Direction direction; //in the context of an Instruction, STOP represents an internal button press
} Instruction;

typedef struct {
    int length;
    int capacity;
    Instruction* instructions;
} Queue;

#endif // DATASTRUCTURES_H