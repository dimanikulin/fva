# Headline
What I learned as an Application Architect while creating my product

Creating Effective Documentation for Small Applications: Lessons Learned from a Solution Architect

# Article description
In this article, we explore the importance of solution architecture in small applications, and why having an architect involved can make a significant difference. 
The author shares their experience in creating a solution architecture document, and how they decided to keep all notes and documents in the form of text files in MD format, providing insights on how to create attractive README.md files with a use of badges from shields, providing document structure and required chapters. 
The article also covers topics such as defining requirements, managing development tools and libraries, and creating effective documentation for small applications. 

# Tags
Solution Architecture, Development Tools, Quality Attributes, Requirements, Branch Strategy, Licenses, Text-Based Documentation, Mermaid.js

# Content
- "*Are you kidding?*"  you might ask. "*Do such small applications really require an architect to be involved?*"
- "*Yes, yes, and yes!*" I would answer.

Generally, architects create a huge document known as a **Solution Architecture Document** that includes everything. 
Initially, I followed the same approach, but it turned out to be quite difficult to track changes due to the high frequency of modifications. 
So, I decided to keep every note and document in the form of text files in **MD format**, like this one.
I also learned how to create attractive **README.md** files with the use of **badges** from shields, providing document structure and required chapters.

## Requirements 
I started my learning as an architect from requirements selection. 
It is worth noting here, that my requirements selection approach was not the same as it was for [Product Manager](https://github.com/dimanikulin/dimanikulin/blob/main/WhatILearnedAsProductManager.md), because I needed more technical ones.
I divided architect requirements into 3 parts:
- **Constraints**;
- **Assumptions**;
- and **Quality Attributes**.

### Constraints
Constraints are a limitation for a product from techical perspective.
I listed [them](https://github.com/dimanikulin/fva#constraints) in the form of a table with the following columns:
- **Constraint** - to describe a constraint itself;
- **Description** - to add more details about constraint;
- **Business value** - to say it has High, Medium or Low value for business;
- **Architecture viewpoint** - to say if it requires High, Medium or Low resources to implement.

### Assumptions
Then there were [assumptions](https://github.com/dimanikulin/fva#assumptions) with explanations to them. 

### Quality Attributes
On top of that, it was very important to define [quality attributes](https://github.com/dimanikulin/fva#quality-attributes).

It is provided in the form of a table with the following columns:
- **name**; 
- **description**;
- **motivation** to tell rationale for selecting;
- **measurable metrics** to show whether a particular quality attribute is achieved or not;
- priorities from **business** points of view;
- and priorities from **architecture** points of view.

But before filling in that table I needed to understand what [Quality Attributes](https://github.com/dimanikulin/dimanikulin/blob/main/QualityAttributes.md) generally are.

### Hardware requirements
Last but no least were [hardware requirements](https://github.com/dimanikulin/fva#hardware-requirements).

## Software Architecture

### Context view 
The first architecture view was a context diagram.
Generally, [context diagram](https://github.com/dimanikulin/fva#context-view) shows the solution bounds and its interactions with other objects and subjects.
I used Mermaid.js to create context view diagram.
Mermaid.js is supported now by GitHub.

### Functional view
The next architecture view was a functional diagram.
Generally, [functional view](https://github.com/dimanikulin/fva#functional-view) shows solution components divided by functional purpose and interactions between them.
and I used Mermaid.js to create that view diagram again.

### Deployment view
The next architecture view had to be a deployment one.
As a rule, deployment view shows nodes used to deploy solution components/processes.
Because FVA Tool Set requires only one node and a few processes to deploy I decided to put it in a simple text [description](https://github.com/dimanikulin/fva#deployment-view).

## Tools, libraries and languages
At some point I realized I need to manage the tools, libraries and languages I use.
 
### Tools and libraries 
All the tools and libraries I used for development I put in the form of a [table](https://github.com/dimanikulin/fva#dependencies) with the following columns:
- **Name** to designate name;
- **Purpose** to set up a purpose of a tool/library;
- **Version** to name the version;
- **License** to say what license is a tool/library under;
- **Environment** to say what environment we can run that tool/library on;
- **Comment** just to add additional comment. 

Initially, it was a much longer list of tools/libraries, but they got outdated and were moved into a separate [table](https://github.com/dimanikulin/fva/blob/master/DependenciesOld.md) with the same format.

### Languages
I just used plain text to describe [programmimng languages](https://github.com/dimanikulin/fva#programming-languages) the FVA Software uses.

## Licenses 
Undeniably, we should remember about licenses.
As FVA Software uses third party, it shall contain its [license](https://github.com/dimanikulin/fva/blob/master/FVACommonLib/LICENSE) in the code.
And I had to declare FVA License, so I gained an understanding of what license is and how to create it.
 
# References
| # | Name                 | Source                | Release date           |  Author                 | Description   |
| - | ---------------------|---------------------- |----------------------- | ----------------------- |:-------------:|
| 1 | A tool to edit diargam in mermaid-js | [Web](https://mermaid-js.github.io/mermaid-live-editor/edit) | | | |
| 2 | Solution Architecture Document | [local resource](./FVADocX/FVAToolSet-SAD.docx) | | | |
| 3 | Solution Architecture Document diagrams | [local resource](./FVADocX/FVAToolSet-SADdiagrams.drawio) | | | |

# Definitions, Acronyms, Abbreviations
| # | Abbreviation or Acronym | Definition     |
| - | ------------------------|:--------------:|
| 1 | [Quality Attribute](https://www.codementor.io/@antoniopfesilva/how-to-write-meaningful-quality-attributes-for-software-development-ez8y90wyo)|is a measurable or testable property of a system that is used to indicate how well the system satisfies the needs of its stakeholders |
