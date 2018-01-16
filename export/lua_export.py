#! python
# -*- coding:utf-8 -*-

lua_module_sample = '''local {0} = {1}
'''

lua_clt_sample = '''    {0}_{1} = {{ {0} + {2},"{3}.pb","{3}.C{4}" }},
'''

lua_srv_sample = '''    {0}_{1} = {{ {0} + {2},"{3}.pb","{3}.S{4}" }},
'''

lua_file_sample = '''\
-- // AUTO GENERATE,DO NOT MODIFY

{0}

local SC =
{{
{1}
}}

local CS =
{{
{2}
}}

return {{ SC,CS }}
'''

class LuaExport:
    def __init__(self,info_list,output_path,output_file):
        self.info_list = info_list
        self.output_path = output_path
        self.output_file = output_file

    def export_module_ctx( self,info ):
        return lua_module_sample.format( info["module"].upper(),info["command"] )

    def export_command_ctx( self,info,ctx_sample,key ):
        command_ctx = ""

        module_name = info["module"].upper()
        package_name = info["module"].lower()
        for cmd in info["commands"]:
            command_ctx += ctx_sample.format( module_name,
                cmd["field"].upper(),cmd["command"],package_name,cmd[key] )

        return command_ctx

    def export_file_ctx( self ):
        module_list = ""
        srv_cmd_list = ""
        clt_cmd_list = ""

        for info in self.info_list:
            module_list += self.export_module_ctx( info )
            srv_cmd_list += self.export_command_ctx( info,lua_srv_sample,"server" )
            clt_cmd_list += self.export_command_ctx( info,lua_clt_sample,"client" )

        return lua_file_sample.format( module_list,srv_cmd_list,clt_cmd_list )

    def export( self ):
        ts_file_ctx = self.export_file_ctx()
        path = self.output_path + "/" + self.output_file + ".lua"
        ts_file = open( path,"wb" )
        ts_file.write( ts_file_ctx.encode("utf-8") )
        ts_file.close()