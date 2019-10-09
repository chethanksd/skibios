#
# OBJGEN Script diagnostics
#
# author : chetandev.ksd@gmail.com
#
#

import ecode

start_stage     = 0
pregen_stage    = 0
objgen_stage    = 0
postgen_stage   = 0

STAGE_DONE    = 20

error = ecode.ERROR_NONE
error_message = ''

def run_diagnostics():

    if(start_stage != STAGE_DONE):
        print('script failed at start stage: ', start_stage)
    elif(pregen_stage != STAGE_DONE):
        print('script failed at pregen stage: ', pregen_stage)
    elif(objgen_stage != STAGE_DONE):
        print('script failed at objgen stage: ', objgen_stage)
    elif(objgen_stage != STAGE_DONE):
        print('script failed at postgen stage: ', postgen_stage)
