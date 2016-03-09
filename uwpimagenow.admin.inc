<?php

/**
 * The ImageNow admin file.
 * 
 * @author Sean Mogran <morganse@uwplatt.edu>
 */

/**
 * ImageNow Admin Page. Builds the form for the user to use.
 */
function uwpimagenow_admin_form() {
  $form = array();
  //$form['#attached']['css'][] = drupal_get_path('module', 'uwpimagenow') . '/css/uwpimagenow-admin.css';
  //$form['#attached']['js'][] = drupal_get_path('module', 'uwpimagenow') . '/js/uwpimagenow-admin.js';

  $form[]['#markup'] = t('<h2>ImageNow Admin Portal</h2>');
  $form[]['#markup'] = t('<p>I allow you to connect Drupal webforms to ImageNow Forms... Do not weary it is all magic..</p>');
  
  $form['btnAddNewPortal'] = array(
    '#type' => 'button',
    '#value' => t('+ Add'),
    '#description' => t('Add a new Potal'),
    '#name' => 'add_drupal_to_imagenow_portal',
  );
  $portals = _get_drupal_to_imagenow_portals();
  dsm($portals);
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
    dsm($i);
    dsm($portals[$i]);
    $row = 'portal-' . strval($i);
    dsm($row);
    $form[$row]['#markup'] = t('<tr id=portal-row-"' . strval($i) . '" class="odd">');
    $form[$row]['#markup'] = t('<td class="in-row>');
    $form[$row]['#markup'] = t($i);
    $form[$row]['#markup'] = t('</td>');
    $form[$row]['#markup'] = t('<td class="in-drupal-form-id">');
    $form[$row]['#markup'] = t($portals[$i]['fid']);
    $form[$row]['#markup'] = t('</td>');
    $form[$row]['#markup'] = t('<td class="in-description>');
    $form[$row]['#markup'] = t($portals[$i]['description']);
    $form[$row]['#markup'] = t('</td>');
    $form[$row]['#markup'] = t('<td class="in-edit>');
    $form[$row]['btnedit'] = array(
      '#type' => 'button',
      '#value' => t('Edit'),
      '#description' => t('Edit Portal'),
      '#name' => 'edit_portal',
    );
    $form[$row]['#markup'] = t('</td>');
    $form[$row]['#markup'] = t('<td class="in-delete>');
    $form[$row]['btndelete'] = array(
      '#type' => 'button',
      '#value' => t('Delet'),
      '#description' => t('Delete Link'),
      '#name' => 'delete_portal',
    );
    $form[$row]['#markup'] = t('</td>');
    $form[$row]['#markup'] = t('</tr >');
  }
  $form[]['#markup'] = t('</tbody>');
  $form[]['#markup'] = t('</table>');
  return $form;
}

/**
 * Implementation of hook_form_submit.
 * 
 * @param type $form
 * @param type $form_state
 */
//function uwpimagenow_admin_submit($form, &$form_state) {
//  dsm($form);
//  dsm($form_state);
//}

/**
 * Get the drupal to imagenow portals.
 */
function _get_drupal_to_imagenow_portals() {
  $portals = [];
  $row1 = [];
  $row1['fid'] = 123455;
  $row1['description'] = 'Test 1';
  $row2 = [];
  $row2['fid'] = 123457;
  $row2['description'] = 'Test 2';
  $row3 = [];
  $row3['fid'] = 123458;
  $row3['description'] = 'Test 3';
  $row4 = [];
  $row4['fid'] = 123459;
  $row4['description'] = 'Test 4';
  $portals[] = $row1;
  $portals[] = $row2;
  $portals[] = $row3;
  $portals[] = $row4;
    
//TODO: Select from Table
  
  return $portals;
}