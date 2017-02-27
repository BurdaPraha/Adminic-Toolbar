<?php

namespace Drupal\adminic_toolbar;

use Drupal\Core\StringTranslation\StringTranslationTrait;

/**
 * Class Permissions
 *
 * @package Drupal\channels_efs
 */
class Permissions {

  use StringTranslationTrait;

  /**
   * Get permissions for Channels Blocks.
   *
   * @return array
   *   Permissions array.
   */
  public function getPermissions() {
    $permissions = [];
    $blockCategories = [
      'node_type',
      'taxonomy_vocabulary',
      'media_bundle',
      'block_content_type',
      'view',
      'image_style',
      'menu',
      'comment_type',
      'paragraphs_type',
    ];

    foreach ($blockCategories as $category) {
      $permissions += [
        'adminic toolbar use ' . $category => [
          'title' => $this->t('Use %category', array('%category' => $category)),
        ],
      ];
    }

    return $permissions;
  }

}
