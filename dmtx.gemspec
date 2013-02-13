Gem::Specification.new do |s|
  s.name        = 'dmtx'
  s.version     = '0.1.3'
  s.date        = '2013-02-13'
  s.summary     = "Libdmtx wrapper gem for ruby"
  s.authors     = ["Srijan Choudhary"]
  s.email       = 'srijan4@gmail.com'
  s.files       = ["Rdmtx.c"]
  s.extensions  = ['extconf.rb']
  s.add_runtime_dependency "rmagick"
end
