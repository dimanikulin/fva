# Integration 
&nbsp;&nbsp;&nbsp; As it was mentioned before in the business requirements(aka features), the integration with offline Multimedia IR system is implemented through the integration with [digiKam](https://www.digikam.org/about/) and integration with online Multimedia IR system is implemented through the integration with [Google Photos](https://en.wikipedia.org/wiki/Google_Photos).
Because the Multimedia IR system performs the search by certain criteria basing on multimedia [metadata](https://en.wikipedia.org/wiki/Metadata), the FVA Software shall be working with the same multimedia [metadata](https://en.wikipedia.org/wiki/Metadata) to be integrated with Multimedia IR system.

<img src="Images/Integration.png" alt="Integration.png" />

## Integration with digiKam
&nbsp;&nbsp;&nbsp; For [digiKam](https://www.digikam.org/about/), the "Collections" setting shall be used to define how it is configured to search the [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) through. 
The setting "Monitor the albums for external changes (requires restart)" shall be enabled. 
Taking into account this setting, the [digiKam](https://www.digikam.org/about/) will pick up the external changes in the collection as soon as the proposed solution will put it in.

<img src="Images/IntegrationDigiKam.png" alt="IntegrationDigiKam.png" />

&nbsp;&nbsp;&nbsp; Still, from the side of FVA Software it is required to provide a possibility to configure the storage to put the [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) processed to be integrated with [digiKam](https://www.digikam.org/about/).
Therefore, the separate [UI](https://en.wikipedia.org/wiki/User_interface) screen is created to ask the user what storage location, [digiKam](https://www.digikam.org/about/) should use for the incoming [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008).

&nbsp;&nbsp;&nbsp; For example, a directory "C:\FVA" is selected for "Local Collections" as shown on a picture above. 
Therefore, when the user wants to integrate the [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) after processing by FVA Software, the user shall be able to select the "C:\FVA" directory as a directory in [UI](https://en.wikipedia.org/wiki/User_interface) to put the processed [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) in. 
As soon as it is done, FVA Software will put [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) into selected directory and Multimedia IR system will be able to search through it.

## Integration with Google Photos

&nbsp;&nbsp;&nbsp; In the same manner, the integration with [Google Photos](https://en.wikipedia.org/wiki/Google_Photos) is implemented. 
First of all, it shall be mentioned that [Google Photos](https://en.wikipedia.org/wiki/Google_Photos) shall be configured to be integrated with FVA Software. 
Thus, appropriate file types and fodders shall be set up as in the figure below:

<img src="Images/IntegrationGooglePhoto.png" alt="IntegrationGooglePhoto.png" />

&nbsp;&nbsp;&nbsp; For example, "Pictures" and "fromDevices" directories are selected for the folders settings as shown above. 
It means that when the user wants to integrate the [multimedia data]((https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008)) after processing by FVA Software, the user shall be able to select the "fromDevices" or "Pictures" directory in [UI](https://en.wikipedia.org/wiki/User_interface) screen, as a directory to put the processed [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) in. 
As soon as it is done, FVA Software will put [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) into selected directory and [Google Photos]((https://en.wikipedia.org/wiki/Google_Photos)) will be able to search through it as soon as it synchronizes the [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008).

## Integration with Synology

TBD

