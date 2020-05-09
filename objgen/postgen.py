#
# Script that executes post objgen process
#
# author : chetandev.ksd@gmail.com
#
#

import sys, os
import subprocess
import datetime

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
    #SS
    #
    diagnostics.postgen_stage = 1

    generate_support_linker_script()

    #
    # POSTGEN STAGE 2: Generate objgen summary report
    #
    #
    diagnostics.postgen_stage = 2

    generate_summary_report()

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

    temp = '_kernel_start_address = ' + str(sparam.slist['kernel_sram_address']) + ';\n'
    param_lds.write(temp)

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


#
# Local Funnction: function to skibios generate summary report
#
#

def generate_summary_report():

    summary_report = open(svar.build_path + "/summary_report.txt", "w+")

    temp = "SKIBIOS Object Generator Summary Report\n\n"
    summary_report.write(temp)

    temp = datetime.datetime.now().strftime("%a, %d %B %Y        %H:%M:%S") + "\n\n"
    summary_report.write(temp)

    temp = ""
    temp = temp + "Target arch: " + device.arch + "    "
    temp = temp + "Target device: " + device.partno + "\n"
    summary_report.write(temp)

    temp = "---------------------------------------------------------\n\n"
    summary_report.write(temp)

    # write common input parameters

    temp = "### Common Input Parameters ###\n"
    summary_report.write(temp)

    temp = ""
    temp = temp + "kernel section size      : " + str(sparam.slist['kernel_section_size']) + "\n"
    temp = temp + "upper region size        : " + str(sparam.slist['upper_region_size']) + "\n"
    temp = temp + "process_stack_size       : " + sparam.process_stack_size + "\n"
    temp = temp + "ghmb_region_size         : " + sparam.ghmb_region_size + "\n"
    temp = temp + "process per sec          : " + sparam.process_per_sec + "\n"

    summary_report.write(temp)
    summary_report.write("\n\n")

    # write commputed parameters

    temp = "### Computed Input Parameters ###\n"
    summary_report.write(temp)

    temp = ""
    temp = temp + "max process count        : " + str(int(sparam.max_process_count)) + "\n"

    summary_report.write(temp)
    summary_report.write("\n\n")

    # common device parameters

    temp = "### Common Device Parameters ###\n"
    summary_report.write(temp)

    temp = ""
    temp = temp + "flash size               : " + str(int(int(device.flash_size)/1024)) + "KB \n"
    temp = temp + "sram size                : " + str(int(int(device.sram_size)/1024)) + "KB \n"
    temp = temp + "\n"

    temp = temp + "flash start address      : " + str(hex(int(device.flash_addr))) + "\n"
    temp = temp + "sram start address       : " + str(hex(int(device.sram_addr))) + "\n"
    temp = temp + "\n"

    temp = temp + "interrupt vector count   : " + str(int(device.intvec_size)) + "\n"

    summary_report.write(temp)
    summary_report.write("\n\n")

    # memory map 
    temp = "### Memory Map ###\n\n"
    summary_report.write(temp)

    temp = ""
    temp = temp + "********************************************\n"
    temp = temp + "Kernel Start             : " + str(hex(int(sparam.slist['kernel_sram_address']))) + "\n"
    temp = temp + "********************************************\n"
    temp = temp + "Vector Table Start       : " + str(hex(int(sparam.slist['kernel_sram_address']))) + "\n"
    temp = temp + "********************************************\n"

    total_vtable_size = int(device.intvec_size) * 4
    vector_table_end_address = sparam.slist['kernel_sram_address'] + total_vtable_size - 1
    temp = temp + "Vector Table\n"
    temp = temp + "Size                     : " + device.intvec_size + " x 4 = " + str(total_vtable_size) + "B\n"
    temp = temp + "********************************************\n"
    temp = temp + "Vector Table End         : " + str(hex(vector_table_end_address)) + "\n"
    temp = temp + "********************************************\n"

    kvar_total_size = objgen.actual_kbss_size + objgen.actual_kdat_size
    kvar_end_address = vector_table_end_address + kvar_total_size
    temp = temp + "Kernel Var Start         : " + str(hex(int(sparam.kernel_base_address))) + "\n"
    temp = temp + "********************************************\n"
    temp = temp + "Kernel Variables \n"
    temp = temp + "Size                     : " + str(kvar_total_size) + "B\n"
    temp = temp + "********************************************\n"
    temp = temp + "Kernel Var End           : " + str(hex(int(kvar_end_address))) + "\n"
    temp = temp + "********************************************\n"

    summary_report.write(temp)

    summary_report.close()
