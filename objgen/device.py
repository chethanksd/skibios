#
# Script that executes pre-objgen process
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


def parse_device_file():

    global sram_size
    global sram_addr
    global flash_size
    global flash_addr

    global intvec_size
    global core_count

    global arch
    global partno
    global devattrb_module

    #
    # Try creating device tree
    #
    try:
        dfile_tree = xml.dom.minidom.parse(svar.dfile_path)
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        exit(1)

    #
    # First child tag name should be 'device'
    #
    if(dfile_tree.firstChild.tagName != 'device'):
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = '<device> tag not found'
        exit(1)
    
    #
    # Start retriving core tags
    #
    try:
        flash_size = (dfile_tree.getElementsByTagName("flash_size")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <flash_size> tag'
        exit(1)
    
    try:
        sram_size  = (dfile_tree.getElementsByTagName("sram_size")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <sram_size> tag'
        exit(1)

    try:
        intvec_size = (dfile_tree.getElementsByTagName("intcnt")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <intcnt> tag'
        exit(1)

    try:
        flash_addr = (dfile_tree.getElementsByTagName("flash_addr")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <flash_addr> tag'
        exit(1)

    try:
        sram_addr = (dfile_tree.getElementsByTagName("sram_addr")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <sram_addr> tag'
        exit(1)

    try:
        arch = (dfile_tree.getElementsByTagName("arch")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <arch> tag'
        exit(1)

    try:
        partno = (dfile_tree.getElementsByTagName("partno")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <partno> tag'
        exit(1)         

    #
    # Parse device attribute
    #
    #
    
    devattrb_path = svar.repo_path + 'src\\arch\\' + arch + '\\script\\devattrb'
    devattrb_dir = os.path.dirname(devattrb_path)
    modname = os.path.basename(devattrb_path)
    sys.path.append(devattrb_dir)

    # device attributes processing modules are dynamically loaded
    # the devattrb script is present script folder of each architechture
    try:
        devattrb_module = __import__(modname)
    except Exception as e:
        diagnostics.error = ecode.ERROR_DEVICE_ATTRIBUTES_NOT_FOUND
        diagnostics.error_message = str(e)
        exit(1)

    try:
        process_devattrb = getattr(devattrb_module, 'process_devattrb')
        process_devattrb()
    except Exception as e:
        diagnostics.error = ecode.ERROR_PROCESS_DEVATTRIB_NOT_DEFINED
        diagnostics.error_message = str(e)
        exit(1)


def validate_device_params():

    try:
        int(sram_size)
    except ValueError:
        diagnostics.error = ecode.ERROR_DEVICE_PARAM_BAD
        diagnostics.error_message = 'sram size provided in device file is not valid'
        exit(1)
    
    try:
        int(flash_size)
    except ValueError:
        diagnostics.error = ecode.ERROR_DEVICE_PARAM_BAD
        diagnostics.error_message = 'flash size provided in device file is not valid'
        exit(1)

    try:
        int(sram_addr)
    except ValueError:
        diagnostics.error = ecode.ERROR_DEVICE_PARAM_BAD
        diagnostics.error_message = 'sram address provided in device file is not valid'
        exit(1)

    try:
        int(flash_addr)
    except ValueError:
        diagnostics.error = ecode.ERROR_DEVICE_PARAM_BAD
        diagnostics.error_message = 'flash address provided in device file is not valid'
        exit(1)

    try:
        int(intvec_size)
    except ValueError:
        diagnostics.error = ecode.ERROR_DEVICE_PARAM_BAD
        diagnostics.error_message = 'interrupt count provided in device file is not valid'
        exit(1)