<?php
/**
 * @file
 * Utitlity functions.
 */

/**
 * Gets all the witnesses that the given genetic edition owns.
 *
 * The results are ordered by their RELS-EXT dateIssued property.
 * Older to newer.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing a Fedora object.
 *
 * @return array
 *   An associative array in the form of:
 *   - pid: The unique persistent identifier for the witness.
 *     - pid: The unique persistent identifier for the witness.
 *     - label: A descriptive label for the witness.
 *     - sequence: The sequence number of the witness, starts at 1.
 *     - issued: A DateTime object repersenting the date the witness was released.
 */
function islandora_genetic_edition_get_witnesses(AbstractObject $object) {
  $query = <<<EOQ
PREFIX islandora-rels-ext: <http://islandora.ca/ontology/relsext#>
PREFIX fedora-rels-ext: <info:fedora/fedora-system:def/relations-external#>
SELECT DISTINCT ?object ?sequence ?label ?issued
FROM <#ri>
WHERE {
  ?object fedora-rels-ext:isMemberOf <info:fedora/{$object->id}> ;
       <fedora-model:hasModel> <info:fedora/islandora:geneticEditionWitnessCModel> ;
       <fedora-model:label> ?label .
  ?object islandora-rels-ext:isSequenceNumber ?sequence .
  ?object islandora-rels-ext:dateIssued ?issued
  !optionals
  !filters
}
ORDER BY ?sequence
EOQ;
  // XXX: Can't really use the fully featured query_filter and query_statements
  // hook as it will return some things we don't want. Perhaps change the return
  // structure in the future to specify which module they are coming from? For
  // now we will just get XACML's directly.
  $query_optionals = (array) module_invoke('islandora_xacml_api', 'islandora_basic_collection_get_query_optionals', 'view');
  $query_filters = (array) module_invoke('islandora_xacml_api', 'islandora_basic_collection_get_query_filters');
  $filter_map = function ($filter) {
    return "FILTER($filter)";
  };
  $query = format_string($query, array(
    '!optionals' => !empty($query_optionals) ? ('OPTIONAL {{' . implode('} UNION {', $query_optionals) . '}}') : '',
    '!filters' => implode(' ', array_map($filter_map, $query_filters)),
  ));
  $results = $object->repository->ri->sparqlQuery($query);
  // Map the results using a default Datetime for missing issued dates.
  $map_results = function($o) {
    try {
      @$issued = new DateTime($o['issued']['value']);
    }
    catch (Exception $e) {
      // Use the current time as a place holder.
      $issued = new DateTime();
      $msg  = 'Failed to get issued date from SPARQL query for @pid';
      $vars = array('@pid' => $o['object']['value']);
      watchdog_exception('islandora_genetic_edition', $e, $msg, $vars, WATCHDOG_ERROR);
    }
    return array(
      'pid' => $o['object']['value'],
      'label' => $o['label']['value'],
      'sequence' => $o['sequence']['value'],
      'issued' => $issued,
    );
  };
  $witnesses = array_map($map_results, $results);
  // Grab the PIDs...
  $get_pid = function($o) {
    return $o['pid'];
  };
  $pids = array_map($get_pid, $witnesses);
  // Make the PIDs the keys.
  return count($pids) ? array_combine($pids, $witnesses) : array();
}
