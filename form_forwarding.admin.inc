<?php
/**
 * The Webform Forwarding admin file.
 * 
 * @author Sean Mogran <morganse@uwplatt.edu>
 */
require_once 'include/portal.inc';

/**
 * Webform Forwarding Admin Page. Builds the form for the user to use.
 */
function form_forwarding_admin_settings() {
  $form = array();
  //$form['#attached']['css'][] = drupal_get_path('module', 'form_forwarding') . '/css/form_forwarding_admin.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'form_forwarding') . '/js/form_forwarding_admin.js';

  $form[]['#markup'] = t('<h2>Webform Forwarding Admin</h2>');
  $form[]['#markup'] = t('<p>I allow you to connect Drupal webforms to 3rd party Webforms. '. l('Edit Permissions', '/admin/config/content/form_forwarding/permissions') . '</p>');
  global $user;
  #dsm($user);
  $portals = _get_drupal_to_third_party_portals($user->roles);
  $form[]['#markup'] = t('<table id="portal-table">');
  $form[]['#markup'] = t('<thead>');
  $form[]['#markup'] = t('<th>Index</th>');
  $form[]['#markup'] = t('<th>Drupal Form ID</th>');
  $form[]['#markup'] = t('<th>Description</th>');
  $form[]['#markup'] = t('<th>&nbsp</th>');
  $form[]['#markup'] = t('<th>&nbsp</th>');
  $form[]['#markup'] = t('</thead>');
  $form[]['#markup'] = t('<tbody>');
  for($i=0; $i < count($portals); $i++){
    $row = 'portal-' . $portals[$i]['inid'];
    $form[$row][]['#markup'] = t('<tr id=portal-row-"' . $portals[$i]['inid'] . '" class="odd">');
    $form[$row][]['#markup'] = t('<td class="in-row">');
    $form[$row][]['#markup'] = t($portals[$i]['inid']);
    $form[$row][]['#markup'] = t('</td>');
    $form[$row][]['#markup'] = t('<td class="in-drupal-form-id">');
    $form[$row][]['#markup'] = t($portals[$i]['fid']);
    $form[$row][]['#markup'] = t('</td>');
    $form[$row][]['#markup'] = t('<td class="in-description">');
    $form[$row][]['#markup'] = t($portals[$i]['description']);
    $form[$row][]['#markup'] = t('</td>');
    $form[$row][]['#markup'] = t('<td class="in-edit">');
    $form[$row][]['btnedit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit'),
      '#description' => t('Edit Portal'),
      '#name' => 'edit_portal_' . $portals[$i]['inid'],
      '#submit' => array('form_forwarding_admin_save'),
    );
    $form[$row][]['#markup'] = t('</td>');
    $form[$row][]['#markup'] = t('<td class="in-delete">');
    $form[$row][]['btndelete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#description' => t('Delete Link'),
      '#name' => 'delete_portal_' . $portals[$i]['inid'],
      '#submit' => array('form_forwarding_admin_save'),
    );
    $form[$row][]['#markup'] = t('</td>');
    $form[$row][]['#markup'] = t('</tr>');
  }
  $form[]['#markup'] = t('</tbody>');
  $form[]['#markup'] = t('</table>');
  $form['btnAddNewPortal'] = array(
    '#type' => 'submit',
    '#value' => t('+ Add'),
    '#description' => t('Add a new Potal'),
    '#name' => 'add_drupal_to_form_forwarding',
    '#submit' => array('form_forwarding_admin_save'),
  );
  return $form;
}

/**
 * Implementation of hook_form_submit.
 * @param array $form
 * @param array $form_state
 */
function form_forwarding_admin_save($form, &$form_state) {
  if ($form_state['clicked_button']['#name'] == 'add_drupal_to_form_forwarding') {
    drupal_goto('admin/config/content/form_forwarding/portal/new');
  }
  global $user;
  #dsm($user);
  $portals = _get_drupal_to_third_party_portals($user->roles);
  for($i = 0; $i < count($portals); $i++) {
    if ($form_state['clicked_button']['#name'] == 'edit_portal_' . $portals[$i]['inid']) {
      //watchdog("CMO-PHP", 'admin/config/content/form_forwarding/portal/' . $portals[$i]['inid']);
      drupal_goto('admin/config/content/form_forwarding/portal/' . $portals[$i]['inid']);
    }
    if ($form_state['clicked_button']['#name'] == 'delete_portal_' . $portals[$i]['inid']) {
      _portal_delete($form, $form_state);
    }
  }
}

/**
 * Deletes the selected portal.
 * @param array $form
 * @param array $form_state
 */
function _portal_delete($form, &$form_state) {
  $tmp = explode('_', $form_state['clicked_button']['#name']);
  $index = count($tmp) - 1;
  $portal_id = $tmp[$index];
  //_delete_connections($portal_id);
  _delete_portal($portal_id);
  drupal_set_message(t("Portal had been Deleted"));
}