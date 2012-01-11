
assert = require 'assert'
sigar = require '..'

module.exports = 
	'Test procList': (next) ->
		s = sigar()
		procList = s.procList()
		# console.log 'procList', procList
		assert.ok procList.indexOf process.pid isnt -1
		next()
	'Test procStat': (next) ->
		s = sigar()
		procStat = s.procStat()
		# console.log 'procStat', procStat
		assert.eql [
			'total', 'sleeping', 'running', 'zombie', 'stopped', 'idle', 'threads'
		], Object.keys(procStat)
		next()
	'Test procMem': (next) ->
		s = sigar()
		procMem = s.procMem process.pid
		# console.log 'procMem', procMem
		assert.eql [
			'size', 'resident', 'share', 'minor_faults', 'major_faults', 'page_faults'
		], Object.keys(procMem)
		next()
	'Test procCred': (next) ->
		s = sigar()
		procCred = s.procCred process.pid
		# console.log 'procCred', procCred
		assert.eql [
			'uid', 'gid', 'euid', 'egid'
		], Object.keys(procCred)
		# procList = s.procList()
		# for pid in procList
		# 	console.log pid, s.procCred pid
		next()
	'Test procTime': (next) ->
		s = sigar()
		procTime = s.procTime process.pid
		# console.log 'procTime', procTime
		assert.eql [
			'start_time', 'user', 'sys', 'total'
		], Object.keys(procTime)
		# procList = s.procList()
		# for pid in procList
		# 	console.log pid, s.procTime pid
		next()
	'Test procCpu': (next) ->
		s = sigar()
		procCpu = s.procCpu process.pid
		# console.log 'procCpu', procCpu
		assert.eql [
			'start_time', 'user', 'sys', 'total', 'last_time', 'percent'
		], Object.keys(procCpu)
		# procList = s.procList()
		# for pid in procList
		# 	console.log pid, s.procCpu pid
		next()
	'Test procState': (next) ->
		s = sigar()
		procState = s.procState process.pid
		# console.log 'procState', procState
		assert.eql [
			'name', 'state', 'ppid', 'tty', 'priority', 'nice', 'processor', 'threads'
		], Object.keys(procState)
		# procList = s.procList()
		# for pid in procList
		# 	state = s.procState pid
			# console.log state.name, String.fromCharCode(state.state)
			# console.log state.name, state.state
		next()
	'Test procArgs': (next) ->
		s = sigar()
		procArgs = s.procArgs process.pid
		# console.log 'procArgs', procArgs
		assert.ok procArgs.length > 0
		assert.eql procArgs[0], 'node'
		# procList = s.procList()
		# for pid in procList
		# 	console.log pid, s.procArgs pid
		next()
	'Test procEnv': (next) ->
		s = sigar()
		procEnv = s.procEnv process.pid
		# console.log 'procEnv', procEnv
		assert.eql procEnv['HOME'], process.env['HOME']
		next()
	'Test procFd': (next) ->
		s = sigar()
		try
			procFd = s.procFd process.pid
			console.log 'procFd', procFd
			assert.eql [
				'total'
			], Object.keys(procFd)
		catch e # OSX
			assert.eql e.message, 'sigar_proc_fd_get error: 20001 (This function has not been implemented on this platform)'
		next()
	'Test procExe': (next) ->
		s = sigar()
		procExe = s.procExe process.pid
		# console.log 'procExe', procExe
		assert.eql [
			'name', 'cwd', 'root'
		], Object.keys(procExe)
		# procList = s.procList()
		# for pid in procList
		# 	console.log pid, s.procExe pid
		next()
	'Test procModules': (next) ->
		s = sigar()
		procModules = s.procModules process.pid
		# console.log 'procModules', procModules
		assert.ok Array.isArray procModules
		assert.ok procModules.length > 0
		# procList = s.procList()
		# for pid in procList
		# 	try
		# 		console.log pid, s.procModules pid
		# 	catch e
		# 		console.log pid, e.message
		next()
	'Test procPort': (next) ->
		s = sigar()
		# TODO: start an http server and compare the return pid with the current pid
		# procPort = s.procPort -1, 4000
		# console.log 'procPort', procPort
		next()
	'Test threadCpu': (next) ->
		# Note, doesn't seem to be implemented by thread on osx, return rusage of the current thread
		s = sigar()
		# threadCpu = s.threadCpu process.pid
		# console.log 'threadCpu', threadCpu
		# procList = s.procList()
		# for pid in procList
		# 	try
		# 		console.log pid, s.threadCpu pid+23
		# 	catch e
		# 		console.log pid, e.message
		next()
