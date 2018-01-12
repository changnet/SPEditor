#! python
# -*- coding:utf-8 -*-

class LuaExport:
    def __init__(self,info_list,output_path,output_file):
        self.info_list = info_list
        self.output_path = output_path
        self.output_file = output_file

    def export( self ):
        ts_file_ctx = "lua"#self.export_file_ctx()
        path = self.output_path + "/" + self.output_file + ".lua"
        ts_file = open( path,"wb" )
        ts_file.write( ts_file_ctx.encode("utf-8") )
        ts_file.close()