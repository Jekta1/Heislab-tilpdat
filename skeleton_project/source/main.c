#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"


/*
Structure:

main driver: manages the different modules, dermining when they should do which work. Sub-modules:

    State finder: Run at program start to initiate a defined state. Initiates a state tracker in its place once a defined state has been reached

    State tracker (input manager): interfaces w. hardware to update the current state of the system. 
        - current floor
        - obstruction
        - stop-button
        - ect.

    Helper Module: Instruction queue:
        - Add to queue given inputs (entry point is the State tracker)
        - Internal logic for queue: what instructions should be executed first (entry point is adding to queue)
        - Recomends next floor to visit (entry point is the Logic controller)
        - Clears queue when requested (entry point is the Logic controller)


    Logic controller #split maybe?: 
        - initiate timed stop if a floor with a relevant instruction in queue has been reached
        - delays the timed stop while an obstruction is active
        - sets outputs to a halt when requested
        - creates ouput instructions to the output manger
        - sets all relevant lights
        - interfaces with instruction queue

    Output manager:
        - interfaces w. hardware to realise instructions
        - extra failsafe that outputs are not given if stop or obstruction are active



Module designs:

Main Driver:
    - public state, queue, outputs init() - initialises the state, queue and outputs
    - public void run() - entry point, runs either state finder or driver depending on if a state has been found
        - private bool stateFinder(&state, &outputs) - runs pipeline to find a defined state: first running state finder. returns wether a state has been found
        - private void driver(&state, &queue, &outputs) - runs pipeline to manage the system: first running state tracker, then logic controller, then output manager

State finder:
    - public bool checkState(&state, &outputs) - interfacing, public function. returns a bool indicating wether the state has been reached. calls
        - private void setOutputs(&outputs) - interfaces with the ouput manager to move the elevator
        - private void initialiseState(&state) - sets the initial state of the system, once a state has been found. 
            Once accived, the main driver will override the state finder with the state tracker and logic controller

State tracker:
    - public struct representing the state of the system
    - private struct representing buttons pressed
    - public updateState(&state, &queue) - entry point, updates the state of the system by calling these sub-functions
                also modifies a reference to the queue struct, adding new instructions to it
        - private getFloorState(&state) - updates the current floor and wether the elevator is at a floor
        - private getObstructionState(&state) - updates the obstruction state
        - private getStopButtonState(&state) - updates the stop button state
        - private getButtonPresses(&queue) - creates an internal struct representing the buttons pressed
            - interfaces to Instruction queue, adding new instructions to it and performing internal logic for the queue

Instruction queue:
    - public struct representing the queue. contains a list of instructions
    - public void addInstruction(&queue, const &instruction, const &state) - entry point, adds a new instruction to the queue. calls sortInstructions before returning
        - private void sortInstructions(&queue) - sorts the instructions in the queue, deciding which order they should be performed in

    public clearQueue(&queue) - entry point, clears the queue
    public clearInstruction(&queue, const int currentFloor) - entry point, clears the instruction at the current floor

Logic Controller:
    - public void controll(&state, &queue, &outputs) - entry point for controll. 
        determines which sub-functions to call, given the state. also clears any instructions that end at the current floor
        
        - private bool manageTimer(&state) - if the timer is not acitve desipite having reached the target, activates tge timer. 
            checks for conditions that should reset the timer and returns a bool indicating wether the timer is active, eg. we should wait
        - private void setTarget(const &state, const &queue, &outputs) - sets the target floor of the elevator.
        - private void setLights(const &state, &outputs) - updates output light instructions
        - private void setMotorState(const &state, &outputs) - sets the motor state of the elevator, based on weather the elevator should move or not and what the target is

Output manager:
    - public void realiseInstructions(const &outputs) - entry point. asserts no illigal outputs are given
        - private void setMotor(const &outputs) - sets the motor direction and turns it on/off
        - private void setLights(const &outputs) - sets all lights of the elevator:
            - floor lights
            - button lights
            - open door light

*/


int main(){
    elevio_init();
    
    printf("=== Example Program ===\n");
    printf("Press the stop button on the elevator panel to exit\n");

    elevio_motorDirection(DIRN_UP);

    while(1){
        int floor = elevio_floorSensor();

        //This is integer printing in C
        printf("%d\n", floor);

        if(floor == 0){
            elevio_motorDirection(DIRN_UP);
        }

        if(floor == N_FLOORS-1){
            elevio_motorDirection(DIRN_DOWN);
        }


        for(int f = 0; f < N_FLOORS; f++){
            for(int b = 0; b < N_BUTTONS; b++){
                int btnPressed = elevio_callButton(f, b);
                elevio_buttonLamp(f, b, btnPressed);
            }
        }

        if(elevio_obstruction()){
            elevio_stopLamp(1);
        } else {
            elevio_stopLamp(0);
        }
        
        if(elevio_stopButton()){
            elevio_motorDirection(DIRN_STOP);
            break;
        }
        
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
    }

    return 0;
}
