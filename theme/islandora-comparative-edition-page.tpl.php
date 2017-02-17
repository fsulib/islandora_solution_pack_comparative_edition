<?php

/**
 * @file
 * This is the template file for comparative edition page objects
 */
?>

<div id="icesp-page-header">
  
  <div id="switch-view-controls">
    <button type="button" id="icesp-focus-button" class="icesp-button-active" disabled>Focus View</button>
    <button type="button" id="icesp-comparison-button">Comparison View</button>
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
