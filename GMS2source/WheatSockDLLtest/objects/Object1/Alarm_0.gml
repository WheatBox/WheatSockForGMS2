if(testnum == 1) {
	var hostname = "vdse.bdstatic.com";
	var getfilepos = "/192d9a98d782d9c74c96f09db9378d93.mp4";
	var destfilepos = program_directory + "/video.mp4";
	// 重要：
	// program_directory 为编译后的exe目录！！！
	// 如果是不编译的情况下进行调试，请改成别的路径，最好是固定路径！！
}
else {
	// 这里还有一个关于图片获取的范例：
	var hostname = "a1.qpic.cn";
	var getfilepos = "/psc?/V10PriMf0cyQiK/bqQfVz5yrrGYSXMvKr.cqSf8NoZdgMZiMnrVv9bVGsbk4.QQzbg5ygtKB.eCI8YbwNFI9XfgyVQnBEuNuUZkGJpElI6Hs1bHxJbdiCrwUH8!/c&ek=1&kp=1&pt=0&bo=AAX5BAAF.QQRECc!&tl=3&vuin=1947791814&tm=1647964800&sce=60-2-2&rf=0-0";
	var destfilepos = program_directory + "/image.jpg";
}

// WheatSock_getFile_set_htmlRemoveHead(false);
// WheatSock_getFile_setshowid(1);

ip_addr = WheatSock_web_getIP(hostname);

init_ret = WheatSock_init(hostname,"80",false);
webGetFile_ret = "";
if(string_copy(init_ret,0,14) != "connect Error!") {
	webGetFile_ret = WheatSock_webGetFile(getfilepos,destfilepos);
}

WheatSock_close();

draw_set_color(c_green);
debug_text = "WheatSock over.";
