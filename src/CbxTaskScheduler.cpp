/*****************************************************************************************************************************\

    Name:           CbxTaskScheduler.cpp
    Created:        03/05/2020 17:51:22
    Author:         Combinatix
    Description:    CPP file

\*****************************************************************************************************************************/

#include "Arduino.h"
#include "CbxTaskScheduler.h"

CbxTaskScheduler::CbxTaskScheduler(int ledPin, bool wdtEnable) {                    // Constructor
    taskLed = ledPin;
    if (taskLed != -1) pinMode(taskLed, OUTPUT);                                    // Set LED pin to OUTPUT
#ifdef __AVR__
    if (wdtEnable) {
        wdt_enable(WDTO_2S);                                                        // Enable watchdog
    } else {
        wdt_disable();                                                              // Disable watchdog
    }
#endif
}

CbxTaskScheduler::~CbxTaskScheduler() {                                             // Destructor
    if (taskLed != -1) pinMode(taskLed, INPUT);                                     // Release pin
}
void CbxTaskScheduler::add(void (*proc)(), uint32_t delay, bool enabled){           // Add task to the task scheduler
    noInterrupts();
    task = (STask *)realloc(task, sizeof(STask) * (tasks + 1));
    task[tasks].proc = proc;
    task[tasks].interval = delay;
    task[tasks].enabled = enabled;
    task[tasks].lastRun = millis();
    tasks++;
    interrupts();
}
void CbxTaskScheduler::loop(){                                                      // Call this from main loop as often as possible
    uint32_t now;
    for (id = 0; id < tasks; id++) {                                                // Loop as long as we have tasks left
        now = millis();                                                             // Store current millis()
        if ((now - task[id].lastRun >= task[id].interval) && task[id].enabled) {    // If it's time
            task[id].lastRun = now;                                                 // store the time when it was called
            if (taskLed != -1) digitalWrite(taskLed, true);                         // Turn on LED during the task
            (*task[id].proc)();                                                     // Fire the task.
            if (taskLed != -1) digitalWrite(taskLed, false);                        // LED off
        }
    }
#ifdef __AVR__
    wdt_reset();                                                                    // Watchdog reset
#endif
}
void CbxTaskScheduler::doTasks(uint32_t ms) {                                       // Non-blocking delay - to be called instead of delay().
    /*  This will proceed other tasks for specified time instead of waiting and doing nothing.
        Remember: Other tasks may (and probably will) block CPU a little bit longer than it was intended.
        doTasks is no good for situations where you need exact timing - see wait()
        It is possible to call doTasks() within another process during doTasks() - see taskDepth
        The taskDepth limit is hardcoded to 8. It means the functions can call doTasks up to 8 levels deep
    */
    taskDepth++;
    if (taskLed != -1) digitalWrite(taskLed, false);            // Turn on LED during the task
    if (taskDepth < 8) {                                        // If taskDepth < 8
        if (ms) {                                               // and delay > 0
            uint32_t start = millis();                          // Do main loop for time specified in ms, instead of waiting
            while ((millis() - start) < ms) loop();
        }
        else {
            loop();                                             // Run loop (only once)
        }
    }
    else {                                                      // If taskDepth is too big then
        if (ms) halt(ms);                                       // just halt()
    }
    if (taskLed != -1) digitalWrite(taskLed, true);             // Turn on LED as we return back to task
    taskDepth--;
#ifdef __AVR__
    wdt_reset();                                                // Watchdog reset
#endif
}
void CbxTaskScheduler::halt(uint32_t t) {                       // Watchdog-safe blocking delay()
    uint32_t now = millis();
    while (millis() - now < t) {
#ifdef __AVR__
        wdt_reset();                                            // keep resetting watchdog until timeout
#endif
    }
}
int CbxTaskScheduler::getTaskId(void (*proc)()) {               // Returns a task ID
    for (int i = 0; i < tasks; i++) {
        if (task[i].proc == proc) return i;
    }
    return -1;
}
int CbxTaskScheduler::count() {
    return tasks;
}
void CbxTaskScheduler::schedule(void (*proc)(), bool later) {   // Schedule task immediately for next run, or after specified interval
    int i = getTaskId(proc);
    if (later) {
        // Schedule task to run after its interval from now
        task[i].lastRun = millis();
    } else {
        // Schedule task to run immediately at next loop
        task[i].lastRun = millis() - task[i].interval;
    }
}
void CbxTaskScheduler::schedule(int taskId, bool later) {       // Schedule task immediately for next run, or after specified interval
    if (later) {
        // Schedule task to run after its interval from now
        task[taskId].lastRun = millis();
    } else {
        // Schedule task to run immediately at next loop
        task[taskId].lastRun = millis() - task[taskId].interval;
    }
}
void CbxTaskScheduler::enabled(void (*proc)(), bool enabled) {              // Disable / Enable task by proc name
    int i = getTaskId(proc);
    task[i].enabled = enabled;
}
void CbxTaskScheduler::enabled(int taskId, bool enabled) {                  // Disable / Enable task by taskID
    task[taskId].enabled = enabled;
}
bool CbxTaskScheduler::enabled(void (*procName)()) {                        // Returns True if ProcName is enabled
    int i = getTaskId(procName);
    return task[i].enabled;
}
bool CbxTaskScheduler::enabled(int taskId) {                                // Returns true if taskID is enabled
    return task[taskId].enabled;
}
void CbxTaskScheduler::interval(void (*procName)(), uint32_t interval){     // Sets the task interval by procName
    task[getTaskId(procName)].interval = interval;
}
void CbxTaskScheduler::interval(int taskId, uint32_t interval){             // Sets the task interval by ID
    task[taskId].interval = interval;
}
uint32_t CbxTaskScheduler::interval(void (*procName)()){                    // Returns current task interval by procName
    return task[getTaskId(procName)].interval;
}
uint32_t CbxTaskScheduler::interval(int taskId){                            // Returns current task interval by procName
    return task[taskId].interval;
}
uint32_t CbxTaskScheduler::lastRun(void (*procName)()){
    int i = getTaskId(procName);
    return task[i].lastRun;
}
uint8_t CbxTaskScheduler::depth() {                                         // Returns current taskDepth
    return taskDepth;
}

#ifdef __AVR__
// void (*softReset)() = 0;                                                 // Software Reset in header file
#endif // __AVR__
#ifdef ESP8266
void CbxTaskScheduler::softReset() {
    ESP.reset();
}
#endif // ESP8266
