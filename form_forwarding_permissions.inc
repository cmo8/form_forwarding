<?php

require_once 'include/portal_permission.inc';
/**
 * The Third Party Portal Details file.
 * 
 * @author Sean Mogran <morganse@uwplatt.edu>
 */

/**
 * Third Party Admin Page. Builds the form for the user to use.
 */
function form_forwarding_permissions_admin() {
  $form = array();
  $form['#attached']['css'][] = drupal_get_path('module', 'form_forwarding') . '/css/form_forwarding_admin.css';
  //$form['#attached']['js'][] = drupal_get_path('module', 'form_forwarding') . '/js/form_forwarding_admin.js';
  $vocabulary = taxonomy_vocabulary_machine_name_load('webform_forwarding_groups');
  $opt = array();
  foreach (taxonomy_get_tree($vocabulary->vid) as $term) {
    $opt[$term->tid] = $term->name;
  }
  $permissions = _get_permissions();
################################################################################
  $form[]['#markup'] = t('<div id="roles">');
  $form[]['#markup'] = t('<h3>Permissions</h3>');
  $form[]['#markup'] = t('<table id="permissions-table">');
  $form[]['#markup'] = t('<thead>');
  $form[]['#markup'] = t('<th class="id-hide">ID</th>');
  $form[]['#markup'] = t('<th>Taxonomy Term</th>');
  $form[]['#markup'] = t('<th>User Role</th>');
  $form[]['#markup'] = t('<th>&nbsp;</th>');
  $form[]['#markup'] = t('</thead>');
  $form[]['#markup'] = t('<tbody>');

  for ($i = 0; $i < count($permissions); $i++) {
    $form['permission'][$i][]['#markup'] = t('<tr id=permission-row-"' . $permissions[$i]['ffid'] . '" class="odd">');
    $form['permission'][$i][]['#markup'] = t('<td class="id-hide">');
    $form['permission'][$i]['p' . strval($i) . '-ffpid'] = array(
      '#type' => 'textfield',
      '#title' => t('Permission ID'),
      '#title_display' => 'invisible',
      '#required' => true,
      '#disabled' => true,
      '#default_value' => $permissions[$i]['ffpid'],
    );
    $form['permission'][$i][]['#markup'] = t('</td>');
    $form['permission'][$i][]['#markup'] = t('<td class="term-id">');
    $form['permission'][$i]['p' . strval($i) . '-tid'] = array(
      '#type' => 'select',
      '#title' => t('Term ID'),
      '#title_display' => 'invisible',
      '#required' => true,
      '#multiple' => false,
      '#options' => $opt,
      '#default_value' => $permissions[$i]['tid'],
    );
    $form['permission'][$i][]['#markup'] = t('</td>');
    $form['permission'][$i][]['#markup'] = t('<td>');
    $form['permission'][$i]['p' . strval($i) . '-rid'] = array(
      '#type' => 'select',
      '#title' => t('Role ID'),
      '#title_display' => 'invisible',
      '#required' => true,
      '#multiple' => false,
      '#options' => user_roles(),
      '#default_value' => $permissions[$i]['rid'],
    );
    $form['permission'][$i][]['#markup'] = t('</td>');
    $form['permission'][$i][]['#markup'] = t('<td>');
    $form['permission'][$i]['p' . strval($i) . '-btndelete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#description' => t('Delete Permission'),
      '#name' => strval($i) . '_delete_permission',
      '#submit' => array('define_permission'),
    );
    $form['permission'][$i][]['#markup'] = t('</td>');
    $form['permission'][$i][]['#markup'] = t('</tr >');
  }
  $form['new-permission'][]['#markup'] = t('<tr id="define-row-new" class="odd">');
  $form['new-permission'][]['#markup'] = t('<td class="id-hide"></td>');
  $form['new-permission'][]['#markup'] = t('<td class="third-party-element-key">');
  $form['new-permission']['tid'] = array(
    '#type' => 'select',
    '#title' => t('Term ID'),
    '#title_display' => 'invisible',
    '#required' => false,
    '#multiple' => false,
    '#options' => $opt,
  );
  $form['new-permission'][]['#markup'] = t('</td>');
  $form['new-permission'][]['#markup'] = t('<td>');
  $form['new-permission']['rid'] = array(
    '#type' => 'select',
    '#title' => t('Role ID'),
    '#title_display' => 'invisible',
    '#required' => false,
    '#multiple' => false,
    '#options' => user_roles(),
    '#default_value' => $permissions[$i]['rid'],
  );
  $form['new-permission'][]['#markup'] = t('</td>');
  $form['new-permission'][]['#markup'] = t('<td>');
  $form['new-permission']['btnadd-define'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#description' => t('Add Permission'),
    '#name' => 'add_permission',
    '#submit' => array('permission_add'),
  );
  $form['new-permission'][]['#markup'] = t('</td>');
  $form['new-permission'][]['#markup'] = t('</tr >');

  $form[]['#markup'] = t('</tbody>');
  $form[]['#markup'] = t('</table>');

  $form[]['#markup'] = t('</div>');

  $form['btnSavePermission'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#description' => t('Save Permission'),
    '#name' => 'permission_save',
    '#submit' => array('permission_save'),
  );

  return $form;
}

function define_permission($form, &$form_state){
  dsm($form_state);
  $index = $form_state['clicked_button']['#name'][0];
  $ffpid = $form_state['values']['p' . strval($index) . '-ffpid'];
  
  $tid = $form_state['values']['tid'];
  $rid = $form_state['values']['rid'];
  _delete_permission($rid, $tid);
  
  drupal_set_message(t("Permission Deleted"));
}

function permission_add($form, &$form_state){
  dsm($form_state['values']);
  
  $tid = $form_state['values']['tid'];
  $rid = $form_state['values']['rid'];
  dsm("TID: " . $tid);
  dsm("RID: " . $rid);
  _create_permission($rid, $tid);
  
  drupal_set_message(t("New Permission Added"));
}


/**
 * Handler that saves all the elements in the portal
 * @param array $form
 * @param array $form_state
 */
function permission_save($form, &$form_state) {
  dsm($form_state);
//  $drupal_nid = $form_state['values']['drupalnid'];
//  $description = $form_state['values']['description'];
//  $imagenow_url = $form_state['values']['submitionurl'];
//  $group = $form_state['values']['group'];
//  if ($portal_id == 'new') {
//    $node = node_load($drupal_nid);
//    #dpm($node);
//    global $user;
//    #dpm($user);
//    $eid = _create_portal($drupal_nid, $description, $imagenow_url, $group); //TODO: change
//    drupal_set_message(t("New Portal Saved"));
//    drupal_goto('/admin/config/content/form_forwarding/portal/' . strval($eid));
//  }
//  else {
//    // Update Portal
//    $update_portal = _update_portal($portal_id, /* $drupal_nid, */ $description, $imagenow_url, $group);
//    // Update defines
//    $index = 0;
//    $dkid = $form_state['values']['d' . strval($index) . '-dkid'];
//    while ($dkid != NULL) {
//      $third_party_key = $form_state['values']['d' . strval($index) . '-third-party-element-key'];
//      $third_party_value = $form_state['values']['d' . strval($index) . '-third-party-element-value'];
//      $update_connection = _update_define($dkid, $third_party_key, $third_party_value);
//      $index++;
//      $dkid = $form_state['values']['d' . strval($index) . '-dkid'];
//    }
//    // Add Define
//    define_add($form, $form_state);
//    // Update Connections
//    $index = 0;
//    $eid = $form_state['values']['c' . strval($index) . '-eid'];
//    while ($eid != NULL) {
//      $drupal_form_element = $form_state['values']['c' . strval($index) . '-drupal-form-element'];
//      $third_party_key = $form_state['values']['c' . strval($index) . '-third-party-key'];
//      $update_connection = _update_connection($eid, $drupal_form_element, $third_party_key);
//      $index++;
//      $eid = $form_state['values']['c' . strval($index) . '-eid'];
//    }
//    //Add Connection
//    connection_add($form, $form_state);
//    // Update files
//    $index = 0;
//    $filekid = $form_state['values']['f' . strval($index) . '-filekid'];
//    while ($filekid != NULL) {
//      $third_party_key = $form_state['values']['f' . strval($index) . '-third-party-key'];
//      $drupal_form_element = $form_state['values']['f' . strval($index) . '-drupal-form-element'];
//      $update_connection = _update_file($filekid, $third_party_key, $drupal_form_element);
//      $index++;
//      $filekid = $form_state['values']['f' . strval($index) . '-filekid'];
//    }
//    //Add File
//    file_add($form, $form_state);
//    drupal_set_message(t("Portal had been reconfigured"));
//  }
  // redirect to the specific portal
  drupal_set_message(t("Portal had been reconfigured"));
}
