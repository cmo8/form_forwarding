<?php

require_once 'inow_connections.inc';

function _delete_portal($portal_id) {
  db_delete('uwpimagenow_portals')
    ->condition('inid', $portal_id, '=')
    ->execute();
}

function _create_portal($drupal_nid, $description, $imagenow_url) {
  $eid = db_insert('uwpimagenow_portals')
    ->fields(
      array(
        'drupal_nid' => $drupal_nid,
        'description' => $description,
        'imagenow_submission_url' => $imagenow_url,
        'date' => REQUEST_TIME,
      )
    )
    ->execute();
  return $eid;
}

function _update_portal($portal_id, $drupal_nid, $description, $imagenow_url) {
  $update_portal = db_update('uwpimagenow_portals')
    ->condition('inid', $portal_id, '=')
    ->fields(
      array(
        'drupal_nid' => $drupal_nid,
        'description' => $description,
        'imagenow_submission_url' => $imagenow_url,
      )
    )
    ->execute();
  return $update_portal;
}

function _is_imagenow_form($nid) {
  $dbNids = NULL;
  // Get the portals from the Database
  $result = db_select('uwpimagenow_portals', 'portals')
    ->fields('portals', array('drupal_nid'))
    ->orderBy('inid', 'ASC')
    ->execute();
  // Organize portal elements
  foreach ($result as $row) {
    $dbNids[] = $row->drupal_nid;
  }
  for ($i = 0; $i < count($dbNids); $i++) {
    if ($nid == $dbNids[$i]) {
      return $dbNids[$i];
    }
  }
  return NULL;
}

function _get_portal_config($cmo, $pid_flag) {
  $portal_config = null;
  $sqlQuery = db_select('uwpimagenow_portals', 'portals')
    ->fields('portals', array('inid', 'drupal_nid', 'description', 'imagenow_submission_url'));
  if ($pid_flag) {
    $sqlQuery->condition('inid', $cmo, '=');
  }
  else {
    $sqlQuery->condition('drupal_nid', $cmo, '=');
  }
  $result = $sqlQuery->orderBy('inid', 'ASC')->execute();
  // Organize portal elements

  foreach ($result as $row) {
    $portal_config['inid'] = $row->inid;
    $portal_config['fid'] = $row->drupal_nid;
    $portal_config['description'] = $row->description;
    $portal_config['imagenow_url'] = $row->imagenow_submission_url;
    $portal_config['connections'] = _get_portal_connections($row->inid);
  }
  return $portal_config;
}

/**
 * Get the drupal to imagenow portals.
 */
function _get_drupal_to_imagenow_portals() {
  $portals = [];
  // Get the portals from the Database
  $result = db_select('uwpimagenow_portals', 'portals')
    ->fields('portals', array('inid', 'drupal_nid', 'description'))
    ->orderBy('inid', 'ASC')
    ->execute();
  // Organize portal elements
  foreach ($result as $row) {
    $row1 = [];
    $row1['inid'] = $row->inid;
    $row1['fid'] = $row->drupal_nid;
    $row1['description'] = $row->description;
    $portals[] = $row1;
  }
  return $portals;
}
