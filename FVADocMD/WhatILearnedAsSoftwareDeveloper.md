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
Before diving into the coding phase, I considered how I could effectively track the coverage of requirements. 
I needed a way to implement tracing and determine which requirements were covered and which ones were not. 
To address this, I devised a table with the following columns:

**Implemented**: Indicates whether the requirement has been implemented, marked as either "yes" or "no."
**Feature ID**: An identifier sourced from the features.
**Component**: Specifies the name of the component.
**File names**: Lists the file names where the implementation for the requirement can be found.
**Description**: Provides a description of the functional requirement.

As of now, I have the **implementation status** for the following components:
- [Data processor](https://github.com/dimanikulin/fva/blob/master/FVADataProcessor/IMPLEMENTATIONSTATUS.MD)
- [Configurator](https://github.com/dimanikulin/fva/blob/master/FVAConfigurator/IMPLEMENTATIONSTATUS.MD)
- [Dictionary editor](https://github.com/dimanikulin/fva/blob/master/FVADictionaryEditor/IMPLEMENTATIONSTATUS.MD)

These references will give you an overview of the implementation status for each respective component.

## Implementation
Finally, we can proceed to the coding phase!
For the development of **FVA Software**, I have chosen the versatile [QT](https://en.wikipedia.org/wiki/Qt_(software)) library, which supports multiple operating systems such as Windows, MacOS, and Linux Ubuntu. 
Since the software requires a user interface ([UI](https://en.wikipedia.org/wiki/User_interface)), I initially utilized **QWidgets**, including components like **QWizard** and **QWizardPage**.
However, I soon realized that employing **QML** would offer a more streamlined approach for [UI](https://en.wikipedia.org/wiki/User_interface) implementation.
Therefore, my plan is to transition from **QWidgets** to **QML**.

I was pleasantly surprised by the ease of utilizing **QT** for creating a multimedia content [player](https://github.com/dimanikulin/fva/blob/master/FVAPlayer). 
The flexibility of [QT](https://en.wikipedia.org/wiki/Qt_(software)) allows me to work with multiple [UI](https://en.wikipedia.org/wiki/User_interface) languages.
To facilitate this, [QT](https://en.wikipedia.org/wiki/Qt_(software)) suggests using the following components:

- [QTranslator](https://doc.qt.io/qt-5/qtranslator.html);
- [QtextCodec](https://doc.qt.io/qt-5/qtextcodec.html);
- [Qlocale](https://doc.qt.io/qt-5/qlocale.html);
- [QtextDecoder](https://doc.qt.io/qt-5/qtextdecoder.html);
- [QtextEncoder](https://doc.qt.io/qt-5/qtextencoder.html).

Additionally, I discovered that working with **SQLite** using [QT](https://en.wikipedia.org/wiki/Qt_(software)) is remarkably straightforward. 
However, I later made the decision to switch from **SQLite** to [CSV](https://github.com/dimanikulin/fva/blob/master/FVACommonLib/fvacsvfile.h) for data storage.

As my project involves working with two programming [languages](https://github.com/dimanikulin/fva/blob/master/README.md#programming-languages), I needed to establish a means of integrating **Python** code into the **C++** application.

Last but not least, I acquired knowledge on how to create a [riff parser](https://github.com/dimanikulin/fva/blob/master/FVACommonLib/fvariffparser.h), adding another valuable capability to the project.
 
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