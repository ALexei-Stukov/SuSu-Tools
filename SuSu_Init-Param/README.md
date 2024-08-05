[Introduction of SuSu-Tools](../README.md)

## Summary about this tool

**Init param tool** provides a good way to process init params.

we can write a file like this:

```
name=susu
path=./
version=0.1
limit=71231
```
The key and value will be read by string.And you can read the value by this way:
```cpp
	auto ins = susu_initparam::get_Init_Param_instance();
	string name = ins->get_value("name");

	int limit = stoi(ins->get_value("limit"));

	string message = ins->get_value("WTF");
	//Because "WTF" is not exist in file,so the message is "ERROR".

```
