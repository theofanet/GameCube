//
//  main.cpp
//  GameTest
//
//  Created by Theo Fanet on 15/02/2015.
//  Copyright (c) 2015 Theo. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>

#include "GameHandler.h"

GameHandler *handler;

void display(void){
    handler->display();
}

void idle(void){
    handler->idle();
}

void keyboard(unsigned char key, int x, int y){
    handler->keyboard(key, x, y);
}

void keyboard_up(unsigned char key, int x, int y){
    handler->keyboard_up(key, x, y);
}

void specialKeyboard(int key, int x, int y){
    handler->specialKeyboard(key, x, y);
}

void specialKeyboard_up(int key, int x, int y){
    handler->specialKeyboard_up(key, x, y);
}

void reshape(int w, int h){
    handler->reshape(w, h);
}

void mouse_click(int button, int state, int x, int y){
    handler->mouse_click(button, state, x, y);
}

void mouse_motion(int x, int y){
    handler->mouse_motion(x, y);
}

void mouse_passive_motion(int x, int y){
    handler->mouse_passive_motion(x, y);
}

void Clear(void){
    delete_obj(handler);
}

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition (100, 50);
    glutCreateWindow(WINDOW_TITLE);
    
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);
    glutSpecialFunc(specialKeyboard);
    glutSpecialUpFunc(specialKeyboard_up);
    glutMouseFunc(mouse_click);
    glutPassiveMotionFunc(mouse_passive_motion);
    glutMotionFunc(mouse_motion);
    
    atexit(Clear);
    
    handler = new GameHandler();
    handler->run();
    
    return 0;
}
