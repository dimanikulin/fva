
| Script Name                 | Description           | Call Example           | Parameters           |
| --------------------------- |---------------------- |----------------------- |:-------------:|
| CLTFixEmptyDateTime.py      | TBD |  TBD | TBD |
| CreateFvaDBDevices.cmd      | TBD |  TBD | TBD |
| PrintExifForDir.py          | TBD |  TBD | TBD |
| checkCLT2.cmd               | TBD |  TBD | TBD |
| checkCLT3.cmd               | TBD |  TBD | TBD |
| createFvaDB.cmd             | TBD |  TBD | TBD |
| createFvaDB.sql             | TBD |  TBD | TBD |
| createFvaDBDevices.sql      | TBD |  TBD | TBD |
| fixCLT2Issues.py            | TBD |  TBD | TBD |
| fixNoGeoIssues.py           | It updates GEO positions (Using Place Id provided as input parameter) in each file from issues3.csv. |  python fixNoGeoIssues.py 1 >> fixNoGeoIssuesPlaceID-1.txt | where `1` - place id to use for fixing, `fixNoGeoIssuesPlaceID-1.txt` - log file with fixed records |
| getFotoByPlaceID.py         | TBD |  TBD | TBD |
| merge2csv.py                | TBD |  TBD | TBD |
| setAttr.cmd                 | TBD |  TBD | TBD |
| updatePlaceIdForFolder.py | It updates GEO positions (Using Place Id provided as input parameter) in each file for folder provided as input parameter. It also updates the PlaceId in fvaFile.csv for all files (using name match) updated in input folder.| python updatePlaceIdForFolder.py "C:\FVANOGEO\283" 283 >> placeId283.csv | where `C:\FVANOGEO\283` - full path to input dir we work with - sys.argv[1], `283` - place id to use for updating - sys.argv[2], `placeId283.csv` - log file with fixed records |
