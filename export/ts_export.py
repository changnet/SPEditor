#! python
# -*- coding:utf-8 -*-

clt_command_sample = '''const C{0}_{1} = {0}.module + {2};
'''
srv_command_sample = '''const S{0}_{1} = {0}.module + {2};
'''

# {{}}表示对{}的转义
srv_field_sample = '''        {{ cmd:{0},object:"S{1}" }},
'''
clt_field_sample = '''        {{ cmd:{0},object:"C{1}" }},
'''

ts_module_sample = '''\
const {0} = {{
    package: "{1}",
    module: ({2} << 8),

    clt_cmd: [
{3}
    ],
    srv_cmd: [
{4}
    ]
}};
'''

ts_file_sample = '''\
// AUTO GENERATE,DO NOT MODIFY

{0}

const MODULES = [ {1} ];

{2}

'''

class TSExport:
    def __init__(self,info_list,output_path,output_file):
        self.info_list = info_list
        self.output_path = output_path
        self.output_file = output_file

    def export_module_ctx( self,info ):
        srv_cmd_list = ""
        clt_cmd_list = ""
        for command in info["commands"]:
            cmd = command["command"]
            srv_obj = command["server"]
            clt_obj = command["client"]
            if srv_obj != None:
                srv_cmd_list += srv_field_sample.format( cmd,srv_obj )
            if clt_obj != None:
                clt_cmd_list += clt_field_sample.format( cmd,clt_obj )

        module_name = info["module"].lower()
        return ts_module_sample.format( module_name.upper(),
            module_name,info["command"],clt_cmd_list,srv_cmd_list )

    def export_command_ctx( self,info ):
        command_list = ""

        module_name = info["module"].upper()
        for command in info["commands"]:
            cmd = command["command"]
            srv_obj = command["server"]
            clt_obj = command["client"]
            field = command["field"].upper()
            if clt_obj != None:
                command_list += clt_command_sample.format( module_name,field,cmd )
            if srv_obj != None:
                command_list += srv_command_sample.format( module_name,field,cmd )

        return command_list

    def export_file_ctx( self ):
        ts_module_ctx = ""
        ts_module_list = ""
        ts_command_ctx = ""

        for info in self.info_list:
            ts_module_list += info["module"].upper()
            ts_module_ctx  += self.export_module_ctx( info )
            ts_command_ctx += self.export_command_ctx( info )

        return ts_file_sample.format( ts_module_ctx,ts_module_list,ts_command_ctx )

    def export( self ):
        ts_file_ctx = self.export_file_ctx()
        path = self.output_path + "/" + self.output_file + ".ts"
        ts_file = open( path,"wb" )
        ts_file.write( ts_file_ctx.encode("utf-8") )
        ts_file.close()