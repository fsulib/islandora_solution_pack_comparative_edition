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

  public function ingest($parameters, $files) {

    module_load_include('inc', 'islandora', 'includes/utilities');
    $parent_id = $parameters['parent'];
    $parent_namespace = islandora_get_namespace($parent_id);

    $connection = islandora_get_tuque_connection();
    $repository = $connection->repository;
    $witness_object = $repository->constructObject($parent_namespace);
    $witness_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isMemberOf', $parent_id);
    $witness_object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:comparativeEditionWitnessCModel');

    // Construct MODS datastream
    $mods_datastream = $witness_object->constructDatastream('MODS');
    $mods_datastream->label = 'MODS';
    $mods_datastream->mimeType = 'application/xml';
    $mods_datastream->setContentFromFile($files['mods']);
    $witness_object->ingestDatastream($mods_datastream);
   
    $witness_object->label = simplexml_load_file($files['mods'])->titleInfo->title;

    // Construct OBJ datastream
    $obj_datastream = $witness_object->constructDatastream('OBJ');
    $obj_datastream->label = 'OBJ';
    $obj_datastream->mimeType = mime_content_type($files['obj']); 
    $obj_datastream->setContentFromFile($files['obj']);
    $witness_object->ingestDatastream($obj_datastream);

    // Construct DISPLAY-CSS datastream
    $css_datastream = $witness_object->constructDatastream('DISPLAY-CSS');
    $css_datastream->label = 'DISPLAY-CSS';
    $css_datastream->mimeType = 'text/css'; 
    $css_datastream->setContentFromFile($files['css']);
    $witness_object->ingestDatastream($css_datastream);

    $witness_id = $repository->ingestObject($witness_object)->id;

    // Construct page objects
    foreach ($files['pages'] as $page) {
      $page_object = new IslandoraComparativeEditionPageObject();
      $page_object->ingest($page, $witness_id);
    }

  }

}

class IslandoraComparativeEditionPageObject { 

  public static function getParent($pid) {
  }

  public function ingest($files, $parent_id) {
    $connection = islandora_get_tuque_connection();
    $repository = $connection->repository;

    $namespace = explode(":", $parent_id);
    $namespace = $namespace[0];

    $page_object = $repository->constructObject($namespace);
    $page_object->relationships->add(ISLANDORA_RELS_EXT_URI, 'isMemberOf', $parent_id);
    $page_object->relationships->add(FEDORA_MODEL_URI, 'hasModel', 'islandora:comparativeEditionPageCModel');

    // Construct MODS datastream
    $mods_datastream = $page_object->constructDatastream('MODS');
    $mods_datastream->label = 'MODS';
    $mods_datastream->mimeType = 'application/xml';
    $mods_datastream->setContentFromFile($files['mods']);
    $page_object->ingestDatastream($mods_datastream);
   
    $page_object->label = simplexml_load_file($files['mods'])->titleInfo->title;

    // Construct OBJ datastream 
    $obj_datastream = $page_object->constructDatastream('OBJ');
    $obj_datastream->label = 'OBJ';
    $obj_datastream->mimeType = mime_content_type($files['obj']); 
    $obj_datastream->setContentFromFile($files['obj']);
    $page_object->ingestDatastream($obj_datastream);

    // Construct DISPLAY-HTML datastream 
    $html_datastream = $page_object->constructDatastream('DISPLAY-HTML');
    $html_datastream->label = 'DISPLAY-HTML';
    $html_datastream->mimeType = 'text/html'; 
    $html_datastream->setContentFromFile($files['html']);
    $page_object->ingestDatastream($html_datastream);

    // Construct CONTENT-XML datastream 
    $xml_datastream = $page_object->constructDatastream('CONTENT-XML');
    $xml_datastream->label = 'CONTENT-XML';
    $xml_datastream->mimeType = 'application/xml'; 
    $xml_datastream->setContentFromFile($files['xml']);
    $page_object->ingestDatastream($xml_datastream);

    // Construct PINDEX datastream 
    $pindex_datastream = $page_object->constructDatastream('PINDEX');
    $pindex_datastream->label = 'PINDEX';
    $pindex_datastream->mimeType = 'text/plain'; 
    $pindex_datastream->setContentFromFile($files['pindex']);
    $page_object->ingestDatastream($pindex_datastream);


    $page_id = $repository->ingestObject($page_object)->id;
  }

}

class IslandoraComparativeEditionWitnessIngestPackage { 

  public function validate($witness_url) {

    $validity = array(
      'valid' => TRUE, 
      'msg' => "The submitted witness package could not be ingested because it was missing the following parts:", 
      'files' => array(),
    ); 

    if (!is_dir($witness_url)) {
      $validity['valid'] = FALSE;
      $validity['msg'] = "<br/>- Error with unzipped witness at {$witness_url}."; 
      return $validity;
    }
    else {
      $witness_files = array();

      // Validate existence of witness MODS record
      $witness_files['mods'] = glob("{$witness_url}/*.xml");
      if (@file_exists($witness_files['mods'][0])) {
        $validity['files']['mods'] = $witness_files['mods'][0];
      }
      else {
        $validity['valid'] = FALSE;
        $validity['msg'] .= "<br/>- missing witness MODS record";
      }

      // Validate existence of witness image file 
      $witness_files['obj'] = glob("{$witness_url}/*.{jpg,jpeg,png,tiff}", GLOB_BRACE);
      if (@file_exists($witness_files['obj'][0])) {
        $validity['files']['obj'] = $witness_files['obj'][0];
      }
      else {
        $validity['valid'] = FALSE;
        $validity['msg'] .= "<br/>- missing witness image file";
      }

      // Validate existence of witness css file 
      $witness_files['css'] = glob("{$witness_url}/*.css");
      if (@file_exists($witness_files['css'][0])) {
        $validity['files']['css'] = $witness_files['css'][0];
      }
      else {
        $validity['valid'] = FALSE;
        $validity['msg'] .= "<br/>- missing witness css file";
      }

      $page_files = glob("{$witness_url}/pages/*");
      $pages = array();

      foreach ($page_files as $page_file) {
        $page_file_parts = explode(".", $page_file);
        $page_file_parts = explode("/", $page_file_parts[0]);
        $pages[] = end($page_file_parts);
      }

      $pages = array_unique($pages);
      foreach ($pages as $page) {

        // Validate existence of page MODS record
        $witness_files['pages'][$page]['mods'] = "{$witness_url}/pages/{$page}.mods";
        if (@file_exists($witness_files['pages'][$page]['mods'])) {
          $validity['files']['pages'][$page]['mods'] = $witness_files['pages'][$page]['mods'];
        }
        else {
          $validity['valid'] = FALSE;
          $validity['msg'] .= "<br/>- missing page MODS record from {$page} (this must have the file extension '.mods')";
        }

        // Validate existence of page XML file 
        $witness_files['pages'][$page]['xml'] = "{$witness_url}/pages/{$page}.xml";
        if (@file_exists($witness_files['pages'][$page]['xml'])) {
          $validity['files']['pages'][$page]['xml'] = $witness_files['pages'][$page]['xml'];
        }
        else {
          $validity['valid'] = FALSE;
          $validity['msg'] .= "<br/>- missing page content XML file from {$page}";
        }

        // Validate existence of page HTML file 
        $witness_files['pages'][$page]['html'] = "{$witness_url}/pages/{$page}.html";
        if (@file_exists($witness_files['pages'][$page]['html'])) {
          $validity['files']['pages'][$page]['html'] = $witness_files['pages'][$page]['html'];
        }
        else {
          $validity['valid'] = FALSE;
          $validity['msg'] .= "<br/>- missing page HTML file from {$page}";
        }

        // Validate existence of page PINDEX file 
        $witness_files['pages'][$page]['pindex'] = "{$witness_url}/pages/{$page}.txt";
        if (@file_exists($witness_files['pages'][$page]['pindex'])) {
          $validity['files']['pages'][$page]['pindex'] = $witness_files['pages'][$page]['pindex'];
        }
        else {
          $validity['valid'] = FALSE;
          $validity['msg'] .= "<br/>- missing page PINDEX file from {$page}";
        }

        // Validate existence of page image file 
        $witness_files['pages'][$page]['obj'] = glob("{$witness_url}/pages/{$page}*.{jpg,jpeg,png,tiff}", GLOB_BRACE);
        if (@file_exists($witness_files['pages'][$page]['obj'][0])) {
          $validity['files']['pages'][$page]['obj'] = $witness_files['pages'][$page]['obj'][0];
        }
        else {
          $validity['valid'] = FALSE;
          $validity['msg'] .= "<br/>- missing page image file from {$page}";
        }

      }

    }

    return $validity;
  }

  public function ingest($form_state) {
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
      $witness_object = new IslandoraComparativeEditionWitnessObject();
      $witness_object->ingest($parameters, $witness['files']);
    }
    else {
      drupal_set_message($witness['msg'], 'error');
    }

    array_map('unlink', glob("$witness_url/pages/{*.*,.DS_Store}", GLOB_BRACE));
    rmdir("$witness_url/pages");
    array_map('unlink', glob("$witness_url/{*.*,.DS_Store}", GLOB_BRACE));
    rmdir($witness_url);
    $deletion = file_delete($target);
  }

}
