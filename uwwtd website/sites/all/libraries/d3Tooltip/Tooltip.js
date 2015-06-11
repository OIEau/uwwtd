function Tooltip(tooltipId, width){
  if(jQuery("div#"+tooltipId).length==0){
    jQuery("body").append("<div class='d3js-tooltip' id='"+tooltipId+"'></div>");
  }
  if(width){
    jQuery("div#"+tooltipId).css("width", width);
  }

  hideTooltip();

  function showTooltip(content, event) {
    jQuery("div#"+tooltipId).html(content);
    jQuery("div#"+tooltipId).show();
    updatePosition(event);
  }

  function hideTooltip(){
    jQuery("div#"+tooltipId).hide();
  }

  function updatePosition(event){
    var ttid = "#"+tooltipId;
    var xOffset = 20;
    var yOffset = 10;

    var toolTipW = jQuery(ttid).width();
    var toolTipeH = jQuery(ttid).height();
    var windowY = jQuery(window).scrollTop();
    var windowX = jQuery(window).scrollLeft();
    /* la div d'accueil doit etre hors du flux des autres balise afin d'éviter les collisions ou décallage liés aux autres balises  */
    var curX = event.pageX;
    var curY = event.pageY;
    var ttleft = ((curX) < jQuery(window).width() / 2) ? curX - toolTipW - xOffset*2 : curX + xOffset;
    if (ttleft < windowX + xOffset){
      ttleft = windowX + xOffset;
    }
    var tttop = ((curY - windowY + yOffset*2 + toolTipeH) > jQuery(window).height() ? curY - toolTipeH - yOffset*2 : curY + yOffset);
    if (tttop < windowY + yOffset){
      tttop = curY + yOffset;
    }
    jQuery(ttid).css('top', tttop + 'px').css('left', ttleft + 'px');
  }

  return {
    showTooltip: showTooltip,
    hideTooltip: hideTooltip,
    updatePosition: updatePosition
  }
}
