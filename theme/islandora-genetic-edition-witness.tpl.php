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
  $label = islandora_object_load($result)->label;
  $children[] = array('pid' => $result, 'label' => $label);
}

?>

<h1>I AM GENETIC EDITION WITNESS OBJECT <?php print $islandora_object->id; ?>!!1</h1>

<h2>Pages:</h2>

<?php foreach ($children as $child) { ?>
  <div>
    <a href="/islandora/object/<?php print $child['pid']; ?>">
      <strong><?php print $child['label']; ?></strong>
      <img src="/islandora/object/<?php print $child['pid']; ?>/datastream/TN">
    </a>
  </div>
<?php } ?>

