# Layers
&nbsp;&nbsp;&nbsp; The Layered Architecture shall be applied to the system. 
Following layers are considered:

```mermaid
flowchart BT
    A(UI) --> B(Processing)
    B(Processing) --> C(Data and Configuration)
```
</br> TBD - describe why layered arch </br> 
All the main components interact with the Logging component to log the important events during processing.

## Logging 
&nbsp;&nbsp;&nbsp; During logging the logging component uses a "log level" setting got from FVAConfigurator.
The log shall be present as a CSV file with name of component it was created by.
The format of log file shall be following:
TBD
 
# ([UI](https://en.wikipedia.org/wiki/User_interface))layer 
The photo album owner interacts with the system through the user interface ([UI](https://en.wikipedia.org/wiki/User_interface)) layer components. 
Basing on flow and user needs, [UI](https://en.wikipedia.org/wiki/User_interface) interacts with other components described later.

```mermaid
graph TD
    A(UI) --> B(Configurator)
    A(UI) --> C(Description Editor)
    A(UI) --> D(Dictionary Editor)
    A(UI) --> E(Player)
    A(UI) --> F(Viewer)
    A(UI) --> G(Organizer Wizard)  
```
## Configurator
The main function of the Configurator is to provide the settings to other components when requested and to save/load them to/from data layer.
Configurator works directly with saved in the disk settings. 
Instead, it uses FvaConfiguration.

## Description Editor
TBD 

## Dictionary Editor
TBD 

## Player
TBD 

## Viewer
TBD 

## Organizer Wizard
TBD 

# Processing layer
```mermaid
graph TD
    A(Processing) --> B(Data Processor)
    A(Processing) --> C(Flow Controller)
    A(Processing) --> D(Organizer)
```
&nbsp;&nbsp;&nbsp; The purpose of Flow Controller is to manage the action flows between [UI](https://en.wikipedia.org/wiki/User_interface) and the Data processor. The flows can be configured by the Configurator component.
</br>
&nbsp;&nbsp;&nbsp; Data Processor performs such actions as checking the multimedia [Metadata](https://en.wikipedia.org/wiki/Metadata) for presence and consistency and enriching it.
</br>
&nbsp;&nbsp;&nbsp; Organizer makes possible to start any command in separate process from command line or in separate thread.
</br>

# Data layer
```mermaid
graph TD
    A(Data) --> B(Input Multimedia Data)
    A(Data) --> C(Output Multimedia Data)
    A(Data) --> D(FVA Software configuration)
    A(Data) --> E(FVA Software internal metadata)
    A(Data) --> E(FVA Software dictionary)
```
Basing on requirements FVA Software operates with five levels of data:
- Input Multimedia Data;
- Output Multimedia Data;
- FVA Software configuration;
- FVA Software internal metadata;
- FVA Software dictionary data used along with FVA Software internal metadata;

 