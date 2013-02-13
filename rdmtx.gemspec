Gem::Specification.new do |s|
  s.name        = 'rdmtx'
  s.version     = '0.1.2'
  s.date        = '2013-02-13'
  s.summary     = "libdmtx wrapper for ruby"
  s.description = "libdmtx wrapper for ruby"
  s.authors     = ["Srijan Choudhary"]
  s.email       = 'srijan4@gmail.com'
  s.files       = ["Rdmtx.c"]
  s.extensions  = ['extconf.rb']
  s.add_runtime_dependency "rmagick"
end
