
should = require 'should'
sigar = require '..'

describe 'todo', ->

	s = sigar()

	it 'should return net adress', ->
		testNetAdress = s.testNetAdress '127.0.0.1'
		# console.log testNetAdress
		# assert.eql testNetAdress, '127.0.0.1'

	it 'should ping rpc', ->
		# rpcPing = s.rpcPing '127.0.0.1', 0x10, 2
		# console.log 'rpcPing', rpcPing

	# it 'Test password', ->
	# 	s = sigar()
	# 	password = s.password 'Password please'
	# 	console.log 'password', password