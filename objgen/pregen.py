#
# Script that executes pre-objgen process
#
# author : chetandev.ksd@gmail.com
#
#

import sys, os
import subprocess
import time

import diagnostics
import ecode
import svar
import device
import sparam


def run_pregen():

    #
    # PREGEN STAGE1: Device file parsing
    #
    #
    diagnostics.pregen_stage = 1

    device.parse_device_file()


    #
    # PREGEN STAGE2: Device parameters validation
    #
    #
    diagnostics.pregen_stage = 2
    
    device.validate_device_params()

    
    #
    # PREGEN STAGE3: Param file parsing
    #
    #
    diagnostics.pregen_stage = 3

    sparam.parse_param_file()


