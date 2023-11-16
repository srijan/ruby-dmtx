Gem::Specification.new do |s|
  s.name        = 'Rdmtx'
  s.version     = '0.4.0'
  s.date        = '2020-10-01'
  s.summary     = 'Ruby libdmtx wrapper'

  s.description = 'This is a ruby wrapper for libdmtx, which is a open source software for reading and writing Data Matrix barcodes.'
  s.authors     = ['Srijan Choudhary']
  s.email       = 'srijan4@gmail.com'
  s.homepage    = 'https://github.com/srijan/ruby-dmtx'
  s.license     = 'LGPL-2.1'

  s.files       = Dir.glob('ext/**/*.{c,h,rb}')
  s.extensions  = ['ext/rdmtx/extconf.rb']

  s.add_runtime_dependency 'rmagick', '~> 5'
  s.requirements << 'libdmtx'
end
