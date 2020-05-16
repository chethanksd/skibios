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
    basic_param = basic_param + ' ARCH_PATH=' + svar.repo_path + '/src/arch/' + device.dlist['arch']

    #
    # OBJGEN SKIP Check
    #
    #
    if(sparam.slist['skip_objgen'] == 1):
        print('***** Skipping Objgen *****')
        print('Done!')
        exit(0)

    #
    # OBJGEN STAGE 1: Invoke symgen
    #
    #
    diagnostics.objgen_stage = 1

    print('***** Generating Symbol Header *****')

    spath_exists = os.path.isdir(svar.build_path + '/symgen')
    if not spath_exists:
        os.mkdir(svar.build_path + '/symgen')

    call_make_target('symgen', basic_param, ecode.ERROR_SYMBOL_GEN_FAILED)


    #
    # OBJGEN STAGE 2: Unit Proess Count Compilation
    #
    #
    diagnostics.objgen_stage = 2

    print('***** Calculating Resources *****')

    rpath_exists = os.path.isdir(svar.build_path + '/resource_cal')
    if not rpath_exists:
        os.mkdir(svar.build_path + '/resource_cal')

    call_make_target('resource', basic_param, ecode.ERROR_UPC_RESOURCE_CALCULATION_FALIED)

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

    #
    # OBJGEN STAGE 5: Resource allocation feasibility check 
    #
    #
    diagnostics.objgen_stage = 5

    temp = 0
    temp = sparam.slist['upper_region_size'] + sparam.slist['ghmb_region_size']
    temp = sparam.slist['kernel_section_size'] - temp
    temp = temp * 1024
    temp = temp / sparam.slist['process_stack_size']

    sparam.max_process_count = temp
    
    if (sparam.max_process_count < 1):
        diagnostics.error = ecode.ERROR_NO_MEMORY_FOR_PROCESS_STACK
        exit(1)

    kstack_size = 0
    kstack_size = kstack_size + (mpc_array_kbss_size * int(sparam.max_process_count))
    kstack_size = kstack_size + (mpc_array_kdat_size * int(sparam.max_process_count))
    kstack_size = kstack_size + (upc_kbss_size - mpc_array_kbss_size)
    kstack_size = kstack_size + (upc_kdat_size - mpc_array_kdat_size)
    kstack_size = kstack_size + (device.dlist['intcnt'] * 4)

    kstack_size = (sparam.slist['upper_region_size'] * 1024) - kstack_size

    if(kstack_size < 1024):
        diagnostics.error = ecode.ERROR_KERNEL_STACK_SIZE_VIOLATION
        exit(1)

    actual_kbss_size = upc_kbss_size - mpc_array_kbss_size
    actual_kbss_size = actual_kbss_size + (mpc_array_kbss_size * int(sparam.max_process_count))
    actual_kdat_size = upc_kdat_size - mpc_array_kdat_size
    actual_kdat_size = actual_kdat_size + (mpc_array_kdat_size * int(sparam.max_process_count))

    #
    # OBJGEN STAGE 6: Final resource allocation calculations
    #
    #
    diagnostics.objgen_stage = 6

    sparam.kernel_base_address = sparam.slist['kernel_sram_address'] + device.dlist['intcnt'] * 4
    sparam.app_base_address = sparam.slist['kernel_sram_address'] + (sparam.slist['kernel_section_size'] * 1024)
    sparam.ghmb_address = sparam.app_base_address - (sparam.slist['ghmb_region_size'] * 1024)

    sparam.proc_heap_address = sparam.slist['kernel_sram_address'] + (sparam.slist['upper_region_size'] * 1024)
    sparam.kernel_stack_end_address = sparam.proc_heap_address - 4

    #
    # OBJGEN STAGE 7: Generate param header file
    #
    #
    diagnostics.objgen_stage = 7

    generate_param_header()


    #
    # OBJGEN STAGE 8: Run sripts to customize skibios for target device 
    #
    #
    diagnostics.objgen_stage = 8

    print('***** Customizing for target device *****')

    try:
        arch_specific_objgen = getattr(device.devattrb_module, 'arch_specific_objgen')
        arch_specific_objgen()
    except Exception as e:
        diagnostics.error = ecode.ERROR_ARCH_OBJGEN_NOT_DEFINED
        diagnostics.error_message = str(e)
        exit(1)

    #
    # OBJGEN STAGE 9: copy required source files to allsrc
    #
    #
    diagnostics.objgen_stage = 9

    print('***** Copying required files to allsrc *****')

    call_make_target('allsrc_copy', basic_param, ecode.ERROR_SOURCE_FILE_COPY_ERROR)

    #
    # OBJGEN STAGE 10: Compile files in allsrc to generate obj files
    #
    #
    diagnostics.objgen_stage = 10

    if(sparam.slist['skip_compile'] == 1):
        print('***** Skipping Compilation of source files *****')
        return

    print('***** Compiling source files *****')

    opath_exists = os.path.isdir(svar.build_path + '/obj')
    if not opath_exists:
        os.mkdir(svar.build_path + '/obj')

    call_make_target('compile_allsrc', basic_param, ecode.ERROR_OBJECT_GENERATION_ERROR)

    
    #
    # OBJGEN STAGE: DONE
    #
    #
    diagnostics.objgen_stage = diagnostics.STAGE_DONE



#
# Local Function: function to call make target
#
#

def call_make_target(target, param, error_code):

    make_invoke = svar.repo_path + "/tools/make/make.exe " + target + " -f" + svar.repo_path + "objgen/makefile"
    make_invoke = make_invoke + ' BUILD_PATH=' + svar.build_path
    make_invoke = make_invoke + param

    try:
        process_obj = subprocess.Popen(make_invoke, stdout=subprocess.PIPE, universal_newlines=True, shell=True)
        if(target == 'compile_allsrc'):
            while True:
                output = process_obj.stdout.readline()
                if output == '' and process_obj.poll() is not None:
                    break
                if output:
                    print(output.strip())
    except:
        diagnostics.error = error_code
        exit(1)

    try:

        (process_output, process_error) = process_obj.communicate()
        process_rcode = process_obj.returncode
    except:
        diagnostics.error = error_code
        exit(1)

    if(process_rcode != 0):
        diagnostics.error = error_code
        exit(1)

    process_obj.wait()


#
# Local Function: function to genereate param header file
#
#

def generate_param_header():

    param_header = open(svar.build_path + "/allsrc/param.h", "w+")

    param_header.write("#ifndef _PARAM_H_\n")
    param_header.write("#define _PARAM_H_\n")
    param_header.write("\n")

    param_header.write("#include <arch_param.h>\n")
    param_header.write("\n\n")

    param_header.write("// Common Symbolic Constants\n\n")

    temp = "#define NUM_OF_INTERRUPTS " + str(device.dlist['intcnt']) + "\n"
    param_header.write(temp)

    temp = "#define MAX_PROCESS_COUNT " + str(int(sparam.max_process_count)) + "\n"
    param_header.write(temp)

    temp = "#define PROCESS_STACK_SIZE " + str(int(sparam.slist['process_stack_size'] / 4)) + "\n"
    param_header.write(temp)

    temp = "#define ENABLE_SKIBIOS 1 \n"
    param_header.write(temp)

    temp = "#define PROCESS_PER_SEC " + str(sparam.slist['process_per_sec']) + "\n"
    param_header.write(temp)

    temp = "#define HEAP_BOOKEEPING_SIZE " + str(sparam.slist['ghmb_region_size'] * 256) + "\n"
    param_header.write(temp)

    temp = "#define KERNEL_START_ADDRESS " + str(sparam.slist['kernel_sram_address']) + "\n"
    param_header.write(temp)

    #
    # inlcude parameters registered in <param_gen> of XSD schema
    #
    for param in sparam.param_gen_list:
        temp = "#define " + str(sparam.param_gen_sym[param]) + " " + str(sparam.slist[param]) + "\n"
        param_header.write(temp)

    param_header.write("\n\n")
    param_header.write("#endif")
    param_header.close()