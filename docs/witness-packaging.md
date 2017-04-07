# Description of Witness Ingest Packages and their Creation

While top-level CE objects can only be created via the GUI forms, Witness can only be added to a CE object by clicking on the "Attach Witness" link in the "Manage" menu on a CE object and uploading properly structured witness ingest package. A witness ingest package MUST have the following structure:

```
- mods.xml
- display.css
- obj.[jpg|png|tiff]
- pages/
  - page1.mods.xml
  - page1.content.xml
  - page1.display.html
  - page1.obj.[jpg|png|tiff]
  - page1.index.txt
  - page1.index.html
```

Note that when creating a witness ingest package, the top level files (mods.xml, display.css, etc) must be in the top level of the zip file and NOT inside of a folder themselves. When a properly created witness ingest package is unzipped, it should NOT create folder containing these files. Furthermore, the file names for the top level files must follow the naming convention in the example; the MODS record must be named mods.xml, any CSS to be used must be named display.css (this file must be present, but may be empty), the image to represent the witness must be obj. + the appropriate file extension, and any pages that make up the witness must be included in the pages/ directory.

## Pages
There must be 6 individial files in the pages/ directory for any given page that should appear as the child of a witness, and these pages must follow the standard Islandora naming convention of having the base filename be same for every file associated with the same page, with only the extensions changing. For instance, to create an example page "foo", it is required that each of the following files be included:
- foo.mods.xml (MODS record)
- foo.content.xml (raw XML file to be shown under "XML" tab of page display)
- foo.display.html (HTML file to be shown under "Transcript" tab of page display)
- foo.obj.[jpg|png|tiff] (image file of page)
- foo.index.txt (plain text page index)
- foo.index.html (HTML page index)

All files in the pages/ directory starting with "foo.*" will be used to create the same page, and the actual naming convention you use to label your pages does not matter so long as it is consistent across all 6 included files that represent the page. There is no limit to how many pages you may include in the pages/ directory so long as they all follow this naming convention and no files are missing.

## Ingesting Witness Packages
To ingest a witness package, go to the CE object that you want to attach the witness to and click "Manage" and then the "Attach Witness" link. This will lead you to a form where you can upload the zipped witness ingest package and click "Submit". This process is very involved and can take a while, so give Islandora plenty of time to sort out the process and do not end the process prematurely. If there is an error with the package it should let you know immediately (there is a package validation process that takes place before the actual building of the witness takes place to avoid creation of half-built but broken witnesses), so if it does not error out immediately that means it is working. Keep in mind that every time a new witness is uploaded, the CE object must have its MINDEX datastreams rebuilt from scratch to accomodate the new information, and this accounts for the majority of the time spent waiting on new witness packages to be ingested.

## Package Structure Rationale
Originally when building this module considered having users upload XSLT files that could generate the DISPLAYHTML, TEXTPINDEX and HTMLPINDEX datastreams from the CONTENTXML datastream, but we determined that this would be too limiting. Having users generate all of these "derivative" files OUTSIDE of Islandora allows them to use their own tools (not just XSLT, and not just the version of XSLT that Islandora/PHP has access to), and run various post-processing scripts on these files as well. Keep this in mind when looking at the 6 different files required for each page, it is much easier for YOU to generate these files individually than have to write XSLTs for Islandora.

