#
# OBJGEN Script diagnostics
#
# author : chetandev.ksd@gmail.com
#
#

import ecode
import sparam

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


def run_objgen_debug_commands():

    #
    # Print parameter list if enabled
    #

    if(sparam.slist['print_param_list'] == 1):
        print("***** Printing parameter list *****")
        for param in sparam.param_list:
            print(param + " : " + str(sparam.slist[param]))


    #
    # Print param gen list if enabled
    #

    if(sparam.slist['print_param_gen'] == 1):
        print("***** Printing param.h include list *****")
        for param in sparam.param_gen_list:
            print(param + " : " + str(sparam.param_gen_sym[param]))