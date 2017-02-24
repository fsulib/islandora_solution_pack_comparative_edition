<?php

/**
 * @file
 * Handles the uploading of files for the comparative edition ingest process.
 */

/**
 * Defines a file upload form for uploading comparative edition files.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_comparative_edition_comp_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $obj_extensions = array('tif tiff jp2 jpg png');
  $form = array();

  $form['obj'] = array(
    '#title' => t('OBJ File'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select file to upload for Comparative Edition OBJ datastream.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong>', array('@size' => $upload_size, '@ext' => $obj_extensions[0])),
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $obj_extensions, 
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );

  return $form;
}

/**
 * Addes the uploaded files into the ingestable objects associated datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_comparative_edition_comp_upload_form_submit(array $form, array &$form_state) {
  $object = islandora_ingest_form_get_object($form_state);

  if ($form_state['values']['obj']) {
    if (empty($object['OBJ'])) {
      $ds = $object->constructDatastream('OBJ', 'M');
      $object->ingestDatastream($ds);
    }
    else {
      $ds = $object['OBJ'];
    }

    $obj_file = file_load($form_state['values']['obj']);
    $obj_path = drupal_realpath($obj_file->uri);
    $ds->setContentFromFile($obj_path, FALSE);
    $ds->label = $obj_file->filename;
    $ds->mimetype = $obj_file->filemime;
  }

}
