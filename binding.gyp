{
  'targets': [
    {
      'target_name': 'sigar',
      'sources': [ 'src/node_sigar.cc' ],
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      "libraries": [
        "<(module_root_dir)/build/sigar/src/.libs/libsigar.a"
      ],
      "include_dirs": [
        'deps/sigar/include'
      ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          }
        }]
      ]
    }
  ]
}
