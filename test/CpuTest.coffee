
assert = require 'assert'
sigar = require '..'

module.exports = 
	'Test cpu': (next) ->
		s = sigar()
		cpu = s.cpu()
		# console.log 'cpu', cpu
		assert.eql [
			'user', 'sys', 'nice', 'idle', 'wait', 'irq', 'soft_irq', 'stolen', 'total'
		], Object.keys(cpu)
		next()
	'Test cpuList': (next) ->
		s = sigar()
		cpuList = s.cpuList()
		# console.log 'cpuList', cpuList
		assert.ok cpuList.length > 0
		assert.eql [
			'user', 'sys', 'nice', 'idle', 'wait', 'irq', 'soft_irq', 'stolen', 'total'
		], Object.keys(cpuList[0])
		next()
	'Test cpuInfoList': (next) ->
		s = sigar()
		cpuInfoList = s.cpuInfoList()
		# console.log 'cpuInfoList', cpuInfoList
		assert.ok cpuInfoList.length > 0
		assert.eql [
			'vendor', 'model', 'mhz', 'mhz_max', 'mhz_min', 'cache_size', 'total_sockets', 'total_cores', 'cores_per_socket'
		], Object.keys(cpuInfoList[0])
		next()
