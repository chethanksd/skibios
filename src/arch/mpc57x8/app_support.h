#ifndef _APP_SUPPORT_H_
#define _APP_SUPPORT_H_

#define svc(code) \
    asm volatile ("e_li    3, %[i_code]     \n\t" \
                  "se_sc                    \n\t" \
                  :: [i_code] "I" (code) :  \
                  )

#endif