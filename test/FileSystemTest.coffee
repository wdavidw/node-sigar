
assert = require 'assert'
sigar = require '..'

module.exports = 
	'Test fileSystem constants': (next) ->
		# console.log 'FSTYPE_UNKNOWN', sigar.FSTYPE_UNKNOWN
		# console.log 'FSTYPE_NONE', sigar.FSTYPE_NONE
		# console.log 'FSTYPE_LOCAL_DISK', sigar.FSTYPE_LOCAL_DISK
		# console.log 'FSTYPE_NETWORK', sigar.FSTYPE_NETWORK
		# console.log 'FSTYPE_RAM_DISK', sigar.FSTYPE_RAM_DISK
		# console.log 'FSTYPE_CDROM', sigar.FSTYPE_CDROM
		# console.log 'FSTYPE_SWAP', sigar.FSTYPE_SWAP
		# console.log 'FSTYPE_MAX', sigar.FSTYPE_MAX
		assert.eql sigar.FSTYPE_UNKNOWN, 0
		assert.eql sigar.FSTYPE_NONE, 1
		assert.eql sigar.FSTYPE_LOCAL_DISK, 2
		assert.eql sigar.FSTYPE_NETWORK, 3
		assert.eql sigar.FSTYPE_RAM_DISK, 4
		assert.eql sigar.FSTYPE_CDROM, 5
		assert.eql sigar.FSTYPE_SWAP, 6
		assert.eql sigar.FSTYPE_MAX, 7
		next()
	'Test fileSystemList': (next) ->
		# Note, doesn't seem to be implemented by thread on osx, return rusage of the current thread
		s = sigar()
		fileSystemList = s.fileSystemList()
		# console.log fileSystemList
		for fs in fileSystemList
			if fs.dir_name is '/'
				foundRoot = true
				assert.eql [
					'dir_name', 'dev_name', 'type_name', 'sys_type_name', 'options', 'type', 'flags'
				], Object.keys(fs)
		assert.ok foundRoot
		next()
	'Test fileSystemUsage': (next) ->
		# Note, doesn't seem to be implemented by thread on osx, return rusage of the current thread
		s = sigar()
		fileSystemUsage = s.fileSystemUsage '/'
		# console.log 'fileSystemUsage', fileSystemUsage
		assert.eql [
			'disk', 'use_percent', 'total', 'free', 'used', 'avail', 'files', 'free_files'
		], Object.keys(fileSystemUsage)
		assert.eql [
			'reads', 'writes', 'write_bytes', 'read_bytes', 'rtime', 'wtime', 'qtime', 'time', 'snaptime', 'service_time', 'queue'
		], Object.keys(fileSystemUsage.disk)
		next()
	'Test fileSystemPing': (next) ->
		# Note, text decoding is wrong
		s = sigar()
		fileSystemPing = s.fileSystemPing()
		# console.log fileSystemPing
		assert.eql [
			'dir_name', 'dev_name', 'type_name', 'sys_type_name', 'options', 'type', 'flags'
		], Object.keys(fileSystemPing)
		next()