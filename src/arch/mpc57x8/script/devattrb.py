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
    print('var2: ', var2)

def arch_specific_objgen():
    print("running arch specific objgen")
    print("kernel region size: ", sparam.kernel_section_size)