subin_sources = [
  'main.c',
  'subin-application.c',
  'subin-window.c',
]

subin_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('gtk+-3.0', version: '>= 3.22'),
  dependency('libgtop-2.0', version: '>=2.37.0'),
  dependency('glib-2.0',    version: '>=2.56.0')
]

gnome = import('gnome')

subin_sources += gnome.compile_resources('subin-resources',
  'subin.gresource.xml',
  c_name: 'subin'
)

executable('subin', subin_sources,
  dependencies: subin_deps,
  install: true,
)
