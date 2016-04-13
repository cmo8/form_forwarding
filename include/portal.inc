<?php

require_once 'portal_connection.inc';
require_once 'portal_define_key.inc';
require_once 'portal_file_key.inc';

/**
 * Deletes the given portal from the database.
 * @param string/array $eid
 */
function _delete_portal($portal_id) {
  _delete_defines($portal_id);
  _delete_connections($portal_id);
  _delete_files($portal_id);
  db_delete('form_forwarding_portals')
    ->condition('inid', $portal_id, '=')
    ->execute();
}

/**
 * Creates a new portal with the given drupal node id, description, and 3rd party url.
 * @param string $drupal_nid
 * @param string $description
 * @param string $submission_url
 * @param string    $group
 * @return int $eid
 */
function _create_portal($drupal_nid, $description, $submission_url, $group) {
  $eid = db_insert('form_forwarding_portals')
    ->fields(array('drupal_nid', 'description', 'submission_url', 'date', 'group_id',))
    ->values(
        array(
          'drupal_nid' => $drupal_nid,
          'description' => $description,
          'submission_url' => $submission_url,
          'date' => REQUEST_TIME,
          'group_id' => $group,
        )
      )
    ->execute();
  return $eid;
}

/**
 * Updates the given portal with the new drupal node id, description, and 3rd party url.
 * @param int/string $portal_id
 * @param string $drupal_nid
 * @param string $description
 * @param string $submission_url
 * @return int
 */
function _update_portal($portal_id, $drupal_nid, $description, $submission_url) {//TODO: Group
  $update_portal = db_update('form_forwarding_portals')
    ->condition('inid', $portal_id, '=')
    ->fields(
      array(
        'drupal_nid' => $drupal_nid,
        'description' => $description,
        'submission_url' => $submission_url,
        //'group_id' => $group,
      )
    )
    ->execute();
  return $update_portal;
}

/**
 * Checks if the form is an imagenow form.
 * @param int $nid
 * @return type
 */
function _is_imagenow_form($nid) {
  $dbNids = NULL;
  // Get the portals from the Database
  $result = db_select('form_forwarding_portals', 'portals')
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

/**
 * Gets the portal for the portal connection or the drupal node id.
 * @param string $inid
 * @param bool $pid_flag
 * @return array
 */
function _get_portal_config($inid, $pid_flag) {
  $portal_config = null;
  if (is_numeric($inid)) {
    $sqlQuery = db_select('form_forwarding_portals', 'portals')
      ->fields('portals', array('inid', 'drupal_nid', 'description', 'submission_url', 'group_id'));
    if ($pid_flag === TRUE) {
      $sqlQuery->condition('inid', $inid, '=');
    }
    else {
      $sqlQuery->condition('drupal_nid', $inid, '=');
    }
    $result = $sqlQuery->orderBy('inid', 'ASC')->execute();
    // Organize portal elements

    foreach ($result as $row) {
      $portal_config['inid'] = $row->inid;
      $portal_config['fid'] = $row->drupal_nid;
      $portal_config['description'] = $row->description;
      $portal_config['submission_url'] = $row->submission_url;
      $portal_config['group_id'] = $row->group_id;
      $portal_config['defines'] = _get_define_keys($row->inid);
      $portal_config['connections'] = _get_portal_connections($row->inid);
      $portal_config['files'] = _get_file_keys($row->inid);
    }
  }
  return $portal_config;
}

/**
 * Get the drupal to imagenow portals.
 * @return array
 */
function _get_drupal_to_third_party_portals($groups) {
  $portals = [];
  // Get the portals from the Database
  $result = db_select('form_forwarding_portals', 'portals')
    ->fields('portals', array('inid', 'drupal_nid', 'description'))
    ->orderBy('inid', 'ASC')
    ->execute();
  // Organize portal elements
  foreach ($result as $row) {
    if(array_key_exists(3, $groups) || in_array($row->group, $groups)){
      $row1 = [];
      $row1['inid'] = $row->inid;
      $row1['fid'] = $row->drupal_nid;
      $row1['description'] = $row->description;
      $portals[] = $row1;
    }
  }
  return $portals;
}
