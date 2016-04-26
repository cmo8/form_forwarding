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
function _update_file($ffpid, $rid, $tid) {
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
function _create_file($rid, $tid) {
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
 * @param int $inid
 * @return array
 */
function _get_user_terms($uid) {
  dpm($uid);
  $portal_files = [];
//  // Get the connections from the Database
//  $result = db_select('form_forwarding_file_keys', 'file_keys')
//    ->fields('file_keys', array('filekid', 'third_party_element_key', 'drupal_element_key'))
//    ->condition('inid', $inid, '=')
//    ->orderBy('filekid', 'ASC')
//    ->execute();
//  // Organize connection elements
//  foreach ($result as $row) {
//    $row1 = [];
//    $row1['filekid'] = $row->filekid;
//    $row1['iek'] = $row->third_party_element_key;
//    $row1['dek'] = $row->drupal_element_key;
//    $portal_files[] = $row1;
//  }
  return $portal_files;
}
