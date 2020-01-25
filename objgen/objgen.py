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


        