CREATE VIEW IF NOT EXISTS checkFvaFolderCount 
AS
select (select Count(*) from fva where fvaFolderId <> -1) - (select Count(*) from fvafolder);

CREATE VIEW IF NOT EXISTS checkFvaFileCount 
AS
select (select Count(*) from fva where fvaFileId <> -1) - (select Count(*) from fvafile);

CREATE VIEW IF NOT EXISTS getFilesAndDirsByDevId 
AS
select 
(
select f.* from fva f, fvaFolder ff where f.FvaFolderId = ff.ID and ff.DevID = 160 
UNION
select f.* from fva f, fvaFile ff where f.FvaFileId = ff.ID and ff.DevID = 160
);