/* Javascript file for Comparative Editions Solution Pack Module */

jQuery(document).ready(function($) {
  
  $("#icesp-tabs").tabs();
  $("#icesp-tabs-comparison").tabs();
  
  $("#icesp-focus-button").click(function() {
    
    //disable focus button and enable comparison button
    $("#icesp-focus-button").prop('disabled', true);
    $("#icesp-comparison-button").removeClass('icesp-button-active');
    $("#icesp-focus-button").addClass('icesp-button-active');
    $("#icesp-comparison-button").removeAttr('disabled');
    
    //hide comparison view
    $("#icesp-tabs-comparison").css("display", "none");
    $("#icesp-tabs").removeClass('icesp-left-column');
    $("#icesp-tabs-comparison").removeClass('icesp-right-column');
    
  });
  
  $("#icesp-comparison-button").click(function() {
    
    //disable comparison button and enable focus button
    $("#icesp-comparison-button").prop('disabled', true);
    $("#icesp-focus-button").removeClass('icesp-button-active');
    $("#icesp-comparison-button").addClass('icesp-button-active');
    $("#icesp-focus-button").removeAttr('disabled');
    
    //show comparison view
    $("#icesp-tabs-comparison").css("display", "block");
    $("#icesp-tabs").addClass('icesp-left-column');
    $("#icesp-tabs-comparison").addClass('icesp-right-column');

  });
  
  $( "#icesp-page-select" ).change(function() {
    window.location.href =  location.origin + '/islandora/object/'+ this.value;
  });
  
  var comp_units = document.getElementsByClassName('icesp-comparable');
 
  for(var i=0;i<comp_units.length;i++){
    comp_units[i].addEventListener("click", function(){
      var mindex_location = window.location.href + "/mindex/" + this.id;
      var sentence_id = this.id;
      
      $.get(mindex_location, function( results_json ) {
        var results_object = JSON.parse(results_json);
        var keys = Object.keys(results_object);
        var result_html = '';
        
        keys.forEach(function(key) {
          var sentence_object = results_object[key];
          var sentence_keys  = Object.keys(sentence_object);
          sentence_keys.forEach(function(sentence_key) {
            result_html = result_html + sentence_key + sentence_object[sentence_key];
          });
        });
        
        $("#icesp-comparison-table").html(result_html);
        
        $( "#icesp-dialog" ).dialog({
          autoOpen: false,
          resizable: false,
          modal: true,
          width: 'auto'
        });
        
        $("#icesp-dialog").dialog('option', 'title', 'Version comparison of sentence id ' + sentence_id);
        $("#icesp-dialog").dialog("open");
        $(".icesp-table-row").blur();
      });      
      
    }, false);   
  }
  
});
