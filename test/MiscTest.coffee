
assert = require 'assert'
sigar = require '..'

module.exports = 
	'Test formatSize': (next) ->
		s = sigar()
		formatSize = s.formatSize 8492389262
		# console.log 'formatSize', formatSize
		assert.eql formatSize, '7.9G'
		next()