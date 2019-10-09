#
# Script that executes pre-objgen process
#
# author : chetandev.ksd@gmail.com
#
#

import svar
import ecode
import diagnostics

import xml.dom.minidom

sram_size = 0
sram_addr = 0
flash_size = 0
flash_addr = 0

intvec_size = 0
core_count  = 0

arch = ''

def parse_device_file():

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