#include "serialmodule.h"
#include <stdio.h>
#include <stdlib.h>

int is_master = 0;
char is_port[32];
#define __ISMASTER__			"--is_master="
#define __ISPORT__				"--is_port="

#define TESTTEST "1234567"
int main(int argc, char *argv[]) {
	int i = 0;
	SP_SERIAL_INPUT_ST obj;
	FILE* fp = 0;
	int k = 0;
	int myid = 0;
	int ret = 0;
	char cfgpath[1024];
	SPSERIAL_ARR_LIST_LINED* objId = 0;

	snprintf(is_port, "%s", "COM2");

	for (i = 0; i < argc; ++i) {
		if (strstr(argv[i], __ISMASTER__)) {
			 k = sscanf(argv[i], __ISMASTER__"%d", &is_master);
			 spl_console_log("k = %d.", k);
			continue;
		}
		if (strstr(argv[i], __ISPORT__)) {
			k = sscanf(argv[i], __ISPORT__"%s", is_port);
			spl_console_log("k = %d.", k);
			continue;
		}
	}

#ifndef UNIX_LINUX
	snprintf(cfgpath, 1024, "C:/z/serialmodule/win32/Debug/simplelog.cfg");
#else
	snprintf(cfgpath, 1024, "simplelog.cfg");
#endif
	ret = spl_init_log(cfgpath);
	memset(&obj, 0, sizeof(obj));
	snprintf(obj.port_name, SPSERIAL_PORT_LEN, is_port);
	/*obj.baudrate = 115200;*/
	obj.baudrate = 9600;
	ret = spserial_module_init();
	if (ret) {
		return EXIT_FAILURE;
	}
	ret = spserial_inst_create(&obj, &myid);
	ret = spserial_get_objbyid(myid, &objId, 0);
	while (1) {
		spl_sleep(1);
		spl_console_log("-----------");
		if (!is_master) {
			spserial_inst_write_to_port(objId->item, TESTTEST, sizeof(TESTTEST));
		}
		fp = fopen("C:/z/serialmodule/win32/Debug/trigger_serial.txt", "r");
		if (fp) {
			break;
		}
		

		
	}
	if (fp) {
		fclose(fp);
	}
	if (myid > 0) {
		spserial_inst_del(myid);
	}
	spserial_module_close();
	spl_finish_log();
	return 0;
}

