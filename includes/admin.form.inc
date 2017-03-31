<?php

/**
 * @file
 * Admin form for the islandora_comparative_edition.module
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_comparative_edition_admin_settings_form(array $form, array &$form_state) {
  $form = array();

  $form['collatex'] = array(
    '#type' => 'fieldset',
    '#title' => t('Collation configuration'),
  );

  $form['collatex']['islandora_comparative_edition_collatex_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('CollateX API endpoint'),
    '#description' => t('URL for a running CollateX service (should end in /collate)'),
    '#default_value' => variable_get('islandora_comparative_edition_collatex_server_url', 'https://collatex.net/demo/collate'),
    '#size' => 40,
  );

  $form['collatex']['islandora_comparative_edition_collatex_alignment_algorithm'] = array(
    '#type' => 'select',
    '#title' => t('Alignment algorithm'),
    '#description' => t('See <a href="https://collatex.net/doc/#alignment-algorithms">https://collatex.net/doc/#alignment-algorithms</a> for details.'),
    '#default_value' => variable_get('islandora_comparative_edition_collatex_alignment_algorithm', 'dekker'),
    '#options' => array(
      'dekker' => 'Dekker',
      'needleman-wunsch' => 'Needleman-Wunsch',
      'medite' => 'MEDITE',
    ),
  );

  $form['islandora_comparative_edition_pindex_delimiter'] = array(
    '#type' => 'textfield',
    '#title' => t('PINDEX delimiter'),
    '#description' => t('Custom delimiter used for splitting lines in PINDEX files'),
    '#default_value' => variable_get('islandora_comparative_edition_pindex_delimiter', '|'),
    '#size' => 10,
  );

  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form += islandora_viewers_form('islandora_comparative_edition_page_viewers', 'image/jp2');
  $form['page_viewers'] = $form['viewers'];
  $form['page_viewers']['#title'] = t('Page Viewers');
  unset($form['viewers']);

  return system_settings_form($form);
}

function islandora_comparative_edition_admin_settings_form_validate($form, &$form_state) {
  if (isset($form_state['values']['islandora_comparative_edition_collatex_server_url']) && !filter_var($form_state['values']['islandora_comparative_edition_collatex_server_url'], FILTER_VALIDATE_URL)) {
    form_set_error('islandora_comparative_edition_collatex_server_url', t('CollateX endpoint is not a valid URL.'));
  }
}
