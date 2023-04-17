# Headline
TBD
What I learned as an Application Architect while creating my product

# Article description
TBD 

# Tags
TBD

# Content
- "Are you kidding?", - you will ask me. - "Do such small applications require an arhitect to be involved?"
- "Yes, yes, and yes!", - my answer will be.

&nbsp;&nbsp;&nbsp; Generally, Architects create a huge document known as Solution Architecture Document [SAD](../FVADocX/FVAToolSet-SAD.docx) plus [diagrams](../FVADocX/FVAToolSet-SAD diagrams.drawio) that keeps everything inside.
Initially, I followed the same way, but it turned out to be quite hard to track changes due to high frequency of modifications.
So, I decided to keep every note, every doc just in the form of text files in MD format like this one ))).
And I learned how to create attractive README.md files with a use of badges from shields, providing document structure and required chapters.

## Requirements 
&nbsp;&nbsp;&nbsp; I started my learning as an architect from requirements selection. It is worth noting here, that my requirements selection approach was not the same as it was for [Product Manager](./LEARNT_AS_PM.md), because I needed more technical ones.
I divided architect requirements into 3 parts:
- **Constraints**;
- **Assumptions**;
- and **Quality Attributes**.

### Constraints
&nbsp;&nbsp;&nbsp; Constraints are a limitation for a product from techical perspective.
I listed [them](./REQUIREMENTS_SA.md#constraints) in the form of a table with the following columns:
- **Constraint** - to describe a constraint itself;
- **Description** - to add more details about constraint;
- **Business value** - to say it has High, Medium or Low value for business;
- **Architecture viewpoint** - to say if it requires High, Medium or Low resources to implement.

### Assumptions
&nbsp;&nbsp;&nbsp; Then there were [assumptions](./REQUIREMENTS_SA.md#assumptions) with explanations to them. 

### Quality Attributes
&nbsp;&nbsp;&nbsp; On top of that, it was very important to define a solution for [quality attributes](./QUALITYATTRIBUTES_FVA.md).

It is provided in the form of a table with the following columns:
- **name**; 
- **description**;
- **motivation** to tell rationale for selecting;
- **measurable metrics** to show whether a particular quality attribute is achieved or not;
- priorities from **business** points of view;
- and priorities from **architecture** points of view.

But before filling in that table I needed to understand what [Quality Attributes](./QUALITYATTRIBUTES.md) generally are.

### Hardware requirements
&nbsp;&nbsp;&nbsp; Last but no least were [hardware requirements](./REQUIREMENTS_HW.md).

## Software Architecture

### Context view 
&nbsp;&nbsp;&nbsp; The first architecture view was a context diagram.
Generally, [context diagram](./VIEWCONTEXT.md) shows the solution bounds and its interactions with other objects and subjects.
I used Mermaid.js to create context view diagram.
Mermaid.js is supported now by GitHub.

### Functional view
&nbsp;&nbsp;&nbsp; The next architecture view was a functional diagram.
Generally, [functional view](./VIEWFUNCTIONAL.md) shows solution components divided by functional purpose and interactions between them.
and I used Mermaid.js to create that view diagram again.

### Deployment view
&nbsp;&nbsp;&nbsp; The next architecture view had to be a deployment one.
As a rule, deployment view shows nodes used to deploy solution components/processes.
Because FVA Tool Set requires only one node and a few processes to deploy I decided to put it in a simple text [description](./DEPLOYMENTAPPROACH.md).

## Tools, libraries and languages
&nbsp;&nbsp;&nbsp; At some point I realized I need to manage the tools, libraries and languages I use.
 
### Tools and libraries 
&nbsp;&nbsp;&nbsp; All the tools and libraries I used for development I put in the form of a [table](./DEPENDENCIES.md) with the following columns:
- **Name** to designate name;
- **Purpose** to set up a purpose of a tool/library;
- **Version** to name the version;
- **License** to say what license is a tool/library under;
- **Environment** to say what environment we can run that tool/library on;
- **Comment** just to add additional comment. 

&nbsp;&nbsp;&nbsp; Initially, it was a much longer list of tools/libraries, but they got outdated and were moved into a separate [table](./DEPENDENCIES_OLD.md) with the same format.

### Languages
&nbsp;&nbsp;&nbsp; I just used plain text to describe [programmimng languages] the FVA Software uses.

## Branch strategy
&nbsp;&nbsp;&nbsp; One more important point was a definition of [branch strategy](./BUILD_RELEASE.md#branchstrategy) 

## Licenses 
&nbsp;&nbsp;&nbsp; Undeniably, we should remember about licenses.
As FVA Software uses third party, it shall contain its license in the [code](../FVACommonLib/LICENSE).
And I had to declare [FVA License](./LICENSE.md), so I gained an understanding of what license is and how to create it.
 
# References
| # | Name                 | Source                | Release date           |  Author                 | Description   |
| - | ---------------------|---------------------- |----------------------- | ----------------------- |:-------------:|
| 1 | A tool to edit diargam in mermaid-js | [Web](https://mermaid-js.github.io/mermaid-live-editor/edit) | | | |

# Definitions, Acronyms, Abbreviations
| # | Abbreviation or Acronym | Definition     |
| - | ------------------------|:--------------:|
| 1 | [Quality Attribute](https://www.codementor.io/@antoniopfesilva/how-to-write-meaningful-quality-attributes-for-software-development-ez8y90wyo)|is a measurable or testable property of a system that is used to indicate how well the system satisfies the needs of its stakeholders |
