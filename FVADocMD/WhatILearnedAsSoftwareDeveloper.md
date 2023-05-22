# Headline
Code Quality, Documentation, and Implementation: Building a Solid Foundation for Software Development

What I learned as a Software Developer while creating my product

# Article description
Here you will
- Discover the essential aspects of software development, including code quality, documentation practices, and implementation strategies 
- Explore the importance of checking code quality using automated tools while also recognizing the value of manual code review for adherence to coding standards
- Learn about effective code documentation techniques in both C++ and Python languages, and uncover the significance of tracking implementation status for better requirement coverage
- Dive into the implementation phase using the versatile QT library for user interface development, exploring the transition from QWidgets to QML
- Gain insights into leveraging various UI languages, working with databases like SQLite and CSV, and bridging the gap between C++ and Python code
- Lastly, explore the creation of a riff parser, adding an additional skill set to your software development toolkit 

# Tags
Software development, code quality, code review, coding standards, documentation practices, C++, Python, implementation status, user interface development, QT library, QWidgets, QML, UI languages, SQLite, CSV, C++ and Python integration, riff parser

# Content

## Code quality
Before creating the code, it is crucial to prioritize its quality.
It should be the foremost consideration in the development process.
To ensure code quality, we can utilize automated tools that help with code analysis.
 owever, it's important to note that automated tools may not cover all the necessary rules. 
As a result, manual code review becomes essential to enforce coding standards.
Personally, I have dedicated significant time to code reviewing and checking for adherence to [code style](https://github.com/dimanikulin/fva/blob/master/CodeStyle.md). Additionally, 
I have employed **Python** to create additional code checks.
It is worth mentioning that not all rules can be checked automatically, requiring manual effort in certain cases.

## Code documentation
"*Now, hold on*!" you might ask, 
"*Are we jumping straight into coding?*". 
My answer is a resounding "*Nope*".

Before embarking on the coding phase, it is vital to consider code documentation. 
Since the **FVA Software** employs two programming languages, it is imperative to establish clear documentation rules for both [C++](https://github.com/dimanikulin/fva/blob/master/CodeDocumentation.md) and **Python**.
 
Consequently, I have familiarized myself with documenting **C++** code using **Doxygen** markdown and have gained proficiency in generating documentation based on Doxygen markdown.
It's quite remarkable how confident I am now in configuring **Doxygen**.

<img src="CodeDocumentation.png" alt="CodeDocumentation"/>

## Implementation status
Before jumping into coding I thought about how I would know which requirement is covered and which is not.
In simple words, how tracing is going to be implemented.
What came up to my mind was a table with the following columns
- **Implemented**, yes or no
- **Feature ID**, an identifier from features
- **Component**, component name
- **File names**, file names where implementation for this requirement is present
- **Description**, functional requirement description

Up to this point, I have **implementation status** for the following components:
- [Data processor](https://github.com/dimanikulin/fva/blob/master/FVADataProcessor/IMPLEMENTATIONSTATUS.MD)
- [Configurator](https://github.com/dimanikulin/fva/blob/master/FVAConfigurator/IMPLEMENTATIONSTATUS.MD)
- [Dictionary editor](https://github.com/dimanikulin/fva/blob/master/FVADictionaryEditor/IMPLEMENTATIONSTATUS.MD)

## Implementation
Finally, we can move to coding!
I chose [QT](https://en.wikipedia.org/wiki/Qt_(software)) as the main library to use as **FVA Software** requires [UI](https://en.wikipedia.org/wiki/User_interface) and supports Windows, MacOS, Linux Ubuntu with the latest update.
To begin with, I used **QWidgets** and its children - **QWizard** and **QWizardPage**. 
But then I realized that using **QML** will be easier for [UI](https://en.wikipedia.org/wiki/User_interface) implementation.
Thus, my plan is to move to using **QML** but not **QWidgets**. 
I was suprised how easy it is to use [QT](https://en.wikipedia.org/wiki/Qt_(software)) for creation of the [player](https://github.com/dimanikulin/fva/blob/master/FVAPlayer) for Multimedia content. 
I'm going to use several [UI](https://en.wikipedia.org/wiki/User_interface) languages, and [QT](https://en.wikipedia.org/wiki/Qt_(software)) suggests to use the following: 

- [QTranslator](https://doc.qt.io/qt-5/qtranslator.html);
- [QtextCodec](https://doc.qt.io/qt-5/qtextcodec.html);
- [Qlocale](https://doc.qt.io/qt-5/qlocale.html);
- [QtextDecoder](https://doc.qt.io/qt-5/qtextdecoder.html);
- [QtextEncoder](https://doc.qt.io/qt-5/qtextencoder.html).

I was really glad to find out how easy it is to work with SQLlite with a help of [QT](https://en.wikipedia.org/wiki/Qt_(software)).
Nevertheless, I then moved from SQLlite to [CSV](https://github.com/dimanikulin/fva/blob/master/FVACommonLib/fvacsvfile.h) to keep the data. 

As I use two [languages](https://github.com/dimanikulin/fva/blob/master/README.md#programming-languages), there was a need to work with **Python** code from **C++** application.
 
Last but not least, I learned how to create [riff parser](https://github.com/dimanikulin/fva/blob/master/FVACommonLib/fvariffparser.h).

# References
| # | Name                 | Source                | Release date           |  Author                 | Description   |
| - | ---------------------|---------------------- |----------------------- | ----------------------- |:-------------:|
| 1 | FVA Tool Set features | [local resource](./FVADocX/FVAToolSetFeatures.pptx) | June 2022 | Dmytro Nikulin | | 

# Definitions, Acronyms, Abbreviations
| # | Abbreviation or Acronym | Definition     |
| - | ------------------------|:--------------:|
| 1 | [Dictionary](https://en.wikibooks.org/wiki/A-level_Computing/AQA/Paper_1/Fundamentals_of_data_structures/Dictionaries)|A dictionary is a general-purpose data structure for storing a group of objects. A dictionary has a set of keys and each key has a single associated value. When presented with a key, the dictionary will return the associated value. |
| 2 | [Doxygen](https://en.wikipedia.org/wiki/Doxygen)| [Doxygen](https://en.wikipedia.org/wiki/Doxygen)  is a [documentation generator](https://en.wikipedia.org/wiki/Documentation_generator) and [static analysis](https://en.wikipedia.org/wiki/Static_analysis) tool for software [source trees](https://en.wikipedia.org/wiki/Source_code#Organization). When used as a documentation generator, Doxygen extracts information from specially-formatted comments within the code.|
| 3 | [exiftool](https://en.wikipedia.org/wiki/ExifTool)| ExifTool is a [free and open-source software](https://en.wikipedia.org/wiki/Free_and_open-source_software) program for reading, writing, and manipulating image, audio, video, and [PDF](https://en.wikipedia.org/wiki/Portable_Document_Format) [metadata](https://en.wikipedia.org/wiki/Metadata)|
| 4 | [IDE](https://en.wikipedia.org/wiki/Integrated_development_environment)| An integrated development environment (IDE) is a [software application](https://en.wikipedia.org/wiki/Application_software) that provides comprehensive facilities to [computer programmers](https://en.wikipedia.org/wiki/Computer_programmer) for [software development](https://en.wikipedia.org/wiki/Software_development). An IDE normally consists of at least a [source code editor](https://en.wikipedia.org/wiki/Source_code_editor), build [automation tools](https://en.wikipedia.org/wiki/Build_automation) and a [debugger](https://en.wikipedia.org/wiki/Debugger).|
| 5 | [Qt](https://en.wikipedia.org/wiki/Qt_(software))|is a [widget toolkit](https://en.wikipedia.org/wiki/Widget_toolkit) for creating [graphical user interfaces](https://en.wikipedia.org/wiki/Graphical_user_interfaces) as well as [cross-platform applications](https://en.wikipedia.org/wiki/Cross-platform) that run on various software and hardware platforms such as [Linux](https://en.wikipedia.org/wiki/Linux), [Windows](https://en.wikipedia.org/wiki/Windows), [macOS](https://en.wikipedia.org/wiki/MacOS), [Android](https://en.wikipedia.org/wiki/Android_(operating_system)) or [embedded systems](https://en.wikipedia.org/wiki/Embedded_system) with little or no change in the underlying codebase while still being a native application with native capabilities and speed. |
| 6 | [UI](https://en.wikipedia.org/wiki/User_interface)| is the space where interactions between humans and machines occur. The goal of this interaction is to allow effective operation and control of the machine from the human end, whilst the machine simultaneously feeds back information that aids the operators' [decision-making](https://en.wikipedia.org/wiki/Decision-making) process|