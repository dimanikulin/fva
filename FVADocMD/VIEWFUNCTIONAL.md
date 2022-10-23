&nbsp;&nbsp;&nbsp; The Layered Architecture shall be applied to the system. </br>
Following layers are considered:
- Data and Configuration layer;
- Processing layer;
- [UI](https://en.wikipedia.org/wiki/User_interface) layer; 

```mermaid
flowchart BT
    A(Data and Configuration) --> B(Processing)
    B(Processing) --> C(UI)
```

TBD – describe why layered arch </br> 
&nbsp;&nbsp;&nbsp; Below is a diagram to show :
- functional decomposition of the system; 
- the connections between the components; 
- the connections between the components and photo album owner;
- the connections between the components and external systems.
</br>
TODO - make a picture


The photo album owner interacts with the system through the user interface ([UI](https://en.wikipedia.org/wiki/User_interface)) components. 
Basing on flow and user needs, [UI](https://en.wikipedia.org/wiki/User_interface) interacts with other main described below. 

There are following functional components of the system:
- Configurator;
- Description Editor;
- Dictionary Editor;
- Player;
- Viewer;
- Data Processor;
- Flow Controller;
- Organizer;
- OrganizerWizard;
- Data.

