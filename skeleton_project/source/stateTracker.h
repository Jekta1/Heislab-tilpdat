#ifndef STATETRACKER_H
#define STATETRACKER_H

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

void updateState(State* state);

#endif // STATETRACKER_H