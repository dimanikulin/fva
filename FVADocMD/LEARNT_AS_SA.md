# What I learnt as an Application Architect
- "What?" - you will ask me, "Really so small applications requires the arhitect work?"
- "Yes, yes and yes" 0 my answer will be.

&nbsp;&nbsp;&nbsp; Usually the Arhitects create a big document called Solution Architecture Document (SAD)[../FVADocX/FVAToolSet-SAD.docx] plus [diagrams](../FVADocX/FVAToolSet-SAD diagrams.drawio) that keeps everyting inside.
first I used that way but it happened to be quite hard to trace the changes due to big frequence of changes.
So I decided to keep every note, every doc just in text files of MD format as this one ))).

## Requirements 
&nbsp;&nbsp;&nbsp; I started learning being architect from requirement elictions, but not requirements as for [Product Manager](./LEARNT_AS_PM.md) but more technical ones.
I divided architect requirements into 3 parts:
- **Constraints**;
- **Assumptions**;
- and **Quality Attributes**.

### Constraints
&nbsp;&nbsp;&nbsp; The constraints are limitation for product from techical point of view.
I listed [them](./REQUIREMENTS_SA.md#constraints) in form of table with columns:
- **Constraint** - to tell the constraint itself;
- **Description** - to add more details about constraint;
- **Business value** - to say it has High, Medium or Low value for business;
- **Architecture viewpoint** - to say if it is require High, Medium or Low resources to implement.

### Assumptions
&nbsp;&nbsp;&nbsp; Then there were the [assumptions](./REQUIREMENTS_SA.md#constraints) with explanations for them. 

### Quality Attributes
&nbsp;&nbsp;&nbsp; Last but no least were [quality attributes](./QUALITYATTRIBUTES_FVA.md).

It is provided if form of table with columns:
- **name**; 
- **description**;
- **motivation** to tell rationale for selecting;
- **measurable metrics** to show whether a particular quality attribute is achieved or not;
- priorities from **business** points of view;
- and priorities from **architecture** points of view.

But before filling up that table I had to understand what [Quality Attributes](./QUALITYATTRIBUTES.md) generally are.

# Software Architecture

## Context view 
&nbsp;&nbsp;&nbsp; The first architecture view was context diagram.
Generally, the [context diagram](./VIEWCONTEXT.md) shows the solution bounds and its interactions with other objects and subjects.
I used Mermaid.js to create context view diagram.
Mermaid.js is supported now by GitHub 

## Functional view
&nbsp;&nbsp;&nbsp; The next architecture view was functional diagram.
Generally, the [functional view](./VIEWFUNCTIONAL.md) shows the solution components divided by functional purpose and interactions between them.
and I used Mermaid.js to create that view diagram again.

## Deployment view
DEPLOYMENTAPPROACH.md


how to work with dependencies + old dependencies
Only open source code/tools are used, as a product is **open source**.

dependency 

Branch strategy
Releasing the code and the docs

C++ Documentation rules
Python Documentation rules

CODEDOCUMENTATION.md
CODEQUALITY.md

CODEREVIEW.md

## Links for architects 

- [Tools and dependencies](FVADocMD/DEPENDENCIES.md)
- [Old tools and dependencies](FVADocMD/DEPENDENCIES_OLD.md)
- [FVA License](FVADocMD/LICENSE.md)
- [Exif library license](FVACommonLib/LICENSE)
- [TODO-Hardware requirements](TODO)
- [Programming Languages](FVADocMD/PROGRAMMINGLANGUAGE.md)
- [Building and Releasing the product and documentation](FVADocMD/BUILD&RELEASE.md)
- [Deployment approach](FVADocMD/DEPLOYMENTAPPROACH.md)
- [Solution Architecture Quality Attributes](FVADocMD/QUALITYATTRIBUTES.md)


* how to create attractive README.md files (bages from shields, documents structure, required chapters);
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
