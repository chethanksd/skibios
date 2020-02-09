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

    kernel_base_address = int(sparam.kernel_sram_address) + int(device.intvec_size) * 4
    temp = '_kernel_base_address = ' + str(kernel_base_address) + ';\n'
    param_lds.write(temp)

    app_base_address = int(sparam.kernel_sram_address) + (int(sparam.kernel_section_size) * 1024)
    temp = '_app_base_address = ' + str(app_base_address) + ';\n'
    param_lds.write(temp)

    process_stack_address = int(sparam.kernel_sram_address) + (int(sparam.upper_region_size) * 1024)
    kernel_stack_end_address = process_stack_address - 4
    temp = '_kstack_end_addr = ' + str(kernel_stack_end_address) + ';\n'
    param_lds.write(temp)

    temp = '_proc_heap_addr = ' + str(process_stack_address) + ';\n'
    param_lds.write(temp)

    ghmb_address = app_base_address - (int(sparam.ghmb_region_size) * 1024)
    temp = '_ghmb_addr = ' + str(ghmb_address) + ';\n'
    param_lds.write(temp)


    param_lds.close()
