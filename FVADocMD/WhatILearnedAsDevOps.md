# Headline
TBD

# Article description
TBD 

# Tags
TBD

# Content
## Code Quality

### Automated code checks
&nbsp;&nbsp;&nbsp;  Currently there are following automated checks to verify if the code meets code quality requirements:
* [Code QL](.github/workflows/codeqlanalysis.yml) Please see [[38]](./REFERENCES.md) TBD what for
* [code Checks](.github/workflows/codeChecks.yml) TBD what for
* [code factor](https://www.codefactor.io/repository/github/dimanikulin/fva/issues) TBD what for
* [ms vc analysis](.github/workflows/msvc-analysis.yml) TBD what for
TODO - to add a bage for each code check

## Releasing the code and the docs
&nbsp;&nbsp;&nbsp; The release of product shall be on demand as soon as peace of product functionality is ready for release.
Thus the release branch is being created or updated to keep added/updated product functionality.

&nbsp;&nbsp;&nbsp; Regardless of incremental approach to add or update product functions, the artifacts shall accumulate whole product installation packages and not increments even really small piece of functions is released.

The following artifacts shall be created:
- The documentation for code (based on doxy comments);
- The installation packages for Windows, Mac and Linux latest version. 

Please notice the installation packages for Windows is built using Wix;
Once a release happened the current [documentation generated from the code](https://dimanikulin.github.io/fva/) appeared.

### Building the code 
&nbsp;&nbsp;&nbsp; There are 2 environments to build – CI (git hub) and local developer one. 
[CMake](https://gitlab.kitware.com/cmake/community/-/wikis/Home) is used as a tool to control build process. 
It is selected as cross-platform [tool](https://cmake.org/) available on all target platforms to build the package. 
[This link](https://doc.qt.io/qt-5/cmake-get-started.html) was used to make a build of QT code with a help of [CMake](https://gitlab.kitware.com/cmake/community/-/wikis/Home).

#### Building the code using GitHub
&nbsp;&nbsp;&nbsp; The tool installation, configuration and processing are automated and based on GitHub actions with [main.yml](https://github.com/dimanikulin/fva/blob/master/.github/workflows/main.yml)
All the steps are expected to be yml based only.
I used [[37]](FVADocMD/REFERENCES.md) to help me to set up the main GitHub action flow and [this link](https://github.com/jurplel/install-qt-action) to set up QT.
&nbsp;&nbsp;&nbsp; Once code change is pushed to GitHub, [main workflow](https://github.com/dimanikulin/fva/blob/master/.github/workflows/main.yml) is being executed and you will have new [Installation packages](https://github.com/dimanikulin/fva/releases) to use.
All steps to build a package are called on any push to master or TBD branch.
More details are located in comments and step names [here](https://github.com/dimanikulin/fva/blob/master/.github/workflows/main.yml).
TODO explain how to install and cfg on github site
#### Building the code locally
Still you can use [MS studio solution](./FVASW.sln) to build locally on Windows or [CMake](./CMakeLists.txt) to build locally on any Windows, Mac or Linux. 

### Building the docs
&nbsp;&nbsp;&nbsp; To re-generate the docs you need to re-execute [documentation workflow](https://github.com/dimanikulin/fva/blob/master/.github/workflows/releaseDocs.yml) from workflow. As result you will have [this](./DoxyGeneratedDoc.pdf)

## Branch strategy
&nbsp;&nbsp;&nbsp; GitHub actions is used to implement the releasing of the product. 
It is configured to call release flow to start the creation of release product artifacts on creation or update the release branch.
TBD - describe naming and flow for branches.
TBD - create picture. 

# References
| # | Name                 | Source                | Release date           |  Author                 | Description   |
| - | ---------------------|---------------------- |----------------------- | ----------------------- |:-------------:|
| 1 | Using GitHub Actions with C++ and CMake | [Web](https://cristianadam.eu/20191222/using-github-actions-with-c-plus-plus-and-cmake/) | DEC 22, 2019| Cristian Adam |Providing a GitHub Actions configuration yaml file for C++ projects using CMake|
| 2 | Code scanning finds more vulnerabilities using machine learning|[Web](https://github.blog/2022-02-17-code-scanning-finds-vulnerabilities-using-machine-learning/)| February 17, 2022 | Tiferet Gazit, Alona Hlobina | |


# Definitions, Acronyms, Abbreviations
| # | Abbreviation or Acronym | Definition     |
| - | ------------------------|:--------------:|
| 1 | [Qt](https://en.wikipedia.org/wiki/Qt_(software))|is a [widget toolkit](https://en.wikipedia.org/wiki/Widget_toolkit) for creating [graphical user interfaces](https://en.wikipedia.org/wiki/Graphical_user_interfaces) as well as [cross-platform applications](https://en.wikipedia.org/wiki/Cross-platform) that run on various software and hardware platforms such as [Linux](https://en.wikipedia.org/wiki/Linux), [Windows](https://en.wikipedia.org/wiki/Windows), [macOS](https://en.wikipedia.org/wiki/MacOS), [Android](https://en.wikipedia.org/wiki/Android_(operating_system)) or [embedded systems](https://en.wikipedia.org/wiki/Embedded_system) with little or no change in the underlying codebase while still being a native application with native capabilities and speed. |
| 2 | [Cpplint](https://github.com/google/styleguide/blob/gh-pages/cpplint/cpplint.py)|is a C++ static code analysis tool which looks for programming errors, helps enforcing a coding standard, sniffs for code smells and offers simple refactoring suggestions. |
| 3 | [Pylint](https://pypi.org/project/pylint/)| is a Python static code analysis tool which looks for programming errors, helps enforcing a coding standard, sniffs for code smells and offers simple refactoring suggestions.|
