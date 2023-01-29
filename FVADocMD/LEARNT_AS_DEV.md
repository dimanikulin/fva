# What I learnt as a Software Developer
## Code quality
&nbsp;&nbsp;&nbsp; Before creating the code we need to think about code quality. 
Yes, it is first thing to start from.
So we shall be able to check code quality. 
It can be done automaticly by cool tools (provided by third parties) mentioned [here](./CODEQUALITY.md).
Unfortunatly automatic tools do not check all rules I need and...
I decided to check other rules manually during [code review](./CODEREVIEW.md) 
but when I spent much time on code reviewing and trying to check code I used **Python** to create other code [checks](../FVACodeChecks). 
Still, not every rule is checked automaticly and manual effort is excpected. 

## Code documentation
"Wait, wait!" - you will say, "Will we now start coding?"
"Nope" - my answer will be.

&nbsp;&nbsp;&nbsp; Also before creating the code we need to think about code documentation.
Because there are two languages used to create the **FVA Software**, there also should be: 
- **C++** [documentation rules](./CODEDOCUMENTATION.md) defined and
- **Python** documentation rules defined.

So I learnt how to document the C++ code using **Doxygen** markdown and build documentation basing on Doxygen markdown. 
O my god, for sure now I know how to configure **Doxygen**.

## Implementation status
&nbsp;&nbsp;&nbsp; Before jumping into coding I thought how I will know what requirement is covered and what is not.
So in simmply words, how tracing is going to be implemented.
What came up to my mind is a table with following columns
- **Implemented**, yes or no
- **Feature ID**, it is an identifier from [features](../FVADocX/FVAToolSetFeatures.pptx)
- **Component**, component name
- **File names**, file names where there is implementation for this requirement
- **Description** functional requirement description

So far I have **implementation status** for following components
- [Data processor](FVADataProcessor/IMPLEMENTATIONSTATUS.MD)
- [Configurator](FVAConfigurator/IMPLEMENTATIONSTATUS.MD)
- [Dictionary editor](FVADictionaryEditor/IMPLEMENTATIONSTATUS.MD)

## Implementation
&nbsp;&nbsp;&nbsp; Finally we can move to coding!
I chose [QT](https://en.wikipedia.org/wiki/Qt_(software)) as main library to use because the **FVA Software** requires [UI](https://en.wikipedia.org/wiki/User_interface) and support latest Windows, MacOS, Linux Ubuntu with the latest update.
For start I used **QWidgets** and its children **QWizard** and **QWizardPage**. 
But then I realized the use of **QML** will be more easier for [UI](https://en.wikipedia.org/wiki/User_interface) implementation.
So my plan is to move to use **QML** but not **QWidgets**. 
Also I was suprized how it is easy to use [QT](https://en.wikipedia.org/wiki/Qt_(software)) for creation the [player](../FVAPlayer) for Multimedia content. 
 
* how to build package file for WinOS using Wix;
* how to create and work with Python code from C++ application;
* how to work with SQLlite;
* how to work with CSV files;
* how to create riff parser;
* how to build the code using MS Visual Studio and CMake on local env or using GitHub on cloud env(Yml);
                                                                
[QTranslator](https://doc.qt.io/qt-5/qtranslator.html);
[QtextCodec](https://doc.qt.io/qt-5/qtextcodec.html);
[Qlocale](https://doc.qt.io/qt-5/qlocale.html);
[QtextDecoder](https://doc.qt.io/qt-5/qtextdecoder.html);
[QtextEncoder](https://doc.qt.io/qt-5/qtextencoder.html).

## Releasing the code and the docs
&nbsp;&nbsp;&nbsp; One more important point was definition of [building and releasing the product and documentation](./BUILD&RELEASE.md) 
There you can find a 

- [Building the code](./BUILD_RELEASE.md#buildingthecode) 
- [Building the code using GitHub](./BUILD_RELEASE.md#buildingthecodeusinggithub)
- [Building the code locally](./BUILD_RELEASE.md#buildingthecodelocally)
- [Building the docs](./BUILD_RELEASE.md#buildingthedocs)

- [Docs generated from the code](https://dimanikulin.github.io/fva/)
* how to buuld and release the documentation using GitHub;
* how to release the product using GitHub;