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
  $portal_config = _get_portal_config_admin($portal_id);
  #dsm($portal_id);
  #dsm($portal_config);
  $form = array();
  $form['#attached']['css'][] = drupal_get_path('module', 'uwpimagenow') . '/css/uwpimagenow_admin.css';
  //$form['#attached']['js'][] = drupal_get_path('module', 'uwpimagenow') . '/js/uwpimagenow-admin.js';

  $form[]['#markup'] = t('<h2>' . l('ImageNow Admin', '/admin/config/uwplatteville/imagenow') . ' Portal ' . $portal_id . '</h2>');
  //$form[]['#markup'] = t(request_path());

  $form['drupalnid'] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal Node ID'),
    '#required' => TRUE,
    '#default_value' => $portal_config['fid'],
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
    '#default_value' => $portal_config['description'],
  );
  
   $form['submitionurl'] = array(
    '#type' => 'textfield',
    '#title' => t('ImageNow Submition URL'),
    '#required' => TRUE,
    '#default_value' => $portal_config['imagenow_url'],
    '#rules' => array(
      'url',
    ),
  );
  
  if($portal_id != 'new'){
    $form_elements = _get_drupal_form_elements($portal_config['fid']);
    $form['connections'][]['#markup'] = t('<table id="portal-table">');
    $form['connections'][]['#markup'] = t('<thead>');
    $form['connections'][]['#markup'] = t('<th class="connection-id">ID</th>');
    $form['connections'][]['#markup'] = t('<th>Drupal<br>Form Elements</th>');
    $form['connections'][]['#markup'] = t('<th>ImageNow<br>Element Key</th>');
    $form['connections'][]['#markup'] = t('<th>&nbsp;</th>');
    $form['connections'][]['#markup'] = t('</thead>');
    $form['connections'][]['#markup'] = t('<tbody>');

    for($i=0; $i < count($portal_config['connections']); $i++){
      $connection = 'connection-' . strval($i);
      $form['connections'][$connection][]['#markup'] = t('<tr id=connection-row-"' . $portal_config['connections'][$i]['eid'] . '" class="odd">');
      $form['connections'][$connection][]['#markup'] = t('<td class="connection-id">');
      $form['connections'][$connection]['c' . strval($i) . '-eid'] = array(
        '#type' => 'textfield',
        '#title' => t('ID'),
        '#title_display' => 'invisible',
        '#required' => true,
        '#disabled' => true,
        '#default_value' => $portal_config['connections'][$i]['eid'],
      );
      $form['connections'][$connection][]['#markup'] = t('</td>');
      $form['connections'][$connection][]['#markup'] = t('<td class="drupal-form-elements-select">');
      $form['connections'][$connection]['c' . strval($i) . '-drupal-form-element'] = array(
        '#type' => 'select',
        '#title' => t('Drupal Form Select'),
        '#title_display' => 'invisible',
        '#multiple' => false,
        '#options' => $form_elements, //TODO: Need to set
        '#default_value' => $portal_config['connections'][$i]['dek'],
      );
      $form['connections'][$connection][]['#markup'] = t('</td>');
      $form['connections'][$connection][]['#markup'] = t('<td>');
      $form['connections'][$connection]['c' . strval($i) . '-imagenow-key'] = array(
        '#type' => 'textfield',
        '#title' => t('ImageNow Key'),
        '#title_display' => 'invisible',
        #'#description' => t('Key as set in ImageNow'),
        '#required' => false,
        '#default_value' => $portal_config['connections'][$i]['iek'],
        //'#rules' => array(
        //  array('rule' => 'regexp[/^[a-z-A-Z,\' ]*$/]', 'error' => '%field, can only have alpha characters.'),
        //),
      );
      $form['connections'][$connection][]['#markup'] = t('</td>');
      $form['connections'][$connection][]['#markup'] = t('<td>');
      $form['connections'][$connection]['c' . strval($i) . '-btndelete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#description' => t('Delete Link'),
        '#name' => strval($i) . '_delete_link',
        '#submit' => array('connection_delete'),
      );
      $form['connections'][$connection][]['#markup'] = t('</td>');
      $form['connections'][$connection][]['#markup'] = t('</tr >');
    }
    $form['connections']['new-link'][]['#markup'] = t('<tr id=connection-row-"' . i . '" class="odd">');
    $form['connections']['new-link'][]['#markup'] = t('<td class="connection-id"></td>');
    $form['connections']['new-link'][]['#markup'] = t('<td class="drupal-form-elements-select">');
    $form['connections']['new-link']['drupal-element'] = array(
      '#type' => 'select',
      '#title' => t('Drupal Form Select'),
      '#multiple' => false,
      '#options' => $form_elements, //TODO: Need to set
    );
    $form['connections']['new-link'][]['#markup'] = t('</td>');
    $form['connections']['new-link'][]['#markup'] = t('<td>');
    $form['connections']['new-link']['imagenow-key'] = array(
      '#title' => t('ImageNow Key'),
      '#description' => t('Key as set in ImageNow'),
     '#type' => 'textfield',
      '#required' => false,
      //'#rules' => array(
      //  array('rule' => 'regexp[/^[a-z-A-Z,\' ]*$/]', 'error' => '%field, can only have alpha characters.'),
    );
    $form['connections']['new-link'][]['#markup'] = t('</td>');
    $form['connections']['new-link'][]['#markup'] = t('<td>');
    $form['connections']['new-link']['btnadd'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
      '#description' => t('Add Link'),
      '#name' => 'add_link',
      '#submit' => array('connection_add'),
    );
    $form['connections']['new-link'][]['#markup'] = t('</td>');
    $form['connections']['new-link'][]['#markup'] = t('</tr >');

    $form['connections'][]['#markup'] = t('</tbody>');
    $form['connections'][]['#markup'] = t('</table>');
  }
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
  //dsm($form_state);
  $portal_id = end(explode('/', request_path()));
  
  $drupal_nid = $form_state['values']['drupalnid'];
  $description = $form_state['values']['description'];
  $imagenow_url = $form_state['values']['submitionurl'];

  if($portal_id == 'new'){
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
    drupal_set_message(t("New Portal Saved"));
    drupal_goto('/admin/config/uwplatteville/imagenow/portal/' . strval($eid));
  } else {
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

    $index = 0;
    $eid = $form_state['values']['c' . strval($index) . '-eid'];
    //dsm("eid: " . $eid);
    while($eid != NULL){
      $drupal_form_element = $form_state['values']['c' . strval($index) . '-drupal-form-element'];
      $imagenow_key = $form_state['values']['c' . strval($index) . '-imagenow-key'];
      //dsm("drupal_form_element: " . $drupal_form_element);
      //dsm("imagenow_key: " . $imagenow_key);
      $update_connection = db_update('uwpimagenow_connections')
        ->condition('eid', $eid, '=')
        ->fields(
          array(
            'drupal_element_key' => $drupal_form_element,
            'imagenow_element_key' => $imagenow_key,
          )
        )
        ->execute();
      
      $index++;
      $eid = $form_state['values']['c' . strval($index) . '-eid'];
      //dsm("eid: " . $eid);
    }
    drupal_set_message(t("Portal had been reconfigured"));
  }
  // redirect to the specific portal
}

function connection_delete($form, &$form_state) {
  //dsm($form_state);
  //dsm($form_state['clicked_button']['#name']);
  $index = $form_state['clicked_button']['#name'][0];
  //dsm($index);
  $eid = $form_state['values']['c' . strval($index) . '-eid'];
  //dsm("eid: " . $eid);
  $num_deleted = db_delete('uwpimagenow_connections')
    ->condition('eid', $eid, '=')
    ->execute();
  drupal_set_message(t("Delete Connection"));
}

function connection_add($form, &$form_state) {
  $portal_id = end(explode('/', request_path()));
  $drupal_element_key = $form_state['values']['drupal-element'];
  $imagenow_element_key = $form_state['values']['imagenow-key'];
  // Insert new connection
  $eid = db_insert('uwpimagenow_connections')
      ->fields(
        array(
          'inid' => $portal_id,
          'drupal_element_key' => $drupal_element_key,
          'imagenow_element_key' => $imagenow_element_key,
        )
      )
      ->execute();
  drupal_set_message(t("Add New Connection."));
}
/**
 * 
 */
function _get_drupal_form_elements($nid) {
  $elements['select'] = ' - Select- ';
  $result = db_select('webform_component', 'components')
    ->fields('components', array('cid', 'form_key'))
    ->condition('nid', $nid, '=')
    ->orderBy('cid', 'ASC')
    ->execute();
  // Organize portal elements
  foreach ($result as $element) {
    $elements[$element->form_key] = $element->form_key;
  }
  return $elements;
}

/**
 * Not Used
 */
function _get_drupal_form_element_cid($nid, $form_key) {
  $result = db_select('webform_component', 'components')
    ->fields('components', array('cid'))
    ->condition('nid', $nid, '=')
    ->condition('form_key', $form_key, '=')
    ->orderBy('cid', 'ASC')
    ->execute();
  // Organize portal elements
  //dsm($result);
  foreach ($result as $element) {
    return $element->cid;
  }
  return null;
}

function _get_portal_connections_inid($inid){
  $portal_connections = [];
  // Get the connections from the Database
  $result = db_select('uwpimagenow_connections', 'connections')
    ->fields('connections', array('eid', 'drupal_element_key', 'imagenow_element_key'))
    ->condition('inid', $inid, '=')
    ->orderBy('eid', 'ASC')
    ->execute();
  // Organize connection elements
  foreach ($result as $row) {
    $row1 = [];
    $row1['eid'] = $row->eid;
    $row1['dek'] = $row->drupal_element_key;
    $row1['iek'] = $row->imagenow_element_key;
    $portal_connections[] = $row1;
  }
  return $portal_connections;
}

function _get_portal_config_admin($pid) {
  $portal_config = null;
  // Get the portals from the Database
  $result = db_select('uwpimagenow_portals', 'portals')
    ->fields('portals', array('inid', 'drupal_nid', 'description', 'imagenow_submission_url'))
    ->condition('inid', $pid, '=')
    ->orderBy('inid', 'ASC')
    ->execute();
  // Organize portal elements
  #dsm($result);
  foreach ($result as $row) {
    $portal_config['inid'] = $row->inid;
    $portal_config['fid'] = $row->drupal_nid;
    $portal_config['description'] = $row->description;
    $portal_config['imagenow_url'] = $row->imagenow_submission_url;
    $portal_config['connections'] = _get_portal_connections_inid($row->inid);
  }
  return $portal_config;
}