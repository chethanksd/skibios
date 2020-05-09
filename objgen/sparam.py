#
# SKIBIOS Param file parser Script
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


def parse_param_file():

    # input parameters
    global kernel_sram_address
    global kernel_section_size
    global upper_region_size
    global process_stack_size
    global ghmb_region_size
    global process_id_start
    global process_per_sec
    global enable_safe_lock
    global disable_buffer

    #output parameters
    global max_process_count
    global kernel_base_address
    global app_base_address
    global proc_heap_address
    global kernel_stack_end_address
    global ghmb_address

    #
    # Load XSD file
    #
    xmlschema_doc = etree.parse(svar.repo_path + "objgen/xsd_schema/sparam_schema.xsd")
    xmlschema = etree.XMLSchema(xmlschema_doc)


    #
    # Extract list of param and its data type using XPATH
    #
    namespaces = {"xs": "http://www.w3.org/2001/XMLSchema"}
    param_list = xmlschema_doc.xpath("//xs:element[@name and @type]/@name", namespaces=namespaces)
    param_type_dict = {}

    for param in param_list:
        param_type_dict[param] = xmlschema_doc.xpath("//xs:element[@name='" + str(param) + "']/@type", namespaces=namespaces)[0]
        

    #
    # Validate param xml with XSD schema defined for sparam
    #
    xml_doc = etree.parse(svar.pfile_path)
    result = xmlschema.validate(xml_doc)
    
    if(result == False):
        diagnostics.error = ecode.ERROR_PARAM_FILE_BAD
        for error in xmlschema.error_log:
            diagnostics.error_message = diagnostics.error_message + '\nLine ' + str(error.line) + ' : ' + error.message
        exit(1)   

    #
    # Extract paramters value from input param xml file
    #
    sparam = {}


    #
    # Try creating param tree
    #
    try:
        pfile_tree = xml.dom.minidom.parse(svar.pfile_path)
    except:
        diagnostics.error = ecode.ERROR_PARAM_FILE_BAD
        diagnostics.error_message = 'syntax error in skibios param xml'
        exit(1)

    #
    # First child tag name should be 'param'
    #
    if(pfile_tree.firstChild.tagName != 'param'):
        diagnostics.error = ecode.ERROR_PARAM_FILE_BAD
        diagnostics.error_message = '<param> tag not found'
        exit(1)

    #
    # There should be 'basic' tag name as Second child
    #
    param_node = pfile_tree.firstChild

    basic_node_found = False
    for second_node in param_node.childNodes:
        if(second_node.nodeName == 'basic'):
            basic_node_found = True
            basic_node = second_node
            break;
    
    if(basic_node_found == False):
        diagnostics.error = ecode.ERROR_PARAM_FILE_BAD
        diagnostics.error_message = '<basic> tag not found inside <param> tag in skibios param xml'
        exit(1)


    #
    # Start retriving basic skibios data
    #
    try:
        kernel_sram_address = (basic_node.getElementsByTagName("kernel_sram_address")[0]).firstChild.data

        if('0x' in kernel_sram_address):
            kernel_sram_address = int(kernel_sram_address, 16)

    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <kernel_sram_address> tag'
        exit(1)    

    try:
        kernel_section_size = (basic_node.getElementsByTagName("kernel_section_size")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <kernel_section_size> tag'
        exit(1)

    try:
        upper_region_size = (basic_node.getElementsByTagName("upper_region_size")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <upper_region_size> tag'
        exit(1)
    
    try:
        process_stack_size = (basic_node.getElementsByTagName("process_stack_size")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <process_stack_size> tag'
        exit(1)

    try:
        ghmb_region_size = (basic_node.getElementsByTagName("ghmb_region_size")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <ghmb_region_size> tag'
        exit(1)

    try:
        process_id_start = (basic_node.getElementsByTagName("process_id_start")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <process_id_start> tag'
        exit(1)  

    try:
        process_per_sec = (basic_node.getElementsByTagName("process_per_sec")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <process_per_sec> tag'
        exit(1)

    try:
        enable_safe_lock = (basic_node.getElementsByTagName("enable_safe_lock")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <enable_safe_lock> tag'
        exit(1)

    try:
        disable_buffer = (basic_node.getElementsByTagName("disable_buffer")[0]).firstChild.data
    except:
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        diagnostics.error_message = 'error retriving <disable_buffer> tag'
        exit(1)