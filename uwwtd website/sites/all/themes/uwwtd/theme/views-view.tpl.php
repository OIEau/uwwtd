<?php

/**
 * @file
 * Main view template.
 *
 * Variables available:
 * - $classes_array: An array of classes determined in
 *   template_preprocess_views_view(). Default classes are:
 *     .view
 *     .view-[css_name]
 *     .view-id-[view_name]
 *     .view-display-id-[display_name]
 *     .view-dom-id-[dom_id]
 * - $classes: A string version of $classes_array for use in the class attribute
 * - $css_name: A css-safe version of the view name.
 * - $css_class: The user-specified classes names, if any
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 *
 * @ingroup views_templates
 */
?>
<div class="<?php print $classes; ?>">
  <?php print render($title_prefix); ?>
    <?php if (!empty($variables['view']) && isset($variables['view']->name) && $variables['view']->name === 'error_list') : ?>
        <h1>Agglomerations without treament plant</h1>
        <?php
        $aggsWithoutTp = variable_get(AGGS_WITHOUT_TP, array());
        ?>
        <?php if (!empty($aggsWithoutTp)) : ?>
            <p>
                <?php foreach ($aggsWithoutTp as $aggWithoutTp) : ?>
                    <?php print $aggWithoutTp; ?>,
                <?php endforeach ?>
            </p>
        <?php else : ?>
            <p>All agglomerations are linked to a treament plant.</p>
        <?php endif; ?>
        <h1>Treament plants without agglomeration</h1>
        <?php
        $tpsWithoutAgg = variable_get(TPS_WITHOUT_AGG, array());
        ?>
        <?php if (!empty($tpsWithoutAgg)) : ?>
            <p>
                <?php foreach ($tpsWithoutAgg as $tpWithoutAgg) : ?>
                    <?php print $tpWithoutAgg; ?>,
                <?php endforeach ?>
            </p>
        <?php else : ?>
            <p>All treament plants are linked to an agglomerations.</p>
        <?php endif; ?>
        <h1>Discharging points without treament plant</h1>
        <?php
        $dpsWithoutTp = variable_get(DPS_WITHOUT_TP, array());
        ?>
        <?php if (!empty($dpsWithoutTp)) : ?>
            <p>
                <?php foreach ($dpsWithoutTp as $dpWithoutTp) : ?>
                    <?php print $dpWithoutTp; ?>,
                <?php endforeach ?>
            </p>
        <?php else : ?>
            <p>All discharging points are linked to a treament plant.</p>
        <?php endif; ?>
        <p class="clear"></p>
    <?php endif; ?>
  <?php if ($title): ?>
    <?php print $title; ?>
  <?php endif; ?>
  <?php if (!$title && $view->human_name != ""): ?>
    <?php print "<h1>" . $view->human_name . "</h1>"; ?>
  <?php endif; ?>
  <?php print render($title_suffix); ?>
  <?php if ($header): ?>
    <div class="view-header">
      <?php print $header; ?>
    </div>
  <?php endif; ?>

  <?php if ($exposed): ?>
    <div class="view-filters">
      <?php print $exposed; ?>
    </div>
  <?php endif; ?>

  <?php if ($attachment_before): ?>
    <div class="attachment attachment-before">
      <?php print $attachment_before; ?>
    </div>
  <?php endif; ?>

  <?php if ($rows): ?>
    <div class="view-content">
      <?php print $rows; ?>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

  <?php if ($pager): ?>
    <?php print $pager; ?>
  <?php endif; ?>

  <?php if ($attachment_after): ?>
    <div class="attachment attachment-after">
      <?php print $attachment_after; ?>
    </div>
  <?php endif; ?>

  <?php if ($more): ?>
    <?php print $more; ?>
  <?php endif; ?>

  <?php if ($footer): ?>
    <div class="view-footer">
      <?php print $footer; ?>
    </div>
  <?php endif; ?>

  <?php if ($feed_icon): ?>
    <div class="feed-icon">
      <?php print $feed_icon; ?>
    </div>
  <?php endif; ?>

</div><?php /* class view */ ?>
