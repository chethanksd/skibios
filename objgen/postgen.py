#
# Script that executes post objgen process
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
import objgen


def run_postgen():
    print('running postgen')

    #
    # POSTGEN STAGE 1: Linker support file generation
    #
    #
    diagnostics.postgen_stage = 1
    
    param_lds = open(svar.build_path + "/param.lds", "w+")

    temp = '_kernel_base_address '

    param_lds.close()