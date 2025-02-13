#include "include/logicController.h"
#include "driver/elevio.h"
#include "include/dataStructures.h"
#include "include/instructionQueue.h"
#include "include/utils.h"

void sequenceLogic(State* state, Queue* queue, Output* output){
    if (state->stopButton){
        clearQueue(queue);
    }
    output->lights.stopLight = state->stopButton;
    output->lights.floorIndicator = state->currentFloor;
    lightQueueStops(queue, output);

    //disable timer if inactive
    if (getSysTime() > state->timer.startTime + 3000){
        state->timer.active = false;
    }

    if (state->onFloor && output->motorDirection != STOP){
        //edge case handeling: remember if you are above or below the current floor
        state->directionFromLastFloor = output->motorDirection;
    }

    if (!state->obstruction){
        //edge case handeling: only ever close the door if there is no obstruction
        output->lights.openDoorLight = false;
    }


    if (state->stopButton && state->onFloor){
        //edge case handeling: if the stop button is pressed on a floor, open the door
        output->lights.openDoorLight = true;
    }
}

void noState(Output* output, Queue* queue){
    output->motorDirection = DOWN;
    clearQueue(queue);
}

void onFloor(State* state, Queue* queue, Output* output){

    output->motorDirection = STOP;
    output->lights.openDoorLight = true;

    //this should only happen once, making only posible to enter this function if the timer is active
    removeInstruction(queue, state->currentFloor);

    if (!state->timer.active){
        //activate the timer
        state->timer.startTime = getSysTime();
        state->timer.active = true;
    }

    if (state->obstruction || state->stopButton){
        //reset timer if obstructed or stopped
        state->timer.startTime = getSysTime();
    }
}

void idle(Output* output){
    output->motorDirection = STOP;
}

void moving(Instruction targetInstruction, State* state, Output* output){
    if (state->currentFloor < targetInstruction.targetFloor){
        output->motorDirection = UP;
    }
    else if (state->currentFloor > targetInstruction.targetFloor){
        output->motorDirection = DOWN;
    }
    else{
        //edge case handeling: elevator was last on the target floor
        if (state->directionFromLastFloor == UP){
            output->motorDirection = DOWN;
        }
        else{
            output->motorDirection = UP;
        }
    }
    
}

void controller(State* state, Queue* queue, Output* output){
    if (state->currentFloor == -1){
        noState(output, queue);
        return;
    }

    sequenceLogic(state, queue, output);

    

    //fetch the target instruction
    Instruction targetInstruction;
    if (queue->length >= 1){
        targetInstruction = queue->instructions[0]; 
    }
    else {
        // edge case handleded: queue is empty
        targetInstruction = (Instruction){-2, STOP};
    }


    if ((state->currentFloor == targetInstruction.targetFloor && state->onFloor) || state->timer.active){
        onFloor(state, queue, output);
    }
    else if ((!state->timer.active && state->stopButton) || queue->length < 1){
        idle(output);
    }
    else{
        moving(targetInstruction, state, output);
    }
}