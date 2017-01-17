<?php

/**
 * @file
 * This is the template file for genetic edition objects
 */

// Get child witness objects
$query = <<<EOQ
SELECT ?child FROM <#ri> WHERE {
  ?child <http://islandora.ca/ontology/relsext#isMemberOf> <info:fedora/{$islandora_object->id}>
}
EOQ;
$connection = islandora_get_tuque_connection();
$results = $connection->repository->ri->sparqlQuery($query);
$pids = array();
foreach ($results as $result) {
  $result = $result['child']['value'];
  $pids[] = $result;
}

$mods_string = $islandora_object['MODS']->content;
$mods_obj = simplexml_load_string($mods_string);
?>

<div id="islandora-genetic-edition-header">
  <p>
    <img id="islandora-genetic-edition-preview" 
         src="/islandora/object/<?php print $islandora_object->id; ?>/datastream/PREVIEW">
    <span id="islandora-genetic-edition-abstract"><?php print $mods_obj->abstract;?></span>
  </p>
</div>

<div id="islandora-genetic-edition-children-container">
  <h1>Witnesses</h1>
  <hr />
  <ul>
  <?php foreach ($pids as $pid) { ?>
    <li><a href="/islandora/object/<?php print $pid?>"><?php print $pid; ?></a></li>
  <?php } ?>
  </ul>
</div>

