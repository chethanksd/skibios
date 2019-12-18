#
# Script that executes objgen process
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



def run_objgen():


    #
    # OBJGEN STAGE 1: Invoke symgen
    #
    #
    print(svar.bin_path)
    cpath = svar.repo_path + "/tools/make/make.exe symgen -f./objgen/makefile"
    cpath = cpath + " ARCH_PATH=" + svar.repo_path + "/src/arch/" + device.arch
    cpath = cpath + " BUILD_PATH=" + svar.build_path
    cpath = cpath + " BIN_PATH=" + svar.bin_path
    f = os.popen(cpath, "w") 