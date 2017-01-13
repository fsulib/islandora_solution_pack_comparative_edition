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

?>

<h1>I AM GENETIC EDITION WITNESS OBJECT <?php print $islandora_object->id; ?>!!1</h1>

<h2>AND I HAVE THE FOLLOWING CHILDREN:</h2>
<ul>

<?php foreach ($pids as $pid) { ?>
  <li><a href="/islandora/object/<?php print $pid?>"><?php print $pid; ?></a></li>
<?php } ?>

</ul>


