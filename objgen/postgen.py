#
# Script that executes post objgen process
#
# author : chetandev.ksd@gmail.com
#
#

import sys, os
import subprocess
import time

import diagnostics
import device
import ecode
import svar
import device
import sparam
import objgen


def run_postgen():

    #
    # POSTGEN STAGE 1: Linker support file generation
    #
    #
    diagnostics.postgen_stage = 1

    generate_support_linker_script()

    #
    # POSTGEN STAGE 2: Generate objgen summary report
    #
    #
    diagnostics.postgen_stage = 2

    #
    # POSTGEN STAGE: DONE
    #
    #
    diagnostics.postgen_stage = diagnostics.STAGE_DONE  



#
# Local Funnction: function to generate support linker script to integrate skibios
#
#

def generate_support_linker_script():

    param_lds = open(svar.build_path + "/param.lds", "w+")

    temp = '_kernel_base_address = ' + str(sparam.kernel_base_address) + ';\n'
    param_lds.write(temp)

    temp = '_app_base_address = ' + str(sparam.app_base_address) + ';\n'
    param_lds.write(temp)

    temp = '_kstack_end_addr = ' + str(sparam.kernel_stack_end_address) + ';\n'
    param_lds.write(temp)

    temp = '_proc_heap_addr = ' + str(sparam.proc_heap_address) + ';\n'
    param_lds.write(temp)

    temp = '_ghmb_addr = ' + str(sparam.ghmb_address) + ';\n'
    param_lds.write(temp)

    param_lds.close()
