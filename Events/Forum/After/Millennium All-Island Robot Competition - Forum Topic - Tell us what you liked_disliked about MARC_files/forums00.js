// JavaScript Document

function cleanText(pText) {
	var regex = new RegExp(/([\~\!\@\#\$\%\^\&\*\-\_\+\=\|\:\;\,\.\/\?\<\>]){4,}/gi);
	return pText.replace(regex, "$1&hellip;");
}

function createTopicIndex(idx, obj) {
	
	if (!idx || !obj) return;
	
	var ostr = "<table width=\"100%\" border=\"0\" cellpadding=\"2\" cellspacing=\"1\" bgcolor=\"#25272E\" id=\"forumTable\">";
	ostr += "<tr class=\"textpath\">";
	ostr += "<td height=\"25\" align=\"center\" bgcolor=\"#22252D\"></td>";
	//ostr += "<td width=\"5%\" height=\"25\" align=\"center\" bgcolor=\"#22252D\"><b>User</b></td>";
	ostr += "<td width=\"100%\" bgcolor=\"#22252D\" ><b>Topic</b></td>";
	ostr += "<td height=\"25\" align=\"center\" bgcolor=\"#22252D\">Replies</td>";
	ostr += "</tr>";
	
	if (idx.length > 0) {
		//for (var i = idx.length - 1; i >= 0; i--) { // decending order
		var d = null;
		for (var i = 0; i < idx.length; i++) { // acending order
		// the new order: 0 - topic id, 1 - lastupdate, 2 - username, 3 - topic title, 4 - body, 5 - total replies
			d = idx[i][1].split(" ");
			ostr += "<tr class=\"topic\">";
			ostr += "<td bgcolor=\"#343843\" class=\"date\">" + d[0] + "<br>" + d[1] + "</td>";
		//	ostr += "<td bgcolor=\"#343843\">" + idx[i][2] + "</td>";
			ostr += "<td bgcolor=\"#343843\">";
			ostr += "  <table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"1\" style=\"padding:8px;\" >";
			ostr += "    <tr><td class=\"username\">" + idx[i][2] + "&nbsp;&raquo;&nbsp;</td><td width=\"100%\"><a href=\"forum.php?forum_action=viewtopic&topicid=" + idx[i][0] + "\" class=\"textlink\">" + cleanText(idx[i][3]) + "</a></td></tr>";
			ostr += "    <tr><td colspan=2>" + cleanText(idx[i][4]) + "</td></tr>";
			ostr += "  </table>";
			ostr += "</td>";
			ostr += "<td bgcolor=\"#343843\" align=\"center\">" + (idx[i][5]==0?"-":idx[i][5]) + "</td>";
			ostr += "</tr>";
		}
	} else {
		ostr += "<tr class=\"topic\">";
		ostr += "<td bgcolor=\"#343843\" colspan=\"4\"><center>No Topics Found</center></td></tr>";
	}
	
	ostr += "</table>";
	
	document.getElementById(obj).innerHTML = ostr;


}

function createTopTenIndex(idx, obj) {
	
	if (!idx || !obj) return;
	
	var ostr = "<table width=\"368\" border=\"0\" cellpadding=\"2\" cellspacing=\"1\" bgcolor=\"#25272E\" id=\"forumTable\">";
	
	if (idx.length > 0) {
		for (var i = 0; i < idx.length; i++) { // decending order
			ostr += "<tr class=\"topic\" height=\"15\">";
			ostr += "<td class=\"date\">" + idx[i][3] + "</td><td bgcolor=\"#3C414B\"><a href=\"forum.php?forum_action=viewtopic&topicid=" + idx[i][0] + "\" class=\"textlink\">" + idx[i][1] + "</a></td>";
			ostr += "</tr>";
		}
	} else {
		ostr += "<tr class=\"topic\"><td bgcolor=\"#343843\"><center>No Topics Found</center></td></tr>";
	}
	
	ostr += "</table>";
	
	document.getElementById(obj).innerHTML = ostr;


}

function createReplies(idx, obj) {
	
	if (!idx || !obj) return;
			
	var ostr = "<table width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\" id=\"replyTable\">";
	
	if (idx.length > 0) {
		for (var i = 0; i < idx.length; i++) { // acending order
			ostr += "<table class=\"replyTable\" width=\"100%\" border=\"0\" cellpadding=\"0\" cellspacing=\"0\">";
			ostr += "<tr>";
			ostr += "<td><img src=\"images/forumreply_tl.gif\" width=\"7\" height=\"23\"></td>";
			ostr += "<td background=\"images/forumreply_tbg.gif\" width=\"100%\"><table><tr><td width=\"100%\"><span class=\"user\">" + idx[i][1] + "</span></td><td class=\"date\">" + idx[i][3] + "</td></tr></table></td>";
			ostr += "<td><img src=\"images/forumreply_tr.gif\" width=\"7\" height=\"23\"></td>";
			ostr += "</tr>";
			ostr += "<tr>";
			ostr += "<td background=\"images/forumreply_l.gif\"></td>";
			ostr += "<td height=\"31\" background=\"images/forumreply_bg.gif\">";
			ostr += "<div class=\"reply\">" + makeLinks(idx[i][2]) + "</div>";
			ostr += "</td>";
			ostr += "<td background=\"images/forumreply_r.gif\"></td>";
			ostr += "</tr>";
			ostr += "<tr>";
			ostr += "<td><img src=\"images/forumreply_bl.gif\" width=\"7\" height=\"7\"></td>";
			ostr += "<td background=\"images/forumreply_b.gif\"></td>";
			ostr += "<td><img src=\"images/forumreply_br.gif\" width=\"7\" height=\"7\"></td>";
			ostr += "</tr>";
			ostr += "</table>";
		}
	} else {
			ostr += "<tr><td colspan=2 bgcolor=\"#343843\" class=\"reply\"><i>No replies posted for this topic</i></td></tr>";
	}
	
	ostr += "</table>";
	
	document.getElementById(obj).innerHTML = ostr;


}


function submitPost() {
	var oa = submitPost.arguments;
	if (oa.length == 0) return true;
	
	var okay = true;
	
	for (var i = 0; i < oa.length; i++) {
		if (document.getElementById(oa[i]).value == "" || document.getElementById(oa[i]).value == null) {
			alert("Oops, you haven\'t filled some fields.\nCan\'t post anything without all required information.");
			okay = false;
			break;
		}
	}
	return okay;
}