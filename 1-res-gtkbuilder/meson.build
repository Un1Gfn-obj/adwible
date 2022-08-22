# https://mesonbuild.com/Gnome-module.html#gnomecompile_resources
j72qkdresources = gnome.compile_resources('ck3fm7',
  'ck3fm7.gresource.xml',
  c_name: 'j72qkd',
  gresource_bundle: false,
)

executable('main',
  'main.c',
  j72qkdresources,
  dependencies: [
    dependency('gtk4'),
    dependency('libadwaita-1'),
  ]
)
