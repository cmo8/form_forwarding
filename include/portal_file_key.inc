<?php
/**
 * Deletes all the files for the given portal from the database.
 * @param string/int $portal_id
 */
function _delete_files($portal_id) {
  $filekids = _get_portal_connections($portal_id);
  foreach ($filekids as $filekid) {
    _delete_connection($filekid);
  }
}
/**
 * Deletes the given file from the database.
 * @param string/array $filekid
 */
function _delete_file($filekid) {
  $tmp = $filekid;
  if(is_array($filekid)){
    $tmp = $filekid['filekid'];
  }
  db_delete('form_forwarding_file_keys')
    ->condition('filekid', $tmp, '=')
    ->execute();
}
/**
 * Updates the given file with the new 3rd party key and drupal element key.
 * @param int $filekid
 * @param string $imagenow_key
 * @param string $drupal_form_element
 * @return int
 */
function _update_file($filekid, $third_party_key, $drupal_form_element) {
  $update_file = db_update('form_forwarding_file_keys')
    ->condition('filekid', $filekid, '=')
    ->fields(
      array(
        'third_party_element_key' => $third_party_key,
        'drupal_element_key' => $drupal_form_element,
      )
    )
    ->execute();
  return $update_file;
}
/**
 * Creates a new file with the given 3rd party key and drupal element key.
 * @param string $portal_id
 * @param string $imagenow_element_key
 * @param string $drupal_element_key
 * @return int $ file
 */
function _create_file($portal_id, $third_party_element_key, $drupal_element_key) {
  $filekid = db_insert('form_forwarding_file_keys')
    ->fields(
      array(
        'inid' => $portal_id,
        'third_party_element_key' => $third_party_element_key,
        'drupal_element_key' => $drupal_element_key,
      )
    )
    ->execute();
  return $filekid;
}
/**
 * Gets the files for the given portal.
 * @param int $inid
 * @return array
 */
function _get_file_keys($inid) {
  $portal_files = [];
  // Get the connections from the Database
  $result = db_select('form_forwarding_file_keys', 'file_keys')
    ->fields('file_keys', array('filekid', 'third_party_element_key', 'drupal_element_key'))
    ->condition('inid', $inid, '=')
    ->orderBy('filekid', 'ASC')
    ->execute();
  // Organize connection elements
  foreach ($result as $row) {
    $row1 = [];
    $row1['filekid'] = $row->filekid;
    $row1['iek'] = $row->third_party_element_key;
    $row1['dek'] = $row->drupal_element_key;
    $portal_files[] = $row1;
  }
  return $portal_files;
}
