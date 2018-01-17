# SPEditor

A highly customized stream protocol editor for game development.

# Settings

* If no setting.ini found in the working directory,it will create one automatically
* The first field of module must be unique,command as well.

# Custom protocol

This editor is designed to generate custom protocol schemal,but NOT implement yet.

```
uint8:id // the unique id
string:name // the name of this id
array:list // test list
    uin32:sub_id // sub item id
  int16:value // value number
```
* // comment
* datatype:field > comment
* if array exist,those fields who's indent greater than it belongs its element.

# Depoly

* using windepolyqt to deploly editor
* using scripts(bat、bash、python script) to export schemal file.  
ps: If using python script,recommend using pyinstaller to pack scripts into a stand-alone executable program. See export/depoly.bat.


There is a example in the release.
