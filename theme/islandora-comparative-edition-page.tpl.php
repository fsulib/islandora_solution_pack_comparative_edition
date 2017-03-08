<?php
/**
 * Template file for page level objects 
 */
?>

<div id="icesp-page-header">
  
  <div id="switch-view-controls">
      
    <?php if($previous): ?>
      <span class="icesp-previous"><a href="/islandora/object/<?php print $previous['pid']; ?>">&#9668; Prev</a></span>
    <?php endif; ?>
    
    <span id="icesp-go-to-page">
    
    <select id="icesp-page-select">
    <?php foreach($siblings as $sibling): ?>    
    <option value="<?php print $sibling['pid']; ?>" <?php if ($sibling['pid'] == $current['pid']) { print('selected="selected"'); } ?>>
      <?php print $sibling['label']; ?>
    </option>  
    <?php endforeach; ?>
    </select>
      
    </span>
      
    <?php if($next): ?>
      <span class="icesp-next"><a href="/islandora/object/<?php print $next['pid']; ?>">Next &#9658;</a></span>
    <?php endif; ?>
    
    <button type="button" id="icesp-comparison-button">Comparison View</button>
    <button type="button" id="icesp-focus-button" class="icesp-button-active" disabled>Focus View</button>
  </div>
  
</div>  
  
<div id="icesp-comparison-tabs-container">
  
  <div id="icesp-tabs">
    <ul>
      <li><a href="#tab-1">Image</a></li>
      <li><a href="#tab-2">Transcript</a></li>
      <li><a href="#tab-3">TEI</a></li>
    </ul>
  
    <div id="tab-1">
      <?php print $page_image; ?>
    </div>
    
    <div id="tab-2">
      <?php print $display_html; ?>
    </div>
    
    <div id="tab-3">
      <?php print $page_tei; ?>
    </div>
  
  </div>
  
  <div id="icesp-tabs-comparison">
    <ul>
      <li><a href="#tab-comp-2">Transcript</a></li>
      <li><a href="#tab-comp-3">TEI</a></li>
    </ul>
    
    <div id="tab-comp-2">
      <?php print $display_html; ?>
    </div>
    
    <div id="tab-comp-3">
      <?php print $page_tei; ?>
    </div>
  
  </div>
  
</div>

<div id="icesp-dialog"><div id="icesp-comparison-table"></div></div>
