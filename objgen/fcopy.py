import sys, os
import subprocess
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

    #
    # Copy include files to hdr
    #

    #
    # Copy api include file to api
    #




    