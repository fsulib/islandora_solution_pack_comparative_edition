# Explanation of Page CModel Display

In the Islandora Comparative Edition Solution Pack, most of the higher level functionality is accessible via the page object display. This includes skipping to the next/previous pages and jumping to any other "sibling" page of the same witness, which is fairly intuitive from the user interface.

Different representations of the page may be viewed by clicking on the three tabs above the box in the center of the page; the "Image" tab will display a zoomable view of the image of the page, the "Transcript" button will display a rendered HTML view of the transcript of the page, and the "XML" tab will show the raw XML data of the page transcript (usually TEI but not necessarily).

By default the page loads in "Focus View" where there is a single box on the page with the three tabs above it, but a user may also click the "Comparison View" button to split the screen into two separate boxes with their own tabs so that they may compare the "Image" view with the "Transcript" view (or any other combination).

Finally, when displaying the "Transcript" view, users may compare any sentence in the transcript against all other versions of that same sentence by clicking on the sentenceitself. This will activate a popup window showing every version of the sentence across witnesses, along with what witness and page they come from (with the witness/page description linking to that Islandora page object). This popup comparison window will show each sentence as a whole if indexed without CollateX, and if indexed with CollateX enabled it will show each sentence tokenized and aligned. In the tokenized and aligned version, tokens will appear with a green box around them if that token 
