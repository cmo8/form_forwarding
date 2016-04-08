<?php
/**
 * Deletes all the connections for the given portal from the database.
 * @param string/int $portal_id
 */
function _delete_connections($portal_id) {
  $eids = _get_portal_connections($portal_id);
  foreach ($eids as $eid) {
    _delete_connection($eid);
  }
}

/**
 * Deletes the given connection from the database.
 * @param string/array $eid
 */
function _delete_connection($eid) {
  $tmp = $eid;
  if(is_array($eid)){
    $tmp = $eid['eid'];
  }
  db_delete('uwpimagenow_connections')
    ->condition('eid', $tmp, '=')
    ->execute();
}

/**
 * Updates the given connection with the new  drupal key and 3rd party key.
 * @param int $eid
 * @param string $drupal_form_element
 * @param string $imagenow_key
 * @return int
 */
function _update_connection($eid, $drupal_form_element, $imagenow_key) {
  $update_connection = db_update('uwpimagenow_connections')
    ->condition('eid', $eid, '=')
    ->fields(
      array(
        'drupal_element_key' => $drupal_form_element,
        'imagenow_element_key' => $imagenow_key,
      )
    )
    ->execute();
  return $update_connection;
}

/**
 * Creates a new connection with the given drupal key and 3rd party key.
 * @param int $inid
 * @param string $drupal_element_key
 * @param string $imagenow_element_key
 * @return int
 */
function _create_connection($inid, $drupal_element_key, $imagenow_element_key) {
  $eid = db_insert('uwpimagenow_connections')
    ->fields(
      array(
        'inid' => $inid,
        'drupal_element_key' => $drupal_element_key,
        'imagenow_element_key' => $imagenow_element_key,
      )
    )
    ->execute();
  return $eid;
}

/**
 * Gets the connections for the given portal.
 * @param int $inid
 * @return array
 */
function _get_portal_connections($inid) {
  $portal_connections = [];
  // Get the connections from the Database
  $result = db_select('uwpimagenow_connections', 'connections')
    ->fields('connections', array('eid', 'drupal_element_key', 'imagenow_element_key'))
    ->condition('inid', $inid, '=')
    ->orderBy('eid', 'ASC')
    ->execute();
  // Organize connection elements
  foreach ($result as $row) {
    $row1 = [];
    $row1['eid'] = $row->eid;
    $row1['dek'] = $row->drupal_element_key;
    $row1['iek'] = $row->imagenow_element_key;
    $portal_connections[] = $row1;
  }
  return $portal_connections;
}

//function _is_connection_exist($inid, $drupal_element_key, $imagenow_element_key) {
//  $result = db_select('uwpimagenow_connections', 'connections')
//    ->fields('connections', array('eid', 'drupal_element_key', 'imagenow_element_key'))
//    ->condition('inid', $inid, '=')
//    ->condition(db_or()->condition('drupal_element_key', $drupal_element_key, '=')
//      ->condition('imagenow_element_key', $imagenow_element_key, '='))
//    ->orderBy('eid', 'ASC')
//    ->execute();
//  // Organize connection elements
//  foreach ($result as $row) {
//    if($row->drupal_element_key == $drupal_element_key || $row->imagenow_element_key == $imagenow_element_key){
//      return TRUE;
//    }
//  }
//  return FALSE;
//}