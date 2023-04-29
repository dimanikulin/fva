# Headline
TBD

# Article description
TBD 
This chapter consists of quality attributes explanation, solution constrains and assumptions. 

# Tags
TBD

# Content
## Constraints
This section lists constrains and explanations for them. 

| # | Constraint  | Description | Business value | Architecture viewpoint |
| - | ------------------------|----|----|:--------------:|
| 1 | The FVA software shall be installed on the same environment that Multimedia IR system is already working on. | Not to implement the network interaction with another environment | M | L |
| 2 | The same photo album storage shall be used for offline or online Multimedia IR system and the FVA software. | Not to implement the interaction to another storage | M | L |
| 3 | Target FVA file name shall be unique for a directory it is kept in (for any directory in the album). If several photos were taken at one time the time shift shall be applied for them. | It simplifies implementation of solution | L | H |
| 4 | The taken time that is kept in file name has the highest priority comparing for taken time in EXIF or file modification time. | A lot of already renamed files that are now in photo album have the EXIF date is empty as well as file modification time.(It is only applicable for the author photo album) | L | L |
| 5 | One format for pictures should be used (jpeg) as a widely used with EXIF support, any other format must be converted to this one with adding EXIF when needed | It simplifies implementation of solution | L | H |
| 6 | One folder shall keep multimedia data for one event only | It simplifies organization structure of photo album, implementation of solution and make photo album more searchable  | L | H |
| 7 | The solution shall use open source tools only | As it is going to be open source as well | H | M |
| 8 |  |  |  |  |

## Assumptions
This section lists all the assumptions with explanations for them. 
This is critical to do in order to cover all the gaps in the requirements.

| # | Assumption Description  | 
| - |:--------------:|
| 1 | It is assumed that the user, which the proposed solution is going to run under, has the write access to the [Google Photo](https://en.wikipedia.org/wiki/Google_Photos) (and other IR Multimedia systems) folder where [multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008) processed is going to be put in. | 
| 2 | User restarted the wizard after changing the configuration (because of feature of QTWizard and QTWizardPage) |

## Quality Attributes
[Quality Attributes for the FVA are described here](QUALITYATTRIBUTES_FVA.md)

# Definitions, Acronyms, Abbreviations
| # | Abbreviation or Acronym | Definition     |
| - | ------------------------|:--------------:|
| 1 | [Exif](https://en.wikipedia.org/wiki/Exif)| Exchangeable image file [format](https://en.wikipedia.org/wiki/File_format) (officially Exif, according to JEIDA/JEITA/CIPA specifications) is a standard that specifies the [formats](https://en.wikipedia.org/wiki/File_format) for [images](https://en.wikipedia.org/wiki/Image), [sound](https://en.wikipedia.org/wiki/Sound), and ancillary tags used by [digital cameras](https://en.wikipedia.org/wiki/Digital_camera) (including [smartphones](https://en.wikipedia.org/wiki/Smartphone)), [scanners](https://en.wikipedia.org/wiki/Image_scanner) and other systems handling image and sound files recorded by digital cameras.|
| 2 | [Google](https://en.wikipedia.org/wiki/Google)| Google LLC is an American multinational technology company that specializes in Internet-related services and products, which include [online advertising technologies](https://en.wikipedia.org/wiki/Online_advertising), a [search engine](https://en.wikipedia.org/wiki/Search_engine), [cloud computing](https://en.wikipedia.org/wiki/Cloud_computing), software, and hardware.|
| 3 | IR Multimedia Systems |multimedia information retrieval means the process of searching for and finding multimedia documents; the corresponding research field is concerned with building multimedia search engines. |
| 4 | [Multimedia data](https://link.springer.com/referenceworkentry/10.1007%2F978-0-387-39940-9_1008)| Multimedia in principle means data of more than one medium. It usually refers to data representing multiple types of medium to capture information and experiences related to objects and events. Commonly used forms of data are numbers, alphanumeric, text, images, audio, and video. In common usage, people refer a data set as multimedia only when time-dependent data such as audio and video are involved.|
| 5 | Multimedia IR| Multimedia Information Retrieval (MIR) is an organic system made up of Text Retrieval (TR); Visual Retrieval (VR); Video Retrieval (VDR); and Audio Retrieval (AR) systems. So that each type of digital document may be analyzed and searched by the elements of language appropriate to its nature, search criteria must be extended.|
