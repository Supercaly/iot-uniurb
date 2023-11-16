#ifndef BUTTON_H
#define BUTTON_H

#ifdef HAS_BUTTON
#define BUTTON_TASK_STACK_SIZE 4096
#define BUTTON_TASK_PRIORITY 5
#define BUTTON_TASK_CORE tskNO_AFFINITY

/*
 * Function representing a task running on the board
 * with the purpose of handling the button presses.
 */
void button_task_code(void *args);
#endif // HAS_BUTTON

#endif // BUTTON_H