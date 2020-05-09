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
import importlib

from lxml import etree


def parse_param_file():

    # input parameters
    global slist

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
    slist = {}
    for param in param_list:
        
        try:
            value =  xml_doc.xpath("//" + str(param) + "/text()", namespaces=namespaces)[0]
        except:
            slist[param] = '0'
        
        if(param_type_dict[param] == 'U32HexInt'):
            if('0x' in value):
                slist[param] = int(value, 16)
            else:
                slist[param] = int(value)

        if(param_type_dict[param] == 'xs:integer'):
                slist[param] = int(value)
