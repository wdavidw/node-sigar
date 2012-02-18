
should = require 'should'
sigar = require '..'

describe 'cpu', ->

	s = sigar()

	it 'should return overall cpu usage', ->
		cpu = s.cpu()
		# console.log 'cpu', cpu
		Object.keys(cpu).should.eql [
			'user', 'sys', 'nice', 'idle', 'wait', 'irq', 'soft_irq', 'stolen', 'total'
		]
	
	it 'should list cpu with current usage', ->
		cpuList = s.cpuList()
		# console.log 'cpuList', cpuList
		cpuList.length.should.be.above 0
		Object.keys(cpuList[0]).should.eql [
			'user', 'sys', 'nice', 'idle', 'wait', 'irq', 'soft_irq', 'stolen', 'total'
		]
	
	it 'should list cpu with information': ->
		cpuInfoList = s.cpuInfoList()
		# console.log 'cpuInfoList', cpuInfoList
		cpuList.length.should.be.above 0
		Object.keys(cpuInfoList[0]).should.eql [
			'vendor', 'model', 'mhz', 'mhz_max', 'mhz_min', 'cache_size', 'total_sockets', 'total_cores', 'cores_per_socket'
		]
