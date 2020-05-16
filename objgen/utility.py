#
# Objen utility functions
#
# author : chetandev.ksd@gmail.com
#
#


def extract_value_xsd(value, data_type):

    
    if(data_type == 'U32HexInt'):
        if('0x' in value):
            return_value = int(value, 16)
        else:
            return_value = int(value)

    if(data_type == 'BinarySwitch'):
        if(('True' in value) or ('1' in value)):
            return_value = 1
        if(('False' in value) or ('0' in value)):
            return_value = 0

    if(data_type == 'Integer'):
        return_value = int(value)

    if(data_type == 'String'):
        return_value = value

    return return_value