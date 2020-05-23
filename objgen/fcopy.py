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
    copy_files_from_list(svar.build_path + "/misc/src_list.txt", svar.build_path + '/allsrc', ecode.ERROR_SRC_LIST_NOT_FOUND)


    #
    # Copy include files to hdr
    #
    copy_files_from_list(svar.build_path + "/misc/hdr_list.txt", svar.build_path + '/hdr', ecode.ERROR_HDR_LIST_NOT_FOUND)


    #
    # Copy api include file to api
    #
    copy_files_from_list(svar.build_path + "/misc/api_list.txt", svar.build_path + '/api', ecode.ERROR_API_LIST_NOT_FOUND)



#
# Local function: function to copy file present in list file to destination
#

def copy_files_from_list(list_path, destination, error_code):

    try:
        list_file = open(list_path, 'r') 
        copy_list = list_file.readlines() 
        list_file.close()
    except:
        diagnostics.error = error_code
        exit(1)

    for file in copy_list:
        file = file.replace('//', '/')
        file = file.strip()

        if(file == ''):
            continue
            
        file_name = os.path.basename(file)
        file_name = file_name.strip()
        shutil.copyfile(file, destination + '/' + file_name)



    