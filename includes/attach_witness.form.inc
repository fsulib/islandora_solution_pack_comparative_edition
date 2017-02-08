<?php

/**
 * @file
 * Form to attach a witness to a genetic edition.
 */

/**
 * Form building function.
 */
function islandora_genetic_edition_attach_witness_form($form, &$form_state, $object) {

  $form['target'] = array(
    '#type' => 'managed_file',
    '#title' => t('Zip file'),
    '#description' => t('A Zip file containing a single witness to attach to the parent genetic edition object.'),
    '#required' => TRUE,
    '#upload_location' => 'public://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('zip'),
    ),
  );
  
  $form['parent'] = array(
    '#type' => 'value',
    '#value' => $object->id,
  );
 
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  return $form;
}

function islandora_genetic_edition_attach_witness_form_submit($form, $form_state) {
  $target = file_load($form_state['values']['target']);
  $connection = islandora_get_tuque_connection();
  $parameters = $form_state['values'] + array(
    'zip_file_uri' => $target->uri,
    'type' => 'zip',
    'parent_relationship_uri' => 'info:fedora/fedora-system:def/relations-external#',
    'parent_relationship_pred' => 'isMemberOf',
    'wait_for_metadata' => FALSE,
    'directory_dedup' => FALSE,
    'content_models' => array('islandora:geneticEditionWitnessCModel'),
  );

  module_load_include('inc', 'islandora_genetic_edition', 'includes/classes');
  $package = new IslandoraGeneticEditionWitnessIngestPackage();
  $package->ingest($connection, $parameters);
  $deletion = file_delete($target);
  drupal_goto("islandora/object/{$form_state['values']['parent']}");
}
