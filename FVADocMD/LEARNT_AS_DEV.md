# What I learnt as a Software Developer

C++ Documentation rules
Python Documentation rules

CODEDOCUMENTATION.md
CODEQUALITY.md

CODEREVIEW.md


## Releasing the code and the docs & Branch strategy
&nbsp;&nbsp;&nbsp; One more important point was definition of [building and releasing the product and documentation](./BUILD&RELEASE.md) 
There you can find a 

- [Building the code](./BUILD_RELEASE.md#buildingthecode) 
- [Building the code using GitHub](./BUILD_RELEASE.md#buildingthecodeusinggithub)
- [Building the code locally](./BUILD_RELEASE.md#buildingthecodelocally)
- [Building the docs](./BUILD_RELEASE.md#buildingthedocs)


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
