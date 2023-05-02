//
// Created by brian on 9/15/16.
//

#ifndef INC_05_C_JOYSTICK_H
#define INC_05_C_JOYSTICK_H

enum eJoystickDirections
{
    JOYSTICK_UP = 0,
    JOYSTICK_DOWN,
    JOYSTICK_LEFT,
    JOYSTICK_RIGHT,
    JOYSTICK_CENTER,
    JOYSTICK_MAX_NUMBER_DIRECTIONS      // Get the number of directions via the enum
};

_Bool Joystick_init();

_Bool Joystick_isDirectionPressed(enum eJoystickDirections direction);
const char* Joystick_getDirectionName(enum eJoystickDirections direction);



#endif //INC_05_C_JOYSTICK_H
