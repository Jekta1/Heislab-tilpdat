#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

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

typedef enum Direction{
    UP,
    DOWN
};

void executeOutput(const Output* output);

#endif // OUTPUTMANAGER_H