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
import diagnostics

var1 = 0
var2 = 1

def process_devattrb():
    var1 = 0
    var2 = 1


def arch_specific_objgen():

    #
    # STAGE 1: Kernel SRAM address vs MPU alignment check and bound check
    #
    #

    if(int(sparam.kernel_sram_address) < int(device.sram_addr)):
        diagnostics.error = ecode.ERROR_DEVICE_ATTRIBUTES_FAILED
        diagnostics.error_message = '<kernel_sram_address> is out of bound with device xml specified sram region'
        exit(1)

    if(int(sparam.kernel_sram_address) >= (int(device.sram_addr) + int(device.sram_size))):
        diagnostics.error = ecode.ERROR_DEVICE_ATTRIBUTES_FAILED
        diagnostics.error_message = '<kernel_sram_address> is out of bound with device xml specified sram region'
        exit(1)

    next_fit_mpu_region = next_power_of_2(int(sparam.kernel_section_size))
    next_fix_mpu_region_in_kb = next_fit_mpu_region * 1024

    if(int(sparam.kernel_sram_address) % next_fix_mpu_region_in_kb != 0):
        diagnostics.error = ecode.ERROR_DEVICE_ATTRIBUTES_FAILED
        diagnostics.error_message = '<kernel_sram_address> is not in par with ARM M3/M4 mpu region alignment requirement'
        exit(1)



    #
    # STAGE 2: MPU SRD Bit calculation
    #
    #


    srd_shift = 0
    srd_bit = 0
    srd_shift = (next_fit_mpu_region - int(sparam.kernel_section_size)) * 8
    srd_shift = srd_shift / next_fit_mpu_region

    srd_bit = 8 - srd_shift
    srd_bit = 255 << int(srd_bit)
    srd_bit = 255 & srd_bit

    #
    # STAGE 3: Generate Process context switch MPU table
    #
    #

    generate_pcs_mpu_table()


#
# Local function: next power of 2
# 
#

def next_power_of_2(number):
    pos = 0
    temp = number

    while (number > 0):
        pos = pos + 1
        number = number >> 1

    number = pow(2, pos)
            
    if ((temp * 2) == number):
            number = temp

    return number

#
# Local function: genreate process context switch MPU table
#
#

def generate_pcs_mpu_table():

    mtable_src = open(svar.build_path + "/mtable.c", "w+")

    mtable_src.write("#include <stdint.h>\n")
    mtable_src.write("#include <mpu.h>\n")
    mtable_src.write("\n")

    temp = "const uint32_t mpu_table[" + str(int(2 * sparam.max_process_count)) + "] = {" + "\n"
    mtable_src.write(temp)
                
    for i in range(int(sparam.max_process_count)):

        temp = "// Process " + str(i) + "\n"
        mtable_src.write(temp)

        pstack_addr = 0
        addr = pstack_addr + (i * int(sparam.process_stack_size)) + 23
        
        temp = hex(addr) + ",\n"
        mtable_src.write(temp)

        temp = "(" + pstack_size_to_string(int(sparam.process_stack_size)) + " << 1) | MPU_KERNEL_DEFAULT | (MPU_AP_PRW_URW << 24) | MPU_REGION_ENABLE"
        mtable_src.write(temp)

        if(i != (sparam.max_process_count - 1)):
            mtable_src.write(",\n")
        else:
            mtable_src.write("\n")


    mtable_src.write("};")

    mtable_src.close()


#
# Local function: to convert process stack size into MPU Region string
#
#

def pstack_size_to_string(size):

    str_value = ""

    if(size == 512):
        str_value = "MPU_REGION_SIZE_512B"

    elif(size == 1024):
        str_value = "MPU_REGION_SIZE_1KB"

    return str_value

