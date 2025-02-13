#include "include/instructionQueue.h"
#include "include/dataStructures.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

void insertToQueue(Queue* queue, Instruction instruction, int index){
    //insert an element in the (potentially) middle of the queue
    assert(index >= 0 && index < queue->length);

    for (int i = index; i < queue->length+1; i++){
        Instruction temp = queue->instructions[i];
        queue->instructions[i] = instruction;
        instruction = temp;
    }
    queue->length++;

}

void smartInsert(const State* state, Queue* queue, Instruction instruction){
    //should we use directionFromLastFloor? How to set?
    Instruction prevFloor = (Instruction){state->currentFloor, state->directionFromLastFloor};
    for (int i = 0; i < queue->length; i++){

        //check up direction
        if (queue->instructions[i].targetFloor >= instruction.targetFloor && instruction.targetFloor >= prevFloor.targetFloor){
            
            //edge-case handeling:
            if (prevFloor.targetFloor != instruction.targetFloor || (prevFloor.direction == DOWN && i == 0)){

                if (instruction.direction == UP || instruction.direction == STOP){
                    insertToQueue(queue, instruction, i);
                    return;
                }
            }
        }

        //check down direction
        if (queue->instructions[i].targetFloor <= instruction.targetFloor && instruction.targetFloor <= prevFloor.targetFloor){
            if (prevFloor.targetFloor != instruction.targetFloor || (prevFloor.direction == UP && i == 0)){
                if (instruction.direction == DOWN || instruction.direction == STOP){
                    insertToQueue(queue, instruction, i);
                    return;
                }
            }
        }
        prevFloor = queue->instructions[i];

    }
    //insertion failed, add to end of queue
    queue->instructions[queue->length] = instruction;
    queue->length++;
}

void addInstruction(Queue* queue, Instruction instruction, const State* state){
    //Only add instructions that are not already in the queue
    for (int i = 0; i < queue->length; i++){
        if (queue->instructions[i].targetFloor == instruction.targetFloor && queue->instructions[i].direction == instruction.direction){
            return;
        }
    }

    if (queue->instructions == NULL || queue->length == 0){
        //initialise the queue, since class constructors are illegal now, apparantly
        queue->length = 0;
        queue->capacity = 1;
        queue->instructions = (Instruction*)malloc(queue->capacity * sizeof(Instruction));
    }

    if (queue->length >= queue->capacity){
        //alloc more memory if needed
        queue->capacity *= 2;
        queue->instructions = (Instruction*)realloc(queue->instructions, queue->capacity * sizeof(Instruction));
    }

    smartInsert(state, queue, instruction);
}

void clearQueue(Queue* queue){
    free(queue->instructions);
    queue->instructions = NULL;
    queue->length = 0;
    queue->capacity = 0;
}

void removeInstruction(Queue* queue, int currentFloor){
    //ok a linked list might acctually have been better in this case, but now im commited
    for (int i = 0; i < queue->length; i++){
        if (queue->instructions[i].targetFloor == currentFloor){

            //shift all instructions one step back to fill the gap
            for (int j = i; j < queue->length - 1; j++){
                queue->instructions[j] = queue->instructions[j+1];
            }
            queue->length--;
            return;
        }
    }
}

void resetQueueLights(Output* output){
    for (int i = 0; i < 4; i++){
        output->lights.internalButtonLights[i] = false;
    }
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 2; j++){
            output->lights.buttonLights[i][j] = false;
        }
    }
}



void lightQueueStops(const Queue* queue, Output* output){

    resetQueueLights(output);

    for (int i = 0; i < queue->length; i++){
        if (queue->instructions[i].direction == STOP){
            output->lights.internalButtonLights[queue->instructions[i].targetFloor] = true;
        }
        else if (queue->instructions[i].direction == UP){
            output->lights.buttonLights[queue->instructions[i].targetFloor][0] = true;
        }
        else if (queue->instructions[i].direction == DOWN){
            output->lights.buttonLights[queue->instructions[i].targetFloor][1] = true;
        }
    }
}