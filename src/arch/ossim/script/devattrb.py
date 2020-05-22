#
# Device attribute file for E200 Architecture
#
# author : chetandev.ksd@gmail.com
#
#

import sys, os

import svar
import ecode
import sparam
import device

var1 = 0
var2 = 1

def process_devattrb():
    var1 = var2

def arch_specific_objgen():

    #
    # STAGE 1: Append arch specific constants to param header
    #
    #

    append_arch_constants()


#
# Local function: function to append arch specific symbolic constants to param header
#
#

def append_arch_constants():

    param_header = open(svar.build_path + "/hdr/arch_param.h", "w+")

    param_header.write("#ifndef _ARCH_PARAM_H_\n")
    param_header.write("#define _ARCH_PARAM_H_\n")
    param_header.write("\n")

    param_header.write("// Arch Specific Symbolic Constants\n\n")

    param_header.write("#define OSSIM_RUN   1\n")

    param_header.write("\n\n")
    param_header.write("#endif")
    param_header.close()