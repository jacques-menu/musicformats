/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2023

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/



function mylog(text, level) {
  let dest = document.getElementById("logs");
  if (level==1)     text = '<span class="warning">'+text+'</span>';
  else if (level==2)   text = '<span class="error">'+text+'</span>';
  dest.innerHTML += text + "\n";
}

var oldcons= console;
var console =(function(oldCons){
    return {
        log: function(text){
      oldCons.log(text);
      mylog(text, 0);
        },
        warn: function (text) {
            oldCons.warn(text);
      mylog(text, 1);
        },
        error: function (text) {
            oldCons.error(text);
      mylog(text, 2);
        }
    };
}(window.console));
