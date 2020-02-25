gooroom_example_sources = [
  'main.c',
  'gooroom-example-application.c',
  'gooroom-example-window.c',
]

gooroom_example_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('gtk+-3.0', version: '>= 3.22'),
  dependency('libgtop-2.0', version: '>=2.37.0'),
  dependency('glib-2.0',    version: '>=2.56.0')
]

gnome = import('gnome')

gooroom_example_sources += gnome.compile_resources('gooroom-example-resources',
  'gooroom-example.gresource.xml',
  c_name: 'gooroom_example'
)

executable('gooroom-example', gooroom_example_sources,
  dependencies: gooroom_example_deps,
  install: true,
)
