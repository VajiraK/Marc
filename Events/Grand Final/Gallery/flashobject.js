FlashObject=function(swf,id,w,h,ver,c){
this.swf=swf;
this.id=id;
this.width=w;
this.height=h;
this.version=ver||6;
this.align="middle";
this.redirect="";
this.sq=document.location.search.split("?")[1]||"";
this.altTxt="Please <a href='http://www.macromedia.com/go/getflashplayer'>upgrade your Flash Player</a>.";
this.bypassTxt="<p>Already have Flash Player? <a href='?detectflash=false&"+this.sq+"'>Click here if you have Flash Player "+this.version+" installed</a>.</p>";
this.params=new Object();
this.variables=new Object();
if(c)this.color=this.addParam('bgcolor',c);
this.addParam('quality','high');
this.doDetect=getQueryParamValue('detectflash');
}
FlashObject.prototype.addParam=function(name,value){
this.params[name]=value;
}
FlashObject.prototype.getParams=function(){
return this.params;
}
FlashObject.prototype.getParam=function(name){
return this.params[name];
}
FlashObject.prototype.addVariable=function(name,value){
this.variables[name]=value;
}
FlashObject.prototype.getVariable=function(name){
return this.variables[name];
}
FlashObject.prototype.getVariables=function(){
return this.variables;
}
FlashObject.prototype.getParamTags=function(){
var paramTags="";
for(var param in this.getParams()){
paramTags+='<param name="'+param+'" value="'+this.getParam(param)+'" />';
}
if(paramTags==""){
paramTags=null;
}
return paramTags;
}
FlashObject.prototype.getHTML=function(){
var flashHTML="";
if(window.ActiveXObject&&navigator.userAgent.indexOf('Mac')==-1){
flashHTML+='<object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="'+this.width+'" height="'+this.height+'" id="'+this.id+'" align="'+this.align+'">';
flashHTML+='<param name="movie" value="'+this.swf+'" />';
if(this.getParamTags()!=null){
flashHTML+=this.getParamTags();
}
if(this.getVariablePairs()!=null){
flashHTML+='<param name="flashVars" value="'+this.getVariablePairs()+'" />';
}
flashHTML+='</object>';
}
else{
flashHTML+='<embed type="application/x-shockwave-flash" src="'+this.swf+'" width="'+this.width+'" height="'+this.height+'" id="'+this.id+'" align="'+this.align+'"';
for(var param in this.getParams()){
flashHTML+=' '+param+'="'+this.getParam(param)+'"';
}
if(this.getVariablePairs()!=null){
flashHTML+=' flashVars="'+this.getVariablePairs()+'"';
}
flashHTML+='></embed>';
}
return flashHTML;
}
FlashObject.prototype.getVariablePairs=function(){
var variablePairs=new Array();
for(var name in this.getVariables()){
variablePairs.push(name+"="+escape(this.getVariable(name)));
}
if(variablePairs.length>0){
return variablePairs.join("&");
}
else{
return null;
}
}
FlashObject.prototype.write=function(elementId){
if(detectFlash(this.version)||this.doDetect=='false'){
if(elementId){
document.getElementById(elementId).innerHTML=this.getHTML();
}else{
document.write(this.getHTML());
}
}else{
if(this.redirect!=""){
document.location.replace(this.redirect);
}else{
if(elementId){
document.getElementById(elementId).innerHTML=this.altTxt+""+this.bypassTxt;
}else{
document.write(this.altTxt+""+this.bypassTxt);
}
}
}
}
function getFlashVersion(){
var flashversion=0;
if(navigator.plugins&&navigator.plugins.length){
var x=navigator.plugins["Shockwave Flash"];
if(x){
if(x.description){
var y=x.description;
flashversion=y.charAt(y.indexOf('.')-1);
}
}
}else{
result=false;
for(var i=15;i>=3&&result!=true;i--){
execScript('on error resume next: result = IsObject(CreateObject("ShockwaveFlash.ShockwaveFlash.'+i+'"))','VBScript');
flashversion=i;
}
}
return flashversion;
}
function detectFlash(ver){
if(getFlashVersion()>=ver){
return true;
}else{
return false;
}
}
function getQueryParamValue(param){
var q=document.location.search;
var detectIndex=q.indexOf(param);
var endIndex=(q.indexOf("&",detectIndex)!=-1)?q.indexOf("&",detectIndex):q.length;
if(q.length>1&&detectIndex!=-1){
return q.substring(q.indexOf("=",detectIndex)+1,endIndex);
}else{
return"";
}
}
if(Array.prototype.push==null){
Array.prototype.push=function(item){
this[this.length]=item;
return this.length;
}
}