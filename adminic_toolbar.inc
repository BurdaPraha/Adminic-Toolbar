<?php

use Drupal\Core\Cache\Cache;
use Drupal\Core\Url;

function add_manage($type, $add_url, $list_url, $list_parametter = NULL, $crete_permissions = NULL, $add_bundle_url = NULL, $add_bundle_label = NULL) {
  $user = \Drupal::currentUser();

  if ($user->hasPermission('adminic toolbar use ' . $type)) {
    if (!is_null($type)) {
      $entity = \Drupal::entityTypeManager()->getDefinition($type);

      $admin_permissions = $entity->getAdminPermission();

      $link_templates = $entity->getLinkTemplates();
      if (is_null($list_parametter)) {
        $list_parametter = $type;
      }
      if (in_array('collection', $link_templates)) {
        $collection_url = $link_templates['collection'];
      }
      else {
        $collection_url = NULL;
      }

      $links = [];

      if ($user->isAuthenticated()) {
        $links_cache_contexts[] = 'user';

        $variantions = \Drupal::entityTypeManager()
          ->getStorage($type)
          ->loadMultiple();
        foreach ($variantions as $variantion) {

          $bundle = $variantion->id();
          $name = $variantion->label();

          $links[$bundle] = [];
          $links[$bundle]['name'] = t($name);
          $links[$bundle]['bundle'] = $bundle;
          $links[$bundle]['attributes'] = [
            'class' => ['content-links', 'content-' . $type . '-' . $bundle],
          ];

          // Add Route
          if ($crete_permissions != NULL) {
            $add_permissions = str_replace('{bundle}', $bundle, $crete_permissions);
          }
          if ($crete_permissions == NULL || (isset($add_permissions) && $user->hasPermission($add_permissions))) {
            if (!is_null($add_url)) {
              $links[$bundle]['add_url'] = Url::fromRoute($add_url, [$type => $bundle])
                ->toString();
            }
          }

          // Edit route
          if ($user->hasPermission($admin_permissions)) {
            $links[$bundle]['edit_url'] = Url::fromRoute(
              $variantion->toUrl()->getRouteName(),
              $variantion->toUrl()->getRouteParameters())
              ->toString();
          }
          // List route
          if (!is_null($list_url)) {
            $links[$bundle]['list_url'] = Url::fromRoute($list_url, [$list_parametter => $bundle])
              ->toString();
          }

        }

        $return = [];
        $return['#theme'] = 'adminic_content_bundler';

        if ($user->hasPermission($admin_permissions)) {

          if (!is_null($collection_url)) {
            $return['#collection_url'] = $collection_url;
          }

          if (is_null($add_bundle_url) && array_key_exists('add-form', $link_templates)) {
            $add_bundle_url = $link_templates['add-form'];
            $return['#add_bundle_url'] = $add_bundle_url;
          }
          elseif (!is_null($add_bundle_url)) {
            $return['#add_bundle_url'] = Url::fromRoute($add_bundle_url)
              ->toString();
          }

          if (is_null($add_bundle_label)) {
            $add_bundle_label = 'Add ' . $entity->getLabel();
          }
          $return['#add_bundle_text'] = $add_bundle_label;
        }
        $return['#links'] = $links;
        $return['#title'] = $entity->getLabel();
        $return['#attributes'] = [
          'class' => ['toolbar-menu', $type . '-add'],
        ];
        $return['#attributes'] = [
          '#cache' => [
            'contexts' => Cache::mergeContexts(['user.roles:authenticated'], $links_cache_contexts),
          ],
        ];

        return $return;
      }
    }
  }

  return NULL;
}
