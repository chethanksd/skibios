#
# Script that executes pre-objgen process
#
# author : chetandev.ksd@gmail.com
#
#

import svar
import ecode
import diagnostics

import sys, os
import xml.dom.minidom
import importlib

from lxml import etree


def parse_device_file():

    global sram_addr
    global flash_addr

    global intvec_size
    global core_count

    global arch
    global partno
    global devattrb_module

    global dlist
    global devp_list

    #
    # Load XSD file
    #
    devschema_doc = etree.parse(svar.repo_path + "objgen/xsd_schema/device_schema.xsd")
    xmlschema = etree.XMLSchema(devschema_doc)


    #
    # Extract list of param and its data type using XPATH
    #
    namespaces = {"xs": "http://www.w3.org/2001/XMLSchema"}
    devp_list = devschema_doc.xpath("//xs:element[@name and @type]/@name", namespaces=namespaces)
    devp_type_dict = {}

    for param in devp_list:
        devp_type_dict[param] = devschema_doc.xpath("//xs:element[@name='" + str(param) + "']/@type", namespaces=namespaces)[0]


    #
    # Validate param xml with XSD schema defined for sparam
    #
    xml_doc = etree.parse(svar.dfile_path)
    result = xmlschema.validate(xml_doc)
    
    if(result == False):
        diagnostics.error = ecode.ERROR_PARAM_FILE_BAD
        for error in xmlschema.error_log:
            diagnostics.error_message = diagnostics.error_message + '\nLine ' + str(error.line) + ' : ' + error.message
        exit(1)   


    #
    # Extract paramters value from input param xml file using XPATH
    #
    dlist = {}
    for param in devp_list:
        
        try:
            value =  xml_doc.xpath("//" + str(param) + "/text()", namespaces=namespaces)[0]
        except:

            try:
                value = devschema_doc.xpath("//xs:element[@name='" + str(param) + "']/@default", namespaces=namespaces)[0]
            except:
                diagnostics.error = ecode.ERROR_BAD_XSD_SCHEMA
                diagnostics.error_message = param + " is a optional parameter with not default value defined in XSD"
                exit(1)

        
        if(devp_type_dict[param] == 'U32HexInt'):
            if('0x' in value):
                dlist[param] = int(value, 16)
            else:
                dlist[param] = int(value)

        if(devp_type_dict[param] == 'BinarySwitch'):
            if(('True' in value) or ('1' in value)):
                dlist[param] = 1
            if(('False' in value) or ('0' in value)):
                dlist[param] = 0

        if(devp_type_dict[param] == 'Integer'):
            dlist[param] = int(value)

        if(devp_type_dict[param] == 'String'):
            dlist[param] = value


    #
    # Try creating device tree
    #
    try:
        dfile_tree = xml.dom.minidom.parse(svar.dfile_path)
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        exit(1)

    #
    # First child tag name should be 'device'
    #
    if(dfile_tree.firstChild.tagName != 'device'):
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = '<device> tag not found'
        exit(1)
    
    #
    # Start retriving core tags
    #
    try:
        intvec_size = (dfile_tree.getElementsByTagName("intcnt")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <intcnt> tag'
        exit(1)

    try:
        flash_addr = (dfile_tree.getElementsByTagName("flash_addr")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <flash_addr> tag'
        exit(1)

    try:
        sram_addr = (dfile_tree.getElementsByTagName("sram_addr")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <sram_addr> tag'
        exit(1)

    try:
        arch = (dfile_tree.getElementsByTagName("arch")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <arch> tag'
        exit(1)

    try:
        partno = (dfile_tree.getElementsByTagName("partno")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <partno> tag'
        exit(1)         

    #
    # Parse device attribute
    #
    #
    
    devattrb_path = svar.repo_path + 'src\\arch\\' + arch + '\\script\\devattrb'
    devattrb_dir = os.path.dirname(devattrb_path)
    modname = os.path.basename(devattrb_path)
    sys.path.append(devattrb_dir)

    # device attributes processing modules are dynamically loaded
    # the devattrb script is present script folder of each architechture
    try:
        devattrb_module = __import__(modname)
    except Exception as e:
        diagnostics.error = ecode.ERROR_DEVICE_ATTRIBUTES_NOT_FOUND
        diagnostics.error_message = str(e)
        exit(1)

    try:
        process_devattrb = getattr(devattrb_module, 'process_devattrb')
        process_devattrb()
    except Exception as e:
        diagnostics.error = ecode.ERROR_PROCESS_DEVATTRIB_NOT_DEFINED
        diagnostics.error_message = str(e)
        exit(1)


def validate_device_params():

    try:
        int(sram_addr)
    except ValueError:
        diagnostics.error = ecode.ERROR_DEVICE_PARAM_BAD
        diagnostics.error_message = 'sram address provided in device file is not valid'
        exit(1)

    try:
        int(flash_addr)
    except ValueError:
        diagnostics.error = ecode.ERROR_DEVICE_PARAM_BAD
        diagnostics.error_message = 'flash address provided in device file is not valid'
        exit(1)

    try:
        int(intvec_size)
    except ValueError:
        diagnostics.error = ecode.ERROR_DEVICE_PARAM_BAD
        diagnostics.error_message = 'interrupt count provided in device file is not valid'
        exit(1)