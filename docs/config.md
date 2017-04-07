# Module Configuration

This module offers several configuration options, and should be configured prior to the ingestion of any witness packages as these configuration options will affect the ingest process.

## CollateX
Administrators must enter a valid CollateX API endpoint (a complete URL that typically ends in '/collate', for example the default value for the CollateX demo server's API endpoint which is `https://collatex.net/demo/collate` but should be switched to something else out of consideration). This configuration determines where to send all of the sentence-level data for processing as part of the *MINDEX generation process.

In addition to configuring your CollateX API endpoint, you may also choose what algorithm you want to use for the tokenization and variance comparison of your sentences. The default is the Dekker algorithm, but the Needleman-Wunsch and MEDITE algorithms are also available for use (see [the CollateX algorithm documentation](https://collatex.net/doc/#alignment-algorithms) for more info).

## PINDEX Delimiters
This field (which is also used during *MINDEX creation and as such should be configured BEFORE ingesting any witnesses) specifies what character should be used as the delimiter character in your page's *PINDEX datastreams. During *MINDEX creation, *PINDEX files will be read line-by-line and broken apart on this character, so take care to pick a character which will never appear in your project's sentences. By default this is a | (bar) symbol since this symbol is rarely used, but we chose to make this character a user-configurable variable since bar characters actually DO appear in the project this module was built for. More than one character may be used, for instance || (two bars) may be chosen as your *PINDEX delimiter if this series of characters never appears in our project even though a single bar does. Remember that changing the PINDEX delimiter will only apply to new *MINDEX generations going forward and will not work retroactively, and this variable applies to ALL Comparative Edition objects in your Islandora instance, so choose carefully.

## Page Viewer
A standard option for choosing which Islandora viewer you want to use for page-level images. No viewer is selected by default, but we highly recommend you use the Open Seadragon viewer to have a display similar to the Islandora Large Image Solution Pack. Please note that, due to the way Open Seadragon works, this viewer will fail if the "View repository objects" permission is not granted to anonymous users, as the javascript calls in Open Seadragon will not be able to retrieve the JP2 datastream.

