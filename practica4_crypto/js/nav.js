$(document).ready(function(){
            $(window).scroll(function(){
                scroll = $(window).scrollTop();
                    if (scroll > 100) {
                        
                        $('.barra').css({"position":"fixed"});
                        $('.barra').css({"width":"100%"});
                        $('.barra').css({"top":"0"});
                        $('.barra').css({"background":"#fff"});
                        $('.barra a').css({"color":"#000"});
                        $('.logo').css({"color":"#000"});
                        $('.barra').css({"box-shadow":"rgba(0, 0, 0, 0.22) 6px 1px 1px"});
                        $('.barra').css({"z-index":"100"});  
                    }
                    else {
                        $('.barra').css({"position":"relative"});
                        $('.barra').css({"background":"transparent"});
                        $('.barra').css({"box-shadow":"0 0 0"});
                        $('.barra a').css({"color":"#fff "});
                        $('.logo').css({"color":"#fff"});
                    }
                })

            $('.menu-resp').click(function(){
                $('nav').slideToggle();
                
            })
        })