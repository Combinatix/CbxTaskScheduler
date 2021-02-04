# CbxTaskScheduler
### Basic Task Scheduler for Arduino / ESP family with Soft Reset and CPU Load Indication

List of all methods:

```cpp
CbxTaskScheduler task;                              // Constructor
CbxTaskScheduler task(int pin);                     // Constructor with pin number (pin is LOW when CPU is Idle, HIGH when CPU is busy)

void add(void procName, uint32_t interval, bool enabled);   // Add task to the task scheduler
void loop();                                        // Check all tasks, call them if it's time. Call this from main loop as often as possible
void doTasks(uint32_t time);                        // Non-blocking delay - use instead of delay()
void halt(uint32_t time);                           // Watchdog-safe blocking delay()
int getTaskId(void procName);                       // Returns a ProcName's ID
int count();                                        // Returns number of tasks
void schedule(void procName, bool later = false);   // Schedule task: either run ASAP (default) or later, after its interval from now 
void schedule(int taskId, bool later = false);      // Same as above but based on taskId 
void enabled(void procName, enabled);               // Disable / Enable task by ProcName
void enabled(int taskId, bool enabled);             // Disable / Enable task by taskID
bool enabled(void procName);                        // Returns true if ProcName is enabled
bool enabled(int taskId);                           // Returns true if taskID is enabled
void interval(void procName, uint32_t interval);    // Sets the task interval by ProcName
void interval(int id, uint32_t interval);           // Sets the task interval by taskID
uint32_t interval(void procName);                   // Returns current task interval by procName
uint32_t interval(int id);                          // Returns current task interval by taskID
uint32_t lastRun(void procName);                    // Returns system time (in millis) of last run
uint8_t depth();                                    // Returns current taskDepth*
```
Note that **uint32_t** corresponds to **unsigned long**

### Example of use: see the examples

