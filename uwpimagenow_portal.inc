<?php

/**
 * The ImageNow Portal Details file.
 * 
 * @author Sean Mogran <morganse@uwplatt.edu>
 */

/**
 * ImageNow Admin Page. Builds the form for the user to use.
 */
function uwpimagenow_portal_admin() {
  $form = array();
  //$form['#attached']['css'][] = drupal_get_path('module', 'uwpimagenow') . '/css/uwpimagenow-admin.css';
  //$form['#attached']['js'][] = drupal_get_path('module', 'uwpimagenow') . '/js/uwpimagenow-admin.js';

  $form[]['#markup'] = t('<h2>ImageNow Admin Portal</h2>');
  $form[]['#markup'] = t('<p>I allow you to connect Drupal webforms to ImageNow Forms... Do not weary it is magic..</p>');

//  $portals = _get_drupal_to_imagenow_portals();
  $form[]['#markup'] = t('<table id="portal-table">');
  $form[]['#markup'] = t('<thead>');
  $form[]['#markup'] = t('<th>Drupal<br>Form Elements</th>');
  $form[]['#markup'] = t('<th>ImageNow<br>Element Key</th>');
  $form[]['#markup'] = t('<th>&nbsp;</th>');
  $form[]['#markup'] = t('</thead>');
  $form[]['#markup'] = t('<tbody>');
//  for($i=0; $i < len($portals); $i++){
//    $row = 'portal-' . $i;
//    $form[$row]['#markup'] = t('<tr id=portal-row-"' . i . '" class="odd">');
//    $form[$row]['#markup'] = t('<td class="c-select">');
//    $form[$row]['drupal-element'] = array(
//      '#type' => 'select',
//      '#title' => t('Drupal Form Select'),
//      '#multiple' => false,
//      '#options' => $opt, //TODO: Need to set
//    );
//    $form[$row]['#markup'] = t('</td>');
//    $form[$row]['#markup'] = t('<td>');
//    $form[$row]['imagenow-key'] = array(
//      '#title' => t('ImageNow Key'),
//      '#description' => t('Key as set in ImageNow'),
//      '#type' => 'textfield',
//      '#required' => false,
//      //'#rules' => array(
//      //  array('rule' => 'regexp[/^[a-z-A-Z,\' ]*$/]', 'error' => '%field, can only have alpha characters.'),
//    );
//    $form[$row]['#markup'] = t('</td>');
//    $form[$row]['#markup'] = t('<td>');
//    $form[$row]['btndelete'] = array(
//      '#type' => 'button',
//      '#value' => t('Delet'),
//      '#description' => t('Delete Link'),
//      '#name' => 'delete_link',
//    );
//    $form[$row]['#markup'] = t('</td>');
//    $form[$row]['#markup'] = t('</tr >');
//  }
  $form['new-link']['#markup'] = t('<tr id=portal-row-"' . i . '" class="odd">');
  $form['new-link']['#markup'] = t('<td class="c-select">');
//  $form['new-link']['drupal-element'] = array(
//    '#type' => 'select',
//    '#title' => t('Drupal Form Select'),
//    '#multiple' => false,
//    '#options' => $opt, //TODO: Need to set
//  );
//  $form['new-link']['#markup'] = t('</td>');
//  $form['new-link']['#markup'] = t('<td>');
//  $form['new-link']['imagenow-key'] = array(
//    '#title' => t('ImageNow Key'),
//    '#description' => t('Key as set in ImageNow'),
//    '#type' => 'textfield',
//    '#required' => false,
//    //'#rules' => array(
//    //  array('rule' => 'regexp[/^[a-z-A-Z,\' ]*$/]', 'error' => '%field, can only have alpha characters.'),
//  );
//  $form['new-link']['#markup'] = t('</td>');
//  $form['new-link']['#markup'] = t('<td>');
//  $form['new-link']['btnadd'] = array(
//    '#type' => 'button',
//    '#value' => t('Add'),
//    '#description' => t('Add Link'),
//    '#name' => 'add_link',
//  );
  $form['new-link']['#markup'] = t('</td>');
  $form['new-link']['#markup'] = t('</tr >');

  $form[]['#markup'] = t('</tbody>');
  $form[]['#markup'] = t('</table>');

  
  
  return $form;
}

/**
 * Get the drupal to imagenow portals.
 */
function _get_drupal_to_imagenow_portal() {
  $portals = [];
  
  //TODO: Select from Table
  
  return $portals;
}