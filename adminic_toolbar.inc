<?php

use Drupal\Core\Cache\Cache;
use Drupal\Core\Url;

function manage_nodes() {
  return add_manage(
    'node',
    'node_type',
    'node.add',
    'system.admin_content',
    'type',
    'node.type_add'
  );
}

function manage_terms() {
  return add_manage(
    'taxonomy',
    'taxonomy_vocabulary',
    'entity.taxonomy_term.add_form',
    'entity.taxonomy_vocabulary.overview_form',
    NULL,
    'entity.taxonomy_vocabulary.add_form',
    'Add Vocabulary'
  );
}

function manage_media() {
  return add_manage(
    'media_entity',
    'media_bundle',
    'entity.media.add_form',
    'view.media.media_page_list',
    'provider'
  );
}

function manage_blocks() {
  return add_manage(
    'block_content',
    'block_content_type',
    'block_content.add_form',
    'entity.block_content.collection',
    NULL,
    'block_content.type_add'
  );
}

function manage_paragraphs() {
  return add_manage(
    'paragraphs',
    'paragraphs_type',
    NULL,
    NULL,
    NULL,
    'paragraphs.type_add'
  );
}

function manage_views() {
  return add_manage(
    'views_ui',
    'view',
    NULL,
    NULL,
    NULL,
    'views_ui.add'
  );
}

function manage_image_styles() {
  return add_manage(
    'image',
    'image_style',
    NULL,
    NULL,
    NULL,
    'image.style_add'
  );
}

function manage_menus() {
  return add_manage(
    'menu_ui',
    'menu',
    'entity.menu.add_form',
    'entity.menu.edit_form'
  );
}

function manage_comments() {
  return add_manage(
    'comment',
    'comment_type',
    NULL,
    'comment.admin'
  );
}

function add_manage($module, $type, $add_url, $list_url, $list_parametter = NULL, $add_bundle_url = NULL, $add_bundle_label = NULL) {
  if (\Drupal::moduleHandler()->moduleExists($module) == TRUE) {
    $user = \Drupal::currentUser();
    $entity = \Drupal::entityTypeManager()->getDefinition($type);

    $admin_permissions = $entity->getAdminPermission();

    $link_templates = $entity->getLinkTemplates();
    if(is_null($list_parametter)) {
      $list_parametter = $type;
    }

    $collection_url = $link_templates['collection'];

    $links = [];

    if ($user->isAuthenticated() && $user->hasPermission($admin_permissions)) {
      $links_cache_contexts[] = 'user';

      $variantions = \Drupal::entityTypeManager()->getStorage($type)->loadMultiple();
      //kint($variantions);
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
        if(!is_null($add_url)) {
          $links[$bundle]['add_url'] = Url::fromRoute($add_url, [$type => $bundle])
            ->toString();
        }

        // Edit route
        $links[$bundle]['edit_url'] = Url::fromRoute(
          $variantion->toUrl()->getRouteName(),
          $variantion->toUrl()->getRouteParameters())
          ->toString();

        //List route
        if(!is_null($list_url)) {
          $links[$bundle]['list_url'] = Url::fromRoute($list_url, [$list_parametter => $bundle])
            ->toString();
        }

      }

      $return = [];
      $return['#theme'] = 'adminic_content_bundler';

      if(!is_null($collection_url)) {
        $return['#collection_url'] = $collection_url;
      }

      if(is_null($add_bundle_url) && array_key_exists('add-form', $link_templates)) {
        $add_bundle_url = $link_templates['add-form'];
        $return['#add_bundle_url'] = $add_bundle_url;
      }
      elseif(!is_null($add_bundle_url)) {
        $return['#add_bundle_url'] = Url::fromRoute($add_bundle_url)->toString();
      }

      if(is_null($add_bundle_label)) {
        $add_bundle_label = 'Add ' . $entity->getLabel();
      }
      $return['#add_bundle_text'] = $add_bundle_label;

      $return['#links'] = $links;
      $return['#title'] = $entity->getLabel();
      $return['#attributes'] = [
        'class' => ['toolbar-menu', $type . '-add'],
      ];
      $return['#attributes'] = [
        '#cache' => [
          'contexts' => Cache::mergeContexts(array('user.roles:authenticated'), $links_cache_contexts),
        ]
      ];

      return $return;
    }
  }

  return NULL;
}