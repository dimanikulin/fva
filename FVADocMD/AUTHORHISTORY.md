&nbsp;&nbsp;&nbsp; When I was a child, I used to observe the family photo album to look at my relatives who I have not seen in real life.
At that time, we had film cameras only and so the amount of photos was not so big.
Time changed and right now let's say almost everyone owns a camera or a smartphone with a camera or even both.
Because of that, the size of my photo album increased significantly. However, I do not think it is for me only.
<img src="Images/Anaveragefamilyphotoalbum.gif" alt="Anaveragefamilyphotoalbum.gif" />

&nbsp;&nbsp;&nbsp; Once Me and my friend decided to recall an event we both attended. We took my photo album and tried to search there. 
That time my album kept more than 30 thousand items inside. That search process looked like
<img src="Images/Asearchinphotoalbumwith30thousandphotos.gif" alt="Asearchinphotoalbumwith30thousandphotos.gif" />

&nbsp;&nbsp;&nbsp; Of course, we found nothing that time. Therefore, I started looking for a solution to search in [Multimedia Data](./MULTIMEDIADATA.md) in an automatic way. 
What I found as a solution were the [Multimedia IR systems](./MULTIMEDIAIRSYSTEMS.md). 
However, there are many **Multimedia IR Systems** and it was a long way to choose one of them to start searching in a way that is more efficient.
So, the **IR Multimedia Systems** were analyzed that are present on the market and available for end users. 

&nbsp;&nbsp;&nbsp; Finally, [digiKam](https://www.digikam.org/) <img src="Images/digiKam.png" alt="digiKam.png" width="20" height="20"/> as Offline **Multimedia IR System** and [Synology Moments](https://www.synology.com/en-global/dsm/feature/moments) <img src="Images/SynologyMoments.png" alt="SynologyMoments.png" height="20" /> as Online **Multimedia IR System** were selected to use. 
Please notice they were selected based on author criteria only and there is no aim to advertise anyone.
You can find [here](./MULTIMEDIAIRSYSTEMSANALYZE.md) a description of the process of analysis and comparison for **Multimedia IR Systems** I have made to choose the most suitable system for me.
</br> </br>
&nbsp;&nbsp;&nbsp; Then I started using it...  What I found just immediately there, that the current **IR Multimedia systems** do not provide a flexible and automated functionality to import the **multimedia data** into **IR Multimedia systems**.
However, it was not the last issue.
</br> </br>
&nbsp;&nbsp;&nbsp; A short usage of these systems gave me understanding that **Multimedia IR systems** significantly rely on the **multimedia metadata** as a main information source that is used for the search by device, time, location, etc.
Therefore, the presence and consistency of metadata is very important for **Multimedia IR systems** to work and for other purposes [[21]](./REFERENCES.md).
The working experience with one of those systems shows that the search results might not be full or even incorrect due to inconsistent or not-existing **multimedia metadata**.
</br> </br>
&nbsp;&nbsp;&nbsp; Let us provide two examples of incorrect search results. 
In the first case, we tried to search for the photos with the existing **Geo** positions and to see them on a map. 
The search returned four photos taken near Ghana, marked by the red circle.
<img src="Images/AnalyzeofprivateMultimediaIRsystems.png" alt="AnalyzeofprivateMultimediaIRsystems.png" />
&nbsp;&nbsp;&nbsp; In fact, nobody has taken photos near Ghana.
</br> </br>

&nbsp;&nbsp;&nbsp; In the second case, we searched for the photos taken in 2011. 
Actually, we got one video that was recognized as of March 2020, which is underlined with the red color.
<img src="Images/AnalyzeofprivateMultimediaIRsystems2.png" alt="AnalyzeofprivateMultimediaIRsystems2.png" />
&nbsp;&nbsp;&nbsp; The system has provided such results due to incorrect **metadata**. 
</br> </br>
&nbsp;&nbsp;&nbsp; The reasons for incorrect and absent **metadata** are rather different - from not setting proper data on the device that takes the photos to the privacy issues. 
This resource [[22]](./REFERENCES.md) describes the **metadata** importance in more detail and there is information about other reasons for incorrect and absent **metadata**. 
Therefore, I understood that **Multimedia IR systems** are not so effective without other tools, often called **Multimedia Organizers**.

Usually, **Multimedia Organizers** provide automation for importing the new photo set into existing photo albums and it is not implemented feature in **IR Multimedia systems**.
In addition, they might identify and address some issues that shall be fixed before the user starts searching the data using the **IR Multimedia system**. 
However, they do not implement any search function like search by emotions and it is expected.
You can find [here](./MULTIMEDIAORGANIZERS.md) more information regarding **Multimedia Organizers**. 
</br> </br>
&nbsp;&nbsp;&nbsp; Unfortunately, I did not find a **Multimedia Organizer** that fully suited my needs (fixing problem metadata, working with **digiKam** and **Synology** etc.)
That is why I created my own product to cover my needs.