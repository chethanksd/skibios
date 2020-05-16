#
# Script that executes pre-objgen process
#
# author : chetandev.ksd@gmail.com
#
#

import svar
import ecode
import diagnostics
import utility

import sys, os
import xml.dom.minidom
import importlib

from lxml import etree


def parse_device_file():

    global devattrb_module

    global dlist
    global devp_list

    global alist
    global archp_list


    #
    # Load core device XSD file
    #
    devschema_doc = etree.parse(svar.repo_path + "objgen/xsd_schema/device_schema.xsd")
    xmlschema = etree.XMLSchema(devschema_doc)


    #
    # Extract list of param and its data type using XPATH for core device paramaters
    #
    namespaces = {"xs": "http://www.w3.org/2001/XMLSchema"}
    devp_xpath = "//xs:element[@name='device']//xs:element[@name='basic']"

    devp_list = devschema_doc.xpath(devp_xpath + "//xs:element/@name", namespaces=namespaces)
    devp_type_dict = {}

    for param in devp_list:
        devp_type_dict[param] = devschema_doc.xpath(devp_xpath + "//xs:element[@name='" + str(param) + "']/@type", namespaces=namespaces)[0]


    #
    # Validate param xml with XSD schema defined for core device paramaters
    #
    xml_doc = etree.parse(svar.dfile_path)
    result = xmlschema.validate(xml_doc)
    
    if(result == False):
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        for error in xmlschema.error_log:
            diagnostics.error_message = 'error in device file \n'
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

        dlist[param] = utility.extract_value_xsd(value, devp_type_dict[param])


    #
    # Load arch specific XSD file
    #
    #
    archschema_doc = etree.parse(svar.repo_path + "src/arch/" + dlist['arch'] + "/script/devattrb_schema.xsd")
    archschema = etree.XMLSchema(archschema_doc)


    #
    # Validate param xml with XSD schema defined for devattrb tag
    #
    result = archschema.validate(xml_doc)
    
    if(result == False):
        diagnostics.error = ecode.ERROR_DEVICE_FILE_BAD
        for error in archschema.error_log:
            diagnostics.error_message = 'error in device file \n'
            diagnostics.error_message = diagnostics.error_message + '\nLine ' + str(error.line) + ' : ' + error.message
        exit(1)  


    #
    # Extract list of param and its data type using XPATH for devattrb paramaters
    #
    archp_xpath = "//xs:element[@name='device']//xs:element[@name='devattrb']"

    archp_list = archschema_doc.xpath(archp_xpath + "//xs:element/@name", namespaces=namespaces)
    archp_type_dict = {}

    for param in archp_list:
        archp_type_dict[param] = archschema_doc.xpath(archp_xpath + "//xs:element[@name='" + str(param) + "']/@type", namespaces=namespaces)[0]


    #
    # Extract paramters value from input param xml file using XPATH
    #
    alist = {}
    for param in archp_list:
        
        try:
            value =  xml_doc.xpath("//" + str(param) + "/text()", namespaces=namespaces)[0]
        except:

            try:
                value = archschema_doc.xpath("//xs:element[@name='" + str(param) + "']/@default", namespaces=namespaces)[0]
            except:
                diagnostics.error = ecode.ERROR_BAD_XSD_SCHEMA
                diagnostics.error_message = param + " is a optional parameter with not default value defined in XSD"
                exit(1)

        alist[param] = utility.extract_value_xsd(value, archp_type_dict[param])
        

    #
    # Parse device attribute
    #
    #
    
    devattrb_path = svar.repo_path + 'src\\arch\\' + dlist['arch'] + '\\script\\devattrb'
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