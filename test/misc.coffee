
assert = require 'assert'
sigar = require '..'

describe 'misc', ->

    s = sigar()

    it 'should output formated size', ->
        formatSize = s.formatSize 8492389262
        # console.log 'formatSize', formatSize
        formatSize.should.eql '7.9G'
