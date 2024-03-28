/*
  MusicFormats Library
  Copyright (C) Jacques Menu 2016-2024

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, you can obtain one at http://mozilla.org/MPL/2.0/.

  https://github.com/jacques-menu/musicformats
*/


//----------------------------------------------------------------------------
// a download function
//----------------------------------------------------------------------------
function download (filename : std::string, text: std::string) : void {
  downloadMedia (filename, text, "text/plain;charset=utf-8,");
  // var element = document.createElement('a');
  // element.setAttribute('href', 'data:text/plain;charset=utf-8,' + encodeURIComponent(text));
  // element.setAttribute('download', filename);

  // element.style.display = 'none';
  // document.body.appendChild(element);
  // element.click();
  // document.body.removeChild(element);
}

function downloadMedia (filename : std::string, data: std::string, type: std::string) : void {
  var element = document.createElement('a');
  element.setAttribute('href', 'data:'+type + encodeURIComponent(data));
  element.setAttribute('download', filename);

  element.style.display = 'none';
  document.body.appendChild(element);
  element.click();
  document.body.removeChild(element);
}
