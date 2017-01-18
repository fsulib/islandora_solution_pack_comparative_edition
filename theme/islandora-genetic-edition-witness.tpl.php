<?php

/**
 * @file
 * This is the template file for genetic edition witness objects
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
  $label = islandora_object_load($result)->label;
  $children[] = array('pid' => $result, 'label' => $label);
}

$mods_string = $islandora_object['MODS']->content;
$mods_obj = simplexml_load_string($mods_string);
?>

<div id="islandora-genetic-edition-witness-header">
  <p>
    <img id="islandora-genetic-edition-witness-preview" 
         src="/islandora/object/<?php print $islandora_object->id; ?>/datastream/PREVIEW">
    <span id="islandora-genetic-edition-witness-abstract"><?php print $mods_obj->abstract;?></span>
  </p>
</div>

<div id="islandora-genetic-edition-witness-children-container">
  <h1>Pages</h1>
  <hr />

  <?php foreach ($children as $child) { ?>
    <div class="islandora-genetic-edition-witness-child-container">
      <a href="/islandora/object/<?php print $child['pid']; ?>">
        <img src="/islandora/object/<?php print $child['pid']; ?>/datastream/TN">
        <strong><?php print $child['label']; ?></strong>
      </a>
    </div>
  <?php } ?>

</div>


