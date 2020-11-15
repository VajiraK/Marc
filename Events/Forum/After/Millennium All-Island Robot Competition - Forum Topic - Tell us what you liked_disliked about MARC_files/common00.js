// JavaScript Document

var helpMode = true;
var browserID = ""; 
var disabledObj = "";
var popUpWin = 0;

// Browser Object
function BrowserInfo()
{
	this.name = navigator.appName;
	this.codename = navigator.appCodeName;
	this.version = navigator.appVersion;
	this.userAgent = navigator.userAgent;
	this.platform = navigator.platform;
	this.javaEnabled = navigator.javaEnabled();
	this.screenWidth = screen.width;
	this.screenHeight = screen.height;
}

function setBrowserID() {
	
	var b = new BrowserInfo();
	var theVer = 0.0;
	
	browserID = "ie";

	if (b.name != "Microsoft Internet Explorer") {
		if (b.name == "Netscape" && b.userAgent.indexOf("Opera") == -1) {
			browserID = "ns";
		} else if (b.userAgent.indexOf("Opera") != -1) {
			browserID = "op";
		} else {
			browserID = "";
		}
	} else if (b.userAgent.indexOf("Opera") != -1) {
		browserID = "op";
	}
	if (b.version.indexOf("MSIE") != -1) {
		var temp = b.version.split("MSIE");
		theVer = parseFloat(temp[1]);
		if (theVer < 5.5) {
			browserID = "";
		}
	} else {
		theVer = parseFloat(b.version.substr(0, 4));
		if (theVer < 5.0) {
			browserID = "";
		}
	}
}

setBrowserID();

function toggle (node1) {
	if (document.getElementById && document.createTextNode) {
		var node2 = node1;
		while ((node2 = node2.parentNode) && !/tr/i.test(node2.nodeName))
			continue;
		while ((node2 = node2.nextSibling) && !/tr/i.test(node2.nodeName))
			continue;
		if (null != node2) {
			var cnames = 'expandcontract', i = 0;
			node2.className = cnames.replace(node2.className, '');
			node1 = node1.childNodes;
			while ((node1 = node1.item(i++)) && !/img/i.test(node1.nodeName))
				continue;
			if (null != node1)
				node1.src = (/loginheadon/.test(node1.src)) ? node1.src.replace(/loginheadon/, 'loginhead') : node1.src.replace(/loginhead/, 'loginheadon');
		}
	}
	return false;
}

function hLight (obj) {
	tab = obj.parentNode
	for (i=0;i<tab.rows.length;i++)
	tab.rows[i].style.background="#343843"
	obj.style.background='#4A4E58'
}

function CheckNumeric() {
   // Get ASCII value of key that user pressed
   var key = window.event.keyCode;

   // Was key that was pressed a numeric character (0-9)?
   if ( key > 47 && key < 58 )
      return; // if so, do nothing
   else
      window.event.returnValue = null; // otherwise, 
	                               // discard character
}

function enableHelp() {
	var hlpen = document.getElementById("helpEn");
	if (hlpen.checked) helpMode = true;
	else helpMode = false;
}

function showHelp(obj) {
	
	if (!helpMode) return; // exit if the show hints is switched off
	
	var xy = Array();
	var s = 0; // slack length
	var tip = document.getElementById("ToolTip");
	hideHelp();
	
	if (!obj || !tip) return;
	var h = parseInt(obj.offsetHeight);
	var w = parseInt(obj.offsetWidth);
	
	xy = getPosXY(obj);
	if (browserID == "ie" && obj.opponent) {
		document.getElementById(obj.opponent).style.visibility = "hidden";
		disabledObj = obj.opponent;
	}
	
	//h += xy[1];
	tip.style.left = xy[0] + "px";
	tip.style.top = (xy[1] + h) + "px";
	tip.style.width = w>64?w + "px":"auto";
	tip.style.height = "auto";
	tip.innerHTML = "<p>" + obj.title + "</p>";
	tip.style.display = "block";
}

function hideHelp() {
	var tip = document.getElementById("ToolTip");
	tip.style.display = "none";
	if (browserID == "ie" && disabledObj != "") {
		document.getElementById(disabledObj).style.visibility = "visible";
		disabledObj = "";
	}
}

function getPosXY(obj) {
	var curleft = 2;
	var curtop = 2;

	if (obj.offsetParent)
	{
		while (obj.offsetParent)
		{
			curleft += obj.offsetLeft
			curtop += obj.offsetTop
			obj = obj.offsetParent;
		}
	}
	else if (obj.x) {
		curleft += obj.x;
		curtop += obj.y;
	}
	
	return Array(curleft, curtop);
		
}

function showErrFld(fldID) {
	if (!fldID) return;
	if (fldID == "") return;
	if (!document.getElementById(fldID)) return;
	fldObj = document.getElementById(fldID);
	
	fldObj.className = fldObj.className + " errorInput";
}

function makeLinks(pText) {
	var regex = new RegExp(/(http:\/\/[^ \f\n\r\t\v\u00A0\u2028\u2029\<]+)/g);
	return pText.replace(regex, "<a href='$1' target='_blank' class='textlink'>$1</a>");	
}


function popUpWindow(URLStr, left, top, width, height) {
	if(popUpWin) {
		if(!popUpWin.closed) popUpWin.close();
	}
	popUpWin = open(URLStr, 'popUpWin', 'toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=no,resizable=yes,copyhistory=no,width='+width+',height='+height+',left='+left+', top='+top+',screenX='+left+',screenY='+top+'');
}

