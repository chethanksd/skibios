#
# SKIBIOS objgen entry scipt
#
# author : chetandev.ksd@gmail.com
#
#

import sys, os
import subprocess
import atexit
import time

import ecode
import diagnostics
import pregen
import objgen
import svar


#
# Script exit handler definition
#
#

def exit_handler():

    if(diagnostics.error != ecode.ERROR_NONE):
        print('ERROR!', ecode.error_description[diagnostics.error])
        print('objgen error: ', diagnostics.error)
        
        if(diagnostics.error_message != ''):
            print(diagnostics.error_message)

        diagnostics.run_diagnostics()

atexit.register(exit_handler)

#
# START STAGE 1: Get repository path      
#
#
diagnostics.start_stage = 1 
svar.repo_path = os.path.abspath(__file__)
svar.repo_path = svar.repo_path.replace("objgen\start.py", "")
svar.repo_path = svar.repo_path.replace("\\","/") 

print('repopath :', svar.repo_path)

#
# START STAGE 2: Get device file argument
#
#
diagnostics.start_stage = 2

try:
    svar.dfile_path = sys.argv[1]
except IndexError:
    diagnostics.error = ecode.ERROR_DEVICE_FILE_NOT_PROVIDED
    exit(1)


dfile_exists = os.path.isfile(svar.dfile_path)
if not dfile_exists:
    diagnostics.error = ecode.ERROR_DEVICE_FILE_NOT_FOUND
    exit(1)

#
# START STAGE 3: Get param file argument
#
#
diagnostics.start_stage = 3

try:
    svar.pfile_path = sys.argv[2]
except IndexError:
    diagnostics.error = ecode.ERROR_PARAM_FILE_NOT_PROVIDED
    exit(1)


pfile_exists = os.path.isfile(svar.pfile_path)
if not pfile_exists:
    diagnostics.error = ecode.ERROR_PARAM_FILE_NOT_FOUND
    exit(1)

#
# START STAGE 3: Check if build path exists
#
#
diagnostics.start_stage = 4

try:
    svar.build_path = sys.argv[3]
except IndexError:
    diagnostics.error = ecode.ERROR_BUILD_PATH_NOT_PROVIDED
    exit(1)


bpath_exists = os.path.isdir(svar.build_path)
if not bpath_exists:
    diagnostics.error = ecode.ERROR_BUILD_PATH_NOT_FOUND
    exit(1)

#
# START STAGE 4: Check if bin path exists
#
#
diagnostics.start_stage = 5

try:
    svar.bin_path = sys.argv[4]
except IndexError:
    diagnostics.error = ecode.ERROR_BIN_PATH_NOT_PROVIDED
    exit(1)


binpath_exists = os.path.isdir(svar.bin_path)
if not binpath_exists:
    diagnostics.error = ecode.ERROR_BIN_PATH_NOT_FOUND
    exit(1)



#
# Run Pregen
#
#
diagnostics.start_stage = diagnostics.STAGE_DONE

pregen.run_pregen()

#
# Run objgen
#
#

objgen.run_objgen()


print('Done!')