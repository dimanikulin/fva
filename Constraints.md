| # | Constraint  | Description | Business value | Architecture viewpoint |
| - | ------------------------|----|----|:--------------:|
| 1 | The FVA software shall be installed on the same environment that Multimedia IR system is already working on. | Not to implement the network interaction with another environment | M | L |
| 2 | The same photo album storage shall be used for offline or online Multimedia IR system and the FVA software. | Not to implement the interaction to another storage | M | L |
| 3 | Target FVA file name shall be unique for a directory it is kept in (for any directory in the album). If several photos were taken at one time the time shift shall be applied for them. | It simplifies implementation of solution | L | H |
| 4 | The taken time that is kept in file name has the highest priority comparing for taken time in EXIF or file modification time. | A lot of already renamed files that are now in photo album have the EXIF date is empty as well as file modification time.(It is only applicable for the author photo album) | L | L |
| 5 | One format for pictures should be used (jpeg) as a widely used with EXIF support, any other format must be converted to this one with adding EXIF when needed | It simplifies implementation of solution | L | H |
| 6 | One folder shall keep multimedia data for one event only | It simplifies organization structure of photo album, implementation of solution and make photo album more searchable  | L | H |
| 7 | The solution shall use open source tools only | As it is going to be open source as well | H | M |
