## Automated code checks
Currently there are following automated checks to verify if the code meets code quality requirements:
* [Code QL](.github/workflows/codeqlanalysis.yml) Please see [[38]](./REFERENCES.md) TBD what for
* [code Checks](.github/workflows/codeChecks.yml) TBD what for
* [code factor](https://www.codefactor.io/repository/github/dimanikulin/fva/issues) TBD what for
* [ms vc analysis](.github/workflows/msvc-analysis.yml) TBD what for
TODO - to add a bage for each code check

## Please follow next rules 
* The code shall be developed using Google code style [[27]](./REFERENCES.md) 
* The "Return Code" and not "Exception" approach shall be used to provide a result to calling code.
* No commented code is allowed.
* Compiler shall be configured to threat warning as errors for all projects.
* The code shall use QDir::separator() and not "/" or "\".
* The code shall be compiled with level 4 for warnings (MS compiler)
* The file names shall be in lower case to make cross-compilation simpler.
* The constants shall be placed in separate file/files.