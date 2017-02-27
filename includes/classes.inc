<?php

/**
 * @file
 * Implementation of classes for the comparative edition solution pack. 
 */

class IslandoraComparativeEditionObject { 

  public static function getChildren($pid) {
  }

}

class IslandoraComparativeEditionWitnessObject { 

  public static function getParent($pid) {
  }

  public static function getChildren($pid) {
  }

  public function ingest($connection, $parameters, $tmpdir) {

    // Get parent namespace
    module_load_include('inc', 'islandora', 'includes/utilities');
    $parent_id = $parameters['parent'];
    $parent_namespace = islandora_get_namespace($parent_id);

    $repository = $connection->repository;
    $witness_object = $repository->constructObject($parent_namespace);
    $witness_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isMemberOf', $parent_id);
    $witness_object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:comparativeEditionWitnessCModel');

    // Construct TEI DS
    $tei_datastream = $witness_object->constructDatastream('TEI');
    $tei_datastream->label = 'TEI';
    $tei_datastream->mimeType = 'application/xml';
    $tei_datastream->setContentFromFile("{$tmpdir}/tei.xml");
    $witness_object->ingestDatastream($tei_datastream);

    // Construct MODS DS from TEI DS
    $mods = self::constructModsFromTei($witness_object['TEI']->content);
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

    $witness_id = $repository->ingestObject($witness_object)->id;
    watchdog('gesp', "New witness ingested at {$witness_id}");

    // Construct page objects
    $facsdir = $tmpdir . "/facsimiles";
    $images = array_diff(scandir($facsdir), array('.', '..'));
    foreach ($images as $image) {
      $page_object = new IslandoraComparativeEditionPageObject();
      $page_object->ingest($image, $parent_namespace, $witness_id, $repository, $facsdir);
    }
  }

  public static function constructModsFromTei($tei) {
   
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

}

class IslandoraComparativeEditionPageObject { 

  public static function getParent($pid) {
  }

  public function ingest($image, $parent_namespace, $witness_id, $repository, $facsdir) {
    $page_object = $repository->constructObject($parent_namespace);
    $page_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isMemberOf', $witness_id);
    $page_object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:comparativeEditionPageCModel');

    // Construct OBJ DS
    $obj_datastream = $page_object->constructDatastream('OBJ');
    $obj_datastream->label = 'OBJ';
    $obj_datastream->mimeType = 'image/png';
    $obj_datastream->setContentFromFile("{$facsdir}/{$image}");
    $page_object->ingestDatastream($obj_datastream);

    $page_id = $repository->ingestObject($page_object)->id;
    watchdog('gesp', "New page ingested at {$page_id}");
  }

}

class IslandoraComparativeEditionWitnessIngestPackage { 

  public function validate($witness_url) {
    if (!is_dir($witness_url)) {
      return array('valid' => FALSE, 'msg' => "Error with unzipped witness at {$witness_url}."); 
    }
    else {
      $witness_files = array();
      $witness_files['mods'] = glob("{$witness_url}/*.xml");
      $witness_files['obj'] = glob("{$witness_url}/*.{jpg,jpeg,png,tiff}", GLOB_BRACE);
      $witness_files['css'] = glob("{$witness_url}/*.css");

      $page_files = glob("{$witness_url}/pages/*");
      $pages = array();
      foreach ($page_files as $page_file) {
	$page_file_parts = explode(".", $page_file);
	$page_file_parts = explode("/", $page_file_parts[0]);
        $pages[] = end($page_file_parts);
      }
      $pages = array_unique($pages);
      foreach ($pages as $page) {
        $witness_files['pages'][$page]['mods'] = "{$witness_url}/pages/{$page}.mods";
        $witness_files['pages'][$page]['content-xml'] = "{$witness_url}/pages/{$page}.xml";
        $witness_files['pages'][$page]['display-html'] = "{$witness_url}/pages/{$page}.html";
        $witness_files['pages'][$page]['pindex'] = "{$witness_url}/pages/{$page}.txt";
        $witness_files['pages'][$page]['obj'] = glob("{$witness_url}/pages/{$page}*.{jpg,jpeg,png,tiff}", GLOB_BRACE);

      }

    }

    $validation_message = "This is a test";
    return array('valid' => TRUE, 'msg' => $validation_message, 'files' => $witness_files);

  }

  public function ingest($form_state) {
    $connection = islandora_get_tuque_connection();
    $target = file_load($form_state['values']['target']);
    $parameters = $form_state['values'] + array(
      'zip_file_uri' => $target->uri,
      'type' => 'zip',
      'parent_relationship_uri' => 'info:fedora/fedora-system:def/relations-external#',
      'parent_relationship_pred' => 'isMemberOf',
      'wait_for_metadata' => FALSE,
      'directory_dedup' => FALSE,
      'content_models' => array('islandora:comparativeEditionWitnessCModel'),
    );
    $file_url = drupal_realpath($parameters['zip_file_uri']);
    $witness_url = variable_get('file_temporary_path', '/tmp') . "/cesp-witness";
    $zip = new ZipArchive;
    $response = $zip->open($file_url);
    if ($response === TRUE) {
      $zip->extractTo($witness_url);
      $zip->close();
    } else {
      watchdog('Islandora Solution Pack Comparative Edition', "Error extracting witness ingest zip package to tmp directory: '$witness_url'");
      break;
    }

    $witness = $this->validate($witness_url);
    if ($witness['valid']) {
      watchdog('cesp', json_encode($witness));
      //$witness_object = new IslandoraComparativeEditionWitnessObject();
      //$witness_object->ingest($witness['files']);
    }
    else {
      drupal_set_message("The submitted witness package could not be ingested because it was missing the following parts: {$witness['msg']}", 'error');
    }


    $deletion = file_delete($target);
  }

}
