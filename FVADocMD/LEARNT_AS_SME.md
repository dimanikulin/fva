# What I learned as a Subject Matter Expert while creating my own product
## Multimedia IR systems
&nbsp;&nbsp; My first step in the role of an SME was getting to know what [Multimedia Data](./MULTIMEDIADATA.md) is.
I started learning it as I understood that my whole photo album is [Multimedia Data](./MULTIMEDIADATA.md) itself.
Then I realised that I need to use a solution to search in [Multimedia Data](./MULTIMEDIADATA.md) - [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md). 
However, there are many [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md), and it was a long way to select one of them to start searching in a more efficient way.
Thus, several [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md), that are present on the market and available for end users, were analyzed. 

I described the process of analysis and comparison for [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) [here](./MULTIMEDIAIRSYSTEMSANALYZE.md).
Consequently, I was able to choose the most suitable system for me.

## Metadata
&nbsp;&nbsp;&nbsp; A short usage of these systems gave me an understanding that [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) significantly rely on the [metadata](https://en.wikipedia.org/wiki/Metadata) as a main information source that is used for a search following different criteria, like device, time, location, etc.
Therefore, a presence and consistency of [metadata](https://en.wikipedia.org/wiki/Metadata) is very important for [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) to operate, as well as for other purposes [[21]](./REFERENCES.md).
Working experience with one of those systems shows that search results might be incomplete or even incorrect due to inconsistent or not-existing [metadata](https://en.wikipedia.org/wiki/Metadata).
[Metadata](https://en.wikipedia.org/wiki/Metadata) is usually kept inside of each file as a header with predefined format.
There are a plenty of [metadata](https://en.wikipedia.org/wiki/Metadata) formats like [Exif](https://en.wikipedia.org/wiki/Exif), [IPTC](https://en.wikipedia.org/wiki/International_Press_Telecommunications_Council) or [XMP](https://en.wikipedia.org/wiki/Extensible_Metadata_Platform).
</br>

&nbsp;&nbsp;&nbsp; The reasons for incorrect and absent [metadata](https://en.wikipedia.org/wiki/Metadata) are quite different - from not setting proper data on the device that takes photos to privacy issues. 
This resource [[22]](./REFERENCES.md) describes [metadata](https://en.wikipedia.org/wiki/Metadata) importance in more detail and there is information about other reasons for incorrect and absent [metadata](https://en.wikipedia.org/wiki/Metadata). 

## Examples of incorrect search results 
&nbsp;&nbsp;&nbsp; Let's consider two examples of incorrect search results. 
In the first case we tried to search for photos with existing **Geo** positions and to see them on a map. 
A search returned four photos taken near **Ghana**, marked by red circle.
<img src="./Images/AnalyzeofprivateMultimediaIRsystems.png" alt="AnalyzeofprivateMultimediaIRsystems.png" />
&nbsp;&nbsp;&nbsp; In fact, nobody took photos near **Ghana**.

&nbsp;&nbsp;&nbsp; In the second case we searched for photos taken in **2011**. 
Literally, we got one video that was recognized as **March 2020**, underlined in red.
<img src="./Images/AnalyzeofprivateMultimediaIRsystems2.png" alt="AnalyzeofprivateMultimediaIRsystems2.png" />
&nbsp;&nbsp;&nbsp; The system has provided such results due to incorrect [metadata](https://en.wikipedia.org/wiki/Metadata). 
</br>

## Multimedia Organizers
&nbsp;&nbsp;&nbsp; That being the case, I understood that [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) are not so effective without other tools, often called [Multimedia Organizers](./MULTIMEDIAORGANIZERS.md).
Usually, [Multimedia Organizers](./MULTIMEDIAORGANIZERS.md) provide automation for importing a new photo set into existing photo albums and it is not an implemented feature in [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md).
In addition, they might identify and address some issues that shall be fixed before a user starts searching data using the [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md).
</br> </br> 

## Integration 
&nbsp;&nbsp;&nbsp; Then I started studying [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) the **FVA Tool Set** shall integrate with. 
Integration with offline [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) is implemented through integration with [digiKam](https://www.digikam.org/about/), and integration with online [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) is implemented through integration with [Google Photos](https://en.wikipedia.org/wiki/Google_Photos).
As [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) performs a search by certain criteria, based on multimedia [metadata](https://en.wikipedia.org/wiki/Metadata), the FVA Software shall be able to work with the same multimedia [metadata](https://en.wikipedia.org/wiki/Metadata) to be integrated with [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md).
<img src="Images/Integration.png" alt="Integration.png" />

### Integration with digiKam
&nbsp;&nbsp;&nbsp; For [digiKam](https://www.digikam.org/about/), I found that **Collections** setting shall be used to define how it is configured to search through [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008). 
**Monitor the albums for external changes (requires restart)** setting shall be enabled. 
Taking into account this setting, [digiKam](https://www.digikam.org/about/) will pick up external changes in the collection as soon as proposed solution puts it in.

<img src="Images/IntegrationDigiKam.png" alt="IntegrationDigiKam.png" />

&nbsp;&nbsp;&nbsp; However, from **FVA Tool Set** side it is required to provide a possibility to configure storage in a way to be able to put [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) processed and to be integrated with [digiKam](https://www.digikam.org/about/).
Therefore, a separate [UI](https://en.wikipedia.org/wiki/User_interface) screen shall be created to ask a user what storage location, [digiKam](https://www.digikam.org/about/) should be used for incoming [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008).

&nbsp;&nbsp;&nbsp; For example, a directory **C:\FVA** is selected for **Local Collections** as shown on the picture above. 
Therefore, when a user wants to integrate [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) after processing by FVA Tool Set, he/she shall be able to select **C:\FVA** directory as a directory in [UI](https://en.wikipedia.org/wiki/User_interface) to put the processed [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) in.
As soon as it is done, **FVA Tool Set** will put [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) into a selected directory and Multimedia IR system will be able to search through it.

### Integration with Google Photos
&nbsp;&nbsp;&nbsp; Integration with [Google Photos](https://en.wikipedia.org/wiki/Google_Photos) is implemented in the same way. 
First of all, it's worth noting that [Google Photos](https://en.wikipedia.org/wiki/Google_Photos) shall be configured to be integrated with **FVA Tool Set**. 
Thus, appropriate file types and folders shall be set up as in the figure below:

<img src="Images/IntegrationGooglePhoto.png" alt="IntegrationGooglePhoto.png" />

&nbsp;&nbsp;&nbsp; For example, **Pictures** and **fromDevices** directories are selected for the folders settings as shown above. 
It means that when a user wants to integrate [multimedia data]((https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008)) after processing by **FVA Tool Set**, he/she shall be able to select **fromDevices** or **Pictures** directory in [UI](https://en.wikipedia.org/wiki/User_interface) screen, as a directory to put processed [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) in. 
As soon as it is done, **FVA Tool Set** will put [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) into a selected directory and [Google Photos](https://en.wikipedia.org/wiki/Google_Photos) will be able to search through it as soon as it synchronizes [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008).
