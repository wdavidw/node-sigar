
should = require 'should'
sigar = require '..'

describe 'process', ->
	
	s = sigar()

	it 'should list all of them', ->
		procList = s.procList()
		# console.log 'procList', procList
		procList.should.include process.pid

	it 'should return statistics info', ->
		procStat = s.procStat()
		# console.log 'procStat', procStat
		Object.keys(procStat).should.eql [
			'total', 'sleeping', 'running', 'zombie', 'stopped', 'idle', 'threads'
		]

	it 'should return memory info', ->
		procMem = s.procMem process.pid
		# console.log 'procMem', procMem
		Object.keys(procMem).should.eql [
			'size', 'resident', 'share', 'minor_faults', 'major_faults', 'page_faults'
		]

	it 'should return credential info', ->
		procCred = s.procCred process.pid
		# console.log 'procCred', procCred
		Object.keys(procCred).should.eql [
			'uid', 'gid', 'euid', 'egid'
		]
		# procList = s.procList()
		# for pid in procList
		# 	console.log pid, s.procCred pid

	it 'should return time info', ->
		procTime = s.procTime process.pid
		# console.log 'procTime', procTime
		Object.keys(procTime).should.eql [
			'start_time', 'user', 'sys', 'total'
		]
		# procList = s.procList()
		# for pid in procList
		# 	console.log pid, s.procTime pid

	it 'should return cpu info', ->
		procCpu = s.procCpu process.pid
		# console.log 'procCpu', procCpu
		Object.keys(procCpu).should.eql [
			'start_time', 'user', 'sys', 'total', 'last_time', 'percent'
		]
		# procList = s.procList()
		# for pid in procList
		# 	console.log pid, s.procCpu pid

	it 'should return state', ->
		procState = s.procState process.pid
		# console.log 'procState', procState
		Object.keys(procState).should.eql [
			'name', 'state', 'ppid', 'tty', 'priority', 'nice', 'processor', 'threads'
		]
		# procList = s.procList()
		# for pid in procList
		# 	state = s.procState pid
			# console.log state.name, String.fromCharCode(state.state)
			# console.log state.name, state.state

	it 'should return arguments', ->
		procArgs = s.procArgs process.pid
		# console.log 'procArgs', procArgs
		procArgs.length.should.be.above 0
		procArgs[0].should.eql 'node'
		# procList = s.procList()
		# for pid in procList
		# 	console.log pid, s.procArgs pid

	it 'should return environment', ->
		procEnv = s.procEnv process.pid
		# console.log 'procEnv', procEnv
		procEnv['HOME'].should.eql process.env['HOME']

	it 'should return file descriptor', ->
		try
			procFd = s.procFd process.pid
			# console.log 'procFd', procFd
			Object.keys(procFd).should.eql [
				'total'
			]
		catch e # OSX
			e.message.should.eql 'sigar_proc_fd_get error: 20001 (This function has not been implemented on this platform)'

	it 'should return exec information', ->
		procExe = s.procExe process.pid
		# console.log 'procExe', procExe
		Object.keys(procExe).should.eql [
			'name', 'cwd', 'root'
		]
		# procList = s.procList()
		# for pid in procList
		# 	console.log pid, s.procExe pid

	it 'should return module information', ->
		procModules = s.procModules process.pid
		# console.log 'procModules', procModules
		procModules.should.be.instanceof Array 
		procModules.length.should.be.above 0
		# procList = s.procList()
		# for pid in procList
		# 	try
		# 		console.log pid, s.procModules pid
		# 	catch e
		# 		console.log pid, e.message

	it 'should return port information', ->
		# TODO: start an http server and compare the return pid with the current pid
		# procPort = s.procPort -1, 4000
		# console.log 'procPort', procPort

	it 'should return cpu usage per thread', ->
		# Note, doesn't seem to be implemented by thread on osx, return rusage of the current thread
		# threadCpu = s.threadCpu process.pid
		# console.log 'threadCpu', threadCpu
		# procList = s.procList()
		# for pid in procList
		# 	try
		# 		console.log pid, s.threadCpu pid+23
		# 	catch e
		# 		console.log pid, e.message

