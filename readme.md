
Node Sigar binding
==================

A complete binding to the [SIGAR](http://support.hyperic.com/display/SIGAR/Home) library.

Documentation is available on the [project website](http://www.adaltas.com/projects/node-sigar.html).

Problems
--------

Those are minor issues which I wasn't yet able to resolve.

-	Calling `sigar_net_address_to_string` throw symbol error   
	Impacted functions: `netRouteListGet`, `netInterfaceConfigGet`, `netInterfaceConfigPrimaryGet`, `netConnectionListGet`
	Solution: import related source code in './src/node_sigar_format.cc'
-	`netconnectionlist()` doesn't handle flag, use '-1' for now
-	String to address function isn't working in `node_sigar_str2net_address`.
	Impacted function: `SigarNetStatPortGet`
	See: `./tests/TodoTest.coffee # Test testNetAdress`
-	Protocal in `sigar_proc_port_get` seems disregarded
-	No idea on how to use `rpcPing`, see `./tests/TodoTest.coffee # Test rpcPing`

Notes
-----

-	`sigar_net_connection_walk` is not implemented
-	`sigar_who_list_get` return 0 records (tested on OSX)
