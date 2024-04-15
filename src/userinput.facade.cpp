//
// Created by ICE on 15/04/2024.
//

#include "userinput.facade.h"
#include "serialinputs.h"

bool user_inputs[4] = {
    0, // right
    0, // left
    0, // enter
    0 // back
};

bool old_user_inputs[4] = {
    0, // right
    0, // left
    0, // enter
    0 // back
};

void update_user_inputs(){
    update_inputs();
}

enum {
    RIGHT,
    LEFT,
    ENTER,
    BACK
} UserInput;