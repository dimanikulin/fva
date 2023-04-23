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

# Description
FVA Software (Photo-Video-Audio Software) is a convergent tool set in a form of desktop applications aimed at automating importing process of [Multimedia Data](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaData.md) (photo/audio/video)
into [Multimedia IR systems](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaIRSystems.md) like [digiKam](https://www.digikam.org/) 
<img src="./FVADocMD/Images/digiKam.png" alt="digiKam.png" width="20" height="20"/>,
 Google Photo, Apple Photo, [Synology Moments](https://www.synology.com/en-global/dsm/feature/moments) 
<img src="./FVADocMD/Images/SynologyMoments.png" alt="SynologyMoments.png" height="20" />, 
etc. to make multimedia more fully-searchable as well as to refine family photos.
<img src="./FVADocMD/Images/InputAndOutputExamples.gif" alt="Input And Output Examples" />

# Capabilities
- **Organizing the multimedia:** Organizing the [Multimedia Data](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaData.md) to be more searchable
- **Multimedia metadata gaps identification:** It identifies the gaps and tries to fill it up (enriching the metadata and improving its consistency with minimal human involvement).
- **Integration:** The system is integrated with online/offline [Multimedia IR systems](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaIRSystems.md):
[digiKam](https://www.digikam.org/) <img src="./FVADocMD/Images/digiKam.png" alt="digiKam.png" width="20" height="20"/> as Offline [Multimedia IR systems](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaIRSystems.md) and 
[Synology Moments](https://www.synology.com/en-global/dsm/feature/moments) <img src="./FVADocMD/Images/SynologyMoments.png" alt="SynologyMoments.png" height="20" /> as Online [Multimedia IR systems](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaIRSystems.md) are now supported. 
- **Supporting text search for [Multimedia IR systems](https://github.com/dimanikulin/dimanikulin/blob/master/content/MultimediaIRSystems.md):** It is possible to search by events, photo author, places, people using text search 
- **Configuration:** The system flow depends on configuration.
- **Automating the import:** to decrease the probability of human factor influence on the process of importing new photos into the existing photo album;
Despite many steps are automated, still human involvement is required. 

# SA Requirements
## Hardware requirements
TBD

## Programming Languages
Now C++ and Python are considered as main languages to implement FVA Software.
TBD - to describe for which cases C++ and for which Python.

## License
we use 3-rd party code here 
and own licence is TBD

## Deployment Approach
&nbsp;&nbsp;&nbsp; The target installation folder shall have next folder structure:
*  “#bin#” – a directory with all binaries (product one either third party);
*  “#data#” - a directory to keep user populated dictionaries and application data;
*  “#logs#” - a directory to keep the application logs. Each log file name shall start from name of process it was created by;
*  “#scripts#” - a directory to keep all the scripts.

&nbsp;&nbsp;&nbsp;Installation must be possible at any target directory but not only in root folder.

TBD describe dependencies installed along with FVA Software.
TBD fvaParams.csv

# Dependencies
Only open source [code/tools](./Dependencies.md) are used, as a product is **open source**.

There were other [dependencies](./DependenciesOld.md) but they got outdated.

# Contributing
So far, the development is in a hot stage, and everyone is welcome to contribute anytime.
You can fill in this [form](https://docs.google.com/forms/d/1EI3oOumRnHxDjEYgV6PFB-AMfV5plLtuV2r5S8BYn_g/edit) to understand your needs in Multimedia search and organizing.
You can also post an issue here [GitHub issues](https://github.com/dimanikulin/fva/issues) to request a feature or report a bug.
[Here](https://github.com/dimanikulin/fva/discussions) you can start a discussion.

Everyone can try themselves in different roles to learn something new: 
- [As a product manager](./FVADocMD/LEARNT_AS_PM.md);
- [As a delivery manager](./FVADocMD/LEARNT_AS_DM.md);
- [As an application architect](./FVADocMD/LEARNT_AS_SA.md); 
- [As a software developer](./FVADocMD/LEARNED_AS_DEV.md).
- [As a subject matter expert](./FVADocMD/LEARNT_AS_SME.md). 
- [As a software tester you can download installation packages](https://github.com/dimanikulin/fva/releases) that are published for Linux (x86-64), Mac and Windows (latest versions) to try it.

So, there are many items to help the author with.
  
# Stay in touch
* [Twitter](https://twitter.com/DmytroNikulin)
* [LinkedIn](https://www.linkedin.com/in/dimanikulin/)
* [Author Telegram](https://t.me/dimanikulin79) 
* [Community Telegram](https://t.me/+XqWdomi90-00Yzgy) 
