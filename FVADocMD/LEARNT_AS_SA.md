# What I learnt as an Application Architect

how to work with dependencies + old dependencies
Only open source code/tools are used, as a product is **open source**.

dependency 

Branch strategy
Releasing the code and the docs

C++ Documentation rules
Python Documentation rules

PROGRAMMINGLANGUAGE.md
QUALITYATTRIBUTES
REQUIREMENTS
REQUIREMENTS_SA

VIEWCONTEXT.md
VIEWFUNCTIONAL.md

BUILD&RELEASE.md
CODEDOCUMENTATION.md
CODEQUALITY.md
DEPENDENCIES.md
DEPENDENCIES_OLD.md
DEPLOYMENTAPPROACH.md
LICENSE.md

QUALITYATTRIBUTES_FVA.md

SAD.docx
C:\work\fva\FVADocX\FVAToolSet-SAD diagrams.drawio

## Links for architects 
- [Solution Architecture Document](FVADocX/SAD.docx)
- [Solution Architecture Requirements](FVADocMD/REQUIREMENTS_SA.md)
- [Context View](FVADocMD/VIEWCONTEXT.md)
- [Functional Decomposition](FVADocMD/VIEWFUNCTIONAL.md)
- [Tools and dependencies](FVADocMD/DEPENDENCIES.md)
- [Old tools and dependencies](FVADocMD/DEPENDENCIES_OLD.md)
- [FVA License](FVADocMD/LICENSE.md)
- [Exif library license](FVACommonLib/LICENSE)
- [TODO-Hardware requirements](TODO)
- [Programming Languages](FVADocMD/PROGRAMMINGLANGUAGE.md)
- [Building and Releasing the product and documentation](FVADocMD/BUILD&RELEASE.md)
- [Deployment approach](FVADocMD/DEPLOYMENTAPPROACH.md)
- [Solution Architecture Quality Attributes](FVADocMD/QUALITYATTRIBUTES.md)

CODEREVIEW.md

* how to create attractive README.md files (bages from shields, documents structure, required chapters, diagrams on Mermaid.js);
## You will learn - documentation chapter
* how to document the C++ code using Doxygen markdown and build documentation basing on Doxygen markdown;
* how to buuld and release the documentation;
* how to release the product;
* how to create the license;
* how to configure Doxygen;

## You will learn - code checks chapter
* how to use Python to create code checks;
* how to control code quality (from "Code checks" chapter);

## Implementation status
- [Data processor](FVADataProcessor/IMPLEMENTATIONSTATUS.MD)
- [Configurator](FVAConfigurator/IMPLEMENTATIONSTATUS.MD)
- [Dictionary editor](FVADictionaryEditor/IMPLEMENTATIONSTATUS.MD)


# What I learnt as a Software Developer

Building the code 
Building the code using GitHub
Building the code locally
Building the docs

## Links for developers
- [Requirements for documentation](FVADocMD/CODEDOCUMENTATION.md)
- [Docs generated from the code](https://dimanikulin.github.io/fva/)
- [Code Quality](FVADocMD/CODEQUALITY.md)
- [Where to publish](FVADocMD/WHERETOPUBLISH.md)

## You will learn - code chapter
* how to use QT for creation the player for Multimedia content;
* how to build package file for WinOS using Wix;
* how to create and work with Python code from C++ application;
* how to work with SQLlite;
* how to work with QWizard and QWizardPage;
* how to work with CSV files;
* how to create riff parser;
* how to build the code using MS Visual Studio and CMake on local env or using GitHub on cloud env(Yml);

The native fast API to implement [UI](https://en.wikipedia.org/wiki/User_interface) is developed based on [QT](https://en.wikipedia.org/wiki/Qt_(software)). 
The [QT](https://en.wikipedia.org/wiki/Qt_(software)) library is also used for the implementation to support latest Windows, MacOS, Linux Ubuntu with the latest update. 
The QML shall be used for UI implementation and not QWidgets.

the [QT](https://en.wikipedia.org/wiki/Qt_(software)) shall be used with such features as:
[QTranslator](https://doc.qt.io/qt-5/qtranslator.html);
[QtextCodec](https://doc.qt.io/qt-5/qtextcodec.html);
[Qlocale](https://doc.qt.io/qt-5/qlocale.html);
[QtextDecoder](https://doc.qt.io/qt-5/qtextdecoder.html);
[QtextEncoder](https://doc.qt.io/qt-5/qtextencoder.html).
