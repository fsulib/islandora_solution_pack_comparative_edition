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
function islandora_genetic_edition_ingest_witness(AbstractObject $object, $allowed_extensions, $model) {
  return drupal_get_form('islandora_genetic_edition_witness_upload_form', $object->id, $allowed_extensions, $model);
}

function islandora_genetic_edition_witness_upload_form(array $form, array &$form_state, $pid, $allowed_extensions, $model) {
  module_load_include('inc', 'islandora_ocr', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $object = islandora_object_load($pid);
  $current_pages = islandora_paged_content_get_pages($object);
  $last_page_number = count($current_pages);
  $languages = module_exists('islandora_ocr') ? islandora_ocr_get_enabled_tesseract_languages() : array();
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('zip');
  $form = array();
  $message = t("This sequence currently has @count pages. Additional pages will be appended to the end of the sequence by default. !break", array("@count" => $last_page_number, '!break' => '<br />'));
  $message .= t("Choose a number lower than @count to insert page(s) at a specific location in the sequence.", array("@count" => $last_page_number, '!break' => '<br />'));
  if ($current_pages) {
    $form['insertion_point'] = array(
      '#type' => 'textfield',
      '#title' => t('Last sequence number'),
      '#default_value' => $last_page_number,
      '#description' => check_plain($message),
      '#size' => 5,
    );
  }
  // Value behaves more consistently when passed as a string.
  $form['current_pages'] = array(
    '#type' => 'hidden',
    '#value' => serialize($current_pages),
  );
  $form['file'] = array(
    '#title' => t('Compressed images file.'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select file to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b>', array('@size' => $upload_size, '@ext' => $extensions[0])),
    '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );
  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $pid,
  );
  $form['allowed_extensions'] = array(
    '#type' => 'hidden',
    '#value' => $allowed_extensions,
  );
  $form['model'] = array(
    '#type' => 'hidden',
    '#value' => $model,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add files'),
  );
  return $form;
}
