#ifndef _DEFINES_H_
#define _DEFINES_H_

// process states
#define TASK_STATE_ACTIVE           40
#define TASK_STATE_SLEEP            41
#define TASK_STATE_IDLE             42
#define TASK_STATE_HOLD             43
#define TASK_STATE_HIBERNATE_G      80
#define TASK_STATE_HIBERNATE_L      81

#define HIBERNATE_STATE_MASK        0x80
#define HIBEARNTE_REV_MASK          0x00010000

#define TASK_PRIO_CURRENT           0
#define TASK_PRIO_STASHED           1

#endif