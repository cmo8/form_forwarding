<?php

/**
 * The ImageNow admin file.
 * 
 * @author Sean Mogran <morganse@uwplatt.edu>
 */

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
  #dsm($portals);
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
    #dsm($i);
    #dsm($portals[$i]);
    $row = 'portal-' . strval($i);
    #dsm($row);
    $form[]['#markup'] = t('<tr id=portal-row-"' . strval($i) . '" class="odd">');
    $form[]['#markup'] = t('<td class="in-row">');
    $form[]['#markup'] = t(strval($i));
    $form[]['#markup'] = t('</td>');
    $form[]['#markup'] = t('<td class="in-drupal-form-id">');
    $form[]['#markup'] = t($portals[$i]['fid']);
    $form[]['#markup'] = t('</td>');
    $form[]['#markup'] = t('<td class="in-description">');
    $form[]['#markup'] = t($portals[$i]['description']);
    $form[]['#markup'] = t('</td>');
    $form[]['#markup'] = t('<td class="in-edit">');
    $form[]['btnedit'] = array(
      '#type' => 'submit',
      '#value' => t('Edit'),
      '#description' => t('Edit Portal'),
      '#name' => 'edit_portal_' . strval($i),
      '#submit' => array('uwpimagenow_admin_save'),
    );
    $form[]['#markup'] = t('</td>');
    $form[]['#markup'] = t('<td class="in-delete">');
    $form[]['btndelete'] = array(
      '#type' => 'submit',
      '#value' => t('Delet'),
      '#description' => t('Delete Link'),
      '#name' => 'delete_portal_' . strval($i),
      '#submit' => array('uwpimagenow_admin_save'),
    );
    $form[]['#markup'] = t('</td>');
    $form[]['#markup'] = t('</tr>');
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
  $count = 4;// _get_drupal_to_imagenow_portal_count();
  for($i = 0; $i < $count; $i++) {
    if ($form_state['clicked_button']['#name'] == 'edit_portal_' . strval($i)) {
      drupal_goto('admin/config/uwplatteville/imagenow/portal/' . strval($i));
    }
    if ($form_state['clicked_button']['#name'] == 'delete_portal_' . strval($i)) {
      drupal_set_message(t('Delete Portal ' . strval($i)));
    }
  }
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
    $opt['description'] = $row->description;
    $portals[] = $row1;
  }
  return $portals;
}