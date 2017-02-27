<?php

/**
 * @file
 * Form to attach a witness to a comparative edition.
 */

/**
 * Form building function.
 */
function islandora_comparative_edition_attach_witness_form($form, &$form_state, $object) {

  $form['target'] = array(
    '#type' => 'managed_file',
    '#title' => t('Zip file'),
    '#description' => t('A Zip file containing a single witness to attach to the parent comparative edition object.'),
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

function islandora_comparative_edition_attach_witness_form_submit($form, $form_state) {
  module_load_include('inc', 'islandora_comparative_edition', 'includes/classes');
  $package = new IslandoraComparativeEditionWitnessIngestPackage();
  $package->ingest($form_state);
  drupal_goto("islandora/object/{$form_state['values']['parent']}");
}
