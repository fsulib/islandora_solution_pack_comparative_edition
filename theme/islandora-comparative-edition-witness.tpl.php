<div id="icesp-witness-header">
  <p>
    <img id="icesp-witness-preview" 
         src="/islandora/object/<?php print $islandora_object->id; ?>/datastream/OBJ">
    <span id="icesp-witness-abstract"><?php print $mods_obj->abstract;?></span>
  </p>
</div>

<div id="icesp-witness-children-container">
  <h1>Pages</h1>
  <hr />

  <ul>
  
  <?php foreach ($children as $child) { ?>
    <li class="icesp-witness-child-container">
      <p><a href="/islandora/object/<?php print $child['pid']; ?>">
          <img src="/islandora/object/<?php print $child['pid']; ?>/datastream/TN"></a></p>
        <p class="icesp-witness-page-label"><strong><?php print $child['label']; ?></strong><p>
    </li>
  <?php } ?>

  </ul>  
</div>


