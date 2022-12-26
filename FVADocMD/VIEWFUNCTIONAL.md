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
## Input Multimedia Data
TBD

## Output Multimedia Data
TBD

## FVA Software configuration
TBD
The formatting strings shall be kept in a separated category

## FVA Software internal metadata
&nbsp;&nbsp;&nbsp; It is used to enrich Output Multimedia Data. </br> 
&nbsp;&nbsp;&nbsp; Let describe how it was before current implementation. 
Firstly it was kept at file system level inside of the Photo Album. 
Each folder in the Photo Album could keep two files: “folderDescription.json” and “description.csv”. 
“FolderDescription.json” kept information about all the files under a folder that was common. 
For example it could be device id that was the same for all multimedia files.   “FolderDescription.json” structure was:
</br>
```json
{
"deviceId":"",
"tags":"",
"people": "",
"place":"",
"event":"" 
}
```

“folderDescription.json” example:
```json
{
"deviceId":"3",
"tags":"At home, with a family",
"people": "3,6,8",
"place":"3,4",
"event":"45"
}
```

&nbsp;&nbsp;&nbsp; Description.csv has been used to keep information about files under a folder for cases if some multimedia files had different internal metadata. 
“description.csv” structure was:
</br>
&nbsp;&nbsp;&nbsp; Name,Place,People,Device,WhoTook,Description,Scaner
</br>
&nbsp;&nbsp;&nbsp; So the FVA software created or updated these files during import new files to photo album. </br> 
&nbsp;&nbsp;&nbsp; Keeping the internal metadata in this approach did not give good flexibility and maintainability. 
So adding one column in “folderDescription.json” or “description.csv” could cause whole photo album file system structure to update.
</br> 
&nbsp;&nbsp;&nbsp; So it was decided to move all “folderDescription.json” and “description.csv” to SQLlite database. 
The scheme has been created to keep the same information as “folderDescription.json” and “description.csv” did keep.
The FVA software created SQL updates to DB during import new files to photo album and all those SQL updates were kept to create SQL at any time.
</br>
&nbsp;&nbsp;&nbsp; Again keeping the internal metadata in this approach did not give good flexibility and maintainability. 
So merging one folder in photo album to another caused significant change of SQL updates.
</br>
&nbsp;&nbsp;&nbsp; So it was decided to move all information in one CSV file.
That internal metadata file does not keep information in which folder a file is kept. 
So merging one folder in photo album to another does not cause an issue.
Still the duplication of information takes place because for all files in one folder common information is just copied.
</br>

## FVA Software dictionary 
FVA Software dictionary is data used along with FVA Software internal metadata.
 