#ifndef _APP_SUPPORT_H_
#define _APP_SUPPORT_H_

extern uint32_t call_kernel_service(uint32_t svc_code, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4);


#define svc(code) call_kernel_service(code, 0, 0, 0, 0)

#endif