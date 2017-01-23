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

?>

<div id="igesp-genetic-edition-header">
  <p>
    <img id="igesp-genetic-edition-preview" 
         src="/islandora/object/<?php print $islandora_object->id; ?>/datastream/PREVIEW">
    <span id="igesp-genetic-edition-abstract"><?php print $mods_obj->abstract;?></span>
  </p>
</div>

<div id="igesp-genetic-edition-children-container">
  <h1>Witnesses</h1>

  <div id="igesp-tabs">
    <ul>
      <li><a href="#tabs-1">List View</a></li>
      <li><a href="#tabs-2">Grid View</a></li>
    </ul>
  
    <div id="tabs-1">
      <?php foreach ($children as $child) { ?>
      <div class="igesp-genetic-edition-child-container">
        <a href="/islandora/object/<?php print $child['pid']; ?>">
          <img src="/islandora/object/<?php print $child['pid']; ?>/datastream/TN">
          <strong><?php print $child['label']; ?></strong>
          <p><?php print $child['abstract']; ?></p>
        </a>
      </div>
      <?php } ?>
    </div>
    
    <div id="tabs-2">
      <ul id="igesp-genetic-edition-child-grid-container">
      <?php foreach ($children as $child) { ?>
      <li>
        <a href="/islandora/object/<?php print $child['pid']; ?>">
          <p><img src="/islandora/object/<?php print $child['pid']; ?>/datastream/TN"></p>
          <p><?php print $child['label']; ?><p>
        </a>
      </li>
      <?php } ?>
      </ul>
    </div>
  </div>
</div>

