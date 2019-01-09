#ifndef PARAM_H
#define PARAM_H

/* scheduler Parameters */
#ifndef PROCESS_PER_SEC
#define PROCESS_PER_SEC  100
#endif


/* Process Parameters*/
#ifndef PROCESS_ID_START
#define PROCESS_ID_START    4000
#endif

#define PROCESS_TIME_SHARE_LOW      1
#define PROCESS_TIME_SHARE_MEDIUM   4
#define PROCESS_TIME_SHARE_HIGH     8

#endif