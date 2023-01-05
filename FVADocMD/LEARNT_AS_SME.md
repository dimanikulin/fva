# What I learnt as Subject Matter Expert
## Multimedia IR systems
&nbsp;&nbsp; My first step as SME was knowledge on what is [Multimedia Data](./MULTIMEDIADATA.md).
I started learning it as I undestood whole my photo album is [Multimedia Data](./MULTIMEDIADATA.md).
Then I realised that I need to use a solution to search in [Multimedia Data](./MULTIMEDIADATA.md) - [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md). 
However, there are many [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) and it was a long way to choose one of them to start searching in a way that is more efficient.
So, several [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) were analyzed that are present on the market and available for end users. 

I described the process of analysis and comparison for [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) [here](./MULTIMEDIAIRSYSTEMSANALYZE.md).
So I have made to choose the most suitable system for me.

## Metadata
&nbsp;&nbsp;&nbsp; A short usage of these systems gave me understanding that [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) significantly rely on the [metadata](https://en.wikipedia.org/wiki/Metadata) as a main information source that is used for the search by different critiries like device, time, location, etc.
Therefore, the presence and consistency of [metadata](https://en.wikipedia.org/wiki/Metadata) is very important for [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) to work and for other purposes [[21]](./REFERENCES.md).
The working experience with one of those systems shows that the search results might not be full or even incorrect due to inconsistent or not-existing [metadata](https://en.wikipedia.org/wiki/Metadata).
[Metadata](https://en.wikipedia.org/wiki/Metadata) is usually kept inside of each file as a header with predefined format.
There are a plenty of [metadata](https://en.wikipedia.org/wiki/Metadata) formats like [Exif](https://en.wikipedia.org/wiki/Exif), [IPTC](https://en.wikipedia.org/wiki/International_Press_Telecommunications_Council) or [XMP](https://en.wikipedia.org/wiki/Extensible_Metadata_Platform).
</br>

&nbsp;&nbsp;&nbsp; The reasons for incorrect and absent [metadata](https://en.wikipedia.org/wiki/Metadata) are rather different - from not setting proper data on the device that takes the photos to the privacy issues. 
This resource [[22]](./REFERENCES.md) describes the [metadata](https://en.wikipedia.org/wiki/Metadata) importance in more detail and there is information about other reasons for incorrect and absent **metadata**. 

## Examples of incorrect search results 
&nbsp;&nbsp;&nbsp; Let us provide two examples of incorrect search results. 
In the first case, we tried to search for the photos with the existing **Geo** positions and to see them on a map. 
The search returned four photos taken near Ghana, marked by the red circle.
<img src="./Images/AnalyzeofprivateMultimediaIRsystems.png" alt="AnalyzeofprivateMultimediaIRsystems.png" />
&nbsp;&nbsp;&nbsp; In fact, nobody has taken photos near Ghana.
</br>

&nbsp;&nbsp;&nbsp; In the second case, we searched for the photos taken in 2011. 
Actually, we got one video that was recognized as of March 2020, which is underlined with the red color.
<img src="./Images/AnalyzeofprivateMultimediaIRsystems2.png" alt="AnalyzeofprivateMultimediaIRsystems2.png" />
&nbsp;&nbsp;&nbsp; The system has provided such results due to incorrect **metadata**. 
</br> </br>

## Multimedia Organizers
&nbsp;&nbsp;&nbsp; Therefore, I understood that [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) are not so effective without other tools, often called [Multimedia Organizers](./MULTIMEDIAORGANIZERS.md).
Usually, [Multimedia Organizers](./MULTIMEDIAORGANIZERS.md) provide automation for importing the new photo set into existing photo albums and it is not implemented feature in [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md).
In addition, they might identify and address some issues that shall be fixed before the user starts searching the data using the [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md).
</br> </br> 

## Integration 
&nbsp;&nbsp;&nbsp; The integration with offline Multimedia IR system is implemented through the integration with [digiKam](https://www.digikam.org/about/) and integration with online Multimedia IR system is implemented through the integration with [Google Photos](https://en.wikipedia.org/wiki/Google_Photos).
Because the [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md) performs the search by certain criteria basing on multimedia [metadata](https://en.wikipedia.org/wiki/Metadata), the FVA Software shall be working with the same multimedia [metadata](https://en.wikipedia.org/wiki/Metadata) to be integrated with Multimedia IR system.
<img src="Images/Integration.png" alt="Integration.png" />

### Integration with digiKam
&nbsp;&nbsp;&nbsp; For [digiKam](https://www.digikam.org/about/), the "Collections" setting shall be used to define how it is configured to search the [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) through. 
The setting "Monitor the albums for external changes (requires restart)" shall be enabled. 
Taking into account this setting, the [digiKam](https://www.digikam.org/about/) will pick up the external changes in the collection as soon as the proposed solution will put it in.

<img src="Images/IntegrationDigiKam.png" alt="IntegrationDigiKam.png" />

&nbsp;&nbsp;&nbsp; Still, from the side of FVA Software it is required to provide a possibility to configure the storage to put the [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) processed to be integrated with [digiKam](https://www.digikam.org/about/).
Therefore, the separate [UI](https://en.wikipedia.org/wiki/User_interface) screen is created to ask the user what storage location, [digiKam](https://www.digikam.org/about/) should use for the incoming [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008).

&nbsp;&nbsp;&nbsp; For example, a directory "C:\FVA" is selected for "Local Collections" as shown on a picture above. 
Therefore, when the user wants to integrate the [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) after processing by FVA Software, the user shall be able to select the "C:\FVA" directory as a directory in [UI](https://en.wikipedia.org/wiki/User_interface) to put the processed [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) in. 
As soon as it is done, FVA Software will put [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) into selected directory and Multimedia IR system will be able to search through it.

### Integration with Google Photos

&nbsp;&nbsp;&nbsp; In the same manner, the integration with [Google Photos](https://en.wikipedia.org/wiki/Google_Photos) is implemented. 
First of all, it shall be mentioned that [Google Photos](https://en.wikipedia.org/wiki/Google_Photos) shall be configured to be integrated with FVA Software. 
Thus, appropriate file types and fodders shall be set up as in the figure below:

<img src="Images/IntegrationGooglePhoto.png" alt="IntegrationGooglePhoto.png" />

&nbsp;&nbsp;&nbsp; For example, "Pictures" and "fromDevices" directories are selected for the folders settings as shown above. 
It means that when the user wants to integrate the [multimedia data]((https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008)) after processing by FVA Software, the user shall be able to select the "fromDevices" or "Pictures" directory in [UI](https://en.wikipedia.org/wiki/User_interface) screen, as a directory to put the processed [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) in. 
As soon as it is done, FVA Software will put [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) into selected directory and [Google Photos]((https://en.wikipedia.org/wiki/Google_Photos)) will be able to search through it as soon as it synchronizes the [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008).