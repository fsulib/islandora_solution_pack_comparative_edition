<?php

/**
 * @file
 * This file contains functions for creating derivatives 
 */

/**
 * Fetches the uploaded image file from Fedora, and saves it to a temp location.
 */
function islandora_genetic_edition_get_uploaded_file(AbstractObject $object) {
  $base_name = str_replace(':', '-', $object->id);
  $mime_detector = new MimeDetect();
  $ext = $mime_detector->getExtension($object['OBJ']->mimeType);
  $filename = file_create_filename("{$base_name}_OBJ.{$ext}", 'temporary://');
  $object['OBJ']->getContent($filename);
  return $filename;
}

/**
 * Creates the TN derived datatstream from the uploaded file.
 *
 * @param AbstractObject $object
 *   The object to add the derived datastream to.
 * @param bool $force
 *   Whether or not we should force the derivatives to be created, in the case
 *   that they already exist on the given object.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_genetic_edition_create_tn_derivative(AbstractObject $object, $force = TRUE) {
  $base_name = str_replace(':', '-', $object->id);
  if ($force || !isset($object['TN'])) {
    $to_return = array(
      'success' => FALSE,
      'messages' => array(),
    );
    $uploaded_file = islandora_genetic_edition_get_uploaded_file($object);
    $args = array();
    $args[] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
    $args[] = '-resize ' . escapeshellarg("200 x 200");
    $derivative_file = islandora_genetic_edition_imagemagick_convert($uploaded_file, "temporary://{$base_name}_TN.jpg", $args);
    if ($derivative_file === FALSE) {
      $to_return['messages'][] = array(
        'message' => t('Failed to create TN derivative.'),
        'type' => 'watchdog',
        'severity' => WATCHDOG_WARNING,
      );
    }
    else {
      $added_successfully = islandora_genetic_edition_add_datastream($object, 'TN', $derivative_file, 'image/jpeg', t('Thumbnail'));
      if (TRUE === $added_successfully) {
        $to_return['messages'][] = array(
          'message' => t('Created TN derivative.'),
          'type' => 'dsm',
          'severity' => 'status',
        );
        $to_return['success'] = TRUE;
      }
      else {
        $to_return['messages'][] = array(
          'message' => t('Failed to add TN derivative to the object. Error message: @message', array('@message' => $added_successfully)),
          'type' => 'dsm',
          'severity' => 'warning',
        );
      }
    }

    file_unmanaged_delete($uploaded_file);
    file_unmanaged_delete($derivative_file);
    return $to_return;
  }
}

/**
 * Calls imagemagick's convert command with the given arguments.
 *
 * @param string $src
 *   The source file uri to convert from.
 * @param string $dest
 *   The destination file uri to convert into.
 *
 * @args int $width
 *   The source file to convert from.
 * @args int $height
 *   The destination file to convert into.
 *
 * @return string
 *   The destination file path if successful otherwise FALSE.
 */
function islandora_genetic_edition_imagemagick_convert($src, $dest, $args) {
  $src = drupal_realpath($src);
  if (islandora_genetic_edition_is_tiff($src)) {
    $src = $src . '[0]';
  }
  $dest = drupal_realpath($dest);
  $context = array(
    'source' => $src,
    'destination' => $dest,
  );
  drupal_alter('imagemagick_arguments', $args, $context);

  // To make use of ImageMagick 6's parenthetical command grouping we need to
  // make the $source image the first parameter and $dest the last.
  // See http://www.imagemagick.org/Usage/basics/#cmdline
  $command = escapeshellarg($src) . ' ' . implode(' ', $args) . ' ' . escapeshellarg($dest);
  $output = array();
  $ret = -1;
  if (_imagemagick_convert_exec($command, $output, $ret) !== TRUE) {
    $variables = array(
      '@ret' => $ret,
      '@command' => $command,
      '!output' => implode('<br/>', $output),
    );
    watchdog('islandora_genetic_edition', 'ImageMagick failed to convert.<br/>Error: @ret<br/>Command: @command <br/>Output !output', $variables, WATCHDOG_ERROR);
    return FALSE;
  }
  return $dest;
}

/**
 * Adds a datastream to an object.
 *
 * @param AbstractObject $object
 *   The object to add a datastream to.
 * @param string $dsid
 *   The datastream ID to be added.
 * @param string $file
 *   The URI of the file to be added as the datastream content.
 * @param string $mimetype
 *   The mimetype of the file.
 * @param string $label
 *   The label for the datastream.
 *
 * @return bool|string
 *   TRUE if the datastream was added/modified successfully, the error message
 *   otherwise.
 */
function islandora_genetic_edition_add_datastream(AbstractObject $object, $dsid, $file, $mimetype, $label) {
  try {
    $ingest = !isset($object[$dsid]);
    if ($ingest) {
      $ds = $object->constructDatastream($dsid, 'M');
      $ds->label = $label;
      $ds->mimeType = $mimetype;
    }
    else {
      $ds = $object[$dsid];
    }
    $ds->setContentFromFile($file);
    if ($ingest) {
      $object->ingestDatastream($ds);
    }
    return TRUE;
  }
  catch (exception $e) {
    $variables = array(
      '@ret' => $e->getTraceAsString(),
    );
    watchdog('islandora_genetic_edition',
      'Genetic Edition failed to ingest data stream.<br/>Stack: @ret',
      $variables,
      WATCHDOG_ERROR);
    $message = $e->getMessage();
    return $message;
  }
}

/**
 * Uses Imagemagick's identify to determine if the file is a TIFF.
 *
 * @param string $file
 *   A file-system path to the file in question.
 *
 * @return bool
 *   A boolean indicating if the file contains a TIFF.
 */
function islandora_genetic_edition_is_tiff($file) {
  $identify = islandora_genetic_edition_get_identify();
  $file = escapeshellarg(drupal_realpath($file));
  $codec = exec(escapeshellcmd("$identify -format \"%m\" $file"));
  $is_tiff = strrpos(strtolower($codec), 'tiff');
  if ($is_tiff !== FALSE) {
    $is_tiff = TRUE;
  }
  return $is_tiff;
}

/**
 * Attempt to get the "identify" executable...
 *
 * Assume convert is sanely named, and that identify can be obtained from the
 * same place.
 *
 * @return string
 *   The command to run for "identify".
 */
function islandora_genetic_edition_get_identify() {
  $convert = variable_get('imagemagick_convert', 'convert');
  $identify = str_replace('convert', 'identify', $convert);
  return $identify;
}
