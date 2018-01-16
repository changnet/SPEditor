#! python
# -*- coding:utf-8 -*-

import os
import time
import importlib
import  xml.dom.minidom
from optparse import OptionParser

from ts_export import TSExport
from lua_export import LuaExport

class Exporter:
    def __init__(self,input_path,output_path,output_file):
        self.input_path  = input_path
        self.output_path = output_path
        self.output_file = output_file

    def get_file_list(self):
        xml_list = []
        file_list = os.listdir( self.input_path )
        for file_name in file_list:
            abspath = os.path.join( self.input_path,file_name )
            if os.path.isfile( abspath ) and file_name.endswith( "xml" ):
                xml_list.append( abspath )

        xml_list.sort()
        return xml_list

    def xml_get_field_text( self,xml_node,field ):
        elements = xml_node.getElementsByTagName( field )[0]
        for child in elements.childNodes:
            if child.nodeType == child.TEXT_NODE: return child.data

        return None

    def xml_parser_one( self,path ):
        xml_info = {}
        xml_info["path"] = path

        xml_dom = dom = xml.dom.minidom.parse( path )
        root = xml_dom.documentElement

        # getElementsByTagName返回的是一个数组
        xml_info["command"] = self.xml_get_field_text( root,"command" )
        xml_info["module"]  = self.xml_get_field_text( root,"module" )
        xml_info["comment"] = self.xml_get_field_text( root,"comment" )

        xml_info["commands"] = []
        subobject_root = root.getElementsByTagName( "subobjects" )[0]
        subobjects = subobject_root.getElementsByTagName( "subobject" )
        for subobject in subobjects:
            command = {}
            command["command"] = self.xml_get_field_text( subobject,"command" )
            command["field"]   = self.xml_get_field_text( subobject,"field" )
            command["comment"] = self.xml_get_field_text( subobject,"comment" )
            command["server"]  = self.xml_get_field_text( subobject,"server" )
            command["client"]  = self.xml_get_field_text( subobject,"client" )

            xml_info["commands"].append( command )

        return xml_info

    def xml_parser( self ):
        info_list = []
        file_list = self.get_file_list()
        for path in file_list:
            info = self.xml_parser_one( path )
            info_list.append( info )

        return info_list

    def lua_export( self,info_list ):
        print( "lua" )

    def export( self ):
        info_list = self.xml_parser()

        ts_export = TSExport( info_list,self.output_path,self.output_file )
        ts_export.export()

        lua_export = LuaExport( info_list,self.output_path,self.output_file )
        lua_export.export()

        print( "export done..." )

if __name__ == '__main__':

    parser = OptionParser()
    parser.add_option( "-i", "--input", dest="input_path",
                    default="proto/",
                    help="read all files from this path" )
    parser.add_option( "-o", "--output", dest="output_path",
                    default="export/",
                    help="write all export file to this path" )
    parser.add_option( "-f", "--file", dest="output_file",
                    default="proto",
                    help="the name of exported file" )

    options, args = parser.parse_args()
    if not os.path.exists( options.output_path ) :
        os.makedirs( options.output_path )

    exporter = Exporter( options.input_path,options.output_path,options.output_file )
    exporter.export()