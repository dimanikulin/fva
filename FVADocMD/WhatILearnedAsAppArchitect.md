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
I began my journey as an architect with **requirements** selection. 
By selecting the right **requirements**, an architect can ensure that the software is designed to meet specific **business** goals and objectives, as well as technical and operational requirements. 
This helps ensure that the software is scalable, maintainable, and meets the needs of both the **end-users** and the **stakeholders**.
It's worth noting that my approach to selecting **requirements** differed from that of a [product manager](https://github.com/dimanikulin/dimanikulin/blob/main/WhatILearnedAsProductManager.md) because I needed more technical requirements. 
I divided architect requirements into four parts:
- **Constraints**;
- **Assumptions**;
- **Quality Attributes**;
- and **Hardware requirements**.

### Constraints
**Constraints** are factors that limit the architect's ability to design and implement a software solution.
These factors may include technical, business, or organizational constraints that impact the development process.

I have listed them on the following [page](https://github.com/dimanikulin/fva#constraints) in the form of a table with the following columns:
- **Constraint** - to describe the constraint itself;
- **Description** - to provide more details about the constraint;
- **Business value** - to indicate the business value of the constraint (High, Medium, or Low);
- **Architecture viewpoint** - to indicate the resources required to implement the constraint (High, Medium, or Low).

### Assumptions
**Assumptions** are [statements](https://github.com/dimanikulin/fva#assumptions) made by the architect about the environment, conditions, or factors that are expected to be true for the software solution to function as intended. 
These statements are made based on available information, but they **are not** necessarily proven to be true.

**Assumptions** are important to consider during requirements selection because they can impact the design and implementation of the software solution. 
By identifying assumptions early in the development process, the architect can take steps to verify the assumptions and adjust the design as needed to ensure that the solution will function as intended.

### Quality Attributes
[Quality Attributes](https://github.com/dimanikulin/dimanikulin/blob/main/QualityAttributes.md), also known as non-functional requirements, are the characteristics of a software solution that describe how well it performs in terms of its operation, maintenance, and evolution. 
**Quality attributes** are not directly related to the functionality of the software, but rather to how well it meets certain standards or requirements.

**Quality attributes** are important to consider during requirements selection because they can impact the user experience, the cost of ownership, and the long-term success of the software solution.

The **quality attributes** are provided in the form of a [table](https://github.com/dimanikulin/fva#quality-attributes) with the following columns:

- **Name**: to describe the quality attribute
- **Description**: to provide additional details about the quality attribute
- **Motivation**: to explain the rationale for selecting the quality attribute
- **Measurable Metrics**: to indicate the metrics used to measure whether the quality attribute has been achieved or not
- **Business Priorities**: to indicate the business priorities for the quality attribute
- **Architecture Priorities**: to indicate the architecture priorities for the quality attribute

### Hardware requirements
Last but no least were [hardware requirements](https://github.com/dimanikulin/fva#hardware-requirements).

**Hardware requirements** refer to the physical components, such as servers, storage devices, and networking equipment, that are necessary to support the software solution. 
These requirements are based on the needs of the software and the expected workload or usage patterns.

It is important to consider hardware requirements during the architecture design phase to ensure that the solution is designed to work within the limitations and capabilities of the hardware that will be used to support it.

## Software Architecture Views
**Software architecture views** are representations of different aspects of the software system that are designed to communicate specific information to different stakeholders. 
They provide a structured way to organize and present information about the architecture of the software system.
By using **software architecture views**, architects can ensure that all stakeholders have a clear understanding of the system and its architecture, which can help to avoid misunderstandings and ensure that the system is designed to meet the needs of all stakeholders.

I created three **Software architecture views**:
- Context view,
- Functional view,
- and Deployment view.

I used [Mermaid.js](https://mermaid-js.github.io/mermaid-live-editor/edit) as it is supported now by GitHub.

### Context view 
A **Context view** is a type of software architecture view that provides a high-level overview of the software system and its external environment. 

It shows the relationships between the software system and its users, other systems, and external entities.

The [Context](https://github.com/dimanikulin/fva#context-view) view is useful for stakeholders who need a high-level understanding of the software system and its context, such as business analysts, project managers, or system owners.

### Functional view
A **Functional view** is a type of software architecture view that focuses on the functional or behavioral aspects of the software system.
It describes the functional elements or components of the system and how they interact with each other to perform the desired tasks or functions.

The [Functional view](https://github.com/dimanikulin/fva#functional-view) is useful for software architects, developers, and testers who need to understand the functional requirements of the system and how they are implemented.

### Deployment view
A **Deployment view** is a type of software architecture view that describes how the software system is deployed or installed in the computing environment. 
It shows the physical elements of the system, such as servers, networks, and storage devices, and how they are connected and configured to support the software system.

The **Deployment view** is useful for system administrators, operations teams, and infrastructure specialists who need to understand how the software system is deployed and managed in the computing environment.

Since the FVA Tool Set can be deployed using only one node and a few processes, I provided a simple text [description](https://github.com/dimanikulin/fva#deployment-view) for the **Deployment view**.

## Tools, libraries and languages
At some point, I realized that I needed a better way to manage the various IDEs, UI and command line tools, libraries, and languages that I use.
 
### Tools and libraries 
I compiled a list of all the tools and libraries that I used for development and presented them in a table on the project's GitHub [page](https://github.com/dimanikulin/fva#dependencies).
The table includes the following columns: 
- **Name** the name of the tool/library, 
- **Purpose**: its intended purpose, 
- **Version**: the version I used, 
- **License**: the license it is distributed under, 
- **Environment**: the platform on which it can be run, 
- and **Comment**: any additional information I wanted to provide.

Initially, the list was much longer, but as some of the tools/libraries became outdated, I moved them to a separate table using the same format. 
You can find the old dependencies list [here](https://github.com/dimanikulin/fva/blob/master/DependenciesOld.md).

### Languages
I just used plain text to describe [programming languages](https://github.com/dimanikulin/fva#programming-languages) the FVA Software uses.

## Licenses 
Undeniably, we should remember about licenses. 
As FVA Software uses third-party tools and libraries, their respective [licenses](https://github.com/dimanikulin/fva/blob/master/FVACommonLib/LICENSE) should be included in the code. 
In addition, I had to create the FVA License and gained an understanding of what a license is and how to create one.
 
# References
| # | Name                 | Source                | Release date           |  Author                 | Description   |
| - | ---------------------|---------------------- |----------------------- | ----------------------- |:-------------:|
| 1 | A tool to edit diargam in mermaid-js | [Web](https://mermaid-js.github.io/mermaid-live-editor/edit) | | | |
| 2 | Solution Architecture Document | [local resource](./FVADocX/FVAToolSet-SAD.docx) | | | |
| 3 | Solution Architecture Document diagrams | [local resource](./FVADocX/FVAToolSet-SADdiagrams.drawio) | | | |

# Definitions, Acronyms, Abbreviations
| # | Abbreviation or Acronym | Definition     |
| - | ------------------------|:--------------:|
| 1 | [IDE](https://en.wikipedia.org/wiki/Integrated_development_environment)| An integrated development environment (IDE) is a [software application](https://en.wikipedia.org/wiki/Application_software) that provides comprehensive facilities to [computer programmers](https://en.wikipedia.org/wiki/Computer_programmer) for [software development](https://en.wikipedia.org/wiki/Software_development). An IDE normally consists of at least a [source code editor](https://en.wikipedia.org/wiki/Source_code_editor), build [automation tools](https://en.wikipedia.org/wiki/Build_automation) and a [debugger](https://en.wikipedia.org/wiki/Debugger).|
| 2 | [Quality Attribute](https://www.codementor.io/@antoniopfesilva/how-to-write-meaningful-quality-attributes-for-software-development-ez8y90wyo)|is a measurable or testable property of a system that is used to indicate how well the system satisfies the needs of its stakeholders |
