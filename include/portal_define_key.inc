<?php
/**
 * Deletes all the defines for the given portal from the database.
 * @param string/int $portal_id
 */
function _delete_defines($portal_id) {
  $dkids = _get_portal_connections($portal_id);
  foreach ($dkids as $dkid) {
    _delete_connection($dkid);
  }
}
/**
 * Deletes the given define from the database.
 * @param string/array $dkid
 */
function _delete_define($dkid) {
  $tmp = $dkid;
  if(is_array($dkid)){
    $tmp = $dkid['dkid'];
  }
  db_delete('form_forwarding_define_keys')
    ->condition('dkid', $tmp, '=')
    ->execute();
}
/**
 * Updates the given define with the new 3rd party key and 3rd party value.
 * @param int $dkid
 * @param string $imagenow_key
 * @param string $imagenow_value
 * @return int
 */
function _update_define($dkid, $imagenow_key, $imagenow_value) {
  $update_define_key = db_update('form_forwarding_define_keys')
    ->condition('dkid', $dkid, '=')
    ->fields(
      array(
        'third_party_element_key' => $imagenow_key,
        'third_party_element_value' => $imagenow_value,
      )
    )
    ->execute();
  return $update_define_key;
}
/**
 * Creates a new define with the given 3rd party key and 3rd party value.
 * @param string $portal_id
 * @param string $imagenow_key
 * @param string $imagenow_value
 * @return int $dkid
 */
function _create_define($portal_id, $third_party_key, $third_party_value) {
    $dkid = db_insert('form_forwarding_define_keys')
    ->fields(
      array(
        'inid' => $portal_id,
        'third_party_element_key' => $third_party_key,
        'third_party_element_value' => $third_party_value,
      )
    )
    ->execute();
  return $dkid;
}
/**
 * Gets the defines for the given portal.
 * @param int $inid
 * @return array
 */
function _get_define_keys($inid) {
  $portal_defines = [];
  // Get the connections from the Database
  $result = db_select('uwpimagenow_define_keys', 'define_keys')
    ->fields('define_keys', array('dkid', 'third_party_element_key', 'third_party_element_value'))
    ->condition('inid', $inid, '=')
    ->orderBy('dkid', 'ASC')
    ->execute();
  // Organize connection elements
  foreach ($result as $row) {
    $row1 = [];
    $row1['dkid'] = $row->dkid;
    $row1['ikey'] = $row->third_party_element_key;
    $row1['ivalue'] = $row->third_party_element_value;
    $portal_defines[] = $row1;
  }
  return $portal_defines;
}
