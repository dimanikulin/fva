# What I learned as a Software Developer while creating my own product
## Code quality
&nbsp;&nbsp;&nbsp; Before creating the code we need to think about its quality. 
Yes, it's the first thing to start from.
So, we should be able to check code quality. 
It can be done automatically by cool tools (provided by third parties) mentioned [here](./CODEQUALITY.md).
Unfortunately, automatic tools do not check all rules I need and...
I decided to check other rules manually during [code review](./CODEREVIEW.md).
However, when I spent a lot of time on code reviewing and trying to check the code, I used **Python** to create other code [checks](../FVACodeChecks). 
Still, not every rule can be checked automatically, and manual effort is excpected. 

## Code documentation
"Hold on!", - you'll say. - "Are we going to start coding now?".
"Nope", - my answer will be.

&nbsp;&nbsp;&nbsp; Additionally, before creating the code we need to think about code documentation.
Since there are two languages used to create the **FVA Software**, there also should be: 
- **C++** [documentation rules](./CODEDOCUMENTATION.md) defined and
- **Python** documentation rules defined.

Therefore, I learned how to document the C++ code using **Doxygen** markdown and build documentation basing on Doxygen markdown. 
Oh my God, now I definitely know how to configure **Doxygen**.

## Implementation status
&nbsp;&nbsp;&nbsp; Before jumping into coding I thought about how I would know which requirement is covered and which is not.
In simple words, how tracing is going to be implemented.
What came up to my mind was a table with the following columns
- **Implemented**, yes or no
- **Feature ID**, an identifier from [features](../FVADocX/FVAToolSetFeatures.pptx)
- **Component**, component name
- **File names**, file names where implementation for this requirement is present
- **Description**, functional requirement description

Up to this point, I have **implementation status** for the following components:
- [Data processor](FVADataProcessor/IMPLEMENTATIONSTATUS.MD)
- [Configurator](FVAConfigurator/IMPLEMENTATIONSTATUS.MD)
- [Dictionary editor](FVADictionaryEditor/IMPLEMENTATIONSTATUS.MD)

## Implementation
&nbsp;&nbsp;&nbsp; Finally, we can move to coding!
I chose [QT](https://en.wikipedia.org/wiki/Qt_(software)) as the main library to use as **FVA Software** requires [UI](https://en.wikipedia.org/wiki/User_interface) and supports Windows, MacOS, Linux Ubuntu with the latest update.
To begin with, I used **QWidgets** and its children - **QWizard** and **QWizardPage**. 
But then I realized that using **QML** will be easier for [UI](https://en.wikipedia.org/wiki/User_interface) implementation.
Thus, my plan is to move to using **QML** but not **QWidgets**. 
I was suprised how easy it is to use [QT](https://en.wikipedia.org/wiki/Qt_(software)) for creation of the [player](../FVAPlayer) for Multimedia content. 
I'm going to use several [UI](https://en.wikipedia.org/wiki/User_interface) languages, and [QT](https://en.wikipedia.org/wiki/Qt_(software)) suggests to use the following: 

- [QTranslator](https://doc.qt.io/qt-5/qtranslator.html);
- [QtextCodec](https://doc.qt.io/qt-5/qtextcodec.html);
- [Qlocale](https://doc.qt.io/qt-5/qlocale.html);
- [QtextDecoder](https://doc.qt.io/qt-5/qtextdecoder.html);
- [QtextEncoder](https://doc.qt.io/qt-5/qtextencoder.html).

I was really glad to find out how easy it is to work with SQLlite with a help of [QT](https://en.wikipedia.org/wiki/Qt_(software)).
Nevertheless, I then moved from SQLlite to [CSV](../FVACommonLib/fvacsvfile.h) to keep the data. 

As I use two [languages](./PROGRAMMINGLANGUAGE.md), there was a need to work with **Python** code from **C++** application.
 
Last but not least, I learned how to create [riff parser](../FVACommonLib/fvariffparser.h).
                                                                
## Releasing code and docs
&nbsp;&nbsp;&nbsp; One more important point was a definition of [building and releasing the product and documentation](./BUILD_RELEASE.md) 
Here you can find a description for: 
- [Building the code](./BUILD_RELEASE.md#buildingthecode) 
- [Building the code using GitHub](./BUILD_RELEASE.md#buildingthecodeusinggithub)
- [Building the code locally](./BUILD_RELEASE.md#buildingthecodelocally)
- [Building the docs](./BUILD_RELEASE.md#buildingthedocs)

Thus, I learned:
- how to build and release documentation using GitHub;
- how to release product using GitHub.
