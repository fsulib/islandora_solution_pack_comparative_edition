<?php

/**
 * @file
 * Form to attach a witness to a genetic edition.
 */

/**
 * Form building function.
 */
function islandora_genetic_edition_attach_witness_form($form, &$form_state, $object) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $default_namespace = islandora_get_namespace($object->id);

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

  $form['namespace'] = array(
    '#type' => 'textfield',
    '#title' => t('Namespace for created objects?'),
    '#description' => t('Witness and page objects will be contructed within this namespace.'),
    "#default_value" => $default_namespace,
    '#size' => 15,
  );

}
