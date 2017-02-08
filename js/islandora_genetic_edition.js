/* Javascript file for Genetic Editions Solution Pack Module */


jQuery(document).ready(function($) {
  
  $("#igesp-tabs").tabs();
  $("#igesp-tabs-comparison").tabs();
  
  $("#igesp-focus-button").click(function() {
    
    //disable focus button and enable comparison button
    $("#igesp-focus-button").prop('disabled', true);
    $("#igesp-comparison-button").removeClass('igesp-button-active');
    $("#igesp-focus-button").addClass('igesp-button-active');
    $("#igesp-comparison-button").removeAttr('disabled');
    
    //hide comparison view
    $("#igesp-tabs-comparison").css("display", "none");
    $("#igesp-tabs").removeClass('igesp-left-column');
    $("#igesp-tabs-comparison").removeClass('igesp-right-column');
    
  });
  
  $("#igesp-comparison-button").click(function() {
    
    //disable comparison button and enable focus button
    $("#igesp-comparison-button").prop('disabled', true);
    $("#igesp-focus-button").removeClass('igesp-button-active');
    $("#igesp-comparison-button").addClass('igesp-button-active');
    $("#igesp-focus-button").removeAttr('disabled');
    
    //show comparison view
    $("#igesp-tabs-comparison").css("display", "block");
    $("#igesp-tabs").addClass('igesp-left-column');
    $("#igesp-tabs-comparison").addClass('igesp-right-column');

  });
  
});