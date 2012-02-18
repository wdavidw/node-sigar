
should = require 'should'
sigar = require '..'

describe 'file_system', ->

	s = sigar()

	it 'should return associated constant code', ->
		# console.log 'FSTYPE_UNKNOWN', sigar.FSTYPE_UNKNOWN
		# console.log 'FSTYPE_NONE', sigar.FSTYPE_NONE
		# console.log 'FSTYPE_LOCAL_DISK', sigar.FSTYPE_LOCAL_DISK
		# console.log 'FSTYPE_NETWORK', sigar.FSTYPE_NETWORK
		# console.log 'FSTYPE_RAM_DISK', sigar.FSTYPE_RAM_DISK
		# console.log 'FSTYPE_CDROM', sigar.FSTYPE_CDROM
		# console.log 'FSTYPE_SWAP', sigar.FSTYPE_SWAP
		# console.log 'FSTYPE_MAX', sigar.FSTYPE_MAX
		sigar.FSTYPE_UNKNOWN.should.eql 0
		sigar.FSTYPE_NONE.should.eql 1
		sigar.FSTYPE_LOCAL_DISK.should.eql 2
		sigar.FSTYPE_NETWORK.should.eql 3
		sigar.FSTYPE_RAM_DISK.should.eql 4
		sigar.FSTYPE_CDROM.should.eql 5
		sigar.FSTYPE_SWAP.should.eql 6
		sigar.FSTYPE_MAX.should.eql 7

	it 'should return information for each file system', ->
		# Note, doesn't seem to be implemented by thread on osx, return rusage of the current thread
		fileSystemList = s.fileSystemList()
		# console.log fileSystemList
		for fs in fileSystemList
			continue unless fs.dir_name is '/'
			foundRoot = true
			Object.keys(fs).should.eql [
				'dir_name', 'dev_name', 'type_name', 'sys_type_name', 'options', 'type', 'flags'
			], 
		foundRoot.should.be.ok

	it 'should print a file system usage', ->
		# Note, doesn't seem to be implemented by thread on osx, return rusage of the current thread
		fileSystemUsage = s.fileSystemUsage '/'
		# console.log 'fileSystemUsage', fileSystemUsage
		Object.keys(fileSystemUsage).should.eql [
			'disk', 'use_percent', 'total', 'free', 'used', 'avail', 'files', 'free_files'
		]
		Object.keys(fileSystemUsage.disk).should.eql [
			'reads', 'writes', 'write_bytes', 'read_bytes', 'rtime', 'wtime', 'qtime', 'time', 'snaptime', 'service_time', 'queue'
		]

	it 'should ping ping the file system', ->
		# Note, text decoding is wrong
		fileSystemPing = s.fileSystemPing()
		# console.log fileSystemPing
		Object.keys(fileSystemPing).should.eql [
			'dir_name', 'dev_name', 'type_name', 'sys_type_name', 'options', 'type', 'flags'
		]
