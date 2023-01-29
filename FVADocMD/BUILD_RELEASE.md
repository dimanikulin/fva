# Releasing the code and the docs
&nbsp;&nbsp;&nbsp; The release of product shall be on demand as soon as peace of product functionality is ready for release.
Thus the release branch is being created or updated to keep added/updated product functionality.

&nbsp;&nbsp;&nbsp; Regardless of incremental approach to add or update product functions, the artifacts shall accumulate whole product installation packages and not increments even really small piece of functions is released.

The following artifacts shall be created:
- The documentation for code (based on doxy comments);
- The installation packages for Windows, Mac and Linux latest version. 

Please notice the installation packages for Windows is built using Wix;
Once a release happened the current [documentation generated from the code](https://dimanikulin.github.io/fva/) appeared.

## Building the code 
&nbsp;&nbsp;&nbsp; There are 2 environments to build – CI (git hub) and local developer one. 
[CMake](https://gitlab.kitware.com/cmake/community/-/wikis/Home) is used as a tool to control build process. 
It is selected as cross-platform [tool](https://cmake.org/) available on all target platforms to build the package. 
[This link](https://doc.qt.io/qt-5/cmake-get-started.html) was used to make a build of QT code with a help of [CMake](https://gitlab.kitware.com/cmake/community/-/wikis/Home).

### Building the code using GitHub
&nbsp;&nbsp;&nbsp; The tool installation, configuration and processing are automated and based on GitHub actions with [main.yml](https://github.com/dimanikulin/fva/blob/master/.github/workflows/main.yml)
All the steps are expected to be yml based only.
I used [[37]](FVADocMD/REFERENCES.md) to help me to set up the main GitHub action flow and [this link](https://github.com/jurplel/install-qt-action) to set up QT.
&nbsp;&nbsp;&nbsp; Once code change is pushed to GitHub, [main workflow](https://github.com/dimanikulin/fva/blob/master/.github/workflows/main.yml) is being executed and you will have new [Installation packages](https://github.com/dimanikulin/fva/releases) to use.
All steps to build a package are called on any push to master or TBD branch.
More details are located in comments and step names [here](https://github.com/dimanikulin/fva/blob/master/.github/workflows/main.yml).

### Building the code locally
Still you can use [MS studio solution](./FVASW.sln) to build locally on Windows or [CMake](./CMakeLists.txt) to build locally on any Windows, Mac or Linux. 

## Building the docs
&nbsp;&nbsp;&nbsp; To re-generate the docs you need to re-execute [documentation workflow](https://github.com/dimanikulin/fva/blob/master/.github/workflows/releaseDocs.yml) from workflow. As result you will have [this](./DoxyGeneratedDoc.pdf)

# Branch strategy
&nbsp;&nbsp;&nbsp; GitHub actions is used to implement the releasing of the product. 
It is configured to call release flow to start the creation of release product artifacts on creation or update the release branch.
TBD - describe naming and flow for branches.
TBD - create picture. 