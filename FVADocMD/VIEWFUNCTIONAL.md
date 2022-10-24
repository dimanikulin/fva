&nbsp;&nbsp;&nbsp; The Layered Architecture shall be applied to the system. 
Following layers are considered:

```mermaid
flowchart BT
    A(UI) --> B(Processing)
    B(Processing) --> C(Data and Configuration)
```
</br> TBD - describe why layered arch </br> 

# ([UI](https://en.wikipedia.org/wiki/User_interface))layer
 
The photo album owner interacts with the system through the user interface ([UI](https://en.wikipedia.org/wiki/User_interface)) layer components. 

```mermaid
graph TD
    A(UI) --> B(Configurator)
    A(UI) --> C(Description Editor)
    A(UI) --> D(Dictionary Editor)
    A(UI) --> E(Player)
    A(UI) --> F(Viewer)
    A(UI) --> G(Organizer Wizard)  
```

Basing on flow and user needs, [UI](https://en.wikipedia.org/wiki/User_interface) interacts with other components described later.

# Processing layer
```mermaid
graph TD
    A(Processing) --> B(Data Processor)
    A(Processing) --> C(Flow Controller)
    A(Processing) --> D(Organizer)
```
# Data layer



##################################################################
&nbsp;&nbsp;&nbsp; Below is a diagram to show :
- functional decomposition of the system; 
- the connections between the components; 
- the connections between the components and photo album owner;
- the connections between the components and external systems.
</br>
 