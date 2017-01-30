<?php

/**
 * @file
 * Handles the uploading of files for the genetic edition ingest process.
 */

/**
 * Defines a file upload form for uploading genetic edition files.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_genetic_edition_gened_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $obj_extensions = array('tif tiff jp2 jpg png');
  $form = array();

  $form['obj'] = array(
    '#title' => t('OBJ File'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select file to upload for Genetic Edition OBJ datastream.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong>', array('@size' => $upload_size, '@ext' => $obj_extensions[0])),
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $obj_extensions, 
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );

  $form['css'] = array(
    '#title' => t('CSS File'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select file to upload for Genetic Editon CSS datastream.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong>', array('@size' => $upload_size, '@ext' => 'css')),
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('css'), 
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );

  $form['display-xslt'] = array(
    '#title' => t('DISPLAY-XSLT File'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select file to upload for Genetic Editon DISPLAY datastream.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong>', array('@size' => $upload_size, '@ext' => 'xsl')),
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('xsl'), 
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );

  $form['index-xslt'] = array(
    '#title' => t('INDEX-XSLT File'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select file to upload for Genetic Editon DISPLAY datastream.<br/>Files must be less than <strong>@size MB.</strong><br/>Allowed file types: <strong>@ext.</strong>', array('@size' => $upload_size, '@ext' => 'xsl')),
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('xsl'), 
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );

  return $form;
}

/*
    '#upload_validators' => array(
      'file_validate_extensions' => 'xsl', 
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),

*/

/**
 * Addes the uploaded files into the ingestable objects associated datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_genetic_edition_gened_upload_form_submit(array $form, array &$form_state) {
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

  if ($form_state['values']['css']) {
    if (empty($object['CSS'])) {
      $ds = $object->constructDatastream('CSS', 'M');
      $object->ingestDatastream($ds);
    }
    else {
      $ds = $object['CSS'];
    }

    $css_file = file_load($form_state['values']['css']);
    $css_path = drupal_realpath($css_file->uri);
    $ds->setContentFromFile($css_path, FALSE);
    $ds->label = $css_file->filename;
    $ds->mimetype = $css_file->filemime;
  }

  if ($form_state['values']['display-xslt']) {
    if (empty($object['DISPLAY-XSLT'])) {
      $ds = $object->constructDatastream('DISPLAY-XSLT', 'M');
      $object->ingestDatastream($ds);
    }
    else {
      $ds = $object['DISPLAY-XSLT'];
    }

    $display_file = file_load($form_state['values']['display-xslt']);
    $display_path = drupal_realpath($display_file->uri);
    $ds->setContentFromFile($display_path, FALSE);
    $ds->label = $display_file->filename;
    $ds->mimetype = $display_file->filemime;
  }

  if ($form_state['values']['index-xslt']) {
    if (empty($object['INDEX-XSLT'])) {
      $ds = $object->constructDatastream('INDEX-XSLT', 'M');
      $object->ingestDatastream($ds);
    }
    else {
      $ds = $object['INDEX-XSLT'];
    }

    $index_file = file_load($form_state['values']['index-xslt']);
    $index_path = drupal_realpath($index_file->uri);
    $ds->setContentFromFile($index_path, FALSE);
    $ds->label = $index_file->filename;
    $ds->mimetype = $index_file->filemime;
  }

}
