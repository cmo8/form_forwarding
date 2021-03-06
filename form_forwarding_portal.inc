<?php

require_once 'include/portal.inc';
require_once 'include/portal_connection.inc';
require_once 'include/portal_define_key.inc';
require_once 'include/portal_file_key.inc';
/**
 * The Third Party Portal Details file.
 * 
 * @author Sean Mogran <morganse@uwplatt.edu>
 */

/**
 * Third Party Admin Page. Builds the form for the user to use.
 */
function form_forwarding_portal_admin() {
  $path_url = request_path();
  $path_explode = explode('/', $path_url);
  $portal_id = end($path_explode);
  $portal_config = _get_portal_config($portal_id, TRUE);
  $form = array();
  $form['#attached']['css'][] = drupal_get_path('module', 'form_forwarding') . '/css/form_forwarding_admin.css';
  //$form['#attached']['js'][] = drupal_get_path('module', 'form_forwarding') . '/js/form_forwarding_admin.js';
  if ($portal_id != 'new' && $portal_id != 'portal') {
    $form[]['#markup'] = t('<h2>' . l('Webform Forwarding Admin', '/admin/config/content/form_forwarding') . ' Portal ' . $portal_id . '</h2>');
  }
  else {
    $form[]['#markup'] = t('<h2>' . l('Webform Forwarding Admin', '/admin/config/content/form_forwarding') . ' Portal New</h2>');
  }

  $form['drupalnid'] = array(
    '#type' => 'textfield',
    '#title' => t('Drupal Node ID'),
    '#description' => t('The Node ID can be found in the url of the page the form is on when you are in the webform tab. The URL will look like "uwplatt.edu/node/[The Node ID]/webform'),
    '#required' => TRUE,
    '#default_value' => $portal_config['fid'],
    '#rules' => array(
      array('rule' => 'numeric', 'error' => '%field - Must be numeric.'),
      array('rule' => 'is_node')
    ),
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#required' => TRUE,
    '#default_value' => $portal_config['description'],
  );
  $vocabulary = taxonomy_vocabulary_machine_name_load('webform_forwarding_groups');
  $opt = array();
  $opt['-select-'] = '- Select -';
  foreach (taxonomy_get_tree($vocabulary->vid) as $term) {
    $opt[$term->tid] = $term->name;
  }
  #dsm($opt);
  $form['group'] = array(
    '#type' => 'select',
    '#title' => t('Portal Group'),
    '#multiple' => false,
    '#options' => $opt,
    '#default_value' => $portal_config['group_id'],
  );

  $form['submitionurl'] = array(
    '#type' => 'textfield',
    '#title' => t('Third Party Submission URL'),
    '#required' => TRUE,
    '#default_value' => $portal_config['submission_url'],
    '#rules' => array(
      'url',
    ),
  );
  
  $form['pingTest'] = array(
    '#type' => 'button',
    '#value' => t('Test URL'),
    '#description' => t('Tests if the URL is valid'),
    '#button_type' => 'submit ajax-trigger',
    '#prefix' => '<div id="ping-test-result">',
    '#suffix' => '</div>',
    '#ajax' => array(
      'submitter' => FALSE,
      'event' => 'mousedown',
      'callback' => 'pingTest',
      'wrapper' => 'ping-test-result',
      'method' => 'replace',
    ),
  );



  if ($portal_id != 'new' && $portal_id != 'portal') {
    $form_elements = _get_drupal_form_elements($portal_config['fid']);
################################################################################
    $form['defines'][]['#markup'] = t('<div id="defines">');
    $form['defines'][]['#markup'] = t('<h3>Portal Value Defines</h3>');
    $form['defines'][]['#markup'] = t('<table id="portal-define-table">');
    $form['defines'][]['#markup'] = t('<thead>');
    $form['defines'][]['#markup'] = t('<th class="id-hide">ID</th>');
    $form['defines'][]['#markup'] = t('<th>Third Party<br>Element Key</th>');
    $form['defines'][]['#markup'] = t('<th>Element Value</th>');
    $form['defines'][]['#markup'] = t('<th>&nbsp;</th>');
    $form['defines'][]['#markup'] = t('</thead>');
    $form['defines'][]['#markup'] = t('<tbody>');

    for ($i = 0; $i < count($portal_config['defines']); $i++) {
      $define = 'define-' . strval($i);
      $form['defines'][$define][]['#markup'] = t('<tr id=define-row-"' . $portal_config['defines'][$i]['dkid'] . '" class="odd">');
      $form['defines'][$define][]['#markup'] = t('<td class="id-hide">');
      $form['defines'][$define]['d' . strval($i) . '-dkid'] = array(
        '#type' => 'textfield',
        '#title' => t('Define ID'),
        '#title_display' => 'invisible',
        '#required' => true,
        '#disabled' => true,
        '#default_value' => $portal_config['defines'][$i]['dkid'],
      );
      $form['defines'][$define][]['#markup'] = t('</td>');
      $form['defines'][$define][]['#markup'] = t('<td class="third-party-element-key">');
      $form['defines'][$define]['d' . strval($i) . '-third-party-element-key'] = array(
        '#type' => 'textfield',
        '#title' => t('Third Party Element Key'),
        '#title_display' => 'invisible',
        '#required' => false,
        '#default_value' => $portal_config['defines'][$i]['ikey'],
      );
      $form['defines'][$define][]['#markup'] = t('</td>');
      $form['defines'][$define][]['#markup'] = t('<td>');
      $form['defines'][$define]['d' . strval($i) . '-third-party-element-value'] = array(
        '#type' => 'textfield',
        '#title' => t('Third Party Element Value'),
        '#title_display' => 'invisible',
        '#required' => false,
        '#default_value' => $portal_config['defines'][$i]['ivalue'],
        //'#rules' => array(
        //  array('rule' => 'regexp[/^[a-z-A-Z,\' ]*$/]', 'error' => '%field, can only have alpha characters.'),
        //),
      );
      $form['defines'][$define][]['#markup'] = t('</td>');
      $form['defines'][$define][]['#markup'] = t('<td>');
      $form['defines'][$define]['d' . strval($i) . '-btndelete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#description' => t('Delete Define'),
        '#name' => strval($i) . '_delete_define',
        '#submit' => array('define_delete'),
      );
      $form['defines'][$define][]['#markup'] = t('</td>');
      $form['defines'][$define][]['#markup'] = t('</tr >');
    }
    $form['defines']['new-define'][]['#markup'] = t('<tr id="define-row-new" class="odd">');
    $form['defines']['new-define'][]['#markup'] = t('<td class="id-hide"></td>');
    $form['defines']['new-define'][]['#markup'] = t('<td class="third-party-element-key">');
    $form['defines']['new-define']['third-party-element-key'] = array(
      '#type' => 'textfield',
      '#title' => t('Third Party Element Key'),
      '#title_display' => 'invisible',
      '#required' => false,
    );
    $form['defines']['new-define'][]['#markup'] = t('</td>');
    $form['defines']['new-define'][]['#markup'] = t('<td>');
    $form['defines']['new-define']['third-party-element-value'] = array(
      '#type' => 'textfield',
      '#title' => t('Third Party Element Value'),
      '#title_display' => 'invisible',
      '#required' => false,
      //'#rules' => array(
      //  array('rule' => 'regexp[/^[a-z-A-Z,\' ]*$/]', 'error' => '%field, can only have alpha characters.'),
      //),
    );
    $form['defines']['new-define'][]['#markup'] = t('</td>');
    $form['defines']['new-define'][]['#markup'] = t('<td>');
    $form['defines']['new-define']['btnadd-define'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
      '#description' => t('Add Define'),
      '#name' => 'add_define',
      '#submit' => array('define_add'),
    );
    $form['defines']['new-define'][]['#markup'] = t('</td>');
    $form['defines']['new-define'][]['#markup'] = t('</tr >');

    $form['defines'][]['#markup'] = t('</tbody>');
    $form['defines'][]['#markup'] = t('</table>');

    $form['defines'][]['#markup'] = t('</div>');

################################################################################
    $form['connections'][]['#markup'] = t('<div id="connections">');
    $form['connections'][]['#markup'] = t('<h3>Portal Connections</h3>');
    $form['connections'][]['#markup'] = t('<table id="portal-connections-table">');
    $form['connections'][]['#markup'] = t('<thead>');
    $form['connections'][]['#markup'] = t('<th class="id-hide">ID</th>');
    $form['connections'][]['#markup'] = t('<th>Drupal<br>Form Elements</th>');
    $form['connections'][]['#markup'] = t('<th>Third Party<br>Element Key</th>');
    $form['connections'][]['#markup'] = t('<th>&nbsp;</th>');
    $form['connections'][]['#markup'] = t('</thead>');
    $form['connections'][]['#markup'] = t('<tbody>');

    for ($i = 0; $i < count($portal_config['connections']); $i++) {
      $connection = 'connection-' . strval($i);
      $form['connections'][$connection][]['#markup'] = t('<tr id=connection-row-"' . $portal_config['connections'][$i]['eid'] . '" class="odd">');
      $form['connections'][$connection][]['#markup'] = t('<td class="id-hide">');
      $form['connections'][$connection]['c' . strval($i) . '-eid'] = array(
        '#type' => 'textfield',
        '#title' => t('Connection ID'),
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
        '#options' => $form_elements['fields'], //TODO: Need to set
        '#default_value' => $portal_config['connections'][$i]['dek'],
      );
      $form['connections'][$connection][]['#markup'] = t('</td>');
      $form['connections'][$connection][]['#markup'] = t('<td>');
      $form['connections'][$connection]['c' . strval($i) . '-third-party-key'] = array(
        '#type' => 'textfield',
        '#title' => t('Third Party Key'),
        '#title_display' => 'invisible',
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
    $form['connections']['new-link'][]['#markup'] = t('<tr id="connection-row-new" class="odd">');
    $form['connections']['new-link'][]['#markup'] = t('<td class="id-hide"></td>');
    $form['connections']['new-link'][]['#markup'] = t('<td class="drupal-form-elements-select">');
    $form['connections']['new-link']['drupal-element'] = array(
      '#type' => 'select',
      '#title' => t('Drupal Form Select'),
      '#multiple' => false,
      '#options' => $form_elements['fields'], //TODO: Need to set
    );
    $form['connections']['new-link'][]['#markup'] = t('</td>');
    $form['connections']['new-link'][]['#markup'] = t('<td>');
    $form['connections']['new-link']['third-party-key'] = array(
      '#title' => t('Third Party Key'),
      '#description' => t('Key as set in Third Party'),
      '#type' => 'textfield',
      '#required' => false,
      //'#rules' => array(
      //  array('rule' => 'regexp[/^[a-z-A-Z,\' ]*$/]', 'error' => '%field, can only have alpha characters.'),
    );
    $form['connections']['new-link'][]['#markup'] = t('</td>');
    $form['connections']['new-link'][]['#markup'] = t('<td>');
    $form['connections']['new-link']['btnadd-conn'] = array(
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

    $form['connections'][]['#markup'] = t('</div>');
################################################################################
    $form['files'][]['#markup'] = t('<div id="files">');
    $form['files'][]['#markup'] = t('<h3>Portal Files</h3>');
    $form['files'][]['#markup'] = t('<table id="portal-files-table">');
    $form['files'][]['#markup'] = t('<thead>');
    $form['files'][]['#markup'] = t('<th class="id-hide">ID</th>');
    $form['files'][]['#markup'] = t('<th>Drupal<br>Form Elements</th>');
    $form['files'][]['#markup'] = t('<th>Third Party<br>Element Key</th>');
    $form['files'][]['#markup'] = t('<th>&nbsp;</th>');
    $form['files'][]['#markup'] = t('</thead>');
    $form['files'][]['#markup'] = t('<tbody>');

    for ($i = 0; $i < count($portal_config['files']); $i++) {
      $file = 'file-' . strval($i);
      $form['files'][$file][]['#markup'] = t('<tr id=file-row-"' . $portal_config['files'][$i]['filekid'] . '" class="odd">');
      $form['files'][$file][]['#markup'] = t('<td class="id-hide">');
      $form['files'][$file]['f' . strval($i) . '-filekid'] = array(
        '#type' => 'textfield',
        '#title' => t('File ID'),
        '#title_display' => 'invisible',
        '#required' => true,
        '#disabled' => true,
        '#default_value' => $portal_config['files'][$i]['filekid'],
      );
      $form['files'][$file][]['#markup'] = t('</td>');
      $form['files'][$file][]['#markup'] = t('<td class="drupal-form-elements-select">');
      $form['files'][$file]['f' . strval($i) . '-drupal-form-element'] = array(
        '#type' => 'select',
        '#title' => t('Drupal Form Select'),
        '#title_display' => 'invisible',
        '#multiple' => false,
        '#options' => $form_elements['files'],
        '#default_value' => $portal_config['files'][$i]['dek'],
      );
      $form['files'][$file][]['#markup'] = t('</td>');
      $form['files'][$file][]['#markup'] = t('<td>');
      $form['files'][$file]['f' . strval($i) . '-third-party-key'] = array(
        '#type' => 'textfield',
        '#title' => t('Third Party Key'),
        '#title_display' => 'invisible',
        '#required' => false,
        '#default_value' => $portal_config['files'][$i]['iek'],
        //'#rules' => array(
        //  array('rule' => 'regexp[/^[a-z-A-Z,\' ]*$/]', 'error' => '%field, can only have alpha characters.'),
        //),
      );
      $form['files'][$file][]['#markup'] = t('</td>');
      $form['files'][$file][]['#markup'] = t('<td>');
      $form['files'][$file]['f' . strval($i) . '-btndelete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#description' => t('Delete File'),
        '#name' => strval($i) . '_delete_file',
        '#submit' => array('delete_file'),
      );
      $form['files'][$file][]['#markup'] = t('</td>');
      $form['files'][$file][]['#markup'] = t('</tr >');
    }
    $form['files']['new-file'][]['#markup'] = t('<tr id="file-row-new" class="odd">');
    $form['files']['new-file'][]['#markup'] = t('<td class="id-hide"></td>');
    $form['files']['new-file'][]['#markup'] = t('<td class="drupal-form-elements-select">');
    $form['files']['new-file']['file-drupal-element'] = array(
      '#type' => 'select',
      '#title' => t('Drupal Form Select'),
      '#multiple' => false,
      '#options' => $form_elements['files'],
    );
    $form['files']['new-file'][]['#markup'] = t('</td>');
    $form['files']['new-file'][]['#markup'] = t('<td>');
    $form['files']['new-file']['file-third-party-key'] = array(
      '#title' => t('Third Party Key'),
      '#description' => t('Key as set in Third Party System. For multi File uploads "file_sub_/[1-4]" or "file_sub_/[a-g]" or file_/[1,2,3,a,b,cool]/_sub'),
      '#type' => 'textfield',
      '#required' => false,
      //'#rules' => array(
      //  array('rule' => 'regexp[/^[a-z-A-Z,\' ]*$/]', 'error' => '%field, can only have alpha characters.'),
    );
    $form['files']['new-file'][]['#markup'] = t('</td>');
    $form['files']['new-file'][]['#markup'] = t('<td>');
    $form['files']['new-file']['btnadd-file'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
      '#description' => t('Add Link'),
      '#name' => 'add_file',
      '#submit' => array('file_add'),
    );
    $form['files']['new-file'][]['#markup'] = t('</td>');
    $form['files']['new-file'][]['#markup'] = t('</tr >');

    $form['files'][]['#markup'] = t('</tbody>');
    $form['files'][]['#markup'] = t('</table>');
    $form['files'][]['#markup'] = t('</div>');
################################################################################
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

function pingTest($form, &$form_state){
  $url = $form_state['values']['submitionurl'];
  $file_headers = @get_headers($url);
  $test = '';
  if (strpos($file_headers[0], "200 OK") > 0) {
     $test = '<p>Success</p>';
  } else {
    $test = '<p>Failed</p>';
  }
  $roo = array(
    '#type' => 'button',
    '#value' => t('Test URL'),
    '#description' => t('Tests if the URL is valid'),
    '#button_type' => 'submit ajax-trigger',
    '#prefix' => '<div id="ping-test-result">',
    '#suffix' => $test . '</div>',
    '#ajax' => array(
      'submitter' => FALSE,
      'event' => 'mousedown',
      'callback' => 'pingTest',
      'wrapper' => 'ping-test-result',
      'method' => 'replace',
    ),
  );
  return $roo;
}

/**
 * Handler for deleting a define
 * @param array $form
 * @param array $form_state
 */
function define_delete($form, &$form_state) {
  $index = $form_state['clicked_button']['#name'][0];
  $dkid = $form_state['values']['d' . strval($index) . '-dkid'];
  $num_deleted = _delete_define($dkid);
  drupal_set_message(t("Delete Define"));
}
/**
 * Handler for adding a define
 * @param array $form
 * @param array $form_state
 */
function define_add($form, &$form_state) {
  $portal_id = request_path();
  $portal_id = explode('/', $portal_id);
  $portal_id = end($portal_id);
  $third_party_element_key = $form_state['values']['third-party-element-key'];
  $third_party_element_value = $form_state['values']['third-party-element-value'];
  if ($third_party_element_key !== "" && $third_party_element_value !== "") {
    //Insert new connection
    $dkid = _create_define($portal_id, $third_party_element_key, $third_party_element_value);
    drupal_set_message(t("Added New Define."));
  }
}
/**
 * Handler for deleting a connection
 * @param array $form
 * @param array $form_state
 */
function connection_delete($form, &$form_state) {
  $index = $form_state['clicked_button']['#name'][0];
  $eid = $form_state['values']['c' . strval($index) . '-eid'];
  $num_deleted = _delete_connection($eid);
  drupal_set_message(t("Delete Connection"));
}
/**
 * Handler for adding a connection
 * @param array $form
 * @param array $form_state
 */
function connection_add($form, &$form_state) {
  $portal_id = request_path();
  $portal_id = explode('/', $portal_id);
  $portal_id = end($portal_id);
  $drupal_element_key = $form_state['values']['drupal-element'];
  $third_party_element_key = $form_state['values']['third-party-key'];
  if ($drupal_element_key !== "" && $third_party_element_key !== "") {
    // Insert new connection
    $eid = _create_connection($portal_id, $drupal_element_key, $third_party_element_key);
    drupal_set_message(t("Added New Connection."));
  }
}
/**
 * Handler for deleting a file
 * @param array $form
 * @param array $form_state
 */
function delete_file($form, &$form_state) {
  $index = $form_state['clicked_button']['#name'][0];
  $filekid = $form_state['values']['f' . strval($index) . '-filekid'];
  $num_deleted = _delete_file($filekid);
  drupal_set_message(t("Delete File"));
}
/**
 * Handler for adding a file
 * @param array $form
 * @param array $form_state
 */
function file_add($form, &$form_state) {
  $portal_id = request_path();
  $portal_id = explode('/', $portal_id);
  $portal_id = end($portal_id);
  $file_drupal_element = $form_state['values']['file-drupal-element'];
  $file_third_party_key = $form_state['values']['file-third-party-key'];
  if ($file_drupal_element !== "" && $file_third_party_key !== "") {
    // Insert new connection
    $dkid = _create_file($portal_id, $file_third_party_key, $file_drupal_element);
    drupal_set_message(t("Add New File."));
  }
}
/**
 * Handler that saves all the elements in the portal
 * @param array $form
 * @param array $form_state
 */
function portal_save($form, &$form_state) {
  $portal_id = request_path();
  $portal_id = explode('/', $portal_id);
  $portal_id = end($portal_id);
  #dsm($form_state);
  $drupal_nid = $form_state['values']['drupalnid'];
  $description = $form_state['values']['description'];
  $imagenow_url = $form_state['values']['submitionurl'];
  $group = $form_state['values']['group'];
  if ($portal_id == 'new') {
    $node = node_load($drupal_nid);
    #dpm($node);
    global $user;
    #dpm($user);
    $eid = _create_portal($drupal_nid, $description, $imagenow_url, $group); //TODO: change
    drupal_set_message(t("New Portal Saved"));
    drupal_goto('/admin/config/content/form_forwarding/portal/' . strval($eid));
  }
  else {
    // Update Portal
    $update_portal = _update_portal($portal_id, /*$drupal_nid,*/ $description, $imagenow_url, $group);
    // Update defines
    $index = 0;
    $dkid = $form_state['values']['d' . strval($index) . '-dkid'];
    while ($dkid != NULL) {
      $third_party_key = $form_state['values']['d' . strval($index) . '-third-party-element-key'];
      $third_party_value = $form_state['values']['d' . strval($index) . '-third-party-element-value'];
      $update_connection = _update_define($dkid, $third_party_key, $third_party_value);
      $index++;
      $dkid = $form_state['values']['d' . strval($index) . '-dkid'];
    }
    // Add Define
    define_add($form, $form_state);
    // Update Connections
    $index = 0;
    $eid = $form_state['values']['c' . strval($index) . '-eid'];
    while ($eid != NULL) {
      $drupal_form_element = $form_state['values']['c' . strval($index) . '-drupal-form-element'];
      $third_party_key = $form_state['values']['c' . strval($index) . '-third-party-key'];
      $update_connection = _update_connection($eid, $drupal_form_element, $third_party_key);
      $index++;
      $eid = $form_state['values']['c' . strval($index) . '-eid'];
    }
    //Add Connection
    connection_add($form, $form_state);
    // Update files
    $index = 0;
    $filekid = $form_state['values']['f' . strval($index) . '-filekid'];
    while ($filekid != NULL) {
      $third_party_key = $form_state['values']['f' . strval($index) . '-third-party-key'];
      $drupal_form_element = $form_state['values']['f' . strval($index) . '-drupal-form-element'];
      $update_connection = _update_file($filekid, $third_party_key, $drupal_form_element);
      $index++;
      $filekid = $form_state['values']['f' . strval($index) . '-filekid'];
    }
    //Add File
    file_add($form, $form_state);
    drupal_set_message(t("Portal had been reconfigured"));
  }
  // redirect to the specific portal
}

/**
 * Searched the webform_component table for all the components on the form to
 * display them in the dropdowns for easy selection.
 * @param int $nid
 * @return array
 */
function _get_drupal_form_elements(&$nid) {
  $rtn = array();
  $fields['select'] = ' - Select- ';
  $files['select'] = ' - Select- ';
  $result = db_select('webform_component', 'components')
    ->fields('components', array('cid', 'form_key', 'type'))
    ->condition('nid', $nid, '=')
    ->orderBy('cid', 'ASC')
    ->execute();
  // Organize portal elements
  foreach ($result as $element) {
    #watchdog('CMO-PHP', 'Name: ' . $element->form_key  . ' => ' . $element->type);
    if ($element->type != 'markup' && $element->type != 'fieldset'){
      #watchdog("Made It", 'Name: ' . $element->form_key);
      if($element->type == 'file' || $element->type == 'multifile') {
        #watchdog("Is File", 'Name: ' . $element->form_key);
        $files[$element->form_key] = $element->form_key;
      } else {
        #watchdog("Is Field", 'Name: ' . $element->form_key);
        $fields[$element->form_key] = $element->form_key;
      }
    }
  }
  $rtn['fields'] = $fields;
  $rtn['files'] = $files;
  return $rtn;
}