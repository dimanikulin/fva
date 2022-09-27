## Releasing the code and the docs
&nbsp;&nbsp;&nbsp; The release of product shall be on demand as soon as peace of product functionality is ready for release.  Thus the release branch is being created or updated to keep added/updated product functionality.

&nbsp;&nbsp;&nbsp; Regardless of incremental approach to add or update product functions, the artifacts shall accumulate whole product installation packages and not increments even really small piece of functions is released.

The following artifacts shall be created:
- The documentation for code (based on doxy comments);
- The installation packages for Windows, Mac and Linux latest version. 

## Branch strategy
&nbsp;&nbsp;&nbsp; GitHub actions shall be used to implement the releasing of the product. It shall be configured to call release flow to start the creation of release product artifacts on creation or update the release branch.
TBD - describe naming and flow for branches.
TBD - create picture. 

## Building the code
&nbsp;&nbsp;&nbsp; For building the code you don't need to have any local environment to set up. 
1. So building the code is already done by github. I used [[37]](FVADocMD/REFERENCES.md) to help me to set up the main GitHub action flow and [this link](https://github.com/jurplel/install-qt-action) to set up QT.
As soon as any code change is pushed to GitHub, [main workflow](.github/workflows/main.yml) is being executed and you will have new [Installation packages](https://github.com/dimanikulin/fva/releases) to test.
2. But if you wish you can use [MS studio solution](./FVASW.sln) to build locally on  Windows or [CMake](./CMakeLists.txt) to build locally on any Windows, Mac or Linux. 

## Building the docs

&nbsp;&nbsp;&nbsp; To re-generate the docs you need to re-execute [documentation workflow](.github/workflows/releaseDocs.yml) from workflow. As result you will have [this](./DoxyGeneratedDoc.pdf) TODO - describe how 
