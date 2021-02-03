# CbxTaskScheduler
### Basic Task Scheduler for Arduino / ESP family with Soft Reset and CPU Load Indication

List of all methods:

```cpp
CbxTaskScheduler task;                                      // Constructor
CbxTaskScheduler task(int pin);                             // Constructor with pin number (pin is LOW, when CPU is Idle, HIGH when CPU is busy)

void add(void procName, uint32_t interval, bool enabled);   // Add task to the task scheduler
void loop();                                                // Check all tasks, call them if it's time. Call this from main loop as often as possible
void doTasks(uint32_t time);                                // Non-blocking delay - use instead of delay()
void halt(uint32_t time);                                   // Watchdog-safe blocking delay()
void scheduleTask(void procName, bool later = false);       // Schedule task: either run ASAP (default) or later, after its interval from now 
void enabled(void procName, enabled);                       // Disable / Enable task by ProcName
void enabled(uint16_t taskId, bool enabled);                // Disable / Enable task by taskID
bool enabled(void procName);                                // Returns true if ProcName is enabled
bool enabled(uint16_t taskId);                              // Returns true if taskID is enabled
void interval(void procName, uint32_t interval);            // Sets the task interval by ProcName
void interval(uint16_t id, uint32_t interval);              // Sets the task interval by taskID
uint32_t interval(void procName);                           // Returns current task interval by procName
uint32_t interval(uint16_t id);                             // Returns current task interval by taskID
uint16_t getTaskId(void procName);                          // Returns a ProcName's ID
uint32_t lastRun(void procName);                            // Returns system time (in millis) of last run
uint8_t depth();                                            // Returns current taskDepth*
```

Example of use:

```c++
#include <CbxTaskScheduler.h>

// Create object
CbxTaskScheduler task;

void setup(){
    Serial.begin(115200);

    // task.add (procName, interval, optional enabled = true);
    task.add(task1, 33, false);     // Run task1() every 33ms, disabled by default
    task.add(task2, 250);           // Run task2() every 250ms
    task.add(task3, 1000);          // Run task3() every 1000ms
    task.add(task4, 2000, false);   // Run task4() every 2000ms, disabled by default
    task.add(taskEnable, 10000);    // Run taskEnable() every 10s
}

void loop() {
    task.loop();
}

// Your procedures:

void task1() {
    Serial.print("*");
}
void task2() {
    Serial.print("2 ");
}
void task3() {
    Serial.print("3! ");
}
void task4() {
    Serial.print("\nNewLine:  ");
}
void taskEnable() {
    if (task.enabled(task1)) {
        task.enabled(task1, false);
        task.enabled(task4, false);
    } else {
        task.enabled(task1, true);
        task.enabled(task4, true);
    }
}
```
Note that uint32_t equals to unsigned long, uint16_t equals to unsigned int
