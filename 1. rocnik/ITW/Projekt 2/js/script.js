$(document).ready(function(){
			$(".show_hide").hide();
			
			$(".action").click(function(){
        		$(".show_hide").toggle();
    		});


		    $("a[rel^='prettyPhoto']").prettyPhoto({social_tools:false, overlay_gallery: false, allow_expand: false, deeplinking:false, show_title: false, });
		  });