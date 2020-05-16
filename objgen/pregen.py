#
# Script that executes pre-objgen process
#
# author : chetandev.ksd@gmail.com
#
#

import sys, os
import subprocess
import shutil
import time

import diagnostics
import ecode
import svar
import device
import sparam


def run_pregen():

    #
    # PREGEN STAGE 1: Device file parsing
    #
    #
    diagnostics.pregen_stage = 1

    device.parse_device_file()

    
    #
    # PREGEN STAGE 2: Param file parsing
    #
    #
    diagnostics.pregen_stage = 2

    sparam.parse_param_file()


    #
    # PREGEN STAGE 3: Create allsrc folder
    #
    #
    diagnostics.pregen_stage = 3

    apath_exists = os.path.isdir(svar.build_path + '/allsrc')
    if not apath_exists:
        os.mkdir(svar.build_path + '/allsrc')

    
    #
    # PREGEN STAGGE 4: Run objgen_debug diagnostics
    #
    #
    diagnostics.pregent_stage = 4

    diagnostics.run_objgen_debug_commands()


    #
    # PREGEN STAGE: DONE
    #
    #
    diagnostics.pregen_stage = diagnostics.STAGE_DONE
