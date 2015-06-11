<!-- node --> 
<div id="node-<?php print $node->nid; ?>" class="node <?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print $user_picture; ?>

  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
  <h2 class="title"<?php print $title_attributes; ?>><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  
  <div class="meta">
    <?php if ($display_submitted): ?>
    <div class="submitted">
    <?php print t('!datetime &#151; !username', array('!username' => $name, '!datetime' => $date)); ?>
    </div>
    <?php endif; ?>
  </div>
  
  <?php if (!empty($content['links']['terms'])): ?>
    <div class="terms>
      <?php print render($content['links']['terms']); ?>
    </div>
  <?php endif; ?>
  
  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <?php if ($content['links']): ?>
  <div class="links">
    <?php print render($content['links']); ?>
  </div>
  <?php endif; ?>
  
  <?php print render($content['comments']); ?>
</div>
<!-- /node-<?php print $node->nid; ?> -->
