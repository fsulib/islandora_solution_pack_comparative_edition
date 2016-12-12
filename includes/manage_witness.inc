<?php

/**
 * @file
 * Provides the management interface for genetic edition witness objects.
 */

require_once dirname(__FILE__) . '/utilities.inc';

/**
 * Local menu action that gets the ingest genetic edition page form.
 *
 * @global $user
 *
 * @param AbstractObject $witness
 *   The genetic edition witness to ingest into.
 *
 * @return sting
 *   The HTML repersentation of the ingest page form.
 */
function islandora_genetic_edition_ingest_page(AbstractObject $witness) {
  global $user;
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/ingest.form');
  $tuque = islandora_get_tuque_connection();
  $page = $tuque->repository->constructObject(islandora_get_namespace($witness->id));
  $page->owner = $user->name;
  $page->label = 'New Page';
  $page->models = 'islandora:geneticEditionPageCModel';
  $genetic_edition = islandora_object_load(islandora_genetic_edition_get_genetic_edition($witness));
  drupal_set_title(t('Add page to @genetic_edition witness @witness', array('@genetic_edition' => $genetic_edition->label, '@witness' => $witness->label)));
  return drupal_get_form('islandora_ingest_form', array(
    'models' => array('islandora:geneticEditionPageCModel'),
    'witness' => $witness,
    'objects' => array($page),
    'parent' => $witness->id,
  ));
}

