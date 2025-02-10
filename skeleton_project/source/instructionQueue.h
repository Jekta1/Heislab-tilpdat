#ifndef INSTRUCTIONQUEUE_H
#define INSTRUCTIONQUEUE_H

typedef struct Queue{
    int length;
    Instruction* instructions;
};
typedef struct Instruction{
    int targetFloor;
};

#endif // InstructionQueue_H