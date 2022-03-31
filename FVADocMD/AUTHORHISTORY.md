When I was a child I used to observe the family photo album to take a look at my relations who I have not seen in real life.
At that time we had film cameras only and so the amount of photos was not so big.
Time changed and right now let say almost everyone owns a camera or a smartphone with camera or even both.
Because of that the size of my photo album increased significally. But I don't think it is for me only.
<img src="Images/Anaveragefamilyphotoalbum.gif" alt="Anaveragefamilyphotoalbum.gif" />

Once Me and my friend decided to recall an event we both attended to. We took my photo album and tried to search there. 
That time my album kept more than 30 thousand items inside. That search process looked like
<img src="Images/Asearchinphotoalbumwith30thousandphotos.gif" alt="Asearchinphotoalbumwith30thousandphotos.gif" />

Of course we found nothing that time. And I started looking for a solution to search in [Multimedia Data](./MULTIMEDIADATA.md) in automatic way. 
What I found as a solution were the [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md). 
But there are a lot of Multimedia IR Systems and there was a long way to choose one of them to start searching in more efficient way.
So the IR Multimedia Systems that are present on the market and available for end user were analyzed. 

Finally [digiKam](https://www.digikam.org/) <img src="Images/digiKam.png" alt="digiKam.png" width="20" height="20"/> as Offline Multimedia IR System and [Synology Moments](https://www.synology.com/en-global/dsm/feature/moments) <img src="Images/SynologyMoments.png" alt="SynologyMoments.png" height="20" /> as Online Multimedia IR System were chosen to use. </br> </br> 
Please notice they were chosen basing on author criteria only and there is no aim to advertise anyone.
You can find [here](./MULTIMEDIAIRSYSTEMSANALYZE.md) a description of the process of analyze and comparison for Multimedia IR Systems I have made to choose the most suitable system to me.
</br> </br>
and Then I started using it... 
</br> What I found just immediatly there, that the current IR Multimedia systems don't provide a flexible and automated functionality to import the multimedia data into IR Multimedia systems.
But it was not last issue.
</br> </br>
A short usage of these systems gave me understanding that Multimedia IR systems significantly rely on the multimedia metadata  as a main information source, that is used for the search by device, time, location, etc.  
So, the presence and consistency of metadata is very important for Multimedia IR systems to work and for other purposes [[21]](./REFERENCES.md).
The working experience with one of those systems shows that the search results might not be full or even incorrect due to inconsistent or not existing multimedia metadata.
</br> </br>
Let us provide two examples of incorrect search results. 
In the first case, we tried to search for the photos with the existing Geo positions and to see them on a map. 
The search returned 4 photos taken near Ghana, marked by the red circle.
<img src="Images/AnalyzeofprivateMultimediaIRsystems.png" alt="AnalyzeofprivateMultimediaIRsystems.png" />
In fact, nobody has taken photos nearby Ghana.
</br> </br>
In the second case, we searched for the photos taken in 2011. 
Actually, we got one video that, got recognized as of March 2020, which is underlined with the red color.
<img src="Images/AnalyzeofprivateMultimediaIRsystems2.png" alt="AnalyzeofprivateMultimediaIRsystems2.png" />
The system has provided such results due to incorrect metadata. 
</br> </br>
The reasons for incorrect and absent metadata are rather different - from not setting proper data on the device that takes the photos to the privacy issues. 
This resource [[22]](./REFERENCES.md) describes the metadata importance in more details and there is information about other reasons of incorrect and absent metadata. 
So I understood that Multimedia IR systems are not so effective without other tools, often called as Multimedia Organizers.
Usually, Multimedia Organizers provide automation for importing the new photo set into existing photo album and it is not implemented feature in IR Multimedia systems.
Also they might identify and address some issues that shall be fixed before user starts searching the data using IR Multimedia system. 
But they do not implement any search function like search by emotions and it is expected.
You can find [here](./MULTIMEDIAORGANIZERS.md) more information regarding Multimedia Organizers. 
</br> </br>
Unfortunately I did not find a Multimedia Organizer that fully suited my needs (fixing problem metadata, working with digiKam and Synology etc.)
That is why I created own product to cover my needs.