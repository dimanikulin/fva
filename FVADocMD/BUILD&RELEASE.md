## Building and releasing the code and the docs
&nbsp;&nbsp;&nbsp; For building the code you don't need to have any local environment to set up. 
1. So building the code is already done by github. I used [[37]](FVADocMD/REFERENCES.md) to help me to set up the main GitHub action flow and [this link](https://github.com/jurplel/install-qt-action) to set up QT.
As soon as any code change is pushed to GitHub, [main workflow](.github/workflows/main.yml) is being executed and you will have new [Installation packages](https://github.com/dimanikulin/fva/releases) to test.
2. But if you wish you can use [MS studio solution](./FVASW.sln) to build locally on  Windows or [CMake](./CMakeLists.txt) to build locally on any Windows, Mac or Linux. 

&nbsp;&nbsp;&nbsp; To re-generate the docs you need to re-execute [documentation workflow](.github/workflows/releaseDocs.yml) from workflow. As result you will have [this](./DoxyGeneratedDoc.pdf) TODO - describe how 
