[![Build Status](https://secure.travis-ci.org/wdavidw/node-sigar.png)](http://travis-ci.org/wdavidw/node-sigar)

Node Sigar binding
==================

A complete binding to the [SIGAR](http://support.hyperic.com/display/SIGAR/Home) library.

Documentation is available on the [project website](http://www.adaltas.com/projects/node-sigar).

Note, documentation include important installation instruction for some platforms.

SIGAR is a cross platform interface for gathering system information. From the project website, such information include:

*   System memory, swap, cpu, load average, uptime, logins
*   Per-process memory, cpu, credential info, state, arguments, environment, open files
*   File system detection and metrics
*   Network interface detection, configuration info and metrics
*   TCP and UDP connection tables
*   Network route table

```
var sigar = require('sigar');
console.log(sigar.version()); // returns the version

// invoke the library
var Sigar = sigar.init();
console.log(sigar.cpu()); // {}

```

API
===

Sigar.mem();
```
> {
	ram: Number
	total: Number
	used: Number
	free: Number
	actual_used: Number
	actual_free: Number
	used_percent: Number
	free_percent: Number
}
```
Sigar.swap();
```
{
	total: Number
	used: Number
	free: Number
	page_in: Number
	page_out: Number
}
```
Sigar.uptime();
```
> 52883 // seconds
```
Sigar.loadavg();
```
[0.11, 0.13, 0.09]
```
Sigar.resourceLimit();
Sigar.whoList();
Sigar.sysInfo();
Sigar.fqdn();
// CPU
Sigar.cpu();
Sigar.cpuList();
Sigar.cpuInfoList();
// Process
Sigar.procList();
Sigar.procStat();
Sigar.procMem();
Sigar.procCred();
Sigar.procTime();
Sigar.procCpu();
Sigar.procState();
Sigar.procArgs();
Sigar.procEnv();
Sigar.procFd();
Sigar.procExe();
Sigar.procModules();
Sigar.procPort();
Sigar.threadCpu();
// Disk
Sigar.fileSystemList();
Sigar.fileSystemUsage();
Sigar.fileSystemPing();
// Network
Sigar.netInfo();
Sigar.netRouteList();
Sigar.netInterfaceList();
Sigar.netInterfaceConfig();
Sigar.netInterfaceConfigPrimary();
Sigar.netInterfaceStat();
Sigar.netConnectionList();
Sigar.netListenAddress();
Sigar.netStat();
Sigar.netStatPort();
Sigar.tcp();
Sigar.nfsClientV2();
Sigar.nfsServerV2();
Sigar.arpList();
// Misc
Sigar.rpcPing();



TODO
====

 * compile the entire library with gyp
 * fix up the tests (and improve on them)
 * utilize / reimplement / remove `sigar_format.c` functions
 * convert functions without args to be getters
 * provide async funcs for long-running actions
 * old / broken code still exists for converting network addresses (see `node_sigar_utils.cc::node_sigar_str2net_address()`)


LICENSE
=======

Node Sigar is licensed under the BSD license.
