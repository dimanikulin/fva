# Rules to check code during code review 

* The code shall be developed using Google code style [[27]](./REFERENCES.md) 
* The "Return Code" and not "Exception" approach shall be used to provide a result to calling code.
* No commented code is allowed.
* Compiler shall be configured to threat warning as errors for all projects.
* The code shall use QDir::separator() and not "/" or "\".
* The code shall be compiled with level 4 for warnings (MS compiler)
* The file names shall be in lower case to make cross-compilation simpler.
* The constants shall be placed in separate file/files.
* Commit mesage shall start from the issue ID.