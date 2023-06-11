# Headline
TBD

# Article description
TBD 

# Tags
TBD

# Definitions, Acronyms, Abbreviations
| # | Abbreviation or Acronym | Definition     |
| - | ------------------------|:--------------:|
| 1 | [Google](https://en.wikipedia.org/wiki/Google)| Google LLC is an American multinational technology company that specializes in Internet-related services and products, which include [online advertising technologies](https://en.wikipedia.org/wiki/Online_advertising), a [search engine](https://en.wikipedia.org/wiki/Search_engine), [cloud computing](https://en.wikipedia.org/wiki/Cloud_computing), software, and hardware.|


# Content

## Rules to check code during code review 

* The code shall be developed using Google code style [[27]](./REFERENCES.md) 
* The "Return Code" and not "Exception" approach shall be used to provide a result to calling code.
* No commented code is allowed.
* Compiler shall be configured to threat warning as errors for all projects.
* The code shall use QDir::separator() and not "/" or "\".
* The code shall be compiled with level 4 for warnings (MS compiler)
* The file names shall be in lower case to make cross-compilation simpler.
* The constants shall be placed in separate file/files.
* Commit mesage shall start from the issue ID.

# References
| # | Name                 | Source                | Release date           |  Author                 | Description   |
| - | ---------------------|---------------------- |----------------------- | ----------------------- |:-------------:|
| 1 | Google code style | [Web](https://google.github.io/styleguide/) | |Google ||
