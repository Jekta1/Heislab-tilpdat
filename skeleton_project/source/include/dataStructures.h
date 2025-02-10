#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

typedef struct State{
    int currentFloor;
    struct Timer timer;
    bool onFloor;
    bool obstruction;
    bool stopButton;
};

typedef struct Timer{
    double startTime;
    bool active;
};

typedef struct Output{
    Direction motorDirection;
    bool motorOn;
    struct Lights lights;
};
typedef struct Lights{
    bool floorLights[4];
    bool buttonLights[3][2];
    bool internalButtonLights[4];
    bool openDoorLight;
    bool stopLight;
};

typedef struct Queue{
    int length;
    Instruction* instructions;
};
typedef struct Instruction{
    int targetFloor;
};

typedef enum Direction{
    UP,
    DOWN
};

#endif // DATASTRUCTURES_H