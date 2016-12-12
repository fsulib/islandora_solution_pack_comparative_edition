<?php

require_once dirname(__FILE__) . '/utilities.inc';

/**
 * Local menu action that gets the ingest witness form.
 *
 * @param AbstractObject $genetic_edition
 *   The genetic edition to ingest into.
 *
 * @return string
 *   The HTML repersentation of the ingest witness form.
 */
function islandora_genetic_edition_ingest_witness(AbstractObject $genetic_edition) {
  global $user;
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/ingest.form');
  $tuque = islandora_get_tuque_connection();
  $witness = $tuque->repository->constructObject(islandora_get_namespace($genetic_edition->id));
  $witness->owner = $user->name;
  $witness->label = 'New Witness';
  $witness->models = 'islandora:geneticEditionWitnessCModel';
  // Assume sequential adding of witnesses for now.
  $witnesses = islandora_genetic_edition_get_witnesses($genetic_edition);
  $num_witnesses = count($witnesses) + 1;
  $witness->relationships->add(FEDORA_RELS_EXT_URI, 'isMemberOf', $genetic_edition->id);
  $witness->relationships->add(ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', (string) $num_witnesses, TRUE);
  drupal_set_title(t('Add witness to @genetic_edition', array('@genetic_edition' => $genetic_edition->label)));
  return drupal_get_form('islandora_ingest_form', array(
      'models' => array('islandora:geneticEditionWitnessCModel'),
      'objects' => array($witness),
      'parent' => $genetic_edition->id,
    ));
}
