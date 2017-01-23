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

<div id="igesp-witness-header">
  <p>
    <img id="igesp-witness-preview" 
         src="/islandora/object/<?php print $islandora_object->id; ?>/datastream/PREVIEW">
    <span id="igesp-witness-abstract"><?php print $mods_obj->abstract;?></span>
  </p>
</div>

<div id="igesp-witness-children-container">
  <h1>Pages</h1>
  <hr />

  <ul>
  
  <?php foreach ($children as $child) { ?>
    <li class="igesp-witness-child-container">
      <p><a href="/islandora/object/<?php print $child['pid']; ?>">
          <img src="/islandora/object/<?php print $child['pid']; ?>/datastream/TN"></a></p>
        <p class="igesp-witness-page-label"><strong><?php print $child['label']; ?></strong><p>
    </li>
  <?php } ?>

  </ul>  
</div>


