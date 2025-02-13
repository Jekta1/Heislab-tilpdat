#include "include/stateTracker.h"
#include "include/instructionQueue.h"
#include "driver/elevio.h"

void getFloorState(State *state){
    int floorVal = elevio_floorSensor();
    if (floorVal != -1){
        state->currentFloor = floorVal;
        state->onFloor = true;
    }
    else{
        state->onFloor = false;
    }
}

void getObstructionState(State *state){
    state->obstruction = elevio_obstruction();
}

void getStopButtonState(State *state){
    state->stopButton = elevio_stopButton();
}

void getButtonPresses(State *state, Queue *queue){
    for (int i = 0; i < 4; i++){
        if (elevio_callButton(i, BUTTON_HALL_UP)){
            addInstruction(queue, (Instruction){i, UP});
        }
        if (elevio_callButton(i, BUTTON_HALL_DOWN)){
            addInstruction(queue, (Instruction){i, DOWN});
        }
        if (elevio_callButton(i, BUTTON_CAB)){
            addInstruction(queue, (Instruction){i, STOP});
        }
    }
}

void updateState(State *state, Queue *queue){
    getFloorState(state);
    getObstructionState(state);
    getStopButtonState(state);
    getButtonPresses(state, queue);
}