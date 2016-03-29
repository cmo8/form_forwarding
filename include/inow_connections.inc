<?php

function _delete_connections($portal_id) {
  $eids = _get_portal_connections($portal_id);
  foreach ($eids as $eid) {
    _delete_connection($eid);
  }
}

function _delete_connection($eid) {
  $tmp = $eid;
  if(is_array($eid)){
    $tmp = $eid['eid'];
  }
  db_delete('uwpimagenow_connections')
    ->condition('eid', $tmp, '=')
    ->execute();
}

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

function _create_connection($portal_id, $drupal_element_key, $imagenow_element_key) {
  $eid = db_insert('uwpimagenow_connections')
    ->fields(
      array(
        'inid' => $portal_id,
        'drupal_element_key' => $drupal_element_key,
        'imagenow_element_key' => $imagenow_element_key,
      )
    )
    ->execute();
  return $eid;
}

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
