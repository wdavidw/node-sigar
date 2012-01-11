
assert = require 'assert'
sigar = require '..'

module.exports = 
	'Test testNetAdress': (next) ->
		s = sigar()
		testNetAdress = s.testNetAdress '127.0.0.1'
		# console.log testNetAdress
		# assert.eql testNetAdress, '127.0.0.1'
		next()
	'Test rpcPing': (next) ->
		s = sigar()
		# rpcPing = s.rpcPing '127.0.0.1', 0x10, 2
		# console.log 'rpcPing', rpcPing
		next()
	# 'Test password': (next) ->
	# 	s = sigar()
	# 	password = s.password 'Password please'
	# 	console.log 'password', password
	# 	next()