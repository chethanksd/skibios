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
    global param_gen_list
    global param_gen_sym

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
    # Extract paramters value from input param xml file using XPATH
    #
    slist = {}
    for param in param_list:
        
        try:
            value =  xml_doc.xpath("//" + str(param) + "/text()", namespaces=namespaces)[0]
        except:

            try:
                value = xmlschema_doc.xpath("//xs:element[@name='" + str(param) + "']/@default", namespaces=namespaces)[0]
            except:
                diagnostics.error = ecode.ERROR_BAD_XSD_SCHEMA
                diagnostics.error_message = param + " is a optional parameter with not default value defined in XSD"
                exit(1)

        
        if(param_type_dict[param] == 'U32HexInt'):
            if('0x' in value):
                slist[param] = int(value, 16)
            else:
                slist[param] = int(value)

        if(param_type_dict[param] == 'Integer'):
            slist[param] = int(value)

        if(param_type_dict[param] == 'String'):
            slist[param] = value


    #
    # Create list of parameters to be included in param.h using XPATH
    #
    param_gen_list = {}
    param_gen_sym  = {}

    try:
        param_gen_list = xmlschema_doc.xpath("//xs:element[@name='param_gen']/xs:complexType/xs:all/xs:element/@name", namespaces=namespaces)
    except:
        diagnostics.error = ecode.ERROR_BAD_XSD_SCHEMA
        diagnostics.error_message = "invalid <param_gen> entries"
        exit(1)

    for param in param_gen_list:
        try:
            symbol =  xmlschema_doc.xpath("//xs:element[@name='param_gen']//xs:element[@name='" + str(param) + "']//xs:element/@name", namespaces=namespaces)[0]    
        except:
            diagnostics.error = ecode.ERROR_BAD_XSD_SCHEMA
            diagnostics.error_message = "symbol name not defined for " + param + " . It is included in param_gen list"
            exit(1) 
        
        param_gen_sym[param] = symbol