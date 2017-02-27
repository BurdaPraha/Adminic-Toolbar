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
      'nodes',
      'terms',
      'blocks',
      'media',
      'comments',
      'paragraphs',
      'views',
      'image_styles',
      'menus',
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
