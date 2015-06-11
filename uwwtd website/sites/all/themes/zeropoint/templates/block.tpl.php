<!-- block --> 
<div class="block-wrapper <?php print $block_zebra .' block_'. $block_id; ?>">
  <div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="<?php print $classes; ?> <?php if ($themed_block): ?>themed-block<?php endif; ?>"<?php print $attributes; ?>>
    <?php print render($title_prefix); ?>
    <?php if ($block->subject): ?>
      <?php if ($themed_block): ?>
    <div class="block-icon pngfix"></div>
      <?php endif; ?>
    <h2 class="title block-title"<?php print $title_attributes; ?>><?php print $block->subject ?></h2>
      <?php endif; ?>
    <?php print render($title_suffix); ?>
    <div class="content"<?php print $content_attributes; ?>>
      <?php print $content ?>
    </div>
  </div>
</div>
<!-- /block --> 
