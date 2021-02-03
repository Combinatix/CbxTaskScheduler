/*
    Name:       CbxTaskScheduler.ino
    Created:    02/02/2021 22:27:53
    Author:     Combinatix
 */

#include <CbxTaskScheduler.h>

// Create task scheduler
//CbxTaskScheduler task;

// Or, create task scheduler with LED that shine when CPU is busy
CbxTaskScheduler task(LED_BUILTIN);

void setup(){
    Serial.begin(115200);
    Serial.println("\n\n --- Reset ---\n\n");

    // task.add (procName, interval, optional enabled = true);
    task.add(task0);                // Run task0() every loop
    task.add(task1, 33, false);     // Run task1() every 33ms, disabled by default
    task.add(task2, 250);           // Run task2() every 250ms
    task.add(task3, 1000);          // Run task3() every 1s
    task.add(task4, 2000, false);   // Run task4() every 2s, disabled by default
    task.add(taskEnable, 10000);    // Run taskEnable() every 10s
    task.add(reset, 60000);         // Reset every 1m
    task.add(example, 12000);
    // TaskId is zero based. Tasks are processed in the same order in which they were added.
    // In this case task0 will have taskId = 0, ... taskEnable will have taskId = 5, etc.
}

void loop() {
    task.loop();
}

// Your procedures:
void task0() {
    // Do something every loop, like keyboard scan etc.
    // This is very fast, it would be unpractical to print here
}
void task1() {
    Serial.print(task.depth());
}
void task2() {
    Serial.print("* ");
}
void task3() {
    Serial.print("#> ");
    // Non-blocking delay 500ms:
    task.doTasks(400);
    Serial.print("<# ");
}
void task4() {
    Serial.print("\nNewLine:  ");
}
void taskEnable() {
    // This demonstrates how to disable / enable tasks
    if (task.enabled(task1)) {
        task.enabled(task0, false);
        task.enabled(task1, false);
        task.enabled(task4, false);
    } else {
    // task0 makes CPU busy, as it is fired every loop.
    // watch the built-in LED, when task0 is enabled
        task.enabled(task0, true);
        task.enabled(task1, true);
        task.enabled(task4, true);
    }
    // Run example() ASAP
    task.schedule(example);
}

void reset() {
    task.softReset();
}

void example() {
    Serial.print(F("\nExample:\n________________\n"));
    // To get task ID:
    Serial.print(F("My taskId is "));
    Serial.println(task.getTaskId(example));

    // To iterate thru all tasks
    for (int i = 0; i < task.count(); i++) {
        Serial.print(i);
        Serial.print(" - ");
        Serial.println(task.interval(i));
        // To enable all tasks:
        // task.enabled(i, true);
    }
    // To schedule 3rd task to run after its interval from now:
    task.schedule(3, true);

    // If you need to wait someTime then call task.doTasks(someTime); instead of delay(someTime);
    // doTasks() passes CPU to other tasks, rather than waiting and doing nothing.
    // task.depth() increases every time doTasks() is called. If you do 
    task.doTasks(1000);
    // then when task scheduler fire the task0(); it will increase value until doTasks() finishes:
    Serial.print("- Done\n");
    // Note that there is doTasks(500); also used in task3(), see the result in terminal window.
    // Maximum taskDepth is hardcoded to 8 to prevent stack overflow.
}
