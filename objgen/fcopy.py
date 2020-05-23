import sys, os
import subprocess
import shutil
import time

import diagnostics
import device
import objgen
import ecode
import svar



def run_allsrc_copy():

    basic_param = ' BIN_PATH=' + svar.bin_path
    basic_param = basic_param + ' ROOT_DIR=' + svar.repo_path
    basic_param = basic_param + ' ARCH_PATH=' + svar.repo_path + '/src/arch/' + device.dlist['arch']

    #
    # creat misc, inc and api directory
    #



    #
    # Get list of files in allsrc
    #
    allsrc_files = os.listdir(svar.build_path + '/allsrc')


    #
    # Generate source copy file list
    #
    objgen.call_make_target('gen_copy_list', basic_param, ecode.ERROR_SOURCE_FILE_COPY_ERROR)


    #
    # Copy source files to allsrc
    #
    try:
        src_list_file = open(svar.build_path + "/misc/src_list.txt", 'r') 
        src_list = src_list_file.readlines() 
        src_list_file.close()
    except:
        diagnostics.error = ecode.ERROR_SRC_LIST_NOT_FOUND
        exit(1)

    for file in src_list:
        file = file.replace('//', '/')
        file = file.strip()
        file_name = os.path.basename(file)
        file_name = file_name.strip()
        shutil.copyfile(file, svar.build_path + '/allsrc/' + file_name)

    #
    # Copy include files to hdr
    #

    #
    # Copy api include file to api
    #




    