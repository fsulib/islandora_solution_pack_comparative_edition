<?php

/**
 * @file
 * This file contains functions to create breadcrumbs on comparative witness and page objects.
 */

function _get_comparative_parent($object) {
  $parent_result = $object->relationships->get(FEDORA_RELS_EXT_URI, 'isMemberOf');
  $parent_pid = $parent_result[0]['object']['value'];
  $parent_object = islandora_object_load($parent_pid);
  return $parent_object;
}

function _get_collection_parent($object) {
  $parent_array = $object->getParents();
  $parent_pid = $parent_array[0];
  $parent_object = islandora_object_load($parent_pid);
  return $parent_object;
}

function _create_breadcrumb_html($object){
  $path = drupal_encode_path("/islandora/object/{$object->id}");
  $html = "<a href=\"{$path}\">{$object->label}</a>";
  return $html;
}
