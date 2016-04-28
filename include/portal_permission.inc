<?php
/**
 * Deletes all the permissions with the given taxonomy term.
 * @param string/int $tid
 */
function _delete_term($tid) {
  db_delete('form_forwarding_permission')
    ->condition('tid', $tid, '=')
    ->execute();
}
/**
 * Deletes the given role from the database.
 * @param string/array $rid
 */
function _delete_role($rid) {
  db_delete('form_forwarding_permission')
    ->condition('rid', $rid, '=')
    ->execute();
}
/**
 * Deletes the given role from the database.
 * @param string/array $filekid
 */
function _delete_permission($rid, $tid) {
  db_delete('form_forwarding_permission')
    ->condition('rid', $rid, '=')
    ->condition('tid', $tid, '=')
    ->execute();
}
/**
 * Updates the given permission.
 * @param int $ffpid
 * @param string $rid
 * @param string $tid
 * @return int
 */
function _update_permission($ffpid, $rid, $tid) {
  $update_file = db_update('form_forwarding_permission')
    ->condition('ffpid', $ffpid, '=')
    ->fields(
      array(
        'rid' => $rid,
        'tid' => $tid,
      )
    )
    ->execute();
  return $update_file;
}
/**
 * Creates a new permission
 * @param string $rid
 * @param string $tid
 * @return int $ file
 */
function _create_permission($rid, $tid) {
  $ffid = db_insert('form_forwarding_permission')
    ->fields(
      array(
        'rid' => $rid,
        'tid' => $tid,
      )
    )
    ->execute();
  return $ffid;
}

/**
 * Gets the files for the given portal.
 * @return array
 */
function _get_permissions() {
  $access_control = [];
  // Get the connections from the Database
  $result = db_select('form_forwarding_permission', 'ffp')
    ->fields('ffp', array('ffpid', 'tid', 'rid'))
    ->execute();
  // Organize connection elements
  foreach ($result as $row) {
    if(array_key_exists($row->rid, $uroles)){
      $row1 = array();
      $row1['ffpid'] = $row->ffid;
      $row1['tid'] = $row->tid;
      $row1['rid'] = $row->rid;
      $access_control[] = $row1;
    }
  }
  return $access_control;
}

/**
 * Gets the files for the given portal.
 * @param int $uroles
 * @return array
 */
function _get_user_terms($uroles) {
  $access_control = [];
  // Get the connections from the Database
  $result = db_select('form_forwarding_permission', 'ffp')
    ->fields('ffp', array('tid', 'rid'))
    ->execute();
  // Organize connection elements
  foreach ($result as $row) {
    if(array_key_exists($row->rid, $uroles)){
      $access_control[] = $row->tid;
    }
  }
  return $access_control;
}
