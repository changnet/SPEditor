# SPEditor

A stream protocol editor for game development.

# Settings

* If no setting.ini found in the working directory,it will create one automatically
* The first field of module must be unique,command as well.

# Custom protocol

> uint8:id > the unique id
> string:name > the name of this id
> array:list > test list
>     uin32:sub_id > sub item id
>   int16:value > value number

* datatype:field > comment
* if array exist,those fields who's indent greater than it belongs its element.



