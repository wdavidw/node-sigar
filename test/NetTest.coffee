
assert = require 'assert'
sigar = require '..'

module.exports = 
	'Test netInfo': (next) ->
		s = sigar()
		netInfo = s.netInfo()
		# console.log netInfo
		assert.eql [
			'default_gateway', 'default_gateway_interface', 'host_name', 'domain_name', 'primary_dns', 'secondary_dns'
		], Object.keys(netInfo)
		next()
	'Test netRouteList': (next) ->
		s = sigar()
		netRouteList = s.netRouteList()
		assert.ok netRouteList.length > 0
		for netRoute in netRouteList
			# console.log netRoute
			assert.eql [
				'destination', 'gateway', 'mask', 'flags', 'refcnt', 'use', 'metric', 'mtu', 'window', 'irtt', 'ifname'
			], Object.keys(netRoute)
			# assert.eql [
			# 	'family', 'addr_in', 'addr_in6', 'addr_mac'
			# ], Object.keys(netRoute.destination)
			# assert.eql [
			# 	'family', 'addr_in', 'addr_in6', 'addr_mac'
			# ], Object.keys(netRoute.gateway)
			# assert.eql [
			# 	'family', 'addr_in', 'addr_in6', 'addr_mac'
			# ], Object.keys(netRoute.mask)
		next()



