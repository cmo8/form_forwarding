<?php

function _delete_defines($portal_id) {
  $dkids = _get_portal_connections($portal_id);
  foreach ($dkids as $dkid) {
    _delete_connection($dkid);
  }
}

function _delete_define($dkid) {
  $tmp = $dkid;
  if(is_array($dkid)){
    $tmp = $dkid['dkid'];
  }
  db_delete('uwpimagenow_define_keys')
    ->condition('dkid', $tmp, '=')
    ->execute();
}

function _update_define($dkid, $imagenow_key, $imagenow_value) {
  $update_define_key = db_update('uwpimagenow_define_keys')
    ->condition('dkid', $dkid, '=')
    ->fields(
      array(
        'imagenow_element_key' => $imagenow_key,
        'imagenow_element_value' => $imagenow_value,
      )
    )
    ->execute();
  return $update_define_key;
}

function _create_define($portal_id, $imagenow_key, $imagenow_value) {
    $dkid = db_insert('uwpimagenow_define_keys')
    ->fields(
      array(
        'inid' => $portal_id,
        'imagenow_element_key' => $imagenow_key,
        'imagenow_element_value' => $imagenow_value,
      )
    )
    ->execute();
  return $dkid;
}

function _get_define_keys($inid) {
  $portal_defines = [];
  // Get the connections from the Database
  $result = db_select('uwpimagenow_define_keys', 'define_keys')
    ->fields('define_keys', array('dkid', 'imagenow_element_key', 'imagenow_element_value'))
    ->condition('inid', $inid, '=')
    ->orderBy('dkid', 'ASC')
    ->execute();
  // Organize connection elements
  foreach ($result as $row) {
    $row1 = [];
    $row1['dkid'] = $row->dkid;
    $row1['ikey'] = $row->imagenow_element_key;
    $row1['ivalue'] = $row->imagenow_element_value;
    $portal_defines[] = $row1;
  }
  return $portal_defines;
}
