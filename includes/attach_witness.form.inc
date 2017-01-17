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
  $tmpdir = variable_get('file_temporary_path', '/tmp') . "/witness_ingest";

  $zip = new ZipArchive;
  $response = $zip->open($file_url);
  if ($response === TRUE) {
    $zip->extractTo($tmpdir);
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
  $witness_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isMemberOf', $parent_id);
  $witness_object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:geneticEditionWitnessCModel');

  // Construct TEI DS
  $tei_datastream = $witness_object->constructDatastream('TEI');
  $tei_datastream->label = 'TEI';
  $tei_datastream->mimeType = 'application/xml';
  $tei_datastream->setContentFromFile("{$tmpdir}/tei.xml");
  $witness_object->ingestDatastream($tei_datastream);

  // Construct MODS DS from TEI DS
  $mods = islandora_genetic_edition_construct_mods_from_tei($witness_object['TEI']->content);
  $mods_datastream = $witness_object->constructDatastream('MODS');
  $mods_datastream->label = 'MODS';
  $mods_datastream->mimeType = 'application/xml';
  $mods_datastream->setContentFromString($mods);
  $witness_object->ingestDatastream($mods_datastream);
 
  $witness_object->label = simplexml_load_string($mods)->titleInfo->title;

  // Construct OBJ DS
  $obj_datastream = $witness_object->constructDatastream('OBJ');
  $obj_datastream->label = 'OBJ';
  $obj_datastream->mimeType = 'image/png';
  $obj_datastream->setContentFromFile("{$tmpdir}/obj.png");
  $witness_object->ingestDatastream($obj_datastream);

  // TEMPORARY! Construct TN DS
  $tn_datastream = $witness_object->constructDatastream('TN');
  $tn_datastream->label = 'TN';
  $tn_datastream->mimeType = 'image/png';
  $tn_datastream->setContentFromFile("{$tmpdir}/obj.png");
  $witness_object->ingestDatastream($tn_datastream);

  $witness_id = $repository->ingestObject($witness_object)->id;
  watchdog('gesp', "New witness ingested at {$witness_id}");

  // Construct page objects
  $facsdir = $tmpdir . "/facsimiles";
  $pages = array_diff(scandir($facsdir), array('.', '..'));
  foreach ($pages as $page) {

    $page_object = $repository->constructObject($parent_namespace);
    $page_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isMemberOf', $witness_id);
    $page_object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:geneticEditionPageCModel');

    // Construct OBJ DS
    $obj_datastream = $page_object->constructDatastream('OBJ');
    $obj_datastream->label = 'OBJ';
    $obj_datastream->mimeType = 'image/png';
    $obj_datastream->setContentFromFile("{$facsdir}/{$page}");
    $page_object->ingestDatastream($obj_datastream);

    // TEMPORARY! Construct TN DS
    $tn_datastream = $page_object->constructDatastream('TN');
    $tn_datastream->label = 'TN';
    $tn_datastream->mimeType = 'image/png';
    $tn_datastream->setContentFromFile("{$facsdir}/{$page}");
    $page_object->ingestDatastream($tn_datastream);

    $page_id = $repository->ingestObject($page_object)->id;
    watchdog('gesp', "New page ingested at {$page_id}");

  }
  
}

function islandora_genetic_edition_construct_mods_from_tei($tei) {
 
  // Extract TEI metadata
  $tei_obj = simplexml_load_string($tei);
  $title = $tei_obj->teiHeader->fileDesc->sourceDesc->biblFull->titleStmt->title;
  $author_first_name = $tei_obj->teiHeader->fileDesc->sourceDesc->biblFull->titleStmt->author->forename;
  $author_middle_name = $tei_obj->teiHeader->fileDesc->sourceDesc->biblFull->titleStmt->author->addName;
  if (!empty($author_middle_name)) {
    $author_first_name = "{$author_first_name} {$author_middle_name}";
  }
  $author_last_name = $tei_obj->teiHeader->fileDesc->sourceDesc->biblFull->titleStmt->author->surname;
  $abstract = $tei_obj->teiHeader->profileDesc->abstract->p;

  // Construct MODS record
  $mods = new SimpleXMLElement('<mods xmlns="http://www.loc.gov/mods/v3" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:mods="http://www.loc.gov/mods/v3" xmlns:dcterms="http://purl.org/dc/terms/" xmlns:etd="http://www.ndltd.org/standards/metadata/etdms/1.0/" xmlns:flvc="info:flvc/manifest/v1" xsi:schemaLocation="http://www.loc.gov/standards/mods/v3/mods-3-4.xsd" version="3.4"></mods>');

  // Title
  $mods->addChild('titleInfo');
  $mods->titleInfo->addAttribute('lang', 'eng');
  $mods->titleInfo->addChild('title', htmlspecialchars($title));
  
  // Author
  $name = $mods->addChild('name');
  $name->addAttribute('type', 'personal');
  $name->addAttribute('authority', 'local');
  $name->addChild('namePart', htmlspecialchars($author_first_name))->addAttribute('type', 'given');
  $name->addChild('namePart', htmlspecialchars($author_last_name))->addAttribute('type', 'family');
  
  // Abstract
  $mods->addChild('abstract', $abstract);

  return $mods->asXML();
}
