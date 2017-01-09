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

  // Really should unzip file here and verify that it has all the required parts as part of validation
  islandora_genetic_edition_ingest_witness_from_zip_file($connection, $parameters);
  $deletion = file_delete($target);

  drupal_set_message(t('The uploaded file has been scheduled for ingestion. '));
  drupal_goto("islandora/object/{$form_state['values']['parent']}");
}


function islandora_genetic_edition_ingest_witness_from_zip_file($connection, $parameters) {
  $file_url = drupal_realpath($parameters['zip_file_uri']);
  $tmpdir = variable_get('file_temporary_path', '/tmp');

  $zip = new ZipArchive;
  $response = $zip->open($file_url);
  if ($response === TRUE) {
    $zip->extractTo("{$tmpdir}/witness_ingest/");
    $zip->close();
  } else {
    watchdog('gesp', "Error extracting zip to tmp directory: temporary://witness_ingest/");
    break;
  }
 

  // Get parent namespace
  module_load_include('inc', 'islandora', 'includes/utilities');
  $parent_id = $parameters['parent'];
  $parent_namespace = islandora_get_namespace($parent_id);
  watchdog('gesp', $parent_namespace);

  $repository = $connection->repository;
  $witness_object = $repository->constructObject($parent_namespace);

  // Construct TEI DS
  $tei_datastream = $witness_object->constructDatastream('TEI');
  $tei_datastream->label = 'TEI';
  $tei_datastream->mimeType = 'application/xml';
  $tei_datastream->setContentFromFile("{$tmpdir}/witness_ingest/tei.xml");
  $witness_object->ingestDatastream($tei_datastream);

  // Construct OBJ DS
  $obj_datastream = $witness_object->constructDatastream('OBJ');
  $obj_datastream->label = 'OBJ';
  $obj_datastream->mimeType = 'image/png';
  $obj_datastream->setContentFromFile("{$tmpdir}/witness_ingest/obj.png");
  $witness_object->ingestDatastream($obj_datastream);

  $witness_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isMemberOf', $parent_id);

  $repository->ingestObject($witness_object);
  
}
