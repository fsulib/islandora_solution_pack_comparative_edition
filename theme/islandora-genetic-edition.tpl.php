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
$children = array();
foreach ($results as $result) {
  $result = $result['child']['value'];
  $object = islandora_object_load($result);
  $abstract = simplexml_load_string($object['MODS']->content)->abstract;
  $children[] = array('pid' => $result, 'label' => $object->label, 'abstract' => $abstract);
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

  <?php foreach ($children as $child) { ?>
    <div class="islandora-genetic-edition-child-container">
      <a href="/islandora/object/<?php print $child['pid']; ?>">
        <img src="/islandora/object/<?php print $child['pid']; ?>/datastream/TN">
        <strong><?php print $child['label']; ?></strong>
        <p><?php print $child['abstract']; ?></p>
      </a>
    </div>
  <?php } ?>

</div>

