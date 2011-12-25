
assert = require 'assert'
sigar = require '..'

module.exports = 
	'Test mem': (next) ->
		s = sigar()
		mem = s.mem()
		#console.log 'mem', mem
		assert.eql [
			'ram', 'total', 'used', 'free', 'actual_used', 'actual_free', 'used_percent', 'free_percent'
		], Object.keys(mem)
		next()
	'Test swap': (next) ->
		s = sigar()
		swap = s.swap()
		#console.log 'swap', swap
		assert.eql [
			'total', 'used', 'free','page_in','page_out'
		], Object.keys(swap)
		next()
	'Test uptime': (next) ->
		s = sigar()
		uptime = s.uptime()
		# console.log 'uptime', uptime
		assert.ok uptime > 0
		next()
	'Test loadavg': (next) ->
		s = sigar()
		loadavg = s.loadavg()
		# console.log 'loadavg', loadavg
		assert.eql loadavg.length, 3
		assert.ok loadavg[0] > 0
		next()
	'Test resourceLimit': (next) ->
		s = sigar()
		resourceLimit = s.resourceLimit()
		# console.log 'resourceLimit', resourceLimit
		assert.eql [
			'cpu_cur', 'cpu_max', 
			'file_size_cur', 'file_size_max', 
			'pipe_size_cur', 'pipe_size_max', 
			'data_cur', 'data_max', 
			'stack_cur', 'stack_max', 
			'core_cur', 'core_max', 
			'memory_cur', 'memory_max', 
			'processes_cur', 'processes_max', 
			'open_files_cur', 'open_files_max', 
			'virtual_memory_cur', 'virtual_memory_max'
		], Object.keys(resourceLimit)
		next()












