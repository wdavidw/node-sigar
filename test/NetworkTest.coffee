
assert = require 'assert'
sigar = require '..'

module.exports = 
	'Test netInfo': (next) ->
		s = sigar()
		netInfo = s.netInfo()
		# console.log 'netInfo', netInfo
		# assert.eql [
		# 	'default_gateway', 'default_gateway_interface', 'host_name', 'domain_name', 'primary_dns', 'secondary_dns'
		# ], Object.keys(netInfo)
		next()
	'Test netRouteList': (next) ->
		s = sigar()
		netRouteList = s.netRouteList()
		assert.ok netRouteList.length > 0
		for netRoute in netRouteList
			# console.log 'netRoute', netRoute
			assert.eql [
				'destination', 'gateway', 'mask', 'flags', 'refcnt', 'use', 'metric', 'mtu', 'window', 'irtt', 'ifname'
			], Object.keys(netRoute)
		next()
	'Test netInterfaceList': (next) ->
		s = sigar()
		netInterfaceList = s.netInterfaceList()
		assert.ok netInterfaceList.length > 0
		for netInterface in netInterfaceList
			assert.ok /^[\w]+$/.test(netInterface) # Note: might be too restricive
		next()
	'Test netInterfaceConfig': (next) ->
		s = sigar()
		netInterfaceList = s.netInterfaceList()
		for netInterface in netInterfaceList
			netInterfaceConfig = s.netInterfaceConfig netInterface
			# console.log 'netInterfaceConfig', netInterfaceConfig
		next()
	'Test netInterfaceConfigPrimary': (next) ->
		s = sigar()
		netInterfaceConfigPrimary = s.netInterfaceConfigPrimary()
		# console.log 'netInterfaceConfigPrimary', netInterfaceConfigPrimary
		next()
	'Test netInterfaceStat': (next) ->
		s = sigar()
		netInterfaceList = s.netInterfaceList()
		for netInterface in netInterfaceList
			netInterfaceStat = s.netInterfaceStat netInterface
			# console.log 'netInterfaceStat', netInterface, netInterfaceStat
			assert.eql [
				'rx_packets', 'rx_bytes', 'rx_errors', 'rx_dropped', 
				'rx_overruns', 'rx_frame', 'tx_packets', 'tx_bytes', 
				'tx_errors', 'tx_dropped', 'tx_overruns', 'tx_collisions', 
				'tx_carrier', 'speed'
			], Object.keys(netInterfaceStat)
		next()
	'Test netConnectionList': (next) ->
		s = sigar()
		netConnectionList = s.netConnectionList -1
		assert.ok netConnectionList.length > 0
		for netConnection in netConnectionList
			# console.log 'netConnection', netConnection
			assert.eql [
				'local_port', 'local_address', 'remote_port', 'remote_address', 'uid', 'inode', 'type', 'state', 'send_queue', 'receive_queue'
			], Object.keys(netConnection)
		next()
	'Test netListenAddress': (next) ->
		s = sigar()
		netListenAddress = s.netListenAddress 22
		# console.log 'netListenAddress', netListenAddress
		assert.eql netListenAddress, '0.0.0.0'
		next()
	'Test netStat': (next) ->
		s = sigar()
		netStat = s.netStat -1
		# Note, tcp_states are:
		# SIGAR_TCP_ESTABLISHED = 1, SIGAR_TCP_SYN_SENT, SIGAR_TCP_SYN_RECV, SIGAR_TCP_FIN_WAIT1, 
		# SIGAR_TCP_FIN_WAIT2, SIGAR_TCP_TIME_WAIT, SIGAR_TCP_CLOSE, SIGAR_TCP_CLOSE_WAIT, SIGAR_TCP_LAST_ACK, 
		# SIGAR_TCP_LISTEN, SIGAR_TCP_CLOSING, SIGAR_TCP_IDLE, SIGAR_TCP_BOUND, SIGAR_TCP_UNKNOWN
		# console.log 'netStat', netStat
		assert.eql [
			'tcp_states', 'tcp_inbound_total', 'tcp_outbound_total', 'all_inbound_total', 'all_outbound_total'
		], Object.keys(netStat)
		assert.eql netStat.tcp_states.length, 14
		next()
	'Test netStatPort': (next) ->
		s = sigar()
		netStatPort = s.netStatPort -1, '127.0.0.1', 22
		# console.log 'netStatPort', netStatPort
		assert.eql [
			'tcp_states', 'tcp_inbound_total', 'tcp_outbound_total', 'all_inbound_total', 'all_outbound_total'
		], Object.keys(netStatPort)
		assert.eql netStatPort.tcp_states.length, 14
		next()
	'Test tcp': (next) ->
		s = sigar()
		tcp = s.tcp()
		# console.log 'tcp', tcp
		assert.eql [
			'active_opens', 'passive_opens', 'attempt_fails', 'estab_resets', 'curr_estab', 'in_segs', 'out_segs', 'retrans_segs', 'in_errs', 'out_rsts'
		], Object.keys(tcp)
		next()
	'Test nfsClientV2': (next) ->
		s = sigar()
		try
			cpu = s.nfsClientV2()
			console.log 'nfsClientV2', nfsClientV2
			assert.eql [
				'null', 'getattr', 'setattr', 'root', 'lookup', 'readlink', 
				'read', 'writecache', 'write', 'create', 'remove', 'rename', 'link', 'symlink', 'mkdir', 'rmdir', 'readdir', 'fsstat'
			], Object.keys(nfsClientV2)
		catch e # OSX
			assert.eql e.message, 'sigar_nfs_client_v2_get error: 20001 (This function has not been implemented on this platform)'
		next()
	'Test nfsServerV2': (next) ->
		s = sigar()
		try
			cpu = s.nfsServerV2()
			console.log 'nfsServerV2', nfsServerV2
			assert.eql [
				'null', 'getattr', 'setattr', 'root', 'lookup', 'readlink', 
				'read', 'writecache', 'write', 'create', 'remove', 'rename', 'link', 'symlink', 'mkdir', 'rmdir', 'readdir', 'fsstat'
			], Object.keys(nfsServerV2)
		catch e # OSX
			assert.eql e.message, 'sigar_nfs_server_v2_get error: 20001 (This function has not been implemented on this platform)'
		next()
	'Test arpList': (next) ->
		s = sigar()
		arpList = s.arpList()
		# console.log 'arpList', arpList
		for arp in arpList
			assert.eql [
				'ifname', 'type', 
				'hwaddr', 'address', 
				'flags'
			], Object.keys(arp)
		next()

