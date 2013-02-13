Gem::Specification.new do |s|
  s.name        = 'Rdmtx'
  s.version     = '0.1.4'
  s.date        = '2013-02-13'
  s.summary     = "Ruby libdmtx wrapper"
  s.authors     = ["Srijan Choudhary"]
  s.email       = 'srijan4@gmail.com'

  s.files       = Dir.glob('ext/**/*.{c,h,rb}')
  s.extensions  = ['ext/rdmtx/extconf.rb']

  s.add_runtime_dependency "rmagick"
end
