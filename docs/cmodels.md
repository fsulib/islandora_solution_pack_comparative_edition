# Explanation of New CModels

The Islandora Comparative Edition Solution Pack introduces three new CModels: the Comparative Edition object, the Witness object, and the Page object. 

The terminology used in the naming and description of these CModels is heavily influenced by ["Genetic Criticism"](http://www.textualscholarship.org/gencrit/index.html), and in fact this module was originally built to be used specifically for Genetic Criticism projects but was later genericised so that it could be used to compare other types of works as well. Still, the idea of Genetic Criticism informs the functionality of the module and it can be best understood in this context.

For the purposes of documentation, we will refer to a ficticious example project in which we want to compare various drafts of a novel at sentence-level.

## The Comparative Edition Object
A CE represents the "idea" of a text that may have many different representations, each embodied in a separate witness. The CE wraps them all together under one logical umbrella. In the example of a novel with multiple drafts, the idea of the novel itself, abstracted away from any specific draft, would be the CE.

In Islandora, the Comparative Edition object is a top-level object relative to the other two new cmodels. A Comparative Edition object (or CE for short) is the object that would live in a collection, and has Witness objects as its children which are built into its display. This CModel has two special datastreams, "ANALYSISMINDEX" and "DISPLAYMINDEX" ("MINDEX" is a portmanteau of Master Index) which contain JSON encoded data structures representing the collation of all the sentence-level data from each witness. The difference between the two is subtle; "DISPLAYMINDEX" may contain HTML and is meant to show a comparison of the sentences *as they appear on the page*, while the "ANALYSISMINDEX" is purely plain-text and gets passed to CollateX for syntactical comparison.


## The Witness Object
A Witness object is a child of Comparative Edition object, and represents one "view" of that Comparative Edition. In a Genetic Criticism project, a witness would be an individual draft or evolution of the work itself, with the witness itself being the point of comparison. In the example of the novel with multiple drafts, each draft would be its own witness.

In this module, [a witness and all of its pages are ingested as a single package](https://github.com/fsulib/islandora_solution_pack_comparative_edition/blob/master/docs/witness-packaging.md), which is handled by a special ingest process to create the [master indices (or "MINDEX"s)](https://github.com/fsulib/islandora_solution_pack_comparative_edition/blob/master/docs/indexing.md) of all the page-level data for comparison. 

## The Page Object
While not specifically a Genetic Criticism term, it is helpfull to define a page-level object for usage in Islandora in order to break the witness down into smaller chunks, in terms of digitized images, transcriptions and user-created indicies. In this module, most of the functionality is accessed via the [display of page objects](https://github.com/fsulib/islandora_solution_pack_comparative_edition/blob/master/docs/page-display.md). Page objects have individual datastreams for raw XML content (CONTENTXML), the rendered HTML version of that content (DISPLAYHTML), and the page-level indices ("DISPLAYPINDEX" which may contain HTML and "ANALYSISPINDEX" which is plain-text) of the sentences contained in that page ("PINDEX" is a portmanteau of Page Index). Each page's "DISPLAYPINDEX" and "ANALYSISPINDEX" are gathered for collation into the CE object's "DISPLAYMINDEX" and "ANALYSISPINDEX" (respectively) which are used to display all available versions of a given sentence.

In the example of the novel with multiple drafts, a page object would represent one specific page of one witness. The same page of different witnesses will be represented in Islandora by individual page objects.

Page level objects are created as part of the witness ingest process, as page data is included in [witness ingest packages](https://github.com/fsulib/islandora_solution_pack_comparative_edition/blob/master/docs/witness-packaging.md).
