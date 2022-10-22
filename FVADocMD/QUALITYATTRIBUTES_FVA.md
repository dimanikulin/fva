# Quality attributes
&nbsp;&nbsp;&nbsp; This section lists quality attributes for target architecture.
For each quality attribute it provides name, description, motivation (rationale for selecting), measurable metrics and priorities from business and architecture points of view.
The metrics show whether a particular quality attribute is achieved or not.
The components where metric is measured is noted as well.

- [Conceptual Integrity](#conceptualintegrity)
- [Maintainability](#maintainability)
- [Reusability](#reusability)
- [Availability](#availability)
- [Interoperability](#interoperability)
- [Manageability](#manageability)
- [Performance](#performance)
- [Reliability](#reliability)
- [Scalability](#scalability)
- [Security](#security)
- [Portability](#portability)
- [Supportability](#supportability)
- [Testability](#testability)
- [Learnability](#learnability)
- [Traceability](#traceability)

# Conceptual Integrity
The software shall be decomposed into well-structured modules according to functional decomposition. 

## Motivation for selecting
TBD

## Metrics
The code review process shall be established to make sure the modules are structured according to functional decomposition

## Business value - Low
## Architecture viewpoint - Middle

# Maintainability
The system shall be able to undergo changes with a high level of easiness.

## Motivation for selecting
The following changes might happen frequently:
- changes applied to the [UI](https://en.wikipedia.org/wiki/User_interface) like adding new screen;
- adding new multimedia formats to support;
- new [metadata](https://en.wikipedia.org/wiki/Metadata) search criteria to handle.
- new offline or online IR Multimedia Systems can be easily integrated

## Metrics
The code review process shall be defined to make sure the [OOP](https://en.wikipedia.org/wiki/Object-oriented_programming), [SOLID](https://en.wikipedia.org/wiki/SOLID) principles and [MVC](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller) pattern are going to be used.

## Business value - Middle
## Architecture viewpoint - High
                                              
# Reusability
The system components shall be usable in other functionality or other flows.

## Motivation for selecting
It is applied for deduplication of implementation and to reduce the implementation time spent).

## Metrics
A common library is going to be created to keep the following functions used by several components:
- To work with configuration;
- Logging functions;
- Common [UI](https://en.wikipedia.org/wiki/User_interface) functions;

## Business value - Middle
## Architecture viewpoint - High

# Availability
TBD

## Motivation for selecting
TBD

## Metrics
TBD

## Business value - TBD
## Architecture viewpoint - TBD

# Interoperability
The proposed solution shall have the ability to be integrated with online and offline Multimedia IR systems in the common way.

## Motivation for selecting
The system shall be able to be integrated to any IR Multimedia Systems with a high level of easiness.

## Metrics
The code review process shall be defined to make sure the [OOP](https://en.wikipedia.org/wiki/Object-oriented_programming), [SOLID](https://en.wikipedia.org/wiki/SOLID) principles and [MVC](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller) pattern are going to be used.

## Business value - High
## Architecture viewpoint  - High

# Manageability
The system shall be monitorable via [UI](https://en.wikipedia.org/wiki/User_interface) and logging. The system shall support debugging as well.

## Motivation for selecting
It is applied to easily support the systems operations in the fields.

## Metrics
The code review process shall be established to make sure the  Each component shall use [UI](https://en.wikipedia.org/wiki/User_interface) and logging component:
- to save any important events - to disk via logging;
- to show the progress of processing in [UI](https://en.wikipedia.org/wiki/User_interface);
- to show any important events - to user in [UI](https://en.wikipedia.org/wiki/User_interface);
- to provide the user with an ability to cancel the processing in [UI](https://en.wikipedia.org/wiki/User_interface).

## Business value - Middle
## Architecture viewpoint - High

# Performance
The same as for the whole system, the [UI](https://en.wikipedia.org/wiki/User_interface) shall respond in less than 1 second. 
As there might be time-consuming jobs, it shall show the current progress for such operations to provide the user with a status.

## Motivation for selecting
It is applied to support great UX.

## Metrics
The regular performance test shall be performed to check it.

## Business value - High
## Architecture viewpoint - Low

# Reliability
If the system is going to fail during an operation, it shall report the operation status to user with error description and roll back all applied changes for this operation.

## Motivation for selecting
TBD

## Metrics
TBD

## Business value - High
## Architecture viewpoint - High

# Scalability
TBD

## Motivation for selecting
TBD

## Metrics
TBD

## Business value - TBD
## Architecture viewpoint - TBD

# Security
Each multimedia file shall have “read only” attribute set up after processing on Windows.

## Motivation for selecting
It is applied not to allow it easily change multimedia after processing.

## Metrics
Functional test shall be created to check if all multimedia files after processing are marked with “read only” flag.

## Business value - Low
## Architecture viewpoint - Low

# Portability
The software shall support all wide known desktop/laptop [operating systems](https://en.wikipedia.org/wiki/Operating_system) with the latest update. 
No support of the web or mobile platform required.

## Motivation for selecting
Those platforms are selected to cover about 100% of desktop user.

## Metrics
TBD

## Business value - High
## Architecture viewpoint - Middle

# Supportability
During abnormal function, the software shall provide the information to find and to solve the issues via prompting user in [UI](https://en.wikipedia.org/wiki/User_interface) and saving detailed information into the log file. Also it has 
- to show the progress of processing in [UI](https://en.wikipedia.org/wiki/User_interface);
- to show any important events to user in [UI](https://en.wikipedia.org/wiki/User_interface);
- to provide the user with an ability to cancel the processing in [UI](https://en.wikipedia.org/wiki/User_interface). 

The user shall be able to change a log level as configurable parameter.

## Motivation for selecting
It is applied to easily support the systems operations in the fields and for monitoring. 
TBD - Add a description for Administration tools and specific deployment aspects

## Metrics
TBD

## Business value - High
## Architecture viewpoint - Middle

# Testability
TBD

## Motivation for selecting
TBD

## Metrics
TBD

## Business value - TBD
## Architecture viewpoint - TBD

# Usability
The system shall be intuitive, easy to localize (to support multiple time zones, languages, etc.) and globalize. 
In addition, the installation package must be provided for each supportable platform to increase the usability of product.

## Motivation for selecting
TBD

## Metrics
TBD

## Business value - High 
## Architecture viewpoint - Middle

# Learnability
The system shall have the [UI](https://en.wikipedia.org/wiki/User_interface) allowing users to quickly become familiar with it and be able to make good use of all their features and capabilities.
The user guideline and working instruction shall be available.

## Motivation for selecting
It is required to help the user to get familiar with the system.

## Metrics
TBD

## Business value - High
## Architecture viewpoint  - High

# Traceability
The system code shall be traceable to the requirements.

## Motivation for selecting
To make possible to measure the product readiness.

## Metrics
TBD

## Business value - Low
## Architecture viewpoint - Low