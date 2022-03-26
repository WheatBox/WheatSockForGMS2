alarm_set(0,2);
draw_set_color(c_red);
debug_text = "WheatSock is running.";

testnum = get_integer("输入测试编号：0 = 图片获取测试，1 = 视频获取测试（用时可能比较久）", 0);

ip_addr = "";
init_ret = "";
webGetFile_ret = "";
