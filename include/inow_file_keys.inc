<?php

function _delete_files($portal_id) {
  $fileids = _get_portal_connections($portal_id);
  foreach ($fileids as $fileid) {
    _delete_connection($fileid);
  }
}

function _delete_file($fileid) {
  db_delete('uwpimagenow_file_keys')
    ->condition('fileid', $fileid['fileid'], '=')
    ->execute();
}

function _update_file($fileid, $imagenow_key, $drupal_form_element) {
  $update_file = db_update('uwpimagenow_file_keys')
    ->condition('fileid', $fileid, '=')
    ->fields(
      array(
        'imagenow_element_key' => $imagenow_key,
        'drupal_element_key' => $drupal_form_element,
      )
    )
    ->execute();
  return $update_file;
}

function _create_file($portal_id, $imagenow_element_key, $drupal_element_key) {
  $fileid = db_insert('uwpimagenow_file_keys')
    ->fields(
      array(
        'inid' => $portal_id,
        'imagenow_element_key' => $imagenow_element_key,
        'drupal_element_key' => $drupal_element_key,
      )
    )
    ->execute();
  return $fileid;
}

function _get_portal_files($inid) {
  $portal_files = [];
  // Get the connections from the Database
  $result = db_select('uwpimagenow_file_keys', 'file_keys')
    ->fields('file_keys', array('fileid', 'imagenow_element_key', 'drupal_element_key'))
    ->condition('inid', $inid, '=')
    ->orderBy('fileid', 'ASC')
    ->execute();
  // Organize connection elements
  foreach ($result as $row) {
    $row1 = [];
    $row1['fileid'] = $row->fileid;
    $row1['iek'] = $row->imagenow_element_key;
    $row1['dek'] = $row->drupal_element_key;
    $portal_files[] = $row1;
  }
  return $portal_files;
}
