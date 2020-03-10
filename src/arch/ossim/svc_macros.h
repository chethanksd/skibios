#ifndef _SVC_MACROS_H_
#define _SVC_MACROS_H_

#ifdef ARCH_SVC_VERSION_MAJOR
#error "ARCH_SVC_VERSION_MAJOR is defined elsewhere in the program"
#endif

#ifdef ARCH_SVC_VERSION_MINOR
#error "ARCH_SVC_VERSION_MINOR is defined elsewhere in the program"
#endif

#define ARCH_SVC_VERSION_MAJOR      1
#define ARCH_SVC_VERSION_MINOR      1


#endif