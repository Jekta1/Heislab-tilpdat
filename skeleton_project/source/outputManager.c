#include "include/outputManager.h"
#include "driver/elevio.h"
#include "include/dataStructures.h"

void setMotor(const Output* output){
    switch (output->motorDirection) {
        case STOP:
            elevio_motorDirection(DIRN_STOP);
            break;
        
        case UP:
            elevio_motorDirection(DIRN_UP);
            break;
        
        case DOWN:
            elevio_motorDirection(DIRN_DOWN);
            break;
    }
}

void setLights(const Output* output){
    for (int i = 0; i < 4; i++){
        elevio_buttonLamp(i, BUTTON_CAB, output->lights.internalButtonLights[i]);
    }
    for (int i = 0; i < 4; i++){
        elevio_buttonLamp(i, BUTTON_HALL_UP, output->lights.buttonLights[i][0]);
        elevio_buttonLamp(i, BUTTON_HALL_DOWN, output->lights.buttonLights[i][1]);
    }
    
    if (output->lights.floorIndicator > -1){
        elevio_floorIndicator(output->lights.floorIndicator);
    }
    elevio_doorOpenLamp(output->lights.openDoorLight);  
}
void executeOutput(const Output* output){
    setMotor(output);
    setLights(output);
}