/*****************************************************************************************************************************\

	Name:       	CbxTaskScheduler.h
	Created:		03/05/2020 17:51:22
	Author:     	Combinatix
	Description:	Header file

\*****************************************************************************************************************************/

#ifndef CBXTASKSCHEDULER_H
#define CBXTASKSCHEDULER_H

#include <Arduino.h>

#ifdef __AVR__
#include <avr/wdt.h>
#endif //__AVR__

class CbxTaskScheduler{
	struct STask{								// Task structure
		void (*proc)();							// Process callback
		uint32_t interval;						// How often the program should run in ms
		bool enabled;							// Task is enabled if true, disabled if false
		uint32_t lastRun;						// last run in millis()
	};

public:
	CbxTaskScheduler(int ledPin = -1, bool wdtEnable = false);		// Constructor
	~CbxTaskScheduler();											// Destructor
	void add(void (*proc)(), uint32_t delay = 0, bool enabled = true);	// Add task to the task scheduler
	void loop();													// Call this from main loop as often as possible
	void doTasks(uint32_t ms = 0);									// Non-blocking delay - to be called instead of delay().
	void halt(uint32_t t);											// Watchdog-safe blocking delay()
	int getTaskId(void (*proc)());									// Returns a process ID
	int count();													// Returns number of tasks
	void schedule(void (*proc)(), bool later = false);				// Schedule task immediate or after its interval
	void schedule(int taskId, bool later = false);					// Schedule task immediate or after its interval
	void enabled(void (*proc)(), bool enabled);						// Disable / Enable task by proc name
	void enabled(int taskId, bool enabled);							// Disable / Enable task by taskID
	bool enabled(void (*procName)());								// Returns true if ProcName is enabled
	bool enabled(int taskId);										// Returns true if taskID is enabled
	void interval(void (*procName)(), uint32_t interval);			// Sets the task interval by procName
	void interval(int taskId, uint32_t interval);					// Sets the task interval by taskID
	uint32_t interval(void (*procName)());							// Returns current task interval by procName
	uint32_t interval(int taskId);									// Returns current task interval by taskID
	uint32_t lastRun(void (*procName)());							// Returns system time (in millis) of last run
	uint8_t depth();												// Returns current taskDepth
#ifdef __AVR__
	void (*softReset)() = 0;										// Arduino family software reset
#endif // __AVR__
#ifdef ESP8266
	void softReset();												// ESP8266 family software reset
#endif // ESP8266

private:
	STask *task;								// Pointer to Tasks structure
	int tasks = 0;								// Number of tasks
	int taskLed = -1;							// Task LED Port - High when CPU is busy. Set to -1 do disable taskLed
	int id;										// Current task index
	uint8_t taskDepth = 0;						// Task Depth
};

#endif //CBXTASKSCHEDULER_H