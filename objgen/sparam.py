#
# SKIBIOS Param file parser Script
#
# author : chetandev.ksd@gmail.com
#
#

import svar
import ecode
import diagnostics

import sys, os
import xml.dom.minidom
import importlib


def parse_param_file():

    # input parameters
    global kernel_sram_address
    global kernel_section_size
    global upper_region_size
    global process_stack_size
    global ghmb_region_size
    global process_id_start
    global process_per_sec
    global enable_safe_lock
    global disable_buffer

    #output parameters
    global max_process_count
    global kernel_base_address
    global app_base_address
    global proc_heap_address
    global kernel_stack_address
    global ghmb_address

    #
    # Try creating param tree
    #
    try:
        pfile_tree = xml.dom.minidom.parse(svar.pfile_path)
    except:
        diagnostics.error = ecode.ERROR_PARAM_FILE_BAD
        exit(1)

    #
    # First child tag name should be 'param'
    #
    if(pfile_tree.firstChild.tagName != 'param'):
        diagnostics.error = ecode.ERROR_PARAM_FILE_BAD
        diagnostics.error_message = '<param> tag not found'
        exit(1)

    #
    # Start retriving data
    #
    try:
        kernel_sram_address = (pfile_tree.getElementsByTagName("kernel_sram_address")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <kernel_sram_address> tag'
        exit(1)    

    try:
        kernel_section_size = (pfile_tree.getElementsByTagName("kernel_section_size")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <kernel_section_size> tag'
        exit(1)

    try:
        upper_region_size = (pfile_tree.getElementsByTagName("upper_region_size")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <upper_region_size> tag'
        exit(1)
    
    try:
        process_stack_size = (pfile_tree.getElementsByTagName("process_stack_size")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <process_stack_size> tag'
        exit(1)

    try:
        ghmb_region_size = (pfile_tree.getElementsByTagName("ghmb_region_size")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <ghmb_region_size> tag'
        exit(1)

    try:
        process_id_start = (pfile_tree.getElementsByTagName("process_id_start")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <process_id_start> tag'
        exit(1)  

    try:
        process_per_sec = (pfile_tree.getElementsByTagName("process_per_sec")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <process_per_sec> tag'
        exit(1)

    try:
        enable_safe_lock = (pfile_tree.getElementsByTagName("enable_safe_lock")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <enable_safe_lock> tag'
        exit(1)

    try:
        disable_buffer = (pfile_tree.getElementsByTagName("disable_buffer")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <disable_buffer> tag'
        exit(1)


    
def validate_skibios_param():
    print('Todo: define skibios param validation')