/*
  Example of a State Machine                      2022-01-31       ec2021

  The states can change on different criteria:

  Time:
       The state WORKING changes after a given time
       using the function "TimeToChange"
  Counter:
       The state COUNTING changes after an EndCount was reached
       using the function "EndCounterReached"
  Direct:
      The states WORK, WAIT, COUNT and BATCH are used to prepare the corresponding
      states WORKING, WAITING, COUNTING and RUNBATCH. Therefore they are passed only
      once and link directly to their corresponding "main" state.
  Externally:
      All repetitive states can be quit by Serial command.
      The state WAITING (entered via WAIT) can only be quit by Serial command.

  If two states shall be performed one after another this can be initiated
  by State.ActState = <FirstState> and State.NextState = <NextState> without changing
  the state machine. However this way it is also possible to create new state that
  calls two or more States one after another (it must keep track of the sequence
  by its own of course).

  The enum State_Type lists names for different states:

  enum {
    UNKNOWN,                  Kind of a "Null Pointer" telling that no legal state is used
    WAIT,                     Prepares for WAITING
    WAITING,                  Idles until quit by a Serial command
    WORK,                     Prepares for WORKING
    WORKING,                  In this example: Pretends to work for a certain time
    COUNT,                    Prepares for COUNTING
    COUNTING                  Counts until the counter reaches the limit "EndCount"
    BATCH,                    Prepares RUNBATCH
    RUNBATCH                  Example how to automatically control the state machine
  } State_Type;

  The structure State_Struct organizes the relevant data for the actual State and points
  to the following (next) State:

  typedef struct {
   int  ActState;               Actual State to be performed (value from enum State_Type)
   long ActDuration;            Time how long the State shall be performed
   int  NextState;              The State to change to when the ActState is done
   int StateCount;              Counter used for "Batch processing" of States
   bool GetStarttime;           Boolean to control the acquisition of Starttime
   long Starttime;              Time in millis() when the ActState started
   long Worktime;               Time in millis() used to control an internal function of "WORKING"
   String WorkText;             Text written during the state "WORKING"
   long TimePerCount;           Interval in millis() for counting
   long Count;                  The variable that counts upwards until EndCount
   long DefaultEndCount;        Used as default for EndCount (can be changed via Serial)
   bool DefaultEndCountHasChanged Forces EndCount to change when "COUNT" is called
   long EndCount;               End value for COUNTING (default or temporarily changed via Serial)
  } State_Struct;

  Input "?" or "Help" via Serial to get a list of the available commands.

  You may play around with the State Machine, e.g. by command "Count 40" and then command "WAIT"
  during the progress of COUNTING to see what happens. You can always quit states by "WAIT".
  Up to now there is no "Stop" and "Resume" command available ... Of course this is possible:
  It would require to store different data and restoring them after "Resume", including
  - where appropriate - the consideration of the "remaining time-to-go", to finalize the
  stopped state. Still something to do for the future ...


*/

#include <Arduino.h>
#include "serialinputs.h"

// Global Variables
State_Struct State;
String Serialcurrentline = "";

// State Functions
boolean TimeToChange() {
    return (millis() - State.Starttime >= State.ActDuration);
}

boolean TimeToWork(long wTime) {
    if (millis() - State.Worktime >= wTime) {
        State.Worktime = millis();
        return true;
    } else return false;
}

boolean EndCounterReached(long Criteria) {
    return (Criteria >= State.EndCount);
}

void PrintActState() {
    String sState = "UNKNOWN";
    switch (State.ActState) {
        case WAIT     : sState = "WAIT"; break;
        case WAITING  : sState = "WAITING"; break;
        case WORK     : sState = "WORK"; break;
        case WORKING  : sState = "WORKING"; break;
        case COUNT    : sState = "COUNT"; break;
        case COUNTING : sState = "COUNTING"; break;
        case BATCH    : sState = "BATCH"; break;
        case RUNBATCH : sState = "RUNBATCH"; break;
        case UP       : sState = "UP"; break;
        case DOWN     : sState = "DOWN"; break;
        case ENTER    : sState = "ENTER"; break;
        case BACK     : sState = "BACK"; break;
    }
    Serial.println();
    Serial.println(sState);
}

void GetStarttime() {
    if (State.GetStarttime) {
        State.GetStarttime = false;
        State.Starttime = millis();
        State.Worktime  = State.Starttime;
    }
}

void ChangeState() {
    if (!State.NextState == UNKNOWN) {
        State.ActState = State.NextState;
        State.NextState = UNKNOWN;
    } else State.ActState = WAIT;

}

void InitStateMachine() {
    State.ActState = WAIT;
    State.NextState = UNKNOWN;
}

void HandleStateBatch() {
    State.NextState = RUNBATCH;
    switch (State.StateCount) {
        case 0 : State.ActState  = COUNT;          // Using the default entry to COUNT/COUNTING
            State.EndCount = 3;                      // But change the EndCount which is not
            break;                                   // reset by COUNT
        case 1 :  State.ActDuration = 3200;        // By setting the data like in the state "WORK"
            State.ActState     = WORKING;            // we can jump to "WORKING" directly
            State.GetStarttime = true;               // and overwrite the defaults from "WORK"
            State.WorkText = "Batch in Progress.. ";
            State.ActState = WORKING;
            PrintActState();
            break;
        case 2 : State.ActState = COUNT;           // see above
            State.EndCount = 7;
            break;
        default: State.ActState = WAIT;            // Finally back to WAIT
            break;
    }
    State.StateCount++;
}

void ResetCounter() {
    State.EndCount = State.DefaultEndCount;
    State.Count    = 0;

}

void HandleStates() {
    switch (State.ActState) {
        case WAIT :    State.ActDuration = 0;
            State.ActState     = WAITING;
            State.GetStarttime = true;
            PrintActState();
            break;
        case WAITING : // Wait forever or until Serial Cmd
            break;
        case WORK  :   State.ActDuration = 5200;
            State.GetStarttime = true;
            State.WorkText = "Work in Progress.. ";
            State.ActState     = WORKING;
            PrintActState();
            break;
        case WORKING : GetStarttime();
            if (TimeToWork(1000)) Serial.print(State.WorkText);
            if (TimeToChange()) {
                Serial.println();
                ChangeState();
            };
            break;
        case COUNT  :  State.Count        = 0;
            State.TimePerCount = 500;
            State.ActState      = COUNTING;
            State.GetStarttime = true;
            if (State.DefaultEndCountHasChanged) ResetCounter();
            PrintActState();
            break;
        case COUNTING: GetStarttime();
            if (TimeToWork(State.TimePerCount)) {
                Serial.print(++State.Count);
                if (State.Count < State.EndCount) Serial.print(F(","));
            }
            if (EndCounterReached(State.Count)) {
                ResetCounter();
                Serial.println();
                ChangeState();
            };
            break;
        case BATCH:   State.StateCount    = 0;
            State.ActState      = RUNBATCH;
            break;
        case RUNBATCH : HandleStateBatch();
            break;
        case UP:
            State.NextState = WAIT;
            break;
        case DOWN:
            State.NextState = WAIT;
            break;
        case ENTER:
            State.NextState = WAIT;
            break;
        case BACK:
            State.NextState = WAIT;
            break;
        default :      State.ActState = WAIT;
            break;
    }

}

// Help

void PrintHelp() {
    Serial.println(F("Example State Machine"));
    Serial.println(F("---------------------"));
    Serial.println(F("Commands:"));
    Serial.println(F("Help or ?           : Print this help text"));
    Serial.println(F("Wait                : Do nothing but wait for next serial command"));
    Serial.println(F("Work                : Pretend to work ..."));
    Serial.println(F("Count               : Count to EndCount (default = 10, 2 counts per second)"));
    Serial.println(F("Count <value>       : Count to <value> = {1,2,3, ...}"));
    Serial.println(F("Both                : Do Work followed by Count "));
    Serial.println(F("Batch               : Perform a batch of states "));
    Serial.println(F("DefEndCount         : Prints value of default EndCount"));
    Serial.println(F("SetEndCount <value> : Sets default EndCount to <value> = {1,2,3, ..."));
    Serial.println();
}


// Serial Functions

void HandleSerial() {
    while (Serial.available()) {
        char c = Serial.read();
        bool CR = (c == char(10));
        if (c >= ' ') {
            Serialcurrentline += c;
        };
        if (CR)      {
            HandleCommands(Serialcurrentline);
            Serialcurrentline = "";
        };
    }
}

void HandleCounts(String Cmd) {
    if (Cmd.length() > 5) {
        long counts = Cmd.substring(6).toInt();
        if (counts <= 0) counts = 1;
        State.EndCount = counts;
    } else State.EndCount = State.DefaultEndCount;
    State.ActState = COUNT;
}

void SetDefEndCount(String Cmd) {
    if (Cmd.length() > sizeof(F("SetEndCount"))) {
        long counts = Cmd.substring(12).toInt();
        if (counts <= 0) counts = 1;
        State.DefaultEndCount = counts;
        State.DefaultEndCountHasChanged = true;
        Serial.print(F(" > Default EndCount set to "));
        Serial.print(counts);
        Serial.println(F(" < "));
    };
}

void HandleCommands(String Cmd) {
    bool unknown = true;
    if (Cmd.startsWith(F("Help")) || Cmd.startsWith(F("?")) )  {
        PrintHelp();
        unknown = false;
    };
    if (Cmd.startsWith(F("Wait")))  {
        State.ActState = WAIT;
        unknown = false;
    };
    if (Cmd.startsWith(F("Work")))  {
        State.ActState = WORK;
        unknown = false;
    };
    if (Cmd.startsWith(F("Count"))) {
        HandleCounts(Cmd);
        unknown = false;
    };
    if (Cmd.startsWith(F("Both")))  {
        State.ActState = WORK;
        State.NextState  = COUNT;
        unknown = false;
    };
    if (Cmd.startsWith(F("Batch"))) {
        State.ActState = BATCH;
        State.NextState  = UNKNOWN;
        unknown = false;
    };
    if (Cmd.startsWith(F("DefEndCount"))) {
        Serial.print(F("Default EndCount = "));
        Serial.println(State.DefaultEndCount);
        unknown = false;
    };
    if (Cmd.startsWith(F("SetEndCount"))) {
        SetDefEndCount(Cmd);
        unknown = false;
    };
    if (Cmd.startsWith(F("Up"))) {
        State.ActState = UP;
        unknown = false;
    };
    if (Cmd.startsWith(F("Down"))) {
        State.ActState = DOWN;
        unknown = false;
    };
    if (Cmd.startsWith(F("Enter"))) {
        State.ActState = ENTER;
        unknown = false;
    };
    if (Cmd.startsWith(F("Back"))) {
        State.ActState = BACK;
        unknown = false;
    };
    if (unknown) {
        Serial.print (Cmd);
        Serial.println(F(" is unknown! For Help input \"Help\" or \"?\" "));
    };
}

void update_inputs(){
    HandleSerial();
    HandleStates();
}

/*
// Setup

void setup() {
    Serial.begin(115200);
    Serial.println(F("Example State Machine"));
    InitStateMachine();  // Not really necessary ;-)
}

// Loop

void loop() {
    HandleSerial();
    HandleStates();
}
*/