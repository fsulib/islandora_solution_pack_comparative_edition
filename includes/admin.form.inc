<?php

/**
 * @file
 * Admin form for the islandora_genetic_edition.module
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
function islandora_genetic_edition_admin_settings_form(array $form, array &$form_state) {
  $form = array();
  $form['islandora_genetic_edition_collatex_server_url'] = array(
    '#type' => 'textfield',
    '#title' => t('CollateX endpoint'),
    '#description' => t('URL for a running CollateX service.'),
    '#default_value' => variable_get('islandora_genetic_edition_collatex_server_url', ''),
    '#size' => 100,
  );
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form += islandora_viewers_form('islandora_newspaper_page_viewers', 'image/jp2');
  $form['page_viewers'] = $form['viewers'];
  $form['page_viewers']['#title'] = t('Page Viewers');
  unset($form['viewers']);
  return system_settings_form($form);
}

function islandora_genetic_edition_admin_settings_form_validate($form, &$form_state) {
  if (isset($form_state['values']['islandora_genetic_edition_collatex_server_url']) && !filter_var($form_state['values']['islandora_genetic_edition_collatex_server_url'], FILTER_VALIDATE_URL)) {
    form_set_error('islandora_genetic_edition_collatex_server_url', t('CollateX endpoint is not a valid URL.'));
  }
}
