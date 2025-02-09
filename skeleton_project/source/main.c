#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "driver/elevio.h"


/*
Structure:

main driver: manages the different modules, dermining when they should do which work. Sub-modules:
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


    Logic controller state machine:
        - finds a defined state if none has been reached
        - initiate timed stop if a floor with a relevant instruction in queue has been reached
        - delays the timed stop while an obstruction is active
        - sets outputs to a halt when requested
        - creates ouput instructions to the output manger
        - interfaces with instruction queue

    Output manager:
        - interfaces w. hardware to realise instructions
        - extra failsafe that outputs are not given if stop or obstruction are active



Module designs:

Main Driver:
    - public state, queue, outputs init() - initialises the state, queue and outputs
    - public void run() - entry point, runs either state finder or driver depending on if a state has been found

Utils: any utility functions that may be usefull but not related to a certain module.
    - retrieving the system time
    - logging functions for state, queue and outputs.

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

    - public clearQueue(&queue) - entry point, clears the queue
    - public clearInstruction(&queue, const int currentFloor) - entry point, clears the instruction at the current floor
    - public lightQueueStops(const &queue, &outputs) - sets the output lights based on the queue

Logic Controller Finite State Machine: 
    The current floor number and direction of movement are not considered as states, as they do not change the behaviour in a state.
    State is read both from the inputs and queue.
    - possible states:
        1 Non-defined state - move down untill state is defined. 
        2 On floor, waiting - turn on open door button. If stop or obstruction active, reset the timer. Run clearInstruction for the current floor.
        3 Stopping while moving - turn off motor output, preserving all other inputs. This state is also entered if there is no target.
        4 Moving - set motor inputs based on queue target.
        
    - State transitions happen based on a change of the observed state of the physical system. Conditions for each state:
        1: currentFloor == -1, the init value
        2: currentFloor == target or timer active (if the timer runs out, the controller, not the state should deactivate it)
        3: !onFloor && stopButton || queue.length < 1
        4: currentFloor != target && !timer.active 
            (conditions uneccecary as an "else" statement should suffice, but for error catching they are provided)
        5: else: ambigous (error) state
        
    - Outputs:
        1: No lights, motorOn = true, motorDir = DOWN
        2: queue lights, openDoorLight = true, motorOn = false
        3: queue lights, motorOn = false
        4: queue lights, openDoorLight = false, motorOn = true, motorDir = Towards target.

    Implementation:
    - public void controll(&state, &queue, &outputs) - entry point for controll.
        
Output manager:
    - public void realiseInstructions(const &outputs) - entry point. asserts no illigal outputs are given
        - private void setMotor(const &outputs) - sets the motor direction and turns it on/off
        - private void setLights(const &outputs) - sets all lights of the elevator:
            - floor lights
            - button lights
            - open door light


Structs:
state:
    - int currentFloor
    - timer timer
    - bool onFloor
    - bool obstruction
    - bool stopButton

timer:
    - double startTime
    - bool active

queue:
    - int length
    - instruction instructions[10]

outputs:
    - bool motorDirection
    - bool motorOn
    - lights lights  

lights:
    - bool floorLights[4]
    - bool buttonLights[3][2]
    - bool internalButtonLights[4]
    - bool openDoorLight

Enums:
UP and DOWN

Tests:
Integration testing: ---
Unit testing: ---
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
