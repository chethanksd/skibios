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
    
    cpath = svar.repo_path + "/tools/make/make.exe -f./objgen/makefile ARCH_PATH=" + svar.repo_path + "/src/arch/" + device.arch + " all"
    f = os.popen(cpath) 

    for line in f.readlines():
        print(line)