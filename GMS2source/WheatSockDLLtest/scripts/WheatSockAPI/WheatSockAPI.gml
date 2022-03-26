globalvar WheatSockDLL_fname, WheatSockDLL_Debugmode;
WheatSockDLL_fname = "main.dll"; // 你可以在Assets面板下Included Files里看到这个dll文件
WheatSockDLL_Debugmode = true; // 将该变量改为false以关闭show_debug_message的输出

function WheatSock_web_getIP(hostname) {
	// const char * WheatSock_web_getIP(const char * hostname);
	static func = external_define(WheatSockDLL_fname,"WheatSock_web_getIP",dll_cdecl,ty_string,1,ty_string);
	var ret = external_call(func,hostname);
	if(WheatSockDLL_Debugmode) show_debug_message(ret);
	return ret;
}

function WheatSock_init(ip_addr, port_char, is_ipv6) {
	// const char * WheatSock_init(const char * ip_addr, const char * portchar, double is_ipv6);
	static func = external_define(WheatSockDLL_fname,"WheatSock_init",dll_cdecl,ty_string,3,ty_string,ty_string,ty_real);
	var ret = external_call(func,ip_addr,port_char,is_ipv6);
	if(WheatSockDLL_Debugmode) show_debug_message(ret);
	return ret;
}

function WheatSock_webGetFile(getFilename, destFilename) {
	// const char * WheatSock_webGetFile(const char * getFilename, const char * destFilename);
	static func = external_define(WheatSockDLL_fname,"WheatSock_webGetFile",dll_cdecl,ty_string,2,ty_string,ty_string);
	var ret = external_call(func,getFilename,destFilename);
	if(WheatSockDLL_Debugmode) show_debug_message(ret);
	return ret;
}

function WheatSock_close() {
	// const char * WheatSock_close();
	static func = external_define(WheatSockDLL_fname,"WheatSock_close",dll_cdecl,ty_string,0);
	var ret = external_call(func);
	if(WheatSockDLL_Debugmode) show_debug_message(ret);
	return ret;
}

function WheatSock_getFile_setshowid(showid) {
	// void WheatSock_getFile_setshowid(double showid);
	static func = external_define(WheatSockDLL_fname,"WheatSock_getFile_setshowid",dll_cdecl,ty_string,1,ty_real);
	external_call(func,showid);
}

function WheatSock_getFile_set_htmlRemoveHead(TorF) {
	// void WheatSock_getFile_set_htmlRemoveHead(double TorF);
	static func = external_define(WheatSockDLL_fname,"WheatSock_getFile_set_htmlRemoveHead",dll_cdecl,ty_string,1,ty_real);
	external_call(func,TorF);
}

function WheatSock_uncompressFile(srcFilename, destFilename) {
	// const char * WheatSock_uncompressFile(const char * srcFilename, const char * destFilename);
	static func = external_define(WheatSockDLL_fname,"WheatSock_uncompressFile",dll_cdecl,ty_string,2,ty_string,ty_string);
	var ret = external_call(func,srcFilename,destFilename);
	if(WheatSockDLL_Debugmode) show_debug_message(ret);
	return ret;
}
