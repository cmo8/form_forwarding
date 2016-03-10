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
  $portal_id = end(explode('/', request_path()));
  $portal_config = _get_portal_config($portal_id);
  dsm($portal_id);
  dsm($portal_config);
  $form = array();
  //$form['#attached']['css'][] = drupal_get_path('module', 'uwpimagenow') . '/css/uwpimagenow-admin.css';
  //$form['#attached']['js'][] = drupal_get_path('module', 'uwpimagenow') . '/js/uwpimagenow-admin.js';

  $form[]['#markup'] = t('<h2>' . l('ImageNow Admin', '/admin/config/uwplatteville/imagenow') . ' Portal ' . $portal_id . '</h2>');
  $form[]['#markup'] = t(request_path());

  $form['drupalnid'] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal Node ID'),
    '#required' => TRUE,
    '#rules' => array(
      array('rule' => 'numeric', 'error' => '%field - Must be numeric.'),
    ),
    //'#ajax' => array(
    //  'callback' => 'update_drupal_form_elements',
    //  'wrapper' => 'poll-choices',
    //  'method' => 'replace',
    //),
  );
  
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#required' => TRUE,
  );
  
   $form['submitionurl'] = array(
    '#type' => 'textfield',
    '#title' => t('ImageNow Submition URL'),
    '#required' => TRUE,
    '#rules' => array(
      'url',
    ),
  );
  
  $connections = _get_drupal_to_imagenow_connections();
  $form[]['#markup'] = t('<table id="portal-table">');
  $form[]['#markup'] = t('<thead>');
  $form[]['#markup'] = t('<th>Drupal<br>Form Elements</th>');
  $form[]['#markup'] = t('<th>ImageNow<br>Element Key</th>');
  $form[]['#markup'] = t('<th>&nbsp;</th>');
  $form[]['#markup'] = t('</thead>');
  $form[]['#markup'] = t('<tbody>');
  for($i=0; $i < count($connections); $i++){
    $connection = 'connection-' . strval($i);
    $form[$connection][]['#markup'] = t('<tr id=connection-row-"' . i . '" class="odd">');
    $form[$connection][]['#markup'] = t('<td class="drupal-form-elements-select">');
    $form[$connection][]['drupal-form-element'] = array(
      '#type' => 'select',
      '#title' => t('Drupal Form Select'),
      '#multiple' => false,
      '#options' => array($connections[$i]['drupal_element'],'- Select -'), //TODO: Need to set
    );
    $form[$connection][]['#markup'] = t('</td>');
    $form[$connection][]['#markup'] = t('<td>');
    $form[$connection][]['imagenow-key'] = array(
      '#title' => t('ImageNow Key'),
      '#description' => t('Key as set in ImageNow'),
      '#type' => 'textfield',
      '#required' => false,
      '#default_value' => $connections[$i]['imagenow_key'],
      //'#rules' => array(
      //  array('rule' => 'regexp[/^[a-z-A-Z,\' ]*$/]', 'error' => '%field, can only have alpha characters.'),
      //),
    );
    $form[$connection][]['#markup'] = t('</td>');
    $form[$connection][]['#markup'] = t('<td>');
    $form[$connection][]['btndelete'] = array(
      '#type' => 'submit',
      '#value' => t('Delet'),
      '#description' => t('Delete Link'),
      '#name' => 'delete_link',
      '#submit' => array('connection_delete'),
    );
    $form[$connection][]['#markup'] = t('</td>');
    $form[$connection][]['#markup'] = t('</tr >');
  }
  $form['new-link'][]['#markup'] = t('<tr id=connection-row-"' . i . '" class="odd">');
  $form['new-link'][]['#markup'] = t('<td class="drupal-form-elements-select">');
  $form['new-link']['drupal-element'] = array(
    '#type' => 'select',
    '#title' => t('Drupal Form Select'),
    '#multiple' => false,
    '#options' => $opt, //TODO: Need to set
  );
  $form['new-link'][]['#markup'] = t('</td>');
  $form['new-link'][]['#markup'] = t('<td>');
  $form['new-link']['imagenow-key'] = array(
    '#title' => t('ImageNow Key'),
    '#description' => t('Key as set in ImageNow'),
   '#type' => 'textfield',
    '#required' => false,
    //'#rules' => array(
    //  array('rule' => 'regexp[/^[a-z-A-Z,\' ]*$/]', 'error' => '%field, can only have alpha characters.'),
  );
  $form['new-link'][]['#markup'] = t('</td>');
  $form['new-link'][]['#markup'] = t('<td>');
  $form['new-link']['btnadd'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#description' => t('Add Link'),
    '#name' => 'add_link',
    '#submit' => array('connection_add'),
  );
  $form['new-link'][]['#markup'] = t('</td>');
  $form['new-link'][]['#markup'] = t('</tr >');

  $form[]['#markup'] = t('</tbody>');
  $form[]['#markup'] = t('</table>');

  $form['btnSavePortal'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#description' => t('Save Potal'),
    '#name' => 'save_portal',
    '#submit' => array('portal_save'),
  );
  
  return $form;
}

function portal_save($form, &$form_state) {
  drupal_set_message(t("Save Portal"));
}

function connection_delete($form, &$form_state) {
  drupal_set_message(t("Delete Connection"));
}

function connection_add($form, &$form_state) {
  drupal_set_message(t("Add New Connection."));
}

function update_drupal_form_elements() {
  
}
/**
 * Get the drupal to imagenow portals.
 */
function _get_drupal_to_imagenow_connections() {
  $connections = [];
  $row1 = [];
  $row1['drupal_element'] = 'name';
  $row1['imagenow_key'] = 'IM_Name_ld';
  $row2 = [];
  $row2['drupal_element'] = 'birthday';
  $row2['imagenow_key'] = 'IM_Birthday_ld';
  $row3 = [];
  $row3['drupal_element'] = 'address';
  $row3['imagenow_key'] = 'IM_Address_ld';
  $connections[] = $row1;
  $connections[] = $row2;
  $connections[] = $row3;
  //TODO: Select from Table
  
  return $connections;
}

function _get_portal_connections($inid){
  $portal_connections = [];
     // Get the portals from the Database
  $result = db_select('uwpimagenow_portal_connections', 'connections')
    ->fields('connections', array('eid', 'drupal_element_key', 'imagenow_element_key'))
    ->condition('inid', $inid, '=')
    ->orderBy('eid', 'ASC')
    ->execute();
  // Organize portal elements
  foreach ($result as $row) {
    $row1 = [];
    $row1['eid'] = $row->eid;
    $row1['dek'] = $row->drupal_element_key;
    $row1['iek'] = $row->imagenow_element_key;
    $portal_connections[] = $row1;
  }
  return $portal_connections;
}

function _get_portal_config($pid) {
  $portal_config = [];
  // Get the portals from the Database
  $result = db_select('uwpimagenow_portals', 'portals')
    ->fields('portals', array('inid', 'drupal_nid', 'description', 'imagenow_submission_url'))
    ->condition('inid', $pid, '=')
    ->orderBy('inid', 'ASC')
    ->execute();
  // Organize portal elements
  foreach ($result as $row) {
    $row1 = [];
    $row1['inid'] = $row->inid;
    $row1['fid'] = $row->drupal_nid;
    $row1['description'] = $row->description;
    $row1['imagenow_url'] = $row->imagenow_submission_url;
    $row1['connections'] = _get_portal_connections($row->inid);
    $portal_config[] = $row1;
  }
  return $portal_config;
}