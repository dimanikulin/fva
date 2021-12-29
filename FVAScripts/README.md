
| Script Name                 | Description           | Call Example           | Parameters           |
| --------------------------- |---------------------- |----------------------- |:-------------:|
| CLTFixEmptyDateTime.py      | It fixes up the exif date-time taken by file modification time |  python  CLTFixEmptyDateTime.py "C:\FVANOGEO\283" | where `C:\FVANOGEO\283` - full path to input dir we work with - sys.argv[1] |
| CLTMerge2csv.py             | It appends the content of second file (sys.argv[2]) into first file (sys.argv[1]) | python CLTMerge2csv.py "C:/fva/#data#/fvafile.csv" "C:/fva/#data#/fvafileN.csv" | where `C:/fva/#data#/fvafile.csv` and `C:/fva/#data#/fvafileN.csv` - full paths to fvafile.csv and fvafile.csv we work with |
| CLTUpdateCopyRightAndDesc.py | TBD |  TBD | TBD |
| CLTUpdateEventForDir.py     | It updates the fvafile.csv for each file in input folder by provided event Id  |  python CLTUpdateEventForDir.py "C:/fva/#data#/fvafile.csv" "C:/fvainput/#test#/" 1  | where `C:/fva/#data#/fvafile.csv` - full path to fvafile.csv we work with, `C:/fvainput/#test#/` - full path to input dir we work with, `1` - Event Id to update by |
| CLTUpdateEventPeopleForDir.py | It updates the fvafile.csv for each file in input folder by provided people list  |  python CLTUpdateEventPeopleForDir.py "C:/fva/#data#/fvafile.csv" "C:/fvainput/#test#/" 1  | where `C:/fva/#data#/fvafile.csv` - full path to fvafile.csv we work with, `C:/fvainput/#test#/` - full path to input dir we work with, `1,2` - people list to update by |
| CLTUpdatePlaceForDir.py     | It updates GEO positions (Using Place Id provided as input parameter) in each file for folder provided as input parameter. It also updates the PlaceId in fvaFile.csv for all files (using name match) updated in input folder.| python CLTUpdatePlaceForDir.py "C:\FVANOGEO\283" 283 >> placeId283.csv | where `C:\FVANOGEO\283` - full path to input dir we work with - sys.argv[1], `283` - place id to use for updating - sys.argv[2], `placeId283.csv` - log file with fixed records |
| createFvaDB.sql             | it creates the full FVA DB |  "../sqlite3.exe" "C:/FVA/digikam4.db" < createFvaDB.sql | where `../sqlite3.exe` - path to sqlite3 bin, `C:/FVA/digikam4.db` - path to DB to create FVA DB in |
| createFvaDBDevices.sql      | it creates the full FVA DB devices table |  "../sqlite3.exe" "C:/FVA/digikam4.db" < createFvaDBDevices.sql | where `../sqlite3.exe` - path to sqlite3 bin, `C:/FVA/digikam4.db` - path to DB to create FVA DB in |
| fixCLT2Issues.py            | TBD |  TBD | TBD |
| fixNoGeoIssues.py           | It updates GEO positions (Using Place Id provided as input parameter) in each file from issues3.csv. |  python fixNoGeoIssues.py 1 >> fixNoGeoIssuesPlaceID-1.txt | where `1` - place id to use for fixing, `fixNoGeoIssuesPlaceID-1.txt` - log file with fixed records |
| getFotoByPlaceID.py         | TBD |  TBD | TBD |
| PrintExifForDir.py          | TBD |  TBD | TBD |
