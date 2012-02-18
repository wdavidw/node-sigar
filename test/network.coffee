
should = require 'should'
sigar = require '..'

describe 'network', ->

	s = sigar()

	it 'should return info', ->
		netInfo = s.netInfo()
		# console.log 'netInfo', netInfo
		Object.keys(netInfo).should.eql [
			'default_gateway', 'default_gateway_interface', 'host_name', 'domain_name', 'primary_dns', 'secondary_dns'
		]

	it 'should return route list', ->
		netRouteList = s.netRouteList()
		netRouteList.length.should.be.above 0
		for netRoute in netRouteList
			# console.log 'netRoute', netRoute
			Object.keys(netRoute).should.eql [
				'destination', 'gateway', 'mask', 'flags', 'refcnt', 'use', 'metric', 'mtu', 'window', 'irtt', 'ifname'
			]

	it 'should return interface list', ->
		netInterfaceList = s.netInterfaceList()
		netInterfaceList.length.should.be.above 0
		for netInterface in netInterfaceList
			/^[\w]+$/.test(netInterface).should.be.ok # Note: might be too restricive

	it 'should return config for a give interface', ->
		netInterfaceList = s.netInterfaceList()
		for netInterface in netInterfaceList
			netInterfaceConfig = s.netInterfaceConfig netInterface
			Object.keys(netInterfaceConfig).should.eql [
				'name', 'type', 'description', 'hwaddr', 
				'address', 'destination', 'broadcast', 'netmask', 
				'address6', 'prefix6_length', 'scope6', 'flags', 
				'mtu', 'metric', 'tx_queue_len'
			]

	it 'should return primary interface configuration', ->
		netInterfaceConfigPrimary = s.netInterfaceConfigPrimary()
		# console.log 'netInterfaceConfigPrimary', netInterfaceConfigPrimary
		Object.keys(netInterfaceConfigPrimary).should.eql [
			'name', 'type', 'description', 'hwaddr', 
			'address', 'destination', 'broadcast', 'netmask', 
			'address6', 'prefix6_length', 'scope6', 'flags', 
			'mtu', 'metric', 'tx_queue_len'
		]

	it 'should return stat for a given interface', ->
		netInterfaceList = s.netInterfaceList()
		for netInterface in netInterfaceList
			netInterfaceStat = s.netInterfaceStat netInterface
			# console.log 'netInterfaceStat', netInterface, netInterfaceStat
			Object.keys(netInterfaceStat).should.eql [
				'rx_packets', 'rx_bytes', 'rx_errors', 'rx_dropped', 
				'rx_overruns', 'rx_frame', 'tx_packets', 'tx_bytes', 
				'tx_errors', 'tx_dropped', 'tx_overruns', 'tx_collisions', 
				'tx_carrier', 'speed'
			]

	it 'should return network connection list', ->
		netConnectionList = s.netConnectionList -1
		netConnectionList.length.should.be.above 0
		for netConnection in netConnectionList
			# console.log 'netConnection', netConnection
			Object.keys(netConnection).should.eql [
				'local_port', 'local_address', 'remote_port', 
				'remote_address', 'uid', 'inode', 'type', 
				'state', 'send_queue', 'receive_queue'
			]

	it 'should return listen address for a give port', ->
		try
			netListenAddress = s.netListenAddress 22
			# console.log 'netListenAddress', netListenAddress
			netListenAddress.should.eql '0.0.0.0'
		catch e
			# SSH port 22 not open
			e.message.should.eql 'sigar_net_listen_address_get error: 2 (No such file or directory)'

	it 'should return stat', ->
		netStat = s.netStat -1
		# Note, tcp_states are:
		# SIGAR_TCP_ESTABLISHED = 1, SIGAR_TCP_SYN_SENT, SIGAR_TCP_SYN_RECV, SIGAR_TCP_FIN_WAIT1, 
		# SIGAR_TCP_FIN_WAIT2, SIGAR_TCP_TIME_WAIT, SIGAR_TCP_CLOSE, SIGAR_TCP_CLOSE_WAIT, SIGAR_TCP_LAST_ACK, 
		# SIGAR_TCP_LISTEN, SIGAR_TCP_CLOSING, SIGAR_TCP_IDLE, SIGAR_TCP_BOUND, SIGAR_TCP_UNKNOWN
		# console.log 'netStat', netStat
		Object.keys(netStat).should.eql [
			'tcp_states', 'tcp_inbound_total', 'tcp_outbound_total', 'all_inbound_total', 'all_outbound_total'
		]
		netStat.tcp_states.length.should.eql 14

	it 'should return stat for a give port', ->
		netStatPort = s.netStatPort -1, '127.0.0.1', 22
		# console.log 'netStatPort', netStatPort
		Object.keys(netStatPort).should.eql [
			'tcp_states', 'tcp_inbound_total', 'tcp_outbound_total', 'all_inbound_total', 'all_outbound_total'
		]
		netStatPort.tcp_states.length.should.eql 14

	it 'should return tcp info', ->
		tcp = s.tcp()
		# console.log 'tcp', tcp
		Object.keys(tcp).should.eql [
			'active_opens', 'passive_opens', 'attempt_fails', 'estab_resets', 'curr_estab', 'in_segs', 'out_segs', 'retrans_segs', 'in_errs', 'out_rsts'
		]

	it 'should return nfsClientV2 info', ->
		try
			cpu = s.nfsClientV2()
			console.log 'nfsClientV2', nfsClientV2
			Object.keys(nfsClientV2).should.eql [
				'null', 'getattr', 'setattr', 'root', 'lookup', 'readlink', 
				'read', 'writecache', 'write', 'create', 'remove', 'rename', 'link', 'symlink', 'mkdir', 'rmdir', 'readdir', 'fsstat'
			]
		catch e # OSX
			e.message.should.eql 'sigar_nfs_client_v2_get error: 20001 (This function has not been implemented on this platform)'

	it 'should return nfsServerV2 info', ->
		try
			cpu = s.nfsServerV2()
			console.log 'nfsServerV2', nfsServerV2
			Object.keys(nfsServerV2).should.eql [
				'null', 'getattr', 'setattr', 'root', 'lookup', 'readlink', 
				'read', 'writecache', 'write', 'create', 'remove', 'rename', 'link', 'symlink', 'mkdir', 'rmdir', 'readdir', 'fsstat'
			]
		catch e # OSX
			e.message.should.eql 'sigar_nfs_server_v2_get error: 20001 (This function has not been implemented on this platform)'

	it 'should list arp', ->
		arpList = s.arpList()
		# console.log 'arpList', arpList
		for arp in arpList
			Object.keys(arp).should.eql [
				'ifname', 'type', 
				'hwaddr', 'address', 
				'flags'
			]


