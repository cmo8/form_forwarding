<?php

function _delete_files($portal_id) {
  $filekids = _get_portal_connections($portal_id);
  foreach ($filekids as $filekid) {
    _delete_connection($filekid);
  }
}

function _delete_file($filekid) {
  db_delete('uwpimagenow_file_keys')
    ->condition('filekid', $filekid['filekid'], '=')
    ->execute();
}

function _update_file($filekid, $imagenow_key, $drupal_form_element) {
  $update_file = db_update('uwpimagenow_file_keys')
    ->condition('filekid', $filekid, '=')
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
  $filekid = db_insert('uwpimagenow_file_keys')
    ->fields(
      array(
        'inid' => $portal_id,
        'imagenow_element_key' => $imagenow_element_key,
        'drupal_element_key' => $drupal_element_key,
      )
    )
    ->execute();
  return $filekid;
}

function _get_file_keys($inid) {
  $portal_files = [];
  // Get the connections from the Database
  $result = db_select('uwpimagenow_file_keys', 'file_keys')
    ->fields('file_keys', array('filekid', 'imagenow_element_key', 'drupal_element_key'))
    ->condition('inid', $inid, '=')
    ->orderBy('filekid', 'ASC')
    ->execute();
  // Organize connection elements
  foreach ($result as $row) {
    $row1 = [];
    $row1['filekid'] = $row->filekid;
    $row1['iek'] = $row->imagenow_element_key;
    $row1['dek'] = $row->drupal_element_key;
    $portal_files[] = $row1;
  }
  return $portal_files;
}
