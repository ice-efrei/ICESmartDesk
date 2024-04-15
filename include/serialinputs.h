//
// Created by ICE on 15/04/2024.
//

#ifndef ICESMARTDESK_SERIALINPUTS_H
#define ICESMARTDESK_SERIALINPUTS_H


#include <Arduino.h>

enum {
    UNKNOWN,
    WAIT,
    WAITING,
    WORK,
    WORKING,
    COUNT,
    COUNTING,
    BATCH,
    RUNBATCH,
    UP,
    DOWN,
    ENTER,
    BACK
} State_Type;

typedef struct {
    int ActState;
    long ActDuration;
    int NextState;
    int StateCount;
    bool GetStarttime;
    long Starttime;
    long Worktime;
    String WorkText;
    long TimePerCount;
    long Count;
    long DefaultEndCount;
    bool DefaultEndCountHasChanged;
    long EndCount;
} State_Struct;

extern State_Struct State;
extern String Serialcurrentline;

boolean TimeToChange();
boolean TimeToWork(long wTime);
boolean EndCounterReached(long Criteria);
void PrintActState();
void GetStarttime();
void ChangeState();
void InitStateMachine();
void HandleStateBatch();
void ResetCounter();
void HandleStates();
void PrintHelp();
void HandleSerial();
void HandleCounts(String Cmd);
void SetDefEndCount(String Cmd);
void HandleCommands(String Cmd);
void update_inputs();

#endif //ICESMARTDESK_SERIALINPUTS_H
