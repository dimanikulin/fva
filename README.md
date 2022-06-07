<p align="center">
  <a href="FVAIcons/main.png" target="blank"><img src="FVAIcons/main.png" width="120" alt="FVA Logo" /></a>
</p>
<p align="center">
  <img alt="GitHub Repo stars" src="https://img.shields.io/github/stars/dimanikulin/fva?style=social">
  <img alt="GitHub contributors" src="https://img.shields.io/github/contributors/dimanikulin/fva">
  <img alt="GitHub followers" src="https://img.shields.io/github/followers/dimanikulin?style=social">
  <img alt="GitHub Sponsors" src="https://img.shields.io/github/sponsors/dimanikulin">
  <img alt="Twitter URL" src="https://img.shields.io/twitter/url?style=social&url=https%3A%2F%2Ftwitter.com%2FDmytroNikulin" href="https%3A%2F%2Ftwitter.com%2FDmytroNikulin">
  </br>
  <img alt="GitHub release (latest by date including pre-releases)" src="https://img.shields.io/github/v/release/dimanikulin/fva?include_prereleases">
  <img alt="GitHub all releases" src="https://img.shields.io/github/downloads/dimanikulin/fva/total">
  <img alt="GitHub Release Date" src="https://img.shields.io/github/release-date-pre/dimanikulin/fva">
  </br>
  <a href="https://github.com/dimanikulin/fva/actions/workflows/main.yml"><img src="https://github.com/dimanikulin/fva/actions/workflows/main.yml/badge.svg?branch=master" alt="Tests"/></a>
  <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/m/dimanikulin/fva">
  <img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/dimanikulin/fva">  
  <a href="https://www.codefactor.io/repository/github/dimanikulin/fva"><img src="https://www.codefactor.io/repository/github/dimanikulin/fva/badge" alt="CodeFactor" /></a>
  </br>
  <img alt="GitHub issues" src="https://img.shields.io/github/issues/dimanikulin/fva">
  <img alt="GitHub closed issues" src="https://img.shields.io/github/issues-closed/dimanikulin/fva">
  <img alt="GitHub pull requests" src="https://img.shields.io/github/issues-pr/dimanikulin/fva">
  <img alt="GitHub closed pull requests" src="https://img.shields.io/github/issues-pr-closed/dimanikulin/fva">
  </br>
  <img alt="GitHub top language" src="https://img.shields.io/github/languages/top/dimanikulin/fva">
  <img alt="GitHub language count" src="https://img.shields.io/github/languages/count/dimanikulin/fva">
  <img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/dimanikulin/fva">
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/dimanikulin/fva">
  </br>
  <a href="https://github.com/dimanikulin/fva/projects/4"><img src="https://img.shields.io/badge/roadmap-view-d90368"> </a>
  <img alt="GitHub Discussions" src="https://img.shields.io/github/discussions/dimanikulin/fva">
  <img alt="GitHub" src="https://img.shields.io/github/license/dimanikulin/fva">
  <a href="https://github.com/dimanikulin/fva/edit/master/README.md"><img src="https://img.shields.io/badge/documentation-read-d90368"> </a>
</p>

## Quick Links
- [Description](#description)
- [Features](#features)
- [User documentation](#user-documentation)
- [User scenarios with links to YouTube](#user-scenarios)
- [Documentation](#documentation)
- [Implementation status](#impl-status)
- [Non-functional requirements](#non-functional-requirements)
- [Building the code](#building-the-code)
- [Contributing](#contributing)
- [Stay in touch](#stay-in-touch)
- [Code Quality](FVADocMD/CODEQUALITY.md)
- [What you will learn here as a SW Developer](FVADocMD/TECHSTOLEARN.md)
- [FVA License](FVADocMD/LICENSE.md)
- [Exif library license](FVACommonLib/LICENSE)
- [References to external documents like web resources, publication, patents etc. (applicable for all other documents)](FVADocMD/REFERENCES.md)
- [Questionar to clarify user needs](https://docs.google.com/forms/d/1EI3oOumRnHxDjEYgV6PFB-AMfV5plLtuV2r5S8BYn_g/)
- [Definitions,Acronyms,Abbreviations](FVADocMD/DEFINITIONS.MD)

## Description
A convergent tool set in form of desktop applications to automate the process of importing the multimedia data (photo/audio/video)
into the IR multimedia systems like digiKam, Google Photo, Apple Photo, Synology Photo Station, etc. to make the multimedia more searchable

The main functions(basing on user search needs) of that tool set are: 
* to automate the import (with decreasing the probability of human factor influence on the process of importing new photos into the existing photo album). 
* to make photo album more searchable (that is driven by IR Multimedia Systems) by enriching the metadata (and improving its consistency) with minimal human involvement.

Despite there are a lot of steps that can be automated, still a human involvement is required.
 
The solution is oriented on end users only.
Only opensource code/tools are used as a product is opensource.

## Features
- **Organizing the mutlimedia:** Organizing the multimedia data to be more searchable
- **Integration:** The system is integrated with online/offline Multimedia IR systems:
[digiKam](https://www.digikam.org/) <img src="FVADocMD/Images/digiKam.png" alt="digiKam.png" width="20" height="20"/> as Offline Multimedia IR System and 
[Synology Moments](https://www.synology.com/en-global/dsm/feature/moments) <img src="FVADocMD/Images/SynologyMoments.png" alt="SynologyMoments.png" height="20" /> as Online Multimedia IR System are now supported. 
- **Multimedia metadata gaps identification:** It identifies the gaps and tryies to fill it up.
- **Text search:** It is possible to search by events, photo author, places, people using text search   
- **Configuration:** The system flow depends on configuration.

## User documentation
* [Here is author history on why he created this product](FVADocMD/AUTHORHISTORY.md)
* [What is Multimedia Data?](FVADocMD/MULTIMEDIADATA.md)
* [What are Multimedia IR Systems?](FVADocMD/MULTIMEDIAIRSYSTEMS.md)
* [Multimedia IR Systems analyze](FVADocMD/MULTIMEDIAIRSYSTEMSANALYZE.md)
* [Multimedia Organizers](FVADocMD/MULTIMEDIAORGANIZERS.md)
* [User Journey to understand the cadence of FVA SW in use and the interaction with the user](TODO)

You can download the [Installation packages](https://github.com/dimanikulin/fva/releases) that are published for Linux (x86-64), Mac and Windows (latest versions).
Then you just need to install it on environment you would like to have it running on.
TBD - define HARDWARE REQUIREMENTS

## User scenarios with links to YouTube
* [Scenario without FVA SW](https://youtu.be/_bLXFdIDZtA)
* [Scenario with FVA SW](https://youtu.be/-xnUsO7zW2s)
* [Checking the results of processing](https://youtu.be/NIsVYyioDnM)

## Documentation
* [What you will learn here as a SW Developer](FVADocMD/TECHSTOLEARN.md)
* Please see [the documentation generated from the code](https://dimanikulin.github.io/fva/)
* To re-generate the docs you need to re-execute [documentation workflow](.github/workflows/releaseDocs.yml) from workflow. As result you will have [this](./DoxyGeneratedDoc.pdf)  TBD - describe how 
* [Roadmap](https://github.com/dimanikulin/fva/projects/4).
* [Requirements for documentation](FVADocMD/CODEDOCUMENTATION.md)
* [Tools and dependencies](FVADocMD/DEPENDENCIES.md)
* [Old tools and dependencies](FVADocMD/DEPENDENCIES_OLD.md)
* [Context View](FVADocMD/VIEWCONTEXT.md)
* [Functional Decomposition](TBD)

## Implementation status
* [Data processor](FVADataProcessor/IMPLEMENTATIONSTATUS.MD)

## Non-functional requirements
TBD

## Building the code
For building the code you don't need to have any local environment to set up. 
1. So building the code is already done by github. I used [[37]](FVADocMD/REFERENCES.md) to help me to set up main GitHub action flow and [this link](https://github.com/jurplel/install-qt-action) to set up QT.
As soon as any code change is pushed to the GitHub, [main workflow](.github/workflows/main.yml) is being executed and you will have new [Installation packages](https://github.com/dimanikulin/fva/releases) to test.
2. But if you wish you can use [MS studio solution](./FVASW.sln) to build locally on  Windows or [CMake](./CMakeLists.txt) to build locally on any Windows, Mac or Linux. 

## Contributing
You are always welcome to contribute! There are many items to help author in. 
When commit please don't forget about putting the issue ID there.
TBD - to define the BRANCHING STRATEGY PLAN 

## Stay in touch
An author does not provide any support for now, because the development is in hot stage. 
But you can ask him on:  
* [Author Twitter](https://twitter.com/DmytroNikulin)
* [Author LinkedIn](https://www.linkedin.com/in/dimanikulin/)
* [Author Telegram](https://t.me/dimanikulin79) 
* [Community Twitter](https://twitter.com/TBD)

You can also leave an issue here [GitHub issues](https://github.com/dimanikulin/fva/issues) to request a feature or report a bug.
[Here](https://github.com/dimanikulin/fva/discussions) you can start a discussion. 
