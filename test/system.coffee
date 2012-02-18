
should = require 'should'
os = require 'os'
sigar = require '..'

describe 'system', ->

	s = sigar()

	it 'should return memory', ->
		mem = s.mem()
		# console.log 'mem', mem
		Object.keys(mem).should.eql [
			'ram', 'total', 'used', 'free', 'actual_used', 'actual_free', 'used_percent', 'free_percent'
		]

	it 'should return swap', ->
		swap = s.swap()
		# console.log 'swap', swap
		Object.keys(swap).should.eql [
			'total', 'used', 'free','page_in','page_out'
		]

	it 'should return uptime', ->
		uptime = s.uptime()
		# console.log 'uptime', uptime
		uptime.should.be.above 0

	it 'should return loadavg', ->
		loadavg = s.loadavg()
		# console.log 'loadavg', loadavg
		loadavg.length.should.eql 3
		loadavg[0].should.be.above 0

	it 'should resource limit', ->
		resourceLimit = s.resourceLimit()
		# console.log 'resourceLimit', resourceLimit
		Object.keys(resourceLimit).should.eql [
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
		]

	it 'should list who', ->
		whoList = s.whoList()
		# console.log 'whoList', whoList
		for who in whoList
			Object.keys(who).should.eql [
				'user', 'device', 
				'host', 'time'
			]

	it 'should return version', ->
		version = s.version()
		# console.log 'version', version
		Object.keys(version).should.eql [
			'build_date', 'scm_revision', 
			'version', 'archname', 
			'archlib', 'binname', 'description', 
			'major', 'minor', 
			'maint', 'build'
		]

	it 'should return system info', ->
		sysInfo = s.sysInfo()
		# console.log 'sysInfo', sysInfo
		Object.keys(sysInfo).should.eql [
			'name', 'version', 
			'arch', 'machine', 
			'description', 'patch_level', 
			'vendor', 'vendor_version', 
			'vendor_name', 'vendor_code_name'
		]

	it 'should return fqdn', ->
		fqdn = s.fqdn()
		# console.log 'fqdn', fqdn
		( os.hostname() or /^\d+\.\d+\.\d+\.\d+$/.test fqdn ).should.be.ok






