<div id="icesp-comparative-edition-header">
  <p>
    <img id="icesp-comparative-edition-preview" 
         src="/islandora/object/<?php print $islandora_object->id; ?>/datastream/OBJ">
    <span id="icesp-comparative-edition-abstract"><?php print $mods_obj->abstract;?></span>
  </p>
</div>

<div id="icesp-comparative-edition-children-container">
  <h1>Witnesses</h1>

  <div id="icesp-tabs">
    <ul>
      <li><a href="#tabs-1">List View</a></li>
      <li><a href="#tabs-2">Grid View</a></li>
    </ul>
  
    <div id="tabs-1">
      <?php foreach ($children as $child) { ?>
      <div class="icesp-comparative-edition-child-container">
        <a href="/islandora/object/<?php print $child['pid']; ?>">
          <img class="icesp-comparative-edition-child-container-img" src="/islandora/object/<?php print $child['pid']; ?>/datastream/TN">
          <strong><?php print $child['label']; ?></strong>
          <p><?php print $child['abstract']; ?></p>
        </a>
      </div>
      <?php } ?>
    </div>
    
    <div id="tabs-2">
      <ul id="icesp-comparative-edition-child-grid-container">
      <?php foreach ($children as $child) { ?>
      <li>
        <a href="/islandora/object/<?php print $child['pid']; ?>">
          <p><img src="/islandora/object/<?php print $child['pid']; ?>/datastream/TN"></p>
          <p><?php print $child['label']; ?><p>
        </a>
      </li>
      <?php } ?>
      </ul>
    </div>
  </div>
</div>

