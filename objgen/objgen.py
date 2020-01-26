#
# Script that executes objgen process
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


def run_objgen():

    global process_obj
    global process_output
    global process_error
    global process_rcode

    global mpc_array_kbss_size
    global mpc_array_kdat_size

    global upc_kbss_size
    global upc_kdat_size

    global actual_kbss_size
    global actual_kdat_size


    basic_param = ' BIN_PATH=' + svar.bin_path
    basic_param = basic_param + ' ROOT_DIR=' + svar.repo_path
    basic_param = basic_param + ' ARCH_PATH=' + svar.repo_path + '/src/arch/' + device.arch

    #
    # OBJGEN STAGE 1: Invoke symgen
    #
    #
    diagnostics.objgen_stage = 1

    print('***** Generating Symbol Header *****')
    symgen_invoke = svar.repo_path + "/tools/make/make.exe symgen -f" + svar.repo_path + "objgen/makefile"
    symgen_invoke = symgen_invoke + ' BUILD_PATH=' + svar.build_path
    symgen_invoke = symgen_invoke + basic_param

    try:
        process_obj = subprocess.Popen(symgen_invoke, stdout=subprocess.PIPE, shell=True)
    except:
        diagnostics.error = ecode.ERROR_SYMBOL_GEN_FAILED
        exit(1)

    try:
        (process_output, process_error) = process_obj.communicate()
        process_rcode = process_obj.returncode
    except:
        diagnostics.error = ecode.ERROR_SYMBOL_GEN_FAILED
        exit(1)

    if(process_rcode != 0):
        diagnostics.error = ecode.ERROR_SYMBOL_GEN_FAILED
        exit(1)

    process_obj.wait()


    #
    # OBJGEN STAGE 2: Unit Proess Count Compilation
    #
    #
    diagnostics.objgen_stage = 2

    print('***** Calculating Resources *****')

    bpath_exists = os.path.isdir(svar.build_path + '/resource_cal')
    if not bpath_exists:
        os.mkdir(svar.build_path + '/resource_cal')

    resource_invoke = svar.repo_path + "/tools/make/make.exe resource -f" + svar.repo_path + "objgen/makefile"
    resource_invoke = resource_invoke + ' BUILD_PATH=' + svar.build_path + '/resource_cal'
    resource_invoke = resource_invoke + basic_param

    try:
        process_obj = subprocess.Popen(resource_invoke, stdout=subprocess.PIPE, shell=True)
    except:
        diagnostics.error = ecode.ERROR_UPC_RESOURCE_CALCULATION_FALIED
        exit(1)

    try:
        (process_output, process_error) = process_obj.communicate()
        process_rcode = process_obj.returncode
    except:
        diagnostics.error = ecode.ERROR_UPC_RESOURCE_CALCULATION_FALIED
        exit(1)

    if(process_rcode != 0):
        diagnostics.error = ecode.ERROR_UPC_RESOURCE_CALCULATION_FALIED
        exit(1)


    #
    # OBJGEN STAGE 3: UPC Resource file and kvar file check and readout
    #
    #
    diagnostics.objgen_stage = 3

    resouce_txt_exists = os.path.isfile(svar.build_path + "/resource_cal/resource.txt")
    if not resouce_txt_exists:
        diagnostics.error = ecode.ERROR_UPC_RESOURCE_FILE_NOT_FOUND
        exit(1)

    try:
        resource_file = open(svar.build_path + "/resource_cal/resource.txt", 'r') 
        objdump_lines = resource_file.readlines() 
        resource_file.close()
    except:
        diagnostics.error = ecode.ERROR_UPC_RESOURCE_FILE_FAILED
        exit(1)

    kvar_exists = os.path.isfile(svar.repo_path + "/src/kvar.c")
    if not kvar_exists:
        diagnostics.error = ecode.ERROR_KVAR_SRC_FILE_NOT_FOUND
        exit(1)

    try:
        kvar_file = open(svar.repo_path + "/src/kvar.c", 'r')
        kvar_lines = kvar_file.readlines()
        kvar_file.close()
    except:
        diagnostics.error = ecode.ERROR_KVAR_SRC_FILE_FAILED
        exit(1)

    #
    # OBJGEN STAGE 4: UPC Resource file and kvar LBL analysis
    #
    #
    diagnostics.objgen_stage = 4

    upc_kdat_size = 0
    upc_kbss_size = 0
    mpc_array_kbss_size = 0
    mpc_array_kdat_size = 0

    for objdump_line in objdump_lines:

        columns = objdump_line.split() 

        if(len(columns) != 6):
            continue

        if((columns[5] == ".kdat") or (columns[5] == ".kbss")):
            continue

        if(columns[3] == ".kdat"):
            upc_kdat_size = upc_kdat_size + int(columns[4])
        elif(columns[3] == ".kbss"):
            upc_kbss_size = upc_kbss_size + int(columns[4])
        else:
            continue

        for kvar_line in kvar_lines:

            if(("KSECTION" in kvar_line) and ("MAX_PROCESS_COUNT" in kvar_line)):

                if(columns[5] in kvar_line):

                    if(columns[3] == ".kdat"):
                        mpc_array_kdat_size = mpc_array_kdat_size + int(columns[4])

                    if(columns[3] == ".kbss"):
                        mpc_array_kbss_size = mpc_array_kbss_size + int(columns[4])
    
    print('upc_kdat_size = ', upc_kdat_size) 
    print('upc_kbss_size = ', upc_kbss_size)
    print("mpc_kdat_size = ", mpc_array_kdat_size)
    print("mpc_kbss_size = ", mpc_array_kbss_size)

    #
    # OBJGEN STAGE 5: Resouce allocation feasibility check 
    #
    #
    diagnostics.objgen_stage = 5

    temp = 0
    temp = int(sparam.upper_region_size) + int(sparam.ghmb_region_size)
    temp = int(sparam.kernel_section_size) - temp
    temp = temp * 1024
    temp = temp / int(sparam.process_stack_size)

    sparam.max_process_count = temp
    
    if (sparam.max_process_count < 1):
        diagnostics.error = ecode.ERROR_NO_MEMORY_FOR_PROCESS_STACK
        exit(1)

    kstack_size = 0
    kstack_size = kstack_size + (mpc_array_kbss_size * int(sparam.max_process_count))
    kstack_size = kstack_size + (mpc_array_kdat_size * int(sparam.max_process_count))
    kstack_size = kstack_size + (upc_kbss_size - mpc_array_kbss_size)
    kstack_size = kstack_size + (upc_kdat_size - mpc_array_kdat_size)
    kstack_size = kstack_size + (int(device.intvec_size) * 4)

    kstack_size = (int(sparam.upper_region_size) * 1024) - kstack_size

    if(kstack_size < 1024):
        diagnostics.error = ecode.ERROR_KERNEL_STACK_SIZE_VIOLATION
        exit(1)

    actual_kbss_size = upc_kbss_size - mpc_array_kbss_size
    actual_kbss_size = actual_kbss_size + (mpc_array_kbss_size * int(sparam.max_process_count))
    actual_kdat_size = upc_kdat_size - mpc_array_kdat_size
    actual_kdat_size = actual_kdat_size + (mpc_array_kdat_size * int(sparam.max_process_count))

    print('kernel stack size: ', kstack_size)
    print('max process count: ', sparam.max_process_count)
    print('actual kbss size:', actual_kbss_size)
    print('actual kdat size:', actual_kdat_size)