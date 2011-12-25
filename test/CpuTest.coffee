
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
		cpus = s.cpuList()
		# console.log 'cpus', cpus
		assert.ok cpus.length > 0
		assert.eql [
			'user', 'sys', 'nice', 'idle', 'wait', 'irq', 'soft_irq', 'stolen', 'total'
		], Object.keys(cpus[0])
		next()
	'Test cpuInfo': (next) ->
		s = sigar()
		cpus = s.cpuInfo()
		# console.log 'cpus', cpus
		assert.ok cpus.length > 0
		assert.eql [
			'vendor', 'model', 'mhz', 'mhz_max', 'mhz_min', 'cache_size', 'total_sockets', 'total_cores', 'cores_per_socket'
		], Object.keys(cpus[0])
		next()
