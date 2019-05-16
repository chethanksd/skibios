#ifndef _DEFINES_H
#define _DEFINES_H

// process states
#define PROCESS_STATE_ACTIVE        40
#define PROCESS_STATE_SLEEP         41
#define PROCESS_STATE_IDLE          42
#define PROCESS_STATE_HOLD          43
#define PROCESS_STATE_HIBERNATE_G   80
#define PROCESS_STATE_HIBERNATE_L   81

#define HIBERNATE_STATE_MASK        0x80
#define HIBEARNTE_REV_MASK          0x00010000

#define PROCESS_PRIO_CURRENT        0
#define PROCESS_PRIO_STASHED        1

#endif