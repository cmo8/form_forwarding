<?php
/**
 * The ImageNow admin file.
 * 
 * @author Sean Mogran <morganse@uwplatt.edu>
 */
require_once 'include/inow_portals.inc';
/**
 * ImageNow Admin Page. Builds the form for the user to use.
 */
function uwpimagenow_admin_settings() {
  $form = array();
  //$form['#attached']['css'][] = drupal_get_path('module', 'uwpimagenow') . '/css/uwpimagenow-admin.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'uwpimagenow') . '/js/uwpimagenow_admin.js';

  $form[]['#markup'] = t('<h2>ImageNow Admin Portal</h2>');
  $form[]['#markup'] = t('<p>I allow you to connect Drupal webforms to ImageNow Forms... Do not weary it is all magic..</p>');
  
  $portals = _get_drupal_to_imagenow_portals();
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
      '#submit' => array('uwpimagenow_admin_save'),
    );
    $form[$row][]['#markup'] = t('</td>');
    $form[$row][]['#markup'] = t('<td class="in-delete">');
    $form[$row][]['btndelete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#description' => t('Delete Link'),
      '#name' => 'delete_portal_' . $portals[$i]['inid'],
      '#submit' => array('uwpimagenow_admin_save'),
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
    '#name' => 'add_drupal_to_imagenow_portal',
    '#submit' => array('uwpimagenow_admin_save'),
  );
  return $form;
}

/**
 * Implementation of hook_form_submit.
 * 
 * @param type $form
 * @param type $form_state
 */
function uwpimagenow_admin_save($form, &$form_state) {
  if ($form_state['clicked_button']['#name'] == 'add_drupal_to_imagenow_portal') {
    drupal_goto('admin/config/uwplatteville/imagenow/portal/new');
  }
  $portals = _get_drupal_to_imagenow_portals();
  for($i = 0; $i < count($portals); $i++) {
    if ($form_state['clicked_button']['#name'] == 'edit_portal_' . $portals[$i]['inid']) {
      drupal_goto('admin/config/uwplatteville/imagenow/portal/' . $portals[$i]['inid']);
    }
    if ($form_state['clicked_button']['#name'] == 'delete_portal_' . $portals[$i]['inid']) {
      portal_delete($form, $form_state);
      drupal_set_message(t('Delete Portal ' . $portals[$i]['inid']));
    }
  }
}

function portal_delete($form, &$form_state) {
  $tmp = explode('_', $form_state['clicked_button']['#name']);
  $index = count($tmp) - 1;
  $portal_id = $tmp[$index];
  _delete_connections($portal_id);
  _delete_portal($portal_id);
  drupal_set_message(t("Portal had been warped beyond repair"));
}