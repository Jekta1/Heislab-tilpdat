#include "include/utils.h"


double getSysTime(){
    return (double)clock()/CLOCKS_PER_SEC;
}

void logState(const State* state){
    printf("Observed state: \n");
    printf("Current floor: %d\n", state->currentFloor);
    printf("On floor: %d\n", state->onFloor);
    printf("Obstruction: %d\n", state->obstruction);
    printf("Stop button: %d\n", state->stopButton);
}

void logTimer(const Timer* timer){
    printf("Timer: \n");
    printf("Start time: %f\n", timer->startTime);
    printf("Active: %d\n", timer->active);
}

void logQueue(const Queue* queue){
    printf("Queue: \n");
    printf("Length: %d\n", queue->length);
    for(int i = 0; i < queue->length; i++){
        printf("Instruction %d: %d\n", i, queue->instructions[i].targetFloor);
    }
}

void logOutputs(const Output* output){
    printf("Outputs: \n");
    printf("Motor direction: %d\n", output->motorDirection);
    printf("Lights: \n");
    printf("Floor light: %d\n", output->lights.floorIndicator);
    printf("Button lights: ");
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 2; j++){
            printf("%d ", output->lights.buttonLights[i][j]);
        }
        printf("\n");
    }
    printf("\nInternal button lights: ");
    for(int i = 0; i < 4; i++){
        printf("%d ", output->lights.internalButtonLights[i]);
    }
    printf("\nOpen door light: %d\n", output->lights.openDoorLight);
    printf("Stop light: %d\n", output->lights.stopLight);
}