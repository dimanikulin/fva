| # | Implemented | Feature ID | Component    | Implemented in files with name | Description |
| - | ------------| -----------|--------------|--------------------------------|------------ |
| 1 |  YES        | 5          | Organizer    | CLTFixEmptyDateTime            | The system shall be able to enrich/fix the multimedia metadata for the "date/time" attribute if this attribute is set up in the configuration. If by configuration it is allowed to use the Modification File time - it shall use it. |
| 2 |  YES        | 5          | Organizer    | CLTUpdatePlaceForDir           | The system shall be able to enrich/fix the multimedia metadata for the "Place" attribute if this attribute is set up in the configuration. |
| 3 |  YES        | 5          | Organizer    | CLTUpdateEventForDir           | The system shall be able to enrich/fix the multimedia metadata for the "Event" attribute  if this attribute is set up in the configuration. |
| 4 |  YES        | 5          | Organizer    | CLTUpdateEventPeopleForDir     | The system shall be able to enrich/fix the multimedia metadata for the "Event Reason People" attribute  if this attribute is set up in the configuration. |
| 5 |  YES        | 10         | Organizer    | CLTUpdateGPSForDir             | The system shall provide a possibility to update the multimedia metadata with GEO possition when the content is already in the photo album. |
| 6 |  YES        | 10         | Organizer    | fixCLT2Issues                  | The system shall fix mismatch beetwen file name as time and exif date-time taken |
| 7 |  YES        | 10         | Organizer    | fixCLT2Issues                  | The system shall fix empty device exif metadata |
| 8 |  YES        | 10         | Organizer    | fixCLT2Issues                  | The system shall fix empty exif date-time taken | 
| 9 |  YES        | 10         | Organizer    | fixCLT2Issues                  | The system shall fix not existing (in DB) device id |