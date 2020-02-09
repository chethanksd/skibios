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
    # PREGEN STAGE 2: Device parameters validation
    #
    #
    diagnostics.pregen_stage = 2
    
    device.validate_device_params()

    
    #
    # PREGEN STAGE 3: Param file parsing
    #
    #
    diagnostics.pregen_stage = 3

    sparam.parse_param_file()


    #
    # PREGEN STAGE 4: SKIBIOS Param validation
    #
    #
    diagnostics.pregen_stage = 4
    
    sparam.validate_skibios_param()

    #
    # PREGEN STAGE 5: Create allsrc folder
    #
    #
    diagnostics.pregen_stage = 5

    apath_exists = os.path.isdir(svar.build_path + '/allsrc')
    if not apath_exists:
        os.mkdir(svar.build_path + '/allsrc')
    else:
        shutil.rmtree(svar.build_path + '/allsrc')

    #
    # PREGEN STAGE: DONE
    #
    #
    diagnostics.pregen_stage = diagnostics.STAGE_DONE
